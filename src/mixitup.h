#ifndef MIXITUP_H
#define MIXITUP_H

#include<stdlib.h>
#include "proxy.h"

// permute tokens
// swap elements in 'toks' from index 0 to 'reqlen'
// 'numtoks' must be less than or equal to P_RAND_MAX!!!
#define permute(type, toks, numtoks, reqlen) \
	p_srand(); \
	for(size_t _i = 0; _i < numtoks; _i++) { \
		size_t _j = p_rand() % numtoks; \
		type _temp = toks[_i]; \
		toks[_i] = toks[_j]; \
		toks[_j] = _temp; \
	} //

/*
inline void permute(char ** toks, size_t numtoks, size_t reqlen) {
	p_srand();
	for(size_t i = 0; i < numtoks; i++) {
		size_t j = p_rand() % numtoks;
		char * temp = toks[i];
		toks[i] = toks[j];
		toks[j] = temp;
	}
}

inline void permutei(shufi_t * toks, size_t numtoks, size_t reqlen) {
	p_srand();
	for(size_t i = 0; i < numtoks; i++) {
		size_t j = p_rand() % numtoks;
*/
		
// make list of tokens
// select 'listlen' number of elements from 'toks' at random and insert them to 'list'
// 'numtoks' must be less than or equal to P_RAND_MAX!!!
inline void mklist(char ** toks, size_t numtoks, char ** list, size_t listlen) {
	p_srand();
	for(size_t i = 0; i < listlen; i++)
		list[i] = toks[p_rand() % numtoks];
}

#endif
