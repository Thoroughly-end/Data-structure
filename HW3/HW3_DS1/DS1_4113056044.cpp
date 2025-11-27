#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

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

        Node* constructHelper(vector<int>& inorder, vector<int>& postorder) {
            for(int i = 0;i < inorder.size();++i) {
                inIndexMap[inorder[i]] = i;
            }
            return buildHelper(inorder, 0, inorder.size() - 1, postorder, 0, postorder.size() - 1);
        }

        Node* buildHelper(vector<int>& inorder, int inStart, int inEnd, vector<int>& postorder, int postStart, int postEnd) {
            if (inStart > inEnd || postStart > postEnd) return nullptr;
            int rootVal = postorder[postEnd];
            Node *currentRoot = new Node(rootVal);

            int inRootIndex = inIndexMap[rootVal];
            int sizeOfLeft = inRootIndex - inStart;

            currentRoot->left = buildHelper(inorder, inStart, inRootIndex - 1, postorder, postStart, postStart + sizeOfLeft - 1);
            currentRoot->right = buildHelper(inorder, inRootIndex + 1, inEnd, postorder, postStart + sizeOfLeft, postEnd - 1);

            return currentRoot;
        }

        void preorderHelper(Node *node, string *s) {
            if(node == nullptr) return;
            *s += to_string(node->num);
            *s += " ";
            preorderHelper(node->left, s);
            preorderHelper(node->right, s);
        }

    public:
        BinaryTree(vector<int>& inorder, vector<int>& postorder) {
            root = constructHelper(inorder, postorder);
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
        
};

int main() {
    ifstream in("testcase1.txt");
    ofstream out("output1.txt");
    if(!in.is_open() || !out.is_open()) {
        cerr << "FILE IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;

    for(int testcase = 0;testcase < numOftestcase;++testcase) {
        vector<int> inorder;
        vector<int> postorder;
        int num = 0;
        in >> num;
        inorder.reserve(num);
        postorder.reserve(num);
        for(int i = 0;i < num;++i) {
            int val = 0;
            in >> val;
            inorder.push_back(val);
        }
        for(int i = 0;i < num;++i) {
            int val = 0;
            in >> val;
            postorder.push_back(val);
        }

        BinaryTree bt(inorder, postorder);
        string s = bt.printPreoder();
        out << s << "\n";
    }
    in.close();
    out.close();
    return 0;
}