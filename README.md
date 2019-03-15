# Cache Simulation Demonstration

Disclaimer: This following little project is compiled on a Linux/Unix operating system running a standard x86_64 processor with GCC 5.4 (or higher) and the make package.  Due to the nature of the project's status (demonstration purposes only), knowledge in the Linux operating system may be needed to ensure it runs and support beyond the above mentioned requirements is not guaranteed. 



This project serves as an example of some of my past work in the C programming language.  Demonstrated here is a cache simulator from a course in computer architecture.  The simulator itself uses a LRU (least recently used) algorithm to take a trace (found in the Traces folder) and develop the following parameters appropriately: cache size (in kilobytes), associativity (1 for direct mapping, takes other power of two numbers), and block(line) size in bytes.  For the algorithm found in simulate_cache.c, the following snippet from the original project's report (not included) summarizes how the function "access_cache" operates:



> The ﬁrst piece of the logic is to check to loop over the index and associativity to compare the tags; a hit increments the lru age and ﬁrst in, ﬁrst out (FIFO) counters appropriately, and returns a hit of 1. All other cases give a hit of 0; the next parameter is to check the indexes for a valid bit of 0 (indicating whether or not the slot is vacant); this allows for arbitrary placement of a ”fresh” entry in a not fully utilized cache (within indexing reason). Finally, the code checks for a hit with valid bits, replacing the appropriate entry. Normally, a LRU would replace the entry that has been ”least recently used”; this model does the same thing but with a minor variation. Instead of increasing the count value every time a ”hit” occurs, the lru age counter acts as a counter for how long it has been since an entry is accessed; the higher the bit, the longer it has been since that data was used. Since the method of using an ”access counter” was used, an auxiliary counter is also used to prevent duplicate lru age parameters: a ﬁrst in, ﬁrst out (FIFO). This FIFO value increments no matter if the entry is used or not, only being reset if the value is pulled from main memory in the event of a ”miss”.



The code is compiled using a Makefile so the only command needed is "make" once this project is pulled and opened up in a terminal.  After this, run the initial program with "./simulate_cache" and the following usage block will print:



> *****************USAGE************************
> To simulate a unified cache use the -U option:
> ./simulate_cache -U <cache size in Kilobytes> <set associativity> <cache line size in bytes> -f <trace file>
> For example, ./simulate_cache -U 512 4 128 -f gcc_trace, simulates a 512 KB cache with
> set accociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace
>
> To simulate a system with separate I- and D-caches:
> ./simulate_cache -I <cache size> <set associativity> <cache line size> -D <cache size> <set associativity> <cache line size> -f <trace file>
> For example, ./simulate_cache -I 128 1 64 -D 512 4 128 -f gcc_trace, simulates a 128 KB I cache with
> set accociativity of 1 (direct mapped) and a cache line size of 64 bytes, and a 512 KB D cache with
> set asscociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace



set the file target to "Traces/simple_trace" and adjust the parameters as desired, and a log report will print at the end with statistics of the cache's performance, as demonstrated below:



> vmuser@ubuntuv1:~/workspace/Github/final$ ./simulate_cache -U 128 4 64 -f Traces/simple_trace
> Creating unified cache; size: 128kB, associativity: 4, cache line: 64 bytes
> sets: 512 set bits: 9 block_bits: 6 shift_Amount 15
> Simulating the cache using trace file: Traces/simple_trace.
> Total number of references to the cache: 754.
> Number Hits   : 652
> Number Misses : 102
> Hit  rate: 0.86
> Miss rate: 0.14



To clean up the simulation, run "make clean". 