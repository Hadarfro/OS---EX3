#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <csignal>
#include <sys/wait.h>
#include "Reactor.hpp"

#define PORT "9034"
#define BACKLOG 10

using namespace std;

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handleClientInput(int client_fd) {
    char buf[256];
    int nbytes = recv(client_fd, buf, sizeof(buf) - 1, 0);
    if (nbytes <= 0) {
        if (nbytes == 0) {
            cout << "Socket " << client_fd << " hung up" << endl;
        } else {
            perror("recv");
        }
        close(client_fd);
    } else {
        buf[nbytes] = '\0';
        cout << "Received: " << buf << " from socket " << client_fd << endl;
        // Process command or data received
    }
}

int main() {
    int sockfd;  // Listen on sock_fd
    struct addrinfo hints, *servinfo, *p;
    //struct sockaddr_storage their_addr;  // Connector's address information
    //socklen_t sin_size;
    struct sigaction sa;

    int yes = 1;
    //char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // Use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // Fill in my IP for me

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
        return 1;
    }

    // Loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);  // All done with this structure

    if (p == NULL) {
        cerr << "server: failed to bind" << endl;
        return 2;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // Set up the signal handler to reap all dead processes
    sa.sa_handler = SIG_IGN;  // Ignore the signal
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    cout << "server: waiting for connections..." << endl;

    // Create a Reactor instance
    Reactor* reactor = static_cast<Reactor*>(startReactor());

    // Function to handle new connections
    auto acceptClient = [reactor](int listener) {
        struct sockaddr_in clientaddr;
        socklen_t addrlen = sizeof(clientaddr);
        int new_fd = accept(listener, (struct sockaddr*)&clientaddr, &addrlen);
        if (new_fd < 0) {
            perror("accept");
        } else {
            addFdToReactor(reactor, new_fd, handleClientInput);
            cout << "New connection from " << inet_ntoa(clientaddr.sin_addr) << " on socket " << new_fd << endl;
        }
    };

    // Add the server socket to the Reactor
    addFdToReactor(reactor, sockfd, acceptClient);

    // Start the reactor
    reactor->start();

    close(sockfd);  // Close the server socket
    delete reactor; // Clean up the Reactor instance

    return 0;
}

