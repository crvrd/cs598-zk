#include "vertex.h"

using namespace std;

Vertex::Vertex() {
    color = 0;
    randkey = 0;
}

Vertex::Vertex(int col) {
    color = col;
}

// Generates the hash for the commitment for this vertex
bool Vertex::GenHash() {
    // Hash x|r, where x is the color, and r is a 64-bit 
    // random key.
    unsigned char input[12];
    memcpy(input, &color, 4);
    memcpy(input+4, &randkey, 8);

    // Generate hash
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, input, 12))
        return false;

    if(!SHA256_Final(commithash, &context))
        return false;
    return true;
}

// Verifies that the color and key match the hash.
// Verifier runs this to not be duped.
bool Vertex::VerHash() {
    unsigned char checkhash[SHA256_DIGEST_LENGTH];
    unsigned char input[12];
    memcpy(input, &color, 4);
    memcpy(input+4, &randkey, 8);

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, input, 12))
        return false;

    if(!SHA256_Final(checkhash, &context))
        return false;
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if(checkhash[i] != commithash[i])
            return false;
    }
    return true;
}

// Match the color to the new mapping, and generate a new random key
void Vertex::Randomize(int32_t* colormap) {
    if(0 == color)
        color = (rand() % 3) + 1;
    else
        color = colormap[color-1];
    randkey = (uint64_t)rand();
}