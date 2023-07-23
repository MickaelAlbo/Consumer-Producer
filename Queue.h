#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <semaphore.h>
#include <string>

class Queue {
    std::vector<std::string> items;
    sem_t sem;

public:
    Queue(int accessLimit);
    ~Queue();
    void insert(const std::string& story);
    std::string remove();
    int size();
};

#endif // QUEUE_H
