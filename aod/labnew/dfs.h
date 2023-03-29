#include "mygraph.h"
#include "mystack.h"
#include <stdbool.h>
#include "fifo.h"
#include "quick.h"

void dfsUtil(struct Graph* graph, int vertex, struct Stack* stack, int *clock, int *sorted_vertices, int *skipper);
void dfs(struct Graph* graph, int start_vertex, struct Stack* stack, int *clock, int *sorted_vertices, int *skipper);
void bfs(struct Graph* graph, int start_vertex, struct Queue* queue, bool *cykl);
void sccs(struct Graph* g, struct Graph* r, int *skladowe, int *sorted_vertices);
void dfsR(struct Graph* graph, int start_vertex, struct Stack* stack, int* clock);
void dfsUtilR(struct Graph* graph, int vertex, struct Stack* stack, int *clock);
