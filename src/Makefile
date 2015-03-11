COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -fpermissive
LINKER = g++

test: test.o prover.o verifier.o network.o graph.o node.o utilities.o
	$(LINKER) test.o prover.o verifier.o network.o graph.o node.o utilities.o -o test -lssl -lcrypto

all: test

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

clean:
	-rm -f *.o test
