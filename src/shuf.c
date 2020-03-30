#include <stdlib.h>
#include <stdio.h>

#define START_BUFSIZE

int shuf_main(size_t count, FILE *out, int opts, FILE *in) {
	p_srand();
	char **elems = NULL;
	size_t bufsize = START_BUFSIZE;
	size_t idx = 0;
	while( elems = reallocarray(elems, sizeof(char *), bufsize) ) {
		for(; idx < bufsize; idx++) {
			
