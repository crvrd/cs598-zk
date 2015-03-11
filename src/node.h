#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <openssl/sha.h>
#include <iostream>
#include <cstring>
#include "utilities.h"

using namespace std;

class Node{
public:
    Node();
    Node(int col);
    bool GenHash();
    bool VerHash(int32_t col, uint64_t key);
    void Randomize(int32_t* colormap);

    int32_t color;
    uint64_t randkey;
    unsigned char commithash[SHA256_DIGEST_LENGTH];
private:
};

#endif