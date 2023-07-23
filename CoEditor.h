#ifndef COEDITOR_H
#define COEDITOR_H

#include <semaphore.h>
#include "Queue.h"

class Editor {
    Queue* queue;
    Queue* screenManagerQueue;
    std::string type;
    int maxSize;
    int added;
    sem_t full;
    sem_t access;

public:
    Editor(int max, Queue* sm);
    ~Editor();
    void insert(const std::string& story);
    std::string remove();
    void start();
};

#endif // COEDITOR_H
