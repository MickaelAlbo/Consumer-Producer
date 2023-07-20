// Michael Albo - 328597521 - I choose the option 2

#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <thread>
#include <functional>

using namespace std;

// Queue class
class Queue {
    vector<string> items;
    sem_t sem;

public:
    Queue(int accessLimit) {
        sem_init(&sem, 0, accessLimit);
    }
    ~Queue() {
        sem_destroy(&sem);
    }
    void insert(const string& story) {
        sem_wait(&sem);
        items.push_back(story);
        sem_post(&sem);
    }
    string remove() {
        string story = "";
        sem_wait(&sem);
        if (!items.empty()) {
            story = items[0];
            items.erase(items.begin());
        }
        sem_post(&sem);
        return story;
    }
    int size() {
        return items.size();
    }
};

// Producer class
class Producer {
    int id;
    int numStories;
    Queue* queue;
    int maxSize;
    int added;
    sem_t full;
    sem_t access;
public:
    Producer(int id, int numStories,int max){
        this->id = id;
        this->numStories = numStories;
        this->queue = new Queue(1);
        this->maxSize = max;
        added = 0;
        sem_init(&full, 0, 1);
        sem_init(&access, 0, 1);
    }
    ~Producer() {
        sem_destroy(&full);
        delete queue;
    }
    void insert(const string& story) {
        if (added >= maxSize){
            sem_wait(&full);
        }
        sem_wait(&access);
        queue->insert(story);
        added++;
        sem_post(&access);
    }
    string remove() {
        string story = queue->remove();
        added--;
        sem_post(&full);
        return story;
    }
    void produceStories() {
        int counter_weather = 0;
        int counter_news = 0;
        int counter_sports = 0;
        srand(time(0));
        int num;
        string story = "";
        for(int i = 0; i < numStories; i++) {
            num = rand() % 30;
            if (num > 20) {
                story = "Producer " + to_string(id) + " SPORTS " + to_string(counter_sports++);
            } else if (num > 10) {
                story = "Producer " + to_string(id) + " NEWS " + to_string(counter_news++);
            } else {
                story = "Producer " + to_string(id) + " WEATHER " + to_string(counter_weather++);
            }
            this->insert(story);
            sleep(0.01);
        }
        this->insert("DONE");
    }
};

// Co-Editor
class Editor {
    Queue* queue;
    Queue* screenManagerQueue;
    string type;
    int maxSize;
    int added;
    sem_t full;
    sem_t access;
public:
    Editor(int max, Queue* sm) {
        added = 0;
        this->maxSize = max;
        this->queue = new Queue(1);
        this->type = type;
        this->screenManagerQueue = sm;
        sem_init(&full, 0, 1);
        sem_init(&access, 0, 1);
    }
    ~Editor() {
        sem_destroy(&full);
        delete queue;
    }
    void insert(const string& story) {
        if (added >= maxSize) {
            sem_wait(&full);
        }
        sem_wait(&access);
        queue->insert(story);
        added++;
        sem_post(&access);
    }
    string remove() {
        string story = queue->remove();
        sem_post(&full);
        added--;
        return story;
    }
    void start() {
        bool done = false;
        while(!done) {
            if (queue->size() > 0) {
                string story = this->remove();
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
};

// Dispatcher
class Dispatcher {
    vector<Producer*> producerQueues;
    Editor* newsEditor;
    Editor* sportsEditor;
    Editor* weatherEditor;
public:
    Dispatcher(Editor* newsEditor, Editor* sportsEditor, Editor* weatherEditor) {
        this->newsEditor = newsEditor;
        this->sportsEditor = sportsEditor;
        this->weatherEditor = weatherEditor;
    }
    void addProducerQueue(Producer* q) {
        producerQueues.push_back(q);
    }
    void start() {
        int counter = 0;
        while(!producerQueues.empty()) {
            if (counter >= producerQueues.size()) {
                counter = 0;
            }
            string story = producerQueues[counter]->remove();
            if (story.length() > 0) {
                if (story == "DONE") {
                    producerQueues.erase(producerQueues.begin()+counter);
                } else {
                    if (story.find("SPORTS") != string::npos) {
                        sportsEditor->insert(story);
                    } else if (story.find("WEATHER") != string::npos) {
                        weatherEditor->insert(story);
                    } else if (story.find("NEWS") != string::npos) {
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
};

// Screen Manager
class ScreenManager {
    Queue* queue;
public:
    ScreenManager(Queue* queue) {
        this->queue = queue;
    }
    void start() {
        int finished = 0;
        while(finished < 3) {
            string story = queue->remove();
            if (story == "DONE") {
                finished++;
            } else if (story.length() > 0 ) {
                cout << story << endl;
            }
            sleep(0.01);
        }
        cout << "DONE" << endl;
    }
};

// Functions for threads

void runP(Producer* p) {
    p->produceStories();
}

void runN(Editor* newsEditor) {
    newsEditor->start();
}

void runW(Editor* weatherEditor) {
    weatherEditor->start();
}

void runS(Editor* sportsEditor) {
    sportsEditor->start();
}

void runD(Dispatcher* dispatcher) {
    dispatcher->start();
}

void runSM(ScreenManager* sm) {
    sm->start();
}

int main(int argc, char *argv[])
{
    // Check if there are enough arguments
    if (argc != 2) {
        cout << "Provide 2 arguments when then second is the file name\n";
        exit(1);
    }
    // Read the config txt
    ifstream file(argv[1]);
    int pid;
    int numProducts1;
    int qSize;
    int eSize = 0;
    string line;
    vector<Producer*> v_producers;
    while(!file.eof()) {
        getline(file, line);
        pid = stoi(line);
        getline(file, line);
        if (file.eof()) {
            // Get the Co-Editor queue size
            eSize = pid;
            break;
        }
        numProducts1 = stoi(line);
        getline(file, line);
        qSize = stoi(line);
        getline(file, line);
        v_producers.push_back(new Producer(pid, numProducts1, qSize));
    }
    // Create screen manager queue
    Queue* smq = new Queue(1);
    ScreenManager* sm = new ScreenManager(smq);
    // Create the editors news
    Editor* newsEditor = new Editor(eSize, smq);
    Editor* sportsEditor = new Editor(eSize, smq);
    Editor* weatherEditor = new Editor(eSize, smq);
    // Create queue of the editors
    Dispatcher* dispatcher = new Dispatcher(newsEditor, sportsEditor, weatherEditor);
    // For iterate
    for (auto it : v_producers) {
        dispatcher->addProducerQueue(it);
    }
    // Create vector of threads
    vector<thread> vecOfThreads;
    // Create threads for all producers
    for (auto it : v_producers) {
        thread th(runP, it);
        vecOfThreads.push_back(move(th));
    }
    // Create other threads
    thread t1(runN, newsEditor);
    vecOfThreads.push_back(move(t1));
    thread t2(runW, weatherEditor);
    vecOfThreads.push_back(move(t2));
    thread t3(runS, sportsEditor);
    vecOfThreads.push_back(move(t3));
    thread t4(runD, dispatcher);
    vecOfThreads.push_back(move(t4));
    thread t5(runSM, sm);
    vecOfThreads.push_back(move(t5));
    // Join all the threads
    for (thread & th : vecOfThreads) {
        if (th.joinable()) {
            th.join();
        }
    }
    // Delete all the objects
    delete smq;
    delete sm;
    delete newsEditor;
    delete sportsEditor;
    delete weatherEditor;
    delete dispatcher;
    // Delete all the producers
    for (auto it : v_producers) {
        delete it;
    }
    v_producers.clear();
    return 0;
}

