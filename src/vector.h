#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdlib.h>

#ifndef ENOMEM
#define ENOMEM 1
#endif

typedef struct vector {
	size_t cap;
	size_t size;
	void *buf;
} Vector;

Vector *vec_new(size_t cnt, size_t elem_size);

void vec_add_charp(Vector *vec, char **elem);

void vec_del(Vector *vec);

#endif
