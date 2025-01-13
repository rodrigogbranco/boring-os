#ifndef __KERNEL_H
#define __KERNEL_H

#include "queue.h"
#include "screen.h"
#include "util.h"
#include <atomic>
#include <cstdint>

#define NUM_TASKS 8

#define STACK_SIZE 0x1000
#define START_STACKS_ADDRESS 0x41000
#define STACK_MAX 0x52000
enum TASK_STATUS { READY, RUNNING, EXITED, BLOCKED };

class PCB {
  uint32_t kregs[9]; // EDI, ESI, EBP, original ESP, EBX, EDX, ECX, EAX, EFLAGS
                     // (KERNEL MODE)
  uint32_t uregs[9]; // EDI, ESI, EBP, original ESP, EBX, EDX, ECX, EAX, EFLAGS
                     // (USER MODE)
  uint32_t pid;
  TASK_STATUS status;
  bool kernel_thread;
  // int display_position = 0;
  // char current_color;

public:
  void exit() { status = EXITED; };
  bool is_exited() { return status == EXITED; };
  void ready() { status = READY; };
  void run() { status = RUNNING; };
  void config(int, void (*)(), uint32_t, bool);
  void block() { status = BLOCKED; };
  uint32_t get_pid() { return pid; };
  PCB() {
    this->status = EXITED;
    this->kernel_thread = false;
  };
} __attribute__((__packed__));

class Lock;

class Scheduler {
  QueueNode<PCB> pcbs[NUM_TASKS];
  QueueNode<PCB> *ready_queue;
  uint32_t current_pid{0};
  uint32_t scheduler_count{0};

public:
  void resched(QueueNode<PCB> *);
  void inc_count() { this->scheduler_count++; };
  QueueNode<PCB> *get_ready_task();
  void add_task(void (*)(), bool);
  void block(Lock *);
  void unblock(Lock *);
};

// class PCB;

class Lock {
  std::atomic_bool mutex{false};
  QueueNode<PCB> *blocked{nullptr};

public:
  void lock_acquire();
  void lock_release();
  void block(QueueNode<PCB> *task) {
    // Util::printk("blocking: %x\n", task);
    if (blocked == nullptr) {
      blocked = task;
    } else {
      blocked->insert(task);
    }
  }
  QueueNode<PCB> *unblock() {
    QueueNode<PCB> *tmp = blocked;
    // Util::printk("unblocking: %x\n", tmp);
    this->blocked = blocked->remove(tmp);
    return tmp;
  }
};

void do_exit(void);
void do_yield(void);
extern "C" void scheduler();

// void test_queue();

#endif