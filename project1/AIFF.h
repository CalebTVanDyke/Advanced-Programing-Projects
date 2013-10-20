#ifndef AIFF_H
#define AIFF_H 1

File_Data processAIFF(FILE *outfile, FILE *inf);
void convertAIFFtoCS229(FILE* outfile, FILE* infile, char * infilepath);
File_Data AIFFtoTemp(FILE* outfile, FILE* infile, char * infilepath);
File_Data trimAIFF(highlow_t * highlow, int size);
void writeHeaderAIFF(FILE* outfile, File_Data data);
void setupSoundAIFF(FILE* outfile, File_Data data);
int writeSamplesAIFF(FILE* outf, File_Data data);
File_Data showAIFF(FILE* file, char* filePath, int width, int zoom, int chan, int curses);
File_Data showAIFFrange(FILE* file, char* fileName, int width, int zoom, int chan, int curses, int start, int end, int topChan, int bottomChan);
void freeSamples(File_Data* data);

#endif
