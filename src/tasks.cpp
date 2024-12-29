#include "include/tasks.h"
#include "include/kernel.h"
#include "include/screen.h"
#include "include/util.h"

void thread1() {
  Screen::set_pos(0, 0);
  Screen::set_colors(Screen::BLACK, Screen::WHITE);
  Util::printk("Thread 1\n");
  unsigned int i = 0;
  while (i < 20000) {
    Util::printk("                        ");
    Screen::carriage_return();
    Util::printk("%d", i++);
    do_yield();
  }
  Util::printk("\nThread 1 exited!\n");
  do_exit();
}

void thread2() {
  Screen::set_pos(13, 0);
  Screen::set_colors(Screen::WHITE, Screen::BLACK);
  Util::printk("Thread 2\n");
  unsigned int i = 0;
  while (i < 10000) {
    Util::printk("                        ");
    Screen::carriage_return();
    Util::printk("%d", i++);
    do_yield();
  }
  Util::printk("\nThread 2 exited!\n");
  do_exit();
}