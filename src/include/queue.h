#ifndef __QUEUE_H
#define __QUEUE_H

namespace Datastructure {
template <class T> class QueueNode {
public:
  T data;
  QueueNode<T> *next, *previous;
  constexpr QueueNode<T>() : next(this), previous(this){};
  constexpr explicit QueueNode<T>(T data)
      : data(data), next(this), previous(this){};

  void insert(QueueNode<T> *);
  QueueNode<T> *remove(QueueNode<T> *);
  void print();
  constexpr T &get() { return data; }
};
} // namespace Datastructure

#endif