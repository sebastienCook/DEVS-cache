#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Memory_write.hpp" 

ostream& operator<<(ostream& os, const Memory_write_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.address << " " << msg.mem;
	return os;
}

istream& operator>>(istream& is, Memory_write_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.address;
	is >> msg.mem;
	return is;
}
