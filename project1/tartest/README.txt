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