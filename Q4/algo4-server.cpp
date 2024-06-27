#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  // Needed for addrinfo
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <csignal>
#include <sys/wait.h>

#define PORT "9034"  // Port to listen on
#define BACKLOG 10   // Number of pending connections queue will hold

using namespace std;

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s) {
    (void)s; // Quiet unused variable warning
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int sockfd, new_fd;  // Listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;  // Connector's address information
    socklen_t sin_size;
    struct sigaction sa;

    int yes = 1;
    char s[INET6_ADDRSTRLEN];
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
    sa.sa_handler = sigchld_handler;  // Reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    cout << "server: waiting for connections..." << endl;

    while (true) {  // Main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        cout << "server: got connection from " << s << endl;

        if (!fork()) { // This is the child process
            close(sockfd);  // Child doesn't need the listener

            // Redirect stdout and stderr to the client socket
            if (dup2(new_fd, STDOUT_FILENO) == -1 || dup2(new_fd, STDERR_FILENO) == -1) {
                perror("dup2");
                close(new_fd);
                exit(1);
            }

            // Redirect stdout and stderr to the client socket
            if (dup2(new_fd, STDIN_FILENO) == -1) {
                perror("dup2");
                close(new_fd);
                exit(1);
            }

            if (execlp("/home/mayrozen/Downloads/study/OS/OS---EX3-main/Q4/algo4", "algo4", (char *)0) == -1) {
                perror("execlp");
                close(new_fd);
                exit(1);
            }

            close(new_fd);
            exit(0);
        }

        close(new_fd);  // Parent doesn't need this
    }

    return 0;
}
