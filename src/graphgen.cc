#include "graph.h"

using namespace std;

int main(int argc, char* argv[]) {
    SeedRandom();
    if(5 != argc) {
        cout << "usage: ./graphgen <# of graphs> <# of nodes per graph> <portion of edges> <dir>\n";
        exit(-1);
    }

    int nodes = stoi(argv[2]);
    int graphs = stoi(argv[1]);

    for(int i = 0; i < graphs; i++) {
        ofstream outfile;
        outfile.open(argv[4] + string("/g") + to_string(i));

        Graph* g = new Graph(nodes);

        for(int j = 0; j < nodes*atof(argv[3]); j++) {
            int k;
            int l;
            do {
                k = rand() % nodes;
                l = rand() % nodes;
            } while(k == l);
            g->AssignNeighbors(k, l);
        }

        outfile << nodes << endl;

        for(int j = 0; j < nodes; j++) {
            for(int k = 0; k < nodes; k++) {
                if(g->neighbors[j][k])
                    outfile << "1 ";
                else
                    outfile << "0 ";
            }
            outfile << endl;
        }

        outfile.close();
    }
}