#ifndef VERIFIER_H
#define VERIFIER_H

#include "network.h"

class Verifier{
    Verifier();
    bool RecvSolvedSignal();
    bool RecvGraphCommitment();
    bool SendVerRequest();
    bool RecvVerification();
    bool RecvResult();
private:
    Network network;
};

#endif