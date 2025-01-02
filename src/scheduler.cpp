#include "include/kernel.h"
#include "include/queue.h"
#include "include/util.h"
#include <cstdint>

extern "C" void scheduler_entry(void);
Datastructure::QueueNode<PCB> dummyPCB;
Datastructure::QueueNode<PCB> *current_task = &dummyPCB;

Scheduler sched;

void PCB::config(void (*entry_point)(), uint32_t stack, uint32_t pid,
                 bool kernel_thread) {
  this->pid = pid;
  this->regs[3] = stack - 8;
  this->regs[2] = *(uint32_t *)(stack - 8) = stack;
  *(uint32_t *)(stack - 4) = (uint32_t)entry_point;
  this->kernel_thread = kernel_thread;
  this->status = READY;
}

void Scheduler::resched(Datastructure::QueueNode<PCB> *task) {
  if (this->ready_queue != nullptr) {
    this->ready_queue->insert(task);
  } else {
    this->ready_queue = task;
    // Util::printk("ready queue %x\n", ready_queue);
  }
}

void do_yield() {
  current_task->get().ready();
  sched.resched(current_task);
  scheduler_entry();
}
void do_exit() {
  current_task->get().exit();
  scheduler_entry();
}

Datastructure::QueueNode<PCB> *Scheduler::get_ready_task() {
  if (this->ready_queue == nullptr) {
    Util::printk("!!!No more tasks to run!!!\n");
    while (true) {
      /*NO TASKS TO RUN - BUSY WAIT*/
    }
  } else {
    Datastructure::QueueNode<PCB> *tmp = ready_queue;
    this->ready_queue = ready_queue->remove(tmp);
    // Util::printk("ready queue %x\n", ready_queue);
    return tmp;
  }
}

extern "C" void scheduler() {
  sched.inc_count();
  current_task = sched.get_ready_task();
  current_task->get().run();
}

/*void test_queue() {
  Datastructure::QueueNode<int> array[] = {
      Datastructure::QueueNode<int>(0), Datastructure::QueueNode<int>(1),
      Datastructure::QueueNode<int>(2), Datastructure::QueueNode<int>(3),
      Datastructure::QueueNode<int>(4),
  };
  Datastructure::QueueNode<int> *head = &array[0];
  head->insert(&array[1]);
  head->insert(&array[2]);
  head->insert(&array[3]);
  head->insert(&array[4]);
  head->print();
  head = head->remove(&array[1]);
  head = head->remove(&array[3]);
  head->print();
  head->insert(&array[3]);
  head->insert(&array[1]);
  head->print();
  head = head->remove(&array[0]);
  head->print();
  head->insert(&array[0]);
  head->print();
}*/

void Scheduler::add_task(void (*entry_point)(), bool kernel_thread) {
  int i = 0;
  while (i < NUM_TASKS) {
    if (this->pcbs[i].get().is_exited()) {
      break;
    }
    i++;
  }

  if (i >= NUM_TASKS) {
    Util::panic("No more PCB available\n");
  }

  this->pcbs[i].get().config(entry_point, this->current_stack_address,
                             this->current_pid++, kernel_thread);
  this->current_stack_address += STACK_SIZE;

  if (this->ready_queue == nullptr) {
    this->ready_queue = &this->pcbs[i];
  } else {
    this->ready_queue->insert(&this->pcbs[i]);
  }
}