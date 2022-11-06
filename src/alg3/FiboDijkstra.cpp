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
void dijkstra(struct Graph* graph, int src){
	// Get the number of vertices in graph
    int V = graph->V;
   
    // dist values used to pick
    // minimum weight edge in cut
    int dist[V];
	dist[src] = 0;
	struct node *nodePointers[V];
    for (int v = 0; v < V; ++v){
		if (v!=src){
			dist[v] = INT_MAX;
		}
    	nodePointers[v]=insertion(v , dist[v]);
    }
	while (mini!=NULL){
		int u = mini->n;
		Extract_min();
		//vecinos de u
		struct Node_list* pCrawl = graph->array[u].head;
		while(pCrawl!=NULL){
			int vertice = pCrawl->dest;
			if ( dist[u] != INT_MAX &&pCrawl->weight + dist[u] < dist[vertice]){
				 dist[vertice] = dist[u] + pCrawl->weight;
				 int priority = nodePointers[vertice]->key;
				 Find(mini , priority , dist[vertice]);
			}
			pCrawl = pCrawl->next;
		}
	}
	printArr(dist,V);
}




// Driver code
int main(){

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
 
    dijkstra(graph, 0);
	return 0;
}