#include "prover.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(2 != argc) {
        cout << "usage: ./nosol_server <port>" << endl;
        exit(-1);
    }
    while(1) {
        Prover* peggy = new Prover(argv[1]);

        if(!peggy->RecvAndSolveGraph()) {
            cout << "No Solution" << endl;
            delete peggy;
            continue;
        }
        peggy->CorruptSolution();
        peggy->Prove();
        delete peggy;
    }

}