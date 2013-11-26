#ifndef CELL_H
#define CELL_H 1

class cell
{
	int x;
	int y;
public:
	cell();
	cell(const cell& c);
	~cell();
	void operator=(cell c);
	inline bool isAlive() { return alive; }
	inline void setAlive(bool alive) { this->alive = alive;}
	/**
	*	Switchs the state of the cell to the opposite of what it was
	**/
	void switchState();
	void setXY(int x, int y);
	inline int getX() { return x; }
	inline int getY() { return y; };
private:
	bool alive;
};

#endif