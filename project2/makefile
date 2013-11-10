all: life life-gui

clean:
	rm -rf life.o cell.o board.o life filehandling.o Grid.o Tile.o life-gui life-gui.o; make -f Makefile clean

life-gui: life-gui.pro
	qmake-qt4 life-gui.pro; make -f Makefile

life: life.o cell.o board.o filehandling.o
	g++ -g -o life life.o cell.o board.o filehandling.o -lboost_regex-mt

life.o: life.cpp board.h cell.h
	g++ -c -g life.cpp

cell.o: cell.cpp cell.h
	g++ -c -g cell.cpp

board.o: board.cpp board.h
	g++ -c -g board.cpp

filehandling.o: filehandling.cpp filehandling.h
	g++ -c -g filehandling.cpp