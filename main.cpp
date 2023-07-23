#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "Queue.h"
#include "Producer.h"
#include "Dispatcher.h"
#include "CoEditor.h"
#include "ScreenManager.h"

using namespace std;

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

int main(int argc, char* argv[]) {
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
