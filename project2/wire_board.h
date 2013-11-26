#ifndef WIRE_BOARD_H
#define WIRE_BOARD_H 1

#include "cell.h"
#include "board.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include "wire_cell.h"


class wire_board : public board
{
	char headChar;
	char tailChar;
	char emptyChar;
	char wireChar;
	color headColor;
	color wireColor;
	color emptyColor;
	color tailColor;
	wire_cell** cells;

public:

	wire_board(std::string name, int xmax, int xmin, int ymax, int ymin, char empty, char head, char tail, char wire);
	wire_board(const board& b);
	virtual ~wire_board();

	void operator=(wire_board b);
	/**
	* Updates the cells one generation
	**/
	virtual void updateOne();

	void setHeadColor(int r, int g, int b);

	void setWireColor(int r, int g, int b);

	void setEmptyColor(int r, int g, int b);

	void setTailColor(int r, int g, int b);
	/**
	*	Returns a color struct representing the head color with the following elements: red, green, blue
	**/
	inline color getHeadColor() { return headColor; }
	/**
	*	Returns a color struct representing the tail color with the following elements: red, green, blue
	**/
	inline color getTailColor() { return tailColor; }
	/**
	*	Returns a color struct representing the empty color with the following elements: red, green, blue
	**/
	inline color getEmptyColor() { return emptyColor; }
	/**
	*	Returns a color struct representing the wire color with the following elements: red, green, blue
	**/
	inline color getWireColor() { return wireColor; }

	inline char getHeadChar(){ return headChar; }
	inline char getTailChar(){ return tailChar; }
	inline char getEmptyChar(){ return emptyChar; }
	inline char getWireChar(){ return wireChar; }
	/**
    * Sets the cell located at x, y to alive (x, y are in cartesian coordinates)
    **/
    inline void setCellState(int x, int y, wire_cell::CellState state) { cells[computeY(y)][computeX(x)].setState(state); }
	
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

	inline wire_cell getCell(int x, int y) { return cells[computeY(y)][computeX(x)]; }
private:
	/**
	*	Initializes the cells of the board to contain the x, y coordinate they are pointing too
	**/
	virtual void initializeCells();
protected:
	/**
	*	Frees all cells of the board called by the destructor and whenever the terrain is updated to new dimensions
	**/
	void freeCells();
};


#endif