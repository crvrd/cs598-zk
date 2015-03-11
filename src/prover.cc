#include "prover.h"

#include <iostream>
using namespace std;

Prover::Prover() {
    network.Start();
}

Prover::~Prover() {
    network.Close();
}

bool Prover::RecvAndSolveGraph() {
    int nodenum;
    network.RecvInt(&nodenum);

    g = new Graph(nodenum);
    network.RecvGraph(g);
    return true;
}

bool Prover::SendSolvedSignal() {
    return true;
}

bool Prover::SendGraphCommitment() {
    return true;
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