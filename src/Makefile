COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -fpermissive
LINKER = g++

test: test.o prover.o verifier.o network.o graph.o node.o utilities.o
	$(LINKER) test.o prover.o verifier.o network.o graph.o node.o utilities.o -o test -lssl -lcrypto

honest_server: honest_server.o prover.o network.o graph.o node.o utilities.o
	$(LINKER) honest_server.o prover.o network.o graph.o node.o utilities.o -o honest_server -lssl -lcrypto

cheat_server: cheat_server.o prover.o network.o graph.o node.o utilities.o
	$(LINKER) cheat_server.o prover.o network.o graph.o node.o utilities.o -o cheat_server -lssl -lcrypto

nosol_server: nosol_server.o prover.o network.o graph.o node.o utilities.o
	$(LINKER) nosol_server.o prover.o network.o graph.o node.o utilities.o -o nosol_server -lssl -lcrypto

client: client.o verifier.o network.o graph.o node.o utilities.o
	$(LINKER) client.o verifier.o network.o graph.o node.o utilities.o -o client -lssl -lcrypto

all: test client honest_server cheat_server nosol_server

prover.o: prover.cc prover.h network.h
	$(COMPILER) $(COMPILER_FLAGS) prover.cc

verifier.o: verifier.cc verifier.h network.h
	$(COMPILER) $(COMPILER_FLAGS) verifier.cc

network.o: network.cc network.h graph.h
	$(COMPILER) $(COMPILER_FLAGS) network.cc

graph.o: graph.cc graph.h node.h
	$(COMPILER) $(COMPILER_FLAGS) graph.cc

node.o: node.cc node.h utilities.h
	$(COMPILER) $(COMPILER_FLAGS) node.cc

utilities.o: utilities.cc utilities.h
	$(COMPILER) $(COMPILER_FLAGS) utilities.cc

honest_server.o: honest_server.cc prover.h
	$(COMPILER) $(COMPILER_FLAGS) honest_server.cc

cheat_server.o: cheat_server.cc prover.h
	$(COMPILER) $(COMPILER_FLAGS) cheat_server.cc

nosol_server.o: nosol_server.cc prover.h
	$(COMPILER) $(COMPILER_FLAGS) nosol_server.cc

client.o: client.cc verifier.h
	$(COMPILER) $(COMPILER_FLAGS) client.cc

clean:
	-rm -f *.o test client honest_server cheat_server nosol_server
