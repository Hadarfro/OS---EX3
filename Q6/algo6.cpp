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


// Driver Code Starts

int main1(){
    cout << "starting algo 4" << endl;
    string commend = "start";
    
    while(commend != "end"){
        GFG obj;
        size_t num1 = 0,num2 = 0;
        size_t n = 0, m = 0;
        cout << "enter the commend" << endl;
        cin >> commend;
        vector<vector<int> > edges(n,vector<int>(n, 0));

        if(commend == "Newgraph"){
            cout << "enter the n and m" << endl;
            cin >> n;
            cin >> m;
            vector<vector<int> > edges(n,vector<int>(n, 0));
            for(size_t i = 0;i < m;i++){
                cout << "enter edge: ";
                cin >> num1;
                cin >> num2;
                if(num1 > n || num2 > n){
                    throw invalid_argument("invalid number\n");
                }
                edges[num1-1][num2-1] = 1;
            }

            cout << "enter the commend kosaraju:" << endl;
            cin >> commend;
            if(commend == "kosaraju"){
                vector<vector<int> > ans; // For saving our graph for making add/remove edge
                ans = obj.findSCC_vector_of_lists(n, edges);
                cout << "Strongly Connected Components are:\n";
                for (auto x : ans) {
                    for (auto y : x) {
                        cout << y << " ";
                    }
                    cout << "\n";
                }
            }
        }

        cout << "enter the commend Newedge or continue:" << endl;
        cin >> commend;
        if(commend == "Newedge"){
            cout << "enter i and j: ";
            cin >> num1;
            cin >> num2;
            obj.Newedge(num1,num2,edges);
        }

        cout << "enter the commend Removeedge or continue:" << endl;
        cin >> commend;
        if(commend == "Removeedge"){
            cout << "enter i and j: ";
            cin >> num1;
            cin >> num2;
            obj.Removeedge(num1,num2,edges);
        }
    }

    return 0;
}