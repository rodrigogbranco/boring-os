#include "queue.cpp"

template void Datastructure::QueueNode<PCB>::print();

template void
Datastructure::QueueNode<PCB>::insert(Datastructure::QueueNode<PCB> *);

template Datastructure::QueueNode<PCB> *
Datastructure::QueueNode<PCB>::remove(Datastructure::QueueNode<PCB> *);