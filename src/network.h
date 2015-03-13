#ifndef NETWORK_COMMS
#define NETWORK_COMMS

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include "graph.h"

#define BACKLOG 10

class Network{
public:
    bool Start(char* port);
    bool Connect(char* hostname, char* port);
    void Close();
    bool SendBytes(void* ptr, int32_t num);
    bool RecvBytes(void* ptr, int32_t num);
    bool SendInt(int32_t i);
    bool RecvInt(int32_t* i);
    bool SendGraph(Graph* g);
    bool RecvGraph(Graph* g);
    bool SendCommitment(Graph* g);
    bool RecvCommitment(Graph* g);
private:
    bool SendVertex(Vertex n);
    bool RecvVertex(Vertex* n);
    bool SendVertices(Graph* g);
    bool RecvVertices(Graph* g);
    bool SendBool(bool i);
    bool RecvBool(bool* i);
    bool SendEdges(Graph* g);
    bool RecvEdges(Graph* g);
    bool SendKey(uint64_t k);
    bool RecvKey(uint64_t* k);
    bool SendVertexCommit(Graph* g, int idx);
    bool RecvVertexCommit(Graph* g, int idx);
    int sockfd;
    struct sockaddr their_addr;
    socklen_t sin_size;
};

#endif