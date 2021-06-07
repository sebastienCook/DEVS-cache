#ifndef _Tag_check_HPP_
#define _Tag_check_HPP_

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

struct Tag_change_t{
	Tag_change_t(){}
	Tag_change_t(string i_tag, int i_index)
	 :tag(i_tag), index(i_index){}

	string tag;
	int index;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, Tag_change_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const Tag_change_t& msg);

#endif
