#include <stdio.h>
#include "util.h"
#include <math.h>
#include <limits.h>

char* findDuration(int sampleRate, int channels, int samples, char timeStr[]){
	float seconds = 1 /(sampleRate / (float)samples);
	int minutes = seconds / 60;
	seconds -= minutes * 60;
	int hours = minutes / 60;
	minutes -= hours * 60;
	sprintf(timeStr, "%d:%02d:%05.2f", hours, minutes, seconds);
	return timeStr;
}

char* flipBytes(char* rev, int size){
	int i = 0;
	for(i = 0; i < size / 2; i++){
		char temp = rev[i];
		rev[i] = rev[size - i - 1];
		rev[size - i - 1] = temp;
	}
	return rev;
}
int trim(char* fileName, int size){
	int i;
	for(i = 0; i < size; i++){
		if('\n' == fileName[i]){
			fileName[i] = '\0';
			return i;
		}
	}
	return i;
}
void rewrite(FILE* outf, FILE* inf){
	int c;
	while((c = fgetc(inf)) != EOF){
		fputc(c, outf);
	}
}
char * getFileType(FILE* inf, char * format){
	int i;
	for(i = 0; i < 4; i++){
		format[i] = fgetc(inf);
		if(format[i] == EOF){
			break;
		}
	}
	if(strncmp(format, "FORM", 4) == 0){
		return format;
	} else {
		format[4] = fgetc(inf);
	}
	if(strncmp(format, "CS229", 5) == 0){
		return format;
	}
	return NULL;
}
int countHighLow(int samples ,highlow_t *highlow, int size){
	int count = 0;
	int i, j;

	for(i = 0; i < samples; i++){
		for(j = 0; j < size; j++){
			if(i >= highlow[j].low  && i <= highlow[j].high){
				count++;
				break;
			}
		}
	}
	return count;
}
void moveMaxes(int ***samples, int row, int column, int zoom, File_Data data){
	int i, j;
	int maxSamp = INT_MIN;
	for(i = row; i < zoom && i < data.samples; i++){
		if(abs((*samples)[i][column]) > abs(maxSamp)){
			maxSamp = (*samples)[i][column];
		}
	}
	(*samples)[row][column] = maxSamp;
}

void showSamples(File_Data data, int** samples, int width, int zoom, int chan){

	if(chan > data.channels){
		fprintf(stderr, "Chan was not a valid number. Will display all channels.\n");
		chan = -1;
	}
	if(width < 20){
		fprintf(stderr, "Width was less then the minimum value of 20. A width of 20 will be used instead.\n");
		width = 20;
	}
	if(zoom < 1){
		fprintf(stderr, "Zoom was less than 1. A zoom of 1 will be used instead\n");
		zoom = 1;
	}

	int i, j, k;

	int half = (width-12)/2;

	int MAX = MAX_SAMPLE(data.bitDepth) + 1;
	int MIN = MIN_SAMPLE(data.bitDepth);
	int factor = MAX/half;
	int maxSamp = INT_MIN;

	for(i = 0; i < data.samples; i++){
		if(i % zoom == 0){
			fprintf(stdout, "%9d|", i);
		}else{
			continue;
		}
		for(j = 0; j < data.channels; j++){
			if(chan != -1){
				if( j != chan - 1){
					continue;
				}
			}
			if(zoom > 1){
				moveMaxes(&samples, i, j, zoom + i, data);
			}
			int samp = samples[i][j];

			int size = (int)lround((abs(samp)*half)/((double)MAX));
			for(k = half; k > 0; k--){
				if(samp < 0 && k <= size){
					printf("-");
				}else{
					printf(" ");
				}
			}
			fprintf(stdout, "|");
			for(k = 0; k < half; k++){
				if( k+1 <= size && samp > 0){
					fprintf(stdout, "-");
				}else{
					fprintf(stdout, " ");
				}
			}
			fprintf(stdout, "|\n");
			if(j + 1 < data.channels && chan == -1)
				fprintf(stdout, "         |");
		}
	}
}
