#include<stdlib.h>
#include <stdbool.h>
#include "Stack.h"

Stack* create(int Max){
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->arr = (int *) malloc(sizeof(int)*Max);
    stack->top = -1;
    stack->Max = Max;
    return stack;
}

void push(Stack *stack, int element){
    stack->top += 1;
    int *p = stack->arr;
    *(p + stack->top) = element;
    stack->arr = p;
}

bool isFull(Stack *stack){
    if(stack->top >= (stack->Max - 1)){
        return true;
    } else {
        return false;
    }
}

bool isEmpty(Stack *stack){
    if(stack->top = -1){
        return true;
    } else {
        return false;
    }
}

int pop(Stack *stack){
    int *p = stack->arr;
    int result = *(p + stack->top);
    stack->top -= 1;
    return result;
}