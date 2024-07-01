#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <vector>
#include <mutex>
#include <cstdio>

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

void sigchld_handler(int s) {
    (void)s; // Quiet unused variable warning
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char client_message[2000];
    int read_size;

    // Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
        // Null terminate the string
        client_message[read_size] = '\0';

        // Create a pipe
        int pipefd_in[2];
        int pipefd_out[2];
        if (pipe(pipefd_in) == -1 || pipe(pipefd_out) == -1) {
            perror("pipe");
            break;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) { // Child process
            close(pipefd_in[1]); // Close unused write end
            dup2(pipefd_in[0], STDIN_FILENO); // Redirect stdin to pipefd_in read end
            close(pipefd_out[0]); // Close unused read end
            dup2(pipefd_out[1], STDOUT_FILENO); // Redirect stdout to pipefd_out write end

            execl("/home/mayrozen/Downloads/study/OS/OS---EX3-main/Q7/algo7", "algo7", (char *)NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        } else { // Parent process
            close(pipefd_in[0]); // Close unused read end
            write(pipefd_in[1], client_message, strlen(client_message)); // Write client message to pipe
            close(pipefd_in[1]); // Close write end after writing

            char algo_output[2000];
            close(pipefd_out[1]); // Close unused write end
            read(pipefd_out[0], algo_output, sizeof(algo_output)); // Read output from algo7
            close(pipefd_out[0]); // Close read end after reading

            // Send algo7's output back to the client
            write(sock, algo_output, strlen(algo_output));
        }

        wait(NULL); // Wait for the child process to finish
    }

    if (read_size == 0) {
        cout << "Client disconnected" << endl;
    } else if (read_size == -1) {
        perror("recv failed");
    }

    // Free the socket descriptor
    close(sock);
    free(socket_desc);

    return NULL;
}

int main() {
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;

    int yes = 1;
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

    freeaddrinfo(servinfo);

    if (p == NULL) {
        cerr << "server: failed to bind" << endl;
        return 2;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // Setup signal handler to reap all dead processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    cout << "server: waiting for connections..." << endl;

    while (true) {
        // Accept connection from an incoming client
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        cout << "server: got connection from " << s << endl;

        // Create a new thread to handle the client connection
        pthread_t thread_id;
        int *pclient = new int;
        *pclient = new_fd;
        if (pthread_create(&thread_id, NULL, connection_handler, (void*)pclient) < 0) {
            perror("could not create thread");
            return 1;
        }

        // Detach the thread so it can clean up after itself
        pthread_detach(thread_id);
    }

    return 0;
}
