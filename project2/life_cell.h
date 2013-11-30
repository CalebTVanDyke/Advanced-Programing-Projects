#ifndef LIFE_CELL_H
#define LIFE_CELL_H 1

class life_cell
{
	int x;
	int y;
public:
	life_cell();
	life_cell(const life_cell& c);
	~life_cell();
	void operator=(life_cell c);
	inline bool isAlive() { return alive; }
	inline void setAlive(bool alive) { this->alive = alive;}
	/**
	*	Switchs the state of the life_cell to the opposite of what it was
	**/
	void switchState();
	void setXY(int x, int y);
	inline int getX() { return x; }
	inline int getY() { return y; };
private:
	bool alive;
};

#endif