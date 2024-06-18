#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

using boost::asio::ip::tcp;
using namespace std;

void dfs(int v, vector<vector<int>>& graph, vector<bool>& visited, stack<int>& Stack) {
    visited[v] = true;
    for (int u : graph[v]) {
        if (!visited[u]) {
            dfs(u, graph, visited, Stack);
        }
    }
    Stack.push(v);
}

void reverseGraph(int n, vector<vector<int>>& graph, vector<vector<int>>& reversedGraph) {
    for (int v = 0; v < n; ++v) {
        for (int u : graph[v]) {
            reversedGraph[u].push_back(v);
        }
    }
}

void fillOrder(int n, vector<vector<int>>& graph, vector<bool>& visited, stack<int>& Stack) {
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i, graph, visited, Stack);
        }
    }
}

void dfsComponent(int v, vector<vector<int>>& graph, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int u : graph[v]) {
        if (!visited[u]) {
            dfsComponent(u, graph, visited, component);
        }
    }
}

vector<vector<int>> kosarajuSCC(int n, vector<pair<int, int>>& edges) {
    vector<vector<int>> graph(n);
    vector<vector<int>> reversedGraph(n);
    vector<bool> visited(n, false);
    stack<int> Stack;

    for (auto& edge : edges) {
        graph[edge.first - 1].push_back(edge.second - 1); // Zero-indexed
    }

    fillOrder(n, graph, visited, Stack);
    reverseGraph(n, graph, reversedGraph);

    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> sccList;

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();

        if (!visited[v]) {
            vector<int> component;
            dfsComponent(v, reversedGraph, visited, component);
            sccList.push_back(component);
        }
    }

    return sccList;
}

void handle_client(tcp::socket socket) {
    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        istream is(&buffer);
        
        int n, m;
        is >> n >> m;
        vector<pair<int, int>> edges(m);

        for (int i = 0; i < m; ++i) {
            int u, v;
            is >> u >> v;
            edges[i] = {u, v};
        }

        vector<vector<int>> scc = kosarajuSCC(n, edges);

        ostringstream response;
        for (const auto& component : scc) {
            for (int v : component) {
                response << (v + 1) << " "; // Convert back to 1-indexed
            }
            response << "\n";
        }

        boost::asio::write(socket, boost::asio::buffer(response.str()));
    } catch (std::exception& e) {
        cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void start_server(unsigned short port) {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        // Set socket options
        boost::asio::socket_base::reuse_address option(true);
        acceptor.set_option(option);

        cout << "Server is running on port " << port << "\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            thread(handle_client, std::move(socket)).detach();
        }
    } 
    catch (std::exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }
}

void send_graph_to_server(const string& host, unsigned short port, const string& graph_data) {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(host, to_string(port));
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        boost::asio::write(socket, boost::asio::buffer(graph_data));

        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\n");

        istream is(&response);
        string line;
        while (getline(is, line)) {
            cout << line << endl;
        }
    } catch (std::exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    thread server_thread([]() {
        start_server(4050);
    });

    this_thread::sleep_for(chrono::seconds(1)); // Wait for the server to start

    if (argc == 2) {
        string graph_data = "5 5\n1 2\n2 3\n3 1\n3 4\n4 5\n";
        send_graph_to_server("127.0.0.1", 4050, graph_data);
    }

    server_thread.join();
    return 0;
}
