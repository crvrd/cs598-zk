#include "prover.h"

#include <iostream>
using namespace std;

// Start the prover server, and seed random for key generation
Prover::Prover() {
    network.Start();
    SeedRandom();
}

// Overloaded, if we want to choose port
Prover::Prover(char* port) {
    network.Start(port);
    SeedRandom();
}

Prover::Prover(ifstream& infile) {
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
    g->NormalizeEdges();
    SeedRandom();
}

// Close the socket when we leave
Prover::~Prover() {
    network.Close();
}

bool Prover::SolveGraph() {
    return g->Solve(0);
}

bool Prover::BeginExchange(int k, char* hostname, char* port) {
    if(!network.Connect(hostname, port))
        throw -1;
    int theirk;
    if(!network.SendInt(k))
        return false;
    if(!network.RecvInt(&theirk))
        return false;
    if(k != theirk)
        return false;
    commitnum = k*g->numedges;
    if(!network.SendInt(g->numvertices))
        return false;
    return network.SendGraph(g);
    return true;
}

bool Prover::GenerateCommitments() {
    graphs = new Graph[commitnum];
    for(int i = 0; i < commitnum; i++) {
        graphs[i] = *g;
        graphs[i].GenCommitment();
    }
    for(int i = 0; i < commitnum; i++) {
        if(!network.SendCommitment(graphs + i))
            return false;
    }
    return true;
}

bool Prover::ProcessEdgeRequests() {
    // Receive requests
    requests = new int32_t[commitnum * 2];
    if(!network.RecvBytes(requests, commitnum * 8))
        return false;

    // Find response information
    int32_t colors[commitnum * 2];
    uint64_t keys[commitnum * 2];
    for(int i = 0; i < commitnum; i++) {
        int first = requests[i*2];
        int second = requests[i*2+1];
        if(!graphs[i].edges[first][second])
            return false;
        colors[i*2] = graphs[i].vertices[first].color;
        colors[i*2+1] = graphs[i].vertices[second].color;
        keys[i*2] = graphs[i].vertices[first].randkey;
        keys[i*2+1] = graphs[i].vertices[second].randkey;
    }

    // Send responses
    if(!network.SendBytes(colors, commitnum * 8))
        return false;
    if(!network.SendBytes(keys, commitnum * 16))
        return false;
    return true;
}

// Get the graph from the verifier, and find a solution if one exists
// Also let the verifier know if there's a solution
bool Prover::RecvAndSolveGraph() {
    int vertexnum;
    network.RecvInt(&vertexnum);

    g = new Graph(vertexnum);
    network.RecvGraph(g);
    bool retval = g->Solve(0);
    network.SendBool(retval);
    return retval;
}

// Attempt to cheat at solving the graph.  Pretend there's a solution 
// when there really isn't.
bool Prover::CheatSolveGraph() {
    int vertexnum;
    network.RecvInt(&vertexnum);

    g = new Graph(vertexnum);
    network.RecvGraph(g);
    if(!g->Solve(0))
        cout << "No solution, but we will cheat" << endl;
    network.SendBool(true);
    return true;
}

void Prover::GenerateCommitment() {
    g->GenCommitment();
}

// Send commitment to the verifier
bool Prover::SendGraphCommitment() {
    return network.SendCommitment(g);
}

// Prove to the verifier that the solution works
bool Prover::Prove() {
    int32_t numrequests = pow((g->numedges)/2, 2);
    // Each round, we generate a new commitment, and show that 
    // Two adjacent vertices are not the same color
    for(int i = 0; i < numrequests; i++) {
        GenerateCommitment();
        SendGraphCommitment();
        if(!RespondVerRequest())
            return false;
    }
    return true;
}

// Attempt to cheat at the proof by calling CheatVerRequest()
void Prover::CheatProof() {
    int32_t numrequests = pow((g->numedges)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        GenerateCommitment();
        SendGraphCommitment();
        if(!CheatVerRequest())
            return false;
    }
    return true;
}

// Get a request of two edges to respond to, and show that they
// are not the same color
bool Prover::RespondVerRequest() {
    // Get two ints for the indices of the vertices
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    // Send the data for the vertices to prove their colors
    if(g->edges[none][ntwo]){
        if(!network.SendProof(g, none))
            return false;
        if(!network.SendProof(g, ntwo))
            return false;
    }
    return true;
}

// Attempt to cheat the verification process by making the vertices different
// colors, even if they aren't
bool Prover::CheatVerRequest() {
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    if(g->edges[none][ntwo]){
        // Make the two vertices different colors, even if they aren't
        if(g->vertices[none].color == g->vertices[ntwo].color){
            g->vertices[ntwo].color = (g->vertices[ntwo].color % 3) + 1;
        }
        if(!network.SendProof(g, none))
            return false;
        if(!network.SendProof(g, ntwo))
            return false;
    }
    return true;
}

void Prover::PrintGraph() {
    g->Print();
}

// Corrupt the solution by causing a vertex to have the same color
// as its edge.
void Prover::CorruptSolution() {
    for(int i = 0; i < g->numvertices; i++) {
        if(g->edges[i][0]) {
            g->vertices[0].color = g->vertices[i].color;
            break;
        }
    }
}