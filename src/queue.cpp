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
  QueueNode<T> *tmp = this->next;
  node->previous->next = node->next;
  node->next->previous = node->previous;

  node->previous = node;
  node->next = node;

  return tmp;
}

template <class T> void QueueNode<T>::print() {
  QueueNode<T> *tmp = this;
  do {
    printk("%d ", tmp->data);
    tmp = tmp->next;
  } while (tmp != this);
  printk("\n");
}

template <class T> void queue_insert(QueueNode<T> *&queue, QueueNode<T> *node) {
  if (queue != nullptr) {
    queue->insert(node);
  } else {
    queue = node;
    // printk("ready queue %x\n", ready_queue);
  }
}

template <class T>
void queue_insert_ordered(QueueNode<T> *&queue, QueueNode<T> *node,
                          int (*compare)(T *, T *)) {
  if (queue == nullptr) {
    queue = node;
    return;
  }

  QueueNode<T> *tmp = queue;
  while (tmp != queue->previous) {
    if (compare(&node->get(), &tmp->get()) < 0) {
      break;
    }
    tmp = tmp->next;
  }

  if (compare(&node->get(), &tmp->get()) < 0) {
    tmp = tmp->previous;
  }

  tmp->next->previous = node;
  node->next = tmp->next;
  tmp->next = node;
  node->previous = tmp;

  if (compare(&node->get(), &queue->get()) < 0) {
    queue = node;
  }
}

template <class T>
QueueNode<T> *queue_remove(QueueNode<T> *&queue, QueueNode<T> *node) {
  if (queue == nullptr) {
    return nullptr;
  }

  if (queue == node && queue->next == queue) {
    // Util::printk("\n\n\n\n\nremoved %x - EMPTY QUEUE!\n", node);
    queue = nullptr;
    return node;
  }

  QueueNode<T> *tmp = queue->remove(node);
  // Util::printk("\n\n\n\n\nremoved %x\n", node);

  if (queue == node) {
    queue = tmp;
  }

  return node;
}