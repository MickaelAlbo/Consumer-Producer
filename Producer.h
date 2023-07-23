#ifndef PRODUCER_H
#define PRODUCER_H

#include <semaphore.h>
#include "Queue.h"

class Producer {
    int id;
    int numStories;
    Queue* queue;
    int maxSize;
    int added;
    sem_t full;
    sem_t access;

public:
    Producer(int id, int numStories, int max);
    ~Producer();
    void insert(const std::string& story);
    std::string remove();
    void produceStories();
};

#endif // PRODUCER_H
