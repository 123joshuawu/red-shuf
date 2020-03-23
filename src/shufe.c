#include <stdlib.h>
#include <stdio.h>
#include "proxy.h"

int shufe_main(size_t count, FILE *out, int opts, size_t elemc, char **elemv) {
	if(elemc > P_RAND_MAX) {
		fprintf(stderr, "ERROR: Too many elements. Limit = %" PRI_RAND ".\n", P_RAND_MAX);
		return FAILURE;
	}
	p_srand();
	char term = opts & OPT_Z? '\0': '\n';
	if(opts & OPT_R) {
		if(!(opts & OPT_N)) {
			for(;;) {
				fprintf(out, "%s%c", elemv[p_rand() % elemc], term);
				if(ferror(out)) {
					fprintf(stderr, "ERROR: Failed to write to file.");
					return FAILURE;
				}
			}
		}
		for(size_t i = 0; i < count; i++) {
			fprintf(out, "%s%c", elemv[p_rand() % elemc], term);
			if(ferror(out)) {
				fprintf(stderr, "ERROR: Failed to write to file.");
				return FAILURE;
			}
		}
		return SUCCESS;
	}
	char *temp;
	size_t randind;
	if(!(opts & OPT_N) || count > elemc)
		count = elemc;
	for(size_t i = 0; i < count; i++) {
		randind = p_rand() % elemc;
		fprintf(out, "%s%c", elemv[randind], term);
		if(ferror(out)) {
			fprintf(stderr, "ERROR: failed to write to file.");
			return FAILURE;
		}
		elemv[randind] = elemv[elemc - 1];
		elemc--;
	}
	return SUCCESS;
}
