#ifndef VERIFIER_H
#define VERIFIER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "network.h"

class Verifier{
public:
    Verifier();
    Verifier(std::ifstream& infile);
    Verifier(std::ifstream& infile, char* hostname, char* port);
    Verifier(char* port);
    ~Verifier();
    bool BeginExchange(int k);
    void GenerateRequests();
    bool RecvCommitments();
    bool SendRequests();
    bool VerifyRequests();
    void WriteGraph(std::ofstream& outfile);
    bool SendGraph();
    bool RecvGraphCommitment();
    bool Verify();
    bool SendVerRequest();
    void PrintGraph();
private:
    int* requests;
    int commitnum;
    Graph* g;
    Graph* graphs;
    Network network;
};

#endif