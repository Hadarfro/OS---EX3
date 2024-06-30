#include "Proactor.hpp"

// Constructor initializes member variables
Proactor::Proactor(int listenSock, proactorFunc func)
    : listenSock(listenSock), func(func), running(false) {}

// Destructor stops all threads and cleans up resources
Proactor::~Proactor() {
    stopAll();
}

// Starts the proactor by creating a new thread to accept client connections
pthread_t Proactor::start() {
    pthread_t tid;
    running = true;
    if (pthread_create(&tid, nullptr, acceptConnections, this) != 0) {
        perror("pthread_create");
        return 0;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        threads.push_back(tid);  // Store the thread ID in the threads vector
    }
    return tid;  // Return the ID of the newly created thread
}

// Stops a specific thread given its thread ID
int Proactor::stop(pthread_t tid) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::find(threads.begin(), threads.end(), tid);  // Find the thread ID in the threads vector
    if (it != threads.end()) {  // If thread ID is found
        pthread_cancel(tid);  // Cancel the thread
        pthread_join(tid, nullptr);  // Wait for the thread to terminate
        threads.erase(it);  // Remove the thread ID from the threads vector
        return 0;  // Return success
    }
    return -1;  // Return failure if thread ID was not found
}

// Static method that runs in a loop to accept incoming client connections
void *Proactor::acceptConnections(void *arg) {
    Proactor *proactor = static_cast<Proactor *>(arg);  // Cast the void pointer argument back to Proactor instance
    while (proactor->running) {  // Continue accepting connections while running flag is true
        int clientSock = accept(proactor->listenSock, nullptr, nullptr);  // Accept a new client connection
        if (clientSock < 0) {  // Check for errors in accepting client socket
            perror("accept");
            continue;
        }
        pthread_t tid;  // Declare a pthread_t variable to hold thread ID
        // Create a new thread to handle the accepted client socket using func
        if (pthread_create(&tid, nullptr, threadFuncWrapper, (void *)(intptr_t)clientSock) != 0) {
            perror("pthread_create");
            close(clientSock);  // Close the client socket if thread creation fails
        }
        {
            std::lock_guard<std::mutex> lock(proactor->mtx);  // Lock the mutex to safely modify threads vector
            proactor->threads.push_back(tid);  // Store the new thread ID in the threads vector
        }
    }
    return nullptr;  // Return nullptr when accept loop terminates
}

// Static method that wraps the proactorFunc to match the signature required by pthread_create
void *Proactor::threadFuncWrapper(void *arg) {
    int sockfd = (intptr_t)arg;  // Cast void pointer argument to int (client socket descriptor)
    return (proactorFunc(sockfd));  // Call the proactorFunc with the client socket descriptor and return its result
}

// Stops all active threads and clears the threads vector
void Proactor::stopAll() {
    std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex to safely access threads vector
    running = false;  // Set running flag to false to terminate accept loop
    for (pthread_t tid : threads) {  // Iterate through all active thread IDs in threads vector
        pthread_cancel(tid);  // Cancel each thread
        pthread_join(tid, nullptr);  // Wait for each thread to terminate
    }
    threads.clear();  // Clear the threads vector
}
