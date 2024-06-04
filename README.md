# TugaNet

## Description

This project was developed for the class of Analysis and Synthesis of Algorithms.

## Objective

Calculate the **maximum number of transmissions** the infectious disease can make on a given network of people. When representing the network of people as a potentially cyclic graph (because people can know each other mutually), we can find the longest possible path along the edges of the graph by collapsing strongly connected components (SCCs) followed by a DFS traversal along the resulting directed acyclic graph (DAG) for all nodes.

- Refer to the file `p2.pdf` for the project's instructions and details.
- Refer to the file `relatorio_tuganet.pdf` for the full report on the solution's time complexity and description.

**Source Code:** The source code for the problem's solution is in the `src` directory in the file `proj2.cpp`.

To compile `proj2.cpp`, use:
```text
g++ -std=c++11 -O3 -Wall proj2.cpp -lm
```
**Unit Tests:** To generate unit tests you can use `circular.sh` for circular networks of people and
`gen_tuganet.cpp` for potencially cyclic networks. These files can be found in `test_generators` directory.

To compile `gen_tuganet.cpp`, use:
```text
g++ -std=c++11 -O3 -Wall -o gen_tuganet gen_tuganet.cpp -lm
```

To run `gen_tuganet.cpp` use:
```text
./gen_tuganet V E SubN m M seed  > testfile
```
>Where:
>- V is the number of vertices of the graph
>- E is the number of edges of the graph
>- SubN the number of sub-networks in the graph (SCCs)
>- m is the minimal number of individuals per sub-network
>- M is the maximum number of individuals per sub-network

To run `circular.sh` use:
```text
bash circular.sh V > testfile
```
(where V is the number of vertices in the circular graph)
