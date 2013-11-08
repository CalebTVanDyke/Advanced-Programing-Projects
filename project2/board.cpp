#include "board.h"
#include <vector>
#include <iostream>
#include <sstream>

board::board(std::string name, int xmax, int xmin, int ymax, int ymin, char alive, char dead){
	this->name = name;
	aliveChar = alive;
	deadChar = dead;
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
	cells = new cell*[height];
	for(int i = 0; i < height; ++i){
		cells[i] = new cell[width];
	}
	initializeCells();
}
board::~board(){
	freeCells();
}
void board::updateOne(){
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
                rightI = 0;
            }else if(i == 0){
                leftI = height - 1;
            }
            if(j == width - 1){
                downJ = 0;
            }else if(j == 0){
                upJ = width - 1;
            }
            if(cells[leftI][j].isAlive()){
                neighbors++;
            }
            if(cells[rightI][j].isAlive()){
                neighbors++;
            }
            if(cells[i][upJ].isAlive()){
                neighbors++;
            }
            if(cells[i][downJ].isAlive()){
                neighbors++;
            }
            if(cells[leftI][upJ].isAlive()){
                neighbors++;
            }
            if(cells[leftI][downJ].isAlive()) {
                neighbors++;
            }
            if(cells[rightI][downJ].isAlive()){
                neighbors++;
            }
            if(cells[rightI][upJ].isAlive()){
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
void board::updateN(int n){
	for(int i = 0; i < n; ++i){
		updateOne();
	}
}
void board::setAliveColor(int r, int g, int b){
	aliveColor.red = r;
	aliveColor.blue = b;
	aliveColor.green = g;
}
void board::setDeadColor(int r, int g, int b){
	deadColor.red = r;
	deadColor.blue = b;
	deadColor.green = g;
}
std::string board::toString(){
	std::string theBoard;
	theBoard.resize((abs(winYmax - winYmin) + 1) * (abs(winXmax - winXmin) + 2 ));
	int si = 0;
	for(int i = computeY(winYmax); i <= computeY(winYmin); ++i){
		for(int j = computeX(winXmin); j <= computeX(winXmax); ++j){
			if(cells[i][j].isAlive()){
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
std::string board::toFile(){
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
void board::updateTerrain(int xhigh, int xlow, int yhigh, int ylow){

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
	if(winXmin < xlow){
		winXmin = xlow;
	}
	if(winXmax > xhigh){
		winXmax = xhigh;
	}
	if(winYmin < ylow){
		winYmin = ylow;
	}
	if(winYmax > yhigh){
		winYmax = yhigh;
	}
	initializeCells();
}
void board::freeCells(){
	for(int i = 0; i < height; ++i){
		delete [] cells[i];
	}
	delete [] cells;
}
void board::printArray(){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(cells[i][j].isAlive()){
				std::cout << aliveChar;
			}else{
				std::cout << deadChar;
			}
		}
		std::cout << '\n';
	}
}
void board::initializeCells(){
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
void board::setWinWidth(int wxmax, int wxmin){
	if(wxmax <= xmax){
		winXmax = wxmax;
	}else{
		std::cerr << "New window X max value was out of range of the terrain, max terrain X value will be used\n";
		winXmax = xmax;
	}
	if(wxmin >= xmin){
		winXmin = wxmin;
	}else{
		std::cerr << "New window Y min value was out of range of the terrain, min terrain X value will be used\n";
		winXmin = xmin;
	}
}
void board::setWinHeight(int wymax, int wymin){
	if(wymax <= ymax){
		winYmax = wymax;
	}else{
		std::cerr << "New window Y max value was out of range of the terrain, max terrain Y value will be used\n";
		winYmax = ymax;
	}
	if(wymax >= ymin){
		winYmin = wymin;
	}else{
		std::cerr << "New window Y min value was out of range of the terrain, min terrain Y value will be used\n";
		winYmin = ymin;
	}
}