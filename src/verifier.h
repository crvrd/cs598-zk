#ifndef VERIFIER_H
#define VERIFIER_H

#include <fstream>

#include "network.h"

class Verifier{
public:
    Verifier(char* port);
    ~Verifier();
    bool BeginExchange(int k, int j);
    void GenerateEdgeRequests();
    bool RecvCommitments();
    bool SendEdgeRequests();
    bool VerifyEdgeRequests();
    void WriteGraph(std::ofstream& outfile);
private:
    int* requests;
    int commitnum;
    Graph* g;
    Graph* graphs;
    Network network;
};

#endif