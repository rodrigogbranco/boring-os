#include "include/kernel.h"
#include "include/gdt.h"
#include "include/queue.h"
#include "include/screen.h"
#include "include/tasks.h"
#include "include/util.h"

extern "C" void _init(void);
extern "C" void _fini(void);

extern Scheduler sched;

extern "C" void _start() {
  _init();
  GDT::install_gdt();

  Screen::clear_screen();
  // test_queue();

  sched.add_task(&thread1, true);
  sched.add_task(&thread2, true);

  do_exit();

  while (true) {
    /* BUSY LOOP*/
  }

  /*If code has landed here, something very wrong happened...*/
  _fini();
}