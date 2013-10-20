all: qtest

clean:
	rm -rf qtest qtest.o Queue.o

qtest: qtest.o Queue.o
	gcc -g -o qtest qtest.o Queue.o

qtest.o: qtest.c
	gcc -g -c qtest.c

Queue.o: Queue.c Queue.h
	gcc -g -c Queue.c