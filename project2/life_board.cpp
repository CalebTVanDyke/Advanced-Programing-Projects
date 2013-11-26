#include "life_board.h"
#include <vector>
#include <iostream>
#include <sstream>

life_board::life_board(std::string name, int xmax, int xmin, int ymax, int ymin, char aliveChar, char deadChar)
: board(name, xmax, xmin, ymax, ymin)
{
	this->aliveChar = aliveChar;
	this->deadChar = deadChar;
	cells = new cell*[height];
	for(int i = 0; i < height; ++i){
		cells[i] = new cell[width];
	}
	initializeCells();
}
life_board::~life_board(){
	freeCells();
}
void life_board::updateOne(){
	int neighbors;
	std::vector<std::pair<int, int> > update;
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			neighbors = 0;
            int leftI = i - 1;
            int rightI = i + 1;
            int downJ = j + 1;
            int upJ = j - 1;
            if (i == height -1) {
                rightI = -1;
            }else if(i == 0){
                leftI = -1;
            }
            if(j == width - 1){
                downJ = -1;
            }else if(j == 0){
                upJ = -1;
            }
            if(-1 != leftI && cells[leftI][j].isAlive()){
                neighbors++;
            }
            if(-1 != rightI && cells[rightI][j].isAlive()){
                neighbors++;
            }
            if(-1 != upJ && cells[i][upJ].isAlive() ){
                neighbors++;
            }
            if(-1 != downJ && cells[i][downJ].isAlive()){
                neighbors++;
            }
            if(-1 != leftI && -1 != upJ && cells[leftI][upJ].isAlive()){
                neighbors++;
            }
            if(-1 != leftI && -1 != downJ && cells[leftI][downJ].isAlive()) {
                neighbors++;
            }
            if(-1 != rightI && -1 != downJ && cells[rightI][downJ].isAlive()){
                neighbors++;
            }
            if(-1 != rightI && -1 != upJ && cells[rightI][upJ].isAlive()){
                neighbors++;
            }
            if(cells[i][j].isAlive()) {
                if(neighbors < 2){
                    std::pair<int, int> pair;
                    pair.first = i;
                    pair.second = j;
                    update.push_back(pair);
                }else if(neighbors > 3){
                    std::pair<int, int> pair;
                    pair.first = i;
                    pair.second = j;
                    update.push_back(pair);
                }
                
            }
            else{
                if (neighbors == 3) {
                    std::pair<int, int> pair;
                    pair.first = i;
                    pair.second = j;
                    update.push_back(pair);
                }
            }
		}
	}
	while(update.size() > 0){
        std::pair<int, int> pair = update.back();
        update.pop_back();
        cells[pair.first][pair.second].switchState();
    }
}
std::string life_board::toString(){
	std::string theBoard;
	theBoard.resize((abs(winYmax - winYmin) + 1) * (abs(winXmax - winXmin) + 2 ));
	int si = 0;
	for(int i = winYmax; i >= winYmin; --i){
		for(int j = winXmin; j <= winXmax; ++j){
			if(i < ymin || i > ymax || j < xmin || j > xmax){
				theBoard[si] = deadChar;
			}
			else if(cells[computeY(i)][computeX(j)].isAlive()){
				theBoard[si] = aliveChar; 
			}else{
				theBoard[si] = deadChar;
			}
			++si;
		}
		theBoard[si] = '\n';
		++si;
	}

	return theBoard;
}
std::string life_board::toFile(){
	std::stringstream sstm;
	sstm << "Life={";
	if("" != name){
		sstm << "Name=\"" << name << "\";";
	}
	sstm << "Terrain={Xrange=" << xmin << ".." << xmax << ";";
	sstm << "Yrange=" << ymin << ".." << ymax << ";};";

	sstm << "Window={Xrange=" << winXmin << ".." << winXmax << ";";
	sstm << "Yrange={" << winYmin << ".." << winYmax << ";};";

	sstm << "Chars={Alive=" << (int) aliveChar << ";Dead=" << (int) deadChar << ";};";

	sstm << "Colors={Alive=(" << aliveColor.red << "," << aliveColor.green << "," << aliveColor.blue << ");";
	sstm << "Dead=(" << deadColor.red << "," << deadColor.green << "," << deadColor.blue << ");};";

	bool printComma = false;

	sstm << "Initial={";
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			if(printComma){
				sstm << ",";
			}
			if(cells[i][j].isAlive()){
				sstm << "(" << cells[i][j].getX() << "," << cells[i][j].getY() << ")";
			}
		}
	}
	sstm << ";};};";

	return sstm.str();
}
void life_board::updateTerrain(int xhigh, int xlow, int yhigh, int ylow){

	cell **newCells = new cell*[yhigh - ylow + 1];
	for (int i = 0; i < yhigh - ylow + 1; ++i){
		newCells[i] = new cell[xhigh - xlow + 1];
	}
	xmax = xhigh;
	xmin = xlow;
	ymax = yhigh;
	ymin = ylow;

	for (int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			cell curr = cells[i][j];
			if(curr.isAlive()){
				if(curr.getX() >= xlow && curr.getX() <= xhigh &&
						curr.getY() >= ylow && curr.getY() <= yhigh){

					newCells[computeY(curr.getY())][computeX(curr.getX())].setAlive(true);
				}
			}
		}
	}
	freeCells();
	cells = newCells;
	width = xhigh - xlow + 1;
	height = yhigh - ylow + 1;
	winXmin = xlow;
	winXmax = xhigh;
	winYmin = ylow;
	winYmax = yhigh;
	initializeCells();
}
void life_board::initializeCells(){
	int x = xmin;
	int y = ymax;
	for(int i = 0; i < height; i++){
		x = xmin;
		for(int j = 0;j < width; j++){
			cells[i][j].setXY(x, y);
			x++;
		}
		y--;
	}
}
void life_board::setAliveColor(int r, int g, int b){
	aliveColor.red = r;
	aliveColor.blue = b;
	aliveColor.green = g;
}
void life_board::setDeadColor(int r, int g, int b){
	deadColor.red = r;
	deadColor.blue = b;
	deadColor.green = g;
}
void life_board::freeCells(){
	for(int i = 0; i < height; ++i){
		delete [] cells[i];
	}
	delete [] cells;
}