#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <pthread.h>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <algorithm> // Include for std::find

// Define a function pointer type for the callback function
typedef void *(*proactorFunc)(int sockfd);

class Proactor {
private:
    int listenSock;  // Listening socket descriptor
    proactorFunc func;  // Function pointer for client handling function
    std::vector<pthread_t> threads;  // Vector to hold active thread IDs
    std::mutex mtx;  // Mutex for thread safety in accessing shared data
    bool running;  // Flag to control the main loop of the proactor
    
public:
    Proactor(int listenSock, proactorFunc func);
    ~Proactor();
    pthread_t start();  // Starts the proactor and returns the thread ID
    int stop(pthread_t tid);  // Stops a specific thread given its ID

    // Stops all threads and clears the threads vector
    void stopAll();

    // Static method used as the entry point for pthread_create
    static void *acceptConnections(void *arg);

    // Static method to wrap the proactorFunc to match pthread_create's signature
    static void *threadFuncWrapper(void *arg);
};

#endif // PROACTOR_HPP