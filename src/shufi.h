#ifndef _SHUFI_H_
#define _SHUFI_H_
#include <stdlib.h>
#include <stdio.h>

/*
 * Called by main if -i option given
 * --------------------------
 * Params:
 * 	lohi - argument for -i option
 *	count - argument for -n option converted to size_t. -1 if not specified
 *	outfile - argument for -o option opend as a file pointer, or stdout if not used
 *	opts - bitflags. result of bitwise-or-ing OPT_R and OPT_Z if -r and -z were specified, respectively
 * Returns:
 *	FAILURE upon failure, else SUCCESS
 */
int shufi_main(char *lohi, size_t count, FILE *outfile, int opts);

#endif
