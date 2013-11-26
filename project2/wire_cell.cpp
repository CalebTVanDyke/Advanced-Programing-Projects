#include "wire_cell.h"

wire_cell::wire_cell(){
	state = EMPTY;
	x = -1;
	y = -1;
}
wire_cell::wire_cell(const wire_cell& c){
	state = c.state;
	x = c.x;
	y = c.y;
}
void wire_cell::operator=(wire_cell c){
	state = c.state;
	x = c.x;
	y = c.y;
}
wire_cell::~wire_cell(){
}
void wire_cell::setXY(int x, int y){
	this->x = x;
	this->y = y;
}