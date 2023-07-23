#include "Queue.h"

Queue::Queue(int accessLimit) {
    sem_init(&sem, 0, accessLimit);
}

Queue::~Queue() {
    sem_destroy(&sem);
}

void Queue::insert(const std::string& story) {
    sem_wait(&sem);
    items.push_back(story);
    sem_post(&sem);
}

std::string Queue::remove() {
    std::string story = "";
    sem_wait(&sem);
    if (!items.empty()) {
        story = items[0];
        items.erase(items.begin());
    }
    sem_post(&sem);
    return story;
}

int Queue::size() {
    return items.size();
}
