typedef struct _IO_FILE FILE;
#define MAX_VERTICES 10000000
#include <stdbool.h>

typedef struct node {
    int vertex;
    char dupa[4];
    struct node* next;
} Node;

typedef struct {
    bool visited;
    char dupa[3];
    int pre;
    int post;
    int degree;
    Node* head;
    Node* tail;
} Vertex;

struct Graph{
    int num_vertices;
    int num_edges;
    int directed;
    char dupa[4];
    Vertex *vertices;
};

void destroy_graph(struct Graph* g);
void read_graph(struct Graph* g, FILE* f);
void reverse_graph(struct Graph* g, FILE* f);
void print_list (struct Graph* g);
