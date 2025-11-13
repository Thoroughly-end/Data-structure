#include<iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Node {
    int order, num;
    struct Node *prev, *next;
};
typedef Node Node;
struct LinkedList {
    Node *head;
};

class CLinkedList {
    private:
        struct LinkedList list;
        Node *ptr;
        Node *Remember;
        int size;
    public:
        CLinkedList() {
            Node *node = new Node;
            node->next = node;
            node->prev = node;
            node->num = -1;
            node->order = -1;
            list.head = node;
            ptr = list.head;
            Remember = NULL;
            size = 0;
        }
        ~CLinkedList() {
            Node *head;
            head = list.head;
            list.head = list.head->next;
            while (list.head != head){
                struct Node *temp;
                temp = list.head;
                list.head = list.head->next;
                delete(temp);
            }
            delete(head);
        }

        void add(int order, int num) {
            // add node at the endl of it
            Node *previous = list.head->prev;
            Node *temp = new Node;
            temp->num = num;
            temp->order = order;
            temp->prev = previous;
            temp->next = previous->next;
            previous->next->prev = temp;
            previous->next = temp;
            ++size;
        }

        void remove() {
            //discard the node
            if(list.head == ptr) {
                cout << "Detetion of head node not permitted." << endl;
            } else {
                Node *node = ptr;
                ptr = ptr->prev;
                node->prev->next = node->next;
                node->next->prev = node->prev;
                --size;
                delete(node);
            }
        }

        Node* next() {
            //get next node address
            if(CLinkedList::length() == 0) {
                return NULL;
            } else {
                ptr = ptr->next;
                if(ptr == list.head) ptr = ptr->next;
                return ptr;
            }
        }
        
        int length() {
            /*Node *temp = list.head->next;
            int count = 0;
            while (temp != list.head)
            {
                temp = temp->next;
                ++count;
            }*/
            //reduce the time complexcity
            return size;
        }
        Node* Last_remembered() {
            return Remember;
        }

        void remember(Node *node) {
            Remember = node;
        }
        void forget() {
            Remember = NULL;
        }
};

int gcd(int a, int b);

int main() {
    CLinkedList linkedlist;
    ifstream in;
    ofstream out;
    in.open("testcase2.txt");
    out.open("output2.txt");
    if(!in.is_open() || !out.is_open()) {
        cout << "IO Exception!!!" << endl;
        exit(-1);
    }

    int numOftestcase = 0;
    in >> numOftestcase;
    for(int i = 0;i < numOftestcase;++i) {
        int num = 0;
        in >> num;
        int *arr = new int[num];
        int *result = new int[num];
        for(int j = 0;j < num;++j) {
            //initialize
            result[j] = -1;
        }

        CLinkedList linkedlist;
        for(int j = 0;j < num;++j) {
            in >> arr[j];
            linkedlist.add(j + 1, arr[j]);
        }
        int index = 0, times = 0;
        while (true)
        {   
            Node *current = linkedlist.next();
            if(linkedlist.Last_remembered() == NULL) {
                linkedlist.remember(current);
                continue;
            }
            if(gcd(current->num, linkedlist.Last_remembered()->num) == 1) {
                result[index++] = current->order;
                //cout << current->num << " " << linkedlist.Last_remembered()->num << endl;
                linkedlist.remove();
                linkedlist.forget();
                times = 0;
            } else {
                linkedlist.remember(current);
                ++times;
            }
            if(times == linkedlist.length()) break;
        }
        
        int length = 0;
        for(int j = 0;j < num;++j) {
            if(result[j] == -1) {
                break;
            }
            ++length;
        }
        
        if(length == 0) {
            out << length << endl;
        } else {
            out << length << " ";
        }
        for(int j = 0;j < length;++j) {
            out << result[j];
            if(j == length - 1) {
                out << endl;
            } else {
                out << " ";
            }
        }
        delete [] arr;
        delete [] result;
    }
    in.close();
    out.close();
    return 0;
}

int gcd(int a, int b) {
    if(b == 0) {
        return a;
    } else {
        return gcd(b, a%b);
    }
}