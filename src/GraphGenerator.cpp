#include <random> // std=c++11
#include <set>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <chrono>

#include "GraphGenerator.h"
#include "GraphUtils.h"
#include "types.h"

using namespace std;
inline double uniform_random()
{
	return (double) rand()/( (double) RAND_MAX + 1) ;
}

inline int random_uniform_int(const int min, const int max)
{
	return  (int) ( (max - min)*uniform_random() + min );
}


void generate_ER_graph_fast(double lambda, uint nVertices, AdjacencyList &E){
	std::default_random_engine generator1; // two generators to make sure they are independent
	std::default_random_engine generator2;
	std::uniform_int_distribution<int> uniform_int(0,nVertices-1);
	std::poisson_distribution<int> poisson(lambda/2); // FIXME replace it w binomial
	
	std::set<int> rSet;

	for(uint v1=0; v1<nVertices; v1++){
		const int nE = poisson(generator1);
		while(rSet.size() < nE ){
			const int pv = uniform_int(generator2);
			if ( pv == v1 ) continue; // no self loop
			rSet.insert( pv  ); // no duplication	
		}
		std::set<int>::iterator iter = rSet.begin();

		while( iter != rSet.end() ){
			const int v2 = *iter++;
			while( E.size() <= v1 )
				E.push_back(VList());

			if ( std::find(E[v1].begin(), E[v1].end(), v2) == E[v1].end() ) //FIXME
				E[v1].push_back(v2);

			while( E.size() <= v2 ) 
				E.push_back(VList());

			if ( std::find(E[v2].begin(), E[v2].end(), v1) == E[v2].end() ) //FIXME
				E[v2].push_back(v1);
		}
		rSet.erase(rSet.begin(), rSet.end());
	}
}


// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]
Rcpp::NumericMatrix generate_ER_graph(const double lambda, const int nVertices, const int rseed=1){

    AdjacencyList E;
    int eCounter = 0;
    const double p = lambda / nVertices;
    
    srand(rseed);
    for(int v1=0; v1<nVertices; ++v1)
        for(int v2=v1+1; v2<nVertices; ++v2){ // v<->u only once
            if( uniform_random() > p )  
                continue;

            while( E.size() <= v1 )
                E.push_back(VList());
            E[v1].push_back(v2);
            ++eCounter;

            while( E.size() <= v2 ) 
                E.push_back(VList());
            E[v2].push_back(v1);
            ++eCounter;
        }

    Rcpp::NumericMatrix edgeList(eCounter, 2);
    int eC = 0;
    for(int i=0;i<E.size();++i)
        for(int j=0; j<E[i].size(); ++j){
            edgeList(eC,0) = i;
            edgeList(eC,1) = E[i][j];
            ++eC;
        }
    return(edgeList);
}

template <class T>
T sample(vector<T> &iv, bool replacement = true){
	if(replacement){
		unsigned int idx = random_uniform_int(0, iv.size() ); 
		return iv[idx];
	}else{
		// implement me!
		assert(false);
	}
}

void generate_graph_from_degree_dist(const uint numEdges, AdjacencyList &E, AdjacencyList &Enew){
	const uint nE = get_num_edges(E);
	const uint nV = E.size(); // the new graph has the same number of nodes as given graph
	assert( numEdges < nV*(nV-1)/2); //even less!

	vector<int> list_1/*,list_2*/; // undirected graph for now!
	list_1.reserve(2*nE);
	for(int i=0; i<E.size(); i++)
		for(int j=0; j<E[i].size(); j++)
			list_1.push_back(i);
	//idx = 0;
	//for(int i=0; i<E.size(); i++)
	//	for(int j=0; j<E[i].size(); j++)
	//		list_2[idx++]=E[i][j];
	Enew.reserve(nV);
	srand( chrono::system_clock::now().time_since_epoch().count() );
	for(int i=0; i<numEdges; i++){
		const int v1 = sample<int>(list_1), v2 = sample<int>(list_1);//only undirected for now!
		if ( v1 == v2 ) 
			continue; //omiting self loop. maybe we need to sample again FIXME

		while( Enew.size() <= v1 )
			Enew.push_back(VList());

		if ( find(Enew[v1].begin(), Enew[v1].end(), v2) == Enew[v1].end() )
			Enew[v1].push_back(v2);

		while( Enew.size() <= v2 ) 
			Enew.push_back(VList());

		if ( find(Enew[v2].begin(), Enew[v2].end(), v1) == Enew[v2].end() ) 
			Enew[v2].push_back(v1);
	}
}

void generate_graph_from_degree_dist(const uint nVertices, FreqVec &fv, AdjacencyList &E){

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator1(seed1);

	unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator2(seed2);

	discrete_distribution<int> discrete_dist(fv.begin(), fv.end() );
	uniform_real_distribution<double> uniform_int(0, nVertices);
	
	vector<int> theta(nVertices,0);
	for(int v=0; v<nVertices; v++)
		theta[v] = discrete_dist(generator1); 

	int nStubs = 0, nV = 0;
	for(int v=0; v<theta.size(); v++){
		nStubs += theta[v];
		nV += theta[v] ? 1 : 0; 
	}
	// making the sum of degrees even
	theta[ uniform_int(generator2) ] += nStubs % 2;
	//printf("# of edges %d \n", nStubs/2);
	
	//int nCollision = 0;
	for(uint v1=0; v1<nVertices; v1++){

		set<int> rSet;
		while( theta[v1] > 0 && nV > 1){ // the last one is crazy!

			//FIXME!
			int t = random_uniform_int(0, nV )+1, i=0;
			for(; i < theta.size(); i++){
				t -= theta[i] > 0 ? 1 : 0;
				if( t == 0) 
					break;
			}
			const int v2 = i;	

			if(v1 == v2) continue;

			nV -= --theta[v1] == 0 ? 1: 0;	
			nV -= --theta[v2] == 0 ? 1: 0;	

			rSet.insert(v2); //just to eliminate duplication
		}

		set<int>::iterator iter = rSet.begin();
		while( iter != rSet.end() ){
			const int v2 = *iter++;
			while( E.size() <= v1 )
				E.push_back(VList());

			if ( find(E[v1].begin(), E[v1].end(), v2) == E[v1].end() )
				E[v1].push_back(v2);

			while( E.size() <= v2 ) 
				E.push_back(VList());

			if ( find(E[v2].begin(), E[v2].end(), v1) == E[v2].end() ) 
				E[v2].push_back(v1);
		}
	}
}

void generate_theta_graph(double lambda, double v, uint nVertices, AdjacencyList &E){

	std::default_random_engine generator1, generator2;

	const double var = v, mean = lambda; 
	const double alpha = mean*mean/var, beta = var/mean; 

	gamma_distribution<double> gamma(alpha, beta);
	uniform_real_distribution<double> uniform_int(0, nVertices);
	
	vector<int> theta(nVertices,0);
	for(int v=0; v<nVertices; v++)
		theta[v] = (int) min<double>(gamma(generator1), nVertices); 

	int nStubs = 0, nV = 0;
	for(int v=0; v<theta.size(); v++){
		nStubs += theta[v];
		nV += theta[v] ? 1 : 0; 
	}
	theta[ uniform_int(generator2) ] += nStubs % 2;
	printf("# of edges %d \n", nStubs/2);
	
	int nCollision = 0;
	for(uint v1=0; v1<nVertices; v1++){

		set<int> rSet;
		while( theta[v1] > 0 && nV > 1){ // the last one is crazy!

			int t = random_uniform_int(0, nV )+1, i=0;
			for(; i < theta.size(); i++){
				t -= theta[i] > 0 ? 1 : 0;
				if( t == 0) 
					break;
			}
			const int v2 = i;	

			if(v1 == v2) continue;

			nV -= --theta[v1] == 0 ? 1: 0;	
			nV -= --theta[v2] == 0 ? 1: 0;	

			rSet.insert(v2); //eliminating duplication
		}

		set<int>::iterator iter = rSet.begin();
		while( iter != rSet.end() ){
			const int v2 = *iter++;
			while( E.size() <= v1 )
				E.push_back(VList());

			if ( find(E[v1].begin(), E[v1].end(), v2) == E[v1].end() )
				E[v1].push_back(v2);

			while( E.size() <= v2 ) 
				E.push_back(VList());

			if ( find(E[v2].begin(), E[v2].end(), v1) == E[v2].end() ) 
				E[v2].push_back(v1);
		}
	}
}
