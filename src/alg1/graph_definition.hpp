
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>


//-----------------DEFINICIÓN ESTRUCTURA PARA UN GRAFO ------------------------

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
};




//--------------FUNCIONES PARA PODER CREAR UN GRAFO ----------------
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
    graph->array = (struct Adj_List*) malloc(V * sizeof(struct Adj_List));
    // Inicializamos las listas de adyacencia como vacias.
    for (int i = 0; i < V; ++i){
        graph->array[i].head = NULL;
    }
    return graph;
}


// Añade una arista dessde orig a des, que se agrega al comienzo de la lista.
void addEdge(struct Graph* graph, int orig, int dest, int weight) {
    struct Node_list* newNode = new_node_list(dest, weight);
    newNode->next = graph->array[orig].head;
    graph->array[orig].head = newNode;

    // Es simétrico por ser no dirigido, así que se crea la arista 
    //desde dest hasta orig también.
    newNode = new_node_list(orig, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}



// Función para destruir un grafo de V nodos.
void destroyGraph(struct Graph* graph) {
    for (int i = 0; i < graph->V; ++i) {
        struct Node_list* node = graph->array[i].head;
        while (node != NULL) {
            struct Node_list* next = node->next;
            free(node);
            node = next;
        }
    }
    free(graph->array);
    free(graph);
}