#ifndef NETWORK_COMMS
#define NETWORK_COMMS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "graph.h"

#define PROOF_PORT "20079"
#define PROOF_HOST "localhost"

#define BACKLOG 10

class Network{
public:
    Network();
    bool Start();
    bool Start(char* port);
    bool Connect();
    bool Connect(char* hostname, char* port);
    void Close();
    bool SendBytes(void* ptr, int32_t num);
    bool RecvBytes(void* ptr, int32_t num);
    bool SendInt(int32_t i);
    bool RecvInt(int32_t* i);
    bool SendNodes(Graph* g);
    bool RecvNodes(Graph* g);
    bool SendNeighbors(Graph* g);
    bool RecvNeighbors(Graph* g);
    bool SendGraph(Graph* g);
    bool RecvGraph(Graph* g);
    bool SendCommitment(Graph* g);
    bool RecvCommitment(Graph* g);
    bool SendProof(Graph* g, int idx);
    bool RecvProof(Graph* g, int idx);
    bool SendBool(bool i);
    bool RecvBool(bool* i);
private:
    bool SendNode(Node n);
    bool RecvNode(Node* n);
    bool SendKey(uint64_t k);
    bool RecvKey(uint64_t* k);
    bool SendNodeCommit(Graph* g, int idx);
    bool RecvNodeCommit(Graph* g, int idx);
    int sockfd;
    struct sockaddr their_addr;
    socklen_t sin_size;
};

#endif