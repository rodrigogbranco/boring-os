#!/bin/sh -ex
# This script created by AQEMU

CURRDIR=`pwd`

/usr/bin/qemu-system-i386 -smp cpus=1 -accel tcg -m 16 -drive file="$CURRDIR/build/boringos.img",index=0,if=floppy,format=raw -boot once=a,menu=on -net nic -net user -name "BoringOS" -s -S $*
