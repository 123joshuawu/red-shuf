#include <stdlib.h>
#include <time.h>
#include "p_rand.h"

// Seed using time in milliseconds since epoch. Subject to change.
inline void p_srand() {
	srand(time(NULL));
}

inline rand_t p_rand() {
	return (unsigned int) rand();
}
