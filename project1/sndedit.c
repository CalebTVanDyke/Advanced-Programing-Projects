#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include "util.h"
#include "AIFF.h"
#include "CS229.h"
#include <string.h>

void printHelp(int markon, int modified, int buffered, File_Data data, int marked, int bufferSize);
void clearExtra(int rows, int width);
void markAll(File_Data data, int topMark, int botMark, int topSamp, int botSamp, int topChan, int botChan);
void copy(File_Data data, int** buffer, int firstMark, int lastMark);
void cut(File_Data *data, int firstMark, int lastMark);
void paste(File_Data *data, int** buffer, int bufferSize,int startSample, int origSamples);

int main(int argc, char const *argv[])
{

	if(argc <= 1){
		fprintf(stderr, "Please specify a file you wish to edit\n");
		return;
	}
	FILE* file = fopen(argv[1], "r");

	if(!file){
		fprintf(stderr, "File %s could not be opened.\n", argv[1]);
		return;
	}

	char format[5];
	int rows, cols;
	int markon = 0, buffered = 0, modified = 0;
	int firstMark = -1, lastMark = -1;
	int **buffer;
	int bufferSize = 0;
	int origSamples;
	int isCS229 = 0;

	initscr();

	getmaxyx(stdscr, rows, cols);

	int width = cols - 20;

	noecho();
	keypad(stdscr, 1);

	int i;

	File_Data data;

	move(0, cols/2 - strlen(argv[1])/2);
	printw("%s", argv[1]);
	move(1, 0);

	for(i = 0; i < cols; i++){
		printw("=");
	}

	getFileType(file, format);

	if(strncmp(format, "FORM", 4) == 0){
		data = showAIFF(file, (char*)argv[1], width, 1, -1, USE_NCURSES);
	}
	else if(strncmp(format, "CS229", 5) == 0){
		isCS229 = 1;
		data = showCS229(file, width, 1, -1, USE_NCURSES);
	}else{

		fprintf(stderr, "File was not a CS229 or AIFF file in the correct format\n");
		return;
	}
	findDuration(data.sampleRate, data.channels, data.samples, data.duration);
	origSamples = data.samples;
	int topSamp = 0;
	int topChan = 0;
	int botSamp = (rows - 3) / data.channels;
	int botChan = (rows - 3) % data.channels;
	if(botSamp >= data.samples - 1){
		botSamp = data.samples - 1;
		botChan = data.channels - 1;
	}

 	printHelp(markon, modified, buffered, data, firstMark, bufferSize);


 	move(2, ((cols-20)/2+9)-5);
 	int c;
	while( (c = getch()) != 'q'){
		int prevX, prevY;
		getyx(stdscr, prevY, prevX);
		int currSamp = topSamp + ((prevY - 2 + topChan)/data.channels);
		int currChan = (prevY - 2) % data.channels + topChan;
		if(c == 'm'){
			if(!markon){
				markon = 1;
				firstMark = currSamp;
				lastMark = currSamp;
			}
			else{
				markon = 0;
				move(2, 0);
				showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
				firstMark = -1;
				lastMark = -1;
			}
		}
		else if(c == 's' && modified){
			FILE* saveFile = fopen(argv[1], "w");
			if(!saveFile){
				move(rows - 1, 0);
				printw("File could not be saved");
				continue;
			}
			if(isCS229){
				writeHeaderCS229(saveFile, data);
				writeSamplesCS229(saveFile, data);
			}else{
				writeHeaderAIFF(saveFile, data);
				setupSoundAIFF(saveFile, data);
				writeSamplesAIFF(saveFile, data);
			}
			modified = 0;
		}
		else if((c == 'c' || c == 'x') && markon){
			buffered = 1;
			if(bufferSize > 0){
				for(i = 0; i < bufferSize; i++){
					free(buffer[i]);
				}
				free(buffer);
			}
			bufferSize = abs(lastMark - firstMark) + 1;
			buffer = malloc(bufferSize * sizeof(int*));
			for(i = 0; i < bufferSize; i++){
				buffer[i] = malloc(data.channels * sizeof(int));
			}
			copy(data, buffer, firstMark, lastMark);
			if(c =='x'){
				modified = 1;
				cut(&data, firstMark, lastMark);
				move(2, 0);
				showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);	
				markon = 0;
				firstMark = -1;
				lastMark = -1;
				clearExtra(rows, width);
			}
		}
		else if((c == 'v' || c == '^') && buffered){
			if(c == 'v'){
				paste(&data, buffer, bufferSize, currSamp+1, origSamples);
			}
			else if(c == '^'){
				paste(&data, buffer, bufferSize, currSamp, origSamples);
			}
			move(2, 0);
			showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);	
			modified = 1;
		}
		else if(c == 'g'){
			int newTop;
			move(rows - 1, 0);
			for(i = 0; i < width; i++){
				printw(" ");
			}
			move(rows - 1, 0);
			printw("Enter sample: ");
			move(rows - 1, 14);
			echo();
			int worked = scanw("%d", &newTop);
			noecho();
			if(worked == 1 && newTop < data.samples && newTop >= 0){
				topSamp = newTop;
				topChan = 0;
				botSamp = topSamp + (rows - 3) / data.channels;
		 		botChan = topChan + (rows - 3) % data.channels;
		 		move(2, 0);
				showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
				clearExtra(rows, width);
				printHelp(markon, modified, buffered, data, firstMark, bufferSize);
				if(markon){
					lastMark = topSamp;
				}
				prevY = 2;
				prevX = width/2 + 4;
			}else{
				move(2, 0);
				showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
			}
		}
		else if(c == KEY_DOWN){	
			if(prevY == rows -1){
				if(botSamp != data.samples - 1 || botChan != data.channels - 1){
					topChan++;
					botChan++;
					if(botChan >= data.channels){
						botChan = 0;
						botSamp++;
						if(botSamp > lastMark)
							lastMark = botSamp;
					}
					if(topChan >= data.channels){
						topChan = 0;
						topSamp++;
					}
					move(2, 0);
					currSamp = topSamp + ((prevY - 2 + topChan)/data.channels);
					showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
				}
			}else{
				if(currSamp >= data.samples - 1 && currChan >= data.channels - 1){
					continue;
				}
				move(++prevY, prevX);
				currSamp = topSamp + ((prevY - 2 + topChan)/data.channels);
			}
			if(markon){
				lastMark = currSamp;
			}
		} 
		else if(c == KEY_UP){
			if(prevY == 2){
				if(topSamp != 0 || topChan != 0){
					topChan--;
					botChan--;
					if(botChan < 0){
						botChan = data.channels - 1;
						botSamp--;
					}
					if(topChan < 0){
						topChan = data.channels - 1;
						topSamp--;
					}
					move(2, 0);
					currSamp = topSamp + ((prevY - 2 + topChan)/data.channels);
					showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
				}
			}else{
				move(--prevY, prevX);
				currSamp = topSamp + ((prevY - 2 + topChan)/data.channels);
			}
			if(markon){
				lastMark = currSamp;
			}
		}
		else if(c == KEY_PPAGE){
			prevY = 2;
 			prevX = ((cols-20)/2+9)-5;
			botSamp = topSamp;
			topSamp = botSamp - (rows - 3) / data.channels;
			if(topSamp < 0){
				topSamp = 0;
				botSamp = topSamp + (rows - 3) / data.channels;
			}
			if(markon)
				lastMark = topSamp;
			move(2, 0);
			showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
		}
		else if(c == KEY_NPAGE){
 			prevY = 2;
 			prevX = ((cols-20)/2+9)-5;
			topSamp = botSamp;
			botSamp = topSamp + (rows - 3) / data.channels;
			if(botSamp >= data.samples){
				botSamp = data.samples - 1;
				topSamp = botSamp - (rows - 3) / data.channels;
			}
			if(markon)
				lastMark = topSamp;
			move(2, 0);
			showSamplesRange(data, width, 1, -1, USE_NCURSES, topSamp, botSamp, topChan, botChan);
		}
		printHelp(markon, modified, buffered, data, firstMark, bufferSize);
		markAll(data, firstMark, lastMark, topSamp, botSamp, topChan, botChan);
		move(prevY, prevX);
		refresh();
	}

	endwin();
	return 0;
}
void printHelp(int markon, int modified, int buffered, File_Data data, int firstMark, int bufferSize){
	int i, rows, cols;
	getmaxyx(stdscr, rows, cols);
	move(2, cols - 20);
	printw(" Sample Rate: %d\n", data.sampleRate);
	move(3, cols-20);
	printw("   Bit Depth: %d\n", data.bitDepth);
	move(4, cols-20);
	printw("    Channels: %d\n", data.channels);
	move(5, cols-20);
	printw(" Samples: %d\n", data.samples);
	move(6, cols-20);
	printw("  Length: %s", data.duration);
	move(7, cols-20);
	for(i = 0; i < 20; i++){
		printw("=");
	}
	move(8, cols-20);
	if(markon)
		printw("   m: unmark");
	else
		printw("   m: mark  ");

	move(9, cols-20);
	if(markon)
		printw("   c: copy");
	else
		printw("          ");
	move(10, cols-20);
	if(markon)
		printw("   x: cut");
	else
		printw("         ");
	move(11, cols-20);
	if(buffered)
		printw("   ^: insert before");
	else
		printw("                   ");
	move(12, cols-20);
	if(buffered)
		printw("   v: insert after");
	else
		printw("                  ");
	move(13, cols-20);
	if(modified)
		printw("   s: save");
	else
		printw("          ");
	move(14, cols-20);
	printw("   q: quit");


	move(16, cols-20);
	printw(" Movement:");
	move(17, cols-20);
	printw("   up/dn");
	move(18, cols-20);
	printw("   pgup/pgdwn");
	move(19, cols-20);
	printw("   g: goto sample");
	move(rows - 3, cols-20);
	for(i = 0; i < 20; i++){
		printw("=");
	}
	move(rows - 2, cols-20);
	if(markon)
		printw("   Marked: %d", firstMark);
	else
		printw("                     ");
	move(rows - 1, cols-20);
	if(buffered)
		printw(" Buffered: %d", bufferSize);
	else
		printw("                     ");
}
void clearExtra(int rows, int width){
	int y, x, i;
	getyx(stdscr, y, x);
	if(y == rows - 1)
		return;
	move(y , 0);
	while(y < rows){
		for(i = 0; i < width; i++){
			printw(" ");
		}
		move(++y, 0);
	}
}
void markAll(File_Data data, int topMark, int botMark, int topSamp, int botSamp, int topChan, int botChan){

	move(2, 0);
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	int y = 2;
	int currSamp = topSamp + ((y - 2)/data.channels);;
	if(topMark == -1 || botMark == -1){
		return;
	}
	if(topMark > botMark){
		int temp = topMark;
		topMark = botMark;
		botMark = temp;
	}
	while(y < rows){
		if(topChan % data.channels == 0 && topChan != 0){
			topChan = 0;
			currSamp++;
		}
		if(currSamp >= topMark && currSamp <= botMark){
			mvchgat(y, 0, cols-21, A_REVERSE, 0, NULL);
		}else{
			mvchgat(y, 0, cols-21, A_NORMAL, 0, NULL);
		}
		y++;
		topChan++;
		move(y, 0);
	}	
}
void copy(File_Data data, int** buffer, int firstMark, int lastMark){

	if(firstMark > lastMark){
		int temp = firstMark;
		firstMark = lastMark;
		lastMark = temp;
	}
	int i, j, k = 0;
	for(i = firstMark; i <= lastMark && i < data.samples; i++){
		for(j = 0; j < data.channels; j++){
			buffer[k][j] = data.sampleData[i][j];
		}
		k++;
	}
}
void paste(File_Data *data, int** buffer, int bufferSize,int startSample, int origSamples){
	int i, j, oldSamp = 0, bufferSamp = 0, usedBuffer = 0;
	data->samples += bufferSize;
	int **newSamples = malloc(data->samples * sizeof(int*));
	for(i = 0; i < data->samples; i++){
		newSamples[i] = malloc(data->channels * sizeof(int));
	}

	for(i = 0; i < data->samples; i++){
		for(j = 0; j < data->channels; j++){
			if(i >= startSample && bufferSamp < bufferSize){
				newSamples[i][j] = buffer[bufferSamp][j];
				usedBuffer = 1;
			}else{
				newSamples[i][j] = data->sampleData[oldSamp][j];
				usedBuffer = 0;
			}
		}
		if(usedBuffer){
			bufferSamp++;
		}else{
			oldSamp++;
		}

	}

	for(i = 0; i < origSamples; i++){
		free(data->sampleData[i]);
	}
	free(data->sampleData);
	data->sampleData = newSamples;
}
