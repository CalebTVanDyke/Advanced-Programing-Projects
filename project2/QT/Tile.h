#ifndef TILE_H

#include <QWidget>
#include <QFrame>

class Tile : public QFrame
{
	Q_OBJECT

public:
	void redraw(QColor color);

public:
	Tile(QWidget *parent = NULL);
	void setCellSize(int size);
	~Tile();

 private:
 	bool alive;
 	int cellSize;
 	QWidget* rect;

};

#define TILE_H 1
#endif