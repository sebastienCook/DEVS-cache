#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Data_structure_name.hpp" 

ostream& operator<<(ostream& os, const Data_structure_name_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.val << " " << msg.string;
	return os;
}

istream& operator>>(istream& is, Data_structure_name_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.val;
	is >> msg.string;
	return is;
}
