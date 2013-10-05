#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "CS229.h"
#include "AIFF.h"

void getInt(int* x, char* s);

int main(int argc, char *argv[])
{
	char format[5];

	int i, j;
	int width = 80, zoom = 1, chan = -1;
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			for(j = 0; j < strlen(argv[i]); j++){
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