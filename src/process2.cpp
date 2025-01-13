#include "include/syslib.h"
#include "include/util.h"

int main() {
  set_display_position(23, 0);
  printk("process2 ");
  int i = 0;
  while (i++ < 15000) {
    set_display_position(23, 9);
    printk("%d", i);
    yield();
  }
  printk("\nprocess2 finished!");
  return 0;
}