// dijkstra implementation borrowed from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
// :3

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cmath>
#include <chrono>

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

// A structure to represent a
// node in adjacency list
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
// A structure to represent
// an adjacency list
struct AdjList {
     
   // Pointer to head node of list
   struct AdjListNode *head;
};
 
// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Graph {
    int V;
    struct AdjList* array;
    int ** edgeLookUp; // lookup matrix to see which edges have been added quickly
};
 
// A utility function to create
// a new adjacency list node
struct AdjListNode* newAdjListNode(
                   int dest, int weight) {
    struct AdjListNode* newNode =
            (struct AdjListNode*)
      malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Utility function to destroy an adjacency list node
void freeAdjListNode(struct AdjListNode* node) {
    free(node);
}
 
// A utility function that creates
// a graph of V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)
            malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists. 
    // Size of array will be V
    graph->array = (struct AdjList*)
       malloc(V * sizeof(struct AdjList));
 
    // Initialize each adjacency list
    // as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    // Create an edge lookup matrix and fill with zeros
    graph->edgeLookUp = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        graph->edgeLookUp[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            graph->edgeLookUp[i][j] = 0;
        }
    }

    return graph;
}

// A utility function that destoys a graph of V vertices
void destroyGraph(struct Graph* graph) {
    // free the memory allocated by addEgde
    for (int i = 0; i < graph->V; ++i) {
        struct AdjListNode* node = graph->array[i].head;
        while (node != NULL) {
            struct AdjListNode* next = node->next;
            freeAdjListNode(node);
            node = next;
        }
    }
    // free the memory allocated by createGraph
    // free lookup matrix
    for (int i = 0; i < graph->V; i++) {
        free(graph->edgeLookUp[i]);
    }
    free(graph->edgeLookUp);
    free(graph->array);
    free(graph);
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src,
                   int dest, int weight) {
    // Add an edge from src to dest. 
    // A new node is added to the adjacency
    // list of src.  The node is
    // added at the beginning
    struct AdjListNode* newNode =
            newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected,
    // add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;

    // add these edges to the lookup matrix
    graph->edgeLookUp[src][dest] = 1;
    graph->edgeLookUp[dest][src] = 1;
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
        struct AdjListNode* pCrawl = graph->array[u].head;
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

void fillInGraphRandomly(struct Graph* graph, int E, int wtRange) {
    int V = graph->V;
    int EperV = E / V; // # of edges per vertex
    // the tricky part is making sure that:
    // 1. there are no duplicate edges. e.g: (0, 1) and (1, 0)
    // 2. there are no self loops. e.g: (0, 0)
    // 3. every vertex has at least 2 edges
    // in fact, for simplicity, we will make sure that every vertex has exactly EperV edges.
    // the weights will still be random. Note that no edge has a weight of 0.
    // we will use the lookup matrix of the graph to keep track of which edges have been added.
    // 1 = edge exists, 0 = edge does not exist

    // add edges
    for (int i = 0; i < V; i++) {
        int edgesAdded = 0;
        while (edgesAdded < EperV) {
            int dest = rand() % V;
            int weight = rand() % wtRange + 1;
            if (graph->edgeLookUp[i][dest] == 0 && i != dest) {
                addEdge(graph, i, dest, weight);
                edgesAdded++;
            }
        }
    }
}

void addEdgesRandomly(struct Graph * graph, int E, int wtRange) {
    // adds E edges to an existing graph, randomly.
    // the weights will still be random. Note that no edge has a weight of 0.
    if (E == 0) return;
    int V = graph->V;
    int edgesAdded = 0;
    while (edgesAdded < E) {
        int src = rand() % V;
        int dest = rand() % V;
        int weight = rand() % wtRange + 1;
        if (graph->edgeLookUp[src][dest] == 0 && src != dest) {
            addEdge(graph, src, dest, weight);
            edgesAdded++;
        }
    }
}

void shuffleEdgeWeights(struct Graph* graph) {
    // this won't change the structure of the graph
    // but it will assign all new random weights to all edges
    int V = graph->V;
    for (int i = 0; i < V; i++) {
        struct AdjListNode* pCrawl = graph->array[i].head;
        while (pCrawl != NULL) {
            int weight = rand() % 100 + 1; // all edges have a wt > 0
            pCrawl->weight = weight;
            pCrawl = pCrawl->next;
        }
    }
}

int main(){
    srand(time(NULL));
    int iterations = 50;
    int V = pow(2, 14);
    int E;
    int wtRange = 254;
    int src = 0;
    struct Graph * graph;
    cout << "Results for algorithm 1, with 2^14 = " << V << " vertices, and edges ranging from 2^16 to 2^24." << endl;
    cout << "With " << iterations << " iterations per amount of edges (taking the adverage for each):" << endl;
    cout << endl;
    // graph making
    graph = createGraph(V);
    // graph filling
    int i = 16;
    fillInGraphRandomly(graph, pow(2,i-1), wtRange);
    for(i; i <= 24; i++){
        E = pow(2, i);
        cout << "for E = 2^" << i << " = " << E;
        double avg = 0;
        double std = 0;
        double totals[iterations];
        // edge adding
        addEdgesRandomly(graph, E - pow(2,i-1), wtRange);
        for(int j = 0; j < iterations; j++){
            // timer starting
            auto start = chrono::steady_clock::now();
            // dijkstra running
            struct intArrPair res = dijkstra(graph, src);
            // timer stopping
            auto end = chrono::steady_clock::now();
            // timer results getting
            double total = chrono::duration_cast<chrono::microseconds>(end-start).count();
            // results summing
            avg += total;
            totals[j] = total;
            // results freeing
            destroyIntArrPair(res);
            // graph shuffling
            shuffleEdgeWeights(graph); // instead of destroying and recreating the graph, or we'll be here for days
            cout << ".";
        }
        cout << endl;
        // average taking
        avg /= iterations;
        // standard deviation taking
        for(int j = 0; j < iterations; j++){
            std += pow(totals[j] - avg, 2);
        }
        std /= iterations;
        std = sqrt(std);
        // average printing
        cout << "Average time taken: " << avg << " +- " << std << " microseconds" << endl;
        cout << endl;
        shuffleEdgeWeights(graph);
    }
    cout << "That's it. That's the results. All that's left is to free the graph, but that can take a while. Feel free to hit ctrl+c." << endl;
    // graph freeing
    destroyGraph(graph);

    return 0;
}

// old versions of main:

// main ver. 1
// int main(){
//     //lets test it
//     int V = 9;
//     struct Graph* graph = createGraph(V);
//     addEdge(graph, 0, 1, 4);
//     addEdge(graph, 0, 7, 8);
//     addEdge(graph, 1, 2, 8);
//     addEdge(graph, 1, 7, 11);
//     addEdge(graph, 2, 3, 7);
//     addEdge(graph, 2, 8, 2);
//     addEdge(graph, 2, 5, 4);
//     addEdge(graph, 3, 4, 9);
//     addEdge(graph, 3, 5, 14);
//     addEdge(graph, 4, 5, 10);
//     addEdge(graph, 5, 6, 2);
//     addEdge(graph, 6, 7, 1);
//     addEdge(graph, 6, 8, 6);
//     addEdge(graph, 7, 8, 7);
 
//     // allocate memory for results and run dijkstra
//     struct intArrPair res = dijkstra(graph, 0);

//     // print results
//     for (int i = 0; i < res.size; i++) {
//         cout << "Vertex " << i << " has distance " << res.first[i] << " and previous vertex " << res.second[i] << endl;
//     }
    
//     // free memory
//     destroyIntArrPair(res);
//     destroyGraph(graph);

//     return 0;
// }

// main ver. 2
// int main(){
//     srand(time(NULL));
//     int V = pow(2, 14);
//     int E = pow(2, 24);
//     int wtRange = 254;

//     cout << "Generating graph with " << V << " vertices..." << endl;
//     struct Graph* graph = createGraph(V);

//     cout << "Filling in graph with " << E << " edges..." << endl;
//     fillInGraphRandomly(graph, E, wtRange);

//     cout << "Running Dijkstra..." << endl;
//     auto start = chrono::steady_clock::now();
//     struct intArrPair res = dijkstra(graph, 0);
//     auto end = chrono::steady_clock::now();

//     double duracion = chrono::duration_cast<chrono::microseconds>(end-start).count();
//     cout << "Dijkstra took " << duracion << " microseconds" << endl;

//     // print results
//     // for (int i = 0; i < V; i++) {
//     //     cout << "V: " << i << " , dist[v]: " << res.first[i] << " , prev[v]: " << res.second[i] << endl;
//     // }

//     cout << "Done. Freeing memory..." << endl;
//     destroyGraph(graph);
//     destroyIntArrPair(res);

//     cout << "That was for V = " << V << ", E = " << E << endl;

//     return 0;
// }
