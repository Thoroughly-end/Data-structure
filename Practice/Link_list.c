#include<stdio.h>
#include<stdlib.h>

struct node
{
    int val;
    struct node *next;
};
typedef struct node Node;

void print_link_list(Node *head);
Node* input_link_list(int num);
void insert_link_list(Node *head, int idx,int value);

int main(){
    int num = 4;
    Node *ipt;
    ipt = input_link_list(num);
    print_link_list(ipt);
    return 0;
}

Node* input_link_list(int num){
    Node *head, *current, *previous;
    for(int i = 0;i < num;i++){
        current = (Node *) malloc(sizeof(Node));
        scanf("%d", &(current->val));
        if(i == 0){
            head = current;
        }else{
            previous->next = current;
        }
        current->next = NULL;
        previous = current;
    }
    return head;
}

void print_link_list(Node *head){
    while (head != NULL){
        printf("%d\n", head->val);
        head = head->next;
    }
}

void insert_link_list(Node *head, int idx,int value){
    
}