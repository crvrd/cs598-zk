#ifndef PROVER_H
#define PROVER_H

#include "network.h"

using namespace std;

class Prover{
public:
    Prover();
    Prover(char* port);
    ~Prover();
    bool RecvAndSolveGraph();
    bool CheatSolveGraph();
    void GenerateCommitment();
    bool SendGraphCommitment();
    bool Prove();
    bool RespondVerRequest();
    void PrintGraph();
    void CorruptSolution();
    void CheatProof();
    bool CheatVerRequest();
private:
    Graph* g;
    Network network;
};

#endif