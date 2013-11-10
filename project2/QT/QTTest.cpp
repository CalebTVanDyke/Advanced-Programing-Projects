#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QWidget>
#include "../board.h"
#include "../cell.h"
#include <string>
#include "../filehandling.h"
#include "Tile.h"

using namespace std;

void displayHelp();

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
	
		board gameBoard = stringToBoard(fileInfo);

		updateBoardWithCommands(&gameBoard, argc, argv);

		QScrollArea window;
		window.setWindowTitle(gameBoard.getName().c_str());
		window.setContentsMargins(QMargins(0, 0, 0, 0));
		window.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		

		QWidget holder;
		holder.setContentsMargins(QMargins(0, 0, 0, 0));

		QGridLayout *grid = new QGridLayout();

	    grid->setSpacing(0);
	    grid->setContentsMargins(QMargins(0,0,0,0));

	    std::vector<std::vector<Tile*> > cells;

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
		holder.setLayout(grid);

		window.setWidget(&holder);

		window.show();

		return app.exec();
	}catch(int e){
		return -1;
	}
}
void displayHelp(){
	cout << "help\n";
}