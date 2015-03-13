COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -fpermissive -std=c++11
LINKER = g++

all: graphgen peggy victor

peggy: peggy.o prover.o network.o graph.o node.o utilities.o
	$(LINKER) peggy.o prover.o network.o graph.o node.o utilities.o -o peggy -lssl -lcrypto

victor: victor.o verifier.o network.o graph.o node.o utilities.o
	$(LINKER) victor.o verifier.o network.o graph.o node.o utilities.o -o victor -lssl -lcrypto

graphgen: graphgen.o graph.o node.o utilities.o
	$(LINKER) graphgen.o graph.o node.o utilities.o -o graphgen -lssl -lcrypto

graphgen.o: graphgen.cc graph.h
	$(COMPILER) $(COMPILER_FLAGS) graphgen.cc

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

peggy.o: peggy.cc prover.h
	$(COMPILER) $(COMPILER_FLAGS) peggy.cc

victor.o: victor.cc verifier.h
	$(COMPILER) $(COMPILER_FLAGS) victor.cc

clean:
	-rm -f *.o graphgen peggy victor
