#define SYSCALL_YIELD 0
#define SYSCALL_EXIT 1

#include "include/util.h"

void (**kernel_entry_point)(int) = (void (**)(int))0x00F00;

void yield() { (**kernel_entry_point)(SYSCALL_YIELD); }

void exit() { (**kernel_entry_point)(SYSCALL_EXIT); }