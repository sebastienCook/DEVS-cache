#ifndef _Cache_write_HPP_
#define _Cache_write_HPP_

#include <assert.h>
#include <iostream>
#include <string>

#include "../SimParameters/Parameters.h"

using namespace std;

struct Cache_write_t{
	Cache_write_t(){}
	Cache_write_t(string tag, int i_index, int i_offset, string i_mem)
	 :index(i_index), offset(i_offset), mem(i_mem){}

	string tag;
	int index;
	int offset;
	string mem;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, Cache_write_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const Cache_write_t& msg);

#endif // _Cache_write_HPP_ 

