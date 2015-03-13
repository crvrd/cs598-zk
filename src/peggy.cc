#include "prover.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(5 != argc) {
        cout << "usage: ./peggy <hostname> <port> <graph file> <security parameter>" << endl;
        exit(-1);
    }

    ifstream infile;
    infile.exceptions(ifstream::eofbit | ifstream::badbit | ifstream::failbit | ifstream::goodbit);
    try {
        infile.open(argv[3]);
    } catch(...) {
        cout << "Problem opening file, exiting." << endl;
        exit(-1);
    }

    Prover* peggy;
    try {
        peggy = new Prover(infile);
    } catch(...) {
        cout << "Could not initialize network connection." << endl;
        exit(-1);
    }
    if(!peggy->SolveGraph()) {
        cout << "No solution." << endl;
        delete peggy;
        exit(0);
    }
    if(!peggy->BeginExchange(stoi(argv[4]), argv[1], argv[2])) {
        cout << "Security parameter exchange failed; probable mismatch" << endl;
        delete peggy;
        exit(-1);
    }
    if(!peggy->GenerateCommitments()) {
        cout << "Could not send graph" << endl;
        delete peggy;
        exit(-1);
    }
    if(!peggy->ProcessEdgeRequests()) {
        cout << "Edge requests failed, probable non-neighbor query" << endl;
        delete peggy;
        exit(-1);
    }

    delete peggy;
}