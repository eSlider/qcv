QCV v00.60

To install the source code you will need to install first 
OpenCV 2.X and Qt 4.X. You will also need OpenGL and glut.
To obtain the code from SourceForge.net you need mercurial
as well. CMake and g++ are required to compile the code.

If you run Ubuntu 10.x or 11.x, these lines should provide
you with the required dependencies:

> sudo apt-get install freeglut3-dev mercurial libqt4-dev cmake g++
> sudo add-apt-repository ppa:gijzelaar/opencv2
> sudo apt-get update
> sudo apt-get install opencv
> sudo apt-get install libopencv-dev

If you want 3D display capability, you will need to install
QGLViewer

> sudo apt-get install libqglviewer-qt4-dev

If you run another Ubuntu version (including 12.x) or
another Linux distribution, you will need to download 
OpenCV and install it in your system.

Now, get the code now from SourceForge.net

> hg clone http://hg.code.sf.net/p/qcv/code qcv

or download the tar.gz (tar.bz2 or zip) file from here:

http://sourceforge.net/projects/qcv/?source=directory

To compile the code, cd to the source code directory:

> cd qcv

make a directory called build

> mkdir build
> cd build

and run cmake

> cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/local

If anything went wrong, solve the dependency issues. 
Otherwise just run

> make

That should install the library and binarires into your
local build directory, i.e. $HOME/qcv/build.

Check the example binaries installed in the bin directory

> cd $HOME/qcv/build/bin

# C examples (qcv core only)
> ./helloWorld
> ./toyClockExample
> ./checkStereoPair imgs/left.pgm imgs/right.pgm
> ./sobelExample imgs/seq/*c0*.jpg
> ./imgViewer --cam imgs/seq/*c0*.jpg --cam imgs/seq/*c1*.jpg
> ./anaglyphStereo --left imgs/seq/*c0*.jpg --right imgs/seq/*c1*.jpg

# C++ examples (operators, editor and sequencer)
> ./stereoExample
> ./imgScalerExample somevideo.mpg
> ./sobelExample2 somevideo.mpg
> ./surfExample sequence.xml
> ./houghTransformExample sequence.xml

You will need to provide valid videos as parameters,
(or just use the provided image sequence by using
"sequence.xml" instead).

Plase, send comments or questions to 
hernan.badino@gmail.com
