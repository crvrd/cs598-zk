#include "verifier.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if(5 != argc) {
        std::cout << "usage:  ./victor <port> <security: low> ";
        std::cout << "<security: high> <output dir>" << std::endl;
        exit(-1);
    }
    while(1) {
        std::cout << "here0" << std::endl;
        Verifier* victor = new Verifier(argv[1]);
        std::cout << "here1" << std::endl;
        if(!victor->BeginExchange(std::stoi(argv[2]), std::stoi(argv[3]))) {
            std::cout << "Could not begin exchange; ";
            std::cout << "probable security parameter mismatch" << std::endl;
            delete victor;
            continue;
        }
        std::cout << "here2" << std::endl;
        victor->GenerateEdgeRequests();
        std::cout << "here3" << std::endl;
        if(!victor->RecvCommitments()) {
            std::cout << "Could not receive commitments." << std::endl;
            delete victor;
            continue;
        }
        std::cout << "here4" << std::endl;
        if(!victor->SendEdgeRequests()) {
            std::cout << "Problem sending requests." << std::endl;
            delete victor;
            continue;
        }
        std::cout << "here5" << std::endl;
        if(!victor->VerifyEdgeRequests()) {
            std::cout << "Graph verification Failed; ";
            std::cout << "prover is probably cheating." << std::endl;
            delete victor;
            continue;
        }
        std::cout << "here6" << std::endl;

        std::ofstream outfile;
        outfile.open(argv[4] + std::string("/g") + 
                     std::to_string(time(NULL)) + std::to_string(rand()));
        victor->WriteGraph(outfile);
        outfile.close();
        delete victor;
        std::cout << "Graph verified!" << std::endl;
    }

}