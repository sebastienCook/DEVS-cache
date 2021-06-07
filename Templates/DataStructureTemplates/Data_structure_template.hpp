#ifndef _DATASTRUCTURENAME_HPP_
#define _DATASTRUCTURENAME_HPP_

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

struct Data_structure_name_t{
	Data_structure_name_t(){}
	Data_structure_name_t(int i_val, char *i_string)
	 :val(i_val), string(i_string){}

	int val;
	char *string;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, Data_structure_name_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const Data_structure_name_t& msg);
