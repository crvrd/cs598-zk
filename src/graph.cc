#include "graph.h"

using namespace std;

Graph::Graph(int nodenum) {
    numnodes = nodenum;
    nodes = new Node[numnodes];
    neighbors = new bool*[numnodes];
    for(int i = 0; i < numnodes; i++) {
        neighbors[i] = new bool[numnodes];
        for(int j = 0; j < numnodes; j++) {
            neighbors[i][j] = false;
        }
    }
}

Graph::~Graph() {
    for(int i = 0; i < numnodes; i++) {
        delete[] neighbors[i];
    }
}

void Graph::AssignNeighbors(int i, int j) {
    neighbors[i][j] = true;
    neighbors[j][i] = true;
}

void Graph::UnassignNeighbors(int i, int j) {
    neighbors[i][j] = false;
    neighbors[j][i] = false;
}

// Brute-force Solving (TODO: actually solve)
bool Graph::Solve(int idx) {
    if(idx == numnodes)
        return TestSolution();
    for(int color = 1; color < 4; color++) {
        nodes[idx].color = color;
        if(Solve(idx+1))
            return true;
    }
    return false;

}

bool Graph::VerifyNode(int node) {
    for(int i = 0; i < numnodes; i++) {
        if(neighbors[node][i]) {
            if(nodes[i].color == nodes[node].color)
                return false;
        }
    }
    return true;
}

bool Graph::TestSolution() {
    for(int i = 0; i < numnodes; i++) {
        if(!VerifyNode(i)) {
            return false;
        }
    }
    cout << "Solution: ";
    for(int i = 0; i < numnodes; i++) {
        cout << nodes[i].color << " ";
    }
    cout << endl;
    return true;
}