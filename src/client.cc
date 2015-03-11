#include "verifier.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(4 != argc) {
        cout << "usage: ./client <hostname> <port> <graph file>" << endl;
        exit(-1);
    }

    ifstream infile;
    infile.open(argv[3]);
    
    Verifier* victor = new Verifier(infile, argv[1], argv[2]);
    cout << "Sending Graph" << endl;
    if(!victor->SendGraph()) {
        cout << "No 3-colorable solution found." << endl;
        delete victor;
        exit(-1);
    }
    cout << "Verifying solution" << endl;
    if(victor->Verify())
        cout << "Verified Solution!" << endl;
    else
        cout << "Invalid verification, Prover is probably lying." << endl;

    delete victor;
}