#ifndef __QUEUE_H
#define __QUEUE_H

namespace Datastructure {
template <class T> class QueueNode {
public:
  T data;
  QueueNode<T> *next, *previous;
  QueueNode<T>(T data) {
    this->data = data;
    this->next = this;
    this->previous = this;
  }

  void insert(QueueNode<T> *);
  QueueNode<T> *remove(QueueNode<T> *);
  void print();
};

void test_queue();
} // namespace Datastructure

#endif