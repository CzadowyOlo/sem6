#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include <stdbool.h>
#include <time.h>

void dfsUtil(struct Graph* graph, int vertex, struct Stack* stack, int *clock, int *sorted_vertices, int *skipper) {
    graph->vertices[vertex].visited = true;
    graph->vertices[vertex].pre = ++(*clock);
    if(graph->num_vertices < 200){
        printf("%d ", vertex);
    }
    Node* node = graph->vertices[vertex].head;
    while (node != NULL) {
        int adj_vertex = node->vertex;
        if (!graph->vertices[adj_vertex].visited) {
            push(stack, adj_vertex);
            dfsUtil(graph, adj_vertex, stack, clock, sorted_vertices, skipper);
        }
        node = node->next;
    }
    graph->vertices[vertex].post = ++(*clock);
    sorted_vertices[*skipper] = vertex;
    (*skipper)--;
}

void dfs(struct Graph* graph, int start_vertex, struct Stack* stack, int* clock, int *sorted_vertices, int *skipper) {
    push(stack, start_vertex);
    while (!isEmpty(stack)) {
        int vertex = peek(stack);
        pop(stack);
        if (!graph->vertices[vertex].visited) {
            dfsUtil(graph, vertex, stack, clock, sorted_vertices, skipper);
        }
    }

    if(graph->num_vertices < 200){
        printf("\n");
    }
}

void dfsUtilR(struct Graph* graph, int vertex, struct Stack* stack, int *clock) {
    graph->vertices[vertex].visited = true;
    graph->vertices[vertex].pre = ++(*clock);
    if(graph->num_vertices < 200){
        printf("%d ", vertex);
    }
    Node* node = graph->vertices[vertex].head;
    while (node != NULL) {
        int adj_vertex = node->vertex;
        if (!graph->vertices[adj_vertex].visited) {
            push(stack, adj_vertex);
            dfsUtilR(graph, adj_vertex, stack, clock);
        }
        node = node->next;
    }
    graph->vertices[vertex].post = ++(*clock);
}

void dfsR(struct Graph* graph, int start_vertex, struct Stack* stack, int* clock) {
    push(stack, start_vertex);
    while (!isEmpty(stack)) {
        int vertex = peek(stack);
        pop(stack);
        if (!graph->vertices[vertex].visited) {
            dfsUtilR(graph, vertex, stack, clock);
        }
    }

    if(graph->num_vertices < 200){
        printf("\n");
    }
}

void sccs(struct Graph* g, struct Graph* r, int *skladowe, int *sorted_vertices){
    int clock = 0;
    struct Stack* stack;
    stack = createStack(g->num_vertices);
    int size = g->num_vertices + 1;

    for(int i = 1; i < size; i++){
        if(!r->vertices[sorted_vertices[i]].visited){
            (*skladowe)++;
            //restore(stack);
            if(size < 200){
                printf("składowa od: %d\n", sorted_vertices[i]);
            }
            dfsR(r, sorted_vertices[i], stack, &clock);
        }
    }

    free(stack->array);
    free(stack);
}

void bfs(struct Graph* graph, int start_vertex, struct Queue* queue, bool* cykl) {

    graph->vertices[start_vertex].visited = true;
    enqueue(queue, start_vertex);

    while (!isEmptyQ(queue)) {
        int vertex = dequeue(queue);
        if(graph->num_vertices < 200){
            printf("%d ", vertex);
        }

        Node* node = graph->vertices[vertex].head;
        while (node != NULL) {
            int adj_vertex = node->vertex;
            if (!graph->vertices[adj_vertex].visited) {
                graph->vertices[adj_vertex].visited = true;
                enqueue(queue, adj_vertex);
            }
            else if (graph->vertices[adj_vertex].visited && adj_vertex != vertex)
            {
                *cykl = true;
                if(graph->num_vertices < 200){
                    printf("\n znaleziono cykl skierowny\n");
                }
            }
            node = node->next;
        }
    }
    printf("\n");
}



int main(int argc, char* argv[argc+1]){
    clock_t start, end, overall;
    double cpu_time_used;
    
    FILE* file = fopen(argv[1], "r"); // otwarcie pliku w trybie tylko do odczytu
    if (file == NULL) {
        printf("Nie udalo sie otworzyc pliku!\n");
        return 1; // błąd otwarcia pliku
    }

    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    struct Graph* r = (struct Graph*)malloc(sizeof(struct Graph));
    //printf("dupa2\n");
    printf("\nWczytywanie grafu G z pliku...\n");
    read_graph(g, file);
    //printf("dupa3\n");

    rewind(file);
    //printf("dupa4\n");
    printf("\nOdwracanie grafu G na R...\n");

    reverse_graph(r, file);
    //printf("dupa5\n");

    fclose(file);
    int myclock = 0;
    struct Stack* stack;
    stack = createStack(g->num_vertices);
    struct Queue* queue;
    queue = createQueue(g->num_vertices);

    printf("\nodczytano:\nSkierowany: %d\nWierzchołki: %d\nKrawędzie: %d\n",g->directed, g->num_vertices, g->num_edges);
    
    if(g->num_vertices < 200){    
        printf("\nlista sąsiedztwa G\n");
        print_list(g);
        printf("\nlista sąsiedztwa R\n");
        print_list(r);
    }

    printf("Kolejność odwiedzin w DFS:\n");
    int skipper = g->num_vertices;
    int* sorted_vertices = (int*)malloc((g->num_vertices + 1) * sizeof(int));
    start = clock();
    dfs(g, 1, stack, &myclock, sorted_vertices, &skipper);
    end = clock();
    overall = end - start;
    printf("\nPorządek pre - post na G:\n");
    for (int i = 1; i <= g->num_vertices; i++) {
        g->vertices[i].visited = false;
        if(g->num_vertices < 200){
            printf("Wierzchołek: %d, Pre: %d, Post: %d\n", i, g->vertices[i].pre, g->vertices[i].post);
        }
    }
    bool cykl = false;
    printf("\nKolejność odwiedzin w BFS:\n");
    start = clock();
    bfs(g, 1, queue, &cykl);
    end = clock();
    overall = overall + end - start;
    printf("\nCzy graf zawiera cykl: %d\n", cykl);
    for (int i = 1; i <= g->num_vertices; i++) {
    g->vertices[i].visited = false;
    }

    printf("liczenie składowych\n");
    int dupa = 0;
    start = clock();
    sccs(g,r, &dupa, sorted_vertices);
    end = clock();
    overall = overall + end - start;

    printf("Liczba spojnych skladowych G: %d\n", dupa);
    printf("\nCzyszczenie pamięci...\n");
    //printf("dupa6\n");
    destroy_graph(g);
    //printf("dupa7\n");
    free(g);
    //printf("dupa8\n");
    destroy_graph(r);
    //printf("dupa9\n");
    free(r);
    //printf("dupa10\n");
    free(stack->array);
    free(stack);
    free(sorted_vertices);
    destroyQueue(queue);
    cpu_time_used = ((double) (overall)) / CLOCKS_PER_SEC;
    printf("Funkcje DFS, BFS i SCC zajęły: %f sekund\n\n", cpu_time_used);
    // printf("\ndestroyed\n");
    // fclose(file);

    //valgrind --max-stackframe=640000176 --main-stacksize=838860800 --leak-check=full --show-leak-kinds=all ./mydfs.out aod_testy1/3/g3-6.txt
return 0;
}
