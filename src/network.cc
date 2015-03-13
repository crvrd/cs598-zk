#include "network.h"

using namespace std;

//serv = should act as server, otherwise act as client.
Network::Network() {}

// Start listening, and accept incoming connections.
// This is called by the server
bool Network::Start() {
    int tempsock;
    struct addrinfo messages, *servinfo, *p;
    int sockreuse = 1;
    int rv;

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_INET;
    messages.ai_socktype = SOCK_STREAM;
    messages.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PROOF_PORT, &messages, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((tempsock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(tempsock, SOL_SOCKET, SO_REUSEADDR, &sockreuse,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(-1);
        }

        if (bind(tempsock, p->ai_addr, p->ai_addrlen) == -1) {
            close(tempsock);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return false;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(tempsock, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }


    sin_size = sizeof(their_addr);

    for(int i = 0; i < 1000; i++) {
        sockfd = accept(tempsock, (struct sockaddr*)&their_addr, &sin_size);
        if(-1 == sockfd)
            perror("accept");
        else
            break;
    }
    return true;
}

// Overloaded start, for if we need a different port.  i.e., we have
// multiple servers.
bool Network::Start(char* port) {
    int tempsock;
    struct addrinfo messages, *servinfo, *p;
    int sockreuse = 1;
    int rv;

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_INET;
    messages.ai_socktype = SOCK_STREAM;
    messages.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port, &messages, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((tempsock = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(tempsock, SOL_SOCKET, SO_REUSEADDR, &sockreuse,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(-1);
        }

        if (bind(tempsock, p->ai_addr, p->ai_addrlen) == -1) {
            close(tempsock);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return false;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(tempsock, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }


    sin_size = sizeof(their_addr);

    sockfd = accept(tempsock, (struct sockaddr*)&their_addr, &sin_size);
    if(-1 == sockfd)
        perror("accept");
    close(tempsock);
    return true;
}

// Connect to a server.
// This is called by the client
bool Network::Connect() {
    struct addrinfo messages, *servinfo, *p;
    int rv;

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_UNSPEC;
    messages.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(PROOF_HOST, PROOF_PORT, &messages, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return false;
    }

    freeaddrinfo(servinfo);
    return true;
}

// Overloaded connect function
bool Network::Connect(char* hostname, char* port) {
    struct addrinfo messages, *servinfo, *p;
    int rv;

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_UNSPEC;
    messages.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &messages, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return false;
    }

    freeaddrinfo(servinfo);
    return true;
}

// Close the connection
void Network::Close() {
    close(sockfd);
}

bool Network::SendBytes(void* ptr, int32_t num) {
    if(send(sockfd, ptr, num, 0) == -1)
        return false;
    return true;
}

bool Network::RecvBytes(void* ptr, int32_t num) {
    if(recv(sockfd, ptr, num, 0) < 0)
        return false;
    return true;
}

// Send an int across the socket
bool Network::SendInt(int32_t i) {
    if(send(sockfd, &i, sizeof(int32_t), 0) == -1)
        return false;
    return true;
}

// Receive an int from the socket
bool Network::RecvInt(int32_t* i) {
    if(NULL == i)
        return false;
    if(recv(sockfd, i, sizeof(int32_t), 0) < 0)
        return false;
    return true;
}

// Send all the nodes in a graph 
bool Network::SendNodes(Graph* g) {
    if(!SendInt(g->numnodes))
        return false;
    for(int i = 0; i < g->numnodes; i++) {
        if(!SendNode(g->nodes[i]))
            return false;
    }
    return true;
}

bool Network::RecvNodes(Graph* g) {
    if(!RecvInt(&g->numnodes))
        return false;
    for(int i = 0; i < g->numnodes; i++) {
        if(!RecvNode(&g->nodes[i]))
            return false;
    }
    return true;
}

// Send the neighbor matrix
bool Network::SendNeighbors(Graph* g) {
    for(int i = 0; i < g->numnodes; i++) {
        for(int j = 0; j < g->numnodes; j++) {
            if(!SendBool(g->neighbors[i][j]))
                return false;
        }
    }
    return true;
}

bool Network::RecvNeighbors(Graph* g) {
    for(int i = 0; i < g->numnodes; i++) {
        for(int j = 0; j < g->numnodes; j++) {
            if(!RecvBool(&g->neighbors[i][j]))
                return false;
        }
    }
    return true;
}

// Send an entire graph
bool Network::SendGraph(Graph* g) {
    if(!SendNodes(g)) 
        return false;
    if(!SendNeighbors(g))
        return false;
    if(!SendInt(g->numneighbors))
        return false;
    return true;
}

bool Network::RecvGraph(Graph* g) {
    if(!RecvNodes(g)) 
        return false;
    if(!RecvNeighbors(g))
        return false;
    if(!RecvInt(&g->numneighbors))
        return false;
    return true;
}

// Send a bool
bool Network::SendBool(bool b) {
    if(send(sockfd, &b, sizeof(bool), 0) == -1)
        return false;
    return true;
}

bool Network::RecvBool(bool* b) {
    if(recv(sockfd, b, sizeof(bool), 0) < 0)
        return false;
    return true;
}

// Send a single node
bool Network::SendNode(Node n) {
    if(!SendInt(n.color))
        return false;
    if(!SendKey(n.randkey))
        return false;
    return true;
}

bool Network::RecvNode(Node* n) {
    if(!RecvInt(&n->color))
        return false;
    if(!RecvKey(&n->randkey))
        return false;
    return true;
}

// Send a 64-bit random key
bool Network::SendKey(uint64_t k) {
    if(send(sockfd, &k, sizeof(uint64_t), 0) == -1)
        return false;
    return true;
}

bool Network::RecvKey(uint64_t* k) {
    if(recv(sockfd, k, sizeof(uint64_t), 0) < 0)
        return false;
    return true;
}

// Send the commitment for the whole graph
bool Network::SendCommitment(Graph* g) {
    for(int i = 0; i < g->numnodes; i++) {
        if(!SendNodeCommit(g, i))
            return false;
    }
    return true;
}

bool Network::RecvCommitment(Graph* g) {
    for(int i = 0; i < g->numnodes; i++) {
        if(!RecvNodeCommit(g, i))
            return false;
    }
    return true;
}

// Send the commitment for an individual node
bool Network::SendNodeCommit(Graph* g, int idx) {
    unsigned char* tosend = g->nodes[idx].commithash;
    if(send(sockfd, tosend, SHA256_DIGEST_LENGTH, 0) == -1)
        return false;
    return true;
}

bool Network::RecvNodeCommit(Graph* g, int idx) {
    unsigned char* torecv = g->nodes[idx].commithash;
    if(recv(sockfd, torecv, SHA256_DIGEST_LENGTH, 0) < 0)
        return false;
    return true;
}

// Send the key and color for a node
bool Network::SendProof(Graph* g, int idx) {
    if(!SendInt(g->nodes[idx].color))
        return false;
    if(!SendKey(g->nodes[idx].randkey))
        return false;
    return true;
}

bool Network::RecvProof(Graph* g, int idx) {
    if(!RecvInt(&g->nodes[idx].color))
        return false;
    if(!RecvKey(&g->nodes[idx].randkey))
        return false;
    return true;
}