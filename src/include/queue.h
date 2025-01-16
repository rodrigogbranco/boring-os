#ifndef __QUEUE_H
#define __QUEUE_H

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

template <class T> void queue_insert(QueueNode<T> *&, QueueNode<T> *);
template <class T>
void queue_insert_ordered(QueueNode<T> *&, QueueNode<T> *, int (*)(T *, T *));
template <class T> QueueNode<T> *queue_remove(QueueNode<T> *&, QueueNode<T> *);

#endif