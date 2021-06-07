#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "CPU_Request.hpp" 

ostream& operator<<(ostream& os, const CPU_Request_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.address << " " << msg.R_W << " " << msg.mem;
	return os;
}

istream& operator>>(istream& is, CPU_Request_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.address;
	is >> msg.R_W;
	is >> msg.mem;
	return is;
}
