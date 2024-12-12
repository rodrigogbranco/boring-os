#include "include/queue.h"
#include "include/util.h"

using namespace Datastructure;

template <class T> void QueueNode<T>::insert(QueueNode<T> *node) {
  this->previous->next = node;
  node->previous = this->previous;

  this->previous = node;
  node->next = this;
}

template <class T> QueueNode<T> *QueueNode<T>::remove(QueueNode<T> *node) {
  node->previous->next = node->next;
  node->next->previous = node->previous;

  if (this == node) {
    return this->next;
  }

  return this;
}

template <class T> void QueueNode<T>::print() {
  QueueNode<T> *tmp = this;
  do {
    Util::printk("%d ", tmp->data);
    tmp = tmp->next;
  } while (tmp != this);
  Util::printk("\n");
}

void Datastructure::test_queue() {
  QueueNode<int> array[] = {
      QueueNode<int>(0), QueueNode<int>(1), QueueNode<int>(2),
      QueueNode<int>(3), QueueNode<int>(4),
  };
  QueueNode<int> *head = &array[0];
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
}