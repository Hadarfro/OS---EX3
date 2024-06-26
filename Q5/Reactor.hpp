#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <functional>
#include <vector>
#include <poll.h>

using namespace std;

using ReactorFunc = function<void(int fd)>;

class Reactor {
    public:
        Reactor();
        ~Reactor();

        // Starts the reactor
        void start();

        // Adds fd to Reactor (for reading); returns true on success
        bool addFd(int fd, ReactorFunc func);

        // Removes fd from reactor; returns true on success
        bool removeFd(int fd);

        // Stops the reactor
        void stop();

    private:
        vector<struct pollfd> fds;
        vector<ReactorFunc> funcs;
        bool running;
};

#endif // REACTOR_HPP
