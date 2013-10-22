STRATEGRY
---------
My overall strategy of this project was to be able to read in the file into a common format.  This format is the File_Data format specified in util.h.  Using this I can write that in any form I want.  I can also easily manipulate the data without having to worry about the file format.  I read the samples from both files into an int**.  The dimensions of this are [samples][channels].  This allows me to easily paste new samples in, cut samples out, and also gives me random access to specific samples.  For sndedit I keep track of the top sample, top channel, bottom sample, and bottom channel.  I adjust these accordingly as the user scrolls through the file using the arrow keys, page up/down, or when they use the go to function.  I use the same strategy keeping track of the marked sample.  Using the range of marked samples I can manipulate the data stored in the int** to represent what the user wants.



BREAKDOWN OF FILES
------------------
AIFF.c
------
   - Contains all functions that read from AIFF files
   - Contains functions that display the AIFF files on the screen (used in sndedit and sndshow)
CS229.c
-------
   - Contains all functions that read from CS229 files
   - Contains functions that display the CS229 files on the screen (used in sndedit and sndshow)

snd*.c
------
   - All files that begin with snd contain the main function for their respective program
   - They control calling the correct functions to perform the desired activity
util.c
------
   - Contains functions that are to be used in both AIFF and CS229 files
   - Functions are not dependent on what format file is being used



PROGRAM DESCRIPTIONS
--------------------

sndedit
------------------------------------------------------------
sndedit lets you edit a sound file 

MOVEMENT
--------
up/down: moves the cursor up or down
pgdw: moves the bottom sample to the top of the screen
pgup: moves the top sample to the bottom of the screen

MANIPULATION
------------
m: marks the current sample movment will now highlight all sample between the marked sample and your cursor
c: copys the marked samples into a buffer
x: cuts the marked samples into a buffer (removes the marked samples from the file)
^: inserts the buffered samples above the sample your cursor is currently on
v: inserts the buffered samples below the sample your cursor is currently on



sndinfo
------------------------------------------------------------
sndinfo displays info about a CS229 or AIFF audio file.
Will display information such as the duration of the file, channels, sample, sample rate, and file name.

sndinfo [<file> ...]
sndinfo [-1h]

Options:
   -1     have the program prompt you to enter file name

   -h     help: information on how to use the program

Default:
   If no options are use the files can be specified as command line arguments



sndconv
------------------------------------------------------------
sndconv converts audio files of the form CS229 and AIFF.
Can convert between the forms CS229 and AIFF if -1 is not specified sndconv will take its input from stdin and output to stdout.

sndconv [-1ach]

Options:
   -1     program will prompt you to enter the input file and output file name

   -a     forces output to be in AIFF format

   -c     forces output to be in CS229 format

   -h     help: information on how to use the program

Default:
   If -a or -c are not specified then output will be in the opposite form of the input



sndcat
------------------------------------------------------------
sndcat combines audio files of the form CS229 and AIFF
sndcat takes will output to stdout.
Files must have the same number of channels, the same bit depth, and the same sample rate.

sndcat [-ach] [<file> ...]

Options:
   -a     forces output to be in AIFF format.

   -c     forces output to be in CS229 format.

   -h     help: information on how to use the program

Default:
   If -a or -c are not specified then output will be in the same form as the first input



sndcut
------------------------------------------------------------
sndcut will cut the specified lines from the file starting with line 0
Any number of high low combinations can be specified.

sndcat [-h] [low..high] [low..high] ...

Options:
   -h     help: information on how to use the program



sndshow
------------------------------------------------------------
sndshow reads a file from stdin and displays a textual version of it.

sndshow [-w w] [-c c] [-z n]

Options:
   -w w    How many characters should be printed on each line. Default is 80.

   -c c    Only display a specific channel. Default will display all channels.

   -z n    Zoom out by a factor of n. If not specifed, the default is n = 1.

   -h      help: information on how to use the program