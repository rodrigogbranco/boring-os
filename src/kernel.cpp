#include "include/kernel.h"
#include "include/gdt.h"
#include "include/queue.h"
#include "include/screen.h"
#include "include/tasks.h"
#include "include/util.h"
#include <cstdint>

#define USER_ENTRY_POINT 0x00F00
#define PROCESS1 0x10000
#define PROCESS2 0x20000

extern Scheduler sched;

extern "C" void kernel_entry(void);

extern "C" void _start() {
  _init();
  install_gdt();
  *(uint32_t *)(USER_ENTRY_POINT) = (uint32_t)&kernel_entry;

  clear_screen();

  sched.add_task(&thread1, true);
  sched.add_task(&thread2, true);
  sched.add_task(&thread3, true);
  sched.add_task(&thread4, true);
  sched.add_task((void (*)())PROCESS1, false);
  sched.add_task((void (*)())PROCESS2, false);

  do_exit();

  while (true) {
    /* BUSY LOOP*/
  }

  /*If code has landed here, something very wrong happened...*/
  _fini();
}