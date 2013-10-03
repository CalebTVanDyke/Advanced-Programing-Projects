#Builds all executables
all: mpg2kmpl hello
#Cleans object files and executables
clean:
	rm -f foo. bar.o hello.o mpg2kmpl hello
#creates tarball 
tarball: proj1.tar.gz
#create tarball
proj1.tar.gz: foo.c bar.c hello.c makefile
	tar -czf proj1.tar.gz foo.c bar.c bar.c hello.c makefile
#create mpg2kmpl executable
mpg2kmpl: foo.o bar.o
	gcc -o mpg2kmpl foo.o bar.o
#create foo object file
foo.o: foo.c bar.h
	gcc -c foo.c
#create bar object file
bar.o: bar.c bar.h
	gcc -c bar.c
#create hello executable
hello: hello.o
	gcc -o hello hello.o
#creates hello object file
hello.o: hello.c
	gcc -c hello.c

#COMMENTS