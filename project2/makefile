all: life

clean:
	rm -rf life.o cell.o board.o life filehandling.o

life: life.o cell.o board.o filehandling.o
	g++ -std=c++11 -g -o life life.o cell.o board.o filehandling.o

life.o: life.cpp board.h cell.h
	g++ -std=c++11 -c -g life.cpp

cell.o: cell.cpp cell.h
	g++ -std=c++11 -c -g cell.cpp

board.o: board.cpp board.h
	g++ -std=c++11 -c -g board.cpp

filehandling.o: filehandling.cpp filehandling.h
	g++ -std=c++11 -c -g filehandling.cpp