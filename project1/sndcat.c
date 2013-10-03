#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "CS229.h"
#include "AIFF.h"


int main(int argc, char const *argv[])
{
	
	File_Data orig;
	File_Data next;
	FILE* sampleData = fopen("temporaryfile.cs229", "w");
	int foundFile = 0;
	int totalSamples;

	if (argc <= 1){
		fprintf(stderr, "Use -h for help.\n");
		return -1;
	}
	int i;
	for(i = 1; i < argc; i++){
		FILE* inf = fopen(argv[i], "r");
		if(!inf){
			fprintf(stderr, "Could not open %s, file was skipped.\n", argv[i]);
		}
		if(!foundFile){
			foundFile = 1;
			orig = soundToTemp(sampleData, inf, argv[i]);
		} else {
			next = soundToTemp(sampleData, inf, argv[i]);
			if(next.channels != orig.channels){
				fprintf(stderr, "File %s did not have the same channels as the first file.\n", argv[i]);
			}
			if(next.bitDepth != orig.bitDepth){
				fprintf(stderr, "File %s did not have the same bit depth as the first file.\n", argv[i]);
			}
		}

	}
	return 0;
}