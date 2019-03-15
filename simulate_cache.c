/* 
 * Simulate the performance of a cache using a reference stream of instruction, and load and store addresses. 
 * Types of caches: Unified cache or a separate I-cache and D-cache (I and D not supported or written for).
 * 
 * Tunable cache parameters: cache size, associativity, and cache-line size.
 * 
 * The "access_cache" function is where the unified (U) cache is demonstrated and simulated
 * with the "simple_trace" file...some of this code was taken from a larger cache simulation
 * then adapted for specialized use, thus only the unified cache is supported.
 *
 * Replacement policy: Least-recently used (LRU).
 * Output: Hit (miss) rate.
 * 
 * Date created: 10/28/2017
 * Date modified: 3/15/2019 
 *
 */

#define _STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "parse_user_options.h"
#include "cache.h"

#define DEBUG_FLAG 0

/* Global cache */
CACHE_PROPERTIES 	cache_p;
INSTRUCTIONS 		ins;

/* Function definitions */
void print_usage (void);
extern USER_OPTIONS* parse_user_options (int argc, char **argv);
extern CACHE *create_cache (unsigned int size, unsigned int associativity, unsigned int line_size, CACHE_PROPERTIES *cache_p);
void lru_update_ages(CACHE *cache, int start, int end, int j);

/* This was used to print the cache as needed during development */
/* void print_cache (CACHE *); */

int access_cache (CACHE *cache, int reference_type, uint64_t memory_address);
void simulate_unified_cache (CACHE *u_cache, FILE *fp);
void simulate_i_d_cache (CACHE *i_cache, CACHE *d_cache, FILE *fp);


/* Function to update cache LRU aging */
void lru_update_ages(CACHE *cache, int start, int end, int j)
{
	int i;

	/* Increase every value's age */
    for(i = start; i < end; i++)
    {
        cache[i].lru_age++;
        cache[i].fifo++;
    }

    /* Set the valid entry to 0 */
    cache[j].lru_age = 0;
}


/* Edit this function to compete the cache simulator. */
int access_cache (CACHE *cache, int reference_type, uint64_t memory_address)
{
    /* Parse the memory address (which is in hex) into the corresponding offset, index, and tag bits. */
    uint64_t tag_bits = memory_address >> (cache_p.shiftBlock);    
    uint64_t index_set = ((memory_address - (tag_bits << (cache_p.shiftBlock))) >> cache_p.blockBits);
    
 	int hit = 0; /* Default "not a hit" */
    int loopStart = index_set * cache_p.associativity;
    int loopEnd = loopStart + cache_p.associativity;
    int j;
    
    /* Index into the cache set using the index bits. Check if the tag stored in the cache matches the tag 
     * correcponding to the memory address. If yes and the valid bit is set to true, then declare a 
     * cache hit. Else, declare a cache miss, and update the cache with the specified tag. If all sets are 
     * full, then use the LRU algorithm to evict the appropriate cache line. */
    
    // Declare a cache hit
    for(j = loopStart; j < loopEnd; j++)
    {
        if(cache[j].tag == tag_bits)
        {
			hit = 1;
			cache[j].tag = tag_bits;
			lru_update_ages(cache, loopStart, loopEnd, j);
            return hit;
        }
    }

    /* If no hits identified, declare a miss */ 
    ins.num_misses++;
    
    /* New entries go to arbitrary empty cache block */
    for(j = loopStart; j < loopEnd; j++)
    {
		if(cache[j].valid_bit == 0)
		{
			hit = 0;
			cache[j].valid_bit = 1;
			cache[j].tag = tag_bits;
			lru_update_ages(cache, loopStart, loopEnd, j);
            return hit;
		}
	}
	
   	/* LRU algorithm variables */
    int m;
    int oldest_Slot = 0;
    int oldest_Index = index_set;
    /* Secondary check of fifo */
    int fifo_store = 0;
    /* Miss re-declared */
    hit = 0;
    
    /* Find the oldest LRU */
    for(m = loopStart; m < loopEnd; m++)
    {
        if(cache[m].lru_age > oldest_Slot)
        {
            oldest_Slot = cache[m].lru_age;
            oldest_Index = m;
            fifo_store = cache[m].fifo;
        }
    }
    
    /* Check FIFO and avoid duplication */
	for(m = loopStart; m < loopEnd; m++)
    {
    	if(cache[m].lru_age == cache[oldest_Index].lru_age && cache[m].fifo > fifo_store)
    	{
    	    fifo_store = cache[m].fifo;
            oldest_Index = m;
    	}
    }
    
    /* Replace cache entry and increase cache entry ages */
    cache[oldest_Index].tag = tag_bits;
    lru_update_ages(cache, loopStart, loopEnd, oldest_Index);
    cache[oldest_Index].fifo = 0;
    
    /* Default return is hit = 1 */
    return hit;
}

/* Simulates a unified cache for instructions and data. */
void simulate_unified_cache (CACHE *u_cache, FILE *fp)
{
    char reference_type;
    uint64_t memory_address;
    int status;
    
    while (1){
        /* Obtain the type of reference: instruction fetch, load, or store, and the memory address. */
        status = fscanf (fp, " %c %" SCNx64, &reference_type, &memory_address);         
        if (status == EOF)
            break;

        /* Simulate the cache. */
         switch (reference_type) {
            case 'L':
                /* NO NEED TO IMPLEMENT THIS. */
                break;

             case 'S':
                /* NO NEED TO IMPLEMENT THIS. */                
                break;

             case 'I':
                ins.num_instructions++;
                /* Psuedo LRU w/associativity algorithm */
                ins.num_hits += access_cache (u_cache, reference_type, memory_address);
                break;

             default:
                break;
        }
    }

    /* Print some statistics. */
    printf("Total number of references to the cache: %d. \n", ins.num_instructions);
    printf("Number Hits   : %d\n", ins.num_hits);
    printf("Number Misses : %d\n", ins.num_misses);
    printf("Hit  rate: %.2f \n", (float)(ins.num_hits)/(float)(ins.num_instructions));
    printf("Miss rate: %.2f \n", (float)(ins.num_misses)/(float)(ins.num_instructions));
}

/* Counter function so copy-pasting the result isn't needed in main */
float 
time_counter(struct timeval start, struct timeval stop)
{
	return (stop.tv_sec - start.tv_sec) +\
	(stop.tv_usec - start.tv_usec) / (float) 1000000;
}

int main (int argc, char **argv)
{
    FILE *input_fp;
    CACHE *u_cache = NULL;
    
    /* 
     * Zero out memory blocks for cache and instruction
     * structs 
     *
     * */
    memset(&cache_p, 0, sizeof(CACHE_PROPERTIES));
    memset(&ins, 0, sizeof(INSTRUCTIONS));
    
    /* Parse command line parameters. */
    USER_OPTIONS *user_options = parse_user_options (argc, argv);
    if (user_options == NULL){
        printf ("Error parsing command line arguments. \n");
        print_usage ();
        exit (0);
    }
    
    if (user_options->u_flag == 1){
        printf ("Creating unified cache; size: %dkB, associativity: %d, cache line: %d bytes \n", user_options->u_cache_size, 
                                                                                                user_options->u_cache_associativity, 
                                                                                                user_options->u_cache_line_size);

        u_cache = create_cache (user_options->u_cache_size, 
                                user_options->u_cache_associativity, 
                                user_options->u_cache_line_size, &cache_p);


        if (u_cache == NULL){
            printf ("Error creating cache; parameters must be powers of 2. \n");
            exit (0);
        }
        if (DEBUG_FLAG) {
        /* print_cache (u_cache) */;
        }
    }
    
    printf ("Simulating the cache using trace file: %s. \n", user_options->trace_file_name);
    input_fp = fopen (user_options->trace_file_name, "r");
    if(input_fp == NULL){
        printf ("Error opening trace file. Exiting. \n");
        exit (-1);
    }
    if (user_options->u_flag == 1) {
        simulate_unified_cache (u_cache, input_fp);
        free ((void *)u_cache);
    }
    
    /* 
     * Zero out memory blocks from above struct
     * to prevent potential memory holes
     * 
     * */
    memset(&cache_p, 0, sizeof(CACHE_PROPERTIES));
    memset(&ins, 0, sizeof(INSTRUCTIONS));

    fclose (input_fp);
    free ((void *)user_options);
    
    exit (0);
}

/* Default Help Message */
void print_usage (void)
{
    printf ("*****************USAGE************************ \n");
    printf ("To simulate a unified cache use the -U option: \n");
    printf ("./simulate_cache -U <cache size in Kilobytes> <set associativity> <cache line size in bytes> -f <trace file> \n");
    printf ("For example, ./simulate_cache -U 512 4 128 -f gcc_trace, simulates a 512 KB cache with \n");
    printf ("set accociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace \n \n");


    printf ("To simulate a system with separate I- and D-caches: (DO NOT USE this as this project did not support I or D type caches)\n");
    printf ("./simulate_cache -I <cache size> <set associativity> <cache line size> -D <cache size> <set associativity> <cache line size> -f <trace file> \n");
    printf ("For example, ./simulate_cache -I 128 1 64 -D 512 4 128 -f gcc_trace, simulates a 128 KB I cache with \n");
    printf ("set accociativity of 1 (direct mapped) and a cache line size of 64 bytes, and a 512 KB D cache with \n");
    printf ("set asscociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace \n");
}
