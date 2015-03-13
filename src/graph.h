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
    bool Solve(int idx);
    void GenCommitment();
    int numvertices;
    int numedges;
    Vertex* vertices;
    bool** edges;
private:
    int32_t colormap[3];
    bool VerifyVertexColor(int vertex);
    bool TestSolution();
};

#endif