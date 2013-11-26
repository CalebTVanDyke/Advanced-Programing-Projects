#include "wire_board.h"

wire_board::wire_board(std::string name, int xmax, int xmin, int ymax, int ymin, char empty, char head, char tail, char wire)
: board(name, xmax, xmin, ymax, ymin)
{
	emptyChar = empty;
	headChar = head;
	tailChar = tail;
	wireChar = wire;

	cells = new wire_cell*[height];
	for(int i = 0; i < height; ++i){
		cells[i] = new wire_cell[width];
	}
	initializeCells();
}
wire_board::~wire_board(){
	freeCells();
}
void wire_board::updateOne(){

}
std::string wire_board::toString(){
	std::string theBoard;
	theBoard.resize((abs(winYmax - winYmin) + 1) * (abs(winXmax - winXmin) + 2 ));
	int si = 0;
	for(int i = winYmax; i >= winYmin; --i){
		for(int j = winXmin; j <= winXmax; ++j){
			if(i < ymin || i > ymax || j < xmin || j > xmax){
				theBoard[si] = emptyChar;
			}
			else if(cells[computeY(i)][computeX(j)].getState() == wire_cell::HEAD){
				theBoard[si] = headChar; 
			}else{
				theBoard[si] = emptyChar;
			}
			++si;
		}
		theBoard[si] = '\n';
		++si;
	}

	return theBoard;
}
std::string wire_board::toFile(){
	return "";
}
void wire_board::updateTerrain(int xhigh, int xlow, int yhigh, int ylow){

}
void wire_board::initializeCells(){
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
void wire_board::setHeadColor(int r, int g, int b){
	headColor.red = r;
	headColor.blue = b;
	headColor.green = g;
}
void wire_board::setEmptyColor(int r, int g, int b){
	emptyColor.red = r;
	emptyColor.blue = b;
	emptyColor.green = g;
}
void wire_board::setTailColor(int r, int g, int b){
	tailColor.red = r;
	tailColor.blue = b;
	tailColor.green = g;
}
void wire_board::setWireColor(int r, int g, int b){
	wireColor.red = r;
	wireColor.blue = b;
	wireColor.green = g;
}
void wire_board::freeCells(){
	for(int i = 0; i < height; ++i){
		delete [] cells[i];
	}
	delete [] cells;
}