#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAXSTACK_SIZE 100
#define MAXEXPR_SIZE 100
typedef enum {lparan, rparen, plus, minus, times, divide, mod, eos, operand} precedence;

precedence get_token(char *symbols, int *n, char *symbol){
    int index = (*n)++;
    *symbol = *(symbols + index);
    switch(*(symbols + index)){
        case '(': return lparan;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '*': return times;
        case '/': return divide;
        case '%': return mod;
        case '\0': return eos;
        default: return operand;
    }
    
}

typedef struct
{
    char *c;
    int top;
    int Max;
} Stack;

Stack* create(int Max){
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->c = (char *) malloc(sizeof(char)*Max);
    stack->top = -1;
    stack->Max = Max;
    return stack;
}

void push(Stack *stack, char element){
    stack->top += 1;
    char *c = stack->c;
    *(c + stack->top) = element;
    stack->c = c;
}

bool isFull(Stack *stack){
    if(stack->top >= (stack->Max - 1)){
        return true;
    } else {
        return false;
    }
}

bool isEmpty(Stack *stack){
    if(stack->top == -1){
        return true;
    } else {
        return false;
    }
}

char pop(Stack *stack){
    char *c = stack->c;
    char result = *(c + stack->top);
    stack->top -= 1;
    return result;
}
precedence peek(Stack *stack){
    char *c = stack->c;
    char result = *(c + stack->top);
    switch(result){
        case '(': return lparan;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '*': return times;
        case '/': return divide;
        case '%': return mod;
        case '\0': return eos;
        default: return operand;
    }
}

int isp(precedence token){
    switch(token){
        case lparan: return 0;
        case rparen: return 19;
        case plus: return 12;
        case minus: return 12;
        case times: return 13;
        case divide: return 13;
        case mod: return 13;
        case eos: return 0;
        default: return 0;
    }
}

int icp(precedence token){
    switch(token){
        case lparan: return 20;
        case rparen: return 19;
        case plus: return 12;
        case minus: return 12;
        case times: return 13;
        case divide: return 13;
        case mod: return 13;
        case eos: return 0;
        default: return 0;
    }
}

int main()
{
    char expr[MAXEXPR_SIZE];
    char *s = expr;
    //printf("Enter an infix expression: ");
    fgets(expr, 100, stdin);
    for(int i = 0;i < 100;i++){
        if(*(s + i) == '\n'){
            *(s + i) = '\0';
            break;
        }
    }
    
    Stack *stack = create(MAXSTACK_SIZE);
    int n = 0; //record position in expr
    char symbol = ' ';
    precedence token = get_token(s, &n, &symbol);
    push(stack, '\0');
    while(token != eos)
    {
        if(token == operand) {
            printf("%c", symbol);
        } else if (token == rparen) {
            while (peek(stack) != lparan)
            {
                printf("%c", pop(stack));
            }
            pop(stack); //pop '('
        } else {
            while (isp(peek(stack)) >= icp(token)){
                printf("%c", pop(stack));
            }
            push(stack, symbol);
        }
        token = get_token(s, &n, &symbol);
    }
    
    while(!isEmpty(stack) && peek(stack) != eos){
        printf("%c", pop(stack));
    }
    free(stack);
    return 0;
}