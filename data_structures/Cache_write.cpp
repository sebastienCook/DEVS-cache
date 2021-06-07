#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Cache_write.hpp" 

ostream& operator<<(ostream& os, const Cache_write_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.tag << " " << msg.index << " " << msg.offset << "  " << msg.mem;
	return os;
}

istream& operator>>(istream& is, Cache_write_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.tag;
	is >> msg.index;
	is >> msg.offset;
	is >> msg.mem;
	return is;
}
