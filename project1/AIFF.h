#ifndef AIFF_H
#define AIFF_H 1

File_Data processAIFF(FILE *outfile, FILE *inf);
void convertAIFFtoCS229(FILE* outfile, FILE* infile, char * infilepath);
File_Data AIFFtoTemp(FILE* outfile, FILE* infile, char * infilepath);
File_Data trimAIFF(highlow_t * highlow, int size);
void writeHeaderAIFF(FILE* outfile, File_Data data);
void setupSoundAIFF(FILE* outfile, FILE* infile, File_Data data);
int writeSamplesAIFF(FILE* outf, int **samples, File_Data data, highlow_t *highlow, int size);
int showAIFF(int width, int zoom, int chan);

#endif
