#include "verifier.h"

using namespace std;

Verifier::Verifier() {}

// Make a new graph, and connect to the prover server
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

// Overloaded for choosing connections
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

// Send the graph to the prover
bool Verifier::SendGraph() {
    bool colorable;
    network.SendInt(g->numnodes);
    network.SendGraph(g);
    network.RecvBool(&colorable);
    return colorable;
}

// Get a full graph commitment from the prover
bool Verifier::RecvGraphCommitment() {
    return network.RecvCommitment(g);
}

// Verify that the solution the prover found is correct
bool Verifier::Verify() {
    int32_t numrequests = pow((g->numneighbors)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        RecvGraphCommitment();
        if(!SendVerRequest())
            return false;
    }
    return true;
}

// Choose two nodes to verify, and verify them by getting the 
// colors and random key values for the neighbors
bool Verifier::SendVerRequest() {
    int none, ntwo;
    // Choose two neighbors
    none = (rand() % g->numnodes);
    do {
        ntwo = (rand() % g->numnodes);
    } while(!g->neighbors[none][ntwo]);
    // Get proof of their different colors
    network.SendInt(none);
    network.SendInt(ntwo);
    network.RecvProof(g, none);
    network.RecvProof(g, ntwo);
    // Check to make sure they're different, and there was no tampering.
    if(g->nodes[none].color == g->nodes[ntwo].color)
        return false;
    return (g->nodes[none].VerHash() && g->nodes[ntwo].VerHash());
}

void Verifier::PrintGraph() {
    g->Print();
}