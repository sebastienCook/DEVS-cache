/*
*
* Author: Sebastien Cook
* 
* Parameters:
* This is a simple header file that defines Cache features
*/

#ifndef _PARAMETERS_
#define _PARAMETERS_

/* Print debug messages */
#define DEBUG 0

/* Allocate cache on write miss */
#define CWA_Miss 0

/* Cache Sizes (in bytes) */
#define CACHE_SIZE 64
#define BLOCK_SIZE 8

/* Mem Size */
#define MEM_SIZE 262144 //262144 : 256 KB 131072: 128 KB  65536: 65 KB
#define MEM_BLOCK_SIZE 8

/* Address def */
#define OFFSET 3 // offset = log2(BLOCK_SIZE)
#define INDEX 3 // Index = log2(CACHE_SIZE/BLOCK_SIZE)
#define TAG 26 // Tag = 32 - offset - index

/* Delay paramaters */
#define MEM_RESPONCE_DELAY "00:00:00:100"
#define CACHE_RESPONCE_DELAY "00:00:00:001"
#define MUX_RESPONCE_DELAY "00:00:00:000"


#endif //_PARAMETERS_


