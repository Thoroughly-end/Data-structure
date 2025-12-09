#include <iostream>
#include<fstream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef enum {pour, sip, lookup} operation;

class BinaryTree {
    private:
        typedef struct Node {
            int num;
            Node *left, *right;
            Node(int val) : num(val), left(nullptr), right(nullptr) {}
        } Node;
        Node *root;

        unordered_map<int, int> inIndexMap;

        void deleteHelper(Node *node) {
            if(node == nullptr) return;
            deleteHelper(node->left);
            deleteHelper(node->right);
            delete node;
        }

        Node* constructHelper(vector<int>& inorder, vector<int>& preorder) {
            for(int i = 0;i < inorder.size();++i) {
                inIndexMap[inorder[i]] = i;
            }
            return buildHelper(inorder, 0, inorder.size() - 1, preorder, 0, preorder.size() - 1);
        }

        Node* buildHelper(vector<int>& inorder, int inStart, int inEnd, vector<int>& preorder, int preStart, int preEnd) {
            if (inStart > inEnd || preStart > preEnd) return nullptr;
            int rootVal = preorder[preStart];
            Node *currentRoot = new Node(rootVal);

            int inRootIndex = inIndexMap[rootVal];
            int sizeOfLeft = inRootIndex - inStart;

            currentRoot->left = buildHelper(inorder, inStart, inRootIndex - 1, preorder, preStart + 1, preStart + sizeOfLeft);
            currentRoot->right = buildHelper(inorder, inRootIndex + 1, inEnd, preorder, preStart + sizeOfLeft + 1, preEnd);

            return currentRoot;
        }

        void preorderHelper(Node *node, string *s) {
            if(node == nullptr) return;
            *s += to_string(node->num);
            *s += " ";
            preorderHelper(node->left, s);
            preorderHelper(node->right, s);
        }

        vector<int> in, out, fill, drain;
        int Timer;

        void flattenHelper(Node *node, int& timer) {
            if(node == nullptr) return;

            ++timer;
            in.at(node->num) = timer;

            if(node->left != nullptr) flattenHelper(node->left, timer);
            if(node->right != nullptr) flattenHelper(node->right, timer);

            out.at(node->num) = timer;
        }

        void timerAdd() {++Timer;}
        int now() {return Timer;}

        void updatePour(int& node) {
            int start = in.at(node);
            int end = out.at(node);
            for(int i = start;i <= end;++i) {
                fill.at(i) = now();
            }
        }
        void updateDrain(int& node) {
            drain.at(node) = now();
        }

        int getPourTime(int& node) {return fill.at(node);}
        int getDrainTime(int & node) {
            int start = in.at(node);
            int end = out.at(node);
            int time = 0;
            for(int i = start;i <= end;++i) {
                time = max(drain.at(i), time);
            }
            return time;
        }

    public:
        BinaryTree(vector<int>& inorder, vector<int>& preorder) {
            root = constructHelper(inorder, preorder);

            in.resize(inorder.size() + 1);
            out.resize(inorder.size() + 1);
            in.at(0) = 0;
            out.at(0) = 0;

            int timer = 0;
            Timer = 0;
            flattenHelper(root, timer);

            fill.assign(inorder.size() + 1, 0);
            drain.assign(inorder.size() + 1, 0);

        }
        ~BinaryTree() {
            deleteHelper(root);
        }

        string printPreoder() {
            string s = "";
            if(root == nullptr) return s;
            preorderHelper(root, &s);
            return s;
        }

        bool operate(operation oper, int node) {
            timerAdd();
            if(oper == pour) {
                updatePour(node);
                return false;
            } else if(oper == sip) {
                updateDrain(node);
                return false;
            } else {
                int fill_time = getPourTime(node);
                int drain_time = getDrainTime(node);
                if(fill_time > drain_time) {
                    return true;
                } else {
                    return false;
                }
            }
        }
};

int main() {
    ifstream in("testcase3.txt");
    ofstream out("output3.txt");

    if(!in.is_open() || !out.is_open()) {
        cerr << "FILE IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;
    for(int testcase = 0;testcase < numOftestcase;++testcase) {
        int num = 0;
        in >> num;

        vector<int> inorder, preorder;
        inorder.reserve(num);
        preorder.reserve(num);

        for(int i = 0;i < num;++i) {
            int val = 0;
            in >> val;
            inorder.push_back(val);
        }
        for(int i = 0;i < num;++i) {
            int val = 0;
            in >> val;
            preorder.push_back(val);
        }

        BinaryTree bt(inorder, preorder);
        //cout << bt.printPreoder() << endl;

        int numOfoperation = 0;
        in >> numOfoperation;
        for(int i = 0;i < numOfoperation;++i) {
            int x = 0, node = 0;
            operation oper;
            in >> x >> node;
            switch (x) {
                case 1:
                    oper = pour;
                    bt.operate(oper, node);
                    break;
                case 2:
                    oper = sip;
                    bt.operate(oper, node);
                    break;
                default:
                    oper = lookup;
                    bool result = bt.operate(oper, node);
                    out << result << "\n";
                    break;
            }

        }
    }
    return 0;
}