// Proactor.cpp

#include "Proactor.hpp"

Proactor::Proactor(int listenSock, proactorFunc func)
    : listenSock(listenSock), func(func), running(false) {}

Proactor::~Proactor() {
    stopAll();
}

pthread_t Proactor::start() {
    pthread_t tid;
    this->running = true;
    std::cout<<"check 1"<<std::endl;
    if (pthread_create(&tid, nullptr, acceptConnections, this) != 0) {
        perror("pthread_create error");
        return 0;
    }
    std::lock_guard<std::mutex> lock(mtx);
    threads.push_back(tid);

    return tid;
}

void *Proactor::threadFuncWrapper(void *arg) {
    intptr_t sockfd = (intptr_t)arg;
    Proactor *proactor = reinterpret_cast<Proactor*>(pthread_getspecific(pthread_self()));

    if (proactor && proactor->func) {
        proactor->func(static_cast<int>(sockfd)); // Call the function pointer with sockfd
    } else {
        std::cerr << "Error: Proactor instance or function pointer not found for sockfd: " << sockfd << std::endl;
    }

    return nullptr;
}

void *Proactor::acceptConnections(void *arg) {
    Proactor *proactor = reinterpret_cast<Proactor*>(arg);

    while (proactor->running == true) {
        int clientSock = accept(proactor->listenSock, nullptr, nullptr);
        if (clientSock < 0) {
            perror("accept");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, nullptr, Proactor::threadFuncWrapper, (void *)(intptr_t)clientSock) != 0) {
            perror("pthread_create error");
            close(clientSock);
            continue; // Continue accepting connections on failure
        }

        {
            std::lock_guard<std::mutex> lock(proactor->mtx);
            proactor->threads.push_back(tid);
        }
    }

    return nullptr;
}

int Proactor::stop(pthread_t tid) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::find(threads.begin(), threads.end(), tid);
    if (it != threads.end()) {
        pthread_cancel(tid);
        pthread_join(tid, nullptr);
        threads.erase(it);
        return 0;
    }
    return -1;
}

void Proactor::stopAll() {
    std::lock_guard<std::mutex> lock(mtx);
    running = false;
    for (pthread_t tid : threads) {
        pthread_cancel(tid);
        pthread_join(tid, nullptr);
    }
    threads.clear();
}

