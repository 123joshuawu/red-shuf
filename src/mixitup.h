#ifndef MIXITUP_H
#define MIXITUP_H

#include<stdlib.h>

// permute tokens
// take elements from 'toks', reorder them, and put them in 'res'
inline void permute(char ** toks, size_t numtoks);

// make list of tokens
// select 'listlen' number of elements from 'toks' at random and insert them to 'list'
inline void mklist(char ** toks, size_t numtoks, char ** list, size_t listlen);

#endif
