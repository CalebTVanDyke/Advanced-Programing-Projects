#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

#define MAX_LINE_LENGTH 200

int foundStartData = 0;

int processStartData(FILE* outfile, FILE* infile ,File_Data * data){
	if(foundStartData = 0){
		data->bitDepth = -1;
		return 1;
	}
	char line[MAX_LINE_LENGTH];
	int numSam = 0;
	while(fgets(line, MAX_LINE_LENGTH, infile)){
		if(line[0] == '\n' || line[0] == '#'){
			continue;
		}
		int sample = 0;
		int i;
		char * sampleStr = NULL;
		char bytes[4];
		for(i = 0, sampleStr = strtok(line, " "); i < data->channels; i++){
			sscanf(sampleStr, "%d", &sample);
			sampleStr = strtok(NULL, " ");
			if(data->bitDepth == -1 || sample > pow(2, data->bitDepth-1) - 1 || sample < pow(-2, data->bitDepth - 1)){
				data->success = 0;
				data->bitDepth = -1;
				return;
			}
			if(outfile){
				memcpy(bytes, (char*)&sample, 4);
				flipBytes(bytes, 4);
				if(data->bitDepth == 8){
					fwrite(&bytes[3], 1, 1, outfile);
				} 
				else if(data->bitDepth == 16){
					fwrite(&bytes[2], 2, 1, outfile);
				} 
				else if(data->bitDepth == 32){
					fwrite(&bytes, 4, 1, outfile);
				}
			}
		}
		numSam++;
	}
	if(data->samples < 0){
		data->samples = numSam;
	} else {
		if(data->samples != numSam){
			data->bitDepth = -1;
			return 1;
		}
	}
	return 0;
}

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
		else if(strncmp(line, "SampleRate", strlen("SampleRate")) == 0){
			sscanf(line, "SampleRate %d", &data->sampleRate);
		}
		else if(strncmp(line, "BitDepth", strlen("BitDepth")) == 0){
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
	processStartData(NULL, file, &data);
	if(data.sampleRate < 0 || data.bitDepth < 0 || data.channels < 0 || data.channels > 32 || (data.bitDepth != 8 && data.bitDepth != 16 && data.bitDepth != 32)){
		data.success = 0;
	} else {
		data.success = 1;
		strcpy(data.duration, findDuration(data.sampleRate, data.channels, data.samples, data.duration));
	}
	return data;
}


void convertCS229toAIFF(FILE* outfile, FILE* infile){
	char bytes[4];
	char format[4] = "FORM";
	fwrite(format, sizeof(format), 1, outfile);
	File_Data data;
	processHeader(infile, &data);

	if(data.success == 0){
		printf("File failed to convert\n");
		return;
	}
	int fileSize = 20 + 18 + data.samples * data.bitDepth;

	memcpy(bytes, (char*)&fileSize, 4);
	flipBytes(bytes, 4);
	fwrite(bytes, sizeof(bytes), 1, outfile);

	strncpy(format, "AIFF", 4);
	fwrite(format, sizeof(format), 1, outfile);
	

	strncpy(format, "COMM", 4);
	fwrite(format, sizeof(format), 1, outfile);

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

	strncpy(format, "SSND", 4);
	fwrite(format, sizeof(format), 1, outfile);

	fileSize = data.samples * data.bitDepth;
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

	processStartData(outfile, infile, &data); 
}

File_Data CS229toTemp(FILE* outfile, FILE* infile, int addHeader){
	File_Data data;
	strncpy(data.format, "CS229", 5);
	processHeader(infile, &data);
	if(addHeader){
		fprintf(outfile, "%s\n", "CS229");
		fprintf(outfile, "SampleRate %d\n", data.sampleRate);
		fprintf(outfile, "Channels %d\n", data.channels);
		fprintf(outfile, "BitDepth %d\n", data.bitDepth);
		fprintf(outfile, "Samples %d\n", data.samples);
		fprintf(outfile, "StartData\n");
	}
	rewrite(outfile, infile);
	return data;
}