#include "cell.h"

cell::cell(){
	alive = false;
	x = -1;
	y = -1;
}
cell::cell(const cell& c){
	alive = c.alive;
	x = c.x;
	y = c.y;
}
void cell::operator=(cell c){
	alive = c.alive;
	x = c.x;
	y = c.y;
}
cell::~cell(){
}
void cell::switchState(){
	if(alive){
		alive = false;
	}else{
		alive = true;
	}
}
void cell::setXY(int x, int y){
	this->x = x;
	this->y = y;
}