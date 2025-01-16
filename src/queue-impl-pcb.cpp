#include "queue.cpp"

template void QueueNode<PCB>::print();

template void QueueNode<PCB>::insert(QueueNode<PCB> *);

template QueueNode<PCB> *QueueNode<PCB>::remove(QueueNode<PCB> *);

template void queue_insert(QueueNode<PCB> *&, QueueNode<PCB> *);

template void queue_insert_ordered(QueueNode<PCB> *&, QueueNode<PCB> *,
                                   int (*)(PCB *, PCB *));

template QueueNode<PCB> *queue_remove(QueueNode<PCB> *&, QueueNode<PCB> *);