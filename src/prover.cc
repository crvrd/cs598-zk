#include "prover.h"

#include <iostream>
using namespace std;

Prover::Prover() {
    network.Start();
    SeedRandom();
}

Prover::~Prover() {
    network.Close();
}

bool Prover::RecvAndSolveGraph() {
    int nodenum;
    network.RecvInt(&nodenum);

    g = new Graph(nodenum);
    network.RecvGraph(g);
    bool retval = g->Solve(0);
    return retval;
}

void Prover::GenerateCommitment() {
    g->GenCommitment();
}

bool Prover::SendGraphCommitment() {
    return network.SendCommitment(g);
}

bool Prover::Prove() {
    int32_t numrequests = pow((g->numneighbors)/2, 2);
    for(int i = 0; i < numrequests; i++) {
        GenerateCommitment();
        SendGraphCommitment();
        if(!RespondVerRequest())
            return false;
    }
    return true;
}

bool Prover::RespondVerRequest() {
    int none, ntwo;
    network.RecvInt(&none);
    network.RecvInt(&ntwo);
    if(g->neighbors[none][ntwo]){
        if(!network.SendProof(g, none))
            return false;
        if(!network.SendProof(g, ntwo))
            return false;
    }
    return true;
}

bool Prover::SendResult() {
    return true;
}

void Prover::PrintGraph() {
    g->Print();
}