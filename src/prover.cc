#include "prover.h"

#include <iostream>
using namespace std;

Prover::Prover() {

}

bool Prover::SendSolvedSignal() {
    return true;
}

bool Prover::SendGraphCommitment() {
    return true;
}

bool Prover::RecvVerRequest(Node* one, Node* two) {
    return true;
}

bool Prover::SendVerification() {
    return true;
}

bool Prover::SendResult() {
    return true;
}
