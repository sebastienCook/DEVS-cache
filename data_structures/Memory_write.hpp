#ifndef _Memory_write_HPP_
#define _Memory_write_HPP_

#include <assert.h>
#include <iostream>
#include <string>

#include "../SimParameters/Parameters.h"

using namespace std;

struct Memory_write_t{
	Memory_write_t(){}
	Memory_write_t(char *i_address, char *i_mem)
	 :address(i_address), mem(i_mem){}

	char *address;
	char *mem;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, Memory_write_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const Memory_write_t& msg);

#endif
