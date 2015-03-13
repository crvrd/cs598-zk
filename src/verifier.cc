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

Verifier::Verifier(char* port) {
    network.Start(port);
    SeedRandom();
}

Verifier::~Verifier() {
    network.Close();
}

bool Verifier::BeginExchange(int k) {
    int nodes, theirk;
    if(!network.RecvInt(&theirk))
        return false;
    if(!network.SendInt(k))
        return false;
    if(theirk != k)
        return false;
    if(!network.RecvInt(&nodes))
        return false;
    g = new Graph(nodes);
    if(!network.RecvGraph(g))
        return false;
    commitnum = k*g->numneighbors;
    return true;
}

void Verifier::GenerateRequests() {
    requests = new int32_t[commitnum * 2];
    for(int i = 0; i < commitnum; i++) {
        int j,k;
        do {
            j = rand() % g->numnodes;
            k = rand() % g->numnodes;
        } while(!g->neighbors[j][k]);
        requests[i*2] = j;
        requests[i*2+1] = k;
    }
}

bool Verifier::RecvCommitments() {
    graphs = new Graph[commitnum];
    for(int i = 0; i < commitnum; i++) {
        graphs[i] = *g;
        if(!network.RecvCommitment(graphs + i))
            return false;
    }
    return true;
}

bool Verifier::SendRequests() {
    if(!network.SendBytes(requests, commitnum * 8))
        return false;
    return true;
}

bool Verifier::VerifyRequests() {
    // Get colors and random keys
    int32_t colors[commitnum * 2];
    uint64_t keys[commitnum * 2];
    if(!network.RecvBytes(colors, commitnum * 8))
        return false;
    if(!network.RecvBytes(keys, commitnum * 16))
        return false;

    // Assign colors and keys, and verify
    for(int i = 0; i < commitnum; i++) {
        int first = requests[i*2];
        int second = requests[i*2+1];

        graphs[i].nodes[first].color = colors[i*2];
        graphs[i].nodes[second].color = colors[i*2+1];
        graphs[i].nodes[first].randkey = keys[i*2];
        graphs[i].nodes[second].randkey = keys[i*2+1];
        if(graphs[i].nodes[first].color == graphs[i].nodes[second].color)
            return false;
        if(!(graphs[i].nodes[first].VerHash() && graphs[i].nodes[second].VerHash()))
            return false;
    }
    return true;
}

void Verifier::WriteGraph(ofstream& outfile) {
    outfile << g->numnodes << endl;

    for(int j = 0; j < g->numnodes; j++) {
        for(int k = 0; k < g->numnodes; k++) {
            if(g->neighbors[j][k])
                outfile << "1 ";
            else
                outfile << "0 ";
        }
        outfile << endl;
    }
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
    do {
        none = (rand() % g->numnodes);
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