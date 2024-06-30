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

void *Proactor::acceptConnections(void *arg) {
    Proactor *proactor = static_cast<Proactor *>(arg);
    while (proactor->running) {
        int clientSock = accept(proactor->listenSock, nullptr, nullptr);
        if (clientSock < 0) {
            perror("accept");
            continue;
        }
        pthread_t tid;
        if (pthread_create(&tid, nullptr, proactor->func, (void *)(intptr_t)clientSock) != 0) {
            perror("pthread_create");
            close(clientSock);
        }
    }
    return nullptr;
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