To install the source code you will need to install first 
OpenCV 2.X and Qt 4.X. You will also need OpenGL and GLU.
If you run Ubuntu, these lines should provide you with the 
required dependencies:

> sudo apt-get install libhighgui-dev libcvaux-dev freeglut3-dev

To compile the code, cd to the source code directory:

> cd qcv-code

make a directory called build

> mkdir build

and run cmake

> cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/local

If anything went wrong, solve the dependency issues. 
Otherwise just run

> make && make install

That should install the library under the local directory
of your home directory, i.e. $HOME/local.

Check the example binaries installed in $HOME/local/bin.

> cd $HOME/local/bin
> ./toyClockExample

and

> ./checkStereoPair left.pgm right.pgm

You need to provide full paths to the files left.pgm 
and right.pgm. 
