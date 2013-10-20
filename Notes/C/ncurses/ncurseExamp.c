#include <ncurses.h>
/*Includes curses header*/

int main(int argc, char const *argv[])
{
	/*All output is through magic ncurses interface, this initializes the screen and does other stuff*/
	initscr();

	/*Acts like printf, but prints to the terminal "window"*/
	printw("Hello, world!\n");

	/*All output is buffered, this causes all changes to be written to the screen*/
	refresh();

	/*Grab a keystroke*/
	getch();

	/*reverse of initsrc tears down the screen and go back to normal terminal*/
	endwin();

	return 0;
}
