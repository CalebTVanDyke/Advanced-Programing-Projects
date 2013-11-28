#ifndef LIFE_BOARD_H
#define LIFE_BOARD_H 1

#include "cell.h"
#include "board.h"
#include <string>
#include <iostream>
#include <stdlib.h>


class life_board : public board
{
protected:
	char aliveChar;
	char deadChar;
	color aliveColor;
	color deadColor;
	cell** cells;

public:
	life_board(std::string name, int xmax, int xmin, int ymax, int ymin, char aliveChar, char deadChar);
	life_board(const board& b);
	virtual ~life_board();

	virtual board* Clone();

	void operator=(life_board b);
	/**
	* Updates the cells one generation
	**/
	virtual void updateOne();
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
	inline char getAliveChar(){ return aliveChar; }
	inline char getDeadChar(){ return deadChar; }
	/**
    * Sets the cell located at x, y to alive (x, y are in cartesian coordinates)
    **/
    inline void setAlive(int x, int y) { cells[computeY(y)][computeX(x)].setAlive(true); }
	
	/**
	*	Returns a visual representation of the board in string format
	**/
	virtual std::string toString();
	/**
	*	Returns a string containing the board converted to a life file
	**/
	virtual std::string toFile();
	/**
	*	Updates the terrain to match with the new coordinate system
	**/
	virtual void updateTerrain(int xhigh, int xlow, int yhigh, int ylow);

	inline cell getCell(int x, int y) { return cells[computeY(y)][computeX(x)]; }
protected:
	/**
	*	Initializes the cells of the board to contain the x, y coordinate they are pointing too
	**/
	virtual void initializeCells();
	/**
	*	Frees all cells of the board called by the destructor and whenever the terrain is updated to new dimensions
	**/
	void freeCells();
};


#endif