
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cmath>
#include <chrono>



//----------REPRESENTACIÓN DE UN GRAFO -------------------------
//Representando un nodo dentro del grafo.
struct Node_list {
    int dest;
    int weight;
    struct Node_list* next;
};
 
// Para representar una lista de adyacencia.
struct Adj_List {
   // Tiene un puntero al head de la lista.
   struct Node_list *head;
};
 
//REPRESENTACIÓN DE UN GRÁFICO.
//Un gráfico es un arreglo con V nodos.
//Contiene un array de listas de adyacencia.
struct Graph {
    int V;
    struct Adj_List* array;
    int ** edgeLookUp; // lookup matrix to see which edges have been added quickly
};



// -------------- FUNCIONES PARA PODER TESTEAR -----------------------

// Función para crear un nodo
struct Node_list* new_Adj_ListNode( int dest, int weight) {
    struct Node_list* newNode = (struct Node_list*) malloc(sizeof(struct Node_list));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Función para liberar un nodo.
void freeNode_list(struct Node_list* node) {
    free(node);
}
 
// Función para crear un grafo de V vértices.
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    //Se crean las listas de adyacencia.
    graph->array = (struct Adj_List*) malloc(V * sizeof(struct Adj_List));
    // Inicializamos las listas de adyacencia como vacias.
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
    }
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

// Función para destruir un gráfico de V vértices.
void destroyGraph(struct Graph* graph) {
    // free the memory allocated by addEgde
    for (int i = 0; i < graph->V; ++i) {
        struct Node_list* node = graph->array[i].head;
        while (node != NULL) {
            struct Node_list* next = node->next;
            freeNode_list(node);
            node = next;
        }
    }
    // Liberamos la memoria.
    // free lookup matrix
    for (int i = 0; i < graph->V; i++) {
        free(graph->edgeLookUp[i]);
    }
    free(graph->array);
    free(graph);
}
 
// Función para añadir un adge o arista al grafo no dirigido desde orig a dest.
void addEdge(struct Graph* graph, int orig, int dest, int weight) {
    // Se crea arista desde orgig y dest. 
    struct Node_list* newNode = new_Adj_ListNode(dest, weight);
    newNode->next = graph->array[orig].head;
    graph->array[orig].head = newNode;
 
    // Simétricamente se crea de dest a orig.
    newNode = new_Adj_ListNode(orig, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;

    // add these edges to the lookup matrix
    graph->edgeLookUp[orig][dest] = 1;
    graph->edgeLookUp[dest][orig] = 1;
}



// -------------- GENERAR GRAFOS ALEATORIOS -----------------------

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
        struct Node_list* node = graph->array[i].head;
        while (node != NULL) {
            node->weight = rand() % 100 + 1;
            node = node->next;
        }
    }
}