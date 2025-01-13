#include "include/tasks.h"
#include "include/kernel.h"
#include "include/screen.h"
#include "include/util.h"

Lock lock;
int order = 0;

extern unsigned long long time_to_do_yield;

void thread1() {
  printk("#1 started (%d) and is trying to acquire the lock\n", order++);
  lock.lock_acquire();
  printk("#1 acquired the lock and yielded (%d).\n", order++);
  do_yield();
  printk("#1 woke up (%d).\n", order++);
  lock.lock_release();
  printk("#1 released the lock and exited (%d).\n", order++);
  do_exit();
}

void thread2() {
  while (true) {
    do_yield();
  }
}

void thread3() {
  printk("#3 started and yielded (%d).\n", order++);
  do_yield();
  printk("#3 woke up (%d) and is trying to obtain the lock.\n", order++);
  lock.lock_acquire();
  printk("#3 acquired the lock (%d).\n", order++);
  lock.lock_release();
  printk("#3 released the lock and exited (%d).\n", order++);
  do_exit();
}

void thread4() {
  printk("#4 started (%d) and is trying to obtain the lock.\n", order++);
  lock.lock_acquire();
  printk("#4 acquired the lock (%d).\n", order++);
  lock.lock_release();
  printk("#4 released the lock and exited (%d).\n", order++);
  do_exit();
}