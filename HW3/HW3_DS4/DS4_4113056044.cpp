#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Solve {
    private:
        struct Interval {
            int L, R;
            
            bool operator<(const Interval& other) const {
                return (R < other.R);
            }
        };
        
        vector<int> pos;
        vector<int> *inorder, *levelorder;
        int n;


    public:
        Solve(vector<int>& inorder, vector<int>& levelorder) {
            this->inorder = &inorder;
            this->levelorder = &levelorder;
            this->n = inorder.size();
            pos.resize(n + 1);
            for(int i = 0;i < n;++i) {
                pos.at(inorder.at(i)) = i;
            }
        }

        ~Solve() {
            
        }
        
        bool solve(int number) {
            set<Interval> ranges;
            ranges.insert({0, n-1});
            for(int val : *levelorder) {
                int idx = pos.at(val);

                auto it = ranges.lower_bound({0, idx});
                //cout << it->L << " " << it->R << endl;
                int L = it->L;
                int R = it->R;

                if(val == number) {
                    int sizeLeft = idx - L;
                    int sizeRight = R - idx;
                    int sizeParent = n - (R - L + 1);
                    int half = (n >> 1);

                    if(sizeLeft > half || sizeRight > half || sizeParent > half) {
                        return true;
                    } else {
                        return false;
                    }
                }

                ranges.erase(it);
                if(idx > L) ranges.insert({L, idx - 1});
                if(idx < R) ranges.insert({idx + 1, R});
            }
            return false;
        }
};

int main() {
    ifstream in("testcase4.txt");
    ofstream out("output4.txt");
    int numOftestcase = 0;
    in >> numOftestcase;

    for(int testcase = 0;testcase < numOftestcase;++testcase) {
        int firstNode = 0, n = 0;
        in >> firstNode >> n;
        vector<int> inorder, levelorder;
        inorder.reserve(n); levelorder.reserve(n);
        for(int i = 0;i < n;++i) {
            int val; in >> val;
            inorder.push_back(val);
        }
        for(int i = 0;i < n;++i) {
            int val; in >> val;
            levelorder.push_back(val);
        }

        Solve p(inorder, levelorder);
        //p.solve(firstNode);
        //cout << endl;
        out << p.solve(firstNode) << "\n";
    }
    return 0;
}