#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <string>
#include "types.h"

int get_num_edges(AdjacencyList &);
int get_num_non_zero_degrees(AdjacencyList &);
void read_edgelistfile(std::string, AdjacencyList &);
void verify_undirectedness(AdjacencyList &);

#endif
