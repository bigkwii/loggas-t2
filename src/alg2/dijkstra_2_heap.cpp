
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>

  #include "heap.hpp"


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










struct intArrPair dijkstra(struct Graph* graph, int src, int V) {
    int V = graph->V; // # of vertices in graph
    // index = vertex
    int dist[V]; // dist values for each vertex
    int prev[V]; // previous vertex in optimal path

    // Se crea el Heap con espacio igual al numero de
    struct Heap_* heap = nuevo_heap_(V);


    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        // Se inicializa el heap con todos los vértices. Esto corresponde a hacer heapify.
        //Agrego un nuevo nodo al heap.
        heap->array[i] = nuevo_Heap_nodo(i, dist[i]);
        //Seteo la posición en pos.
        heap->pos[i] = i;
    }

    

    //Mientras el heap no esté vacío.
    while (!isEmpty(heap)) {
        //extracción del vértice v con menor distancia a la raíz (que ahora sólo requiere un ExtractMin)
        struct Heap_nodo * min =  extract_Min(heap);
        // lo añadimos al arreglo de previos
        prev[min->v] = min->v;


}