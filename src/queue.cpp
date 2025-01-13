#include "include/queue.h"
#include "include/kernel.h"
#include "include/util.h"

template <class T> void QueueNode<T>::insert(QueueNode<T> *node) {
  this->previous->next = node;
  node->previous = this->previous;

  this->previous = node;
  node->next = this;
  // Util::printk("added %x\n", node);
}

template <class T> QueueNode<T> *QueueNode<T>::remove(QueueNode<T> *node) {
  if (this == node && this->next == this) {
    // Util::printk("\n\n\n\n\nremoved %x - EMPTY QUEUE!\n", node);
    return nullptr;
  }

  QueueNode<T> *tmp = this->next;
  node->previous->next = node->next;
  node->next->previous = node->previous;

  node->previous = node;
  node->next = node;

  // Util::printk("\n\n\n\n\nremoved %x\n", node);

  return this == node ? tmp : this;
}

template <class T> void QueueNode<T>::print() {
  QueueNode<T> *tmp = this;
  do {
    printk("%d ", tmp->data);
    tmp = tmp->next;
  } while (tmp != this);
  printk("\n");
}