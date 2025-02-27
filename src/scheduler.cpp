#include "include/common.h"
#include "include/kernel.h"
#include "include/queue.h"
#include "include/util.h"
#include <cstdint>

#define EFLAGS_RESERVED 0x2
#define EFLAGS_INTERRUPT_ENABLED (1 << 9)
#define EFLAGS_INTERRUPT_NOT_ENABLED (0 << 9)

extern "C" void scheduler_entry(void);
extern "C" uint32_t returning_kernel_entry;
QueueNode<PCB> dummyPCB;
QueueNode<PCB> *current_task = &dummyPCB;
void (*syscall_operations[])() = {&do_yield, &do_exit};
unsigned long long curr_time = 0;

// FairScheduler fairSched;
// Scheduler *sched = &fairSched;

Scheduler roundRobinSched;
Scheduler *sched = &roundRobinSched;

extern "C" void start_timer() {
  curr_time = get_timer();
  // printk("start diff pid %d %l\n", current_task->get().get_pid(), diff);
}
extern "C" void stop_kernel_timer() {
  unsigned long long tmp = get_timer();
  unsigned long long diff = tmp - curr_time;
  // printk("diff %l\n", diff);

  current_task->get().set_kernel_cpu_time(diff);

  curr_time = tmp;
  // printk("stop diff pid %d %l\n", current_task->get().get_pid(), diff);
}

extern "C" void stop_user_timer() {
  unsigned long long tmp = get_timer();
  unsigned long long diff = tmp - curr_time;

  current_task->get().set_user_cpu_time(diff);

  curr_time = tmp;
  // printk("stop diff pid %d %l\n", current_task->get().get_pid(), diff);
}

void PCB::config(int pcb_index, void (*entry_point)(), uint32_t pid,
                 bool kernel_thread) {
  this->pid = pid;
  this->kernel_thread = kernel_thread;
  uint32_t stack = START_STACKS_ADDRESS + 2 * pcb_index * STACK_SIZE;

  if (stack > STACK_MAX) {
    panic("Stack overflow\n");
  }
  this->kregs[3] = stack - 8;
  this->kregs[2] = *(uint32_t *)(stack - 8) = stack;
  *(uint32_t *)(stack - 4) =
      kernel_thread ? (uint32_t)entry_point : (uint32_t)&returning_kernel_entry;
  // printk("kt: %d ep: %x rke: %x\n", kernel_thread, entry_point,
  //              &returning_kernel_entry);

  if (!kernel_thread) {
    stack += STACK_SIZE;
    if (stack > STACK_MAX) {
      panic("Stack overflow\n");
    }
    this->uregs[3] = stack - 8;
    this->uregs[2] = *(uint32_t *)(stack - 8) = stack;
    *(uint32_t *)(stack - 4) = (uint32_t)entry_point;
  }

  // set eflags to current value (to allow interrupts, if set)
  this->kregs[8] = EFLAGS_RESERVED | EFLAGS_INTERRUPT_NOT_ENABLED;
  this->uregs[8] = EFLAGS_RESERVED | EFLAGS_INTERRUPT_NOT_ENABLED;

  this->status = READY;
}

void do_yield() {
  stop_kernel_timer();

  current_task->get().ready();
  sched->sched(current_task);

  start_timer();
  scheduler_entry();
}
void do_exit() {

  current_task->get().exit();

  scheduler_entry();
}

QueueNode<PCB> *Scheduler::get_ready_task() {
  if (this->ready_queue == nullptr) {
    printk("!!!No more tasks to run!!!\n");
    while (true) {
      /*NO TASKS TO RUN - BUSY WAIT*/
    }
  } else {
    return queue_remove(this->ready_queue, this->ready_queue);
  }
}

extern "C" void scheduler() {

  sched->inc_count();
  current_task = sched->get_ready_task();
  current_task->get().clear_last_cpu_time();
  current_task->get().run();
}

void Scheduler::add_task(void (*entry_point)(), bool kernel_thread) {
  int i = 0;
  while (i < NUM_TASKS) {
    if (this->pcbs[i].get().is_exited()) {
      break;
    }
    i++;
  }

  if (i >= NUM_TASKS) {
    panic("No more PCB available\n");
  }

  this->pcbs[i].get().config(i, entry_point, this->current_pid++,
                             kernel_thread);

  queue_insert(this->ready_queue, (QueueNode<PCB> *)&this->pcbs[i]);
}

void Scheduler::block(Lock *lock) {

  current_task->get().block();

  lock->block(current_task);
  // printk("Blocking task %d\n", current_task->get().get_pid() + 1);
  scheduler_entry();
}

void Scheduler::unblock(Lock *lock) {
  QueueNode<PCB> *tmp = lock->unblock();
  tmp->get().ready();
  // printk("Unblocking task %d\n", tmp->get().get_pid() + 1);
  this->sched(tmp);
}

static int comparePCB(PCB *a, PCB *b) {
  /*printk("a:%d %l b:%d %l\n", a->get_pid() + 1, a->get_cpu_time(),
         b->get_pid() + 1, b->get_cpu_time());*/
  if (a->get_cpu_time() < b->get_cpu_time()) {
    return -1;
  } else if (a->get_cpu_time() > b->get_cpu_time()) {
    return 1;
  } else {
    return 0;
  }
}

void Scheduler::sched(QueueNode<PCB> *task) {
  queue_insert(this->ready_queue, task);
}

void FairScheduler::sched(QueueNode<PCB> *task) {
  // printk("Fair scheduler!\n");
  queue_insert_ordered(this->ready_queue, task, &comparePCB);

  QueueNode<PCB> *tmp = this->ready_queue;
  do {
    printk("(%d %l) ", tmp->get().get_pid(), tmp->get().get_cpu_time());
    tmp = tmp->next;
  } while (tmp != this->ready_queue);
  printk("\n");
}
