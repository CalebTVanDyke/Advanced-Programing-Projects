all: sndinfo sndconv sndcat sndcut sndshow sndedit

tarball: cvandyke.tar.gz

cvandyke.tar.gz: sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile sndinfo.c sndedit.c
	tar -czf cvandyke.targz sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile README.txt sndinfo.c sndshow.c sndcut.c sndcat.c sndedit.c

clean:
	rm -f AIFF.o CS229.o util.o sndinfo.o sndconv.o sndinfo sndconv sndcat sndcat.o sndshow.o sndshow sndcut.o sndcut sndedit sndedit.o

sndedit: sndedit.o AIFF.o CS229.o util.o
	gcc -ansi -g -o sndedit sndedit.o AIFF.o CS229.o util.o -lm -lncurses

sndshow: sndshow.o AIFF.o CS229.o util.o
	gcc -ansi  -g -o sndshow sndshow.o AIFF.o CS229.o util.o -lm -lncurses

sndcut: sndcut.o AIFF.o CS229.o util.o
	gcc -ansi -g  -o sndcut sndcut.o AIFF.o CS229.o util.o -lm -lncurses

sndcat: sndcat.o AIFF.o CS229.o util.o
	gcc -ansi  -g -o sndcat sndcat.o AIFF.o CS229.o util.o -lm -lncurses

sndconv: sndconv.o AIFF.o CS229.o util.o
	gcc -ansi -g  -o sndconv sndconv.o AIFF.o CS229.o util.o -lm -lncurses

sndinfo: sndinfo.o AIFF.o CS229.o util.o
	gcc -ansi -g  -o sndinfo sndinfo.o AIFF.o CS229.o util.o -lm -lncurses

sndedit.o: sndedit.c util.h CS229.h AIFF.h
	gcc -ansi -g -c sndedit.c

sndshow.o: sndshow.c util.h CS229.h AIFF.h
	gcc -ansi  -g -c sndshow.c

sndcut.o: sndcut.c util.h CS229.h AIFF.h
	gcc -ansi  -g -c sndcut.c

sndcat.o: sndcat.c util.h CS229.h AIFF.h
	gcc -ansi  -g -c sndcat.c

sndconv.o: sndconv.c util.h CS229.h AIFF.h
	gcc -ansi  -g -c sndconv.c

sndinfo.o: sndinfo.c util.h CS229.h AIFF.h
	gcc -ansi  -g -c sndinfo.c

util.o: util.c
	gcc -ansi  -g -c util.c

AIFF.o: AIFF.c util.h
	gcc -ansi  -g -c AIFF.c

CS229.o: CS229.c util.h
	gcc -ansi -g -c CS229.c