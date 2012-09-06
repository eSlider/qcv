To install the source code you will need to install first 
OpenCV 2.X and Qt 4.X. You will also need OpenGL and GLU.
To obtain the code from SourceForge.net you will need 
mercurial as well.

If you run Ubuntu, these lines should provide you with the 
required dependencies:

> sudo apt-get install libhighgui-dev libcvaux-dev freeglut3-dev mercurial

Get the code from SourceForge.net

> hg clone http://hg.code.sf.net/p/qcv/code qcv-code

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

Set the LD_LIBRARY_PATH variable so that the qcv library 
can be found:

> export LD_LIBRATY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH

Check the example binaries installed in $HOME/local/bin.

> cd $HOME/local/bin
> ./toyClockExample
> ./checkStereoPair left.pgm right.pgm
> ./sobelExample pathtoimages/*.png
> ./imgViewer --cam path1toimages/*.png --cam path2toimages/*.jpg

You need to provide valid images as parameters.
