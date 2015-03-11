#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <iostream>

using namespace std;

class Node{
public:
    Node();
    Node(int col, int tempcol);

    int color;
    int tempcolor;
    uint64_t randkey;
private:
};

#endif