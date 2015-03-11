#include "network.h"

using namespace std;

//serv = should act as server, otherwise act as client.
Network::Network() {}

bool Network::Start() {
    int tempsock;
    struct addrinfo messages, *servinfo, *p;
    //char s[INET6_ADDRSTRLEN];
    int sockreuse = 1;
    int rv;

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_INET;
    messages.ai_socktype = SOCK_STREAM;

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

bool Network::Connect() {
    struct addrinfo messages, *servinfo, *p;
    int rv;
    //char s[INET6_ADDRSTRLEN];

    memset(&messages, 0, sizeof messages);
    messages.ai_family = AF_UNSPEC;
    messages.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(PROOF_HOST, PROOF_PORT, &messages, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and connect to the first we can
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

    /*inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);*/

    freeaddrinfo(servinfo);
    return true;
}

void Network::Close() {
    close(sockfd);
}

bool Network::SendInt(int32_t i) {
    if(send(sockfd, &i, sizeof(int32_t), 0) == -1)
        return false;
    return true;
}

bool Network::RecvInt(int32_t* i) {
    if(recv(sockfd, i, sizeof(int32_t), 0) < 0)
        return false;
    return true;
}

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
    return false;
}

bool Network::SendGraph(Graph* g) {
    if(!SendNodes(g)) 
        return false;
    if(!SendNeighbors(g))
        return false;
    return true;
}

bool Network::RecvGraph(Graph* g) {
    if(!RecvNodes(g)) 
        return false;
    if(!RecvNeighbors(g))
        return false;
    return true;
}

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