// RUN THIS
#include <iostream>
#include <limits.h>

using namespace std;

#define MAX_NODES 16384
#define MAX_EDGES 16777216

bool empty_array(int aux[],int V) {
    for (int i = 0; i < V; i++) {
        if (aux[i] != -1) {
            return false;
        }
    }
    return true;
}


int minDistance(int dist[], int sptSet[], int V) {
    // Initialize min value
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == -1 && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}


void dijkstra(int ** graph, int src, int V) {
    int dist[V];  
    int aux[V];
    int prev[V];  

    // Se inicializan todas las distancias como infinitas.
    //Se inicializa los nodos prev como indefinidos.
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        aux[i] = i;
    }
    // Para el nodo raíz, definimos su distancia al nodo raíz como 0.
    dist[src] = 0;

    //mientras tenemos vértices en el arreglo auxiliar.
    while (!empty_array(aux, V)) {
        cout << "while '\n";
        //Buscamos el vértice que tenga la menor distancia guardada en el
        //arreglo de distancias y lo eliminamos del arreglo auxiliar.
        int u = minDistance(dist, prev, V);
        cout << u;
        //Ahora se elimina el nodo v del arreglo auxiliar.
        aux[u] = -1;
        //Por cada vecino v del vértice recién eliminado u:
        for (int v = 0; v < V; v++) {
            if (prev[v]==-1) {
                if  (graph[u][v] && (dist[u] != INT_MAX)) {
                    //si la distancia guardada para v es mayor a la distancia guardada para u más el peso de
                    //la arista (u, v)
                    if ((dist[u] + graph[u][v] < dist[v])){
                        //cambiamos el valor de la distancia de u y guardamos 
                        //v como el nodo previo de u.
                        dist[v] = dist[u] + graph[u][v];
                        prev[u] = v;
                    }
                } 
            }
        }

    }
    cout << "Finished ";
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

    run(8, 12);
 
    return 0;
}