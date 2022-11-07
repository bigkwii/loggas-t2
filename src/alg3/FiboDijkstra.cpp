// C++ program to demonstrate Extract min, Deletion()
// and Decrease key() operations in a fibonacci heap

#include "graph_2.hpp"
#include "fibonacci_heap.hpp"




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






//------------- ALGORITMO 3 ------------------
struct intArrPair dijkstra(struct Graph* graph, int src){
	// Get the number of vertices in graph
    int V = graph->V; 
    int dist[V];  
    int prev[V]; 

    // we declara an array of pointers to the nodes in the heap so we can access each node
    // in constant time 
    struct node * nodePointers[V];

   
    // dist values used to pick
    // minimum weight edge in cut
	dist[src] = 0;
    for (int v = 0; v < V; ++v){
		if (v!=src){
			dist[v] = INT_MAX;
            prev[v] = -1;
		}
    	nodePointers[v]=insertion(v , dist[v]);
    }
	while (mini!=NULL){
		int u = mini->n;
		Extract_min();
        prev[u] = 1;
		//vecinos de u
		struct Node_list* pCrawl = graph->array[u].head;
		
        while(pCrawl!=NULL){
			int vertice = pCrawl->dest;
			
            if ( dist[u] != INT_MAX &&pCrawl->weight + dist[u] < dist[vertice]){
				 dist[vertice] = dist[u] + pCrawl->weight;
                 prev[vertice] = u;
				 Decrease_key(nodePointers[vertice] , dist[vertice]);
			}
			pCrawl = pCrawl->next;
		}
	}
	//printArr(dist,V);

    //Se hacen los arreglos para poder retornar el par.
    struct intArrPair ret = createIntArrPair(V);
    //Se recorren los vértices.
    for (int i = 0; i < V; i++) {
        ret.first[i] = dist[i];
        ret.second[i] = prev[i];
    }
    return ret;
}
//--------- EN EL MAIN SE DESARROLLA EL TESTEO PARA ESTE ALGORITMO -------------
int main(){
    srand(time(NULL));
    int iterations = 50;
    int V = pow(2, 14);
    int E;
    int wtRange = 254;
    int src = 0;
    struct Graph * graph;
    cout << "Results for algorithm 3, with 2^14 = " << V << " vertices, and edges ranging from 2^16 to 2^24." << endl;
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