#include "graph.h"

using namespace std;

Graph::Graph() {
    numvertices = 0;
    numedges = 0;
}

// Create an empty graph with the appropriate number of vertices
Graph::Graph(int vertexnum) {
    numvertices = vertexnum;
    vertices = new Vertex[numvertices];
    edges = new bool*[numvertices];
    numedges = 0;
    for(int i = 0; i < numvertices; i++) {
        edges[i] = new bool[numvertices];
        for(int j = 0; j < numvertices; j++) {
            edges[i][j] = false;
        }
    }
}

Graph::Graph(const Graph &g1) {
    numvertices = g1.numvertices;
    numedges = g1.numedges;
    vertices = new Vertex[numvertices];
    edges = new bool*[numvertices];
    for(int i = 0; i < numvertices; i++) {
        edges[i] = new bool[numvertices];
        for(int j = 0; j < numvertices; j++) {
            edges[i][j] = g1.edges[i][j];
        }
    }
    for(int i = 0; i < 3; i++) {
        colormap[i] = g1.colormap[i];
    }
}

Graph::~Graph() {
    for(int i = 0; i < numvertices; i++) {
        delete[] edges[i];
    }
}

// Mark two vertices as being adjacent
void Graph::AssignEdges(int i, int j) {
    edges[i][j] = true;
    edges[j][i] = true;
    numedges++;
}

void Graph::NormalizeEdges() {
    numedges /= 2;
}

// Mark two vertices as being distant (not often used)
void Graph::UnassignEdges(int i, int j) {
    edges[i][j] = false;
    edges[j][i] = false;
    numedges--;
}

// Brute-force solving the coloring problem (TODO: actually solve)
bool Graph::Solve(int idx) {
    if(idx == numvertices)
        return TestSolution();
    for(int color = 1; color < 4; color++) {
        vertices[idx].color = color;
        if(Solve(idx+1))
            return true;
    }
    return false;

}

// Make sure all edges are of different color
// used to test our solution
bool Graph::VerifyVertex(int vertex) {
    for(int i = 0; i < numvertices; i++) {
        if(edges[vertex][i]) {
            if(vertices[i].color == vertices[vertex].color)
                return false;
        }
    }
    return true;
}

// Make sure the solution we found works,
// meaning all edges are of different colors
bool Graph::TestSolution() {
    for(int i = 0; i < numvertices; i++) {
        if(!VerifyVertex(i)) {
            return false;
        }
    }
    cout << "Solution: ";
    for(int i = 0; i < numvertices; i++) {
        cout << vertices[i].color << " ";
    }
    cout << endl;
    return true;
}

// Generate a commitment for the graph
// This randomizes the colors, and re-assigns new random 
// values to the vertices, and re-generates hashes
void Graph::GenCommitment() {
    // Randomize the colors, so the client can't link rounds
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

    // Generate random keys and hashes for vertices
    for(int i = 0; i < numvertices; i++) {
        vertices[i].Randomize(colormap);
        vertices[i].GenHash();
    }
}

// Print out the graph
// Prints out a list of all the vertices and associated values
// Also prints out a matrix for edges
void Graph::Print() {
    for(int i = 0; i < numvertices; i++) {
        cout << "Vertex " << i << ": ";
        cout << vertices[i].color << ", ";
        cout << vertices[i].randkey << endl;
    }
    for(int i = 0; i < numvertices; i++) {
        for(int j = 0; j < numvertices; j++) {
            if(edges[i][j]) 
                cout << "1 ";
            else
                cout << "0 ";
        }
        cout << endl;
    }
    cout << endl;
}