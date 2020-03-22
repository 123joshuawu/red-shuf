#ifndef _P_RAND_H_
#define _P_RAND_H_

#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0 // return vals indicating success or failure (for error checking)
#define FAILURE 1
#define OPT_R 0b01 // bit flags for -r and -z options
#define OPT_Z 0b10

// these are proxies for use in our code so that
//	it'll be easier to change our random implementation
//	if we ever want to

// they're inline, so there's no lost efficiency

#define P_RAND_MAX ((rand_t) RAND_MAX) // maximum possible random value
#define RAND_T_MAX UINT_MAX // maximum value that can be held by rand_t
typedef unsigned int rand_t; // guaranteed to be unsigned

// selects a seed and uses it to seed p_rand
#define p_srand() \
	srand(time(NULL))

// returns a random number in the range (0, RAND_MAX)
#define p_rand() \
	(rand_t) rand()

#endif
