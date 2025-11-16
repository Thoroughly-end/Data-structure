#include <iostream>
#include <cstdlib>
using namespace std;

#define MAX_SIZE 50
typedef enum {head, entry} tagfield;

typedef struct entryNode {
    int row;
    int col;
    int value;
}entryNode;
typedef struct matrixNode {
    tagfield tag;
    matrixNode *right;
    matrixNode *down;
    union {
        entryNode entry;
        matrixNode *next;
    } u;
}matrixNode;
matrixNode *hdnode[MAX_SIZE];

matrixNode* mread(void);
void mwrite(matrixNode *matrix);
void merase(matrixNode *matrix);

int main() {
    matrixNode *matrix = mread();
    mwrite(matrix);
    merase(matrix);
    return 0;
}

matrixNode* mread(void) {
    int num_rows, num_cols, num_terms;
    int num_heads, i, row, col, value, current_row;
    matrixNode *temp, *last, *node;

    cout << "Enter number of rows, columns and non-zero terms: ";
    cin >> num_rows >> num_cols >> num_terms;
    num_heads = (num_cols > num_rows) ? num_cols : num_rows;
    node = new matrixNode;
    node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;
    if(num_heads == 0) {
        node->right = node;
        return node;
    } else {
        for(int i = 0;i < num_heads;++i) {
            hdnode[i] = new matrixNode;
            hdnode[i]->tag = head;
            hdnode[i]->right = hdnode[i];
            hdnode[i]->u.next = hdnode[i];
        }
        current_row = 0;
        last = hdnode[0];
        for(int i = 0;i < num_terms;++i) {
            cout << "Enter row, column and value: ";
            cin >> row >> col >> value;
            if(row > current_row) {
                last->right = hdnode[current_row];
                current_row = row;
                last = hdnode[current_row];
            }
            temp = new matrixNode;
            temp->tag = entry;
            temp->u.entry.row = row;
            temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp;
            last = temp;

            hdnode[col]->u.next->down = temp;
            hdnode[col]->u.next = temp;
        }
        last->right = hdnode[current_row];
        for(int i = 0;i < num_cols;++i) {
            hdnode[i]->u.next->down = hdnode[i];
        }
        for(int i = 0;i < num_heads - 1;++i) {
            hdnode[i]->u.next = hdnode[i + 1];
        }
        hdnode[num_heads - 1]->u.next = node;
        node->right = hdnode[0];
        return node;
    }
}

void mwrite(matrixNode *matrix) {
    matrixNode *temp, *head = matrix->right;
    int num_rows = matrix->u.entry.row;
    int num_cols = matrix->u.entry.col;
    cout << "number of rows: " << num_rows << ", number of columns: " << num_cols << endl;
    cout << "The matrix by row, column and value:" << endl;
    for(int i = 0;i < num_rows;++i) {
        for(temp = head->right;temp != head;temp = temp->right) {
            cout << temp->u.entry.row << " " << temp->u.entry.col << " " << temp->u.entry.value << endl;
        }
        head = head->u.next;
    }
}

void merase(matrixNode *matrix) {
    matrixNode *head = matrix->right;
    matrixNode *temp, *current;
    int num_rows = matrix->u.entry.row;
    for(int i = 0;i < num_rows;++i) {
        current = head->right;
        while (current != head) {
            temp = current;
            current = current->right;
            delete temp;
        }
        temp = head;
        head = head->u.next;
        delete temp;
    }
    current = head;
    while (current != matrix) {
        temp = current;
        current = current->u.next;
        delete temp;
    }
    delete matrix;
    matrix = nullptr;
}