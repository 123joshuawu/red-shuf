#include "mixitup.h"
#include "p_rand.h"

// 'numtoks' must be less than or equal to P_RAND_MAX!!!
inline void permute(char ** toks, size_t numtoks) {
	p_srand();
	for(size_t i = 0; i < numtoks; i++) {
		size_t j = p_rand() % numtoks;
		char * temp = toks[i];
		toks[i] = toks[j];
		toks[j] = temp;
	}
}
		


// 'numtoks' must be less than or equal to P_RAND_MAX!!!
inline void mklist(char ** toks, size_t numtoks, char * list, size_t listlen) {
	p_srand();
	for(size_t i = 0; i < listlen; i++)
		list[i] = toks[p_rand() % numtoks];
}
