# cs598-zk
CS 598 Cryptography Project: Zero-knowledge proof for graph colorability

### Description ###
Verifier asks the prover to prove that a specific graph is n-colorable.  The prover uses an interactive zero-knowledge proof to prove the colorability.  The protocol uses sha256(x|r) for commitment, where x is the information the prover is committing to, and r is a random string.  Each commitment represents one vertex of the graph.  When two commitments of adjacent vertices are decoded, they should have different colors.

### Protocol ###
* Verifier sends prover a file describing the graph
* Prover finds an n-coloring for the graph
* For |E|^2 rounds:
	* Prover generates a commitment for each edge of the graph, and sends them to the verifier
	* Verifier requests an edge
	* Prover sends the information for the requested edge.  This includes the colors and the random strings for the two corresponding edges
	* Verifier verifies the commitment corresponds to two differently-colored vertices
* If all of the |E|^2 requests are verified, the verifier considers the colorability to be proven.  

