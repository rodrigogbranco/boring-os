#!/bin/sh
# This script created by AQEMU

CURRDIR=`pwd`

/usr/bin/qemu-system-x86_64  -machine accel=kvm -m 16 -fda "$CURRDIR/bootloader" -boot once=a,menu=on -net nic -net user -rtc base=utc -name "BoringOS" -cpu host -s -S $*
