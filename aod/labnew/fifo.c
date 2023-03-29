#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"


// funkcja tworząca nową kolejke
struct Queue* createQueue(int capacity) {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    queue->array = (int*) malloc(capacity * sizeof(int));
    return queue;
}

// funkcja sprawdzająca, czy kolejka jest pusta
int isEmptyQ(struct Queue* queue) {
    return queue->size == 0;
}

// funkcja sprawdzająca, czy kolejka jest pełna
int isFullQ(struct Queue* queue) {
    return queue->size == queue->capacity;
}

// funkcja dodająca element na koniec kolejki
void enqueue(struct Queue* queue, int item) {
    if (isFullQ(queue)) {
        printf("Kolejka jest pelna, element %d nie zostal dodany.\n", item);
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
}

// funkcja usuwająca element z początku kolejki
int dequeue(struct Queue* queue) {
    if (isEmptyQ(queue)) {
        printf("Kolejka jest pusta, nie mozna usunac elementu.\n");
        return -1;
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

// funkcja zwracająca wartość pierwszego elementu w kolejce bez usuwania go
int front(struct Queue* queue) {
    if (isEmptyQ(queue)) {
        printf("Kolejka jest pusta, nie ma pierwszego elementu.\n");
        return -1;
    }
    return queue->array[queue->front];
}

// funkcja zwracająca wartość ostatniego elementu w kolejce bez usuwania go
int rear(struct Queue* queue) {
    if (isEmptyQ(queue)) {
        printf("Kolejka jest pusta, nie ma ostatniego elementu.\n");
        return -1;
    }
    return queue->array[queue->rear];
}

// funkcja wyświetlająca zawartość kolejki
void display(struct Queue* queue) {
    int i;
    printf("Zawartosc kolejki: ");
    for (i = queue->front; i <= queue->rear; i++) {
        printf("%d ", queue->array[i]);
    }
    printf("\n");
}

// funkcja zwalniająca pamięć zajmowaną przez kolejkę
void destroyQueue(struct Queue* queue) {
    free(queue->array);
    free(queue);
}
