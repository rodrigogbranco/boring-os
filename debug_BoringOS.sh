#!/bin/sh

CURRDIR=`pwd`

gdb -ix "$CURRDIR/gdb_util/gdb_init_real_mode.txt" -ex "set tdesc filename $CURRDIR/gdb_util/target.xml" -ex 'target remote localhost:1234' -ex 'set confirm off' -ex "add-symbol-file $CURRDIR/build/bootloader.o 0x7c00" -ex 'set confirm on' -ex 'hbreak *_start' -ex 'c'