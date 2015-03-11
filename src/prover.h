#ifndef PROVER_H
#define PROVER_H

#include "network.h"

using namespace std;

class Prover{
public:
    Prover();
    ~Prover();
    bool RecvAndSolveGraph();
    void GenerateCommitment();
    bool SendGraphCommitment();
    bool Prove();
    bool RespondVerRequest();
    bool SendResult();
    void PrintGraph();
private:
    Graph* g;
    Network network;
};

#endif