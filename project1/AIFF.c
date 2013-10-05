#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "CS229.h"
#include "AIFF.h"
#include <math.h>

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
int** getSamplesAIFF(char * infilepath, File_Data fileData){
	
	int ** ssndData = malloc(fileData.samples*sizeof(int*));
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
	for (i = 0 ; i < fileData.samples; i++){
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
			}
			else if(fileData.bitDepth == 16){
				ssndData[i][k] = (int)(*((short*)sample));
			}
			else if(fileData.bitDepth == 32){
				ssndData[i][k] = (int)(*((int*)sample));
			}
		}
	}
	fclose(inf);
	return ssndData;
}

int writeSamplesAIFF(FILE* outf, int **samples, File_Data data, highlow_t *highlow, int size){
	int i, j, k;
	char bytes[4];
	for(i = 0; i < data.samples; i++){
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
		for(j = 0; j < data.channels; j++){
			int x = samples[i][j];
			memcpy(bytes, &x, 4);
			flipBytes(bytes, 4);
			if(data.bitDepth == 8){
				fwrite(&bytes[3], 1, 1, outf);
			} 
			else if(data.bitDepth == 16){
				fwrite(&bytes[2], 2, 1, outf);
			} 
			else if(data.bitDepth == 32){
				fwrite(&bytes, 4, 1, outf);
			}
		}
	}
	for(i = 0; i < data.samples; i++){
		free(samples[i]);
	}
	/*free(samples);*/
}

int processSSND(FILE* outf, char * infilepath, File_Data fileData){

	int **samples = getSamplesAIFF(infilepath, fileData);
	writeSamplesCS229(outf, samples, fileData, NULL, 0);

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

void convertAIFFtoCS229(FILE* outfile, FILE* infile, char * infilepath){
	File_Data data = processAIFF(outfile, infile);
	if(data.success == 0){
		printf("Failed to convert file\n");
	}
	writeHeaderCS229(outfile, data);
	processSSND(outfile, infilepath, data);
}

File_Data AIFFtoTemp(FILE* outfile, FILE* infile, char * infilepath){


	File_Data data = processAIFF(outfile, infile);

	processSSND(outfile, infilepath, data);
	return data;
}

File_Data trimAIFF(highlow_t* highlow, int size){

	File_Data data = processAIFF(stdout, stdin);

	int **samples = getSamplesAIFF("STDIN", data);

	int exclude = countHighLow(data.samples, highlow, size);
	data.samples -= exclude;
	fprintf(stderr, "%d\n", exclude);
	writeHeaderAIFF(stdout, data);
	data.samples += exclude;

	setupSoundAIFF(stdout, stdin, data);

	writeSamplesAIFF(stdout, samples, data, highlow, size);
}
void setupSoundAIFF(FILE* outfile, FILE* infile, File_Data data){

	char bytes[4];

	strncpy(bytes, "SSND", 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	int fileSize = data.samples * (data.bitDepth / 8) * data.channels + 8;
	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	fileSize = 0;
	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	fileSize = 0;
	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);
}
void writeHeaderAIFF(FILE* outfile, File_Data data){
	int fileSize = 30 + data.samples * (data.bitDepth/8) * data.channels;

	char bytes[4] = "FORM";
	fwrite(bytes, sizeof(bytes), 1, outfile);

	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	strncpy(bytes, "AIFF", 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);
	

	strncpy(bytes, "COMM", 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	fileSize = 18;
	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	memcpy(bytes, (char*)&data.channels, 2);
	flipBytes(bytes, 2);
	fwrite(bytes, sizeof(bytes)/2, 1, outfile);

	memcpy(bytes, (char*)&data.samples, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	memcpy(bytes, (char*)&data.bitDepth, 2);
	flipBytes(bytes, 2);
	fwrite(bytes, sizeof(bytes)/2, 1, outfile);

	char buff[10];
	long double sr = (long double)data.sampleRate;
	memcpy(buff, (char*)&sr, 10);
	flipBytes(buff, 10);
	fwrite(buff, sizeof(buff), 1, outfile);
}
int showAIFF(int width, int zoom, int chan){
	File_Data data = processAIFF(stdout, stdin);

	int ** samples = getSamplesAIFF("STDIN", data);

	showSamples(data, samples, width, zoom, chan);

	int i;
	for(i = 0; i < data.samples; i++){
		free(samples[i]);
	}

}

