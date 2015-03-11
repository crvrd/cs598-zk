#include "prover.h"

#include <iostream>
using namespace std;

Prover::Prover() {
    network.Start();
    SeedRandom();
}

Prover::Prover(char* port) {
    network.Start(port);
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
    network.SendBool(retval);
    return retval;
}

bool Prover::CheatSolveGraph() {
    int nodenum;
    network.RecvInt(&nodenum);

    g = new Graph(nodenum);
    network.RecvGraph(g);
    g->Solve(0);
    network.SendBool(true);
    return true;
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

bool Prover::RespondVerRequest() {
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    if(g->neighbors[none][ntwo]){
        if(!network.SendProof(g, none))
            return false;
        if(!network.SendProof(g, ntwo))
            return false;
    }
    return true;
}

bool Prover::CheatVerRequest() {
    int none, ntwo;
    if(!network.RecvInt(&none))
        return false;
    if(!network.RecvInt(&ntwo))
        return false;
    if(g->neighbors[none][ntwo]){
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

bool Prover::SendResult() {
    return true;
}

void Prover::PrintGraph() {
    g->Print();
}

void Prover::CorruptSolution() {
    for(int i = 0; i < g->numnodes; i++) {
        if(g->neighbors[i][0]) {
            g->nodes[0].color = g->nodes[i].color;
            break;
        }
    }
}