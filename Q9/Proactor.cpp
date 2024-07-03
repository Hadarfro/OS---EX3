// Proactor.cpp

#include "Proactor.hpp"

Proactor::Proactor(int listenSock, proactorFunc func)
    : listenSock(listenSock), func(func), running(false) {}

Proactor::~Proactor() {
    stopAll();
}

pthread_t Proactor::start() {
    pthread_t tid;
    running = true;
    if (pthread_create(&tid, nullptr, acceptConnections, this) != 0) {
        perror("pthread_create");
        return 0;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        threads.push_back(tid);
    }
    return tid;
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

void *Proactor::threadFuncWrapper(void *arg) {
    intptr_t sockfd = (intptr_t)arg;
    Proactor *proactor = nullptr;

    if (proactor) {
        return proactor->func(sockfd); // Call the function pointer through proactor instance
    } else {
        // Handle the case where proactor is nullptr or not found
        std::cerr << "Proactor instance not found for sockfd: " << sockfd << std::endl;
        return nullptr; // Or handle the error accordingly
    }

    return nullptr;
}

void *Proactor::acceptConnections(void *arg) {
    Proactor *proactor = static_cast<Proactor *>(arg);
    while (proactor->running) {
        int clientSock = accept(proactor->listenSock, nullptr, nullptr);
        if (clientSock < 0) {
            perror("accept");
            continue;
        }
        pthread_t tid;
        if (pthread_create(&tid, nullptr, threadFuncWrapper, (void *)(intptr_t)clientSock) != 0) {
            perror("pthread_create");
            close(clientSock);
        }
        std::lock_guard<std::mutex> lock(proactor->mtx);
        proactor->threads.push_back(tid);
    }
    return nullptr;
}
