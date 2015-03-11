#include "verifier.h"

#include <iostream>
using namespace std;

Verifier::Verifier() {

}

bool Verifier::RecvSolvedSignal() {
    return true;
}

bool Verifier::RecvGraphCommitment() {
    return true;
}

bool Verifier::SendVerRequest() {
    return true;
}

bool Verifier::RecvVerification() {
    return true;
}

bool Verifier::RecvResult() {
    return true;
}

