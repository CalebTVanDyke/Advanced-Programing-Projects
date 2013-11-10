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
