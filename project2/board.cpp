#include "board.h"
#include <vector>
#include <iostream>
#include <sstream>

board::board(std::string name, int xmax, int xmin, int ymax, int ymin){
	this->name = name;
	this->xmin = xmin;
	winXmin = xmin;
	this->ymin = ymin;
	winYmin = ymin;
	this->xmax = xmax;
	winXmax = xmax;
	this->ymax = ymax;
	winYmax = ymax;
	width = abs(xmax - xmin) + 1;
	height = abs(ymax - ymin) + 1;
}
board::~board(){
}
void board::updateN(int n){
	for(int i = 0; i < n; ++i){
		updateOne();
	}
}
void board::setWinWidth(int wxmax, int wxmin){
	//if(wxmax <= xmax){
		winXmax = wxmax;
/*	}else{
		std::cerr << "New window X max value was out of range of the terrain, max terrain X value will be used\n";
		winXmax = xmax;
	}
	if(wxmin >= xmin){*/
		winXmin = wxmin;
/*	}else{
		std::cerr << "New window Y min value was out of range of the terrain, min terrain X value will be used\n";
		winXmin = xmin;
	}*/
}
void board::setWinHeight(int wymax, int wymin){
	//if(wymax <= ymax){
		winYmax = wymax;
	/*}else{
		std::cerr << "New window Y max value was out of range of the terrain, max terrain Y value will be used\n";
		winYmax = ymax;
	}
	if(wymin >= ymin){*/
		winYmin = wymin;
	/*}else{
		std::cerr << "New window Y min value was out of range of the terrain, min terrain Y value will be used\n";
		winYmin = ymin;
	}*/
}
int board::getWinHeight(){
	return computeY(winYmin) - computeY(winYmax) + 1;
}
int board::getWinWidth(){
	return computeX(winXmax) - computeX(winXmin) + 1;
}