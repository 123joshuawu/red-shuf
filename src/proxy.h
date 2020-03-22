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

#define P_RAND_MAX ((rand_t) RAND_MAX) // maximum possible random value
#define RAND_T_MAX UINT_MAX // maximum value that can be held by rand_t
#define SUCCESS 0 // return vals indicating success or failure (for error checking)
#define FAILURE 1
#define OPT_R 0b01 // bit flags for -r and -z options
#define OPT_Z 0b10
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
