#include "Proactor.hpp"

Proactor::Proactor(int listenSock, proactorFunc func)
    : listenSock(listenSock), func(func), running(false), monitorRunning(false) {}

Proactor::~Proactor() {
    stopAll();
    stopMonitorThread();
}

pthread_t Proactor::start() {
    pthread_t tid;
    this->running = true;
    if (pthread_create(&tid, nullptr, acceptConnections, this) != 0) {
        perror("pthread_create error");
        return 0;
    }
    std::lock_guard<std::mutex> lock(mtx);
    threads.push_back(tid);

    // Start the monitoring thread
    startMonitorThread();

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
    stopMonitorThread();
}

void Proactor::signalSCCCondition() {
    std::lock_guard<std::mutex> lock(condMtx);
    condVar.notify_all();
}

void *Proactor::monitorGraph(void *arg) {
    Proactor* proactor = reinterpret_cast<Proactor*>(arg);

    while (proactor->monitorRunning) {
        std::unique_lock<std::mutex> lock(proactor->condMtx);
        proactor->condVar.wait(lock); // Wait for the signal to check SCC condition

        if (!proactor->monitorRunning) {
            break; // Exit if monitoring is stopped
        }

        // Example condition: Replace with actual logic
        bool atLeast50PercentSameSCC = true;  // Replace with your actual condition

        if (atLeast50PercentSameSCC) {
            std::cout << "At Least 50% of the graph belongs to the same SCC\n";
        } else {
            std::cout << "At Least 50% of the graph no longer belongs to the same SCC\n";
        }
    }

    return nullptr;
}

void Proactor::startMonitorThread() {
    monitorRunning = true;
    if (pthread_create(&monitorThread, nullptr, monitorGraph, this) != 0) {
        perror("pthread_create error");
        monitorRunning = false;
    }
}

void Proactor::stopMonitorThread() {
    if (monitorRunning) {
        monitorRunning = false;
        condVar.notify_all(); // Wake up the monitoring thread to exit
        pthread_join(monitorThread, nullptr);
    }
}
