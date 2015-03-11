#include "utilities.h"

using namespace std;

void SeedRandom()
{
    uint32_t fileseed, timeseed; 
    ifstream urand;
    urand.open("/dev/urandom", ios::binary);
    if (urand.is_open()) {
        urand.read((char*)&fileseed, sizeof(uint32_t));
        urand.close();
    }
    else
        fileseed = 0;
    timeseed = time(NULL);
    srand(timeseed xor fileseed);
}