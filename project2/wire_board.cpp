#include <sstream>
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
board* wire_board::Clone(){
	wire_board* b = new wire_board(name, xmax, xmin, ymax, ymin, emptyChar, headChar, tailChar, wireChar);
	b->setHeadColor(headColor.red, headColor.green, headColor.blue);
	b->setEmptyColor(emptyColor.red, emptyColor.green, emptyColor.blue);
	b->setTailColor(tailColor.red, tailColor.green, tailColor.blue);
	b->setWireColor(wireColor.red, wireColor.green, wireColor.blue);

	for (int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			b->cells[i][j].setState(cells[i][j].getState());
		}
	}
	return b;
}
void wire_board::updateOne(){
	wire_cell ** newCells;
	newCells = new wire_cell*[height];
	for(int i = 0; i < height; ++i){
		newCells[i] = new wire_cell[width];
	}
	for(int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j){
			if(cells[i][j].getState() == wire_cell::EMPTY){
				newCells[i][j].setState(wire_cell::EMPTY);
			}
			else if(cells[i][j].getState() == wire_cell::HEAD){
				newCells[i][j].setState(wire_cell::TAIL);
			}
			else if(cells[i][j].getState() == wire_cell::TAIL){
				newCells[i][j].setState(wire_cell::WIRE);
			}
			else if(cells[i][j].getState() == wire_cell::WIRE){
				if(i + 1 < height && cells[i + 1][j].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(i - 1 >= 0 && cells[i - 1][j].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(j - 1 >= 0 && cells[i][j - 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(j + 1 < width && cells[i][j + 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(i - 1 >= 0 && j - 1 >= 0 && cells[i - 1][j - 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(i + 1 < height && j - 1 >= 0 && cells[i + 1][j - 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(i - 1 >= 0 && j + 1 < width && cells[i - 1][j + 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}
				else if(i + 1 < height && j + 1 < width && cells[i + 1][j + 1].getState() == wire_cell::HEAD){
					newCells[i][j].setState(wire_cell::HEAD);
				}else{
					newCells[i][j].setState(wire_cell::WIRE);
				}
			}
		}
	}
	freeCells();
	cells = newCells;
	initializeCells();
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
			int x = computeX(j);
			int y = computeY(i);
  			if(cells[y][x].getState() == wire_cell::HEAD){
				theBoard[si] = headChar; 
			}
			else if(cells[y][x].getState() == wire_cell::TAIL){
				theBoard[si] = tailChar; 
			}
			else if(cells[y][x].getState() == wire_cell::WIRE){
				theBoard[si] = wireChar; 
			}else{
				theBoard[si] = emptyChar;
			}
			++si;
			if(y >= height){
				std::cout << "y out of bounds " << y << '\n';
			}
			if(x >= width){
				std::cout << "x out of bounds " << x << '\n';
			}
		}
		theBoard[si] = '\n';
		++si;
	}

	return theBoard;
}
std::string wire_board::toFile(){
	std::stringstream sstm;
	sstm << "WireWorld={";
	if("" != name){
		sstm << "Name=\"" << name << "\";";
	}
	sstm << "Terrain={Xrange=" << xmin << ".." << xmax << ";";
	sstm << "Yrange=" << ymin << ".." << ymax << ";};";
	sstm << "Window={Xrange=" << winXmin << ".." << winXmax << ";";
	sstm << "Yrange={" << winYmin << ".." << winYmax << ";};";
	sstm << "Chars={Empty=" << (int) emptyChar << ";Head=" << (int) headChar << ";Tail=" << (int) tailChar << ";Wire=" << (int) wireChar << ";};";
	sstm << "Colors={Empty=(" << emptyColor.red << "," << emptyColor.green << "," << emptyColor.blue << ");";
	sstm << "Head=(" << headColor.red << "," << headColor.green << "," << headColor.blue << ");";
	sstm << "Tail=(" << tailColor.red << "," << tailColor.green << "," << tailColor.blue << ");";
	sstm << "Wire=(" << wireColor.red << "," << wireColor.green << "," << wireColor.blue << ");};";
	sstm << "Initial={";
	std::stringstream wireStream;
	wireStream << "Wire=";
	std::stringstream tailStream;
	tailStream << "Tail=";
	std::stringstream headStream;
	headStream << "Head=";

	const char * wireSep = "";
	const char * tailSep = "";
	const char * headSep = "";

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			if(cells[i][j].getState() == wire_cell::HEAD){
				headStream << headSep << '(' << cells[i][j].getX() << ',' << cells[i][j].getY() << ')';
				headSep = ",";
			}
			else if(cells[i][j].getState() == wire_cell::TAIL){
				tailStream << tailSep << '(' << cells[i][j].getX() << ',' << cells[i][j].getY() << ')';
				tailSep = ",";
			}
			else if(cells[i][j].getState() == wire_cell::WIRE){
				wireStream << wireSep << '(' << cells[i][j].getX() << ',' << cells[i][j].getY() << ')';
				wireSep = ",";
			}
		}
	}
	wireStream << ";";
	tailStream << ";";
	headStream << ";";
	sstm << wireStream.str() << tailStream.str() << headStream.str();
	sstm << "};};";
	return sstm.str();
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