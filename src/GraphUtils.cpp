
#include <cassert>
#include <fstream>
#include <algorithm>
#include "GraphUtils.h"


int get_num_edges(AdjacencyList &E){
	int nEdges = 0;
	for(uint i=0; i<E.size(); i++)
		nEdges += E[i].size();
	assert( !(nEdges % 2) );
	return nEdges/2; // # edges = sum degrees / 2
}

int get_num_non_zero_degrees(AdjacencyList &E){
	int n = 0;
	for(uint i=0; i<E.size(); i++)
		if(E[i].size()>0)  n++;
	return n;
}

void read_edgelistfile(std::string fname, AdjacencyList &E)
{
	std::ifstream iFile(fname.c_str());
	assert( iFile.is_open() );

	std::string line;
	int v1, v2;
	while( getline(iFile, line) ){
		if ( line[0] == '#' || line.size() == 0) continue;
		sscanf(line.c_str(), "%d %d", &v1, &v2);
		if(v1 == v2 ) // removing selfloops
			continue;
		while( E.size() <= v1 ) 
			E.push_back(VList());

		if( std::find(E[v1].begin(), E[v1].end(), v2) == E[v1].end() )
			E[v1].push_back(v2);

		// symmetrizing the graph! NOTE: W A directed graphs?
		while( E.size() <= v2 ) 
			E.push_back(VList());

		if( std::find(E[v2].begin(), E[v2].end(), v1) == E[v2].end() )
			E[v2].push_back(v1);
	}
	iFile.close();
}

void verify_undirectedness(AdjacencyList &E){
	for(int v=0; v<E.size(); v++)
		for(int i=0; i<E[v].size(); i++){
			int u = E[v][i];
			assert(u != v);
			VList::iterator eI = std::find(E[u].begin(), E[u].end(), v);
			assert(eI != E[u].end());
		}
}


