#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"

#define MAX_SAMPLE(X) pow(2, (X)-1) - 1
#define MIN_SAMPLE(X) pow(-2, (X) - 1)

typedef struct {
	int low;
	int high;
} highlow_t;

typedef struct {
	int chunkSize;
	unsigned int offset;
	unsigned int blockSize;
	unsigned char *WaveformData;
}  SoundDataChunk;

typedef struct {

	int chunkSize;
	short numChannels;
	unsigned int numSampleFrames;
	short sampleSize;
	int sampleRate;

} CommonChunk;

typedef struct {
	char format[6];
	char duration[11];
	int samples;
	int channels;
	int sampleRate;
	int success;
	int bitDepth;
} File_Data;

char* getFileType(FILE* inf, char * format);
char* toLittleEndian(char* rev, int size);
char* findDuration(int sampleRate, int channels, int samples, char timeStr[]);
int trim(char* fileName, int size);
int countHighLow(int samples, highlow_t *highlow, int size);
void showSamples(File_Data data, int** samples, int width, int zoom, int chan);


#endif