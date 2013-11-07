#include "cell.h"

cell::cell(){
	alive = false;
	x = -1;
	y = -1;
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