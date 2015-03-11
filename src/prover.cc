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

// Close the socket when we leave
Prover::~Prover() {
    network.Close();
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