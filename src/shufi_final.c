#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef int INT; //proxy type

//Random function I used
INT PossibleRandom(INT Low, INT High){

        INT range = High - Low + 1; //assume always correct inputs
        INT x = (unsigned int)rand();
        INT y = x%range + Low;
        return y;
        //printf("[%d] %d ==> %d\n",range,x,y);
}


//shuf -i without -r/-rz
//definite output
//NULL if failed
INT* get_list(INT Low, INT High, INT* Size){

	INT Range = High - Low;
	if(Range < 0 || High < 0 || Low < 0){return NULL;} //Invalid Input: invalid range is given
	Range ++; //[Low,High]
	INT ListLength = Range; //output Length
        if(*Size < Range){ListLength = *Size;}

	INT* RangeList = calloc(Range,sizeof(INT));
	INT* ResultList = calloc(ListLength,sizeof(INT));
	if(RangeList == NULL || ResultList == NULL){return NULL;}//Memory allocation failed
	for(INT i=0; i<Range; i++){RangeList[i] = (Low+i);}

	for(INT i=0; i< ListLength; i++){
		
		//for(int j=0; j<Range; j++){printf("[%d] %d ",j,RangeList[j]);}
                //printf("\n");
		
		INT loci = PossibleRandom(0,Range-1); //get the number chose
                ResultList[i] = RangeList[loci]; //random assigned a number
                INT temp = RangeList[Range-1];
                
		RangeList[Range-1] = RangeList[loci];
                RangeList[loci] = temp;
                Range -- ; //range == 0 when finished
                if(Range < 0){break;} //protection
                
		//printf("[%d] %d %d \n",i,Range,loci);
                //for(int j=0; j<Range; j++){printf("[%d] %d ",j,RangeList[j]);}
                //printf("\n");
        }
	*Size = ListLength;

	free(RangeList);
	return ResultList;//Invalid input: Permute is not Y or N
}

//Return int* [Low,High] Success 
//Return NULL Fail
INT* getRange(char* LoHi){

	char* dummy = calloc(strlen(LoHi)+1,sizeof(char));
	int* LowHigh = calloc(2,sizeof(int));
	if(dummy == NULL || LowHigh == NULL){
		perror("ERROR: getRange() Unable to dynamically allocate memory\n");
		return NULL;
	} //failed

	strcpy(dummy,LoHi);
	char* ntr = strtok(dummy,"-");
	int count = 0;
	//for strtol(), atoi() is less safe
	long number;
	char* ptr;
	while(ntr != NULL && count < 2){

		const char* nntr = ntr;
		number = strtol(nntr,&ptr,10); //
		if(number == 0 && strcmp(ntr,"0") != 0){

			perror("ERROR:getRange() Invalid input\n");
			free(LowHigh);
			free(dummy);
			return NULL;
		}
		LowHigh[count] = (INT)number;
	        
		count++;
		ntr = strtok(NULL,"-");	
	}

	if(count < 2){
		 perror("ERROR:getRange() Invalid input, Possible lack of input\n");
                 free(LowHigh);
                 free(dummy);
                 return NULL;
	}
	free(dummy);
	return LowHigh;

}

 
int main(int argc, char** argv){

	//INT* test = get_list(1,10,10);
	//if(test != NULL){
	//	for(int i=0; i<10; i++){printf("[%d] %d\n",i,test[i]);}
	//	free(test);
	//}

	int opt;
	INT* LowHigh; //[Low, High]
	char* LoHi = NULL; //-i 
	INT Count = -1;  //-n
	char* OutputFile = NULL; //-o
	char Premute = 'N'; //-r
	char* EndLine = "\n"; //-z

        while((opt = getopt(argc,argv,"i:n:o:rz")) != -1){

		//printf("%c\n",opt);

		switch(opt){

			case 'i' :
				LoHi = optarg;
				break;

			case 'n' :
			       Count = atoi(optarg);
			       break;
			
			case 'o' :
			        OutputFile = optarg;
				break;
		 	
			case 'r' : 
				Premute = 'Y';
				break;
			
			case 'z' :
				EndLine = "\0";
				break;		
			case '?' :
				perror("ERROR: Invalid input given: ");
				return EXIT_FAILURE;
		}
	}
	
	if(LoHi == NULL){

		perror("ERROR: main() Uanble to get range\n");
		return EXIT_FAILURE;
	}

	if(OutputFile != NULL){
		
		if(strcmp(OutputFile,"-rz") == 0 ||strcmp(OutputFile,"-r") == 0 || strcmp(OutputFile,"-z") == 0){
			perror("ERROR: main() Not output file given\n");
               		return EXIT_FAILURE;
		}
	}

	LowHigh = getRange(LoHi); //NEED FREE
	if(LowHigh == NULL){
		perror("ERROR: Unable to get number, possible input error\n");
		return EXIT_SUCCESS;
	}
	//printf("%d %d\n",LowHigh[0],LowHigh[1]);
        INT check = LowHigh[1] - LowHigh[0];
	if(check < 0){

		perror("ERROR: Invalid range, possible input error\n");
		free(LowHigh);
                return EXIT_SUCCESS;
	}

	if(Premute == 'N'){

		if(Count == -1){Count = check+1;}
		int* result = get_list(LowHigh[0],LowHigh[1],&Count);
		//printf("[%d]\n",Count);
		for(INT i=0; i<Count; i++){
			printf("%d",result[i]);
			printf("%s",EndLine);
		}
		free(result);

	}
	else{
		INT i = 0;
		while(1){

			if(i > Count){break;}
			INT R = PossibleRandom(LowHigh[0],LowHigh[1]);
			printf("%d",R);
			printf("%s",EndLine);
			if(Count != -1){i++;}
		}
	}	

	if(OutputFile != NULL){printf("%s\n",OutputFile);}
	free(LowHigh);
	return EXIT_SUCCESS;
}
