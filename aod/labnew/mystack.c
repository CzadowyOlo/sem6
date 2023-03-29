// Aleksander Głowacki 
// stack

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <tgmath.h>
#include <time.h>
#include "mystack.h"
#include <string.h>
#include <stdbool.h>


static int bottom;
static int top;
static int wynik;
static bool error = false;

void error_occurence(char* dupa){
    printf("Wystąpił błąd: %s\n", dupa);
    error = true;
}

// A structure to represent a stack

  
// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack* createStack(int capacity)
{
    assert(capacity >= 0);
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}
  
// Stack is full when top is equal to the last index
int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}
  
// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}
  
// Function to add an item to stack.  It increases top by 1
void push(struct Stack* stack, int item)
{
    if (isFull(stack)){
        printf("capacity overflow\n");
        return;
    }
    stack->array[++stack->top] = item;
    //printf("%d polożono na stack\n", item);
}
  
// Function to remove an item from stack.  It decreases top by 1
void pop(struct Stack* stack)
{
    if (isEmpty(stack)){
        error_occurence("za malo liczb na stosie");
        return;
    }
    top =  stack->array[stack->top--];
    //printf("%d zdjęto ze stacka\n", top);
    if (isEmpty(stack)){
        //error_occurence("za malo liczb na stosie");
        return;
    }
    bottom =  stack->array[stack->top--];
    //printf("%d zdjęto ze stacka\n", bottom);


}

int getres(struct Stack* stack)
{
    if (isEmpty(stack)){
        error = true;
        return INT_MIN;
    }
    wynik = stack->array[stack->top--];
    //printf("%d zdjęto ze stacka\n", wynik);
    return wynik;
}

void restore(struct Stack* stack){
    int dupa;
    while(!isEmpty(stack)){
        dupa = getres(stack);
        printf("restore: %d\n", dupa);
    }
}

void print_result(struct Stack* stack){
    if (isEmpty(stack)){
        error_occurence("brak liczb na stosie");
        return;
    }
    wynik = getres(stack);
    if (!isEmpty(stack)){
        error_occurence("za mało operatorów");
        return;
    }    
    //printf("oto wynik = %d\n", wynik);
}
  
// Function to return the top from stack without removing it
int peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}
