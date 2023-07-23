#include "CoEditor.h"
#include <iostream>
#include <unistd.h>

Editor::Editor(int max, Queue* sm) {
    added = 0;
    this->maxSize = max;
    this->queue = new Queue(1);
    this->screenManagerQueue = sm;
    sem_init(&full, 0, 1);
    sem_init(&access, 0, 1);
}

Editor::~Editor() {
    sem_destroy(&full);
    delete queue;
}

void Editor::insert(const std::string& story) {
    if (added >= maxSize) {
        sem_wait(&full);
    }
    sem_wait(&access);
    queue->insert(story);
    added++;
    sem_post(&access);
}

std::string Editor::remove() {
    std::string story = queue->remove();
    sem_post(&full);
    added--;
    return story;
}

void Editor::start() {
    bool done = false;
    while (!done) {
        if (queue->size() > 0) {
            std::string story = this->remove();
            if (story == "DONE") {
                this->screenManagerQueue->insert(story);
                done = true;
                break;
            }
            sleep(0.1);
            this->screenManagerQueue->insert(story);
        }
    }
}
