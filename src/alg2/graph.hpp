
#include <iostream>
using namespace std;

//Se define el numero máximo de nodos como 2^14 = 16384
const int maxnodes = 16384;
//Se define el número máximo de aristar como 2^24 = 16777216
const int maxedges = 16777216;

// Se define el gráfico. 
int edges;
int last[maxnodes], head[maxedges], previous[maxedges], len[maxedges];
int prio[maxnodes], pred[maxnodes];

//Se rellena last con -1 usando fill.
//Se coloca edges en 0.
void graphClear() {
    fill(last, last + maxnodes, -1);
    edges = 0;
}

//Agregar arista.
void addEdge(int u, int v, int length) {
    head[edges] = v;
    len[edges] = length;
    previous[edges] = last[u];
    last[u] = edges++;
}


