#include "utilities.h"

void SeedRandom()
{
    // Generate a random number based on urandom and time
    uint32_t fileseed, timeseed; 
    std::ifstream urand;
    urand.open("/dev/urandom", std::ios::binary);
    if (urand.is_open()) {
        urand.read((char*)&fileseed, sizeof(uint32_t));
        urand.close();
    }
    else
        fileseed = 0;
    timeseed = time(NULL);
    srand(timeseed xor fileseed);
}

int max(int a, int b) {
    if(a >= b)
        return a;
    return b;
}

int min(int a, int b) {
    if(a <= b)
        return a;
    return b;
}

int avg(int a, int b) {
    return (a+b)/2;
}