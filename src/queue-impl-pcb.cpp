#include "queue.cpp"

template void QueueNode<PCB>::print();

template void QueueNode<PCB>::insert(QueueNode<PCB> *);

template QueueNode<PCB> *QueueNode<PCB>::remove(QueueNode<PCB> *);