#include <stdio.h>
#include <stdlib.h>


struct Queue {
    int front; // indeks pierwszego elementu w kolejce
    int rear;  // indeks ostatniego elementu w kolejce
    int size;  // aktualny rozmiar kolejki
    int capacity; // pojemność kolejki
    int* array; // tablica przechowująca elementy kolejki
};

struct Queue* createQueue(int capacity);
int isEmptyQ(struct Queue* queue);
int isFullQ(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int dequeue(struct Queue* queue);
int front(struct Queue* queue);
int rear(struct Queue* queue);
void display(struct Queue* queue);
void destroyQueue(struct Queue* queue);
