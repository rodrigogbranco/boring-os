#include "include/syslib.h"

void (**kernel_entry_point)(int) = (void (**)(int))0x00F00;

void yield() { (**kernel_entry_point)(SYSCALL_YIELD); }

void exit() { (**kernel_entry_point)(SYSCALL_EXIT); }