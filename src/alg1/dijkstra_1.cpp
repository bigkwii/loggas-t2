// dijkstra implementation borrowed from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
// :3

 #include "graph_definition.hpp"

using namespace std;

// A structure to store a pair of int arrays of the same size
struct intArrPair {
    int* first;
    int* second;
    int size;
};

// Function that creates and int array pair of a given size
struct intArrPair createIntArrPair(int size) {
    struct intArrPair pair;
    pair.first = (int*)malloc(size * sizeof(int));
    pair.second = (int*)malloc(size * sizeof(int));
    pair.size = size;
    return pair;
}

// Function that destoys an int array pair
void destroyIntArrPair(struct intArrPair pair) {
    free(pair.first);
    free(pair.second);
}

 


// !!! DIJKSTRA HERE !!!
// simple djikstra implementation only using adjacency lists
struct intArrPair dijkstra(struct Graph* graph, int src) {
    int V = graph->V; // # of vertices in graph
    // index = vertex
    int dist[V]; // dist values for each vertex
    int prev[V]; // previous vertex in optimal path
    int aux[V]; // auxillary queue for dijkstra

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        aux[i] = 0;
    }

    dist[src] = 0;

    bool auxIsEmpty = false;

    while(!auxIsEmpty){
        int u;
        // find min idx in dist and save on u
        int min = INT_MAX;
        for (int i = 0; i < V; i++) {
            if (aux[i] == 0 && dist[i] <= min) {
                min = dist[i];
                u = i;
            }
        }
        // mark aux[u] as visited
        aux[u] = -1;

        // for each neighbor v of u still in aux
        struct Node_list* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            if (aux[v] == 0) {
                int alt = dist[u] + pCrawl->weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
            pCrawl = pCrawl->next;
        }
        // check if aux is empty (i.e: if all elements are -1)
        auxIsEmpty = true;
        for (int i = 0; i < V; i++) {
            if (aux[i] != -1) {
                auxIsEmpty = false;
                break;
            }
        }
    }
    // print results
    // for (int i = 0; i < V; i++) {
    //     cout << "Vertex " << i << " has distance " << dist[i] << " and previous vertex " << prev[i] << endl;
    // }
    // save return values dist and prev
    struct intArrPair ret = createIntArrPair(V);
    for (int i = 0; i < V; i++) {
        ret.first[i] = dist[i];
        ret.second[i] = prev[i];
    }
    return ret;

}

int main(){
    //lets test it
    int V = 9;
    struct Graph* graph = new_Graph(V);
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);
 
    // allocate memory for results and run dijkstra
    struct intArrPair res = dijkstra(graph, 0);

    // print results
    for (int i = 0; i < res.size; i++) {
        cout << "Vertex " << i << " has distance " << res.first[i] << " and previous vertex " << res.second[i] << endl;
    }
    
    // free memory
    destroyIntArrPair(res);
    destroyGraph(graph);

    return 0;
}
