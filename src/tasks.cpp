#include "include/tasks.h"
#include "include/kernel.h"
#include "include/screen.h"
#include "include/util.h"

Lock lock;
int order = 0;

extern unsigned long long time_to_do_yield;
extern QueueNode<PCB> *current_task;

/*void thread1() {
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
    carriage_return();
    printk("                         ");
    carriage_return();
    printk("%l", current_task->get().get_cpu_time());
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
}*/

void thread1() {
  volatile int x = 0;
  for (int i = 0; i < 10000; i++) {
    x += i * 2;
  }
  printk("t1 10000 x=%d\n", x);
  do_yield();
  printk("t1 exiting\n");
  do_exit();
}

void thread2() {
  volatile int x = 0;
  for (int i = 1000; i > 0; i--) {
    x += x * i;
  }
  printk("t2 1000 x=%d\n", x);
  do_yield();
  printk("t2 exiting\n");
  do_exit();
}

void thread3() {
  volatile int x = 0;
  for (int i = 0; i < 4000; i++) {
    x += x * i;
  }
  printk("t3 4000 x=%d\n", x);
  do_yield();
  printk("t3 exiting\n");
  do_exit();
}

void thread4() {
  volatile int x = 0;
  for (int i = 2000; i > 0; i--) {
    x += x * i;
  }
  printk("t4 2000 x=%d\n", x);
  do_yield();
  printk("t4 exiting\n");
  do_exit();
}