#include "graph.h"

using namespace std;

Graph::Graph(int nodenum) {
    numnodes = nodenum;
    nodes = new Node[numnodes];
    neighbors = new bool*[numnodes];
    numneighbors = 0;
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
    numneighbors++;
}

void Graph::UnassignNeighbors(int i, int j) {
    neighbors[i][j] = false;
    neighbors[j][i] = false;
    numneighbors--;
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

void Graph::GenCommitment() {
    colormap[0] = ((rand() % 3) + 1);
    do {
        colormap[1] = ((rand() % 3) + 1);
    } while(colormap[1] == colormap[0]);
    if(1 != colormap[0] && 1 != colormap[1])
        colormap[2] = 1;
    else if(2 != colormap[0] && 2 != colormap[1])
        colormap[2] = 2;
    else
        colormap[2] = 3;

    for(int i = 0; i < numnodes; i++) {
        nodes[i].Randomize(colormap);
        nodes[i].GenHash();
    }
    for(int i = 0; i < numnodes; i++) {
        if(!nodes[i].VerHash())
            cout << "PROBLEM" << endl;
    }
}

void Graph::Print() {
    for(int i = 0; i < numnodes; i++) {
        cout << "Node " << i << ": ";
        cout << nodes[i].color << ", ";
        cout << nodes[i].randkey << endl;
    }
    for(int i = 0; i < numnodes; i++) {
        for(int j = 0; j < numnodes; j++) {
            if(neighbors[i][j]) 
                cout << "1 ";
            else
                cout << "0 ";
        }
        cout << endl;
    }
    cout << endl;
}