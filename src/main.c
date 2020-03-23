#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "proxy.h"
#include "shufi.h"
#include "shufe.h"

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

int main(int argc, char** argv){
	int shufe = false;
	char *lohi = NULL;
	int count = -1;
	char *outfile = NULL;
	int opts = 0;
	int opt;
	while((opt = getopt(argc, argv, ":ei:n:o:rz")) > 0) {
		switch(opt) {
		case 'e':
			shufe = true;
			break;
		case 'i':
			lohi = optarg;
			break;
		case 'n':
			errno = 0;
			char *end;
			long l = strtol(optarg, &end, 10);
			if(errno == ERANGE || l < 0 || l > INT_MAX) {
				fprintf(stderr, "ERROR: -n argument out of range: %s\n", optarg);
				return EXIT_FAILURE;
			}
			if(*end != '\0') {
				fprintf(stderr, "ERROR: invalid number passed to -n: %s\n", optarg);
				return EXIT_FAILURE;
			}
			count = (size_t) l;
			opts |= OPT_N;
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
			fprintf(stderr, "ERROR: Missing argument for -%c\n", opt);
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
	if(shufe)
		status = shufe_main(count, out, opts, argc - optind, &argv[optind]);
	else if(lohi)
		status = shufi_main(lohi, count, out, opts);

	if(out != stdout)
		fclose(out);
	if(status == FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
