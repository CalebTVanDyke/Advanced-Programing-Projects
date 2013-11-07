all: life

life: life.o cell.o board.o filehandling.o
	g++ -g -o life life.o cell.o board.o filehandling.o

life.o: life.cpp board.h cell.h
	g++ -c -g life.cpp

cell.o: cell.cpp cell.h
	g++ -c -g cell.cpp

board.o: board.cpp board.h
	g++ -c -g board.cpp

filehandling.o: filehandling.cpp filehandling.h
	g++ -c -g filehandling.cpp