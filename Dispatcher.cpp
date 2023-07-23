#include "Dispatcher.h"
#include <iostream>
#include <unistd.h>

Dispatcher::Dispatcher(Editor* newsEditor, Editor* sportsEditor, Editor* weatherEditor) {
    this->newsEditor = newsEditor;
    this->sportsEditor = sportsEditor;
    this->weatherEditor = weatherEditor;
}

void Dispatcher::addProducerQueue(Producer* q) {
    producerQueues.push_back(q);
}

void Dispatcher::start() {
    int counter = 0;
    while (!producerQueues.empty()) {
        if (counter >= producerQueues.size()) {
            counter = 0;
        }
        std::string story = producerQueues[counter]->remove();
        if (!story.empty()) {
            if (story == "DONE") {
                producerQueues.erase(producerQueues.begin() + counter);
            } else {
                if (story.find("SPORTS") != std::string::npos) {
                    sportsEditor->insert(story);
                } else if (story.find("WEATHER") != std::string::npos) {
                    weatherEditor->insert(story);
                } else if (story.find("NEWS") != std::string::npos) {
                    newsEditor->insert(story);
                }
            }
        }
        counter++;
        sleep(0.01);
    }
    newsEditor->insert("DONE");
    weatherEditor->insert("DONE");
    sportsEditor->insert("DONE");
}
