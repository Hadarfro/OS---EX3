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
#include "Proactor.hpp"

#define PORT "9034"
#define BACKLOG 10

using namespace std;

// Function to handle client input
void *handleClientInput(int client_fd) {
    // Example: Fork a process to handle client interaction
    pid_t pid = fork();
    if (pid == 0) { // Child process
        // Redirect stdin, stdout, stderr to client socket
        if (dup2(client_fd, STDIN_FILENO) == -1 || 
            dup2(client_fd, STDOUT_FILENO) == -1 || 
            dup2(client_fd, STDERR_FILENO) == -1) {
            perror("dup2");
            close(client_fd);
            exit(1);
        }

        // Execute the server-side process or program
        if (execlp("/home/mayrozen/Downloads/study/OS/OS---EX3-main/Q9/algo9", "algo9", (char *)0) == -1) {
            perror("execlp");
            close(client_fd);
            exit(1);
        }

        // Close the client socket in the child process
        close(client_fd);
        exit(0);
    }

    // Close the client socket in the parent process
    close(client_fd);
    return nullptr;
}

int main() {
    int sockfd;  // Listen on sock_fd
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;

    int yes = 1;
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

    // Create a Proactor instance
    Proactor proactor(sockfd, handleClientInput);
    pthread_t tid = proactor.start();
    
    // Wait for termination signal
    cout << "Press Enter to terminate the server..." << endl;
    cin.get();

    // Stop the Proactor (terminate all client handling threads)
    proactor.stopAll();

    close(sockfd);  // Close the server socket

    return 0;
}
