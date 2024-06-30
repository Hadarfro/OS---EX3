#include "Reactor.hpp"
#include <unistd.h>
#include <errno.h>
#include <iostream>

Reactor::Reactor() : running(false) {}

Reactor::~Reactor() {}

void Reactor::start() {
    running = true;
    while (running) {
        int n = poll(fds.data(), fds.size(), -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("poll");
            break;
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                funcs[i](fds[i].fd);
            }
        }
    }
}

bool Reactor::addFd(int fd, ReactorFunc func) {
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    fds.push_back(pfd);
    funcs.push_back(func);

    return true;
}

bool Reactor::removeFd(int fd) {
    for (size_t i = 0; i < fds.size(); i++) {
        if (fds[i].fd == fd) {
            fds.erase(fds.begin() + static_cast<vector<pollfd>::difference_type>(i));
            funcs.erase(funcs.begin() + static_cast<std::vector<pollfd>::difference_type>(i));
            //funcs.erase(funcs.begin() + i);
            return true;
        }
    }
    return false;
}

void Reactor::stop() {
    running = false;
}

extern "C" {
    void* startReactor() {
        return new Reactor();
    }

    int addFdToReactor(void* reactor, int fd, ReactorFunc func) {
        return static_cast<Reactor*>(reactor)->addFd(fd, func);
    }

    int removeFdFromReactor(void* reactor, int fd) {
        return static_cast<Reactor*>(reactor)->removeFd(fd);
    }

    void stopReactor(void* reactor) {
        static_cast<Reactor*>(reactor)->stop();
    }
}

