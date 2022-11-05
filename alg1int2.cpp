// RUN THIS
#include <iostream>
#include <limits.h>

using namespace std;

#define MAX_NODES 16384
#define MAX_EDGES 16777216

// TODO: IMPLEMENTACIÓN DE VERDAD DE LAS LISTAS DE ADYACENCIA
// https://www.softwaretestinghelp.com/wp-content/qa/uploads/2019/08/7.undirected-graph-and-its-adjacency-list.png

// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
int minDistance(int dist[], int prev[], int V) {
 
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (prev[v] == -1 && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

// A utility function to print the constructed distance
// array
// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void printSolution(int dist[], int V) {
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " \t\t\t\t" << dist[i] << endl;
}

//Para saber si un arreglo solo tiene -1, lo que significa
//para la implementación que está vacío.
bool empty(int aux[], int V) {
    for (int i=0; i>V;i++) {
        if (aux[i] != -1) {
            return false;
        }
    }
    return true;
}


// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
// SOURCE: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void dijkstra(int ** graph, int src, int V) {
    int dist[V]; 
    int prev[V]; 
    int aux[V]; //Arreglo auxiliar para contener los índices de los nodos

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX; 
        prev[i] = -1;
        aux[i]=i;
    }
    

    // Definimos distancia a la raiz igual a 0
    dist[src] = 0;

    // Mientras que el arreglo auxiliar no está vacío
    while (!empty(aux, V)) {
        //Buscamos el vértice que tenga la menor distancia guardada que llamamos u
        int u = minDistance(dist, prev, V);
        //Lo eliminamos del arreglo auxiliar.
        aux[u] = -1;
        prev[u] = u;

        // Para cada vecino v de u todavía en aux.
        //for ( each neighbor v of u still in Q)
            //int alt  =  dist[u] + Graph.Edges(u, v)
            //if (alt < dist[v]){
                //dist[v] = alt;
                //prev[v] = u;
            //}
    }
}

void run(int V, int rand_range) {
    // allocate
    int ** graph = (int **) malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph[i] = (int *) malloc(V * sizeof(int));
    }

    // fill in
    for(int node = 0; node < V; node++){
        for(int edge = 0; edge < V; edge++){
            if(node == edge){
                graph[node][edge] = 0;
            }
            if(node < edge){
                graph[node][edge] = rand() % rand_range;
            }
            if(node > edge){ // symmetric
                graph[node][edge] = graph[edge][node];
            }
        }
    }
 
    // print
    // cout << "Graph:" << endl;
    // for(int node = 0; node < V; node++){
    //     for(int edge = 0; edge < V; edge++){
    //         cout << graph[node][edge] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    
    // Function call
    dijkstra(graph, 0, V);

    // free mem
    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);
}

int main(){
    srand(time(NULL));

    int max_nodes = MAX_NODES;

    run(80, 12);
 
    return 0;
}
