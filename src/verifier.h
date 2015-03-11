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
    Verifier(ifstream& infile);
    Verifier(ifstream& infile, char* hostname, char* port);
    ~Verifier();
    bool SendGraph();
    bool RecvGraphCommitment();
    bool Verify();
    bool SendVerRequest();
    void PrintGraph();
private:
    Graph* g;
    Network network;
};

#endif