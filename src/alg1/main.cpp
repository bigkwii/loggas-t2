// RUN THIS
#include <iostream>
#include <limits.h>

using namespace std;

#define MAX_NODES 16384
#define MAX_EDGES 16777216

// TODO: IMPLEMENTACIÓN DE VERDAD DE LAS LISTAS DE ADYACENCIA
// https://www.softwaretestinghelp.com/wp-content/qa/uploads/2019/08/7.undirected-graph-and-its-adjacency-list.png

// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
int minDistance(int dist[], bool sptSet[], int V) {
 
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

// A utility function to print the constructed distance
// array
// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void printSolution(int dist[], int V) {
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " \t\t\t\t" << dist[i] << endl;
}

// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void dijkstra(int ** graph, int src, int V) {
    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet, V);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v]
                && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
 
    // print the constructed distance array
    //printSolution(dist, V);
    
    // no need to return anything, we only care about the time it takes to run

    cout << "Finished for " << V << " nodes, trust me :3" << endl;
}

void run(int V, int rand_range) {
    // allocate
    int ** graph = (int **) malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph[i] = (int *) malloc(V * sizeof(int));
    }

    // fill in
    for(int node = 0; node < V; node++){
        for(int edge = 0; edge < V; edge++){
            if(node == edge){
                graph[node][edge] = 0;
            }
            if(node < edge){
                graph[node][edge] = rand() % rand_range;
            }
            if(node > edge){ // symmetric
                graph[node][edge] = graph[edge][node];
            }
        }
    }
 
    // print
    // cout << "Graph:" << endl;
    // for(int node = 0; node < V; node++){
    //     for(int edge = 0; edge < V; edge++){
    //         cout << graph[node][edge] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    
    // Function call
    dijkstra(graph, 0, V);

    // free mem
    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);
}

int main(){
    srand(time(NULL));

    int max_nodes = MAX_NODES;

    run(16384, 12);
 
    return 0;
}
