#!/bin/sh -ex

CURRDIR=`pwd`

gdb -ix "$CURRDIR/gdb_util/gdb_init_real_mode.txt" -ex "set tdesc filename $CURRDIR/gdb_util/target.xml" -ex 'target remote localhost:1234' -ex 'hbreak *0x7c00' -ex 'set confirm off' -ex "add-symbol-file $CURRDIR/build/bootloader 0x80000" -ex "add-symbol-file $CURRDIR/build/kernel" -ex 'set confirm on' -ex 'c'