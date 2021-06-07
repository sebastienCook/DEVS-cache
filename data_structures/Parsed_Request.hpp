#ifndef _Parsed_Request_HPP_
#define _Parsed_Request_HPP_

#include <assert.h>
#include <iostream>
#include <string>

#include "../SimParameters/Parameters.h"

using namespace std;

struct Parsed_Request_t{
	Parsed_Request_t(){}
	Parsed_Request_t(int i_R_W, string i_tag, int i_index, int i_offset, string i_mem)
	 :R_W(i_R_W), tag(i_tag), index(i_index), offset(i_offset), mem(i_mem){}

	int R_W;
	string tag;
	int index;
	int offset; 
	string mem;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, Parsed_Request_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const Parsed_Request_t& msg);

#endif
