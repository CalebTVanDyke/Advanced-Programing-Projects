#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "AIFF.h"
#include "CS229.h"

#define MAX_LINE_LENGTH 200

int foundStartData = 0;


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
	int ** samps = getSamplesCS229(file, &data);
	int i;
	for(i = 0; i < data.samples; i++){
		free(samps[i]);
	}
	/*free(samps);*/
	if(data.sampleRate < 0 || data.bitDepth < 0 || data.channels < 0 || data.channels > 32 || (data.bitDepth != 8 && data.bitDepth != 16 && data.bitDepth != 32)){
		data.success = 0;
	} else {
		data.success = 1;
		strcpy(data.duration, findDuration(data.sampleRate, data.channels, data.samples, data.duration));
	}
	return data;
}

void convertCS229toAIFF(FILE* outfile, FILE* infile){
	File_Data data;
	data.samples = -1;
	processHeader(infile, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}
	int **samples = getSamplesCS229(infile, &data);
	writeHeaderAIFF(outfile, data); 
	setupSoundAIFF(outfile, infile, data);
	writeSamplesAIFF(outfile, samples, data, NULL, 0);
}

File_Data CS229toTemp(FILE* outfile, FILE* infile){
	File_Data data;
	strncpy(data.format, "CS229", 5);
	processHeader(infile, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	rewrite(outfile, infile);
	printf("\n");
	return data;
}
int writeSamplesCS229(FILE* outf, int** samps, File_Data fileData, highlow_t* highlow, int size){
	int i, j, k;
	for(i = 0; i < fileData.samples; i++){
		int toContinue = 0;
		for(j = 0; j < size; j++){
			if(i >= highlow[j].low  && i <= highlow[j].high){
				toContinue = 1;
				break;
			}
		}
		if(toContinue){
			toContinue = 0;
			continue;
		}
		for(j = 0; j < fileData.channels; j++){
			fprintf(outf, "%d", samps[i][j]);
			if(j + 1 != fileData.channels){
				fprintf(outf, " ");
			}
		}
		if(j == fileData.channels);
			fprintf(outf, "\n");
	}
	for(i = 0; i < fileData.samples; i++){
		free(samps[i]);
	}
	/*free(samps);*/
}
void writeHeaderCS229(FILE* outfile, File_Data data){
	fprintf(outfile, "%s\n", "CS229");
	fprintf(outfile, "SampleRate %d\n", data.sampleRate);
	fprintf(outfile, "Channels %d\n", data.channels);
	fprintf(outfile, "BitDepth %d\n", data.bitDepth);
	fprintf(outfile, "Samples %d\n", data.samples);
	fprintf(outfile, "StartData\n");
}
int ** getSamplesCS229(FILE* inf, File_Data *data){
	fpos_t sampleLocation;

	char line[MAX_LINE_LENGTH];

	fgetpos(inf, &sampleLocation);


	int numSampls = 0;
	if(data->samples <= 0){

		while(fgets(line, MAX_LINE_LENGTH, inf)){
			numSampls++;
		}
		fsetpos(inf, &sampleLocation);
	}


	int **samples = NULL;
	if(data->samples > 0){
		samples = malloc(data->samples * sizeof(int*));
		numSampls = data->samples;
	}else {
		samples = malloc(numSampls * sizeof(int*));
	}
	int i;
	for(i = 0; i < numSampls; i++){
		samples[i] = malloc(data->channels * sizeof(int*));
	}
	int j;
	i = 0;
	while(!feof(inf)){
		for(j = 0; j < data->channels; j++){
			fscanf(inf, "%d " , &samples[i][j]);
			if(samples[i][j] < MIN_SAMPLE(data->bitDepth) || samples[i][j] > MAX_SAMPLE(data->bitDepth)){
				data->bitDepth = -1;
			}
		}
		i++;
	}
	if(i != data->samples){
		data->samples = numSampls;
	}
	return samples;
}
void trimCS229(highlow_t *highlow, int size){

	File_Data data;
	data.samples = -1;
	processHeader(stdin, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	int **samples = getSamplesCS229(stdin, &data);

	int exclude = countHighLow(data.samples, highlow, size);
	data.samples -= exclude;
	writeHeaderCS229(stdout, data);
	data.samples += exclude;
	writeSamplesCS229(stdout, samples, data, highlow, size);
}
int showCS229(int width, int zoom, int chan){
	File_Data data;
	data.samples = -1;
	processHeader(stdin, &data);

	if(!validateData(data)){
		fprintf(stderr, "Error occured in header.\n");
		exit(-1);
	}

	int ** samples = getSamplesCS229(stdin, &data);

	showSamples(data, samples, width, zoom, chan);

	int i;
	for(i = 0; i < data.samples; i++){
		free(samples[i]);
	}

}