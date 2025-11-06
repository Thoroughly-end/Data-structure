#include<iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

typedef struct Node {
    int num;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

class CStack {
    private:
        Stack stack;
    public:
        CStack() {
            stack.top = NULL;
        };

        ~CStack() {
            while (stack.top != NULL) {
                Node *temp = stack.top;
                stack.top = stack.top->next;
                free(temp);
            }
        };

        void push(int num) {
                Node *node = (Node *) malloc(sizeof(Node));
                node->next = stack.top;
                node->num = num;
                stack.top = node;
        }

        int pop() {
            if(!stack.top){
                cout << "No item!";
                exit(-1);
            } else {
                int num = stack.top->num;
                Node *temp = stack.top;
                stack.top = stack.top->next;
                free(temp);
                return num;
            }
        }

        int peek() {
            return stack.top->num;
        }
        
        bool isEmpty(){
            return stack.top == NULL;
        }
};

int main(){
    ifstream in;
    ofstream out;
    in.open("testcase1.txt");
    out.open("output1.txt");

    int numOftestcase = 0;
    in >> numOftestcase;

    for(int i = 0;i < numOftestcase;i++){
        CStack stack;
        int num = 0;
        in >> num;
        
        int *goods = new int[num];
        for(int j = 0;j < num;j++) {
            in >> goods[j];
        } 
        int index = 0;

        for(int good = 1;good <= num;good++){
            stack.push(good);
            while (!stack.isEmpty() && stack.peek() == goods[index]) {
                stack.pop();
                ++index;
            }
            
        }
        
        if(stack.isEmpty()) {
            out << "Yes" << endl;
        } else {
            out << "No" << endl;
        }
        delete [] goods;
    }
    
    in.close();
    out.close();
    return 0;
}