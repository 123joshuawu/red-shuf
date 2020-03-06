#include<stdlib.h>

// permute tokens
// take elements from 'toks', reorder them, and put them in 'res'
inline void permute(const char * const toks, size_t numtoks, char * const res);

// make list of tokens
// select 'listlen' number of elements from 'toks' at random and insert them to 'list'
inline void mklist(const char * const toks, size_t numtoks, char * const list, size_t listlen);
