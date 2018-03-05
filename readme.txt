In order to execute this program, make sure that the makefile, source code (main.c) 
and mytest.dat are in the same directory. 

1) Type "make" into the command line. This should compile the program using gcc compiler.
	a) If the make command does not work, type "gcc -o p3 main.c -lpthread -lrt" into the command line. 
2) Type ./p3 into the command line. This will execute the program.

***** NOTE *****
This project was created on Mac OS X using Text Wrangler as the text editor
It was compiled and and tested on osnode01.csee.usf.edu. 
It has not been tested on either a Windows or Linux machine.

OS X has deprecated the sem_init() and sem_destroy() functions, so these functions 
had to be replaced when testing on my personal machine, but were added back for 
testing on the OS nodes.