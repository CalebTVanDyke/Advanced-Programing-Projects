#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QWidget>
#include "life_board.h"
#include "cell.h"
#include <string>
#include "filehandling.h"
#include "Tile.h"
#include "wire_cell.h"
#include "wire_board.h"
#include "ele_board.h"

using namespace std;

void displayHelp();
std::vector<std::vector<Tile*> > drawLife(QGridLayout * grid, life_board* gameBoard, int cellSize);
std::vector<std::vector<Tile*> > drawWire(QGridLayout * grid, wire_board* gameBoard, int cellSize);


int main(int argc, char *argv[])
{
	int fromFile = 0;
	int cellSize = 10;
	for (int i = 1; i < argc; ++i)
	{
		if('-' == argv[i][0]){
			int len = strlen(argv[i]);
			for (int j = 1; j < len; ++j)
			{
				if('s' == argv[i][j]){
					if(i + 1 < argc){
						++i;
						if(!isdigit(argv[i][0])){
							cerr << "Not a valid digit after -s for cell size. Default of 0 will be used.\n";
						}else{
							cellSize = atoi(argv[i]);
							if(0 >= cellSize){
								cerr << "Cell size must be greater than 0\n";
							}
						}
					}else{
						cerr << "Value needs to be specified after -s.\n";
					}
				}
				else if('h' == argv[i][j]){
					displayHelp();
					return 0;
				}

			}
		}else{
			if(i + 1 == argc){
				fromFile = 1;
			}
		}
	}

	QApplication app(argc, argv);

	std::string fileInfo = "";
	try{
		if(fromFile)
			fileInfo = getStringFromFile(argv[argc - 1]);
		else
			fileInfo = getStringFromStdIn();
	
		board* gameBoard = stringToBoard(fileInfo);

		updateBoardWithCommands(gameBoard, argc, argv);

		QScrollArea window;
		window.setWindowTitle(gameBoard->getName().c_str());
		window.setContentsMargins(QMargins(0, 0, 0, 0));
		window.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		

		QWidget holder;
		holder.setContentsMargins(QMargins(0, 0, 0, 0));

		QGridLayout *grid = new QGridLayout();

		grid->setSpacing(0);
		grid->setContentsMargins(QMargins(0,0,0,0));

		std::vector<std::vector<Tile*> > cells;
		if(dynamic_cast<life_board*>(gameBoard))
			cells = drawLife(grid, (life_board*) gameBoard, cellSize);
		else if(dynamic_cast<wire_board*>(gameBoard)){
			cells = drawWire(grid, (wire_board*) gameBoard, cellSize);
		}else{
			return -1;
		}

		holder.setLayout(grid);

		window.setWidget(&holder);

		window.show();

		return app.exec();
	}catch(int e){
		return -1;
	}
}
std::vector<std::vector<Tile*> > drawLife(QGridLayout * grid, life_board* gameBoard, int cellSize){
    std::vector<std::vector<Tile*> > cells;

    int x = 0, y = 0;
    /**
    *	This fills the cells vector above with tile objects that are colored with respect to the board
    **/
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		std::vector<Tile*> row;
		x = 0;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			cell cur;
			Tile *tile = new Tile();
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tile->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
			}else{
				cur = gameBoard->getCell(j, i);
			}
			if(cur.isAlive()){
				tile->redraw(qRgba(gameBoard->getAliveColor().red, gameBoard->getAliveColor().green, gameBoard->getAliveColor().blue, 255));
			}else{
				tile->redraw(qRgba(gameBoard->getDeadColor().red, gameBoard->getDeadColor().green, gameBoard->getDeadColor().blue, 255));
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
	return cells;
}
std::vector<std::vector<Tile*> > drawWire(QGridLayout * grid, wire_board* gameBoard, int cellSize){

    std::vector<std::vector<Tile*> > cells;

    int x = 0, y = 0;
    /**
    *	This fills the cells vector above with tile objects that are colored with respect to the board
    **/
	for (int j = gameBoard->getWinXMin(); j <= gameBoard->getWinXMax(); ++j)
	{
		std::vector<Tile*> row;
		x = 0;
		for (int i = gameBoard->getWinYMax(); i >= gameBoard->getWinYMin(); --i)
		{
			wire_cell cur;
			Tile *tile = new Tile();
			if(i < gameBoard->getYMin() || i > gameBoard->getYMax() || j < gameBoard->getXMin() || j > gameBoard->getXMax()){
				tile->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
			}else{
				cur = gameBoard->getCell(j, i);
			}
			if(cur.getState() == wire_cell::EMPTY){
				tile->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::HEAD){
				tile->redraw(qRgba(gameBoard->getHeadColor().red, gameBoard->getHeadColor().green, gameBoard->getHeadColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::TAIL){
				tile->redraw(qRgba(gameBoard->getTailColor().red, gameBoard->getTailColor().green, gameBoard->getTailColor().blue, 255));
			}
			else if(cur.getState() == wire_cell::WIRE){
				tile->redraw(qRgba(gameBoard->getWireColor().red, gameBoard->getWireColor().green, gameBoard->getWireColor().blue, 255));
			}else{
				tile->redraw(qRgba(gameBoard->getEmptyColor().red, gameBoard->getEmptyColor().green, gameBoard->getEmptyColor().blue, 255));
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
	return cells;
}
void displayHelp(){
	cout << "life-gui\n";
	cout << "--------\n";
	cout << "life-gui takes in a .life file or a file of a specific format, and can display the output in a graphical form.\n\n";

	cout << "life-gui [-h] [-g n] [-s] [-tx low..high] [-ty low..high] [-wy low..high] [-wx low..high] [file]\n\n";
	cout << "Options:\n";
	cout << "-h              displays a help screen\n";
	cout << "-g n            advances the life file to the n-th generation\n";
	cout << "-s              the size that each tile should be in the window, default is 10\n";
	cout << "-tx low..high   changes the terrain x values in the file to the values specified by low..high\n";
	cout << "                if the window is not specified in the command line it will be set to the whole terrain\n";
	cout << "-ty low..high   changes the terrain y values in the file to the values specified by low..high\n";
	cout << "                if the window is not specified in the command line it will be set to the whole terrain\n";
	cout << "-wx low..high   changes the window x values in the file to the values specified by low..high\n";
	cout << "-wy low..high   changes the window y values in the file to the values specified by low..high\n";
	cout << "file            this will be the file that is read\n";
	cout << "                if ommited file will be read from standard input\n";
}