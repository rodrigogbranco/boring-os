#include "include/gdt.h"
#include "include/screen.h"

extern "C" void _start() {
  install_gdt();

  const char *kernelMsg = "Hello Protected Mode!";

  clear_screen(RED, YELLOW);

  printk((char *)kernelMsg, RED, YELLOW);

  while (true) {
    /* BUSY LOOP*/
  }
}