

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


 //----------------------------
// Structure to represent a min heap node
struct Heap_nodo {
    int  v;     //Numero de  vértice
    int dist;   // Distancia 
};
 
// Para representar un heap
struct Heap_ {
    // Cantidad de Heap_nodos que contiene
    int size;    
    // Capacidad del heap.
    int capacity; 
    // Necesario para el algoritmo.
    int *pos;   
    struct Heap_nodo **array;
};
 

// Para crear un Heap_nodo 
struct Heap_nodo* nuevo_Heap_nodo(int v, int dist) {
    struct Heap_nodo* minHeapNode = (struct Heap_nodo*) malloc(sizeof(struct Heap_nodo));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
// Se crea un heap.
struct Heap_* createMinHeap(int capacity){
    struct Heap_* minHeap = (struct Heap_*) malloc(sizeof(struct Heap_));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct Heap_nodo**) malloc(capacity * sizeof(struct Heap_nodo*));
    return minHeap;
}
 
// Intercambiar dos nodos.
void swapMinHeapNode(struct Heap_nodo** a, struct Heap_nodo** b) {
    struct Heap_nodo* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void Heap_Heapify(struct Heap_* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
        smallest = left;
 
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
        smallest = right;
 
    if (smallest != idx) {
        // The nodes to be swapped in min heap
        Heap_nodo *smallestNode = minHeap->array[smallest];
        Heap_nodo *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest],  &minHeap->array[idx]);
 
        Heap_Heapify(minHeap, smallest);
    }
}
 
// Revisar si está vacio
int isEmpty(struct Heap_* minHeap) {
    return minHeap->size == 0;
}
 
// Extraer el minimo
struct Heap_nodo* extract_Min(struct Heap_*  minHeap) {
    if (isEmpty(minHeap)) {
        return NULL;
    }

    // Se guarda la raiz
    struct Heap_nodo* root =  minHeap->array[0];
 
    // Remplazar la raiz por el ultimo nodo.
    struct Heap_nodo* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Actualizar posición del último nodo
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    // Actualización de parámetros.
    //Se reduce el tamaño del heap
    (minHeap->size) = (minHeap->size)-1;
    //Se hace heapify a la raíz 
    Heap_Heapify(minHeap, 0);

    //Se entrega el minimo
    return root;
}

//IMPORTANTE PARAFRASEAR
 // Función para disminuir el valor dist clave
// de un vértice dado v. Esta función
// usa pos[] del montón mínimo para obtener el
// índice actual del nodo en el montón mínimo

//Se disminuye el parámetro dist de un nodo dado.

void decreaseKey(struct Heap_* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
 
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
 
    // Travel up while the complete
    // tree is not heapified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[ (i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}


// Determina si el vértice v es el mínimo o no
bool isInMinHeap(struct Heap_ *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}
 
// Printea la solucion
void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 