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
protected:
  uint32_t kregs[9]; // EDI, ESI, EBP, original ESP, EBX, EDX, ECX, EAX, EFLAGS
                     // (KERNEL MODE)
  uint32_t uregs[9]; // EDI, ESI, EBP, original ESP, EBX, EDX, ECX, EAX, EFLAGS
                     // (USER MODE)
  uint32_t pid;
  TASK_STATUS status{EXITED};
  bool kernel_thread{false};
  unsigned long long kernel_cpu_time_last_sched{0};
  unsigned long long user_cpu_time_last_sched{0};
  unsigned long long total_kernel_cpu_time{0};
  unsigned long long total_user_cpu_time{0};

public:
  void exit() { this->status = EXITED; };
  bool is_exited() { return this->status == EXITED; };
  bool is_kernel_thread() { return this->kernel_thread; };
  void ready() { this->status = READY; };
  void run() { this->status = RUNNING; };
  void config(int, void (*)(), uint32_t, bool);
  void block() { this->status = BLOCKED; };
  uint32_t get_pid() { return pid; };
  void set_kernel_cpu_time(unsigned long long time) {
    this->kernel_cpu_time_last_sched += time;
    this->total_kernel_cpu_time += time;
  };
  void set_user_cpu_time(unsigned long long time) {
    this->user_cpu_time_last_sched += time;
    this->total_user_cpu_time += time;
  };

  void clear_last_cpu_time() {
    this->kernel_cpu_time_last_sched = 0;
    this->user_cpu_time_last_sched = 0;
  }

  unsigned long long get_cpu_time() {
    return this->kernel_thread ? this->kernel_cpu_time_last_sched
                               : this->user_cpu_time_last_sched;
  }
} __attribute__((__packed__));

class Lock;

class Scheduler {
protected:
  QueueNode<PCB> pcbs[NUM_TASKS];
  QueueNode<PCB> *ready_queue;
  uint32_t current_pid{0};
  uint32_t scheduler_count{0};

public:
  virtual void sched(QueueNode<PCB> *);
  void inc_count() { this->scheduler_count++; };
  QueueNode<PCB> *get_ready_task();
  void add_task(void (*)(), bool);
  void block(Lock *);
  void unblock(Lock *);
};

class FairScheduler : public Scheduler {
public:
  void sched(QueueNode<PCB> *);
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
    queue_insert(this->blocked, task);
  }
  QueueNode<PCB> *unblock() {
    return queue_remove(this->blocked, this->blocked);
  }
};

void do_exit(void);
void do_yield(void);
extern "C" void scheduler();

// void test_queue();

#endif