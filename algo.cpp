#include <bits/stdc++.h>
using namespace std;

class GFG {
public:
    // dfs Function to reach destination
    bool dfs(size_t curr, size_t des, vector<vector<int> >& adj, vector<int>& vis) {

        // If curr node is destination return true
        if (curr == des) {
            return true;
        }
        vis[curr] = 1;
        for (int x : adj[curr]) {
            if (!vis[x]) {
                if (dfs(x, des, adj, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to
    // destination
    bool isPath(int src, int des, vector<vector<int> >& adj)
    {
        vector<int> vis(adj.size(), 0);
        return dfs(static_cast<size_t>(src), static_cast<size_t>(des), adj, vis);
    }

    // Function to return all the strongly connected
    // component of a graph.
    vector<vector<int> > findSCC(int n, vector<vector<int> >& a) {
        // Stores all the strongly connected components.
        vector<vector<int> > ans;

        // Stores whether a vertex is a part of any Strongly
        // Connected Component
        vector<int> is_scc(n + 1, 0);

        vector<vector<int> > adj(n + 1);

        for (size_t i = 0; i < a.size(); i++) {
            adj[a[i][0]].push_back(a[i][1]);
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
                    if (!is_scc[j] && isPath(static_cast<int>(i), static_cast<int>(j), adj)
                        && (isPath(static_cast<int>(j), static_cast<int>(i)))) {
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

int main()
{
    GFG obj;
    int V = 5;
    vector<vector<int> > edges{
        { 1, 2 }, { 2, 3 }, { 1, 3 }, { 4, 3 }, { 5, 4 }
    };
    vector<vector<int> > ans = obj.findSCC(V, edges);
    cout << "Strongly Connected Components are:\n";
    for (auto x : ans) {
        for (auto y : x) {
            cout << y << " ";
        }
        cout << "\n";
    }
}
