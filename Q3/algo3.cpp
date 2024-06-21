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

class GFG {
    public:

    // dfs function using vector of lists
    bool dfs_vector_of_lists(size_t curr, size_t des, vector<list<int>>& adj, vector<int>& vis) {
        stack<size_t> stack;
        stack.push(curr);
        while (!stack.empty()) {
            size_t node = stack.top();
            stack.pop();
            if (node == des) {
                return true;
            }
            vis[node] = (size_t)1;
            for (int neighbor : adj[node]) {
                if (!vis[static_cast<size_t>(neighbor)]) {
                    stack.push(static_cast<size_t>(neighbor));
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to destination using vector of lists
    bool isPath_vector_of_lists(int src, int des, vector<list<int>>& adj) {
        vector<int> vis(adj.size(), 0);
        return dfs_vector_of_lists(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
    }

    // Function to return all the strongly connected components of a graph using vector of lists
    vector<vector<int>> findSCC_vector_of_lists(int n, vector<vector<int>>& edges) {
        vector<vector<int>> ans;
        size_t num = size_t(n);
        vector<int> is_scc(num, 0);
        vector<list<int>> adj(num);

        for (size_t i = 0; i < edges.size(); i++) {
            adj[static_cast<size_t>(edges[i][0])].push_back(static_cast<int>(edges[i][1]));
        }

        // Filling the adj vector-lists
        for(size_t i=0; i<edges.size(); i++){
            for(size_t j=0; j<edges.size(); j++){
                if(edges[i][j]==(size_t)1){ // There is an edge
                    adj[i].push_back(j);
                }
            }
        }

        // Finding SCCs
        for (size_t i = 0; i < num; ++i) {
            if (!is_scc[i]) {
                vector<int> scc;
                stack<size_t> stack;
                stack.push(i);

                while (!stack.empty()) {
                    size_t node = stack.top();
                    stack.pop();

                    if (!is_scc[node]) {
                        is_scc[node] = 1;
                        scc.push_back(static_cast<int>(node+1));  // Convert back to 1-based index for output

                        for (int neighbor : adj[node]) {
                            if (!is_scc[(size_t)neighbor]) {
                                stack.push((size_t)neighbor);
                            }
                        }
                    }
                }

                ans.push_back(scc);
            }
        }  
        return ans;
    }

        void Newedge(size_t i,size_t j,vector<vector<int>>& adj){
            adj[i][j] = 1;
        }

        void Removeedge(size_t i,size_t j,vector<vector<int>>& adj){
            adj[i][j] = 0;
        }

};
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
        error("Error: setting socket option");
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

// Driver Code Starts

int main(int argc, char* argv[])
{
    GFG obj;
    size_t n = (size_t)atoi(argv[1]);
    int m = atoi(argv[2]);
    string commend = argv[3];
    size_t num1 = 0,num2 = 0;
    int input_sockfd = -1, output_sockfd = -1;
    int server_sockfd = -1, client_sockfd = -1;
    std::string exec_command;
    bool input_set = false, output_set = false, both_set = false;
    vector<vector<int> > edges(n,vector<int>(n, 0));
    int port = 9034;
    startTCPServer(port, server_sockfd, client_sockfd);
    input_sockfd = client_sockfd;
    input_set = true;
    printf("input socket = %d\n",input_sockfd);
    
    for(size_t i = 0;i < m;i++){
        cout << "enter edge: ";
        cin >> num1;
        cin >> num2;
        if(num1 > n || num2 > n){
            throw invalid_argument("invalid number\n");
        }
        edges[num1-1][num2-1] = 1;
    }
    vector<vector<int> > ans;
    if(commend == "Kosaraju"){
        ans = obj.findSCC_vector_of_lists(n, edges);
        cout << "Strongly Connected Components are:\n";
        for (auto x : ans) {
            for (auto y : x) {
                cout << y << " ";
            }
            cout << "\n";
        }
    }


    return 0;
}
