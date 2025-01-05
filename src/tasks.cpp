#include "include/tasks.h"
#include "include/kernel.h"
#include "include/screen.h"
#include "include/util.h"

Lock lock;
int order = 0;

void thread1() {
  Screen::set_pos(0, 0);
  Util::printk("#1 started (%d) and is trying to acquire the lock\n", order++);
  lock.lock_acquire();
  Util::printk("#1 acquired the lock and yielded (%d).\n", order++);
  do_yield();
  Util::printk("#1 woke up (%d).\n", order++);
  lock.lock_release();
  Util::printk("#1 released the lock and exited (%d).\n", order++);
  do_exit();
}

void thread2() {
  int thread2_order = 0;
  Screen::set_pos(5, 0);
  Util::printk("#2 started (%d).\n", order++);
  while (true) {
    Util::printk("                      \n");
    Util::printk("                      \n");
    Screen::set_pos(6, 0);
    Util::printk("#2 yielded (t2 %d).\n", thread2_order++);
    do_yield();
    Util::printk("#2 woke up (t2 %d).\n", thread2_order++);
    Screen::set_pos(6, 0);
  }
}

void thread3() {
  Screen::set_pos(9, 0);
  Util::printk("#3 started and yielded (%d).\n", order++);
  do_yield();
  Util::printk("#3 woke up (%d) and is trying to obtain the lock.\n", order++);
  lock.lock_acquire();
  Util::printk("#3 acquired the lock (%d).\n", order++);
  lock.lock_release();
  Util::printk("#3 released the lock and exited (%d).\n", order++);
  do_exit();
}

void thread4() {
  Screen::set_pos(14, 0);
  Util::printk("#4 started (%d) and is trying to obtain the lock.\n", order++);
  lock.lock_acquire();
  Util::printk("#4 acquired the lock (%d).\n", order++);
  lock.lock_release();
  Util::printk("#4 released the lock and exited (%d).\n", order++);
  do_exit();
}