#include "graph.h"

int main(int argc, char* argv[]) {
    SeedRandom();
    if(5 != argc) {
        std::cout << "usage: ./graphgen <# of graphs> <# of vertices per graph> <portion of edges> <dir>\n";
        exit(-1);
    }

    int vertices = std::stoi(argv[2]);
    int graphs = std::stoi(argv[1]);

    for(int i = 0; i < graphs; i++) {
        std::ofstream outfile;
        outfile.open(argv[4] + std::string("/g") + std::to_string(i));

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

        outfile << vertices << std::endl;

        for(int j = 0; j < vertices; j++) {
            for(int k = 0; k < vertices; k++) {
                if(g->edges[j][k])
                    outfile << "1 ";
                else
                    outfile << "0 ";
            }
            outfile << std::endl;
        }

        if(g->Solve(0))
            std::cout << "g" + std::to_string(i) + ": 3-colorable" << std::endl;
        else
            std::cout << "g" + std::to_string(i) + ":not 3-colorable" << std::endl;

        outfile.close();
    }
}