all: sndinfo sndconv sndcat sndcut sndshow

tarball: cvandyke.tar.gz

cvandyke.tar.gz: sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile sndinfo.c
	tar -czf cvandyke.targz sndconv.c util.c util.h AIFF.c AIFF.h CS229.c CS229.h makefile README.txt sndinfo.c sndshow.c sndcut.c sndcat.c

clean:
	rm -f AIFF.o CS229.o util.o sndinfo.o sndconv.o sndinfo sndconv sndcat sndcat.o sndshow.o sndshow sndcut.o sndcut

sndshow: sndshow.o AIFF.o CS229.o util.o
	gcc -ansi  -o sndshow sndshow.o AIFF.o CS229.o util.o -lm

sndcut: sndcut.o AIFF.o CS229.o util.o
	gcc -ansi  -o sndcut sndcut.o AIFF.o CS229.o util.o -lm

sndcat: sndcat.o AIFF.o CS229.o util.o
	gcc -ansi  -o sndcat sndcat.o AIFF.o CS229.o util.o -lm

sndconv: sndconv.o AIFF.o CS229.o util.o
	gcc -ansi  -o sndconv sndconv.o AIFF.o CS229.o util.o -lm

sndinfo: sndinfo.o AIFF.o CS229.o util.o
	gcc -ansi  -o sndinfo sndinfo.o AIFF.o CS229.o util.o -lm

sndshow.o: sndshow.c util.h CS229.h AIFF.h
	gcc -ansi  -c sndshow.c

sndcut.o: sndcut.c util.h CS229.h AIFF.h
	gcc -ansi  -c sndcut.c

sndcat.o: sndcat.c util.h CS229.h AIFF.h
	gcc -ansi  -c sndcat.c

sndconv.o: sndconv.c util.h CS229.h AIFF.h
	gcc -ansi  -c sndconv.c

sndinfo.o: sndinfo.c util.h CS229.h AIFF.h
	gcc -ansi  -c sndinfo.c

util.o: util.c
	gcc -ansi  -c util.c

AIFF.o: AIFF.c util.h
	gcc -ansi  -c AIFF.c

CS229.o: CS229.c util.h
	gcc -ansi  -c CS229.c