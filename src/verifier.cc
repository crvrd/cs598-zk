#include "verifier.h"

using namespace std;

Verifier::Verifier() {}

// Make a new graph, and connect to the prover server
Verifier::Verifier(ifstream& infile) {
    int vertexnum, edgeval;
    infile >> vertexnum;
    g = new Graph(vertexnum);
    for(int i = 0; i < vertexnum; i++) {
        for(int j = 0; j < vertexnum; j++) {
            infile >> edgeval;
            if(edgeval) {
                g->AssignEdges(i, j);
            }
        }
    }
    network.Connect();
}

// Overloaded for choosing connections
Verifier::Verifier(ifstream& infile, char* hostname, char* port) {
    int vertexnum, edgeval;
    infile >> vertexnum;
    g = new Graph(vertexnum);
    for(int i = 0; i < vertexnum; i++) {
        for(int j = 0; j < vertexnum; j++) {
            infile >> edgeval;
            if(edgeval) {
                g->AssignEdges(i, j);
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
    int vertices, theirk;
    if(!network.RecvInt(&theirk))
        return false;
    if(!network.SendInt(k))
        return false;
    if(theirk != k)
        return false;
    if(!network.RecvInt(&vertices))
        return false;
    g = new Graph(vertices);
    if(!network.RecvGraph(g))
        return false;
    commitnum = k*g->numedges;
    return true;
}

void Verifier::GenerateRequests() {
    requests = new int32_t[commitnum * 2];
    for(int i = 0; i < commitnum; i++) {
        int j,k;
        do {
            j = rand() % g->numvertices;
            k = rand() % g->numvertices;
        } while(!g->edges[j][k]);
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

        graphs[i].vertices[first].color = colors[i*2];
        graphs[i].vertices[second].color = colors[i*2+1];
        graphs[i].vertices[first].randkey = keys[i*2];
        graphs[i].vertices[second].randkey = keys[i*2+1];
        if(graphs[i].vertices[first].color == graphs[i].vertices[second].color)
            return false;
        if(!(graphs[i].vertices[first].VerHash() && graphs[i].vertices[second].VerHash()))
            return false;
    }
    return true;
}

void Verifier::WriteGraph(ofstream& outfile) {
    outfile << g->numvertices << endl;

    for(int j = 0; j < g->numvertices; j++) {
        for(int k = 0; k < g->numvertices; k++) {
            if(g->edges[j][k])
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
    network.SendInt(g->numvertices);
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
    int32_t numrequests = pow((g->numedges)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        RecvGraphCommitment();
        if(!SendVerRequest())
            return false;
    }
    return true;
}

// Choose two vertices to verify, and verify them by getting the 
// colors and random key values for the edges
bool Verifier::SendVerRequest() {
    int none, ntwo;
    // Choose two edges
    do {
        none = (rand() % g->numvertices);
        ntwo = (rand() % g->numvertices);
    } while(!g->edges[none][ntwo]);
    // Get proof of their different colors
    network.SendInt(none);
    network.SendInt(ntwo);
    network.RecvProof(g, none);
    network.RecvProof(g, ntwo);
    // Check to make sure they're different, and there was no tampering.
    if(g->vertices[none].color == g->vertices[ntwo].color)
        return false;
    return (g->vertices[none].VerHash() && g->vertices[ntwo].VerHash());
}

void Verifier::PrintGraph() {
    g->Print();
}