#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <vector>
#include "Queue.h"
#include "CoEditor.h"
#include "Producer.h"

class Dispatcher {
    std::vector<Producer*> producerQueues;
    Editor* newsEditor;
    Editor* sportsEditor;
    Editor* weatherEditor;

public:
    Dispatcher(Editor* newsEditor, Editor* sportsEditor, Editor* weatherEditor);
    void addProducerQueue(Producer* q);
    void start();
};

#endif // DISPATCHER_H
