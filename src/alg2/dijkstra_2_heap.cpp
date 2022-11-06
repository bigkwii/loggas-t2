 #include "heap.hpp"
#include "graph_2.hpp"
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
                        prev[v] = u;
                        // Actualizamos dis en el heap usando decreaseKey.
                        decreaseKey(heap, v, dist[v]);
                    }
                }
            }
            pCrawl = pCrawl->next;
       }
    }
    // print the calculated shortest distances
    //printArr(dist, V);

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
    srand(time(NULL));
    int iterations = 50;
    int V = pow(2, 14);
    int E;
    int wtRange = 254;
    int src = 0;
    struct Graph * graph;
    cout << "Results for algorithm 2, with 2^14 = " << V << " vertices, and edges ranging from 2^16 to 2^24." << endl;
    cout << "With " << iterations << " iterations per amount of edges (taking the adverage for each):" << endl;
    cout << endl;
    // graph making
    graph = createGraph(V);
    // graph filling
    int i = 16;
    fillInGraphRandomly(graph, pow(2,i-1), wtRange);
    for(i; i <= 24; i++){
        E = pow(2, i);
        cout << "for E = 2^" << i << " = " << E;
        double avg = 0;
        double std = 0;
        double totals[iterations];
        // edge adding
        addEdgesRandomly(graph, E - pow(2,i-1), wtRange);
        struct Graph * graph_opti;
        graph = createGraph(V);
        for(int j = 0; j < iterations; j++){
            // timer starting
            auto start = chrono::steady_clock::now();
            // dijkstra running
            struct intArrPair res = dijkstra_heap(graph, src);
            // timer stopping
            auto end = chrono::steady_clock::now();
            // timer results getting
            double total = chrono::duration_cast<chrono::microseconds>(end-start).count();
            // results summing
            avg += total;
            totals[j] = total;
            // results freeing
            destroyIntArrPair(res);
            // graph shuffling
            shuffleEdgeWeights(graph); // instead of destroying and recreating the graph, or we'll be here for days
            cout << ".";
        }
        cout << endl;
        // average taking
        avg /= iterations;
        // standard deviation taking
        for(int j = 0; j < iterations; j++){
            std += pow(totals[j] - avg, 2);
        }
        std /= iterations;
        std = sqrt(std);
        // average printing
        cout << "Average time taken: " << avg << " +- " << std << " microseconds" << endl;
        cout << endl;
        shuffleEdgeWeights(graph);
    }
    cout << "That's it. That's the results. All that's left is to free the graph, but that can take a while. Feel free to hit ctrl+c." << endl;
    // graph freeing
    destroyGraph(graph);

    return 0;
}