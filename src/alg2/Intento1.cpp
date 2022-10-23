 #include "heap.hpp"
  #include "graph.hpp"



void dijkstra(int s) {
    fill(pred, pred + maxnodes, -1);
    fill(prio, prio + maxnodes, INT_MAX);
    prio[s] = 0;
    hsize = 0;
    add(s, 0);

    while (hsize) {
        int u = removeMin();
        for (int e = last[u]; e >= 0; e = previous[e]) {
            int v = head[e];
            int nprio = prio[u] + len[e];
            if (prio[v] > nprio) {
                if (prio[v] == INT_MAX)
                    add(v, nprio);
                else
                    increasePriority(v, nprio);
                prio[v] = nprio;
                pred[v] = u;
            }
        }
    }
}

int main() {
    graphClear();
    addEdge(0, 1, 10);
    addEdge(1, 2, -5);
    addEdge(0, 2, 8);

    dijkstra(0);

    for (int i = 0; i < 3; i++)
        cout << prio[i] << endl;
}