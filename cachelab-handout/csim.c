#include "cachelab.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void printHelpInfo();
void printErrorInfo();

typedef struct cachelineInfo {
    bool valid;
    int accessTime;
    unsigned long markBit;
} CacheLine;

//recommend to use the getopt function to parse your command line arguments!!!!
int main(int argc, char *argv[])
{
    /* step1:get command line arguments and parse them */
    /*
     * arguments form 1: -h
     * arguments form 2: -v -s 4 -E 1 -b 4 -t traces/yi.trace
     * arguments form 3: -s 4 -E 1 -b 4 -t traces/yi/trace
     * there are only 3 legal arguments forms, other form is not legal
     *
     * range: s>=0,E>=1,b>=0!!!!	
     */
    int s,E,b;
    bool needPrint = false;
    char *tracefilePath;
    int shift = 0;
    //deal with the command line arguments in s simple way, but not rigorous
    switch (argc) {
    	case 2:	printHelpInfo();
		return 0;
	case 9: break;
	case 10: shift = 1;
		 needPrint = true;
		 break;
	default: printErrorInfo();
		 exit(EXIT_FAILURE); 
    }
    s = atoi(argv[2 + shift]);
    E = atoi(argv[4 + shift]);
    b = atoi(argv[6 + shift]);
    tracefilePath = argv[8 + shift]; 
    s = (int)exp2(s);
    b = (int)exp2(b); 

    /* step2:construct my data strcture of cache and initialize it */
    //calloc could initialize memory automatically
    int numOfCacheline = s * E;
    CacheLine *cache = (CacheLine*)calloc(numOfCacheline, sizeof(CacheLine)); 
    if (cache == NULL) {
        perror("allocate memory for cache fails");	
	exit(EXIT_FAILURE);
    }

    /* step3:read instructions from trace file one by one and deal with it */
    //open file
    FILE *tracefile = fopen(tracefilePath, "r");
    if (tracefile == NULL) {
	perror("error opening file");
	exit(EXIT_FAILURE);
    }
    //read instructions one by one
    char instruction[30];
    int instruction_count, hit_count , miss_count , eviction_count;
    instruction_count = hit_count = miss_count = eviction_count = 0;
    //instruction will include '\n' !!!!!
    //you could use fscanf() to replace fgets() and sscanf()!!!!
    while (fgets(instruction, 30, tracefile)) {
	//ignore if instruction start with 'I'
        if (instruction[0] == 'I') {
	    continue;
	}		
	//parse the instruction
        char operator;
        unsigned long address;
	int size;
	sscanf(instruction, " %c %lx,%d", &operator, &address, &size);
		
        //simulate behavior of cache on this instruction, and print out the outcome
	if (needPrint) {
	    printf("%c %lx,%d", operator, address, size);
	}
	instruction_count++;
        unsigned int indexGroup = address / b % s;
    	unsigned int start = indexGroup * E;
	CacheLine *currentline = cache + start;
    	CacheLine *emptyline = NULL;
    	CacheLine *evictionline = cache + start;
    	unsigned long markBitRef = address / b / s;	
	//check if hit the cache
	for (int i = 0; i < E; i++) {
	    if (currentline->valid && currentline->markBit == markBitRef) {
		//already hit
		currentline->accessTime = instruction_count;
		hit_count++;
		if (needPrint) {
		    printf(" hit");
		}
		goto isM;
	    } 
	    //check if there is a empty cache line
	    if (!currentline->valid) {
		emptyline = currentline; 
		break;
	    }
	    //currrentline must be valid here, update evictionline
	    if (currentline->accessTime < evictionline->accessTime) {
		evictionline = currentline;	
	    }
	    currentline++;
	}
	//print miss info
	miss_count++;
	if (needPrint) {
	    printf(" miss");
	}
	//if there is a empty cache line
	if (emptyline) {
	    emptyline->valid = true;
	    emptyline->accessTime = instruction_count;
	    emptyline->markBit = markBitRef;
	    goto isM; 
	}
	//eviction
	eviction_count++;
	evictionline->markBit = markBitRef;
	evictionline->accessTime = instruction_count;
	if (needPrint) {
	    printf(" eviction");
	}
	//check if the instruction is 'M'
    isM:
	if (operator == 'M') {
	    hit_count++;
	    if (needPrint) {
	        printf(" hit");
	    }
	}
	if (needPrint) {
	    printf("\n");
	}
    }
	
    /* step4:free resource , close file, print summary info */
    fclose(tracefile);
    free(cache);
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}

void printHelpInfo() {
    printf(""
	"Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n"
	"Options:\n"
	"  -h         Print this help message.\n"
	"  -v         Optional verbose flag.\n"
	"  -s <num>   Number of set index bits.\n"
	"  -E <num>   Number of lines per set.\n"
	"  -b <num>   Number of block offset bits.\n"
	"  -t <file>  Trace file.\n"
	"\n"
	"Examples:\n"
	"  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
	"  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n"
	"");
}

void printErrorInfo() {
    printf("./csim: Missing required command line argument\n");
    printHelpInfo();
}
