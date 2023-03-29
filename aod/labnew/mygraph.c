#include <stdio.h>
#include <stdlib.h>
#include "mygraph.h"

// #define MAX_VERTICES 10000

// funkcja wpisująca krawędzie do grafu
void read_graph(struct Graph* g, FILE* f) {
    int i, u, v;
    char dir_flag;

    // odczytanie flagi kierunku grafu
    fscanf(f, "%c\n", &dir_flag);
    //dir_flag = 'D';
    if (dir_flag == 'D') {
        g->directed = 1;
    } else {
        g->directed = 0;
    }

    // odczytanie liczby wierzchołków i krawędzi
    fscanf(f, "%d %d\n", &(g->num_vertices), &(g->num_edges));

    Vertex* vertices;
    vertices = malloc(sizeof(Vertex) * ((g->num_vertices)+1));
    g->vertices = vertices;

    //g->num_edges = 16;
    //g->num_vertices = 16;
    
    //printf("\nnum ver: %d\n", g->num_vertices);
    // inicjalizacja list sąsiedztwa
    for (i = 1; i <= g->num_vertices; i++) {
        (g->vertices[i].head) = NULL;
        (g->vertices[i].tail) = NULL;
        (g->vertices[i].degree) = 0;
        (g->vertices[i].visited) = false;

    }

    // dodanie krawędzi do list sąsiedztwa
    for (i = 1; i <= g->num_edges; i++) {
        fscanf(f, "%d %d\n", &u, &v);
       
        // dodanie krawędzi (u, v)
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->vertex = v;
        new_node->next = NULL;

        Node* current_node = g->vertices[u].head;
        if (current_node == NULL) {
            g->vertices[u].head = new_node;
            g->vertices[u].tail = new_node;

        } else {
            g->vertices[u].tail->next = new_node;
            g->vertices[u].tail = new_node;
            // while (current_node->next != NULL) {
            //     current_node = current_node->next;
            // }
            // current_node->next = new_node;
        }
        g->vertices[u].degree++;

        // jeśli graf nieskierowany, dodaj też krawędź (v, u)
        if (!(g->directed)) {
            Node* new_nodeb = (Node*)malloc(sizeof(Node));
            new_nodeb->vertex = u;
            new_nodeb->next = NULL;

            current_node = g->vertices[v].head;
            if (current_node == NULL) {
            g->vertices[v].head = new_nodeb;
            g->vertices[v].tail = new_nodeb;

        } else {
            g->vertices[v].tail->next = new_nodeb;
            g->vertices[v].tail = new_nodeb;
        }
            g->vertices[v].degree++;
        }
    }
}

void reverse_graph(struct Graph* g, FILE* f){    
    int i, u, v;
    char dir_flag;

    // odczytanie flagi kierunku grafu
    fscanf(f, "%c\n", &dir_flag);
    //dir_flag = 'D';
    if (dir_flag == 'D') {
        g->directed = 1;
    } else {
        g->directed = 0;
    }

    // odczytanie liczby wierzchołków i krawędzi
    fscanf(f, "%d %d\n", &(g->num_vertices), &(g->num_edges));
    Vertex* vertices;
    vertices = malloc(sizeof(Vertex) * ((g->num_vertices)+1));
    g->vertices = vertices;
    //g->num_edges = 16;
    //g->num_vertices = 16;
    
    //printf("\nnum ver: %d\n", g->num_vertices);
    // inicjalizacja list sąsiedztwa
    for (i = 1; i <= g->num_vertices; i++) {
        (g->vertices[i].head) = NULL;
        (g->vertices[i].tail) = NULL;
        (g->vertices[i].degree) = 0;
        (g->vertices[i].visited) = false;

    }

    // dodanie krawędzi do list sąsiedztwa
    for (i = 1; i <= g->num_edges; i++) {
        fscanf(f, "%d %d\n", &v, &u);
       
        // dodanie krawędzi (u, v)
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->vertex = v;
        new_node->next = NULL;

        Node* current_node = g->vertices[u].head;
        if (current_node == NULL) {
            g->vertices[u].head = new_node;
            g->vertices[u].tail = new_node;

        } else {
            g->vertices[u].tail->next = new_node;
            g->vertices[u].tail = new_node;
            // while (current_node->next != NULL) {
            //     current_node = current_node->next;
            // }
            // current_node->next = new_node;
        }
        g->vertices[u].degree++;
    }
}

void destroy_graph(struct Graph* g) {
    int i;
    Node* curr_node;
    Node* temp_node;

    // zwolnienie pamięci zajmowanej przez listy sąsiedztwa
    for (i = 1; i <= g->num_vertices; i++) {
        curr_node = g->vertices[i].head;
        while (curr_node != NULL) {
            temp_node = curr_node;
            curr_node = curr_node->next;
            free(temp_node);
        }
        g->vertices[i].head = NULL;
    }
    g->num_vertices = 0;
    g->num_edges = 0;
    g->directed = 0;
    free(g->vertices);
}

void print_list (struct Graph* g){
    Node* temp_node = NULL;

    for(int i = 1; i <= g->num_vertices ; i++){

        printf("wierchołek: %d; sąsiedzi: ", i);
        temp_node = g->vertices[i].head;

        if (temp_node == NULL) {
            printf("\n"); // Lista sąsiedztwa jest pusta
        }

        else{
            printf("%d, ", temp_node->vertex);
            temp_node = temp_node->next;

            while(temp_node != NULL){
                printf("%d, ", temp_node->vertex);
                temp_node = temp_node->next;
            }

            printf("\n");
        }

    }
}
