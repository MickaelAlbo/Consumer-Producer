# Consumer-Producer

# Concurrent News Broadcasting Simulation

This project is an assignment aimed at gaining experience with concurrent programming and synchronization mechanisms. The simulation models a news broadcasting system with producers, dispatchers, co-editors, and a screen manager. The purpose is to display news stories to the public in the order they arrive while demonstrating the impact of synchronization constructs under different loads.

## Project Structure

1. `main.cpp`: The main function that reads the configuration file, initializes the system, and starts the simulation.

2. `Queue.h` and `Queue.cpp`: A custom Queue object that implements the bounded buffer requirements for the producer and co-editor queues.

3. `Producer.cpp` and `Producer.h`: Implementation of the producer thread that generates news stories and communicates with the dispatcher through the producer queue.

4. `Dispatcher.cpp` and `Dispatcher.h`: Implementation of the dispatcher thread that receives messages from producers, sorts them based on type, and sends them to co-editors through dispatcher queues.

5. `CoEditor.cpp` and `CoEditor.h`: Implementation of the co-editor threads that edit messages from the dispatcher and pass them to the screen manager through a shared queue.

6. `ScreenManager.cpp` and `ScreenManager.h`: Implementation of the screen manager thread that displays messages received from co-editors on the screen and prints a 'DONE' statement after receiving three "DONE" messages.

7. Configuration File (e.g., `config.txt`): The configuration file containing details about producers, their products, and queue sizes for the producer and co-editor queues.

## System Design

<img width="350" alt="image" src="https://github.com/MickaelAlbo/Consumer-Producer/assets/71727260/0dca94c5-ab21-48fc-8914-98f7be21bee3">


## How to Build and Run

1. Ensure that you have a compatible C/C++ compiler installed on your system.

2. Clone this repository to your local machine:

`git clone https://github.com/MickaelAlbo/Consumer-Producer`  
`cd Consumer-Producer`


3. Compile the project using the make command:

`make`  
`./consumer-producer.out config.txt`


Note: Replace `config.txt` with the actual name of your configuration file.

## Configuration File Format

The configuration file has the following format:

```
[NUMBER OF THE PRODUCER 1]
[number of products]
[QUEUE SIZE]

[NUMBER OF THE PRODUCER 2]
[number of products]
[QUEUE SIZE]

...
...
...

[NUMBER OF THE PRODUCER n]
[number of products]
[QUEUE SIZE]

[SIZE OF THE CO-EDITOR]
```

## Exmaple of Configuration File Format
```
1
30
5

2
25
3

3
16
30

17
```
