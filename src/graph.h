#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph{
public:
    Graph();
    Graph(int nodenum);
    Graph(const Graph &g1);
    ~Graph();
    void AssignNeighbors(int i, int j);
    void NormalizeNeighbors();
    void UnassignNeighbors(int i, int j);
    bool Solve(int idx);
    void GenCommitment();
    void Print();
    int numnodes;
    int numneighbors;
    Node* nodes;
    bool** neighbors;
private:
    int32_t colormap[3];
    bool VerifyNode(int node);
    bool TestSolution();
};

#endif