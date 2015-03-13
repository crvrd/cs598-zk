# cs598-zk
CS 598 Cryptography Project: Zero-knowledge proof for graph colorability

### Description ###
Verifier asks the prover to prove that a specific graph is 3-colorable.  The prover uses an interactive zero-knowledge proof to prove the colorability.  The protocol uses sha256(x|r) for commitment, where x is the information the prover is committing to, and r is a random string.  Each commitment represents one vertex of the graph.  When two commitments of adjacent vertices are decoded, they should have different colors.

### Protocol ###
* Prover finds or generates a graph
* Prover finds an n-coloring for the graph
* Prover and Verifier send each other security parameter k.  If k's do not match, then quit.
* Prover sends the graph to Verifier
* Verifier generates E*k edge commitment requests
* Prover generates E*k graph commitments and sends them to Verifier
* Verifier sends all E*k edge commitment requests to Prover
* Prover sends all E*k edge commitments to Verifier
* Verifier verifies all E*k edge commitments
* If all E*k edge commitments are verified, then the Verifier considers the graph colorability to be proven.

* make cheating prover
* test from not-home