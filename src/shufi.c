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
#define SHUFI_MIN INT_MIN
#define PRI_SHUFI "d"

//shuf -i without -r/-rz
//definite output
//NULL if failed
shufi_t* get_list(shufi_t Low, shufi_t High, size_t * Size){

	shufi_t Range = High - Low;
	if(Range < 0 || High < 0 || Low < 0){return NULL;} //Invalid Input: invalid range is given
	Range ++; //[Low,High]

	if(Range < *Size)
		*Size = Range;

	// check for overflow
	if(Range > SIZE_MAX / sizeof(shufi_t))
		return NULL;

	errno = 0;
	shufi_t* RangeList = (shufi_t*) malloc(Range * sizeof(shufi_t));

	// check for error
	// don't check if NULL. Could still be valid.
	if(errno == ENOMEM)
		return NULL;
	
	for(shufi_t i=0; i<Range; i++){RangeList[i] = (Low+i);}

	permute(shufi_t, RangeList, Range, *Size);
	return RangeList;
}

static inline void erange() {
	fprintf(stderr, "ERROR: Invalid range. Usage: shuf -i LOW-HIGH\n");
}

static inline int getshufi(char *nptr, char **endptr, shufi_t * num) {
	errno = 0;
	long long ll = strtoll(nptr, endptr, 10);
	if(errno == ERANGE || ll < SHUFI_MIN || ll > SHUFI_MAX) {
		fprintf(stderr, "ERROR: -i value out of range [%" PRI_SHUFI ",%" PRI_SHUFI "]\n",
				SHUFI_MIN, SHUFI_MAX);
		return FAILURE;
	}
	if(*endptr == nptr) {
		erange();
		return FAILURE;
	}
	*num = (shufi_t) ll;
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
	long long ll = strtoll(optarg, &ptr, 10);
	if(errno == ERANGE || ll < 0) {
		fprintf(stderr, "ERROR: -n value out of range [0,%lld]\n", LLONG_MAX);
		return FAILURE;
	}
	if(ptr == optarg || *ptr != '\0') {
		fprintf(stderr, "ERROR: invalid number '%s'\n", optarg);
		return FAILURE;
	}
	*cnt = (size_t) ll;
	return SUCCESS;
}
 
int main(int argc, char** argv){

	int opt;
	char* LoHi = NULL; //-i 
	size_t Count;  //-n
	int iscount = 0;
	char* OutputFile = NULL; //-o
	char Permute = 'Y'; //-r
	char* EndLine = "\n"; //-z

        while((opt = getopt(argc,argv,"i:n:o:rz")) != -1){

		switch(opt){

			case 'i' :
				LoHi = optarg;
				break;

			case 'n' :
			       if(getcount(&Count) == FAILURE)
				 	return EXIT_FAILURE;
				 iscount = 1;
			       break;
			
			case 'o' :
			        OutputFile = optarg;
				break;
		 	
			case 'r' : 
				Permute = 'N';
				break;
			
			case 'z' :
				EndLine = "\0";
				break;		
			case '?' :
				fprintf(stderr,"ERROR: Invalid input given: ");
				return EXIT_FAILURE;
		}
	}
	
	if(LoHi == NULL){

		fprintf(stderr,"ERROR: main() Unable to get range\n");
		return EXIT_FAILURE;
	}

	if(OutputFile != NULL){
		
		if(strcmp(OutputFile,"-rz") == 0 ||strcmp(OutputFile,"-r") == 0 || strcmp(OutputFile,"-z") == 0){
			fprintf(stderr,"ERROR: main() No output file given\n");
               		return EXIT_FAILURE;
		}
	}
	
	shufi_t low, high;
	if(getRange(LoHi, &low, &high) == FAILURE)
		return EXIT_FAILURE;
	
      shufi_t check = high - low;
	// dont have to check high < 0
	if(low < 0 || check < 0){
		erange();
            return EXIT_FAILURE;
	}

	if(Permute == 'Y'){

		if(!iscount){Count = check+1;}
		shufi_t * result = get_list(low,high,&Count);
		for(size_t i=0; i<Count; i++){
			printf("%d",result[i]);
			printf("%s",EndLine);
		}
		free(result);

	}
	else{
		shufi_t range = check + 1;
		p_srand();
		if(!iscount) {
			for(;;) {
				shufi_t R = low + p_rand() % range;
				printf("%d",R);
				printf("%s",EndLine);
			}
		}
		for(size_t i = 0; i < Count; i++){
			shufi_t R = low + p_rand() % range;
			printf("%d",R);
			printf("%s",EndLine);
		}
	}	

	if(OutputFile != NULL){printf("%s\n",OutputFile);}
	return EXIT_SUCCESS;
}
