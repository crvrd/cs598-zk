#include "verifier.h"

// Start accepting socket connections
Verifier::Verifier(char* port) {
    network.Start(port);
    SeedRandom();
}

Verifier::~Verifier() {
    network.Close();
}

// Exchange security param and graph info
bool Verifier::BeginExchange(int k, int j) {
    int vertices, theirk, theirj, sec;
    if(!network.RecvInt(&theirk))
        return false;
    if(!network.RecvInt(&theirj))
        return false;
    if(!network.SendInt(k))
        return false;
    if(!network.SendInt(j))
        return false;
    if(max(k, theirk) > min(j, theirj))
        return false;
    sec = avg(max(k, theirk), min(j, theirj));
    if(!network.RecvInt(&vertices))
        return false;
    g = new Graph(vertices);
    if(!network.RecvGraph(g))
        return false;
    commitnum = sec*g->numedges;
    return true;
}

// Decide all of the edges we're going to ask to prove
void Verifier::GenerateEdgeRequests() {
    requests = new int32_t[commitnum * 2];
    for(int i = 0; i < commitnum; i++) {
        int j,k;
        do {
            j = rand() % g->numvertices;
            k = rand() % g->numvertices;
        } while(!g->edges[j][k]);
        requests[i*2] = j;
        requests[i*2+1] = k;
    }
    std::cout << commitnum << std::endl;
}

// Get the graph commitments from the prover
bool Verifier::RecvCommitments() {
    graphs = new Graph[commitnum];
    for(int i = 0; i < commitnum; i++) {
        graphs[i] = *g;
        if(!network.RecvCommitment(graphs + i))
            return false;
    }
    return true;
}

// Send the prover all of our requests
bool Verifier::SendEdgeRequests() {
    for(int i = 0; i < commitnum; i++) {
        if(!network.SendInt(requests[i*2]))
            return false;
        if(!network.SendInt(requests[i*2+1]))
            return false;
    }
    /*if(!network.SendBytes(requests, commitnum * 8))
        return false;*/
    return true;
}

bool Verifier::VerifyEdgeRequests() {
    // Get colors and random keys
    /*int32_t colors[commitnum * 2];
    uint64_t keys[commitnum * 2];
    if(!network.RecvBytes((char*)colors, commitnum * 8))
        return false;
    std::cout << "heren" << std::endl;
    std::cout << "heret" << std::endl;
    //if(!network.RecvBytes((char*)keys, commitnum * 16))
    //    return false;
    std::cout << "heref" << std::endl;

    // Assign colors and keys, and verify
    for(int i = 0; i < commitnum; i++) {
        int first = requests[i*2];
        int second = requests[i*2+1];
        std::cout << i << std::endl;
        graphs[i].vertices[first].color = colors[i*2];
        graphs[i].vertices[second].color = colors[i*2+1];
        graphs[i].vertices[first].randkey = keys[i*2];
        graphs[i].vertices[second].randkey = keys[i*2+1];
        std::cout << i << std::endl;
        std::cout << graphs[i].vertices[first].color << " ";
        std::cout << graphs[i].vertices[second].color << " ";
        std::cout << graphs[i].vertices[first].randkey << " ";
        std::cout << graphs[i].vertices[second].randkey << std::endl;
        if(graphs[i].vertices[first].color == graphs[i].vertices[second].color)
            return false;
        std::cout << i << std::endl;
        if(!(graphs[i].vertices[first].VerHash() && graphs[i].vertices[second].VerHash()))
            return false;
    }*/

    for(int i = 0; i < commitnum; i++) {
        int first = requests[i*2];
        int second = requests[i*2+1];
        if(!network.RecvInt(&graphs[i].vertices[first].color))
            return false;
        if(!network.RecvInt(&graphs[i].vertices[second].color))
            return false;
        if(!network.RecvKey(&graphs[i].vertices[first].randkey))
            return false;
        if(!network.RecvKey(&graphs[i].vertices[second].randkey))
            return false;
        if(graphs[i].vertices[first].color == graphs[i].vertices[second].color)
            return false;
        std::cout << i << std::endl;
        std::cout << graphs[i].vertices[first].color << " ";
        std::cout << graphs[i].vertices[second].color << " ";
        std::cout << graphs[i].vertices[first].randkey << " ";
        std::cout << graphs[i].vertices[second].randkey << std::endl;
        if(!(graphs[i].vertices[first].VerHash() && graphs[i].vertices[second].VerHash()))
            return false;
    }
    return true;
}

// Write the graph to a file
void Verifier::WriteGraph(std::ofstream& outfile) {
    outfile << g->numvertices << std::endl;

    for(int j = 0; j < g->numvertices; j++) {
        for(int k = 0; k < g->numvertices; k++) {
            if(g->edges[j][k])
                outfile << "1 ";
            else
                outfile << "0 ";
        }
        outfile << std::endl;
    }
}