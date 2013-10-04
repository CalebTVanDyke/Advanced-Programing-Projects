all: sndinfo sndconv sndcat

tarball: proj1.tar.gz

proj1.tar.gz: sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile README.txt sndinfo.c
	tar -czf proj1.targz sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile README.txt sndinfo.c

clean:
	rm -f AIFF.o CS229.o util.o sndinfo.o sndconv.o sndinfo sndconv sndcat sndcat.o

sndcat: sndcat.o AIFF.o CS229.o util.o
	gcc -ansi -g -o sndcat sndcat.o AIFF.o CS229.o util.o -lm

sndconv: sndconv.o AIFF.o CS229.o util.o
	gcc -ansi -g -o sndconv sndconv.o AIFF.o CS229.o util.o -lm

sndinfo: sndinfo.o AIFF.o CS229.o util.o
	gcc -ansi -g -o sndinfo sndinfo.o AIFF.o CS229.o util.o -lm

sndcat.o: sndcat.c util.h CS229.h AIFF.h
	gcc -ansi -g -c sndcat.c

sndconv.o: sndconv.c util.h CS229.h AIFF.h
	gcc -ansi -g -c sndconv.c

sndinfo.o: sndinfo.c util.h CS229.h AIFF.h
	gcc -ansi -g -c sndinfo.c

util.o: util.c
	gcc -ansi -g -c util.c

AIFF.o: AIFF.c util.h
	gcc -ansi -g -c AIFF.c

CS229.o: CS229.c util.h
	gcc -ansi -g -c CS229.c