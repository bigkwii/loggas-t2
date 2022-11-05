
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node_list {
    int dest;
    int weight;
    struct Node_list* next;
};

// Para representar una lista de adyacencia.
struct AdjList {
   // Tiene un puntero al head de la lista.
   struct Node_list *head;
};

//REPRESENTACIÓN DE UN GRÁFICO.
//Un gráfico es un arreglo con V nodos.
//Contiene una lista de adyacencia.
struct Graph {
    int V;
    struct AdjList* array;
};

// Función para crear un nodo
struct Node_list* new_node_list(int dest, int weight) {
    struct Node_list* newNode = (struct Node_list*) malloc(sizeof(struct Node_list));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
// Función para crear un grafo de V vértices.
struct Graph* new_Graph(int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    // Se crean las listas de adyacencia.
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    // Initialize each adjacency list
    // as empty by making head as NULL
    for (int i = 0; i < V; ++i){
        graph->array[i].head = NULL;
    }
    return graph;
}
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    // Add an edge from src to dest. 
    // A new node is added to the adjacency
    // list of src.  The node is
    // added at the beginning
    struct Node_list* newNode = new_node_list(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    // Since graph is undirected,
    // add an edge from dest to src also
    newNode = new_node_list(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}