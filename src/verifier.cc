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
            if(neighborval) {
                g->AssignNeighbors(i, j);
            }
        }
    }
    network.Connect();
}

Verifier::Verifier(ifstream& infile, char* hostname, char* port) {
    int nodenum, neighborval;
    infile >> nodenum;
    g = new Graph(nodenum);
    for(int i = 0; i < nodenum; i++) {
        for(int j = 0; j < nodenum; j++) {
            infile >> neighborval;
            if(neighborval) {
                g->AssignNeighbors(i, j);
            }
        }
    }
    network.Connect(hostname, port);
}

Verifier::~Verifier() {
    network.Close();
}

bool Verifier::SendGraph() {
    bool colorable;
    network.SendInt(g->numnodes);
    network.SendGraph(g);
    network.RecvBool(&colorable);
    return colorable;
}

bool Verifier::RecvGraphCommitment() {
    return network.RecvCommitment(g);
}

bool Verifier::Verify() {
    int32_t numrequests = pow((g->numneighbors)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        RecvGraphCommitment();
        if(!SendVerRequest())
            return false;
    }
    return true;
}

bool Verifier::SendVerRequest() {
    int none, ntwo;
    none = (rand() % g->numnodes);
    do {
        ntwo = (rand() % g->numnodes);
    } while(!g->neighbors[none][ntwo]);
    network.SendInt(none);
    network.SendInt(ntwo);
    network.RecvProof(g, none);
    network.RecvProof(g, ntwo);
    if(g->nodes[none].color == g->nodes[ntwo].color)
        return false;
    return (g->nodes[none].VerHash() && g->nodes[ntwo].VerHash());
}

void Verifier::PrintGraph() {
    g->Print();
}