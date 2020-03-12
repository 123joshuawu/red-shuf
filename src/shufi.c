#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int PossibleRandom(int Low, int High){

	int range = High - Low + 1; //assume always correct inputs
	int x = (unsigned int)rand();
        int y = x%range + Low;
	return y;
        //printf("[%d] %d ==> %d\n",range,x,y);
}

int* get_line(int Low, int High, char Replace, int size){

	int range = High - Low; //[low,high]
	if(range < 0 || size <= 0){return NULL;} //invalid input
	range ++; // [low, high]
	int ListLength = range;
        if(size < range || Replace == 'y'){ListLength = size;}
	//printf("%d\n",ListLength);
	int* RangeList = calloc(range,sizeof(int));
        int* ResultList = calloc(ListLength,sizeof(int));
        if(RangeList == NULL || ResultList == NULL){return NULL;} //failed to dynamically allocate memory
	int loci = 0;
	for(int i= Low; i<= High; i++){
		RangeList[loci] = i;
		loci ++;
	}

	if(Replace == 'n'){
		
		for(int i=0; i< ListLength; i++){

			//for(int j=0; j<range; j++){printf("[%d] %d ",j,RangeList[j]);}
                        //printf("\n");

			int loci = PossibleRandom(0,range-1); //get the number chose
			ResultList[i] = RangeList[loci]; //random assigned a number
		        int temp = RangeList[range-1]; 
			RangeList[range-1] = RangeList[loci];
		        RangeList[loci] = temp;	
			range -- ; //range == 0 when finished
			if(range < 0){break;} //protection
			//printf("[%d] %d %d \n",i,range,loci);
			//for(int j=0; j<range; j++){printf("[%d] %d ",j,RangeList[j]);}
			//printf("\n");
		}
		free(RangeList);
		return ResultList;

	}
	else if(Replace == 'y'){

		for(int i=0; i<ListLength; i++){
			ResultList[i] = PossibleRandom(Low,High);
		}

		free(RangeList);
		return ResultList;
	}
	free(RangeList);
	free(ResultList);
	return NULL;
	
}


int main(int argc, char** argv){

	if(argc >= 4){
		
		int Len = atoi(argv[3]);
		int* r = get_line(atoi(argv[1]),atoi(argv[2]),'n',Len);
		if(r != NULL){
			for(int i=0; i<Len; i++){
				printf("%d\n",r[i]);
			}
		free(r);
		}
	}
	return EXIT_SUCCESS;
}

