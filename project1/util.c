#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <math.h>
#include <limits.h>
#include <string.h>

/**
	Finds the duration of a sound file based on its sampleRate, channes, and number of samples.
	Returns a string containing a formated version of the time.
**/
char* findDuration(int sampleRate, int channels, int samples, char timeStr[]){
	float seconds = 1 /(sampleRate / (float)samples);
	int minutes = seconds / 60;
	seconds -= minutes * 60;
	int hours = minutes / 60;
	minutes -= hours * 60;
	sprintf(timeStr, "%d:%02d:%05.2f", hours, minutes, seconds);
	return timeStr;
}
/**
	Flips bytes 0-size of the string 
**/
char* flipBytes(char* rev, int size){
	int i = 0;
	for(i = 0; i < size / 2; i++){
		char temp = rev[i];
		rev[i] = rev[size - i - 1];
		rev[size - i - 1] = temp;
	}
	return rev;
}
/**
	trims the string replacing the first '\n' with '\0'
	returns the index of the replaced character 
**/
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
/**
	transfers the contents of inf to outf
**/
void rewrite(FILE* outf, FILE* inf){
	int c;
	while((c = fgetc(inf)) != EOF){
		fputc(c, outf);
	}
}
/**
	updates the string format to contain the the type of file the inf is
	CS229 file: "CS229"
	AIFF  file: "AIFF"
**/
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
/**
	Moves the maxes for the zoomed out part to the top of the samples
**/
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
/**
	Shows all samples in data
**/
void showSamples(File_Data data, int width, int zoom, int chan, int NCURSES){

	showSamplesRange(data, width, zoom, chan, NCURSES, 0, data.samples, 0 , data.channels);

}
/**
	Shows the samples for only a certain range specified by start and end
**/
void showSamplesRange(File_Data data, int width, int zoom, int chan, int NCURSES, int start, int end, int topChan, int botChan){

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

	for(i = start; i <= end && i < data.samples; i++){
		if(i % zoom == 0){
			if(NCURSES){
				if(topChan == 0 || i != start)
					printw("%9d|", i);
				else
					printw("         |");
			}
			else
				fprintf(stdout, "%9d|", i);
		}else{
			continue;
		}
		for(j = 0; j < data.channels; j++){
			if( i == start && j < topChan){
				continue;
			}
			if( i == end && j > botChan){
				break;
			}
			if(chan != -1){
				if( j != chan - 1){
					continue;
				}
			}
			if(zoom > 1){
				moveMaxes(&(data.sampleData), i, j, zoom + i, data);
			}
			int samp = data.sampleData[i][j];

			int size = (int)lround((abs(samp)*half)/((double)MAX));
			for(k = half; k > 0; k--){
				if(samp < 0 && k <= size){
					if(NCURSES)
						printw("-");
					else
						fprintf(stdout, "-");
				}else{
					if(NCURSES)
						printw(" ");
					else
						fprintf(stdout, " ");
				}
			}
			if(NCURSES)
				printw("|");
			else
				fprintf(stdout, "|");
			for(k = 0; k < half; k++){
				if( k+1 <= size && samp > 0){
					if(NCURSES)
						printw("-");
					else
						fprintf(stdout, "-");
				}else{
					if(NCURSES)
						printw(" ");
					else
						fprintf(stdout, " ");
				}
			}
			if(NCURSES)
				printw("|\n");
			else
				fprintf(stdout, "|\n");
			if(j + 1 < data.channels && chan == -1){
				if(NCURSES)
					printw("         |");
				else
					fprintf(stdout, "         |");
			}
		}
	}

}
/**
	Makes sure the the information stored in data is all valid
**/
int validateData(File_Data data){
	return data.channels >= 1 && data.channels <= 32 && 
		(data.bitDepth == 8 || data.bitDepth == 16 || data.bitDepth == 32) &&
		data.success; 
}
/**
	Cuts the file from firstMark to lastMark (inclusively) out of the sampleData from data
	Also updates the duration and number of samples contained in data
**/
void cut(File_Data* data, int firstMark, int lastMark){
	int i, j;
	int samp = 0;

	if(firstMark > lastMark){
		int temp = firstMark;
		firstMark = lastMark;
		lastMark = temp; 
	}

	for(i = 0; i < data->samples; i++){
		if(i >= firstMark && i <= lastMark){
			continue;
		}
		for(j = 0; j < data->channels; j++){
			data->sampleData[samp][j] = data->sampleData[i][j];
		}
		samp++;
	}
	data->samples = samp;
	strcpy(findDuration(data->sampleRate, data->channels, data->samples, data->duration), data->duration);
}
/**
	Frees all of the allocated samples from data
**/
void freeSamples(File_Data* data){
	int i, j;
	for(i = 0; i < data->samples; i++){
		free(data->sampleData[i]);
	}
	free(data->sampleData);
}
