#ifndef TILE_H

#include <QWidget>
#include <QFrame>

class Tile : public QFrame
{
	Q_OBJECT

public:
	/**
	*	Redraws the tile with the respected color as its backgroud color
	**/
	void redraw(QColor color);

public:
	Tile(QWidget *parent = NULL);
	/**
	*	Sets the size of each cell
	**/
	void setCellSize(int size);
	int getCellSize() { return cellSize; }
	~Tile();

 private:
 	bool alive;
 	int cellSize;
 	/**
 	*	The rect that is used to color the background
 	**/
 	QWidget* rect;

};

#define TILE_H 1
#endif