#ifndef PROVER_H
#define PROVER_H

#include "network.h"

class Prover{
public:
    Prover(std::ifstream& infile);
    ~Prover();
    bool SolveGraph(bool cheat);
    bool BeginExchange(int k, int j, char* hostname, char* port);
    bool GenerateCommitments();
    bool ProcessEdgeRequests(bool cheat);
private:
    int32_t* requests;
    int commitnum;
    Graph* g;
    Graph* graphs;
    Network network;
};

#endif