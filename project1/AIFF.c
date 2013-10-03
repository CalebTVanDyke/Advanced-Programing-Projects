#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int processSSND(FILE* outf, char * infilepath, File_Data fileData){
	if(foundSoundData){
		fprintf(stderr, "Error reading sound data chunk.\n");
		return -1;
	}
	FILE* inf = NULL;
	if(strncmp("STDIN", infilepath, 5) == 0){
		inf = stdin;
	}else{
		inf = fopen(infilepath, "r");
	}
	if(fsetpos(inf, &SSNDLocation)){
		fprintf(stderr, "Error reading sound data chunk.\n");
		return -1;
	}
	SoundDataChunk data;
	int i;
	int chan = 0;
	chan = fileData.channels;

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
	data.blockSize = *((int*)buff);
	int count = 0;
	int j = 0;
	unsigned char sample[4];
	for (i = 0 ; i < (data.chunkSize - 8) / (fileData.bitDepth / 8); i++){
		for (j = 0; j < 4; j++){
			sample[j] = 0;
		}	
		for (j = 0; j < fileData.bitDepth / 8; j++){
			sample[j] = getc(inf);
		}
		flipBytes(sample, fileData.bitDepth / 8);
		if(fileData.bitDepth == 8){
			fprintf(outf, "%d", sample);
		} 
		else if(fileData.bitDepth == 16){
			fprintf(outf, "%d", *((short*)sample));
		} 
		else if(fileData.bitDepth == 32){
			fprintf(outf, "%d", *((int*)sample));
		}
		if((i + 1) % chan == 0){
			fprintf(outf, "\n");
		} else {
			fprintf(outf, " ");
		}
	}
	fclose(inf);
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
	fprintf(outfile, "%s\n", "CS229");
	fprintf(outfile, "SampleRate %d\n", data.sampleRate);
	fprintf(outfile, "Channels %d\n", data.channels);
	fprintf(outfile, "BitDepth %d\n", data.bitDepth);
	fprintf(outfile, "Samples %d\n", data.samples);
	fprintf(outfile, "StartData\n");
	processSSND(outfile, infilepath, data);
}

File_Data soundToTemp(FILE* outfile, FILE* infile, char const * infilepath){

	fgetc(infile);
	fgetc(infile);
	fgetc(infile);
	fgetc(infile);

	File_Data data = processAIFF(outfile, infile);
	processSSND(outfile, infilepath, data);
	return data;
}

