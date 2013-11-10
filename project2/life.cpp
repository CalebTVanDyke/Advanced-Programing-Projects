#include <iostream>
#include <string>
#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "filehandling.h"
#include <ctype.h>
#include <string.h>

using namespace std;

void displayHelp();

int main(int argc, char *argv[])
{
	int err = 0;
	int generations = 0;
	int printVisual = 1;
	int fromFile = 0;

	for (int i = 1; i < argc; ++i)
	{
		if('-' == argv[i][0]){
			int len = strlen(argv[i]);
			for (int j = 1; j < len; ++j)
			{
				if('f' == argv[i][j]){
					printVisual = 0;
				}
				else if('h' == argv[i][j]){
					displayHelp();
					return 0;
				}
				else if('v' == argv[i][j]){
					printVisual = 1;
				}
			}
		}else{
			if(i + 1 == argc){
				fromFile = 1;
			}
		}
	}

	try{
		string fileInfo = "";
		if(fromFile){
			fileInfo = getStringFromFile(argv[argc - 1]);
		}else{
			fileInfo = getStringFromStdIn();
		}
		board gameBoard = stringToBoard(fileInfo);
		updateBoardWithCommands(&gameBoard, argc, argv);

		gameBoard.updateN(generations);
		if(printVisual)
			cout << gameBoard.toString();
		else
			cout << gameBoard.toFile();

	}catch(int e){
		return -1;
	}
	return 0;
}
void displayHelp(){
	cout << "life\n";
	cout << "----\n";
	cout << "Life takes in a .life file or a file of a specific format, and can display the output in a visual form or text form.\n\n";

	cout << "life [-h] [-g n] [-f] [-v] [-tx low..high] [-ty low..high] [-wy low..high] [-wx low..high] [file]\n\n";
	cout << "Options:\n";
	cout << "-h              displays a help screen\n";
	cout << "-g n            advances the life file to the n-th generation\n";
	cout << "-v              output will be in a visual form\n";
	cout << "-f              output will be in the form of the file\n";
	cout << "-tx low..high   changes the terrain x values in the file to the values specified by low..high\n";
	cout << "                if the window is not specified in the command line it will be set to the whole terrain\n";
	cout << "-ty low..high   changes the terrain y values in the file to the values specified by low..high\n";
	cout << "                if the window is not specified in the command line it will be set to the whole terrain\n";
	cout << "-wx low..high   changes the window x values in the file to the values specified by low..high\n";
	cout << "-wy low..high   changes the window y values in the file to the values specified by low..high\n";
	cout << "file            this will be the file that is read\n";
	cout << "                if ommited file will be read from standard input\n";
}