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
void enterFilePath(char * path);
void printHelp();
void getFileInfo(char * path);

int main(int argc, char const *argv[])
{
	int help = 0;
	int part1 = 0;
	int success = 0;
	
	char filePath[MAX_PATH_LENGTH];

	int i;

	if(argc <= 1){
		fprintf(stderr, "Use the switch -h for help.\n");
		return -1;
	}
	if(argc > 1){
		for(i = 0; i < argc; i++){
			if(strncmp("-", argv[i], 1) == 0){
				int j;
				for(j =0; j < strlen(argv[i]); j++){
					if(argv[i][j] == '1'){
						part1 = 1;
						enterFilePath(filePath);
						trim(filePath, MAX_PATH_LENGTH);
						getFileInfo(filePath);
					}
					else if(argv[i][j] == 'h'){
						if(part1){
							continue;
						}
						printHelp();
						return 0;
					}
				}
			}
		}
	}
	if(argc > 1 && !part1){
		for(i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				fprintf(stderr, "\"%s\" is not a supported switch.\n", argv[i]);
				continue;
			}
			strcpy(filePath, argv[i]);
			getFileInfo(filePath);
		}
	}
	return 0;
}
void getFileInfo(char * filePath){

	int i;

	File_Data data;

	char line[MAX_LINE_LENGTH];
	char format[5];
	char * fileName; 

	FILE *file = fopen(filePath, "r");
	if(NULL == file){
		fprintf(stderr, "%s could not be located.\n", filePath);
		return;
	}
	getFileType(file, format);
	if(strncmp(format, "FORM", 4) == 0){
		data = processAIFF(NULL ,file);
	}
	else if(strncmp(format, "CS229", 5) == 0){
		data = processCS229(file);
	}
	else{
		fprintf(stderr, "%s was not of the correct file type.\n", filePath);
	}
	if(!data.success){
		fprintf(stderr, "File was not in the correct format\n");
		return;
	}
	for(i = 0; i < 60; i++){
		printf("-");
	}

	fileName = getFileName(filePath, strlen(filePath));

	printf("\n");
	printf("   Filename: %s\n", fileName);
	printf("     Format: %s\n", data.format);
	printf("Sample Rate: %d\n", data.sampleRate);
	printf("   Channels: %d\n", data.channels);
	printf("    Samples: %d\n", data.samples);
	printf("   Duration: %s\n", data.duration);
	for(i = 0; i < 60; i++){
		printf("-");
	}
	printf("\n");
	fclose(file);
}
void printHelp(){
	printf("sndinfo\n");
	int i;
	for (i = 0; i < 60; ++i){
		printf("-");
	}
	printf("\n");
	printf("sndinfo displays info about a CS229 or AIFF audio file.\n\n");
	printf("sndinfo [<file> ...]\n");
	printf("sndinfo [-1h]\n\n");
	printf("Options:\n");
	printf("   -1     have the program prompt you to enter file name\n\n");
	printf("   -h     help: information on how to use the program\n\n");
	printf("Default:\n");
	printf("   If no options are use the files can be specified as command line arguments\n");
}
void enterFilePath(char * filePath){
	printf("Enter the pathname of a sound file:\n");
	fgets(filePath, MAX_PATH_LENGTH, stdin);
}
char * getFileName(char * filePath, int size){
	char * name = malloc(MAX_PATH_LENGTH * sizeof(char));
	int i;
	int j = 0;
	for(i = size - 1; i >= 0; i--){
		if(filePath[i] == '\\' || filePath[i] == '/'){
			flipBytes(name, j);
			return name;
		}else{
			name[j] = filePath[i];
			j++;
		}
	}
	flipBytes(name, j);
	return name;		

}