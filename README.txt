To install the source code you will need to install first 
OpenCV 2.X and Qt 4.X. You will also need OpenGL and GLU.
To obtain the code from SourceForge.net you need mercurial
as well. CMake is required to compile the code.

If you run Ubuntu, these lines should provide you with the 
required dependencies:

> sudo apt-get install freeglut3-dev mercurial libqt4-dev cmake
> sudo add-apt-repository ppa:gijzelaar/opencv2
> sudo apt-get update
> sudo apt-get install opencv
> sudo apt-get install libopencv-dev

Get the code from SourceForge.net

> hg clone http://hg.code.sf.net/p/qcv/code qcv-code

or download the zip file from here:

http://sourceforge.net/projects/qcv/?source=directory

To compile the code, cd to the source code directory:

> cd qcv-code

(or just cd qcv if you downloaded the zip file),

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

You will need to provide valid images as 
parameters.

