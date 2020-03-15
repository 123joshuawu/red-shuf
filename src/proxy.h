#ifndef P_RAND_H
#define P_RAND_H

#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

// these are proxies for use in our code so that
//	it'll be easier to change our random implementation
//	if we ever want to

// they're inline, so theirs no lost efficiency

#define P_RAND_MAX ((rand_t) RAND_MAX)
#define RAND_T_MAX UINT_MAX
#define SUCCESS 0
#define FAILURE 1
typedef unsigned int rand_t; // guaranteed to be unsigned

// selects a seed and uses it to seed p_rand
extern inline void p_srand() {
	srand(time(NULL));
}

// returns a random number in the range (0, RAND_MAX)
extern inline rand_t p_rand() {
	return (rand_t) rand();
}

#endif
