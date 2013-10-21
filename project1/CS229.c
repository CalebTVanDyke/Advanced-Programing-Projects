#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "AIFF.h"
#include "CS229.h"
#include <ncurses.h>

#define MAX_LINE_LENGTH 200

int foundStartData = 0;

/**

Processes the header of a file starting after the initial "CS229" at the top of the file
Stores the information from the file in File_Data structure data;

**/
void processHeader(FILE* file , File_Data * data){
	char line[MAX_LINE_LENGTH];
	int foundChannels = 0;
	int foundSampleRate = 0;
	int foundBitDepth = 0;
	while(fgets(line, MAX_LINE_LENGTH, file)){
		if(strncmp(line, "Samples", 7) == 0){
			sscanf(line, "Samples %d", &data->samples);
		}
		else if(strncmp(line, "Channels", 8) == 0){
			sscanf(line, "Channels %d", &data->channels);
		}
		else if(strncmp(line, "SampleRate", strlen("SampleRate\0")) == 0){
			sscanf(line, "SampleRate %d", &data->sampleRate);
		}
		else if(strncmp(line, "BitDepth", strlen("BitDepth\0")) == 0){
			sscanf(line, "BitDepth %d", &data->bitDepth);
		}
		if(strncmp(line, "StartData", 7) == 0){
			foundStartData = 1;
			break;
		}
	}
	if(data->sampleRate < 0 || data->bitDepth < 0 || data->channels < 0 || data->channels > 32 || (data->bitDepth != 8 && data->bitDepth != 16 && data->bitDepth != 32)){
		data->success = 0;
	} else {
		data->success = 1;
	}
}

/**

Processes the whole CS229 file starting after the "CS229 at the top of the file"
Thie will call processHeader and getSamplesCS229 to process the data of the file

**/
File_Data processCS229(FILE *file){
	char line[MAX_LINE_LENGTH];
	File_Data data;
	strncpy(data.format, "CS229\0", 6);
	data.samples = -1;
	data.channels = -1;
	data.sampleRate = -1;
	data.bitDepth = -1;
	processHeader(file, &data);
	if(data.success == 0){
		fprintf(stderr, "Error reading header.\n");
		exit(EXIT_FAILURE);
	}
	getSamplesCS229(file, &data);
	if(data.sampleRate < 0 || data.bitDepth < 0 || data.channels < 0 || data.channels > 32 || (data.bitDepth != 8 && data.bitDepth != 16 && data.bitDepth != 32)){
		data.success = 0;
	} else {
		data.success = 1;
		strcpy(data.duration, findDuration(data.sampleRate, data.channels, data.samples, data.duration));
	}
	return data;
}

/**

Takes the infile in the form of CS229 and writes in to outfile in the form of AIFF

**/

void convertCS229toAIFF(FILE* outfile, FILE* infile){
	File_Data data;
	data.samples = -1;
	processHeader(infile, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}
	getSamplesCS229(infile, &data);
	writeHeaderAIFF(outfile, data); 
	setupSoundAIFF(outfile, data);
	writeSamplesAIFF(outfile, data);
}
/**

Write the sample data of a cs229 file to a temporary file

**/

File_Data CS229toTemp(FILE* outfile, FILE* infile){
	File_Data data;
	data.samples = -1;
	strncpy(data.format, "CS229", 5);
	processHeader(infile, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	getSamplesCS229(infile, &data);
	writeSamplesCS229(outfile, data);

	return data;
}
/**

Writes the samples in fileData to the outf in the form CS229

**/
int writeSamplesCS229(FILE* outf, File_Data fileData){
	int i, j, k;
	for(i = 0; i < fileData.samples; i++){
		for(j = 0; j < fileData.channels; j++){
			fprintf(outf, "%d ", fileData.sampleData[i][j]);
		}
		fprintf(outf, "\n");
	}
}
/**

Writes the information of data into the header of outfile in the form of CS229

**/
void writeHeaderCS229(FILE* outfile, File_Data data){
	fprintf(outfile, "%s\n", "CS229");
	fprintf(outfile, "SampleRate %d\n", data.sampleRate);
	fprintf(outfile, "Channels %d\n", data.channels);
	fprintf(outfile, "BitDepth %d\n", data.bitDepth);
	fprintf(outfile, "Samples %d\n", data.samples);
	fprintf(outfile, "StartData\n");
}
/**

Gets the sample data from a CS229 file, assuming the the file is at the start of the samples
Updates the data.samples catagory if the number of samples was not specified

**/
void getSamplesCS229(FILE* inf, File_Data *data){
	fpos_t sampleLocation;

	char line[MAX_LINE_LENGTH];

	fgetpos(inf, &sampleLocation);


	int numSampls = 0;
	if(data->samples <= 0){

		while(fgets(line, MAX_LINE_LENGTH, inf)){
			numSampls++;
		}
		fsetpos(inf, &sampleLocation);
		data->samples = numSampls;
	}


	data->sampleData = NULL;
	if(data->samples > 0){
		data->sampleData = malloc(data->samples * sizeof(int*));
		numSampls = data->samples;
	}else {
		data->sampleData = malloc(numSampls * sizeof(int*));
	}
	int i;
	for(i = 0; i < numSampls; i++){
		data->sampleData[i] = malloc(data->channels * sizeof(int*));
	}
	int j;
	i = 0;
	while(!feof(inf)){
		for(j = 0; j < data->channels; j++){
			fscanf(inf, "%d " , &data->sampleData[i][j]);
			if(data->sampleData[i][j] < MIN_SAMPLE(data->bitDepth) || data->sampleData[i][j] > MAX_SAMPLE(data->bitDepth)){
				data->bitDepth = -1;
			}
		}
		i++;
	}
}
/**

Trims the cs229 sample data for every element in the pointer highlow
Size is the amount of highlow combinations there are

**/
void trimCS229(highlow_t *highlow, int size){

	File_Data data;
	data.samples = -1;
	processHeader(stdin, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	getSamplesCS229(stdin, &data);

	int i;
	for(i = 0; i < size; i++){
		cut(&data, highlow[i].low, highlow[i].high);
	}
	writeHeaderCS229(stdout, data);
	writeSamplesCS229(stdout, data);
}
/**

Shows the sample data to stdout.
The specified file should be positioned right after the CS229 to start the file

**/
File_Data showCS229(FILE* file, int width, int zoom, int chan, int curses){
	File_Data data;
	data.samples = -1;
	processHeader(file, &data);

	if(!validateData(data)){
		if(curses)
			endwin();
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	getSamplesCS229(file, &data);
	if(curses){
		int rows, cols;
		getmaxyx(stdscr, rows, cols);
		int botSamp = (rows - 3) / data.channels;
		showSamplesRange(data, width, zoom, chan, curses, 0, botSamp, 0, botSamp*(rows - 3) - botSamp * data.channels);
	}
	else
		showSamplesSTDOUT(data, width, zoom, chan);

	return data;
}