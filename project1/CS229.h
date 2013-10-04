File_Data processCS229(FILE *file);
void processStartData(FILE* file, File_Data data);
void convertCS229toAIFF(FILE* outfile, FILE* infile);
File_Data CS229toTemp(FILE* outfile, FILE* infile, int addHeader);