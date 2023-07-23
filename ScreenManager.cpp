#include "ScreenManager.h"
#include <iostream>
#include <string>
#include <unistd.h>

ScreenManager::ScreenManager(Queue* queue) {
    this->queue = queue;
}

void ScreenManager::start() {
    int finished = 0;
    while (finished < 3) {
        std::string story = queue->remove();
        if (story == "DONE") {
            finished++;
        } else if (!story.empty()) {
            std::cout << story << std::endl;
        }
        usleep(10000); // Sleep for 0.01 seconds (10,000 microseconds)
    }
    std::cout << "DONE" << std::endl;
}
