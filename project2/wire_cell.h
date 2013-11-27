#ifndef wire_cell_H
#define wire_cell_H 1

#include <iostream>

class wire_cell
{
public:
	enum CellState {
		HEAD,
		WIRE,
		TAIL,
		EMPTY
	};
private:
	int x;
	int y;
	CellState state;
public:
	wire_cell();
	wire_cell(const wire_cell& c);
	~wire_cell();
	void operator=(wire_cell c);
	inline CellState getState() { return state; }
	inline void setState(CellState state) { this->state = state; }
	void setXY(int x, int y);
	inline int getX() { return x; }
	inline int getY() { return y; };
private:
	bool alive;
};

#endif