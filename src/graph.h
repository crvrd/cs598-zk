#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"

class Graph{
public:
    Graph();
    Graph(int vertexnum);
    Graph(const Graph &g1);
    ~Graph();
    void AssignEdges(int i, int j);
    void NormalizeEdges();
    void UnassignEdges(int i, int j);
    bool Solve(int idx);
    void GenCommitment();
    void Print();
    int numvertices;
    int numedges;
    Vertex* vertices;
    bool** edges;
private:
    int32_t colormap[3];
    bool VerifyVertex(int vertex);
    bool TestSolution();
};

#endif