# Algorithms for graph partitioning

We consider a graph G = (N,E)
 where N is a set of weighted nodes and E is a set of weighted edges.

## Sequential algorithms

### Local improvement methods

A local improvement algorithm takes as input a partition (usually  a bisection) of a graph G and tries to decrease the cut size by some local search method.

- [KL70] Kernighan and Lin
Given an initial bisection, the Kernighan-Lin method tries to find a sequende of node pair exchange that leads to an improvement of the cut size.

- [FM82] Fiduccia and Mattheyses
O(|E|) time

- [DMP94] Diekmann, Monien and Preis
- SA Simulated annealing
- Tabu Search
- [BM96] Genetic algorithm GA 

### Global methods

#### Geometric methods

- [BB87] Recursive coodinate bisection RCB
- [FL93] inetial method
- [GMT95] Gilbert, Miller and Teng implementation of [MTTV93]
- [BCN] Bokhari, Crockett and Nicol binary dissection

#### Coordinate-free methods

- [RGB] recursive graph bisection
- [RSB] recursice spectral bisection
- [multilevel-RSB] multilevel recursive spctarl bisection by [BS93]
- [multilevel-KL]

### Evaluation

Purely geometric methods are very fast but produce partitions with relatively high cut size.


Combinatorial algorithms such as RSB and recursive KL give good partitions but are relatively slow.

Multilevel approach results in much faster algorithms without any degradation in partition quality. A potential disadvantage of the multilevel approach is that is memory intensive.

Combining a recursive global method with some local improvement method leads to significantly reduced cut sizes: both the inertial and RSB methods give much better cut sizes if combined with the KL method. 

Still, the multilevel-KL method gives as good cut sizes as RSB-KL. in much less time. Multilevel-KL and RSB-KL give better cut sizes than inertial-KL but are slower than inertial-KL LH94.

Methods based on helpful sets do not seem competitive with the multilevel-KL method DMP94

## Parallel algorithms

### Local improvement methods

### Global methods

#### Geometric methods

#### Coordinate-free methods

## Conclusions

