#include "verifier.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(4 != argc) {
        cout << "usage:  ./victor <port> <security parameter> <output dir>" << endl;
        exit(-1);
    }
    while(1) {
        Verifier* victor = new Verifier(argv[1]);
        if(!victor->BeginExchange(stoi(argv[2]))) {
            cout << "Could not begin exchange; probable security parameter mismatch" << endl;
            delete victor;
            continue;
        }
        victor->GenerateRequests();
        if(!victor->RecvCommitments()) {
            cout << "Could not receive commitments." << endl;
            delete victor;
            continue;
        }
        if(!victor->SendRequests()) {
            cout << "Problem sending requests." << endl;
            delete victor;
            continue;
        }
        if(!victor->VerifyRequests()) {
            cout << "Graph verification Failed." << endl;
            delete victor;
            continue;
        }
        // TODO:  Store graph somewhere!
        ofstream outfile;
        outfile.open(argv[3] + string("/g") + 
                     to_string(time(NULL)) + to_string(rand()));
        victor->WriteGraph(outfile);
        outfile.close();
        delete victor;
        cout << "Graph verified!" << endl;
    }

}