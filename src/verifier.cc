#include "verifier.h"

using namespace std;

Verifier::Verifier() {}

Verifier::Verifier(ifstream& infile) {
    int nodenum, neighborval;
    infile >> nodenum;
    g = new Graph(nodenum);
    for(int i = 0; i < nodenum; i++) {
        for(int j = 0; j < nodenum; j++) {
            infile >> neighborval;
            if(neighborval)
                g->AssignNeighbors(i, j);
        }
    }
    network.Connect();
}

Verifier::~Verifier() {
    network.Close();
}

bool Verifier::SendGraph() {
    network.SendInt(g->numnodes);
    network.SendGraph(g);
    return true;
}

bool Verifier::RecvGraphCommitment() {
    return network.RecvCommitment(g);
}

bool Verifier::SendVerRequest() {
    return true;
}

bool Verifier::RecvVerification() {
    return true;
}

bool Verifier::RecvResult() {
    return true;
}

void Verifier::PrintGraph() {
    g->Print();
}