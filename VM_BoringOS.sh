#!/bin/sh
# This script created by AQEMU

CURRDIR=`pwd`

/usr/bin/qemu-system-x86_64  -machine accel=kvm -m 16 -drive file="$CURRDIR/bootloader",index=0,if=floppy,format=raw  -boot once=a,menu=on -net nic -net user -rtc base=utc -name "BoringOS" -cpu host -s -S $*
