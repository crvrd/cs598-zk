COMPILER = g++
COMPILER_FLAGS = -c -g -O0 -Wall -fpermissive
LINKER = g++

test: test.o network.o graph.o node.o
	$(LINKER) test.o network.o graph.o node.o -o test

all: test

network.o: network.cc network.h graph.h
	$(COMPILER) $(COMPILER_FLAGS) network.cc

graph.o: graph.cc graph.h node.h
	$(COMPILER) $(COMPILER_FLAGS) graph.cc

node.o: node.cc node.h
	$(COMPILER) $(COMPILER_FLAGS) node.cc

clean:
	-rm -f *.o test
