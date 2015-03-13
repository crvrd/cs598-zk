#ifndef PROVER_H
#define PROVER_H

#include "network.h"

class Prover{
public:
    Prover();
    Prover(char* port);
    Prover(std::ifstream& infile);
    ~Prover();
    bool SolveGraph();
    bool BeginExchange(int k, char* hostname, char* port);
    bool GenerateCommitments();
    bool ProcessEdgeRequests();
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
    int32_t* requests;
    int commitnum;
    Graph* g;
    Graph* graphs;
    Network network;
};

#endif