#include <stdlib.h>
#include "p_rand.h"

inline void p_srand(unsigned int seed) {
	srand(seed);
}

inline rand_t p_rand() {
	return rand();
}
