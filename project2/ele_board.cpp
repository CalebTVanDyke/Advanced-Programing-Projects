#include "ele_board.h"

ele_board::ele_board(std::string name, int xmax, int xmin, int ymax, int ymin, char one, char zero)
: life_board(name, xmax, xmin, ymax, ymin, one , zero)
{}

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
			if( (two && !one && !zero) || (!two && one && zero) || (!two && one && !zero) || (!two && !one && zero) ){
				newCells[i][j].setAlive(true);
			}else{
				newCells[i][j].setAlive(false);
			}
			std::cout << '(' << j << ',' << i << ')' << "=" << two << one << zero << '\n';
		}
	}
	freeCells();
	cells = newCells;
}
std::string ele_board::toFile(){
	return "";
}