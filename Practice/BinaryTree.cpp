#include <iostream>
#include <cstdlib>

using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node *root;
    Node* insertHelper(Node* node, int val) {
        if(node == nullptr) {
            return new Node(val);
        }

        if(val < node->data) {
            node->left = insertHelper(node->left, val);
        } else {
            node->right = insertHelper(node->right, val);
        }

        return node;
    }

    bool searchHelper(Node* node, int val) {
        if (node == nullptr) {
            return false;
        }

        if (node->data == val) {
            return true;
        }

        if (val < node->data) {
            return searchHelper(node->left, val); 
        } else {
            return searchHelper(node->right, val); 
        }
    }

    void destroyTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void inorder(Node *ptr) {
        if(ptr) {
            inorder(ptr->left);
            cout << ptr->data << " ";
            inorder(ptr->right);
        }
    }

    void preorder(Node *ptr) {
        if(ptr) {
            cout << ptr->data << " ";
            inorder(ptr->left);
            inorder(ptr->right);
        }
    }

    void postorder(Node *ptr) {
        if(ptr) {
            inorder(ptr->left);
            inorder(ptr->right);
            cout << ptr->data << " ";
        }
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() {
        destroyTree(root);
    }

    void insert(int val) {
       root = insertHelper(root, val);
    }

    bool search(int val) {
        return searchHelper(root, val);
    }

    void inorderTraversal() {
        inorder(root);
        cout << endl;
    }
    void preorderTraversal() {
        preorder(root);
        cout << endl;
    }
    void postorderTraversal() {
        postorder(root);
        cout << endl;
    }
};


int main() {
    BinaryTree tree;
    tree.insert(10);
    tree.insert(2);
    tree.insert(13);
    tree.insert(14);
    tree.insert(7);
    tree.insert(16);
    tree.inorderTraversal();
    tree.preorderTraversal();
    tree.postorderTraversal();
    return 0;
}