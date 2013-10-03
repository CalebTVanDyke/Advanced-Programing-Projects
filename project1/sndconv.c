#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "CS229.h"
#include "AIFF.h"

#define MAX_LINE_LENGTH 200
#define MAX_PATH_LENGTH 100

char * getFileName(char * filePath, int size);
void showHelp();
int main(int argc, char const *argv[])
{
	int success = 0;
	int toAIFF;
	int toCS229;
	int fileInput = 0;

	char infilepath[MAX_PATH_LENGTH] = "STDIN";
	char outfilepath[MAX_PATH_LENGTH] = "STDOUT";
	char line[MAX_LINE_LENGTH];
	char format[5];
	char * fileName; 

	File_Data data;

	FILE* infile = NULL;
	FILE* outfile = NULL;

	int i;

	if(argc > 1){
		for(i = 0; i < argc; i++){
			if(strncmp("-", argv[i], 1) == 0){
				int j;
				for(j = 0; j < strlen(argv[i]); j++){

					if(argv[i][j] == '1'){
						fileInput = 1;
						printf("Enter the pathname of a sound file:\n");
						fgets(infilepath, MAX_PATH_LENGTH, stdin);

						printf("Enter the pathname of a sound file:\n");
						fgets(outfilepath, MAX_PATH_LENGTH, stdin);


						trim(infilepath, MAX_PATH_LENGTH);
						trim(outfilepath, MAX_PATH_LENGTH);



						infile = fopen(infilepath, "r");
						outfile = fopen(outfilepath, "w");

						if(NULL == infile){
							fprintf(stderr, "Input file could not be located.\n");
							return -1;
						}
						if(NULL == outfile){
							fprintf(stderr, "Output file could not be created.\n");
							return -1;
						}
					}
					else if(argv[i][j] == 'a'){
						if(toCS229){
							fprintf(stderr, "-a and -c can not both be specified\n");
							return -1;
						}
						toAIFF = 1;
					}
					else if(argv[i][j] == 'c'){
						if(toAIFF){
							fprintf(stderr, "-a and -c can not both be specified\n");
							return -1;
						}
						toCS229 = 1;
					}
					else if(argv[i][j] == 'h'){
						if(fileInput){
							continue;
						}
						showHelp();
						return 0;
					}
				}
			}
		}
	} 
	if( !fileInput ) {
		infile = stdin;
		outfile = stdout;
	}

	for(i = 0; i < 4; i++){
		format[i] = fgetc(infile);
		if(format[i] == EOF){
			break;
		}
	}
	if(strncmp(format, "FORM", 4) == 0){
		if(toAIFF){
			fwrite(format, 1, 4, outfile);
			rewrite(outfile, infile);
		}else {
			convertAIFFtoCS229(outfile, infile, infilepath);
		}
	} else {
		format[4] = fgetc(infile);
	}
	if(strncmp(format, "CS229", 5) == 0){
		if(toCS229){
			fprintf(outfile, "CS229");
			rewrite(outfile, infile);
		} else {
			convertCS229toAIFF(outfile, infile);
		}
	}

	fclose(infile);
	fclose(outfile);
	return 0;
}

void showHelp(){
		printf("sndconv\n");
	int i;
	for (i = 0; i < 60; ++i){
		printf("-");
	}
	printf("\n");
	printf("sndconv converts audio files of the form CS229 and AIFF\n\n");
	printf("sndconv [-1ach]\n\n");
	printf("Options:\n");
	printf("   -1     program will prompt you to enter the input file and output file name\n\n");
	printf("   -a     forces output to be in AIFF format.  Cannot be used with -c\n\n");
	printf("   -c     forces output to be in CS229 format. Cannot be used with -a\n\n");
	printf("   -h     help: information on how to use the program\n\n");
	printf("Default:\n");
	printf("   If -1 is no specified input will be stdin and output will be stdout\n");
	printf("   If -a or -c are not specified then output will be in the opposite form of the input\n");
}