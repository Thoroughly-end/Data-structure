#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

typedef struct Node {
    int power;
    int number;
    struct Node *next, *pre;
} Node;

class Tamers {
    private:
        Node *head;
        int size;
    public:
        Tamers() {
            head = new Node;
            head->number = -1;
            head->power = -1;
            head->next = head;
            head->pre = head;
            size = 0;
        }

        ~Tamers() {
            Node *ptrhead = head;
            head = head->next;
            while (head != ptrhead) {
                Node *temp = head;
                head = head->next;
                delete temp;
            }
            delete head;
        }

        void add(int number, int power) {
            Node *previous = head->pre;
            Node *temp = new Node;
            temp->power = power;
            temp->pre = previous;
            temp->number = number;
            temp->next = previous->next;
            previous->next->pre = temp;
            previous->next = temp;
            ++size;
        }

        Node* front() {
            Node *temp = head->next;
            if(temp == head) {
                return nullptr;
            } else {
                return temp;
            }
        }

        Node* rear() {
            Node *temp = head->pre;
            if(temp == head) {
                return nullptr;
            } else {
                return temp;
            }
        }

        void pop(Node *node) {
            node->pre->next = node->next;
            node->next->pre = node->pre;
            --size;
            delete node;
        }

        int length() {
            return size;
        }

        void through() {
            Node *temp = head->next;
            while (temp != head) {
                cout << temp->number << " " << temp->power << endl;
                temp = temp->next;
            }
            
        }
};

Node* find_min(Node *node1, Node *node2, int *from);
Node* find_max(Node *node1, Node *node2, int *from);

int main() {
    ifstream in("testcase4.txt");
    ofstream out("output4.txt");

    if(!in.is_open() || !out.is_open()) {
        cerr << "FILE IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;
    for(int repeat = 0;repeat < numOftestcase;++repeat) {
        int numOftamers = 0;
        in >> numOftamers;
        Tamers *tamers1 = new Tamers;
        Tamers *tamers2 = new Tamers;
        if(numOftamers <= 1) {
            out << numOftamers << endl;
            continue;
        }

        for(int i = 0;i < numOftamers;++i) {
            int power = 0;
            in >> power;
            tamers1->add(i + 1, power);
        }

        vector<bool> is_safe;

        for(int i = 0; i < numOftamers - 1;++i) {
            Node *minNode, *maxNode;
            int max_from = 0, min_from = 0;

            // find min
            Node *t1_front = tamers1->front();
            Node *t2_rear = tamers2->rear();

            minNode = find_min(t1_front, t2_rear, &min_from);

            //find max
            Node *t1_rear = tamers1->rear();
            Node *t2_front = tamers2->front();

            maxNode = find_max(t1_rear, t2_front, &max_from);

            int new_power = maxNode->power - minNode->power;

            if(new_power == 0) {

            }

            if(min_from == 1) {
                tamers1->pop(minNode);
            } else {
                tamers2->pop(minNode);
            }

            if(max_from == 1) {
                tamers1->pop(maxNode);
            } else {
                tamers2->pop(maxNode);
            }

            //find current min
            int dummy_from;
            Node* currentMinNode = find_min(tamers1->front(), tamers2->rear(), &dummy_from);
            
            if (currentMinNode == nullptr) {
                is_safe.push_back(true);
            } else {
                if (new_power > currentMinNode->power) {
                    is_safe.push_back(true);
                } else if (new_power == currentMinNode->power) {
                    if (maxNode->number > currentMinNode->number) {
                        is_safe.push_back(true);
                    } else {
                        is_safe.push_back(false);
                    }
                } else {
                    is_safe.push_back(false);
                }
            }

            tamers2->add(maxNode->number, new_power);
        }

        vector<bool> will_eat(numOftamers - 1);
        will_eat[numOftamers - 2] = true;

        for (int i = numOftamers - 3;i >= 0;--i) {
            if (is_safe[i]) {
                will_eat[i] = true;
            } else {
                will_eat[i] = !will_eat[i + 1];
            }
        }

        int battles = 0;
        for (int i = 0;i < numOftamers - 1;++i) {
            if (will_eat[i]) {
                ++battles;
            } else {
                break;
            }
        }
        
        out << numOftamers - battles << endl;

        delete tamers1;
        delete tamers2;
    }

    in.close();
    out.close();
    return 0;
}

Node* find_min(Node *node1, Node *node2, int *from) {
    Node *minNode;
    if(node1 == nullptr && node2 == nullptr) {
        *from = 0;
        return nullptr;
    }
    if(node1 == nullptr) {
        minNode = node2;
        *from = 2;
    } else if (node2 == nullptr){
        minNode = node1;
        *from = 1;
    } else {
        if(node1->power < node2->power) {
            minNode = node1;
            *from = 1;
        } else if(node1->power > node2->power) {
            minNode = node2;
            *from = 2;
        } else {
            if(node1->number < node2->number) {
                minNode = node1;
                *from = 1;
            } else {
                minNode = node2;
                *from = 2;
            }
        }
    }
    return minNode;
}

Node* find_max(Node *node1, Node *node2, int *from) {
    Node *maxNode;
    if(node1 == nullptr && node2 == nullptr) {
        *from = 0;
        return nullptr;
    }
    if(node1 == nullptr) {
        maxNode = node2;
        *from = 2;
    } else if (node2 == nullptr){
        maxNode = node1;
        *from = 1;
    } else {
        if(node1->power > node2->power) {
            maxNode = node1;
            *from = 1;
        } else if (node1->power < node2->power) {
            maxNode = node2;
            *from = 2;
        } else {
            if(node1->number < node2->number) {
                maxNode = node2;
                *from = 2;
            } else {
                maxNode = node1;
                *from = 1;
            }
        }
    }
    return maxNode;
}