#include "prover.h"

#include <iostream>

// Build a new graph based on the file
Prover::Prover(std::ifstream& infile) {
    int vertexnum, edgeval;
    infile >> vertexnum;
    g = new Graph(vertexnum);
    for(int i = 0; i < vertexnum; i++) {
        for(int j = 0; j < vertexnum; j++) {
            infile >> edgeval;
            if(edgeval) {
                g->AssignEdges(i, j);
            }
        }
    }
    g->NormalizeEdges();
    SeedRandom();
}

// Close the socket when we leave
Prover::~Prover() {
    network.Close();
}

bool Prover::SolveGraph(bool cheat) {
    if(g->Solve(0) || cheat)
        return true;
    return false;
}

// Connect to the server, and send security param and graph info
bool Prover::BeginExchange(int k, int j, char* hostname, char* port) {
    if(!network.Connect(hostname, port))
        throw -1;
    int theirk, theirj, sec;
    if(!network.SendInt(k))
        return false;
    if(!network.SendInt(j))
        return false;
    if(!network.RecvInt(&theirk))
        return false;
    if(!network.RecvInt(&theirj))
        return false;
    if(max(k, theirk) > min(j, theirj))
        return false;
    sec = avg(max(k, theirk), min(j, theirj));
    commitnum = sec*g->numedges;
    std::cout << "parameter: " << sec << std::endl;
    if(!network.SendInt(g->numvertices))
        return false;
    return network.SendGraph(g);
    return true;
}

// Make commitment hashes for all of the graphs
bool Prover::GenerateCommitments() {
    graphs = new Graph[commitnum];
    for(int i = 0; i < commitnum; i++) {
        graphs[i] = *g;
        graphs[i].GenCommitment();
    }
    for(int i = 0; i < commitnum; i++) {
        if(!network.SendCommitment(graphs + i))
            return false;
    }
    return true;
}

// Prove all of the edges are good
bool Prover::ProcessEdgeRequests(bool cheat) {
    // Receive requests
    requests = new int32_t[commitnum * 2];
    if(!network.RecvBytes((char*)requests, commitnum * 8))
        return false;

    // Find response information
    int32_t colors[commitnum * 2];
    uint64_t keys[commitnum * 2];
    for(int i = 0; i < commitnum; i++) {
        int first = requests[i*2];
        int second = requests[i*2+1];
        if(!graphs[i].edges[first][second])
            return false;
        colors[i*2] = graphs[i].vertices[first].color;
        colors[i*2+1] = graphs[i].vertices[second].color;
        keys[i*2] = graphs[i].vertices[first].randkey;
        keys[i*2+1] = graphs[i].vertices[second].randkey;
        if(cheat && (colors[i*2] == colors[i*2+1]))
            colors[i*2] = (colors[1*2+1]%3) + 1;
    }

    // Send responses
    if(!network.SendBytes(colors, commitnum * 8))
        return false;
    if(!network.SendBytes(keys, commitnum * 16))
        return false;
    return true;
}