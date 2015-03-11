#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

using namespace std;

class Graph{
public:
    Graph(int nodenum);
    ~Graph();
    void AssignNeighbors(int i, int j);
    void UnassignNeighbors(int i, int j);
    bool Solve(int idx);
    void GenCommitment();
    void Print();
    int numnodes;
    Node* nodes;
    bool** neighbors;
private:
    int32_t colormap[3];
    bool VerifyNode(int node);
    bool TestSolution();
};

#endif