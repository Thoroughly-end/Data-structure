#include <iostream>
#include <sstream>
#include <fstream>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define BOLD "\033[1m"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream outputFile("../HW" + string(argv[1]) + "/" + "HW" + string(argv[1]) + "_DS" + string(argv[2]) + "/" + "output" + string(argv[2]) + ".txt");
    ifstream ansFile("../HW" + string(argv[1]) + "/" + "HW" + string(argv[1]) + "_DS" + string(argv[2]) + "/" + "ans" + string(argv[2]) + ".txt");
    if(!outputFile.is_open() || !ansFile.is_open()) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    string outputLine, ansLine;

    while (true) {
        bool outputHasLine = static_cast<bool>(getline(ansFile, ansLine));
        bool ansHasLine = static_cast<bool>(getline(outputFile, outputLine));

        if (!outputLine.empty() && outputLine.back() == '\r') outputLine.pop_back();
        if (!ansLine.empty() && ansLine.back() == '\r') ansLine.pop_back();

        if (!outputHasLine && !ansHasLine) {
            break;
        }

        if(outputHasLine != ansHasLine) {
            if(!(ansLine.empty() || outputLine.empty())) {
                cout << RED << BOLD << "Fail" << endl;
                return 0;
            } else {
                cout << GREEN << BOLD << "Pass" << endl;
                return 0;
            }
        }

        stringstream outputStream(outputLine);
        stringstream ansStream(ansLine);

        while (true) {
            string output, ans;
            bool outputHasNext = static_cast<bool>(outputStream >> output);
            bool ansHasNext = static_cast<bool>(ansStream >> ans);
            if(!outputHasNext && !ansHasNext) break;

            if(outputHasNext != ansHasNext) {
                cout << RED << BOLD << "Fail" << endl;
                return 0;
            }
            //cout << "Comparing: " << output << " and " << ans << endl;
            if (output != ans) {
                cout << RED << BOLD << "Fail" << endl;
                return 0;
            }
        }
        if(!outputHasLine && !ansHasLine) {
            break;
        }
    }
    
    cout << GREEN << BOLD << "Pass" << endl;
    return 0;
}