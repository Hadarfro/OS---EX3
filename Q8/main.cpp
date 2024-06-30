#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "Proactor.hpp"

void *clientHandler(int sockfd) {
    char buffer[1024];
    int bytesRead = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
    close(sockfd);
    return nullptr;
}

int main() {
    // Create a TCP socket
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) {
        perror("socket");
        return -1;
    }

    // Set up the server address
    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    serverAddr.sin_port = htons(9034); // Port 9034

    // Bind the socket to the address and port
    if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        close(listenSock);
        return -1;
    }

    // Start listening for incoming connections
    if (listen(listenSock, 5) < 0) {
        perror("listen");
        close(listenSock);
        return -1;
    }

    // Initialize the Proactor
    Proactor proactor(listenSock, clientHandler);
    pthread_t tid = proactor.start();

    if (tid == 0) {
        std::cerr << "Failed to start Proactor." << std::endl;
        close(listenSock);
        return -1;
    }

    std::cout << "Proactor started, listening on port 9034." << std::endl;

    // Wait for the Proactor thread to finish
    pthread_join(tid, nullptr);

    // Clean up
    close(listenSock);
    return 0;
}
