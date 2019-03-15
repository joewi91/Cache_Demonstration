#define _STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include <math.h>

CACHE *create_cache (unsigned int size, unsigned int associativity, unsigned int line_size, CACHE_PROPERTIES *cache_p);

/* 
 * Creates and initializes the cache structure based on the size (KB), associativity, and
 * line size (Bytes) parameters.
 * */
CACHE *create_cache (unsigned int size, unsigned int associativity, unsigned int line_size, CACHE_PROPERTIES *cache_p)
{
    CACHE *c_block;

    // Upconvert size to kilobytes (size only, line_size preserved)
    int b_To_K = 1024;
    int i;

	// Associativity needs additional logic as direct mapping needs to be supported
    if((size * b_To_K) % 2 != 0  || line_size % 2 != 0 || (associativity > 1 && associativity % 2 != 0))
    {
        printf("Returned value(s) are not of a power of 2\n");
        printf("Size         : %d Mod Size         : %d\n", size*b_To_K, (size*b_To_K) % 2);
        printf("Associativity: %d Mod Associativity: %d\n", associativity, (associativity) % 2);
        printf("Line Size    : %d Mod Line Size    : %d\n", line_size, (line_size) % 2);
        return NULL;
    }
    
    /* set cache properties and store them to global struct */
    cache_p->associativity = associativity;
    cache_p->sets = ((size * b_To_K)/line_size)/(associativity);
    cache_p->cacheEntries = cache_p->sets * associativity; 
    cache_p->setBits = log2(cache_p->sets);
    cache_p->blockBits = log2(line_size); 
    cache_p->shiftBlock = cache_p->setBits + cache_p->blockBits;

	/* Cache properties printf */
    printf("sets: %d set bits: %d block_bits: %d shift_Amount %d\n", cache_p->sets, cache_p->setBits, cache_p->blockBits, cache_p->shiftBlock);
    
    /* malloc cache struct array */
    c_block = malloc(cache_p->cacheEntries * sizeof(CACHE));

    /* Default cache struct values */
    for(i = 0; i < cache_p->cacheEntries; i++)
    {
        c_block[i].tag = 0;
        c_block[i].valid_bit = 0;
        c_block[i].fifo = 0;
        c_block[i].lru_age = 0;
    }

    return c_block;
}
