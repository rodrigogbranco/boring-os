#include "include/syslib.h"
#include "include/util.h"

/*class Test {

public:
  int x;
  Test() : x{15} {};
};

Test y;

int main() {
  set_display_position(20, 0);
  Test t;
  printk("Hello World! t.x %d - ", t.x);
  printk("Hello World! y.x %d - ", y.x);
  t.x = t.x + 100 + y.x;
  printk("Hello World! t.x %d\n", t.x);
  printk("process1 is yielding\n");
  yield();
  printk("process1 woke up and is exiting...\n");
  return 0;
}*/

int main() {
  set_display_position(20, 0);
  printk("p1 sleeping 0.5 second\n");
  delay(500);
  yield();
  printk("p1 exiting\n");
  return 0;
}