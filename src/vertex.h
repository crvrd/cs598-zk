#ifndef VERTEX_H
#define VERTEX_H

#include <stdint.h>
#include <openssl/sha.h>
#include <iostream>
#include <cstring>
#include "utilities.h"

class Vertex{
public:
    Vertex();
    Vertex(int col);
    bool GenHash();
    bool VerHash();
    void Randomize(int32_t* colormap);

    int32_t color;
    uint64_t randkey;
    unsigned char commithash[SHA256_DIGEST_LENGTH];
private:
};

#endif