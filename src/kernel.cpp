#include "include/screen.h"

extern "C" void _start() {
  const char *kernelMsg = "Hello Protected Mode!";

  clear_screen(WHITE, BLACK);

  printk((char *)kernelMsg, YELLOW, RED);

  while (true) {
    /* BUSY LOOP*/
  }
}