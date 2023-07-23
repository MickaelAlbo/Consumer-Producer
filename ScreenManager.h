#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Queue.h"

class ScreenManager {
    Queue* queue;

public:
    ScreenManager(Queue* queue);
    void start();
};

#endif // SCREENMANAGER_H
