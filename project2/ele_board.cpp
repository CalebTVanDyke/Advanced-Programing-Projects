#include "ele_board.h"
#include <math.h>

ele_board::ele_board(std::string name, int xmax, int xmin, int ymax, int ymin, char one, char zero, int rule)
: life_board(name, xmax, xmin, ymax, ymin, one , zero)
{
	this->rule = rule;
}

ele_board::~ele_board(){

}
void ele_board::updateOne(){
	cell ** newCells;
	newCells = new cell*[height];
	for(int i = 0; i < height; ++i){
		newCells[i] = new cell[width];
	}

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			if(cells[i][j].isAlive()){
				newCells[i][j].setAlive(true);
				continue;
			}
			bool zero, one, two;
			if(i - 1 >= 0 && j - 1 >= 0 && cells[i - 1][j - 1].isAlive()){
				two = 1;
			}else{
				two = 0;
			}
			if(i - 1 >= 0 && cells[i - 1][j].isAlive()){
				one = 1;
			}else{
				one = 0; 
			}
			if(i - 1 >= 0 && j + 1 < width && cells[i - 1][j + 1].isAlive()){
				zero = 1;
			}else{
				zero = 0;
			}
			int num = two* pow(2, 2) + one * pow(2, 1) + zero * pow(2, 0);
			if( (rule >> num) & 0x1 ){
				newCells[i][j].setAlive(true);
			}else{
				newCells[i][j].setAlive(false);
			}
		}
	}
	freeCells();
	cells = newCells;
}
std::string ele_board::toFile(){
	return "";
}