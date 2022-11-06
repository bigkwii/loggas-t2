#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
 
//---------ACÁ SE CREA EL HEAP O COLA DE PRIORIDAD----------


// Cada elemento del heap es un Heap_nodo
struct Heap_nodo {
    int  v;     // índice del vértice.
    int dist;   // Distancia = prioridad.
};
 
//Se define el Heap:
struct Heap_ {
    // Cantidad de Heap_nodos que contiene
    int size;    
    // Capacidad del heap.
    int capacity; 
    // Necesario para el algoritmo.
    int *pos;   
    struct Heap_nodo **array;
};
 

// Función para crear un elemento del heap. Recibe de parámetro lo que
// define la estructura creada más arriba.
struct Heap_nodo* nuevo_Heap_nodo(int v, int d) {
    struct Heap_nodo* new_heap_node = (struct Heap_nodo*) malloc(sizeof(struct Heap_nodo));
    new_heap_node -> v = v;
    new_heap_node -> dist = d;
    return new_heap_node;
}
 
// Función para crear una cola de prioridad. Se le debe dar la capacidad.
struct Heap_* nuevo_heap_(int capacity){
    struct Heap_* heap_ = (struct Heap_*) malloc(sizeof(struct Heap_));
    //Se inicializa vacío.
    heap_->size = 0;
    //Se setea la capacidad.
    heap_->capacity = capacity;
    
    heap_->pos = (int *)malloc(capacity * sizeof(int));
    heap_->array = (struct Heap_nodo**) malloc(capacity * sizeof(struct Heap_nodo*));
    return heap_;
}
 
// Función que dado dos nodos, los intercammbia.
void swap_nodes(struct Heap_nodo** a, struct Heap_nodo** b) {
    struct Heap_nodo* t = *a;
    *a = *b;
    *b = t;
}
 
//Función que dado una cola de prioridad (heap) y un índice i hace
//heapify. 
void Heap_Heapify(struct Heap_* heap_, int i) {
    int smallest, izq, der;
    smallest = i;
    izq = 2 * i + 1;
    der = 2 * i + 2;
    //caso 1
    if ( (izq < heap_->size) && (heap_->array[izq]->dist < heap_->array[smallest]->dist )){
        smallest = izq;
    } 
    //caso 2
    if (der < heap_->size && heap_->array[der]->dist < heap_->array[smallest]->dist ) {
        smallest = der;
    }

    //Caso 3
    if (smallest != i) {
        // Nodos a intercambiar.
        Heap_nodo *smallestNode = heap_->array[smallest];
        Heap_nodo *idxNode = heap_->array[i];
 
        // Intercambio de posiciones 
        heap_->pos[smallestNode->v] = i;
        heap_->pos[idxNode->v] = smallest;
 
        // Intercambio de nodos.
        swap_nodes(&heap_->array[smallest],  &heap_->array[i]);

        //Heapify
        Heap_Heapify(heap_, smallest);
    }
}
 
// Revisar si está vacio
int isEmpty(struct Heap_* heap) {
    return heap->size == 0;
}
 
// Extraer el minimo
struct Heap_nodo* extract_Min(struct Heap_*  heap) {
    //Caso base.
    if (isEmpty(heap)) {
        return NULL;
    }

    // Se guarda la raiz
    struct Heap_nodo* root =  heap->array[0];
 
    // Remplazar la raiz por el ultimo nodo.
    struct Heap_nodo* ult = heap->array[heap->size - 1];
    heap->array[0] = ult;
 
    // Actualizar posición del último nodo
    heap->pos[root->v] = heap->size-1;
    heap->pos[ult->v] = 0;
 
    // Actualización de parámetros.
    //Se reduce el tamaño del heap
    (heap->size) = (heap->size)-1;
    //Se hace heapify a la raíz 
    Heap_Heapify(heap, 0);

    //Se entrega el minimo
    return root;
}

//Se disminuye el parámetro dist de un nodo dado dentro de un heap a una nueva distancia.
//Usa pos[] del heap para conocer el índice actual del vértice en el heap.

void decreaseKey(struct Heap_* heap, int v, int dist) {
    //Se obtiene la posición del nodo v en el heap
    int i = heap->pos[v];
 
    //Se obtiene el nodo y se actualiza su distancia.
    heap->array[i]->dist = dist;

    //Mientras no se haya hecho heapify en todo el heap.
    //Para el análisis, este while debería tomar tiempo O(Logn)
    while (i && (heap->array[i]->dist < heap->array[(i - 1) / 2]->dist)) {
        // Se intercambia el nodo con el de su padre.
        heap->pos[heap->array[i]->v] = (i-1)/2;
        heap->pos[heap->array[ (i-1)/2]->v] = i;
        swap_nodes(&heap->array[i],  &heap->array[(i - 1) / 2]);
        // Nos movemos al nodo padre.
        i = (i - 1) / 2;
    }
}


// Determina si el vértice está en el heap.
bool isInHeap(struct Heap_ *heap, int v) {
   if (heap->pos[v] < heap->size)
     return true;
   return heap->pos[v] < heap->size;
}
 

// Printea la solucion
void printArr(int dist[], int n) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 

