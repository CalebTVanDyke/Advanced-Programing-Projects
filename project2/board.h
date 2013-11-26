#ifndef BOARD_H
#define BOARD_H 1

#include "cell.h"
#include <string>
#include <iostream>
#include <stdlib.h>


class board
{
protected:
	int xmin;
	int winXmin;
	int ymin;
	int winYmin;
	int xmax;
	int winXmax;
	int ymax;
	int winYmax;
	int width;
	int height;
	std::string name;
	typedef struct
	{
		int red;
		int blue;
		int green;
		
	}color;

public:
	board(std::string name, int xmax, int xmin, int ymax, int ymin);
	virtual ~board();

	virtual void updateOne() = 0;
	/**
	* Updates the cells N generations
	**/
	virtual void updateN(int n);
	inline std::string getName() { return name; }
	inline void setName(std::string name) {this->name = name;}
	void setWinHeight(int ymax, int ymin);
	void setWinWidth(int xmax, int xmin);
	inline int getXMin(){ return xmin; }
	inline int getYMin(){ return ymin; }
	inline int getXMax(){ return xmax; }
	inline int getYMax(){ return ymax; }
	inline void setXMin(int x){ xmin = x; }
	inline void setYMin(int y){ ymin = y; }
	inline void setXMax(int x){ xmax = x; }
	inline void setYMax(int y){ ymax = y; }
	inline int getWinXMin(){ return winXmin; }
	inline int getWinYMin(){ return winYmin; }
	inline int getWinXMax(){ return winXmax; }
	inline int getWinYMax(){ return winYmax; }
	inline int getHeight(){ return height; }
	inline int getWidth(){ return width; }
	
	/**
	*	Returns a visual representation of the board in string format
	**/
	virtual std::string toString() = 0;
	/**
	*	Returns a string containing the board converted to a life file
	**/
	virtual std::string toFile() = 0;
	/**
	*	Takes in a x in cartesian coordinates and converts it into an index into the array
	**/
	inline int computeX(int x){ return x - xmin; }
	/**
	*	Takes in a y in cartesian coordinates and converts it into an index into the array
	**/
	inline int computeY(int y){ return abs(y - ymax); }
	/**
	*	Updates the terrain to match with the new coordinate system
	**/
	virtual void updateTerrain(int xhigh, int xlow, int yhigh, int ylow) = 0;
	int getWinHeight();
	int getWinWidth();
private:
	/**
	*	Initializes the cells of the board to contain the x, y coordinate they are pointing too
	**/
	virtual void initializeCells() = 0;
};

#endif