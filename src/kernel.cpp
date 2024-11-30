#include "include/gdt.h"
#include "include/screen.h"

extern "C" void _start() {
  install_gdt();

  printk((char*)" Hello Protected Mode! ", RED, YELLOW);

  while (true) {
    /* BUSY LOOP*/
  }
}