#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include "vector.h"
#include "proxy.h"

#define START_VECSIZE 128
#define START_BUFSIZE 128

static inline int mkbuf(char **line) {
	*line = malloc(START_BUFSIZE);
	if(!*line) {
		fprintf(stderr, "ERROR: Out of memory.\n");
		return FAILURE;
	}
}

static inline int getlines(int opt_z, FILE *in, Vector *vec) {
	char *line;
	errno = 0;
	if(mkbuf(&line) == FAILURE)
		return FAILURE;
	size_t start_bufsize = START_BUFSIZE;
	ssize_t len;
	while((len = getline(&line, &start_bufsize, in)) >= 0) {
		if(opt_z) {
			if(line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
		vec_add_charp(vec, line);
		if(errno) {
			fprintf(stderr, "ERROR: Out of memory.\n");
			return FAILURE;
		}
		if(mkbuf(&line) == FAILURE)
			return FAILURE;
	}
	if(errno) {
		fprintf(stderr, "ERROR: Failed to read input.\n");
		return FAILURE;
	}
	return SUCCESS;
}

static inline int repwnul(FILE *out, char **lines, size_t numlines) {
	return fprintf(out, "%s%c", lines[p_rand() % numlines], '\0') < 0? FAILURE: SUCCESS;
}

static inline int repwlf(FILE *out, char **lines, size_t numlines) {
	return fputs(lines[p_rand() % numlines], out) == EOF? FAILURE: SUCCESS;
}

static inline int replace(int opt_n, int opt_z, int count, FILE *out, char **lines, size_t numlines) {
	if(opt_n) {
		if(opt_z) {
			for(size_t i = 0; i < count; i++)
				if(repwnul(out, lines, numlines) == FAILURE)
					return FAILURE;
			return SUCCESS;
		}
		for(size_t i = 0; i < count; i++)
			if(repwlf(out, lines, numlines) == FAILURE)
				return FAILURE;
		return SUCCESS;
	}
	if(opt_z) {
		for(;;)
			if(repwnul(out, lines, numlines) == FAILURE)
				return FAILURE;
		return SUCCESS;
	}
	for(;;)
		if(repwlf(out, lines, numlines) == FAILURE)
			return FAILURE;
	return SUCCESS;
}

static inline int permute(int opt_n, int opt_z, size_t count, FILE *out, char **lines, size_t numlines) {
	if(!opt_n || count > numlines)
		count = numlines;
	size_t randidx;
	if(opt_z) {
		for(size_t i = count; i > 0; i--) {
			randidx = p_rand() % i;
			if(fprintf(out, "%s%c", lines[randidx], '\0') < 0)
				return FAILURE;
			lines[randidx] = lines[i - 1];
		}
		return SUCCESS;
	}
	for(size_t i = count; i > 0; i--) {
		randidx = p_rand() % i;
		if(fputs(lines[randidx], out) == EOF)
			return FAILURE;
		lines[randidx] = lines[i - 1];
	}
	return SUCCESS;
}

int shuf_main(size_t count, FILE *out, int opts, FILE *in) {
	setvbuf(in, NULL, _IONBF, 0);
	int opt_z = opts & OPT_Z;
	Vector *vec = vec_new(START_VECSIZE, sizeof(char *));
	if(getlines(opt_z, in, vec) == FAILURE) {
		vec_del_r(vec);
		return FAILURE;
	}
	p_srand();
	size_t numlines = vec->size;
	char **lines = vec->buf;
	if(numlines > P_RAND_MAX) {
		fprintf(stderr, "ERROR: Too many lines.\n");
		return FAILURE;
	}
	int stat;
	if(opts & OPT_R)
		stat = replace(opts & OPT_N, opt_z, count, out, lines, numlines);
	else
		stat = permute(opts & OPT_N, opt_z, count, out, lines, numlines);
	vec_del_r(vec);
	if(stat == FAILURE) {
		fprintf(stderr, "ERROR: Failed to write to file.\n");
		return FAILURE;
	}
	return SUCCESS;
}
