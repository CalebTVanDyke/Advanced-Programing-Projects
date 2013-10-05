#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "AIFF.H"
#include "CS229.h"

void displayHelp();

int main(int argc, char *argv[])
{
	FILE* inf = stdin;
	int high = INT_MIN;
	int low = INT_MAX;
	int i;
	int j;

	for (i = 1; i < argc; ++i)
	{
		if(strncmp(argv[i], "-h", 2) == 0){
			displayHelp();
			return 0;
		}else{
			
		}
	}
	char format[5];
	FILE* infile = stdin;
	FILE* outfile = stdout;
	getFileType(infile, format);
	if(strncmp(format, "FORM", 4) == 0){
		trimAIFF(low, high);
	}
	else if(strncmp(format, "CS229", 5) == 0){
	}
	return 0;
}

void displayHelp(){
	printf("help\n");
}