#include <iostream>
#include <cstdio>
#include <queue>
#include <set>
#include <algorithm>

#include <cassert>
#include <cstring>
#include <cmath>
#include <chrono>

//R interface files
#include<Rcpp.h>

#include "types.h"
#include "GraphGenerator.h"
#include "GraphStatistics.h"
#include "GraphUtils.h"
#include "io.h"


// [[Rcpp::plugins(cpp11)]]

using namespace std;

inline void add_loop_edges_to_remove(EdgeSet &ES, VList &vParents, int cV, int i, int root){ 

	ES.insert(Edge(cV,i)); 
	ES.insert(Edge(i,cV)); 
	Vertex p = cV, pp;

	//for(int t=0; t<2; t++){
		//p = t ? cV : i;
		while(p != root){ // marking edges in the loop
			pp = vParents[p];
			ES.insert(Edge(pp,p)); 
			ES.insert(Edge(p,pp));
			p = pp;
		}
	//}
}

int bfs_undirected_graph(AdjacencyList const &E, const int loopLen, 
		bool &maxLevelReached, EdgeSet &ES){

	int const nV = E.size();
	int nVinLoop=0, nLoops=0;
	VList vLevels(nV,-1), vParents(nV,-1); 
	VList const negativeOne(nV,-1);
	queue<Vertex> bfsQ;
	maxLevelReached=true;

	for(int root=0; root<nV; root++){

		if( E[root].size() < 2 ) 
			continue;

		bfsQ.push(root);
		vLevels[root] = 0;
		int cLevel = 0; 
		int const maxLevel = loopLen % 2 == 1 ? loopLen/2: loopLen/2 -1;

		while( !bfsQ.empty() ){
			const Vertex cV = bfsQ.front();
			cLevel = vLevels[cV]+1;
			if( cLevel > maxLevel )   
				break;
			for(uint k=0; k<E[cV].size(); k++){
				const int i = E[cV][k]; 
				if( vLevels[i] != -1)
					continue;
				bfsQ.push(i);
				vLevels[i]  = cLevel;
				vParents[i] = cV;
			}
			bfsQ.pop();
		}

		maxLevelReached = maxLevelReached? bfsQ.empty(): false; 

		if ( loopLen % 2 == 1) // if odd
			while( !bfsQ.empty() ){
				const Vertex cV = bfsQ.front();
				for(uint k=0; k<E[cV].size(); k++){
					const int i = E[cV][k]; 
					if ( vLevels[i]  == -1 || vParents[cV] == i ) 
						continue; 
					nLoops++;
					add_loop_edges_to_remove(ES, vParents, cV, i, root);
					assert ( vLevels[i] + vLevels[cV] + 1 == loopLen);
				} 
				bfsQ.pop();
			}

		if ( loopLen % 2 == 0 ) // if even
			while( !bfsQ.empty() ){
				const Vertex cV = bfsQ.front();
				for(uint k=0; k<E[cV].size(); k++){
					const int i = E[cV][k]; 
					if( i == vParents[cV] ) 
						continue;

					assert( vLevels[i] == -1); // there must not be a smaller loop
					for(uint kk=0; kk<E[i].size(); kk++){ 
						const int ii = E[i][kk];
						if ( vLevels[ii] == -1 || ii == cV ) 
							continue;
						// there must not be a smaller loop
						assert( vLevels[ii] + vLevels[cV] + 2 == loopLen );
						nLoops++;
						vParents[i] = cV;
						add_loop_edges_to_remove(ES, vParents, i, ii, root);
						vParents[i] = -1;
					}
				} 
				bfsQ.pop();
			}

        
        //FIXME: TODO: replace memcpy with a more safe way
		memcpy(&(vLevels[0]) , &(negativeOne[0]), nV*sizeof(VList::value_type));
		memcpy(&(vParents[0]), &(negativeOne[0]), nV*sizeof(VList::value_type));
	}
	assert( nLoops%loopLen == 0);
	return nLoops/loopLen/2;
}




// [[Rcpp::export]]
Rcpp::NumericMatrix decompose_graph_into_loops(Rcpp::NumericMatrix edgeList, const int maxLoopLength=10){

    //NOTE: converting the edgelist to edgeset representation
	AdjacencyList E;
    for(int idx=0; idx<edgeList.nrow(); ++idx){
        const int v1 = edgeList(idx, 0), v2 = edgeList(idx, 1);
        while( E.size() <= v1 )
            E.push_back(VList());
        E[v1].push_back(v2);
    }

	const int nVertices=E.size();

    Rcpp::NumericMatrix LoopsStats(maxLoopLength, 5);
    colnames(LoopsStats) = Rcpp::CharacterVector::create("lLength", "nLoops", "nLoopEdges", "nNodes", "nEdges");
    int LoopsStatsC=0;

	EdgeSet ES;
	//printf("LL\tnL\tnLE\tnGV\tnGE\n");
	for(int ll = 3; ll <  std::min(nVertices,maxLoopLength); ++ll){

		bool mllr = false;
		int const nL = bfs_undirected_graph(E, ll, mllr, ES);

        LoopsStats(LoopsStatsC,0) = ll;
        LoopsStats(LoopsStatsC,1) = nL;
        LoopsStats(LoopsStatsC,2) = ES.size()/2;
        LoopsStats(LoopsStatsC,3) = get_num_non_zero_degrees(E);
        LoopsStats(LoopsStatsC,4) = get_num_edges(E);
        ++LoopsStatsC;


		if(mllr){
			//printf("every CC(tree) has depth < %d \n",ll);
			break;
		}

		{
			EdgeSet::iterator iter = ES.begin(); 
			while( iter != ES.end() ){
				Vertex v1 = iter->first, v2 = iter->second;
				VList::iterator eI = find(E[v1].begin(), E[v1].end(), v2);
				assert(eI != E[v1].end());
				E[v1].erase(eI, eI+1);
				iter++;
			}
		}

		ES.erase(ES.begin(), ES.end());
	}
    return(LoopsStats);
}
