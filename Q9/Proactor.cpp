#include "Proactor.hpp"

Proactor::Proactor(int listenSock, proactorFunc func)
    : listenSock(listenSock), func(func), running(false) {}

Proactor::~Proactor() {
    stopAll();
}

pthread_t Proactor::start() {
    pthread_t tid;
    this->running = true;
    std::cout << "check 1" << std::endl;
    if (pthread_create(&tid, nullptr, acceptConnections, this) != 0) {
        perror("pthread_create error");
        return 0;
    }
    std::lock_guard<std::mutex> lock(mtx);
    threads.push_back(tid);

    return tid;
}

void *Proactor::threadFuncWrapper(void *arg) {
    ThreadArg* threadArg = static_cast<ThreadArg*>(arg);
    Proactor* proactor = threadArg->proactor;
    int clientSock = threadArg->clientSock;

    if (proactor && proactor->func) {
        proactor->func(clientSock); // Call the function pointer with clientSock
    } else {
        std::cerr << "Error: Proactor instance or function pointer not found for sockfd: " << clientSock << std::endl;
    }

    delete threadArg; // Clean up dynamically allocated struct
    return nullptr;
}

void *Proactor::acceptConnections(void *arg) {
    Proactor *proactor = reinterpret_cast<Proactor*>(arg);

    while (proactor->running) {
        int clientSock = accept(proactor->listenSock, nullptr, nullptr);
        if (clientSock < 0) {
            perror("accept");
            continue;
        }

        ThreadArg* threadArg = new ThreadArg{proactor, clientSock}; // Allocate struct dynamically

        pthread_t tid;
        if (pthread_create(&tid, nullptr, Proactor::threadFuncWrapper, threadArg) != 0) {
            perror("pthread_create error");
            close(clientSock);
            delete threadArg; // Clean up dynamically allocated struct
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
