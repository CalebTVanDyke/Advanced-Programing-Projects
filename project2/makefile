all: life life-gui

tarball: cvandyke.tar.gz

cvandyke.tar.gz: makefile Tile.cpp Tile.h board.cpp board.h life_cell.cpp life_cell.h filehandling.cpp filehandling.h life-gui.cpp life-gui.pro life.cpp 
	tar -czf cvandyke.targz sample.life makefile Tile.cpp Tile.h board.cpp board.h cell.cpp cell.h filehandling.cpp filehandling.h life-gui.cpp life-gui.pro life.cpp

clean:
	rm -rf GameDialog.o wire_board.o wire_cell.o life_board.o life.o life_cell.o board.o life filehandling.o Grid.o Tile.o life_gui life-gui.o; make -f Makefile clean; rm Makefile

life-gui: board.o life_cell.o Tile.cpp life-gui.cpp filehandling.o life_board.o wire_board.o wire_cell.o ele_board.o
	qmake-qt4 life-gui.pro; make -f Makefile

life: life.o life_cell.o board.o filehandling.o life_board.o wire_board.o wire_cell.o ele_board.o
	g++ -g -o life life.o life_cell.o board.o filehandling.o life_board.o wire_board.o wire_cell.o ele_board.o -lboost_regex-mt

life.o: life.cpp board.h life_cell.h
	g++ -c -g life.cpp

life_cell.o: life_cell.cpp life_cell.h
	g++ -c -g life_cell.cpp

board.o: board.cpp board.h
	g++ -c -g board.cpp

filehandling.o: filehandling.cpp filehandling.h
	g++ -c -g filehandling.cpp

life_board.o: life_board.cpp life_board.h board.o life_cell.o
	g++ -c -g life_board.cpp

wire_board.o: wire_board.cpp wire_board.h board.o wire_cell.o
	g++ -c -g wire_board.cpp

wire_cell.o: wire_cell.cpp wire_cell.h
	g++ -c -g wire_cell.cpp

ele_board.o: ele_board.cpp ele_board.h
	g++ -c -g ele_board.cpp