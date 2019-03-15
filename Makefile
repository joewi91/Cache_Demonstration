# Makefile for cache simulation

# Using GCC c99 standard and flag all warnings 
CCFLAGS = -std=c99 -Wall

# Commands to compile and link the source files
simulate_cache: simulate_cache.o parse_user_options.o create_cache.o
	gcc -o simulate_cache $(CCFLAGS) simulate_cache.o parse_user_options.o create_cache.o -lm
%.o: %.c parse_user_options.h cache.h
	gcc -c $(CCFLAGS) $<

# Clean command...no phony targeting present
clean:
	rm -f *.o  *~ simulate_cache
