#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

void log(std::string );
void log(std::string , std::string );
void write_out_vec(std::vector<int> &t, std::string fname, bool a=false);

#endif
