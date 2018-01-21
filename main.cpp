/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  sample example using fibonacci heap
 *
 *        Version:  1.0
 *        Created:  2018/01/20
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  lionking
 *   Organization:  NCTU_CS_EDA
 *
 * =====================================================================================
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "pheap.h"
#define INF 1e9 /* just infinity */
#define MAX 1024 /* maximum vertex count */


/* weight type */
typedef int weight_t;


/* graph: adjecency matrix graph(start index: 0) 	*
 * d[i]: distance from source to node i 			*
 * both of two need to be initial to INF before use	*/
weight_t d[MAX];
weight_t graph[MAX][MAX];


/* relax node v through node u		*
 * if can be relaxed, return true	*/
bool relax(const int u, const int v, const weight_t w)
{
	if(d[u]+w < d[v]){
		d[v] = d[u] + w;
		return true;
	}
	return false;
}


struct Node
{
    int index;
    weight_t cost;

    Node(int d, weight_t c) : index(d), cost(c) {}
};


/***************** Dijkstra Shortest Path Algorithm	*****************
 * single source shortest path, cannot allow negative edge weight!	*
 * Usage: Dijkstra(source, num)										*
 * parameter: 														*
 * 1. source: shortest path source									*
 * 2. num: graph vertex number(default to MAX)						*
 * Note: need to initial graph and d first							*
 ********************************************************************/
void Dijkstra(const int source, const int num=MAX)
{
    auto get_key = [](Node& node) -> decltype(node.cost)& { return node.cost; };

	d[source] = 0; // important! distance to source is 0
    pheap<Node, decltype(get_key)> heap(get_key);
    pheap_handle<Node> handle[MAX];

	// push all nodes into heap
    for (int i = 0; i < num; ++i) {
        handle[i] = heap.emplace(i, d[i]);
    }
    vector<bool> in_heap(MAX, true);

	while(heap.size() > 0) {
		// pop an element from heap which d[i] is minimum
        auto top = heap.extract();
        int node = top->data.index;
        in_heap[node] = false;

		// all node is infinity, break out
		if(d[node] >= INF) break;

		// relax neighboring nodes
        for (int i = 0; i < num; ++i) {
            if (relax(node, i, graph[node][i]) && in_heap[i]) {
                heap.decreaseKey(handle[i], d[i]);
            }
        }
	}
}


int main()
{
    int node = 0;
    while (scanf("%d\n", &node) == 1) {
        // parse input
        for (int i = 1; i < node; ++i) {
            char buf[102400];
            fgets(buf, 102400, stdin);
            char* ptr = strtok(buf, " \t\r\n");
            for (int j = 0; (j < i) && (ptr != nullptr); ++j, ptr = strtok(nullptr, " \t\r\n")) {
                if (*ptr != 'x') { graph[i][j] = graph[j][i] = atoi(ptr); }
                else { graph[i][j] = graph[j][i] = INF; }
            }
        }

        // initialize
        for (int i = 0; i < node; ++i) { graph[i][i] = 0; d[i] = INF; }
        // find shortest paths from source to all other nodes
		Dijkstra(0, node);
        // find the longest shortest path
        int max_dist = 0;
        for (int i = 0; i < node; ++i) { max_dist = max(max_dist, d[i]); }
        printf("%d\n", max_dist);
    }

    return 0;
}

