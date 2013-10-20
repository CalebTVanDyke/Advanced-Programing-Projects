#ifndef CS229_H
#define CS229_H 1

#include "util.h"

File_Data processCS229(FILE *file);
int processStartData(FILE* outfile, FILE* infile ,File_Data * data);
void convertCS229toAIFF(FILE* outfile, FILE* infile);
File_Data CS229toTemp(FILE* outfile, FILE* infile);
void writeHeaderCS229(FILE* outfile, File_Data data);
void getSamplesCS229(FILE* inf, File_Data *data);
void trimCS229(highlow_t *highlow, int size);
File_Data showCS229(FILE* file, int width, int zoom, int chan, int curses);
int writeSamplesCS229(FILE* outf, File_Data fileData);

#endif