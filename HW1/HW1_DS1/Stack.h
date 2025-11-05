#ifndef DS1_STACK_H_
#define DS1_STACK_H_

typedef struct
{
    int *arr;
    int top;
    int Max;
} Stack;

Stack* create(int Max);
void push(Stack *stack, int element);
bool isFull(Stack *stack);
bool isEmpty(Stack *stack);
int pop(Stack *stack);

#endif