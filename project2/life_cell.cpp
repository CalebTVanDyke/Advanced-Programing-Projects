#include "life_cell.h"

life_cell::life_cell(){
	alive = false;
	x = -1;
	y = -1;
}
life_cell::life_cell(const life_cell& c){
	alive = c.alive;
	x = c.x;
	y = c.y;
}
void life_cell::operator=(life_cell c){
	alive = c.alive;
	x = c.x;
	y = c.y;
}
life_cell::~life_cell(){
}
void life_cell::switchState(){
	if(alive){
		alive = false;
	}else{
		alive = true;
	}
}
void life_cell::setXY(int x, int y){
	this->x = x;
	this->y = y;
}