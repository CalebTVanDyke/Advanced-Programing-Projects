#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "CS229.h"
#include "AIFF.h"


#define WRITE_HEADER 1
#define NO_HEADER 0

File_Data soundToTemp(FILE* soundFile, FILE* infile, char * filePath, int addHeader);
void displayHelp();

int main(int argc, char *argv[])
{
	
	File_Data orig;
	File_Data next;
	char* tempFileName = tmpnam(NULL);
	FILE* sampleData = fopen(tempFileName, "w");
	int foundFile = 0;
	int toAIFF = 0;
	int toCS229 = 0;
	char firstFile[5];

	if (argc <= 1){
		fprintf(stderr, "Use -h for help.\n");
		return -1;
	}
	int i;
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			int j;
			for(j = 0; j < strlen(argv[i]); j++){
				if(argv[i][j] == 'h'){
					displayHelp();
					return 0;
				}
				else if(argv[i][j] == 'c'){
					if(toAIFF){
						fprintf(stderr, "Can not specify -a and -c. Use -h for help.\n");
						return -1;
					}
					toCS229 = 1;
				}
				else if(argv[i][j] == 'a'){
					if(toCS229){
						fprintf(stderr, "Can not specify -a and -c. Use -h for help.\n");
						return -1;
					}
					toAIFF = 1;
				}
			}
		}
		else {
			FILE* inf = fopen(argv[i], "r");
			if(!inf){
				fprintf(stderr, "Could not open %s, file was skipped.\n", argv[i]);
			}
			if(!foundFile){
				foundFile = 1;
				orig = soundToTemp(sampleData, inf, argv[i], WRITE_HEADER);
				strncpy(firstFile, orig.format, 5);
			} else {
				next = soundToTemp(sampleData, inf, argv[i], NO_HEADER);
				if(next.channels != orig.channels){
					fprintf(stderr, "File %s did not have the same channels as the first file.\n", argv[i]);
				}
				if(next.bitDepth != orig.bitDepth){
					fprintf(stderr, "File %s did not have the same bit depth as the first file.\n", argv[i]);
				}
			}
		}
	}

	fclose(sampleData);
	sampleData = fopen(tempFileName, "r");
	if(toAIFF){
		convertCS229toAIFF(stdout, sampleData);
	}
	else if(toCS229){
		rewrite(stdout, sampleData);
	}else {
		if(strncmp(firstFile, "CS229", 5) == 0){
			rewrite(stdout, sampleData);
		}
		else if (strncmp(firstFile, "AIFF", 4) == 0){
			convertCS229toAIFF(stdout, sampleData);
		} else {
			fprintf(stderr, "FIRST FILE NO SPECIFIED.\n");
		}
	}
	fclose(sampleData);
	unlink(tempFileName);
	return 0;
}

File_Data soundToTemp(FILE* soundFile, FILE* infile, char * filePath, int addHeader){

	File_Data data;

	char format[5];
	int i;
	for(i = 0; i < 4; i++){
		format[i] = fgetc(infile);
		if(format[i] == EOF){
			break;
		}
	}
	if(strncmp(format, "FORM", 4) == 0){
		data = AIFFtoTemp(soundFile, infile, filePath, addHeader);
	} else {
		format[4] = fgetc(infile);
	}
	if(strncmp(format, "CS229", 5) == 0){
		data = CS229toTemp(soundFile, infile, addHeader);
	}

	return data;
}

void displayHelp(){
	printf("sndcat\n");
	int i;
	for (i = 0; i < 60; ++i){
		printf("-");
	}
	printf("\n");
	printf("sndcat converts audio files of the form CS229 and AIFF\n\n");
	printf("sndcat [-ach]\n\n");
	printf("Options:\n");
	printf("   -a     forces output to be in AIFF format.  Cannot be used with -c\n\n");
	printf("   -c     forces output to be in CS229 format. Cannot be used with -a\n\n");
	printf("   -h     help: information on how to use the program\n\n");
	printf("Default:\n");
	printf("   If -a or -c are not specified then output will be in the opposite form of the input\n");
}

