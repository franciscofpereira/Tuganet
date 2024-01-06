// Final Version

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

const int WHITE = 0;
const int GRAY = 1;
const int BLACK = 2;

using namespace std;

void add_edge(vector<vector<bool>>& graph, int u, int v) {
    graph[u][v] = true;
}

void remove_edge(vector<vector<bool>>& graph, int u, int v) {
    graph[u][v] = false;
}

void transpose_graph(vector<vector<bool>>& graph, int V) {
    
    for (int i = 1; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            bool temp = graph[i][j];
            graph[i][j] = graph[j][i];
            graph[j][i] = temp;
        }
    }
}

void DFS(int start, vector<int>& color, vector<bool>& visited, stack<int>& finish_order_stack, const vector<vector<bool>>& graph, int V) {
    stack<int> dfs_stack;
    dfs_stack.push(start);

    while (!dfs_stack.empty()) {
        int current = dfs_stack.top();

        if (color[current] == WHITE) {
            color[current] = GRAY;  // Mark as gray when discovered
            visited[current] = true;

            // Pushes each unvisited neighbor of current to the stack 
            for (int neighbor = 1; neighbor < V; neighbor++) {
                if (graph[current][neighbor] && !visited[neighbor] && color[neighbor] == WHITE) {
                    dfs_stack.push(neighbor);
                }
            }
        } else if (color[current] == GRAY) {
            // Process when the node is encountered the second time
            color[current] = BLACK;  // Mark as black
            finish_order_stack.push(current);  // Record finishing order
            dfs_stack.pop();  // Remove the node from the stack
        } else {
            // Ignore if the node is already black
            dfs_stack.pop();
        }
    }
}


void DFSUtil(int start, vector<bool>& visited, vector<int>& scc, const vector<vector<bool>>& transposed_graph, int V) {
    stack<int> dfs_stack;
    dfs_stack.push(start);
    visited[start] = true;

    while (!dfs_stack.empty()) {
        int current = dfs_stack.top();
        dfs_stack.pop();

        scc.push_back(current);

        for (int neighbor = 0; neighbor < V; ++neighbor) {
            if (transposed_graph[current][neighbor] && !visited[neighbor]) {
                dfs_stack.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

void collapse_scc(vector<vector<bool>>& graph, vector<int>& scc, int V){

    int first_node = 1;
    int collapsed_node;

    for (int node : scc) {

        if(first_node == 1){
            collapsed_node = node;
            first_node = 0;
        }

        if (node != collapsed_node) {

            for (int neighbor = 1; neighbor < V; ++neighbor){
                
                // Removes node's outgoing edges, adds edge from collapsed node to node's neighbor if neighbor not in scc
                if(graph[node][neighbor] ){

                    if(find(scc.begin(), scc.end(), neighbor) == scc.end()){
                        add_edge(graph, collapsed_node, neighbor);
                    }
                    remove_edge(graph, node, neighbor);
                }
                
                // Removes node's incoming edges, adds edge from incoming neighbor to collapsed node if incoming neighbor not in scc
                if(graph[neighbor][node]){

                    if(find(scc.begin(), scc.end(), neighbor) == scc.end()){
                        add_edge(graph, neighbor, collapsed_node);
                    }
                    remove_edge(graph, neighbor, node);
                }
            }
        }
    }
}

int kosaraju_sharir(vector<vector<bool>>& graph, int V) {
    int scc_found = 0;
    stack<int> s;
    vector<bool> visited(V, false);
    vector<int> color(V, WHITE);
    
    // First DFS traversal (on original graph)
    for(int i = 1; i < V; i++){
        if(color[i] == WHITE){
            DFS(i, color, visited, s, graph, V);
        }
    }
    
    fill(visited.begin(), visited.end(), false);

    // Second DFS traversal (on transposed graph)
    transpose_graph(graph, V);

    while (!s.empty()) {
        int v = s.top();
        s.pop();

        if (!visited[v]) {
            vector<int> scc;
            DFSUtil(v, visited, scc, graph, V);

            if(scc.size() > 1){
                scc_found = 1;
                collapse_scc(graph, scc, V);
            }
        }
    }
    return scc_found;
}


// Function to identify nodes that have edges that are part of the modified graph after SCC collapsing
vector<int> condensed_nodes_with_edges(const vector<vector<bool>>& graph, int V){

    vector<int> condensed_graph_nodes;
   
    for(int i = 1; i < V; i++){
        for(int j = 1; j < V; j++){
            if(graph[i][j]){
                // If not in condensed_graph_nodes
                if (find(condensed_graph_nodes.begin(), condensed_graph_nodes.end(), i) == condensed_graph_nodes.end()) {
                    condensed_graph_nodes.push_back(i);
                }
                if (find(condensed_graph_nodes.begin(), condensed_graph_nodes.end(), j) == condensed_graph_nodes.end()) {
                    condensed_graph_nodes.push_back(j);
                }
            }
        }
    }
    return condensed_graph_nodes;
}


int dfs_iterative(int start, const vector<vector<bool>>& graph, vector<int>& condensed_nodes, vector<int>& dp, int V) {

    if(dp[start]){
        return dp[start];
    }   

    dp[start] = 1;

    // For all neighbors of starting node
    for(int i = 0; i < V; i++){
        if(graph[start][condensed_nodes[i]]){

            int neighbor = condensed_nodes[i];

            if(dp[start] < dfs_iterative(neighbor, graph, condensed_nodes, dp, V) + 1){
                dp[start] = dp[neighbor] + 1;
            }
        }
    }
    return dp[start];
}


// Function to find the longest path in a DAG using DFS with memoization
int longest_path(const vector<vector<bool>>& graph, int scc_found, int V) {
    
    vector<int> condensed_nodes;
    
    if(scc_found){
        condensed_nodes = condensed_nodes_with_edges(graph, V);
    } else{
        
        int size = V-1;
        condensed_nodes.resize(size);
        
        for(int i = 0; i < size; i++){
            condensed_nodes[i] = i+1;
        }
    }
    
    int nodes_with_edges = condensed_nodes.size();

    // If the condensed graph has no nodes with edges, it means there are no
    if(nodes_with_edges == 0){
        return 0;
    }

    // Memoization table to store the longest path for each node
    vector<int> dp(V, 0);

    int max_path = 0;

    // Iterate over nodes and perform DFS to find the longest path
    for (int i = 0; i < nodes_with_edges; ++i) {
        int path = dfs_iterative(condensed_nodes[i], graph, condensed_nodes, dp, nodes_with_edges);
        max_path = max(max_path, path);
    }
    return max_path - 1;
}


int main() {

    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    int input, num_nodes, num_edges;
    input = scanf("%d %d", &num_nodes, &num_edges);

    if(num_nodes < 2 || num_edges < 0){
        printf("0\n");
        return 0;
    }

    vector<vector<bool>> graph(num_nodes + 1, vector<bool>(num_nodes + 1, false));

    // Sets people's relationships
    int node1, node2;
    for (int i = 0; i < num_edges; i++) {
        input = scanf("%d %d", &node1, &node2);
        add_edge(graph, node1, node2);
    }

    int V = graph.size();
    int scc_found = kosaraju_sharir(graph, V);
    int result = longest_path(graph, scc_found, V);
    printf("%d\n",result);
    
    (void)input;

    return 0;
}
