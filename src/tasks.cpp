#include "include/tasks.h"
#include "include/kernel.h"
#include "include/screen.h"
#include "include/util.h"

Lock lock;
int order = 0;

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
  // int i = 0;
  while (true) {
    // while (i++ < 50000) {
    /*carriage_return();
    printk("                         ");
    carriage_return();
    printk("%l", current_task->get().get_cpu_time());*/
    do_yield();
  }
  do_exit();
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

/*void thread1() {
  printk("t1 sleeping 10 second\n");
  delay(10000);
  do_yield();
  printk("t1 exiting\n");
  do_exit();
}

void thread2() {
  printk("t2 sleeping 1 second\n");
  delay(1000);
  do_yield();
  printk("t2 exiting\n");
  do_exit();
}

void thread3() {
  printk("t3 sleeping 4 second\n");
  delay(4000);
  do_yield();
  printk("t3 exiting\n");
  do_exit();
}*/

// void thread4() {
/*printk("t4 sleeping 2 second\n");
delay(2000);
do_yield();
printk("t4 exiting\n");
do_exit();*/

// printk("Thread4 ready!\n");
// int i = 0;
//  do_exit();
// while (true) {
/*printk("                  ");
carriage_return();
printk("T4 %d           ", i++);
carriage_return();*/
//  do_yield();
//}
//}

void thread5() {
  printk("Thread5 ready!\n");
  int i = 0;
  while (i++ < 20000) {
    do_yield();
    asm("ud2 " : :);
  }
  do_exit();
}