#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H 1

#include "board.h"
#include <string>

board stringToBoard(std::string fileInfo);
std::string getStringFromFile(const char *filename);
std::string trimQuotes(std::string toTrim);
std::string compress(std::string data);
std::string getStringFromStdIn();

#endif