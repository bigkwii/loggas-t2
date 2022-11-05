 #include "heap.hpp"
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
}






struct intArrPair dijkstra_heap(struct Graph* graph, int src ) {
    int V = graph->V; // # of vertices in graph
    // index = vertex
    int dist[V]; // dist values for each vertex
    int prev[V]; // previous vertex in optimal path

    // Se crea el Heap con espacio igual al numero de
    struct Heap_* heap = nuevo_heap_(V);


    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }

    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        heap->array[v] = nuevo_Heap_nodo(v,dist[v]);
        heap->pos[v] = v;
    }

    // Se hace que dist del vértice src sea 0 así podemos extraerlo.
    heap->array[src] =  nuevo_Heap_nodo(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);

    heap->size = V;
    //Mientras el heap no esté vacío.
    cout << "while\n";
    while (!isEmpty(heap)) {
        //extracción del vértice v con menor distancia a la raíz (que ahora sólo requiere un ExtractMin)
        struct Heap_nodo * min =  extract_Min(heap);
        cout << "min is: v =" << min->v << " y dist = " << min->dist;
        int u = min->v;
        // lo añadimos al arreglo de previos
        prev[u] = u;

        //Actualizamos la distancia mínima a cada vecino de v que lo requiera
        struct AdjListNode* pCrawl = graph->array[u].head;

       while (pCrawl != NULL) {
            //Obtenemos 
            int v = pCrawl->dest;
            // If shortest distance to v is
            // not finalized yet, and distance to v
            // through u is less than its
            // previously calculated distance
            //Si la menor distancia a v 
            if (isInHeap(heap, v)) {
                if (dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]) {
                    dist[v] = dist[u] + pCrawl->weight; 
                    // update distance
                    // value in min heap also
                    decreaseKey(heap, v, dist[v]);
                }
            }
            pCrawl = pCrawl->next;
       }
    }
    // print the calculated shortest distances
    printArr(dist, V);
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
    // we will use a 2D array to keep track of which edges have been added.
    // 1 = edge exists, 0 = edge does not exist

    // so we do that. allocate the memory and fill in with zeros
    int ** edges = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        edges[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            edges[i][j] = 0;
        }
    }
    // add edges
    for (int i = 0; i < V; i++) {
        int edgesAdded = 0;
        while (edgesAdded < EperV) {
            int dest = rand() % V;
            if (edges[i][dest] == 0 && i != dest) {
                int weight = rand() % wtRange + 1; // all edges have a wt > 0
                addEdge(graph, i, dest, weight);
                edges[i][dest] = 1;
                edgesAdded++;
            }
        }
    }
    // free edges
    for (int i = 0; i < V; i++) {
        free(edges[i]);
    }
    free(edges);
}




int main(){
    int V = 10;
    int E=15;
    double avg = 0;
    int wtRange = 20;
    struct Graph * graph;
    graph = createGraph(V);
    fillInGraphRandomly(graph, E, wtRange);
    int src = 0;
    // timer starting
    auto start = chrono::steady_clock::now();
    // dijkstra running
    struct intArrPair res = dijkstra_heap(graph, src);
    // timer stopping
    auto end = chrono::steady_clock::now();
    // timer results getting
    double total = chrono::duration_cast<chrono::microseconds>(end-start).count();

    destroyGraph(graph);
    destroyIntArrPair(res);
    cout << ".";

    return 0;
}