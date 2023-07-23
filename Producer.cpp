#include "Producer.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>

Producer::Producer(int id, int numStories, int max) {
    this->id = id;
    this->numStories = numStories;
    this->queue = new Queue(1);
    this->maxSize = max;
    added = 0;
    sem_init(&full, 0, 1);
    sem_init(&access, 0, 1);
}

Producer::~Producer() {
    sem_destroy(&full);
    delete queue;
}

void Producer::insert(const std::string& story) {
    if (added >= maxSize) {
        sem_wait(&full);
    }
    sem_wait(&access);
    queue->insert(story);
    added++;
    sem_post(&access);
}

std::string Producer::remove() {
    std::string story = queue->remove();
    added--;
    sem_post(&full);
    return story;
}

void Producer::produceStories() {
    int counter_weather = 0;
    int counter_news = 0;
    int counter_sports = 0;
    srand(time(0));
    int num;
    std::string story = "";
    for (int i = 0; i < numStories; i++) {
        num = rand() % 30;
        if (num > 20) {
            story = "Producer " + std::to_string(id) + " SPORTS " + std::to_string(counter_sports++);
        } else if (num > 10) {
            story = "Producer " + std::to_string(id) + " NEWS " + std::to_string(counter_news++);
        } else {
            story = "Producer " + std::to_string(id) + " WEATHER " + std::to_string(counter_weather++);
        }
        this->insert(story);
        sleep(0.01);
    }
    this->insert("DONE");
}
