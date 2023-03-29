
struct Stack {
    int top;
    int capacity;
    int* array;
};
struct Stack* createStack(int capacity);
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
void push(struct Stack* stack, int item);
void pop(struct Stack* stack);
int getres(struct Stack* stack);
void restore(struct Stack* stack);
void print_result(struct Stack* stack);
int peek(struct Stack* stack);
void error_occurence(char* dupa);
