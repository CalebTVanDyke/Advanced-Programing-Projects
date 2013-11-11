#ifndef BOARD_H
#define BOARD_H 1

#include "cell.h"
#include <string>
#include <iostream>
#include <stdlib.h>


class board
{
	cell** cells;
	int xmin;
	int winXmin;
	int ymin;
	int winYmin;
	int xmax;
	int winXmax;
	int ymax;
	int winYmax;
	char aliveChar;
	char deadChar;
	int width;
	int height;
	std::string name;

	typedef struct
	{
		int red;
		int blue;
		int green;
		
	}color;

	color aliveColor;
	color deadColor;

public:
	board(std::string name, int xmax, int xmin, int ymax, int ymin, char aliveChar, char deadColor);
	~board();
	/**
	* Updates the cells one generation
	**/
	void updateOne();
	/**
	* Updates the cells N generations
	**/
	void updateN(int n);
	/**
	* Sets the cell located at x, y to alive (x, y are in cartesian coordinates)
	**/
	inline void setAlive(int x, int y) { cells[computeY(y)][computeX(x)].setAlive(true); }
	/**
	* Returns the cell at x, y (x, y are in cartesian coordinates)
	**/
	inline cell getCell(int x, int y) { return cells[computeY(y)][computeX(x)]; }
	inline std::string getName() { return name; }
	inline void setName(std::string name) {this->name = name;}
	/**
	* Sets the AliveColor to the r, g, b combination
	**/
	void setAliveColor(int r, int g, int b);
	/**
	* Sets the DeadColor to the r, g, b combination
	**/
	void setDeadColor(int r, int g, int b);
	/**
	*	Returns a color struct representing the alive color with the following elements: red, green, blue
	**/
	inline color getAliveColor() { return aliveColor; }
	/**
	*	Returns a color struct representing the dead color with the following elements: red, green, blue
	**/
	inline color getDeadColor() { return deadColor; }
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
	/**
	*	Returns a visual representation of the board in string format
	**/
	std::string toString();
	/**
	*	Returns a string containing the board converted to a life file
	**/
	std::string toFile();
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
	void updateTerrain(int xhigh, int xlow, int yhigh, int ylow);
	int getWinHeight();
	int getWinWidth();
private:
	/**
	*	Initializes the cells of the board to contain the x, y coordinate they are pointing too
	**/
	void initializeCells();

private:
	/**
	*	Frees all cells of the board called by the destructor and whenever the terrain is updated to new dimensions
	**/
	void freeCells();
};

#endif