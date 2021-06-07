#ifndef _CPU_Request_HPP_
#define _CPU_Request_HPP_

#include <assert.h>
#include <iostream>
#include <string>

#include "../SimParameters/Parameters.h"

using namespace std;

struct CPU_Request_t{
	CPU_Request_t(){}
	CPU_Request_t(string i_address, int i_R_W, string i_mem)
	 :address(i_address), R_W(i_R_W), mem(i_mem){}


	
	string address; 
	int R_W;
	string mem;
};
// change data structure name bellow for actual name 
istream& operator>> (istream& is, CPU_Request_t& msg);
// change data structure name bellow for actual name
ostream& operator<< (ostream& os, const CPU_Request_t& msg);

#endif
