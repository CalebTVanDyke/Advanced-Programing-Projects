#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "util.h"
#include "AIFF.H"
#include "CS229.h"


void displayHelp();

int main(int argc, char *argv[])
{
	FILE* inf = stdin;
	int high = INT_MIN;
	int low = INT_MAX;
	int i = 0;
	int j = 0;

	highlow_t *highlow = malloc((argc-1)*sizeof(highlow_t));
	if(highlow == 0){
		fprintf(stderr, "Couldn't create enough memory.\n");
		return -1;
	}
	for (i = 1; i < argc; ++i)
	{
		if(strncmp(argv[i], "-h", 2) == 0){
			displayHelp();
			return 0;
		}else{
			int x = sscanf(argv[i], "%d..%d", &(highlow[j].low), &(highlow[j].high));
			if(x != 2){
				fprintf(stderr, "Error reading high low value: %s\n", argv[i]);
				continue;
			}
			j++;
		}
	}
	if (j == 0)
	{
		return 0;
	}
	char format[5];
	FILE* infile = stdin;
	FILE* outfile = stdout;
	getFileType(infile, format);
	if(strncmp(format, "FORM", 4) == 0){
		trimAIFF(highlow, j);
	}
	else if(strncmp(format, "CS229", 5) == 0){
		trimCS229(highlow, j);
	}
	return 0;
}

void displayHelp(){
	printf("help\n");
}