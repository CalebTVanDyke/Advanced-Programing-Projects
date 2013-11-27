#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H 1

#include "life_board.h"
#include "wire_board.h"
#include "ele_board.h"
#include "board.h"
#include <string>

/**
*	Takes in a string representation of a file and converts it into a board object
**/
board* stringToBoard(std::string fileInfo);
/**
*	Takes in a filename and converts it into a string
**/
std::string getStringFromFile(const char *filename);
/**
*	Trims the quotes from a string
**/
std::string trimQuotes(std::string toTrim);
/**
*	Compresses a file removing all comments and whitespace
**/
std::string compress(std::string data);
/**
*	Returns a string representation of the input from stdin
**/
std::string getStringFromStdIn();
/**
*	Updates the board given the command line arguments
**/
void updateBoardWithCommands(board * gameBoard, int argc, char *argv[]);

#endif