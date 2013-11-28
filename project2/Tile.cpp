#include "Tile.h"
#include <QtGui>
#include <iostream>
#include <Qt>

Tile::Tile(QWidget *parent)
: QFrame(parent)
{

	alive = false;
	setFrameStyle(QFrame::Box);

	rect = new QWidget;

    cellSize = 10;
	rect->setMinimumSize(cellSize, cellSize);

	QHBoxLayout * layout = new QHBoxLayout;
	layout->addWidget(rect);
	setLayout(layout);

	layout->setContentsMargins(0,0,0,0);
	setLineWidth(1);
    redraw(qRgba(0, 0, 0, 255));

}
Tile::~Tile(){
	delete rect;
}
void Tile::redraw(QColor color){
	rect->setPalette(QPalette(color, color));
	rect->setAutoFillBackground(true);
}
void Tile::setCellSize(int size){
	cellSize = size;
	if(size < 4){
		setLineWidth(0);
	}else{
		setLineWidth(1);
	}
	rect->setMinimumSize(cellSize, cellSize);
}
