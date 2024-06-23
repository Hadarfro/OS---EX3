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

void startTCPServer(int port, int &server_sockfd, int &client_sockfd) {
    cout << "Starting TCP Server..." << endl;

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creating a socket
    if (server_sockfd < 0) {
        error("Error: opening socket");
    }
    std::cout << "Socket is created successfully!" << std::endl;

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
    std::cout << "bind() success!" << std::endl;

    listen(server_sockfd, 5);
    std::cout << "listening..." << std::endl;
    clilen = sizeof(cli_addr);
    client_sockfd = accept(server_sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (client_sockfd < 0) {
        error("Error: on accept");
    }
    std::cout << "accept() success!" << std::endl;
}

int main(int argc,char* argv[]){
    size_t num1 = 0,num2 = 0;
    int input_sockfd = -1, output_sockfd = -1;
    int server_sockfd = -1, client_sockfd = -1;
    //bool input_set = false, output_set = false, both_set = false;
    int port = 4050;
    startTCPServer(port, server_sockfd, client_sockfd);
    input_sockfd = client_sockfd;
    //input_set = true;
    string exec_command = argv[1];
    printf("input socket = %d\n",input_sockfd);
    if (exec_command.empty()) {
        cerr << "No execution command provided." << endl;
        return 1;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        
            fflush(stdin);
            close(0);
            if (dup2(input_sockfd, STDIN_FILENO) < 0) {
                cerr << "Failed to redirect stdin" << endl;
                exit(1);
            }
            close(input_sockfd);
        
        // if (output_set || both_set) {
        //     fflush(stdout);
        //     if (dup2(output_sockfd, STDOUT_FILENO) < 0) {
        //         cerr << "Failed to redirect stdout" << endl;
        //         exit(1);
        //     }
        //     close(output_sockfd);
        // // }
        execlp("/bin/sh", "sh", "-c", exec_command.c_str(), (char *)0);
        cerr << "Failed to execute " << exec_command << endl;
        exit(1);
    } 
    else if (pid > 0) {
        // Parent process
       // if (input_set || both_set) {
            fflush(stdin);
            close(server_sockfd);
            close(input_sockfd);
       // }
        //if (output_set || both_set) {
            fflush(stdout);
            close(output_sockfd);
       // }
        waitpid(pid, NULL, 0);
    } 
    else {
        cerr << "Failed to fork" << endl;
        exit(1);
    }
    return 0;
}