#include <cassert>
#include <iostream>
#include <set>
#include "GraphStatistics.h"

void degree_dist_un(AdjacencyList &E, FreqVec &fv){
	assert( fv.size() >= E.size()-1 );
	for(int i=0; i<E.size(); i++)
		fv[ E[i].size() ]++;
}

void degree_dist_un(EdgeSet &ES, const int nV, FreqVec &fv){
	assert( fv.size() >= nV -1 );
	FreqVec tmp(nV, 0);
	for(auto iter = ES.begin(); iter != ES.end(); iter++)
		tmp[ iter->first ]++; 
	for(int i=0; i<nV; i++)
		fv[ tmp[i] ]++;
}
