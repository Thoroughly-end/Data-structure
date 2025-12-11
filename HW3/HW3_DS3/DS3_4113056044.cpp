#pragma GCC optimize("Ofast")
#include <iostream>
#include<fstream>
#include <vector>

using namespace std;

typedef enum {pour, sip, lookup} operation;

class BinaryTree {
    private:
        void constructHelper(vector<int>& inorder, vector<int>& preorder) {
            vector<int> pos;
            pos.resize(N + 1);
            for(int i = 0;i < N; ++i) {
                pos.at(inorder.at(i)) = i;
            }
            return buildHelper(preorder, pos, 0, 0, N - 1);
        }

        void buildHelper(vector<int>& preorder, vector<int>& pos, int preStart, int inStart, int inEnd) {
            if(inStart > inEnd) return;
            int rootVal = preorder.at(preStart);

            timerAdd();
            in.at(rootVal) = now();

            int inRootIndex = pos.at(rootVal);
            int sizeOfLeft = inRootIndex - inStart;

            buildHelper(preorder, pos, preStart + 1, inStart, inRootIndex - 1);
            buildHelper(preorder, pos, preStart + sizeOfLeft + 1, inRootIndex + 1, inEnd);

            out.at(rootVal) = now();
        }

        vector<int> in, out, TreeA, lazyA, TreeB;
        int Timer, N;

        inline void timerAdd() {++Timer;}
        inline int now() {return Timer;}
        void setTime() {Timer = 0;}

        void pushDownA(int node) {
            if (lazyA[node] != 0) {
                TreeA.at(node << 1) = lazyA.at(node);
                lazyA.at(node << 1) = lazyA.at(node);
                TreeA.at(node << 1 | 1) = lazyA.at(node);
                lazyA.at(node << 1 | 1) = lazyA.at(node);
                lazyA.at(node) = 0;
            }
        }

        void updateTreeA(int node, int start, int end, int l, int r, int val) {
            if (l > end || r < start) return;
            if (l <= start && end <= r) {
                TreeA.at(node) = val;
                lazyA.at(node) = val;
                return;
            }
            pushDownA(node);
            int mid = (start + end) >> 1;
            updateTreeA(node << 1, start, mid, l, r, val);
            updateTreeA(node << 1 | 1, mid + 1, end, l, r, val);
        }

        int queryTreeA(int node, int start, int end, int idx) {
            if (start == end) return TreeA.at(node);
            pushDownA(node);
            int mid = (start + end) / 2;
            if (idx <= mid) return queryTreeA(node << 1, start, mid, idx);
            else return queryTreeA(node << 1 | 1, mid + 1, end, idx);
        }

        void updateTreeB(int node, int start, int end, int idx, int val) {
            if (start == end) {
                TreeB.at(node) = val;
                return;
            }
            int mid = (start + end) >> 1;
            if (idx <= mid) updateTreeB(node << 1, start, mid, idx, val);
            else updateTreeB(node << 1 | 1, mid + 1, end, idx, val);
            TreeB.at(node) = max(TreeB.at(node << 1), TreeB.at(node << 1 | 1));
        }

        int queryTreeB(int node, int start, int end, int l, int r) {
            if (l > end || r < start) return 0;
            if (l <= start && end <= r) return TreeB.at(node);
            int mid = (start + end) >> 1;
            return max(queryTreeB(node << 1, start, mid, l, r), queryTreeB(node << 1 | 1, mid + 1, end, l, r));
        }

    public:
        BinaryTree(vector<int>& inorder, vector<int>& preorder) {
            N = inorder.size();
            Timer = 0;
            in.resize(N + 1);
            out.resize(N + 1);
            in.at(0) = 0;
            out.at(0) = 0;
            constructHelper(inorder, preorder);

            TreeA.resize(N << 2 | 1, 0);
            lazyA.resize(N << 2 | 1, 0);
            TreeB.resize(N << 2 | 1, 0);
            setTime();
        }
        ~BinaryTree() {
            
        }

        bool operate(operation oper, int node) {
            timerAdd();
            if(oper == pour) {
                updateTreeA(1, 1, N, in.at(node), out.at(node), now());
                return false;
            } else if(oper == sip) {
                updateTreeB(1, 1, N, in.at(node), now());
                return false;
            } else {
                int fill_time = queryTreeA(1, 1, N, in.at(node));
                int drain_time = queryTreeB(1, 1, N, in.at(node), out.at(node));
                return (fill_time > drain_time);
            }
        }
};

int main() {
    ifstream in("testcase3.txt");
    ofstream out("output3.txt");
    in.tie(0);

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