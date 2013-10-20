#ifndef UTIL_H
#define UTIL_H

#include "stdio.h"

#define MAX_SAMPLE(X) pow(2, (X)-1) - 1
#define MIN_SAMPLE(X) pow(-2, (X) - 1)
#define USE_NCURSES 1
#define USE_STDOUT 0

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
	int **sampleData;
} File_Data;

char* getFileType(FILE* inf, char * format);
char* toLittleEndian(char* rev, int size);
char* findDuration(int sampleRate, int channels, int samples, char timeStr[]);
int trim(char* fileName, int size);
int validateData(File_Data data);
int countHighLow(int samples, highlow_t *highlow, int size);
void showSamplesSTDOUT(File_Data data, int width, int zoom, int chan);
void showSamplesNcurse(File_Data data, int width, int zoom, int chan);
void showSamples(File_Data data, int width, int zoom, int chan, int NCURSES);
void showSamplesRange(File_Data data, int width, int zoom, int chan, int NCURSES, int start, int end, int topChan, int botChan);
void cut(File_Data* data, int firstMark, int lastMark);

#endif