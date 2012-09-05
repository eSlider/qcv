#  CONFIG += qt warn_on thread debug
CONFIG             += qt warn_on thread release

# OpenCV (recommend to run "pkg-config --libs opencv" and "pkg-config --cflags opencv" to find out the correct values).
#OPENCV_PATH       = 
OPENCV_INC         = /usr/local/include/opencv -I/usr/local/include  
OPENCV_LIB         = -L/usr/local/lib.a -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann


CONFIG          -= release
CONFIG          += debug

DESTDIR         = .
TEMPLATE	= app

MOC_DIR		= moc/
OBJECTS_DIR	= obj/
UI_DIR		= uic/

HEADERS       = 


SOURCES       = \
                imgPlayer.cpp

INCLUDEPATH   += ..
LIBS	      += -L ../../lib -lqcv


INCLUDEPATH   += $${OPENCV_INC}
LIBS          += $${OPENCV_LIB}

QT           += opengl xml
# 
LIBS 	          += -lgomp
QMAKE_CXXFLAGS   += -fopenmp -fpermissive
QMAKE_LFLAGS     += -fopenmp

# install

target.path = .
sources.files = $$SOURCES $$HEADERS
sources.path = $$[LOCAL_PATH]
INSTALLS += target sources

TARGET = imgPlayer

