 #include "heap.hpp"
#include "graph_2.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cmath>
#include <chrono>

using namespace std;

//------------- ACÁ SE HIZO UNA ESTRUCTURA PARA PODER RETORNAR EL PAR PEDIDO ------------------
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


//------------- ALGORIGMO DE DIJKSTRA USANDO UN HEAP ------------------

struct intArrPair dijkstra_heap(struct Graph* graph, int src ) {
    //Recuperamos el numero de vertices y se definen los arreglos dist y prev.
    int V = graph->V; 
    int dist[V];  
    int prev[V];  

    // Se crea el Heap con espacio igual al numero de
    struct Heap_* heap = nuevo_heap_(V);
    
    //Se llena dist con infinito y prev con idefinido.
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        //Se llena el heap con un elemento por nodo, con prioridad igual a dist.
        heap->array[i] = nuevo_Heap_nodo(i,dist[i]);
        heap->pos[i] = i;
    }

    // Se hace que dist del vértice src sea 0 así podemos extraerlo.
    heap->array[src] =  nuevo_Heap_nodo(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);

    //Seteamos el tamaño del heap.
    heap->size = V;


    //Mientras el heap no esté vacío.

    while (!isEmpty(heap)) {
        //extracción del vértice v con menor distancia a la raíz (que ahora sólo requiere un ExtractMin)
        struct Heap_nodo * min =  extract_Min(heap);
        //Recuperamos el índice del vértice.
        int u = min->v;
        // lo añadimos al arreglo de previos
        prev[u] = 1;

        //Actualizamos la distancia mínima a cada vecino de v que lo requiera
        struct Node_list* pCrawl = graph->array[u].head;
        //Recorremos los vecinos.
        while (pCrawl != NULL) {
            //Obtenemos 
            int v = pCrawl->dest;
            //
            if (isInHeap(heap, v)) {
                if (dist[u] != INT_MAX) {
                    if ( pCrawl->weight + dist[u] < dist[v]) {
                        dist[v] = dist[u] + pCrawl->weight; 
                        // Actualizamos dis en el heap usando decreaseKey.
                        decreaseKey(heap, v, dist[v]);
                    }
                }
            }
            pCrawl = pCrawl->next;
       }
    }
    // print the calculated shortest distances
    printArr(dist, V);

    //Se hacen los arreglos para poder retornar el par.
    struct intArrPair ret = createIntArrPair(V);
    //Se recorren los vértices.
    for (int i = 0; i < V; i++) {
        ret.first[i] = dist[i];
        ret.second[i] = prev[i];
    }
    return ret;
}



// --------- EN EL MAIN SE DESARROLLA EL TESTEO PARA ESTE ALGORITMO -------------

int main(){
    int V = 10;
    int E=15;
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
 

    destroyGraph(graph);
    destroyIntArrPair(res);
    cout << ".";

    return 0;
}