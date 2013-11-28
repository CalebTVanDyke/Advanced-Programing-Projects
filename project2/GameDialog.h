#ifndef GAME_DIALOG_H
#define GAME_DIALOG_H 1

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDialog>
#include <QApplication>
#include "board.h"
#include "Tile.h"
#include "life_board.h"
#include "wire_board.h"

class GameDialog : public QDialog
{
	Q_OBJECT
public:
	GameDialog(QApplication *app, board* gameBoard, QGridLayout* grid, std::vector<std::vector<Tile*> > cells, int cellSize);
	~GameDialog();

public slots:
	void play();
	void restart();
	void step();
	void quit();

private:
	int cellSize;
	std::vector<std::vector<Tile*> > tiles;
	board* initialBoard;
	board* curBoard;
	QGridLayout *grid;

	QLabel *sizeLabel;
	QApplication *app;
	QSlider *sizeSlider;
	QSpinBox *sizeSpinBox;

	QLabel *timeLabel;
	QSlider *timeSlider;
	QSpinBox *timeSpinBox;

	QPushButton *quitButton;
	QPushButton *restartButton;
	QPushButton *playButton;
	QPushButton *stepButton;
	
	QHBoxLayout *topLayout;
	QHBoxLayout *midLayout;
	QHBoxLayout *botLayout;
	QVBoxLayout *mainLayout;

	void redraw();
	void redrawLife();
	void redrawWire();
};

#endif