#include "network.h"

// Start verification server on port specified
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

// Connect to host specified
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

// Send a buffer as-is
bool Network::SendBytes(void* ptr, int32_t num) {
    if(send(sockfd, ptr, num, 0) == -1)
        return false;
    return true;
}

// Receive a buffer as-is
bool Network::RecvBytes(char* ptr, int32_t num) {
    int bytesread = 0;
    int curr = 0;
    do {
        curr = recv(sockfd, ptr, num, 0);
        if(curr < 0) 
            return false;
        bytesread += curr;
        ptr += curr;
    } while(bytesread < num);
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

// Send an entire graph
bool Network::SendGraph(Graph* g) {
    if(!SendVertices(g)) 
        return false;
    if(!SendEdges(g))
        return false;
    if(!SendInt(g->numedges))
        return false;
    return true;
}

bool Network::RecvGraph(Graph* g) {
    if(!RecvVertices(g)) 
        return false;
    if(!RecvEdges(g))
        return false;
    if(!RecvInt(&g->numedges))
        return false;
    return true;
}

// Send the commitment for the whole graph
bool Network::SendCommitment(Graph* g) {
    for(int i = 0; i < g->numvertices; i++) {
        if(!SendVertexCommit(g, i))
            return false;
    }
    return true;
}

bool Network::RecvCommitment(Graph* g) {
    for(int i = 0; i < g->numvertices; i++) {
        if(!RecvVertexCommit(g, i))
            return false;
    }
    return true;
}

// Send a single vertex
bool Network::SendVertex(Vertex n) {
    if(!SendInt(n.color))
        return false;
    if(!SendKey(n.randkey))
        return false;
    return true;
}

bool Network::RecvVertex(Vertex* n) {
    if(!RecvInt(&n->color))
        return false;
    if(!RecvKey(&n->randkey))
        return false;
    return true;
}

// Send all the vertices in a graph 
bool Network::SendVertices(Graph* g) {
    if(!SendInt(g->numvertices))
        return false;
    for(int i = 0; i < g->numvertices; i++) {
        if(!SendVertex(g->vertices[i]))
            return false;
    }
    return true;
}

bool Network::RecvVertices(Graph* g) {
    if(!RecvInt(&g->numvertices))
        return false;
    for(int i = 0; i < g->numvertices; i++) {
        if(!RecvVertex(&g->vertices[i]))
            return false;
    }
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

// Send the edge matrix
bool Network::SendEdges(Graph* g) {
    for(int i = 0; i < g->numvertices; i++) {
        for(int j = 0; j < g->numvertices; j++) {
            if(!SendBool(g->edges[i][j]))
                return false;
        }
    }
    return true;
}

bool Network::RecvEdges(Graph* g) {
    for(int i = 0; i < g->numvertices; i++) {
        for(int j = 0; j < g->numvertices; j++) {
            if(!RecvBool(&g->edges[i][j]))
                return false;
        }
    }
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

// Send the commitment for an individual vertex
bool Network::SendVertexCommit(Graph* g, int idx) {
    unsigned char* tosend = g->vertices[idx].commithash;
    if(send(sockfd, tosend, SHA256_DIGEST_LENGTH, 0) == -1)
        return false;
    return true;
}

bool Network::RecvVertexCommit(Graph* g, int idx) {
    unsigned char* torecv = g->vertices[idx].commithash;
    if(recv(sockfd, torecv, SHA256_DIGEST_LENGTH, 0) < 0)
        return false;
    return true;
}