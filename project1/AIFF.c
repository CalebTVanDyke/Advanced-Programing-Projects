#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "CS229.h"

fpos_t SSNDLocation;
int foundSoundData = 1;
                                         
CommonChunk processComm(FILE* file){
	char buff[10];
	CommonChunk comm;
	int count = 0;
	int i;
	comm.chunkSize = 0;
	for(i = 0; i < 4; i++){
		buff[i] = fgetc(file);
		count++;
	}
	flipBytes(buff, 4);
	comm.chunkSize = *((int *)buff);
	for(i = 0; i < 2; i++){
		buff[i] = fgetc(file);
		count++;
	}
	flipBytes(buff, 2);
	comm.numChannels = *((short *)buff);
	for(i = 0; i < 4; i++){
		buff[i] = fgetc(file);
		count++;
	}
	flipBytes(buff, 4);
	comm.numSampleFrames = *((int *)buff);
	for(i = 0; i < 2; i++){
		buff[i] = fgetc(file);
		count++;
	}
	flipBytes(buff, 2);
	comm.sampleSize = *((short *)buff);
	for(i = 0; i < 10; i++){
		buff[i] = fgetc(file);
		count++;
	}
	flipBytes(buff, 10);
	long double sr = *((long double *)buff);
	comm.sampleRate = (int)sr;
	while(count < comm.chunkSize){
		fgetc(file);
		count++;
	}
	return comm;
}
int** getSamples(FILE* outf, char * infilepath, File_Data fileData){
	
	int ** ssndData = malloc(fileData.samples * (fileData.bitDepth/8)*sizeof(int*));
	int i, j, k;	
	for(i = 0; i < fileData.samples; i++){
		ssndData[i] = malloc(fileData.channels*sizeof(int));
	}

	if(foundSoundData){
		fprintf(stderr, "Sound data chunk was not found.\n");
		return NULL;
	}
	FILE* inf = NULL;
	if(strncmp("STDIN", infilepath, 5) == 0){
		inf = stdin;
	}else{
		inf = fopen(infilepath, "r");
	}
	if(fsetpos(inf, &SSNDLocation)){
		fprintf(stderr, "Error reading sound data chunk.\n");
		return NULL;
	}
	SoundDataChunk data;

	char buff[4];

	for (i = 0; i < 4; i++)
	{
		buff[i] = fgetc(inf);
	}
	flipBytes(buff, 4);
	data.chunkSize = *((int*)buff);
	for (i = 0; i < 4; ++i)
	{
		buff[i] = fgetc(inf);
	}
	flipBytes(buff, 4);
	data.offset = *((int*)buff);
	for (i = 0; i < 4; ++i)
	{
		buff[i] = fgetc(inf);
	}
	flipBytes(buff, 4);
	char sample[4];
	data.blockSize = *((int*)buff);
	for (i = 0 ; i < (data.chunkSize - 8) / (fileData.bitDepth / 8); i++){
		for(k = 0; k < fileData.channels; k++){
			for (j = 0; j < 4; j++){
				sample[j] = 0;
			}	
			for (j = 0; j < fileData.bitDepth / 8; j++){
				sample[j] = getc(inf);
			}
			flipBytes(sample, fileData.bitDepth / 8);
			if(fileData.bitDepth == 8){
				ssndData[i][k] = (int)sample[0];
				printf("%d\n", sample[0]);
			}
			else if(fileData.bitDepth == 16){
				ssndData[i][k] = (int)(*((short*)sample));
			}
			else if(fileData.bitDepth == 32){
				ssndData[i][k] = *((int*)sample);
			}
		}
	}
	fclose(inf);
	return ssndData;
}

int processSSNDtrim(FILE* outf, char * infilepath, File_Data fileData, int low, int high){

	int **samples = getSamples(outf, infilepath, fileData);
	int i, j, k;
	for(i = 0; i < fileData.samples; i++){
		for(j = 0; j < fileData.channels; j++){
			if(samples[i][j] >= low && samples[i][j] <= high){
				break;
			}
			fprintf(outf, "%d", samples[i][j]);
			if(j + 1 != fileData.channels){
				fprintf(outf, " ");
			}
		}
		if(j == fileData.channels)
			fprintf(outf, "\n");
	}
	for(i = 0; i < fileData.samples; i++){
		free(samples[i]);
	}
	free(samples);

}

int processSSND(FILE* outf, char * infilepath, File_Data fileData){

	int low, high;

	high = MAX_SAMPLE(fileData.bitDepth) + 1;
	low = high;
	processSSNDtrim(outf, infilepath, fileData, low, high);

}

File_Data processAIFF(FILE *outfile, FILE* infile){
	int foundComm = 0;
	int foundSSND = 0;
	File_Data data;
	CommonChunk comm;
	SoundDataChunk sdc;
	data.samples = -1;
	data.channels = -1;
	data.sampleRate = -1;
	data.bitDepth = -1;
	data.success = 0;

	char buff[4];


	int i;
	for(i = 0; i < 4; i++){
		buff[i] = fgetc(infile);
	}
	flipBytes(buff, 4);
	int y = *((int*)buff);

	for(i = 0; i < 4; i++){
		buff[i] = fgetc(infile);
	}
	if(strncmp(buff, "AIFF", 4) != 0){
		data.success = 0;
		return data;
	}else {
		strncpy(data.format, "AIFF\0", 5);
	}
	while(!foundComm || !foundSSND){
		buff[0] = fgetc(infile);
		if(EOF == buff[0]){
			data.success = 0;
			return data;
		}else if(buff[0] != 'C' && buff[0] != 'S'){
			continue;
		}

		for(i = 1; i < 4; i++){
			buff[i] = fgetc(infile);
			if(buff[i] == 'C'){
				ungetc(buff[i], infile);
				continue;
			}
		}
		if(strncmp(buff, "COMM", 4) == 0){
			comm = processComm(infile);
			data.samples = comm.numSampleFrames;
			data.channels = comm.numChannels;
			data.sampleRate = comm.sampleRate;
			data.bitDepth = comm.sampleSize;
			strcpy(data.duration, findDuration(data.sampleRate, data.channels, data.samples, data.duration));
			foundComm = 1;
		}
		if(strncmp(buff, "SSND", 4) == 0){
			foundSoundData = fgetpos(infile, &SSNDLocation);
			foundSSND = 1;
		}
	}
	if(foundSSND && foundComm)
		data.success = 1;
	return data;
}
void writeHeader(FILE* outfile, File_Data data){

	fprintf(outfile, "%s\n", "CS229");
	fprintf(outfile, "SampleRate %d\n", data.sampleRate);
	fprintf(outfile, "Channels %d\n", data.channels);
	fprintf(outfile, "BitDepth %d\n", data.bitDepth);
	fprintf(outfile, "Samples %d\n", data.samples);
	fprintf(outfile, "StartData\n");

}

void convertAIFFtoCS229(FILE* outfile, FILE* infile, char * infilepath){
	File_Data data = processAIFF(outfile, infile);
	if(data.success == 0){
		printf("Failed to convert file\n");
	}
	writeHeader(outfile, data);
	processSSND(outfile, infilepath, data);
}

File_Data AIFFtoTemp(FILE* outfile, FILE* infile, char * infilepath, int addHeader){


	File_Data data = processAIFF(outfile, infile);

	if(addHeader){
		writeHeader(outfile, data);
	}

	processSSND(outfile, infilepath, data);
	return data;
}

File_Data trimAIFF(int low, int high){

	File_Data data = processAIFF(stdout, stdin);

	writeHeaderAIFF(stdout, data);
	processSSNDtrim(stdout, "STDIN", data, low, high);
}


