#include "include/kernel.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/pit.h"
#include "include/queue.h"
#include "include/screen.h"
#include "include/tasks.h"
#include "include/util.h"
#include <cstdint>

#define USER_ENTRY_POINT 0x00F00
#define PROCESS1 0x10000
#define PROCESS2 0x20000

#define DESIRED_TIMER_HZ 1000

extern Scheduler *sched;

extern "C" void kernel_entry(void);
void (**user_entry_point)(void) = (void (**)(void))USER_ENTRY_POINT;

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void _start() {
  _init();
  clear_screen();

  install_gdt();
  install_idt();

  install_pic(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
  install_pit(DESIRED_TIMER_HZ);
  *user_entry_point = &kernel_entry;

  sched->add_task(&thread1, true);
  sched->add_task(&thread2, true);
  sched->add_task(&thread3, true);
  sched->add_task(&thread4, true);
  sched->add_task(&thread5, true);
  sched->add_task((void (*)())PROCESS1, false);
  // sched->add_task((void (*)())PROCESS2, false);

  do_exit();

  while (true) {
    /* BUSY LOOP*/
  }

  /*If code has landed here, something very wrong happened...*/
  _fini();
}