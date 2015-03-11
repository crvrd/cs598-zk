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

using namespace std;

class Network{
public:
    Network();
    bool Start();
    bool Connect();
    void Close();
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
private:
    bool SendNode(Node n);
    bool RecvNode(Node* n);
    bool SendBool(bool i);
    bool RecvBool(bool* i);
    bool SendKey(uint64_t k);
    bool RecvKey(uint64_t* k);
    bool SendNodeCommit(Graph* g, int idx);
    bool RecvNodeCommit(Graph* g, int idx);
    int sockfd;
    struct sockaddr their_addr;
    socklen_t sin_size;
};

#endif