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
	void updateOne();
	void updateN(int n);
	inline void setAlive(int x, int y) { cells[computeY(y)][computeX(x)].setAlive(true); }
	inline cell getCell(int x, int y) { return cells[computeY(y)][computeX(x)]; }
	inline std::string getName() { return name; }
	inline void setName(std::string name) {this->name = name;}
	void setAliveColor(int r, int b, int g);
	void setDeadColor(int r, int b, int g);
	inline color getAliveColor() { return aliveColor; }
	inline color getDeadColor() { return deadColor; }
	void updateWidth(int xhigh, int xmin);
	void updateHeight(int yhigh, int ymin);
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
	std::string toString();
	std::string toFile();
	inline int computeX(int x){ return x - xmin; }
	inline int computeY(int y){ return abs(y - ymax); }
	void updateTerrain(int xhigh, int xlow, int yhigh, int ylow);
	int getWinHeight();
	int getWinWidth();
private:
	void initializeCells();

void printArray();
private:
	void freeCells();
};

#endif