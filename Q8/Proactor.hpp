#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <pthread.h>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <mutex>

typedef void *(*proactorFunc)(int sockfd);

class Proactor {
public:
    Proactor(int listenSock, proactorFunc func);
    ~Proactor();
    pthread_t start();
    int stop(pthread_t tid);

private:
    int listenSock;
    proactorFunc func;
    std::vector<pthread_t> threads;
    std::mutex mtx;
    bool running;

    static void *acceptConnections(void *arg);
};

#endif // PROACTOR_HPP