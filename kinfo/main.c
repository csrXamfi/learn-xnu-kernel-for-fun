//
//  main.c
//  ke
//
//  Created by mikhail on 15.11.2025.
//



#include "libkextrw.h"
#include <mach/mach.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "offsetsKrnl.h"
#include <stdlib.h>
// offsets for macbook m1 air 2020 macos 26.1 build 25B78
#define KAUTH_CRED_PROC_REF     kslide(0xFFFFFE0008D59B88)
#define KAUTH_CRED_UNREF        kslide(0xFFFFFE0008D5AE7C)
#define KAUTH_CRED_CREATE       kslide(0xFFFFFE0008D5A5B4)
#define KAUTH_CRED_GET          kslide(0xFFFFFE0008D57730)
#define KALLOC_EXTERNAL         kslide(0xFFFFFE00088283E0)
#define KFREE_EXTERNAL          kslide(0xFFFFFE0008828A24)
#define KERNPROC                kslide(0xFFFFFE0007DC5DF0)
#define KERNEL_TASK             kslide(0xFFFFFE0007DC4CE0)
#define TASK_SIZE               kslide(0xFFFFFE000C4B9BB0) // proc->task = proc + sizeof(proc)



#define off_proc_pid                (0x60)
#define off_proc_prev               (0x8)


#define koffsetof(type, field) off_##type##_##field


static uint64_t kernproc = 0;
static uint64_t off_proc_task = 0;
static uint64_t proc_find(pid_t pid)
{
    if (!kernproc) return 0;
    uint64_t curProc = kernproc;
    while (curProc > gKernelBase) {
        pid_t curPid = kread32(curProc + koffsetof(proc, pid));
        if (curPid == pid) break;
        curProc = kreadptr(curProc + koffsetof(proc, prev));
    }
    return curProc;
}

static uint64_t proc_task(uint64_t proc)
{
    if (!off_proc_task) off_proc_task = kread64(TASK_SIZE);
    return proc + koffsetof(proc, task);
}

void proc_info(void) {
    uint64_t self_proc = proc_find(getpid());
    uint64_t self_task = proc_task(self_proc);
    printf("===Proc info===\n");
    printf("Our proc: 0x%llX\n", self_proc);
    printf("Our task: 0x%llX\n", self_task);
}

void tfp0(void) {
    kernproc = kreadptr(KERNPROC);
    uint64_t kernel_task = proc_task(kernproc);
    printf("===tfp0 primitive===\n");
    printf("kernproc: 0x%llX\n", kernproc);
    printf("kernel_task: 0x%llX\n", kernel_task);
}

void print_cred_info(uint64_t cred_ptr) {
    if (!cred_ptr) {
        printf("Invalid cred pointer\n");
        return;
    }
    printf("=== Process Credentials ===\n");
    printf("Cred pointer: 0x%llx\n", cred_ptr);
    
    uint64_t cr_ref = kread64(cred_ptr + 0x10);
    printf("Reference count: %llu\n", cr_ref);
    
    uint32_t cr_uid = kread32(cred_ptr + 0x18);
    uint32_t cr_ruid = kread32(cred_ptr + 0x18 + 0x04);
    uint32_t cr_svuid = kread32(cred_ptr + 0x18 + 0x08);
    printf("Effective UID: %u\n", cr_uid);
    printf("Real UID: %u\n", cr_ruid);
    printf("Saved UID: %u\n", cr_svuid);
    
    uint16_t cr_ngroups = kread16(cred_ptr + 0x18 + 0x0C);
    printf("Number of groups: %u\n", cr_ngroups);
    
    uint32_t cr_rgid = kread32(cred_ptr + 0x18 + 0x50);
    uint32_t cr_svgid = kread32(cred_ptr + 0x18 + 0x54);
    uint32_t cr_gmuid = kread32(cred_ptr + 0x18 + 0x58);
    uint32_t cr_flags = kread32(cred_ptr + 0x18 + 0x5C);
    
    printf("Real GID: %u\n", cr_rgid);
    printf("Saved GID: %u\n", cr_svgid);
    printf("Group UID: %u\n", cr_gmuid);
    printf("Flags: 0x%x\n", cr_flags);
    
    uint64_t cr_label = kread64(cred_ptr + 0x78);
    printf("MAC label: 0x%llx\n", cr_label);
}
void kernel_info(void) {
    uint64_t kernelTextBase = get_kernel_base();
    uint64_t kernelExecBase = kernelTextBase + macOS26_1;

    uint64_t kernelSlide = kernelTextBase - STATIC_KERNEL_BASE;
    uint64_t kernelExecSlide = kernelExecBase - STATIC_KERNEL_BASE;

    uint64_t KernelCacheBase = kernelTextBase - 0x8000;
    uint64_t KernelCacheSlide = KernelCacheBase - STATIC_KERNEL_BASE;
    printf("===Kernel kaslr bypass info===\n");
    printf("KernelCache slide: 0x%llx\n", KernelCacheSlide);
    printf("KernelCache base: 0x%llx\n", KernelCacheBase);
    printf("Kernel slide: 0x%llX\n", kernelSlide);
    printf("Kernel text base: 0x%llx\n", kernelTextBase);
    printf("Kernel text exec slide: 0x%llx\n", kernelExecSlide);
    printf("Kernel text exec base: 0x%llx\n", kernelExecBase);
}
int main(int argc, const char * argv[]) {
    if (kextrw_init() == -1) {
        printf("Failed to initialize KextRW\n");
        return 1;
    }
    kernel_info();
    uint64_t cred = kcall(KAUTH_CRED_GET, (uint64_t []){}, 0);
    print_cred_info(cred);
    tfp0();
    proc_info();
    kextrw_deinit();
    return 0;
}
