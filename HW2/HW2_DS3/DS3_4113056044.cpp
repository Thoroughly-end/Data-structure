#include <iostream>
#include <map>
#include <fstream>
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
        int size;
    public:
        Route() {
            head = new Node;
            head->hub = -1;
            head->next = head;
            head->pre = head;
            size = 0;
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
            ++size;
        }

        Node* exsist(int hub) {
            Node *temp = head;
            temp = temp->next;
            while(head != temp) {
                if(temp->hub == hub) {
                    return temp;
                }
                temp = temp->next;
            }
            return nullptr;
        }
};

class CQueue {
    private:
        Queue queue;
        int length;
    public:
        CQueue() {
            queue.front = nullptr;
            queue.rear = nullptr;
            length = 0;
        }

        ~CQueue() {
            while(queue.front != nullptr) {
                Node *temp = queue.front;
                queue.front = queue.front->next;
                delete temp;
            }
        }
        
        void enqueue(int hub) {
            Node *temp = new Node;
            temp->hub = hub;
            temp->next = nullptr;
            temp->pre = nullptr;
            if(queue.rear != nullptr) {
                queue.rear->next = temp;
            }
            queue.rear = temp;
            if(queue.front == nullptr) {
                queue.front = temp;
            }
            ++length;
        }

        int dequeue(void) {
            Node *temp = new Node;
            temp->hub = queue.front->hub;
            temp->next = nullptr;
            temp->pre = nullptr;
            Node *deleted = queue.front;
            queue.front = queue.front->next;
            delete deleted;
            --length;
            if(length == 0) queue.rear = nullptr;
            return temp->hub;
        }

        bool isEmpty() {
            return (length == 0) ? true : false;
        }
};

int base(Route *routes, Node** trategicHub, int routesNum, int hubNum, int islandNum);
int totalDistance(Route *routes, Node** trategicHub, int routesNum, int hubNum, int start_hub, int islandNum);

int main() {
    ifstream in;
    ofstream out;

    in.open("testcase3.txt");
    out.open("output3.txt");

    if(!in.is_open() || !out.is_open()) {
        cerr << "FILE IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;
    for(int repeat = 0;repeat < numOftestcase;++repeat) {
        int totalNumberOfisland = 0, numberOfroute = 0;
        in >> totalNumberOfisland >> numberOfroute;
        Route *routes = new Route[numberOfroute];
        for(int routeNumber = 0;routeNumber < numberOfroute;++routeNumber) {
            int num = 0;
            while(true) {
                in >> num;
                if(num != 0) {
                    routes[routeNumber].add(num);
                } else {
                    break;
                }
            }
        }
        Node **trategic_hub = new Node*[totalNumberOfisland];
        int numOfTrategichub = 0;
        for(int i = 1;i <= totalNumberOfisland;++i) {
            int exsistTimes = 0;
            for(int j = 0;j < numberOfroute;++j) {
                if(routes[j].exsist(i)) {
                    ++exsistTimes;
                }
            }
            if(1 < exsistTimes) {
                Node *temp = new Node;
                temp->hub = i;
                temp->next = temp;
                temp->pre = temp;
                trategic_hub[numOfTrategichub++] = temp;
            }
        }
        /*for(int i = 0;i < numOfTrategichub;++i) {
            cout << trategic_hub[i]->hub << " ";
        }*/
        //cout << endl;
        int basenum = 0;
        basenum = base(routes, trategic_hub, numberOfroute, numOfTrategichub, totalNumberOfisland);
        out << basenum << endl;

        delete []trategic_hub;
        delete []routes;
    }

    in.close();
    out.close();
    return 0;
}

int base(Route *routes, Node **trategicHub, int routesNum, int hubNum, int islandNum) {
    int base = 0, current_min = 0;
    current_min = totalDistance(routes, trategicHub, routesNum, hubNum, trategicHub[hubNum - 1]->hub, islandNum);
    base = trategicHub[hubNum - 1]->hub;
    //cout << base << ":" << current_min << endl;
    for(int i = hubNum - 2;i >= 0;--i) {
        int total_step = totalDistance(routes, trategicHub, routesNum, hubNum, trategicHub[i]->hub, islandNum);
        //cout << trategicHub[i]->hub << ":" << total_step << endl;
        if(total_step <= current_min) {
            base = trategicHub[i]->hub;
            current_min = total_step;
        }
    }
    return base;
}

int totalDistance(Route *routes, Node **trategicHub, int routesNum, int hubNum, int start_hub, int islandNum) {
    int *dist = new int[islandNum + 1];
    for(int i = 0;i < islandNum + 1;++i) {
        if(i != start_hub) {
            dist[i] = -1;
        } else {
            dist[i] = 0;
        }
    }
    
    CQueue queue;
    queue.enqueue(start_hub);
    while (!queue.isEmpty()) {
        int num = queue.dequeue();
        for(int i = 0;i < routesNum;++i) {
            Node *address = routes[i].exsist(num);
            if(address != nullptr) {
                int prevNum = address->pre->hub;
                int nextNum = address->next->hub;
                if(prevNum != -1) {
                    if(dist[prevNum] == -1) {
                        dist[prevNum] = dist[address->hub] + 1;
                        queue.enqueue(prevNum);
                    }
                }
                if(nextNum != -1) {
                    if(dist[nextNum] == -1) {
                        dist[nextNum] = dist[address->hub] + 1;
                        queue.enqueue(nextNum);
                    }
                }
            }
        }
    }
    
    int total_step = 0;
    
    for (int i = 0; i < hubNum; ++i) {
        int target_hub = trategicHub[i]->hub;
        if (target_hub != start_hub) {
            total_step += dist[target_hub];
        }
    }
    return total_step;
}