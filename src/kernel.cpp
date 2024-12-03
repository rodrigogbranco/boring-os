#include "include/gdt.h"
#include "include/screen.h"

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void _start() {
  _init();
  install_gdt();

  printk((char *)" Hello Protected Mode! ", RED, YELLOW);

  while (true) {
    /* BUSY LOOP*/
  }

  /*If it reaches here, something very wrong happened...*/
  _fini();
}