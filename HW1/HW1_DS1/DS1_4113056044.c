#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

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
int last(Stack *stack);

int main(){
    FILE *fp_r;
    FILE *fp_w;
    fp_r = fopen("testcase1.txt", "r");
    fp_w = fopen("output1.txt", "w");
    if (fp_r == NULL || fp_w == NULL) {
        printf("error\n");
        return -1;
    }
    
    int numOftestcase = 0;
    fscanf(fp_r, "%d", &numOftestcase);
    for(int i = 0;i < numOftestcase;i++){
        int num = 0;
        fscanf(fp_r, "%d", &num);
        int *nums = NULL;
        nums = (int *)malloc(sizeof(int)*num);
        for(int i = 0;i < num;i++){
            fscanf(fp_r, "%d", (nums + i));
        }
        Stack *stack = create(num);
        int good = 1;
        bool ans = false;
        for(int i = 0;i < num;){
            if(*(nums + i) == last(stack)){
                pop(stack);
                i++;
            } else {
                push(stack, good);
                good++;
            }
            if(good > num && last(stack)!=*(nums + i)){
                break;
            }
            
            if(i == num - 1 && isEmpty(stack)){
                ans = true;
            }
        }
        if(ans){
            fprintf(fp_w, "%s\n", "Yes");
        } else {
            fprintf(fp_w, "%s\n", "No");
        }

        free(nums);
        free(stack->arr);
        free(stack);
    }
    fclose(fp_r);
    fclose(fp_w);
}

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

int last(Stack *stack){
    if(stack->top != -1){
        int *p = stack->arr;
        int num = *(p + stack->top);
        return num;
    } else {
        return -1;
    }
}