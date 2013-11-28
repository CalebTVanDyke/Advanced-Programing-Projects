#include "GameDialog.h"

#include <iostream>
#include "cell.h"
#include "wire_cell.h"
#include <stdlib.h>
#include <string>
#include <sstream>


GameDialog::GameDialog(QApplication* app, board* gameBoard, QWidget * holder, std::vector<std::vector<Tile*> > cells, int cellSize){
	this->cellSize = cellSize;
	tiles = cells;
	curBoard = gameBoard;
	initialBoard = gameBoard->Clone();
	grid = (QGridLayout *)(holder->layout());
	this->holder = holder;

	gen = 0;

	this->app = app;
	setWindowTitle("Controlls");
	
	sizeLabel = new QLabel("Cell Size: ");
	sizeSlider = new QSlider(Qt::Horizontal);
	sizeSpinBox = new QSpinBox;

	timeLabel = new QLabel("Delay (ms): ");
	timeSlider = new QSlider(Qt::Horizontal);
	timeSpinBox = new QSpinBox;

	std::stringstream sstm; 
	sstm << "Generation: " << gen;
	genLabel = new QLabel(QString(sstm.str().c_str()));

	quitButton = new QPushButton("Quit");
	restartButton = new QPushButton("Restart");
	playButton = new QPushButton("Play");
	stepButton = new QPushButton("Step");
	
	topLayout = new QHBoxLayout;
	midLayout = new QHBoxLayout;
	genLayout = new QHBoxLayout;
	botLayout = new QHBoxLayout;
	mainLayout = new QVBoxLayout;

	sizeSlider->setRange(1, 50);
	sizeSpinBox->setRange(1, 50);
	timeSlider->setRange(1, 10000);
	timeSpinBox->setRange(1, 10000);

	QObject::connect(sizeSlider, SIGNAL(valueChanged(int)),
		sizeSpinBox, SLOT(setValue(int)));

	sizeSlider->setSliderPosition(cellSize);

	QObject::connect(sizeSlider, SIGNAL(valueChanged(int)),
		this, SLOT(adjustSize()));

	QObject::connect(sizeSpinBox, SIGNAL(valueChanged(int)),
		sizeSlider, SLOT(setValue(int)));

	QObject::connect(timeSlider, SIGNAL(valueChanged(int)),
		timeSpinBox, SLOT(setValue(int)));

	QObject::connect(timeSpinBox, SIGNAL(valueChanged(int)),
		timeSlider, SLOT(setValue(int)));

	timeSlider->setSliderPosition(1000);

	QObject::connect(quitButton, SIGNAL(clicked()),
		this, SLOT(quit()));

	QObject::connect(playButton, SIGNAL(clicked()),
		this, SLOT(play()));

	QObject::connect(restartButton, SIGNAL(clicked()),
		this, SLOT(restart()));

	QObject::connect(stepButton, SIGNAL(clicked()),
		this, SLOT(step()));

	timer = new QTimer;

	QObject::connect(timer, SIGNAL(timeout()),
		this, SLOT(step()));


	topLayout->addWidget(sizeLabel);
	topLayout->addWidget(sizeSpinBox);
	topLayout->addWidget(sizeSlider);

	midLayout->addWidget(timeLabel);
	midLayout->addWidget(timeSpinBox);
	midLayout->addWidget(timeSlider);

	genLayout->addWidget(genLabel);

	botLayout->addWidget(quitButton);
	botLayout->addWidget(restartButton);
	botLayout->addWidget(playButton);
	botLayout->addWidget(stepButton);

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(midLayout);
	mainLayout->addLayout(genLayout);
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
	delete genLabel;
	delete genLayout;
}
void GameDialog::play(){
	if(timer->isActive()){
		timer->stop();
		playButton->setText("Play");
		return;
	}
	timer->setInterval(timeSlider->value());
	timer->start();
	playButton->setText("Stop");
}
void GameDialog::restart(){
	if(timer->isActive()){
		timer->stop();
		playButton->setText("Play");
	}
	delete curBoard;
	curBoard = initialBoard->Clone();
	gen = 0;
	updateGen();
	redraw();
}
void GameDialog::step(){
	curBoard->updateOne();
	redraw();
	gen++;
	updateGen();
}
void GameDialog::quit(){
	exit(0);
}
void GameDialog::adjustSize(){
	cellSize = sizeSlider->value();
	holder->setMinimumHeight(cellSize * curBoard->getHeight());
	holder->setMinimumWidth(cellSize * curBoard->getWidth());
	holder->setMaximumHeight(cellSize * curBoard->getHeight());
	holder->setMaximumWidth(cellSize * curBoard->getWidth());
	redraw();
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
	int x = 0;
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		int y = 0;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			wire_cell cur;
			tiles[x][y]->setCellSize(cellSize);
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tiles[x][y]->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
				y++;
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
			y++;
		}
		x++;
	}
}
void GameDialog::redrawLife(){
	life_board * gameBoard = (life_board*) curBoard;
	int x = 0;
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		int y = 0;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			cell cur;
			tiles[x][y]->setCellSize(cellSize);
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tiles[x][y]->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
				y++;
				continue;
			}else{
				cur = gameBoard->getCell(j, i);
			}
			if(cur.isAlive()){
				tiles[x][y]->redraw(qRgba(gameBoard->getAliveColor().red, gameBoard->getAliveColor().green, gameBoard->getAliveColor().blue, 255));
			}else{
				tiles[x][y]->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
			}
			y++;
		}
		x++;
	}
}
void GameDialog::updateGen(){
	std::stringstream sstm; 
	sstm << "Generation: " << gen;
	genLabel->setText(sstm.str().c_str());
}
