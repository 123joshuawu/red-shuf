#ifndef P_RAND_H
#define P_RAND_H

#include <stdint.h>
#include <limits.h>

// these are proxies for use in our code so that
//	it'll be easier to change our random implementation
//	if we ever want to

// they're inline, so theirs no lost efficiency

#define P_RAND_MAX RAND_MAX
#define RAND_T_MAX INT_MAX
#define rand_t unsigned int // guaranteed to be unsigned

// selects a seed and uses it to seed p_rand
inline void p_srand();

// returns a random number in the range (0, RAND_MAX)
inline rand_t p_rand();

#endif
