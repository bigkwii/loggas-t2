// dijkstra implementation borrowed from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
// :3

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <queue>

using namespace std;

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
 
    return graph;
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
}

// !!! DIJKSTRA HERE !!!
// simple djikstra implementation only using adjacency lists
void dijkstra(struct Graph* graph, int src) {
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
    // return dist and prev
    for (int i = 0; i < V; i++) {
        cout << "Vertex " << i << " has distance " << dist[i] << " and previous vertex " << prev[i] << endl;
    }

}

int main(){
    //lets test it
    int V = 9;
    struct Graph* graph = createGraph(V);
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
 
    dijkstra(graph, 0);
 
    return 0;
}
