#ifndef _CACHE_
#define _CACHE_

#include <inttypes.h>

/*
 * CACHE structure
 * tag       -> 64 bit address tag (stripped off in bit operations)
 * valid_bit -> Arbitary block assignment...fill at first "0" vacancy
 *
 * lru_age  -> Counter to track "age" of cache entry: 
 * Lower score -> I was used pretty recently
 * Higher score -> I never get used
 * 0 -> Fresh access or allocation 
 * */
typedef struct cache_t{
    uint64_t tag;
    unsigned int valid_bit;
    /* I was here first vs. somebody else's LRU */
    unsigned int fifo;  
    unsigned int lru_age;
}CACHE;


/* 
 * Counter values 
 * num_hits         -> cache hits
 * num_instructions -> How many "I" values were found in trace
 * 
 * */
typedef struct instruc_t{
	int num_hits; 
	int num_instructions;
	int num_misses;
}INSTRUCTIONS;


/* 
 * Cache properties 
 * sets         -> How many sets total in cache
 * cacheEntries -> How many actual entries are in cache
 * setBits      -> Number of bits representing a set
 * blockBits    -> Number of bits representing a block
 * shiftBlock   -> setBits + blockBits -> used in bit shifters
 * associativity-> set associativity 
 *
 * */
typedef struct cache_p_t{
    unsigned int sets;
    unsigned int cacheEntries; 
    unsigned int setBits;
    unsigned int blockBits;
    unsigned int shiftBlock;
    unsigned int associativity;
}CACHE_PROPERTIES;

#endif
