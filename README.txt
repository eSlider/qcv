To install the source code you will need to install first 
OpenCV 2.X and Qt 4.X. You will also need OpenGL and glut.
To obtain the code from SourceForge.net you need mercurial
as well. CMake is required to compile the code.

If you run Ubuntu, these lines should provide you with the 
required dependencies:

> sudo apt-get install freeglut3-dev mercurial libqt4-dev cmake
> sudo add-apt-repository ppa:gijzelaar/opencv2
> sudo apt-get update
> sudo apt-get install opencv
> sudo apt-get install libopencv-dev

Get the code now from SourceForge.net

> hg clone http://hg.code.sf.net/p/qcv/code qcv

or download the zip file from here:

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
> ./helloWorld
> ./toyClockExample
> ./checkStereoPair imgs/left.pgm imgs/right.pgm
> ./sobelExample imgs/seq/*c0*.jpg
> ./imgViewer --cam imgs/seq/*c0*.jpg --cam imgs/seq/*c1*.jpg
> ./anaglyphStereo --left imgs/seq/*c0*.jpg --right imgs/seq/*c1*.jpg
> ./stereoExample
> ./imgScalerExample somevideo.mpg
> ./sobelExample2 somevideo.mpg
> ./surfExample somevideo.mpg

You will need to provide valid videos as parameters.

