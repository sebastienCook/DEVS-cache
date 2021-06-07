#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Tag_change.hpp" 

ostream& operator<<(ostream& os, const Tag_change_t& msg) {
	// chage content bellow to match data structure content of hpp file
	os << msg.tag << " " << msg.index;
	return os;
}

istream& operator>>(istream& is, Tag_change_t& msg) {
	// chage content bellow to match data structure content of hpp file
	is >> msg.tag;
	is >> msg.index;
	return is;
}
