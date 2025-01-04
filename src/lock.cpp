#include "include/kernel.h"
#include <atomic>

extern Scheduler sched;

void Lock::lock_acquire() {

  if (this->mutex.exchange(true)) {
    sched.block(this);
  }
}

void Lock::lock_release() {
  this->mutex.store(false);
  if (this->blocked != nullptr) {
    sched.unblock(this);
  }
}