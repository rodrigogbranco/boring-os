#ifndef __KERNEL_H
#define __KERNEL_H

#include "queue.h"
#include "screen.h"
#include "util.h"
#include <atomic>
#include <cstdint>

#define NUM_TASKS 4

#define STACK_SIZE 0x1000
#define START_STACKS_ADDRESS 0x41000
enum TASK_STATUS { READY, RUNNING, EXITED, BLOCKED };

class PCB {
  uint32_t regs[9]; // EDI, ESI, EBP, original ESP, EBX, EDX, ECX, EAX, EFLAGS
  uint32_t pid;
  TASK_STATUS status;
  bool kernel_thread;
  int display_position = 0;
  char current_color;

public:
  void exit() { status = EXITED; };
  bool is_exited() { return status == EXITED; };
  void ready() { status = READY; };
  void run() { status = RUNNING; };
  void config(void (*)(), uint32_t, uint32_t, bool);
  void block() { status = BLOCKED; };
  [[nodiscard]] constexpr int get_display_position() {
    return display_position;
  };
  constexpr void set_display_position(int pos) { this->display_position = pos; }
  constexpr void set_display_color(Screen::Colors foreground,
                                   Screen::Colors background) {
    this->current_color = (foreground & 0x0f) | (background << 4);
  }
  [[nodiscard]] constexpr char get_display_color() {
    return this->current_color;
  };
  /*void print() {
    Util::printk("\n");
    Util::printk("\tPID: %d\n", pid);
    Util::printk("\tSTATUS: %d\n", status);
    Util::printk("\tEDI: %x ESI: %x  EBP: %x\n", regs[0], regs[1], regs[2]);
    Util::printk("\tESP: %x EBX: %x  EDX: %x\n", regs[3], regs[4], regs[5]);
    Util::printk("\tECX: %x EAX: %x  EFLAGS: %x\n", regs[6], regs[7], regs[8]);
  }*/
  PCB() {
    this->status = EXITED;
    this->kernel_thread = false;
    this->display_position = 0;
    this->set_display_color(Screen::GRAY, Screen::BLACK);
  };
};

class Lock;

class Scheduler {
  Datastructure::QueueNode<PCB> pcbs[NUM_TASKS];
  Datastructure::QueueNode<PCB> *ready_queue;
  uint32_t current_pid{0};
  uint32_t scheduler_count{0};
  uint32_t current_stack_address{START_STACKS_ADDRESS};

public:
  void resched(Datastructure::QueueNode<PCB> *);
  void inc_count() { this->scheduler_count++; };
  Datastructure::QueueNode<PCB> *get_ready_task();
  void add_task(void (*)(), bool);
  void block(Lock *);
  void unblock(Lock *);
};

// class PCB;

class Lock {
  std::atomic_bool mutex{false};
  Datastructure::QueueNode<PCB> *blocked{nullptr};

public:
  void lock_acquire();
  void lock_release();
  void block(Datastructure::QueueNode<PCB> *task) {
    if (blocked == nullptr) {
      blocked = task;
    } else {
      blocked->insert(task);
    }
  }
  Datastructure::QueueNode<PCB> *unblock() {
    Datastructure::QueueNode<PCB> *tmp = blocked;
    this->blocked = blocked->remove(tmp);
    return tmp;
  }
};

void do_exit(void);
void do_yield(void);
extern "C" void scheduler();

// void test_queue();

#endif