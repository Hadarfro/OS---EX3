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
            vis[node] = (size_t)1;
            for (size_t neighbor = 0; neighbor < adj[node].size(); neighbor++) {
                if (!vis[static_cast<size_t>(adj[node][neighbor])]) {
                    stack.push_back(static_cast<size_t>(adj[node][neighbor]));
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
        vector<vector<int>> ans;
        size_t num = (size_t)(n + 1);
        vector<int> is_scc(num, 0);
        vector<vector<int>> adj(num);
        size_t j = 0;
        size_t k = 0;
        for (size_t i = 0; i < a.size(); i++) {
            j = (size_t)a[i][0];
            k = (size_t)a[i][1];
            adj[j].push_back(k);
        }

        for (size_t i = 1; i <= static_cast<size_t>(n); i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(static_cast<int>(i));
                for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {
                    if (!is_scc[j] && isPath_list(static_cast<int>(i), static_cast<int>(j), adj)
                        && isPath_list(static_cast<int>(j), static_cast<int>(i), adj)) {
                        is_scc[j] = 1;
                        scc.push_back(static_cast<int>(j));
                    }
                }
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
            vis[node] = (size_t)1;
            for (size_t neighbor = 0; neighbor < adj[node].size(); neighbor++) {
                if (!vis[static_cast<size_t>(adj[node][neighbor])]) {
                    stack.push_back(static_cast<size_t>(adj[node][neighbor]));
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
        vector<vector<int>> ans;
        size_t num = (size_t)(n + 1);
        vector<int> is_scc(num, 0);
        vector<vector<int>> adj(num);
        size_t j = 0;
        size_t k = 0;
        for (size_t i = 0; i < a.size(); i++) {
            j = (size_t)a[i][0];
            k = (size_t)a[i][1];
            adj[j].push_back(k);
        }

        for (size_t i = 1; i <= static_cast<size_t>(n); i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(static_cast<int>(i));
                for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {
                    if (!is_scc[j] && isPath_deque(static_cast<int>(i), static_cast<int>(j), adj)
                        && isPath_deque(static_cast<int>(j), static_cast<int>(i), adj)) {
                        is_scc[j] = 1;
                        scc.push_back(static_cast<int>(j));
                    }
                }
                ans.push_back(scc);
            }
        }
        return ans;
    }

    // dfs function using adjacency matrix
       // dfs function using adjacency matrix
    bool dfs_matrix(size_t curr, size_t des, vector<vector<int>>& adj, vector<int>& vis) {
        stack<size_t> stack;
        stack.push(curr);
        while (!stack.empty()) {
            size_t node = stack.top();
            stack.pop();
            if (node == des) {
                return true;
            }
            vis[node] = 1;
            for (size_t neighbor = 0; neighbor < adj.size(); neighbor++) {
                if (adj[node][neighbor] && !vis[neighbor]) {
                    stack.push(neighbor);
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to destination using adjacency matrix
    bool isPath_matrix(int src, int des, vector<vector<int>>& adj) {
        vector<int> vis(adj.size(), 0);
        return dfs_matrix(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
    }

    // Function to return all the strongly connected components of a graph using adjacency matrix
    vector<vector<int>> findSCC_matrix(int n, vector<vector<int>>& a) {
        vector<vector<int>> ans;
        size_t num = (size_t)(n + 1);
        vector<int> is_scc(num, 0);
        vector<vector<int>> adj(num, vector<int>(num, 0));
        for (size_t i = 0; i < a.size(); i++) {
            adj[static_cast<size_t>(a[i][0])][static_cast<size_t>(a[i][1])] = static_cast<size_t>(1);
        }

        for (size_t i = 1; i <= static_cast<size_t>(n); i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(static_cast<int>(i));
                for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {
                    if (!is_scc[j] && isPath_matrix(static_cast<int>(i), static_cast<int>(j), adj)
                        && isPath_matrix(static_cast<int>(j), static_cast<int>(i), adj)) {
                        is_scc[j] = 1;
                        scc.push_back(static_cast<int>(j));
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
    vector<vector<int>> findSCC_vector_of_lists(int n, vector<vector<int>>& a) {
        vector<vector<int>> ans;
        size_t num = size_t(n + 1);
        vector<int> is_scc(num, 0);
        vector<list<int>> adj(num);
        for (size_t i = 0; i < a.size(); i++) {
            adj[static_cast<size_t>(a[i][0])].push_back(static_cast<int>(a[i][1]));
        }

        for (size_t i = 1; i <= static_cast<size_t>(n); i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(static_cast<int>(i));
                for (size_t j = i + 1; j <= static_cast<size_t>(n); j++) {
                    if (!is_scc[j] && isPath_vector_of_lists(static_cast<int>(i), static_cast<int>(j), adj)
                        && isPath_vector_of_lists(static_cast<int>(j), static_cast<int>(i), adj)) {
                        is_scc[j] = 1;
                        scc.push_back(static_cast<int>(j));
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
    size_t num1 = 0, num2 = 0;
    vector<vector<int>> edges;

    // Reading edges
    for (size_t i = 0; i < m; i++) {
        cout << "Enter edge: ";
        cin >> num1 >> num2;
        if (num1 > n || num2 > n) {
            throw invalid_argument("Invalid number\n");
        }
        edges.push_back({(int)num1 - 1, (int)num2 - 1});
    }

    vector<vector<int>> ans;
    if (flag == "-d") {
        ans = obj.findSCC_deque(n, edges);
    } else if (flag == "-l") {
        ans = obj.findSCC_list(n, edges);
    } else if (flag == "-m") {
        ans = obj.findSCC_matrix(n, edges);
    } else if (flag == "-v") {
        ans = obj.findSCC_vector_of_lists(n, edges);
    }

    cout << "Strongly Connected Components are:\n";
    for (auto x : ans) {
        for (auto y : x) {
            cout << y << " ";
        }
        cout << "\n";
    }

    return 0;
}