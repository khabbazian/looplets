#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include <set>

typedef unsigned int uint;

typedef int Vertex;
typedef std::vector<Vertex> VList;
typedef std::vector<VList> AdjacencyList;

typedef std::pair<Vertex,Vertex> Edge;
typedef std::set<Edge> EdgeSet; 

typedef std::vector<int> FreqVec;

#endif 
