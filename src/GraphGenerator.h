#ifndef GRAPH_GENERATOR_H 
#define GRAPH_GENERATOR_H 

#include <Rcpp.h>

#include "types.h"
#include "GraphStatistics.h"

void generate_ER_graph_fast(double , uint , AdjacencyList &);
void generate_ER_graph(double, uint, AdjacencyList &, const int);
void generate_theta_graph(double , double, uint , AdjacencyList &);
void generate_graph_from_degree_dist(uint, FreqVec &, AdjacencyList &);
void generate_graph_from_degree_dist(uint, AdjacencyList &, AdjacencyList &);

#endif
