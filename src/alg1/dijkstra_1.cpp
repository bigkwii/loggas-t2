// dijkstra implementation borrowed from:
// https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/
// :3

#include <chrono>
#include "graph_2.hpp"
using namespace std;
//---------- SE DEFINE UNA ESTRUCURA PARA PODER RETORNAR UN PAR --------------------------
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





//------------------------------------
// !!! DIJKSTRA HERE !!!
// simple djikstra implementation only using adjacency lists
struct intArrPair dijkstra(struct Graph* graph, int src) {
    int V = graph->V; // # of vertices in graph
    // index = vertex
    int dist[V]; // dist values for each vertex
    int prev[V]; // previous vertex in optimal path
    int aux[V]; // auxillary queue for dijkstra

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        aux[i] = 0;
    }

    dist[src] = 0;

    bool auxIsEmpty = false;

    while(!auxIsEmpty){
        int u;
        // find min idx in dist and save on u
        int min = INT_MAX;
        for (int i = 0; i < V; i++) {
            if (aux[i] == 0 && dist[i] <= min) {
                min = dist[i];
                u = i;
            }
        }
        // mark aux[u] as visited
        aux[u] = -1;

        // for each neighbor v of u still in aux
        struct Node_list* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            if (aux[v] == 0) {
                int alt = dist[u] + pCrawl->weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
            pCrawl = pCrawl->next;
        }
        // check if aux is empty (i.e: if all elements are -1)
        auxIsEmpty = true;
        for (int i = 0; i < V; i++) {
            if (aux[i] != -1) {
                auxIsEmpty = false;
                break;
            }
        }
    }
    // print results
    // for (int i = 0; i < V; i++) {
    //     cout << "Vertex " << i << " has distance " << dist[i] << " and previous vertex " << prev[i] << endl;
    // }
    // save return values dist and prev
    struct intArrPair ret = createIntArrPair(V);
    for (int i = 0; i < V; i++) {
        ret.first[i] = dist[i];
        ret.second[i] = prev[i];
    }
    return ret;
}

//----------------EXPERIMENTOS SE HACEN EN EL MAIN E IMPRIME LOS RESULTADOS--------------------
int main(){
    srand(time(NULL));
    int iterations = 50;
    int V = pow(2, 14);
    int E;
    int wtRange = 254;
    int src = 0;
    struct Graph * graph;
    cout << "Results for algorithm 1, with 2^14 = " << V << " vertices, and edges ranging from 2^16 to 2^24." << endl;
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
        for(int j = 0; j < iterations; j++){
            // timer starting
            auto start = chrono::steady_clock::now();
            // dijkstra running
            struct intArrPair res = dijkstra(graph, src);
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