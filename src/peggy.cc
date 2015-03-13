#include "prover.h"

int main(int argc, char* argv[]) {
    if(6 != argc) {
        std::cout << "usage: ./peggy <hostname> <port> <graph file> ";
        std::cout << "<security: low> <security: high>" << std::endl;
        exit(-1);
    }

    std::ifstream infile;
    infile.exceptions(std::ifstream::eofbit | std::ifstream::badbit | 
                      std::ifstream::failbit | std::ifstream::goodbit);
    try {
        infile.open(argv[3]);
    } catch(...) {
        std::cout << "Problem opening file, exiting." << std::endl;
        exit(-1);
    }

    Prover* peggy;
    try {
        peggy = new Prover(infile);
    } catch(...) {
        std::cout << "Could not initialize network connection." << std::endl;
        exit(-1);
    }
    if(!peggy->SolveGraph()) {
        std::cout << "No solution." << std::endl;
        delete peggy;
        exit(0);
    }
    if(!peggy->BeginExchange(std::stoi(argv[4]), 
                             std::stoi(argv[5]), 
                             argv[1], 
                             argv[2])) {
        std::cout << "Security parameter exchange failed; probable mismatch";
        std::cout << std::endl;
        delete peggy;
        exit(-1);
    }
    if(!peggy->GenerateCommitments()) {
        std::cout << "Could not send graph" << std::endl;
        delete peggy;
        exit(-1);
    }
    if(!peggy->ProcessEdgeRequests()) {
        std::cout << "Edge requests failed, probable non-edge query";
        std::cout << std::endl;
        delete peggy;
        exit(-1);
    }

    delete peggy;
}