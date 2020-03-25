#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "proxy.h"
#include "shufi.h"
#include "shufe.h"

static void
usage(const char *name)
{
	fprintf(stderr, "usage: %s [-hv] [-n count] [-o file] [-rz] [file]\n"
		"       %s [-hv] -e [-n count] [-o file] [-rz] [args ...]\n"
		"       %s [-hv] -i lo-hi [-n count] [-o file] [-rz]\n",
		name, name, name);

	exit(1);
}

static void
print_version(void)
{

	fputs("shuf 2.9\n"
	      "Copyright (c) 2017-2019 Brian Callahan <bcallah@openbsd.org>\n"
	      "\nPermission to use, copy, modify, and distribute this software"
	      " for any\npurpose with or without fee is hereby granted, "
	      "provided that the above\ncopyright notice and this permission "
	      "notice appear in all copies.\n\nTHE SOFTWARE IS PROVIDED \"AS "
	      "IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\nWITH REGARD TO "
	      "THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n", stderr);
	fputs("MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE "
	      "LIABLE FOR\nANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL "
	      "DAMAGES OR ANY DAMAGES\nWHATSOEVER RESULTING FROM LOSS OF USE, "
	      "DATA OR PROFITS, WHETHER IN AN\nACTION OF CONTRACT, NEGLIGENCE "
	      "OR OTHER TORTIOUS ACTION, ARISING OUT OF\nOR IN CONNECTION "
	      "WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n", stderr);

	exit(1);
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

int main(int argc, char** argv){
	int shufe = false;
	char *lohi = NULL;
	int count = -1;
	char *outfile = NULL;
	int opts = 0;
	int opt;
	const char *program_name = argv[0];
	while((opt = getopt(argc, argv, ":ehi:n:o:rvz")) > 0) {
		switch(opt) {
		case 'e':
			shufe = true;
			break;
		case 'h':
			usage(program_name);
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
		case 'v':
			print_version();
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
