#ifndef PROVER_H
#define PROVER_H

#include "network.h"

using namespace std;

class Prover{
public:
    Prover();
    ~Prover();
    bool RecvAndSolveGraph();
    bool SendSolvedSignal();
    bool SendGraphCommitment();
    bool RecvVerRequest(Node* one, Node* two);
    bool SendVerification();
    bool SendResult();
    void PrintGraph();
private:
    Graph* g;
    Network network;
};

#endif