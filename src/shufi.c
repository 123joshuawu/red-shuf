#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "proxy.h"
#include "mixitup.h"

typedef int shufi_t;
#define SHUFI_MAX INT_MAX
#define SHUFI_MIN 0
#define PRI_SHUFI "d"

//shuf -i without -r/-rz
//definite output
//NULL if failed
shufi_t* get_list(shufi_t Low, shufi_t High, size_t * Size){

	shufi_t Range = High - Low;
	// check if invalid range
	if(Range < 0) {
		fprintf(stderr, "ERROR: Invalid range. LOW greater than HIGH.\n");
		return NULL;
	}
	if(Low < 0) {
		fprintf(stderr, "ERROR: Invalid range. LOW is negative.\n");
		return NULL;
	}
	Range ++; //[Low,High]

	if(Range < *Size)
		*Size = Range;

	// check for overflow
	if(Range > SIZE_MAX / sizeof(shufi_t)) {
		fprintf(stderr, "ERROR: Range to big\n");
		return NULL;
	}

	errno = 0;
	shufi_t* RangeList = (shufi_t*) malloc(Range * sizeof(shufi_t));

	// check for error
	// don't check if NULL. Could still be valid.
	if(errno == ENOMEM) {
		fprintf(stderr, "ERROR: Failed to alocate memory\n");
		return NULL;
	}
	
	for(shufi_t i=0; i<Range; i++){RangeList[i] = (Low+i);}

	permute(shufi_t, RangeList, Range, *Size);
	return RangeList;
}

static inline void erange() {
	fprintf(stderr, "ERROR: Invalid range. Usage: shuf -i LO-HI\n");
}

static inline int getshufi(char *nptr, char **endptr, shufi_t * num) {
	errno = 0;
	long l = strtol(nptr, endptr, 10);
	if(errno == ERANGE || l < SHUFI_MIN || l > SHUFI_MAX) {
		size_t i = 1;
		while(nptr[i] != '\0') {
			if(nptr[i] == '-') {
				nptr[i] = '\0';
				break;
			}
			i++;
		}
		fprintf(stderr, "ERROR: -i value out of range [%" PRI_SHUFI ",%" PRI_SHUFI "]: %s\n",
				SHUFI_MIN, SHUFI_MAX, nptr);
		return FAILURE;
	}
	if(*endptr == nptr) {
		erange();
		return FAILURE;
	}
	*num = (shufi_t) l;
	return SUCCESS;
}

int getRange(char* LoHi, shufi_t * low, shufi_t * hi){
	char * ptr;
	if(getshufi(LoHi, &ptr, low) == FAILURE)
		return FAILURE;
	if(*ptr != '-') {
		erange();
		return FAILURE;
	}
	LoHi = ptr + 1;
	if(getshufi(LoHi, &ptr, hi) == FAILURE)
		return FAILURE;
	if(*ptr != '\0') {
		erange();
		return FAILURE;
	}
}

static inline int getcount(size_t * cnt) {
	char * ptr;
	errno = 0;
	long l = strtol(optarg, &ptr, 10);
	if(errno == ERANGE || l < 0 || l > SIZE_MAX) {
		fprintf(stderr, "ERROR: -n value out of range [0,%ld]\n", SIZE_MAX);
		return FAILURE;
	}
	if(ptr == optarg || *ptr != '\0') {
		fprintf(stderr, "ERROR: invalid number '%s'\n", optarg);
		return FAILURE;
	}
	*cnt = (size_t) l;
	return SUCCESS;
}

static inline int print_rand(const shufi_t range, const shufi_t low, const char term, FILE *out) {
	fprintf(out, "%d%c", p_rand() % range + low, term);
	if(ferror(out)) {
		fprintf(stderr, "ERROR: Failed to write to file\n");
		return FAILURE;
	}
}

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
int shufi_main(char *lohi, size_t count, FILE *outfile, int opts) {
	shufi_t low, high;
	if(getRange(lohi, &low, &high) == FAILURE)
		return FAILURE;
	char term = opts & OPT_Z ? '\0': '\n';
	shufi_t range = high - low;
	// check all elements reachable by p_rand
	if(range > P_RAND_MAX) {
		fprintf(stderr, "ERROR: Range too big.\n");
		return FAILURE;
	}
	// replace
	if(opts & OPT_R) {
		p_srand();
		// unlimited
		if(count < 0)
			for(;;) // infinite loop...
				if(print_rand(range, low, term, outfile) == FAILURE)
					return FAILURE;
		// limited
		for(size_t i = 0; i < count; i++)
			if(print_rand(range, low, term, outfile) == FAILURE)
				return FAILURE;
	}
	// permute
	else {
		// nums is dynamically alocated
		shufi_t *nums = get_list(low, high, &count);
		if(!nums)
			return FAILURE;
		for(size_t i = 0; i < count; i++) {
			fprintf(outfile,"%d%c",nums[i],term);
			if(ferror(outfile)) {
				fprintf(stderr, "ERROR: failed to write to file\n");
				free(nums);
				return FAILURE;
			}
		}
		// free nums!
		free(nums);
	}
	return SUCCESS;
}
			
 
int main(int argc, char** argv){
	char *lohi = NULL;
	size_t count = -1;
	char *outfile = NULL;
	int opts = 0;
	int opt;
	while((opt = getopt(argc, argv, ":i:n:o:rz")) > 0) {
		switch(opt) {
		case 'i':
			lohi = optarg;
			break;
		case 'n':
			errno = 0;
			char *end;
			long l = strtol(optarg, &end, 10);
			if(errno == ERANGE || l < 0 || l > SIZE_MAX) {
				fprintf(stderr, "ERROR: -n argument out of range: %s\n", optarg);
				return EXIT_FAILURE;
			}
			if(*end != '\0') {
				fprintf(stderr, "ERROR: invalid number passed to -n: %s\n", optarg);
				return EXIT_FAILURE;
			}
			count = (size_t) l;
			break;
		case 'o':
			outfile = optarg;
			break;
		case 'r':
			opts |= OPT_R;
			break;
		case 'z':
			opts |= OPT_Z;
			break;
		case ':': // missing argument
			fprintf(stderr, "ERROR: Missing argument for -%c\n", optopt);
			return EXIT_FAILURE;
		default: // unknown option
			fprintf(stderr, "ERROR: Unknown option -%c\n", optopt);
			return EXIT_FAILURE;
		}
	}
	FILE *out;
	if(outfile) {
		out = fopen(outfile, "w");
		if(!out) {
			fprintf(stderr, "ERROR: failed to open file: %s", outfile);
			return EXIT_FAILURE;
		}
	}
	else
		out = stdout;
	int status;
	if(lohi)
		status = shufi_main(lohi, count, out, opts);
	if(out != stdout)
		fclose(out);
	if(status == FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
