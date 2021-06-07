#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Parsed_Request.hpp" 

ostream& operator<<(ostream& os, const Parsed_Request_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.R_W << " " << msg.tag << " " << msg.index << " " << msg.offset << "  " << msg.mem;
	return os;
}

istream& operator>>(istream& is, Parsed_Request_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.R_W;
	is >> msg.tag;
	is >> msg.index;
	is >> msg.offset;
	is >> msg.mem;
	return is;
}
