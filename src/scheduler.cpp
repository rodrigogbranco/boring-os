#include "include/kernel.h"
#include "include/queue.h"
#include "include/util.h"
#include <cstdint>

extern "C" void scheduler_entry(void);

static Datastructure::QueueNode<PCB> pcbs[NUM_TASKS];
Datastructure::QueueNode<PCB> dummyPCB;
Datastructure::QueueNode<PCB> *current_task = &dummyPCB;
static Datastructure::QueueNode<PCB> *ready_queue;
static uint32_t current_pid = 0;
static uint32_t scheduler_count = 0;
static uint32_t current_stack_address = START_STACKS_ADDRESS;

void PCB::config(void (*entry_point)(), uint32_t stack, uint32_t pid,
                 bool kernel_thread) {
  this->pid = pid;
  this->regs[3] = stack - 8;
  this->regs[2] = *(uint32_t *)(stack - 8) = stack;
  *(uint32_t *)(stack - 4) = (uint32_t)entry_point;
  this->kernel_thread = kernel_thread;
  this->status = READY;
}

void do_yield() {
  current_task->get().ready();
  if (ready_queue != nullptr) {
    ready_queue->insert(current_task);
  } else {
    ready_queue = current_task;
    // Util::printk("ready queue %x\n", ready_queue);
  }
  scheduler_entry();
}
void do_exit() {
  current_task->get().exit();
  scheduler_entry();
}

extern "C" void scheduler() {
  scheduler_count++;
  if (ready_queue == nullptr) {
    Util::printk("!!!No more tasks to run!!!\n");
    while (1) {
      /*NO TASKS TO RUN - BUSY WAIT*/
    }
  } else {
    current_task = ready_queue;
    ready_queue = ready_queue->remove(current_task);
    // Util::printk("ready queue %x\n", ready_queue);
    current_task->get().run();
  }
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

void add_task(void (*entry_point)(), bool kernel_thread) {
  int i = 0;
  while (i < NUM_TASKS) {
    if (pcbs[i].get().is_exited()) {
      break;
    }
    i++;
  }

  if (i >= NUM_TASKS) {
    Util::panic("No more PCB available\n");
  }

  pcbs[i].get().config(entry_point, current_stack_address, current_pid++,
                       kernel_thread);
  current_stack_address += STACK_SIZE;

  if (ready_queue == nullptr) {
    ready_queue = &pcbs[i];
  } else {
    ready_queue->insert(&pcbs[i]);
  }
}