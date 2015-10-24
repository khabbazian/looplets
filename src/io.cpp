#include <fstream>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <vector>

#include "io.h"

void log(std::string st){
	std::cerr<< st << std::endl;
}

void log(std::string st, std::string fname){
	std::ofstream ofile(fname.c_str());
	assert(ofile.is_open());
	ofile << st << std::endl;
}

void write_out_vec(std::vector<int> &t, std::string fname, bool append){
	std::ofstream ofile;
	if( append)
		ofile.open(fname.c_str(),  std::ios_base::app);
	else
		ofile.open(fname.c_str());
	assert(ofile.is_open());


	int i=0;
	for(auto iter = t.begin(); iter != t.end(); iter++, i++)
		ofile << i << " "<<*iter << std::endl;
		//ofile <<" "<<*iter;

	//int k=0, i=0;
	//std::vector<int>::iterator iter;
	//for(iter = t.begin(); iter != t.end(); iter++, i++)
	//	if(*iter != 0)
	//		k = i;
	//i=0;
	//iter = t.begin(); 
	//while( i++ < k )
	//	ofile <<*iter++<<" ";

	//ofile << std::endl;
	ofile.close();
}
