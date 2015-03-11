#ifndef VERIFIER_H
#define VERIFIER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "network.h"

class Verifier{
public:
    Verifier();
    Verifier(ifstream& infile);
    ~Verifier();
    bool SendGraph();
    bool RecvSolvedSignal();
    bool RecvGraphCommitment();
    bool SendVerRequest();
    bool RecvVerification();
    bool RecvResult();
    void PrintGraph();
private:
    Graph* g;
    Network network;
};

#endif