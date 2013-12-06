STRATEGY
--------
part1
-----
My overall strategy was to make a board (class) that consisted of a two dimensional array of cells (class).  These cells contained a boolean value that represented their state: true = alive and false = dead.  The board object handles all functions responsible for updating the board.  This includes updating generations, resizing window, resizing terrain, printing the board in a visual format or file format.  The board also stores the information to display the board such as the character for dead and alive cells, and the color for dead and alive cells.  For reading in the file I used boost/regex to parse the file.  First I took in a file and converted it into a string.  With this string I spliced out all of the comments and extra white space.  Then I read the file and created a board object from that.  All of this is done in the filehandling.cpp / filehandling.h files.  For life-gui I made a Tile class that inherits from QFrame.  This tile consists of its QFrame and a QWidget inside of it.  These tiles are what is used to make the graphical representation of the game board.  

part2
-----
I used my previous board class as a model for my next board classes.  It helded all the methods that are common between the two and that should be implemented for both classes.  I added the wire_board, life_board, and ele_board classes these classes represent the board for the specified game that will be played.  wire_board and life_board extend from the board class and ele_board extends from life_board.  The reason I choose to do this is because wire_board and life_board only contain the similarities that are represented in the board class where as ele_board also shares similiarities with the life_board class.  I also add the wire_cell and life_cell class these represent one cell in the wire_board and life_board.  life_cell is also used for ele_board because have the same amount of states.  For the GUI I added a GameDialog class this class holds all of the objects on the dialog and links the updating methods used to update the gui.


BREAKDOWN OF FILES
------------------
board.cpp
---------
	* holder for all cells in the board
	* contains all information for the rendering of the board and each cell based on its state
	* contains all functions responsible for updating the board and each cell
life_cell.cpp
--------
	* representation for one cell inside of a block for life_board
	* has a method to switch its state to the opposite of what it was
wire_cell.cpp
--------
	* representation for one cell inside of a block for wire_board

filehandling.cpp
----------------
	* responsible for converting the file into a board object and also printing out error messages about the files
	* also responsible for handling command line arguments that are similiar between the two programs
Tile.cpp
--------
	* this is a graphical representation of a cell using QT
	* inherits from QFrame so it can have a border around each cell
	* contains a QWidget which is colored to match the alive/dead color
life.cpp
--------
	* responsible for handling command line arguments specific to this program
	* calls appropriate functions to achieve desired output
life-gui.cpp
------------
	* responsible for handling command line arguments that are specific to this program
	* fills a QGridLayout with tiles and their appropriate files based on the board
	* represents this layout in a scrollable window
wire_board.cpp
---------
	* holder for all cells in the board
	* contains all information for the rendering of the board and each cell based on its state
	* contains all functions responsible for updating the wire board and each cell
life_board.cpp
---------
	* holder for all cells in the board
	* contains all information for the rendering of the board and each cell based on its state
	* contains all functions responsible for updating the life board and each cell
ele_board.cpp
---------
	* holder for all cells in the board
	* contains all information for the rendering of the board and each cell based on its state
	* contains all functions responsible for updating the elemetry cell board and each cell


PROGRAM DESCRIPTIONS
--------------------

life
----
Life takes in a .life file or a file of a specific format, and can display the output in a visual form or text form.

life [-h] [-g n] [-f] [-v] [-tx low..high] [-ty low..high] [-wy low..high] [-wx low..high] [file]
Options:
-h              displays a help screen
-g n            advances the life file to the n-th generation\
-v              output will be in a visual form
-f              output will be in the form of the file
-tx low..high   changes the terrain x values in the file to the values specified by low..high
                if the window is not specified in the command line it will be set to the whole terrain
-ty low..high   changes the terrain y values in the file to the values specified by low..high
                if the window is not specified in the command line it will be set to the whole terrain
-wx low..high   changes the window x values in the file to the values specified by low..high
-wy low..high   changes the window y values in the file to the values specified by low..high
file            this will be the file that is read
                if ommited file will be read from standard input

life-gui
--------
life-gui takes in a .life file or a file of a specific format, and can display the output in a graphical form.

life-gui [-h] [-g n] [-s] [-tx low..high] [-ty low..high] [-wy low..high] [-wx low..high] [file]

Options:
-h              displays a help screen
-g n            advances the life file to the n-th generation
-s              the size that each tile should be in the window, default is 10
-tx low..high   changes the terrain x values in the file to the values specified by low..high
                if the window is not specified in the command line it will be set to the whole terrain
-ty low..high   changes the terrain y values in the file to the values specified by low..high
                if the window is not specified in the command line it will be set to the whole terrain
-wx low..high   changes the window x values in the file to the values specified by low..high\n";
-wy low..high   changes the window y values in the file to the values specified by low..high
file            this will be the file that is read
                if ommited file will be read from standard input
