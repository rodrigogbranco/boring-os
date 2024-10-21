#!/bin/sh

gdb -ex 'target remote localhost:1234' -ex 'set confirm off' -ex 'add-symbol-file bootloader.o 0x7c00' -ex 'set confirm on' -ex 'hbreak *_start' -ex 'c'