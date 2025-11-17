# learn-xnu-kernel-for-fun
This application displays basic information about the kernel. I use KextRW.
```bash
brew install ldid -Sent.plist
ldid -Sent.plist kinfo
./kinfo
For users output:
===Kernel kaslr bypass info===
KernelCache slide: 0x25664000
KernelCache base: 0xfffffe002c668000
Kernel slide: 0x2566C000
Kernel text base: 0xfffffe002c670000
Kernel text exec slide: 0x26e18000
Kernel text exec base: 0xfffffe002de1c000
=== Process Credentials ===
Cred pointer: 0xfffffe1773f13b80
Reference count: 3
Effective UID: 501
Real UID: 501
Saved UID: 501
Number of groups: 16
Real GID: 20
Saved GID: 20
Group UID: 501
Flags: 0x2
MAC label: 0x469d7e15dc174980
===tfp0 primitive===
kernproc: 0xFFFFFE28D7F563D0
kernel_task: 0xFFFFFE28D7F56B70
===Proc info===
Our proc: 0xFFFFFE28D60DE3D0
Our task: 0xFFFFFE28D60DEB70

For sudo output: 
===Kernel kaslr bypass info===
KernelCache slide: 0x25664000
KernelCache base: 0xfffffe002c668000
Kernel slide: 0x2566C000
Kernel text base: 0xfffffe002c670000
Kernel text exec slide: 0x26e18000
Kernel text exec base: 0xfffffe002de1c000
=== Process Credentials ===
Cred pointer: 0xfffffe1773f9e8f0
Reference count: 3
Effective UID: 0
Real UID: 0
Saved UID: 0
Number of groups: 16
Real GID: 0
Saved GID: 0
Group UID: 4294967195
Flags: 0x3
MAC label: 0xbf937e15dc1a6080
===tfp0 primitive===
kernproc: 0xFFFFFE28D7F563D0
kernel_task: 0xFFFFFE28D7F56B70
===Proc info===
Our proc: 0xFFFFFE28D6F70128
Our task: 0xFFFFFE28D6F708C8
```
