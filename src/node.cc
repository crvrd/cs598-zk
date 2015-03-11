#include "node.h"

using namespace std;

Node::Node() {
    color = 0;
    tempcolor = 0;
    randkey = 0;
}

Node::Node(int col, int tempcol) {
    color = col;
    tempcolor = tempcol;
}