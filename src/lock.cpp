#include "include/kernel.h"
#include "include/util.h"
#include <atomic>

extern Scheduler sched;

void Lock::lock_acquire() {
  if (this->mutex.exchange(true)) {
    sched.block(this);
  }
}

void Lock::lock_release() {
  if (this->blocked != nullptr) {
    sched.unblock(this);
  } else {
    // Util::printk("empty queue\n");
    this->mutex.store(false);
  }
}