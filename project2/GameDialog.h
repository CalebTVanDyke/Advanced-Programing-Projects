#ifndef GAME_DIALOG_H
#define GAME_DIALOG_H 1

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDialog>
#include <QApplication>
#include <QScrollBar>
#include "board.h"
#include "Tile.h"
#include "life_board.h"
#include "wire_board.h"

class GameDialog : public QDialog
{
	Q_OBJECT
public:
	GameDialog(QApplication *app, board* gameBoard, QWidget* holder, std::vector<std::vector<Tile*> > cells, int cellSize);
	~GameDialog();

public slots:
	void play();
	void restart();
	void step();
	void quit();
	void adjustSize();

private:

	QTimer* timer;
	int gen;
	int cellSize;
	std::vector<std::vector<Tile*> > tiles;
	board* initialBoard;
	board* curBoard;
	QGridLayout *grid;
	QWidget *holder;

	QLabel *sizeLabel;
	QApplication *app;
	QSlider *sizeSlider;
	QSpinBox *sizeSpinBox;

	QLabel *timeLabel;
	QSlider *timeSlider;
	QSpinBox *timeSpinBox;

	QLabel *genLabel;

	QPushButton *quitButton;
	QPushButton *restartButton;
	QPushButton *playButton;
	QPushButton *stepButton;
	
	QHBoxLayout *topLayout;
	QHBoxLayout *midLayout;
	QHBoxLayout *botLayout;
	QHBoxLayout *genLayout;
	QVBoxLayout *mainLayout;

	void redraw();
	void redrawLife();
	void redrawWire();
	void updateGen();
};

#endif