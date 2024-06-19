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

};

// Driver Code Starts

int main(int argc, char* argv[])
{
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
    }

    else if(flag == "-l"){
        ans = obj.findSCC_list(n, edges);
        cout << "Strongly Connected Components are:\n";
        for (auto x : ans) {
            for (auto y : x) {
                cout << y << " ";
            }
            cout << "\n";
        }
    }
}
