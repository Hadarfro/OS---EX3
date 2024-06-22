#include "doctest.h"
#include "algo2-2.cpp"
using namespace std;

TEST_CASE("Test isPath_list with no edges") {
    GFG obj;
    vector<vector<int>> adj(5);
    CHECK(obj.isPath_list(0, 1, adj) == false);
}

TEST_CASE("Test isPath_list with direct edge") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    CHECK(obj.isPath_list(0, 1, adj) == true);
}

TEST_CASE("Test isPath_list with indirect path") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    adj[1].push_back(2);
    CHECK(obj.isPath_list(0, 2, adj) == true);
}

TEST_CASE("Test isPath_list with no path") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    adj[2].push_back(3);
    CHECK(obj.isPath_list(0, 3, adj) == false);
}

TEST_CASE("Test findSCC_list with no edges") {
    GFG obj;
    vector<vector<int>> edges;
    auto scc = obj.findSCC_list(5, edges);
    CHECK(scc.size() == 5);
}

TEST_CASE("Test findSCC_list with single SCC") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1}, {1, 2}, {2, 0}};
    auto scc = obj.findSCC_list(3, edges);
    CHECK(scc.size() == 1);
}

TEST_CASE("Test findSCC_list with multiple SCCs") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1}, {1, 0}, {2, 3}, {3, 2}};
    auto scc = obj.findSCC_list(4, edges);
    CHECK(scc.size() == 2);
}

TEST_CASE("Test isPath_deque with no edges") {
    GFG obj;
    vector<vector<int>> adj(5);
    CHECK(obj.isPath_deque(0, 1, adj) == false);
}

TEST_CASE("Test isPath_deque with direct edge") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    CHECK(obj.isPath_deque(0, 1, adj) == true);
}

TEST_CASE("Test isPath_deque with indirect path") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    adj[1].push_back(2);
    CHECK(obj.isPath_deque(0, 2, adj) == true);
}

TEST_CASE("Test isPath_deque with no path") {
    GFG obj;
    vector<vector<int>> adj(5);
    adj[0].push_back(1);
    adj[2].push_back(3);
    CHECK(obj.isPath_deque(0, 3, adj) == false);
}

TEST_CASE("Test findSCC_deque with no edges") {
    GFG obj;
    vector<vector<int>> edges;
    auto scc = obj.findSCC_deque(5, edges);
    CHECK(scc.size() == 5);
}

TEST_CASE("Test findSCC_deque with single SCC") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1}, {1, 2}, {2, 0}};
    auto scc = obj.findSCC_deque(3, edges);
    CHECK(scc.size() == 1);
}

TEST_CASE("Test findSCC_deque with multiple SCCs") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1}, {1, 0}, {2, 3}, {3, 2}};
    auto scc = obj.findSCC_deque(4, edges);
    CHECK(scc.size() == 2);
}

TEST_CASE("Test isPath_matrix with no edges") {
    GFG obj;
    vector<vector<int>> adj(5, vector<int>(5, 0));
    CHECK(obj.isPath_matrix(0, 1, adj) == false);
}

TEST_CASE("Test isPath_matrix with direct edge") {
    GFG obj;
    vector<vector<int>> adj(5, vector<int>(5, 0));
    adj[0][1] = 1;
    CHECK(obj.isPath_matrix(0, 1, adj) == true);
}

TEST_CASE("Test isPath_matrix with indirect path") {
    GFG obj;
    vector<vector<int>> adj(5, vector<int>(5, 0));
    adj[0][1] = 1;
    adj[1][2] = 1;
    CHECK(obj.isPath_matrix(0, 2, adj) == true);
}

TEST_CASE("Test isPath_matrix with no path") {
    GFG obj;
    vector<vector<int>> adj(5, vector<int>(5, 0));
    adj[0][1] = 1;
    adj[2][3] = 1;
    CHECK(obj.isPath_matrix(0, 3, adj) == false);
}

TEST_CASE("Test findSCC_matrix with no edges") {
    GFG obj;
    vector<vector<int>> edges(5, vector<int>(5, 0));
    auto scc = obj.findSCC_matrix(5, edges);
    CHECK(scc.size() == 5);
}

TEST_CASE("Test findSCC_matrix with single SCC") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}};
    auto scc = obj.findSCC_matrix(3, edges);
    CHECK(scc.size() == 1);
}

TEST_CASE("Test findSCC_matrix with multiple SCCs") {
    GFG obj;
    vector<vector<int>> edges = {{0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}};
    auto scc = obj.findSCC_matrix(4, edges);
    CHECK(scc.size() == 2);
}
