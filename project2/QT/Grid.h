#ifndef GRID_H

#include "../board.h"
#include "Tile.h"
#include <QGridLayout>
#include "../cell.h"

class Grid : public QWidget
{
	Q_OBJECT

public:
	Grid(int xmin, int xmax, int ymin, int ymax, int cellSize, board gameBoard, QWidget * parent = NULL);
	~Grid();

private:
	void initGrid(board gameBoard);

	int xmin;
	int xmax;
	int ymin;
	int ymax;
	int cellSize;
	std::vector<std::vector<Tile*> > cells;
	QGridLayout* grid;
};

#define GRID_H value
#endif