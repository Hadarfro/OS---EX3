#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void startTCPServer(int port, int &server_sockfd) {
    cout << "Starting TCP Server..." << endl;

    struct sockaddr_in serv_addr;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creating a socket
    if (server_sockfd < 0) {
        error("Error: opening socket");
    }
    cout << "Socket is created successfully!" << endl;

    // Set SO_REUSEADDR socket option
    int opt = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(server_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Error: on binding");
    }
    cout << "bind() success!" << endl;

    listen(server_sockfd, 5);
    cout << "listening..." << endl;
}

int main(int argc, char* argv[]) {
    int server_sockfd = -1, client_sockfd = -1;
    int port = 4050;
    startTCPServer(port, server_sockfd);

    while (true) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (client_sockfd < 0) {
            error("Error: on accept");
        }
        cout << "accept() success! client_sockfd = " << client_sockfd << endl;

        // Create pipes for stdin and stdout redirection
        int pipe_in[2];
        int pipe_out[2];

        if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
            error("Error: creating pipes");
        }

        pid_t pid = fork();
        cout << "fork() success!" << endl;

        // Child process
        if (pid == 0) {
            cout << "pid = 0" << endl;
            close(server_sockfd); // Close the server socket in the child process
            
            // Redirect stdin to pipe_in[0]
            close(pipe_in[1]); // Close write end of pipe_in
            fflush(stdin);
            if (dup2(pipe_in[0], STDIN_FILENO) < 0) {
                cerr << "Failed to redirect stdin" << endl;
                exit(1);
            }
            close(pipe_in[0]);
            
            // Redirect stdout and stderr to pipe_out[1]
            close(pipe_out[0]); // Close read end of pipe_out
            fflush(stdout);
            if (dup2(client_sockfd, STDOUT_FILENO) < 0 ) {//removed the part os stderr!!!!
                cerr << "Failed to redirect stdout or stderr" << endl;
                exit(1);
            }
            
            if (execlp("/home/hadarfro/Desktop/OS---EX3/Q3/algo3", "algo3", (char *)0) == -1) {
                perror("execlp failed");
            }
            cerr << "Failed to execute " << argv[0] << endl;
            exit(1);
        } 
        else if (pid > 0) { // Parent process
            cout << "pid > 0" << endl;
            close(pipe_in[0]); // Close read end of pipe_in
            close(pipe_out[1]); // Close write end of pipe_out

            // Read from client socket and write to pipe_in[1]
            char buffer[256];
            int n;

            if ((n = read(client_sockfd, buffer, 255)) < 0) {
                error("Error: reading from client socket");
            } 

            while ((n = read(client_sockfd, buffer, 255)) > 0) {
                if (write(pipe_in[1], buffer, (size_t)n) < 0) {
                    error("Error: writing to pipe_in");
                }
            }
            close(pipe_in[1]); // Close write end of pipe_in

            // Read from pipe_out[0] and write to client socket
            while ((n = read(pipe_out[0], buffer, 255)) > 0) {
                if (write(client_sockfd, buffer, (size_t)n) < 0) {
                    error("Error: writing to client socket");
                }
            }
            close(pipe_out[0]); // Close read end of pipe_out

            waitpid(pid, NULL, 0);
            close(client_sockfd);
        } else {
            cerr << "Failed to fork()" << endl;
            exit(1);
        }
    }

    return 0;
}
