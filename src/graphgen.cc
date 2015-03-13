#include "graph.h"

using namespace std;

int main(int argc, char* argv[]) {
    SeedRandom();
    if(5 != argc) {
        cout << "usage: ./graphgen <# of graphs> <# of vertices per graph> <portion of edges> <dir>\n";
        exit(-1);
    }

    int vertices = stoi(argv[2]);
    int graphs = stoi(argv[1]);

    for(int i = 0; i < graphs; i++) {
        ofstream outfile;
        outfile.open(argv[4] + string("/g") + to_string(i));

        Graph* g = new Graph(vertices);

        for(int j = 0; j < vertices*atof(argv[3]); j++) {
            int k;
            int l;
            do {
                k = rand() % vertices;
                l = rand() % vertices;
            } while(k == l);
            g->AssignEdges(k, l);
        }

        outfile << vertices << endl;

        for(int j = 0; j < vertices; j++) {
            for(int k = 0; k < vertices; k++) {
                if(g->edges[j][k])
                    outfile << "1 ";
                else
                    outfile << "0 ";
            }
            outfile << endl;
        }

        outfile.close();
    }
}