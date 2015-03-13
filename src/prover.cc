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
    g->NormalizeNeighbors();
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
    commitnum = k*g->numneighbors;
    if(!network.SendInt(g->numnodes))
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
        if(!graphs[i].neighbors[first][second])
            return false;
        colors[i*2] = graphs[i].nodes[first].color;
        colors[i*2+1] = graphs[i].nodes[second].color;
        keys[i*2] = graphs[i].nodes[first].randkey;
        keys[i*2+1] = graphs[i].nodes[second].randkey;
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
    int nodenum;
    network.RecvInt(&nodenum);

    g = new Graph(nodenum);
    network.RecvGraph(g);
    bool retval = g->Solve(0);
    network.SendBool(retval);
    return retval;
}

// Attempt to cheat at solving the graph.  Pretend there's a solution 
// when there really isn't.
bool Prover::CheatSolveGraph() {
    int nodenum;
    network.RecvInt(&nodenum);

    g = new Graph(nodenum);
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
    int32_t numrequests = pow((g->numneighbors)/2, 2);
    // Each round, we generate a new commitment, and show that 
    // Two adjacent nodes are not the same color
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
    int32_t numrequests = pow((g->numneighbors)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        GenerateCommitment();
        SendGraphCommitment();
        if(!CheatVerRequest())
            return false;
    }
    return true;
}

// Get a request of two neighbors to respond to, and show that they
// are not the same color
bool Prover::RespondVerRequest() {
    // Get two ints for the indices of the nodes
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    // Send the data for the nodes to prove their colors
    if(g->neighbors[none][ntwo]){
        if(!network.SendProof(g, none))
            return false;
        if(!network.SendProof(g, ntwo))
            return false;
    }
    return true;
}

// Attempt to cheat the verification process by making the nodes different
// colors, even if they aren't
bool Prover::CheatVerRequest() {
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    if(g->neighbors[none][ntwo]){
        // Make the two nodes different colors, even if they aren't
        if(g->nodes[none].color == g->nodes[ntwo].color){
            g->nodes[ntwo].color = (g->nodes[ntwo].color % 3) + 1;
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

// Corrupt the solution by causing a node to have the same color
// as its neighbor.
void Prover::CorruptSolution() {
    for(int i = 0; i < g->numnodes; i++) {
        if(g->neighbors[i][0]) {
            g->nodes[0].color = g->nodes[i].color;
            break;
        }
    }
}