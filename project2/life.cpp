#include <iostream>
#include <string>
#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "filehandling.h"
#include <ctype.h>

using namespace std;

void displayHelp();

int main(int argc, char const *argv[])
{
	int err = 0;
	int generations = 0;
	int printVisual = 1;
	int updateWin = 0;
	int updateTer = 0;
	int tyIndex = -1;
	int txIndex = -1;
	int wyIndex = -1;
	int wxIndex = -1;
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
				else if('g' == argv[i][j]){
					if(i + 1 < argc){
						++i;
						if(!isdigit(argv[i][0])){
							cerr << "Not a valid digit after -g for generations. Default of 0 will be used.\n";
						}else{
							generations = atoi(argv[i]);
						}
					}else{
						cerr << "Value needs to be specified after -g.\n";
					}
					break;
				}
				else if('h' == argv[i][j]){
					displayHelp();
					return 0;
				}
				else if('t' == argv[i][j]){

					if(j + 1 < len && 'y' == argv[i][j + 1]){
						tyIndex = ++i;
						updateTer = 1;
					}
					else if(j + 1 < len && 'x' == argv[i][j + 1]){
						txIndex = ++i;
						updateTer = 1;
					}
					break;
				}
				else if('w' == argv[i][j]){
					if(j + 1 < len && 'y' == argv[i][j + 1]){
						wyIndex = ++i;
						updateWin = 1;
					}
					else if(j + 1 < len && 'x' == argv[i][j + 1]){
						wxIndex = ++i;
						updateWin = 1;
					}
					break;
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

		int xl = gameBoard.getXMin(), xh = gameBoard.getXMax();
		int yl = gameBoard.getYMin(), yh = gameBoard.getYMax();
		if(updateTer){
			if(-1 != txIndex){
				err = sscanf(argv[txIndex], "%d..%d", &xl, &xh);
				if(2 != err || xl > xh){
					cerr << "Values not correct after -tx will use terrian dimensions from file\n";
					xl = gameBoard.getXMin();
					xh = gameBoard.getXMax();
				}
			}			
			if(-1 != tyIndex){
				err = sscanf(argv[tyIndex], "%d..%d", &yl, &yh);				
				if(2 != err || yl > yh){
					cerr << "Values not correct after -ty will use terrian dimensions from file\n";
					yl = gameBoard.getYMin();
					yh = gameBoard.getYMax();
				}
			}
			gameBoard.updateTerrain(xh, xl, yh, yl);
		}
		if(updateWin){

			if(-1 != wxIndex){
				err = sscanf(argv[wxIndex], "%d..%d", &xl, &xh);
				if(2 != err || xl > xh){
					cerr << "Values not correct after -wx will use window dimensions from file\n";
				}else{
					gameBoard.setWinWidth(xh, xl);
				}
			}
			if(-1 != wyIndex){
				err = sscanf(argv[wyIndex], "%d..%d", &yl, &yh);
				if(2 != err || yl > yh){
					cerr << "Values not correct after -wy will use window dimensions from file\n";
				}else{
					gameBoard.setWinHeight(yh, yl);
				}
			}
		}
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
	printf("help\n");
}