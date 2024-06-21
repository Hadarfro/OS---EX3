#include <bits/stdc++.h>
using namespace std;

class GFG {
public:
    // dfs function using list
        bool dfs_list(size_t curr, size_t des, vector<vector<int>>& adj, vector<int>& vis) {
            list<size_t> stack;
            stack.push_back(curr);
            while (!stack.empty()) {
                size_t node = stack.back();
                stack.pop_back();
                if (node == des) {
                    return true;
                }
                vis[node] = 1;
                for (size_t neighbor = 0;neighbor < adj[node].size();neighbor++) {
                    if (!vis[neighbor]) {
                        stack.push_back(neighbor);
                    }
                }
            }
            return false;
        }

        // To tell whether there is path from source to destination using list
        bool isPath_list(int src, int des, vector<vector<int>>& adj) {
            vector<int> vis(adj.size(), 0);
            return dfs_list(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
        }

        // Function to return all the strongly connected components of a graph using list
        vector<vector<int>> findSCC_list(int n, vector<vector<int>>& a) {
            // Stores all the strongly connected components.
            vector<vector<int> > ans;
            size_t num = (size_t)(n + 1);
            // Stores whether a vertex is a part of any Strongly
            // Connected Component
            vector<int> is_scc(num, 0);

            vector<vector<int> > adj(num);
            size_t j = 0;
            size_t k = 0;
            for (size_t i = 0; i < a.size(); i++) {
                j = (size_t)a[i][0];
                k = (size_t)a[i][1];
                adj[j].push_back(k);
            }

            // Traversing all the vertices
            for (size_t i = 1; i <= static_cast<size_t>(n); i++) {

                if (!is_scc[i]) {

                    // If a vertex i is not a part of any SCC
                    // insert it into a new SCC list and check
                    // for other vertices whether they can be
                    // part of this list.
                    vector<int> scc;
                    scc.push_back(static_cast<int>(i));

                    for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {

                        // If there is a path from vertex i to
                        // vertex j and vice versa put vertex j
                        // into the current SCC list.
                        if (!is_scc[j] && isPath_list(static_cast<int>(i), static_cast<int>(j), adj)
                            && (isPath_list(static_cast<int>(j), static_cast<int>(i),a))) {
                            is_scc[j] = 1;
                            scc.push_back(static_cast<int>(j));
                        }
                    }

                    // Insert the SCC containing vertex i into
                    // the final list.
                    ans.push_back(scc);
                }
            }
            return ans;
        }

       // dfs function using deque
        bool dfs_deque(size_t curr, size_t des, vector<vector<int>>& adj, vector<int>& vis) {
            deque<size_t> stack;
            stack.push_back(curr);
            while (!stack.empty()) {
                size_t node = stack.back();
                stack.pop_back();
                if (node == des) {
                    return true;
                }
                vis[node] = 1;
                for (size_t x = 0;x < adj[node].size();x++){
                    if (!vis[x]) {
                        stack.push_back(x);
                    }
                }
            }
            return false;
        }

        // To tell whether there is path from source to destination using deque
        bool isPath_deque(int src, int des, vector<vector<int>>& adj) {
            vector<int> vis(adj.size(), 0);
            return dfs_deque(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
        }

        // Function to return all the strongly connected components of a graph using deque
        vector<vector<int>> findSCC_deque(int n, vector<vector<int>>& a) {
            // Stores all the strongly connected components.
            vector<vector<int> > ans;
            size_t num = (size_t)(n + 1);
            // Stores whether a vertex is a part of any Strongly
            // Connected Component
            vector<int> is_scc(num, 0);

            vector<vector<int> > adj(num);
            size_t j = 0;
            size_t k = 0;
            for (size_t i = 0; i < a.size(); i++) {
                j = (size_t)a[i][0];
                k = (size_t)a[i][1];
                adj[j].push_back(k);
            }

            // Traversing all the vertices
            for (size_t i = 1; i <= static_cast<size_t>(n); i++) {

                if (!is_scc[i]) {

                    // If a vertex i is not a part of any SCC
                    // insert it into a new SCC list and check
                    // for other vertices whether they can be
                    // part of this list.
                    vector<int> scc;
                    scc.push_back(static_cast<int>(i));

                    for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {

                        // If there is a path from vertex i to
                        // vertex j and vice versa put vertex j
                        // into the current SCC list.
                        if (!is_scc[j] && isPath_deque(static_cast<int>(i), static_cast<int>(j), adj)
                            && (isPath_deque(static_cast<int>(j), static_cast<int>(i),a))) {
                            is_scc[j] = 1;
                            scc.push_back(static_cast<int>(j));
                        }
                    }

                    // Insert the SCC containing vertex i into
                    // the final list.
                    ans.push_back(scc);
                }
            }
            return ans;
        }

    // DFS function using adjacency matrix
bool dfs_matrix(size_t curr, size_t des, const vector<vector<int>>& adj, vector<int>& vis) {
    stack<size_t> stack;
    stack.push(curr);
    while (!stack.empty()) {
        size_t node = stack.top();
        stack.pop();
        if (node == des) {
            return true;
        }
        if (!vis[node]) {
            vis[node] = 1;
            for (size_t neighbor = 0; neighbor < adj.size(); ++neighbor) {
                if (adj[node][neighbor] && !vis[neighbor]) {
                    stack.push(neighbor);
                }
            }
        }
    }
    return false;
}

// Function to determine if there is a path from src to des using adjacency matrix
bool isPath_matrix(int src, int des, const vector<vector<int>>& adj) {
    vector<int> vis(adj.size(), 0);
    return dfs_matrix(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
}

void printAdjMatrix(const vector<vector<int>>& adj) {
    for (const auto& row : adj) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}


// Function to find all strongly connected components (SCCs) of a graph using adjacency matrix
vector<vector<int>> findSCC_matrix(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> ans;
    size_t num = static_cast<size_t>(n);
    vector<int> is_scc(num, 0);
    vector<vector<int>> adj(num, vector<int>(num, 0));

    // Copy edges
    for(size_t i=0; i<edges.size(); i++){
        for(size_t j=0; j<edges.size(); j++){
            adj[i][j]=edges[i][j];
        }
    }

    printAdjMatrix(adj);

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
                    scc.push_back(static_cast<int>(node + 1));  // Convert back to 1-based index for output

                    for (size_t neighbor = 0; neighbor < num; ++neighbor) {
                        if (adj[node][neighbor] && !is_scc[neighbor]) {
                            stack.push(neighbor);
                        }
                    }
                }
            }

            ans.push_back(scc);
        }
    }
    return ans;
}


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
};

int main(int argc, char* argv[]) {
    GFG obj;
    size_t n = (size_t)atoi(argv[1]);
    int m = atoi(argv[2]);
    string flag = argv[3];
    size_t num1 = 0,num2 = 0;
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
    vector<vector<int> > ans;
    if(flag == "-d"){
        ans = obj.findSCC_deque(n, edges);
        cout << "Strongly Connected Components are:\n";
        for (auto x : ans) {
            for (auto y : x) {
                cout << y << " ";
            }
            cout << "\n";
        }
    }else if (flag == "-l") {
        ans = obj.findSCC_list(n, edges);
        cout << "Strongly Connected Components are:\n";
        for (auto x : ans) {
            for (auto y : x) {
                cout << y << " ";
            }
            cout << "\n";
        }
    } else if (flag == "-m") {
        ans = obj.findSCC_matrix(n, edges);
        cout << "Strongly Connected Components are:\n";
        for (auto x : ans) {
            for (auto y : x) {
                cout << y << " ";
            }
            cout << "\n";
        }
    } else if (flag == "-v") {
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