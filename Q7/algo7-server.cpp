#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <mutex>

#define PORT "9034"
#define BACKLOG 10

using namespace std;

vector<vector<int>> graph; // Shared graph data structure
mutex graph_mutex; // Mutex to protect the graph

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void* handle_client(void* arg) {
    int new_fd = *(int*)arg;
    delete (int*)arg;

    // Redirect stdout, stdin, and stderr to the client socket
    if (dup2(new_fd, STDOUT_FILENO) == -1 || dup2(new_fd, STDERR_FILENO) == -1 || dup2(new_fd, STDIN_FILENO) == -1) {
        perror("dup2");
        close(new_fd);
        pthread_exit(NULL);
    }

    // Execute algo7
    if (execlp("/home/mayrozen/Downloads/study/OS/OS---EX3-main/Q7/algo7", "algo7", (char *)0) == -1) {
        perror("execlp");
        close(new_fd);
        pthread_exit(NULL);
    }

    close(new_fd);
    pthread_exit(NULL);
}

int main() {
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        int yes = 1;
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

    freeaddrinfo(servinfo);

    if (p == NULL) {
        cerr << "server: failed to bind" << endl;
        return 2;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    cout << "server: waiting for connections..." << endl;

    while (true) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        cout << "server: got connection from " << s << endl;

        pthread_t thread_id;
        int *pclient = new int;
        *pclient = new_fd;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)pclient) < 0) {
            perror("could not create thread");
            return 1;
        }

        pthread_detach(thread_id);  // Detach the thread so it cleans up after itself
    }

    close(sockfd);

    return 0;
}
