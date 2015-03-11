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
    return g->Solve(0);
}

void Prover::GenerateCommitment() {
    g->GenCommitment();
}

bool Prover::SendGraphCommitment() {
    return network.SendCommitment(g);
}

bool Prover::RecvVerRequest(Node* one, Node* two) {
    return true;
}

bool Prover::SendVerification() {
    return true;
}

bool Prover::SendResult() {
    return true;
}

void Prover::PrintGraph() {
    g->Print();
}