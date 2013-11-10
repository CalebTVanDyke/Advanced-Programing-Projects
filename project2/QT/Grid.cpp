#include "Grid.h"

Grid::Grid(int xmin, int xmax, int ymin, int ymax, int cellSize, board gameBoard, QWidget * parent) 
: QWidget(parent){
	this->xmin = xmin;
	this->xmax = xmax;
	this->ymax = ymax;
	this->ymin = ymin;
	this->cellSize = cellSize;

	grid = new QGridLayout;
	grid->setSpacing(0);
    grid->setContentsMargins(QMargins(0,0,0,0));

    initGrid(gameBoard);
    setLayout(grid);

}
Grid::~Grid(){
	delete grid;
}

void Grid::initGrid(board gameBoard){

	int x = 0, y = 0;

	for (int j = gameBoard.getWinXMin(); j <= gameBoard.getWinXMax(); ++j)
	{
		std::vector<Tile*> row;
		x = 0;
		for (int i = gameBoard.getWinYMax(); i >= gameBoard.getWinYMin(); --i)
		{
			Tile *tile = new Tile();

			cell cur = gameBoard.getCell(j, i);
			if(cur.isAlive()){
				tile->redraw(qRgba(gameBoard.getAliveColor().red, gameBoard.getAliveColor().green, gameBoard.getAliveColor().blue, 255));
			}else{
				tile->redraw(qRgba(gameBoard.getDeadColor().red, gameBoard.getDeadColor().green, gameBoard.getDeadColor().blue, 255));
			}
			tile->setCellSize(cellSize);

			row.push_back(tile);
			grid->addWidget(tile,x,y);
			grid->setColumnStretch(x,0);
			x++;
		}
		cells.push_back(row);
		grid->setRowStretch(y,0);
		y++;
	}

}