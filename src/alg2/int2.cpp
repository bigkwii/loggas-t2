
  #include "heap.hpp"

 //----------------------------
// A structure to represent a
// node in adjacency list
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};
// A structure to represent an adjacency list
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
struct AdjListNode* newAdjListNode(int dest, int weight) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
// A utility function that creates
// a graph of V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    // Create an array of adjacency lists. 
    // Size of array will be V
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
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    // Since graph is undirected,
    // add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}



// The main function that calculates
// distances of shortest paths from el nodo 0 a todos
// vertices. It is a O(ELogV) function
void dijkstra_heap(struct Graph* graph, int V, int src) {  
    // Se crean los dos arreglos.
    int dist[V];    
    bool prev[V];

    // La distancia del vértice a si mismo es 0
    dist[src] = 0;


    //Inicializamos todas las distancias como infinitas y
    //prev como indefinido.
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, prev[i] = -1;
 
 
    // Se crea el Heap con espacio igual al numero de
    struct Heap_* heap = nuevo_heap_(V);
 
    // Se inicializa el heap con todos los vértices
    for (int i = 0; i < V; ++i) {
        dist[i] = INT_MAX;
        //Agrego un nuevo nodo al heap.
        heap->array[i] = nuevo_Heap_nodo(i, dist[i]);
        //Seteo la posición en pos.
        heap->pos[i] = i;
    }
 
    // Se hace que dist del vértice src sea 0 así podemos extraerlo.
    heap->array[src] =  nuevo_Heap_nodo(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
 
    // Initially size of min heap is equal to V
    heap->size = V;
 

    // Mientras la cola de prioridad no esté vacía, repetimos la extracción del
    //vértice v con menor distancia a la raíz
    while (!isEmpty(heap)) {
        // Se extrae el vértice con el mínimo.
        struct Heap_nodo* minimo_extraido = extract_Min(heap);
       
        // Almacenar el número de vértice extraído
        int u = minimo_extraido->v;
 
        // Traverse through all adjacent
        // vertices of u (the extracted
        // vertex) and update
        // their distance values

        //Se obtiene un puntero al vértice
        struct AdjListNode* pCrawl = graph->array[u].head;
        
        //Mientras que no esté nulo.
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
}

// Driver program to test above functions
int main() {
    // create the graph given in above figure
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
 
    dijkstra_heap(graph, 9,0);
 
    return 0;
}