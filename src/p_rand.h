#ifndef P_RAND_H
#define P_RAND_H

#include<stdint.h>
#include<limits.h>

// these are proxies for use in our code so that
//	it'll be easier to change our random implementation
//	if we ever want to

// they're inline, so theirs no lost efficiency

#define RAND_T_MAX INTMAX
#define rand_t int

inline void p_srand(unsigned int seed);

inline rand_t p_rand();

#endif
