#include "GameDialog.h"

#include <iostream>
#include "cell.h"
#include "wire_cell.h"
#include <stdlib.h>

GameDialog::GameDialog(QApplication* app, board* gameBoard, QGridLayout * grid, std::vector<std::vector<Tile*> > cells, int cellSize){
	this->cellSize = cellSize;
	tiles = cells;
	curBoard = gameBoard;
	initialBoard = gameBoard->Clone();
	this->grid = grid;

	this->app = app;
	setWindowTitle("Controlls");
	
	sizeLabel = new QLabel("Cell Size: ");
	sizeSlider = new QSlider(Qt::Horizontal);
	sizeSpinBox = new QSpinBox;

	timeLabel = new QLabel("Delay: ");
	timeSlider = new QSlider(Qt::Horizontal);
	timeSpinBox = new QSpinBox;

	quitButton = new QPushButton("Quit");
	restartButton = new QPushButton("Restart");
	playButton = new QPushButton("Play");
	stepButton = new QPushButton("Step");
	
	topLayout = new QHBoxLayout;
	midLayout = new QHBoxLayout;
	botLayout = new QHBoxLayout;
	mainLayout = new QVBoxLayout;

	sizeSlider->setRange(1, 50);
	sizeSpinBox->setRange(1, 50);
	timeSlider->setRange(1, 10000);
	timeSpinBox->setRange(1, 10000);

	QObject::connect(sizeSlider, SIGNAL(valueChanged(int)),
		sizeSpinBox, SLOT(setValue(int)));
	
	sizeSlider->setSliderPosition(cellSize);

	QObject::connect(sizeSpinBox, SIGNAL(valueChanged(int)),
		sizeSlider, SLOT(setValue(int)));

	QObject::connect(timeSlider, SIGNAL(valueChanged(int)),
		timeSpinBox, SLOT(setValue(int)));

	QObject::connect(timeSpinBox, SIGNAL(valueChanged(int)),
		timeSlider, SLOT(setValue(int)));

	QObject::connect(quitButton, SIGNAL(clicked()),
		this, SLOT(quit()));

	QObject::connect(playButton, SIGNAL(clicked()),
		this, SLOT(play()));

	QObject::connect(restartButton, SIGNAL(clicked()),
		this, SLOT(restart()));

	QObject::connect(stepButton, SIGNAL(clicked()),
		this, SLOT(step()));

	topLayout->addWidget(sizeLabel);
	topLayout->addWidget(sizeSpinBox);
	topLayout->addWidget(sizeSlider);

	midLayout->addWidget(timeLabel);
	midLayout->addWidget(timeSpinBox);
	midLayout->addWidget(timeSlider);

	botLayout->addWidget(quitButton);
	botLayout->addWidget(restartButton);
	botLayout->addWidget(playButton);
	botLayout->addWidget(stepButton);

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(midLayout);
	mainLayout->addLayout(botLayout);
	setLayout(mainLayout);
}
GameDialog::~GameDialog(){
	delete sizeLabel;
	delete sizeSlider;
	delete sizeSpinBox;
	delete timeLabel;
	delete timeSpinBox;
	delete timeSlider;
	delete topLayout;
	delete midLayout;
	delete botLayout;
	delete mainLayout;
	delete quitButton;
	delete restartButton;
	delete stepButton;
	delete quitButton;
}
void GameDialog::play(){
	std::cout << "PLAY" << '\n';
}
void GameDialog::restart(){
	std::cout << "RESTART" << '\n';
}
void GameDialog::step(){
	curBoard->updateOne();
	redraw();
}
void GameDialog::quit(){
	exit(0);
}
void GameDialog::redraw(){
	if(dynamic_cast<life_board*>(curBoard)){
		redrawLife();
	}
	else if(dynamic_cast<wire_board*>(curBoard)){
		redrawWire();
	}
}
void GameDialog::redrawWire(){
	wire_board * gameBoard = (wire_board*) curBoard;
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		std::vector<Tile*> row;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			int x = gameBoard->computeX(j);
			int y = gameBoard->computeY(i);
			wire_cell cur;
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tiles[x][y]->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
				continue;
			}else{
				cur = gameBoard->getCell(j, i);
			}
			if(cur.getState() == wire_cell::EMPTY){
				tiles[x][y]->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::HEAD){
				tiles[x][y]->redraw(qRgba(gameBoard->getHeadColor().red, gameBoard->getHeadColor().green, gameBoard->getHeadColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::TAIL){
				tiles[x][y]->redraw(qRgba(gameBoard->getTailColor().red, gameBoard->getTailColor().green, gameBoard->getTailColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::WIRE){
				tiles[x][y]->redraw(qRgba(gameBoard->getWireColor().red, gameBoard->getWireColor().green, gameBoard->getWireColor().blue, 255));
			}else{
				tiles[x][y]->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
			}
		}
	}
}
void GameDialog::redrawLife(){
	life_board * gameBoard = (life_board*) curBoard;
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		std::vector<Tile*> row;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			int x = gameBoard->computeX(j);
			int y = gameBoard->computeY(i);
			cell cur;
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tiles[x][y]->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
				continue;
			}else{
				cur = gameBoard->getCell(j, i);
			}
			if(cur.isAlive()){
				tiles[x][y]->redraw(qRgba(gameBoard->getAliveColor().red, gameBoard->getAliveColor().green, gameBoard->getAliveColor().blue, 255));
			}else{
				tiles[x][y]->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
			}
		}
	}
}
