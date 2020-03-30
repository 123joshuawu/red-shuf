#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "strutil.h"

#define DEF_BUFSIZ 256

char *getline(FILE *in) {
	char *buf = malloc(DEF_BUFSIZ);
	if(!buf)
		return NULL;
	buf[DEF_BUFSIZ-1] = 1;
	if(!fgets(buf, DEF_BUFSIZ, in)) {
		free(buf);
		return NULL;
	}
	if(buf[DEF_BUFSIZ-1] != '\0')
		return buf;
	buf = realloc(buf, DEF_BUFSIZ << 1);
	if(!buf)
		return NULL;
	size_t nulidx = DEF_BUFSIZ - 1;
	size_t xsize = DEF_BUFSIZ + 1;
	size_t size = DEF_BUFSIZ << 1;
	size_t endidx = DEF_BUFSIZ << 1 - 1;
	while(buf = realloc(buf, size)) {
		buf[endidx] = 1;
		if(!fgets(buf + nulidx, xsize, in)) {
			if(ferror(in)) {
				free(buf);
				return NULL;
			}
			return buf;
		}
		if(buf[endidx] != '\0')
			return buf;
		if(size == SIZE_MAX) {
			errno = ENOMEM;
			free(buf);
			return NULL;
		}
		xsize = size + 1;
		nulidx = endidx;
		size <<= 1;
		endidx = size - 1;
	}
	return NULL;
}
