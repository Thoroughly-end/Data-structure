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
        bool outputHasLine = !outputFile.eof();
        bool ansHasLine = !ansFile.eof();
        getline(ansFile, ansLine);
        getline(outputFile, outputLine);
        if((outputHasLine && !ansHasLine) || (!outputHasLine && ansHasLine)) {
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
        string output, ans;
        while (true) {
            bool outputHasWord = !outputStream.eof();
            bool ansHasWord = !ansStream.eof();
            if((outputHasWord && !ansHasWord) || (!outputHasWord && ansHasWord)) {
                cout << RED << BOLD << "Fail" << endl;
                return 0;
            }
            outputStream >> output;
            ansStream >> ans;
            //cout << "Comparing: " << output << " and " << ans << endl;
            if (output != ans) {
                cout << RED << BOLD << "Fail" << endl;
                return 0;
            }
            if (!outputHasWord && !ansHasWord) {
                break;
            }
        }
        if(!outputHasLine && !ansHasLine) {
            break;
        }
    }
    
    cout << GREEN << BOLD << "Pass" << endl;
    return 0;
}