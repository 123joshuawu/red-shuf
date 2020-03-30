#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "vector.h"

Vector *vec_new(size_t cnt, size_t elem_size) {
	if( cnt > SIZE_MAX / elem_size) {
		errno = ENOMEM;
		return NULL;
	}
	Vector *vec = malloc(sizeof(Vector));
	if(!vec) {
		errno = ENOMEM;
		return NULL;
	}
	vec->size = 0;
	vec->cap = cnt * elem_size;
	vec->buf = malloc(vec->cap);
	if(!vec->buf) {
		free(vec);
		errno = ENOMEM;
		return NULL;
	}
	return vec;
}

void vec_add_charp(Vector *vec, char *elem) {
	if(vec->size == vec->cap) {
		vec->cap <<= 1;
		if((signed int) vec->cap < 0) {
			errno = ENOMEM;
			return;
		}
		vec->buf = realloc(vec->buf, vec->cap);
		if(!vec->buf) {
			errno = ENOMEM;
			return;
		}
	}
	((char **)vec->buf)[vec->size] = elem;
	vec->size++;
}

void vec_del(Vector *vec) {
	free(vec->buf);
	free(vec);
}

void vec_del_r(Vector *vec) {
	size_t size = vec->size;
	void **buf = vec->buf;
	for(size_t i = 0; i < size; i++)
		free(buf[i]);
	vec_del(vec);
}
