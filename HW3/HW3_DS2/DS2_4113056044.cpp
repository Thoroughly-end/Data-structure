#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

class BinaryTree {
    private:
        typedef struct Node {
            int num;
            Node *left, *right;
            Node(int val) : num(val), left(nullptr), right(nullptr) {}
        } Node;
        Node *root;

        void deleteHelper(Node *node) {
            if(node == nullptr) return;
            deleteHelper(node->left);
            deleteHelper(node->right);
            delete node;
        }

        /*void preorderHelper(Node *node) {
            if(node == nullptr) return;
            cout << node->num << " ";
            preorderHelper(node->left);
            preorderHelper(node->right);
        }*/

        int stations;
        short int dfs(Node *node) {
            if(node == nullptr) return 2;

            short int left = dfs(node->left);
            short int right = dfs(node->right);

            if(left == 0 || right == 0) {
                ++stations;
                return 1;
            }

            if(left == 1 || right == 1) {
                return 2;
            }

            return 0;
        }

    public:
        BinaryTree(string& s) {
            stations = 0;

            stringstream ss(s);
            queue<Node*> q;
            string token;
            getline(ss, token, ',');
            if(token == "N") {
                root = nullptr;
                return;
            } else if(token == "0") {
                root = new Node(0);
                q.push(root);
            }
            while(!q.empty() && !ss.eof()) {
                Node *current = q.front();
                q.pop();
                getline(ss, token, ',');
                if(token == "0") {
                    current->left = new Node(0);
                    q.push(current->left);
                }
                getline(ss, token, ',');
                if(token == "0") {
                    current->right = new Node(0);
                    q.push(current->right);
                }
            }
        }
        ~BinaryTree() {
            deleteHelper(root);
        }

        /*void printPreoder() {
            if(root == nullptr) return;
            preorderHelper(root);
            cout << endl;
        }*/
        
        int minNumOfStations() {
            if(root == nullptr) return 0;

            if(dfs(root) == 0) {
                ++stations;
                return stations;
            }

            return stations;
        }
};

int main() {
    ifstream in("testcase2.txt");
    ofstream out("output2.txt");

    if(!in.is_open() || !out.is_open()) {
        cerr << "FILE IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;
    in.clear();
    in.ignore(100, '\n');

    for(int testcase = 0;testcase < numOftestcase;++testcase) {
        string s;
        getline(in, s);
        BinaryTree bt(s);
        //bt.printPreoder();
        out << bt.minNumOfStations() << endl;
    }

    in.close();
    out.clear();
    return 0;
}