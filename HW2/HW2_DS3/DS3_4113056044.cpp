#include <iostream>

using namespace std;

typedef struct Node {
    int hub;
    struct Node *next, *pre;
} Node;

typedef struct Queue {
    Node *front, *rear;
} Queue;

class Route {
    private:
        Node *head;
    public:
        Route() {
            head = new Node;
            head->hub = -1;
            head->next = head;
            head->pre = head;
        }

        ~Route() {
            Node *ptrhead = head;
            head = head->next;
            while (head != ptrhead) {
                Node *temp = head;
                head = head->next;
                delete temp;
            }
            delete head;
        }

        void add(int hub) {
            Node *previous = head->pre;
            Node *temp = new Node;
            temp->hub = hub;
            temp->pre = previous;
            temp->next = previous->next;
            previous->next->pre = temp;
            previous->next = temp;
        }
};

int main() {
    int numOftestcase = 0;
    cin >> numOftestcase;
    for(int repeat = 0;repeat < numOftestcase;++repeat) {
        int totalNumberOfisland = 0, numberOfroute = 0;
        cin >> totalNumberOfisland >> numberOfroute;
        Route *routes = new Route[numberOfroute];
        for(int routeNumber = 0;routeNumber < numberOfroute;++routeNumber) {
            int num = 0;
            do {
                cin >> num;
                routes[routeNumber].add(num);
            } while(num != 0);
        }
        

        delete []routes;
    }
    return 0;
}