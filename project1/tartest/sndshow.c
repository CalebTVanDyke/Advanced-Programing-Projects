#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "CS229.h"
#include "AIFF.h"

void getInt(int* x, char* s);
void showHelp();


int main(int argc, char *argv[])
{
	char format[5];

	int i, j;
	int width = 80, zoom = 1, chan = -1;
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			for(j = 0; j < strlen(argv[i]); j++){
				if(argv[i][j] == 'h'){
					showHelp();
					return 0;
				}
				if(argv[i][j] == 'w'){
					if(i + 1 < argc){
						getInt(&width, argv[i+1]);
					}else{
						fprintf(stderr, "-w must be followed by a number. Default value of 80 will be used. Use -h for help.\n");
					}
				}
				if(argv[i][j] == 'z'){
					if(i + 1 < argc){
						getInt(&zoom, argv[i+1]);
					}else{
						fprintf(stderr, "-z must be followed by a number. Default value of 1 will be used. Use -h for help.\n");
					}
				}
				if(argv[i][j] == 'c'){
					if(i + 1 < argc){
						getInt(&chan, argv[i+1]);
						if(chan < 1){
							fprintf(stderr, "Chan can not be less than 1.\n");
						}
					}else{
						fprintf(stderr, "-c must be followed by a number. Default value will be used. Use -h for help.\n");
					}
				}
			}
		}
	}
	getFileType(stdin, format);

	if(strncmp(format, "FORM", 4) == 0){
		showAIFF(width, zoom, chan);
	}
	else if(strncmp(format, "CS229", 5) == 0){
		showCS229(width, zoom, chan);
	}
	return 0;
}
void getInt(int* x, char* s){
	*x = atoi(s);
}
void showHelp(){
	printf("sndshow\n");
	int i;
	for (i = 0; i < 60; ++i){
		printf("-");
	}
	printf("\n");
	printf("sndshow reads a file from stdin and displays a textual version of it\n\n");
	printf("sndshow [-w w] [-c c] [-z n]\n\n");
	printf("Options:\n");
	printf("   -w w    How many characters should be printed on each line. Default is 80.\n\n");
	printf("   -c c    Only display a specific channel. Default will display all channels.\n\n");
	printf("   -z n    Zoom out by a factor of n. If not specifed, the default is n = 1.\n\n");
	printf("   -h      help: information on how to use the program\n\n");
}