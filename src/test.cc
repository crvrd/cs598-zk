#include "verifier.h"
#include "prover.h"

#include <limits>

using namespace std;

void TestInts() {
    cout << "\n============================" << endl;
    cout << "TESTING NETWORK SENDINT:" << endl;

    int tosend = rand();
    int torecv = 0; 

    if(!fork()) {
        Network receiver;
        if(receiver.Start())
            cout << "Started server" << endl;
        else {
            cout << "Error: Unable to start server" << endl;
            exit(-1);
        }
        receiver.RecvInt(&torecv);
        if(torecv == tosend)
            cout << "CORRECT" << endl;
        else
            cout << "INCORRECT" << endl;
        receiver.Close();
        exit(0);
    }

    usleep(1000);
    Network sender;
    if(sender.Connect())
        cout << "Started client" << endl;
    else
        cout << "Error:  unable to start client" << endl;
    usleep(1000);
    sender.SendInt(tosend);
    sender.Close();
}

void TestNeighbors() {
    cout << "\n============================" << endl;
    cout << "TESTING NETWORK SENDNEIGHBORS:" << endl;

    Graph* g = new Graph(8);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(rand()%2 && i != j)
                g->AssignNeighbors(i, j);
            else
                g->UnassignNeighbors(i, j);
        }
    }
    for(int i = 0; i < g->numnodes; i++) {
        for(int j = 0; j < g->numnodes; j++) {
            if(g->neighbors[i][j]) 
                cout << "1 ";
            else
                cout << "0 ";
        }
        cout << endl;
    }
    cout << endl;

    Graph* h = new Graph(8);

    if(!fork()) {
        Network receiver;
        if(!receiver.Start()) {
            cout << "Error: Unable to start server" << endl;
            exit(-1);
        }
        receiver.RecvNeighbors(h);
        for(int i = 0; i < g->numnodes; i++) {
            for(int j = 0; j < g->numnodes; j++) {
                if(h->neighbors[i][j]) 
                    cout << "1 ";
                else
                    cout << "0 ";
            }
            cout << endl;
        }
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(h->neighbors[i][j] != g->neighbors[i][j]) {
                    cout << "INCORRECT" << endl;
                    receiver.Close();
                    exit(0);
                }
            }
        }
        cout << "CORRECT" << endl;
        receiver.Close();
        exit(0);
    }

    usleep(1000);
    Network sender;
    if(!sender.Connect())
        cout << "Error:  unable to start client" << endl;
    usleep(1000);
    sender.SendNeighbors(g);
    sender.Close();
}

void TestNodes() {
    cout << "\n============================" << endl;
    cout << "TESTING NETWORK SENDNODES:" << endl;

    Graph* g = new Graph(8);
    for(int i = 0; i < 8; i++) {
        g->nodes[i] = Node(rand()%3 + 1);
        g->nodes[i].randkey = (uint64_t)rand();
    }
    for(int i = 0; i < g->numnodes; i++) {
        cout << "Node " << i << ": ";
        cout << g->nodes[i].color << ", ";
        cout << g->nodes[i].randkey << endl;
    }
    cout << endl;

    Graph* h = new Graph(8);

    if(!fork()) {
        Network receiver;
        if(!receiver.Start()) {
            cout << "Error: Unable to start server" << endl;
            exit(-1);
        }
        receiver.RecvNodes(h);
        for(int i = 0; i < h->numnodes; i++) {
            cout << "Node " << i << ": ";
            cout << h->nodes[i].color << ", ";
            cout << h->nodes[i].randkey << endl;
        }
        for(int i = 0; i < 8; i++) {
            if(h->nodes[i].color != g->nodes[i].color) {
                cout << "INCORRECT" << endl;
                receiver.Close();
                exit(0);
            }
            if(h->nodes[i].randkey != g->nodes[i].randkey) {
                cout << "INCORRECT" << endl;
                receiver.Close();
                exit(0);
            }
        }
        cout << "CORRECT" << endl;
        receiver.Close();
        exit(0);
    }

    usleep(1000);
    Network sender;
    if(!sender.Connect())
        cout << "Error:  unable to start client" << endl;
    usleep(1000);
    sender.SendNodes(g);
    sender.Close();
}

void TestSendGraph() {
    cout << "\n============================" << endl;
    cout << "TESTING NETWORK SENDGRAPH:" << endl;

    Graph* g = new Graph(8);
    for(int i = 0; i < 8; i++) {
        g->nodes[i] = Node(rand()%3 + 1);
        g->nodes[i].randkey = (uint64_t)rand();
    }
    for(int i = 0; i < g->numnodes; i++) {
        cout << "Node " << i << ": ";
        cout << g->nodes[i].color << ", ";
        cout << g->nodes[i].randkey << endl;
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(rand()%2 && i != j)
                g->AssignNeighbors(i, j);
            else
                g->UnassignNeighbors(i, j);
        }
    }
    for(int i = 0; i < g->numnodes; i++) {
        for(int j = 0; j < g->numnodes; j++) {
            if(g->neighbors[i][j]) 
                cout << "1 ";
            else
                cout << "0 ";
        }
        cout << endl;
    }
    cout << endl;

    Graph* h = new Graph(8);

    if(!fork()) {
        Network receiver;
        if(!receiver.Start()) {
            cout << "Error: Unable to start server" << endl;
            exit(-1);
        }
        receiver.RecvGraph(h);
        for(int i = 0; i < h->numnodes; i++) {
            cout << "Node " << i << ": ";
            cout << h->nodes[i].color << ", ";
            cout << h->nodes[i].randkey << endl;
        }
        for(int i = 0; i < 8; i++) {
            if(h->nodes[i].color != g->nodes[i].color) {
                cout << "INCORRECT" << endl;
                receiver.Close();
                exit(0);
            }
            if(h->nodes[i].randkey != g->nodes[i].randkey) {
                cout << "INCORRECT" << endl;
                receiver.Close();
                exit(0);
            }
        }
        for(int i = 0; i < g->numnodes; i++) {
            for(int j = 0; j < g->numnodes; j++) {
                if(h->neighbors[i][j]) 
                    cout << "1 ";
                else
                    cout << "0 ";
            }
            cout << endl;
        }
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(h->neighbors[i][j] != g->neighbors[i][j]) {
                    cout << "INCORRECT" << endl;
                    receiver.Close();
                    exit(0);
                }
            }
        }
        cout << "CORRECT" << endl;
        receiver.Close();
        exit(0);
    }

    usleep(1000);
    Network sender;
    if(!sender.Connect())
        cout << "Error:  unable to start client" << endl;
    usleep(1000);
    sender.SendGraph(g);
    sender.Close();
}

void TestSolve() {
    cout << "\n============================" << endl;
    cout << "TESTING GOOD GRAPH:" << endl;

    Graph* g = new Graph(9);
    g->AssignNeighbors(0,1);
    g->AssignNeighbors(0,2);
    g->AssignNeighbors(1,3);
    g->AssignNeighbors(1,4);
    g->AssignNeighbors(2,3);
    g->AssignNeighbors(3,4);
    g->AssignNeighbors(3,5);
    g->AssignNeighbors(4,5);
    g->AssignNeighbors(2,7);
    g->AssignNeighbors(5,6);
    g->AssignNeighbors(6,7);
    g->AssignNeighbors(6,8);
    g->AssignNeighbors(7,8);

    if(g->Solve(0))
        cout << "SOLVED" << endl;
    else
        cout << "NO SOLUTION" << endl;

    delete g;

    cout << "\n============================" << endl;
    cout << "TESTING BAD GRAPH:" << endl;

    g = new Graph(9);
    g->AssignNeighbors(0,1);
    g->AssignNeighbors(0,2);
    g->AssignNeighbors(1,3);
    g->AssignNeighbors(1,4);
    g->AssignNeighbors(1,5);
    g->AssignNeighbors(2,3);
    g->AssignNeighbors(3,4);
    g->AssignNeighbors(3,5);
    g->AssignNeighbors(4,5);
    g->AssignNeighbors(2,7);
    g->AssignNeighbors(5,6);
    g->AssignNeighbors(6,7);
    g->AssignNeighbors(6,8);
    g->AssignNeighbors(7,8);

    if(g->Solve(0))
        cout << "SOLVED" << endl;
    else
        cout << "NO SOLUTION" << endl;
}

void TestReadGraph(ifstream& infile) {
    cout << "\n============================" << endl;
    cout << "TESTING GRAPH READING AND SENDING:" << endl;
    if(!fork()) {
        Prover* peggy = new Prover();
        if(!peggy->RecvAndSolveGraph())
            cout << "couldn't solve" << endl;
        peggy->PrintGraph();
        peggy->GenerateCommitment();
        peggy->PrintGraph();
        peggy->SendGraphCommitment();
        delete peggy;
        exit(0);
    }
    usleep(1000);
    Verifier* victor = new Verifier(infile);
    victor->SendGraph();
    victor->RecvGraphCommitment();
    victor->PrintGraph();
    delete victor;
}

int main(int argc, char*argv[]) {
    bool canrun = true;
    if(2 != argc) {
        cout << "usage: ./test <graph file>" << endl;
        cout << "Not running tests which involve graph reading." << endl;
        canrun = false;
    }
    SeedRandom();
    TestSendGraph();
    usleep(1000);
    TestSolve();
    usleep(1000);
    if(canrun) {
        ifstream infile;
        infile.open(argv[1]);
        TestReadGraph(infile);
    }
    usleep(1000);
}