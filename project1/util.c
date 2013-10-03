#include <stdio.h>

char* findDuration(int sampleRate, int channels, int samples, char timeStr[]){
	float seconds = 1 /(sampleRate / (float)samples);
	int minutes = seconds / 60;
	seconds -= minutes * 60;
	int hours = minutes / 60;
	minutes -= hours * 60;
	sprintf(timeStr, "%d:%02d:%05.2f", hours, minutes, seconds);
	return timeStr;
}

char* flipBytes(char* rev, int size){
	int i = 0;
	for(i = 0; i < size / 2; i++){
		char temp = rev[i];
		rev[i] = rev[size - i - 1];
		rev[size - i - 1] = temp;
	}
	return rev;
}
int trim(char* fileName, int size){
	int i;
	for(i = 0; i < size; i++){
		if('\n' == fileName[i]){
			fileName[i] = '\0';
			return i;
		}
	}
	return i;
}
void rewrite(FILE* outf, FILE* inf){
	char c;
	while((c = fgetc(inf)) != EOF){
		fputc(c, outf);
	}
}