#include <iostream>
#include <unistd.h>
#include "Reactor.hpp"

using namespace std;

void onFdReadable(int fd) {
    char buffer[1024];
    int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        cout << "Read from fd " << fd << ": " << buffer << endl;
    } 
    else if (bytesRead == 0) {
        cout << "EOF on fd " << fd << endl;
    } 
    else {
        perror("read");
    }
}

int main() {
    Reactor reactor;

    // Add some file descriptors to the reactor, e.g., STDIN
    reactor.addFd(STDIN_FILENO, onFdReadable);

    // Run the reactor
    reactor.start();

    return 0;
}
