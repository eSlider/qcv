#  Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
# 
#  This file is part of QCV
# 
#  QCV is under the terms of the GNU Lesser General Public License
#  version 3. See the GNU LGPL version 3 for details.
#  QCV is distributed "AS IS" without ANY WARRANTY, without even the
#  implied warranty of merchantability or fitness for a particular
#  purpose. 
# 
#  In no event shall the authors or contributors be liable
#  for any direct, indirect, incidental, special, exemplary, or
#  consequential damages arising in any way out of the use of this
#  software.
# 
#  By downloading, copying, installing or using the software you agree
#  to this license. Do not download, install, copy or use the
#  software, if you do not agree to this license.


#  CONFIG += qt warn_on thread debug
CONFIG          += qt warn_on thread release

#CONFIG          -= release
#CONFIG          += debug

DESTDIR         = lib
TEMPLATE	= lib

MOC_DIR		= moc/
OBJECTS_DIR	= obj/
UI_DIR		= uic/

# Sources
HEADERS       =     cinterface.h \
                    clipLine.h \
                    clock.h \
                    clockHandler.h \
                    clockTreeDlg.h \
                    clockTreeItemModel.h \
                    clockTreeNode.h \
                    clockTreeView.h \
                    colorEncoding.h \
                    colors.h \
                    displayCEImageList.h \
                    display.h \
                    displayImageList.h \
                    displayTreeDlg.h \
                    displayTreeItemModel.h \
                    displayTreeNode.h \
                    displayTreeView.h \
                    displayWidget.h \
                    drawingElementList.h \
                    drawingList.h \
                    drawingListHandler.h \
                    drawingListPreview.h \
                    ellipseList.h \
                    eventHandler.h \
                    events.h \
                    glheader.h \
                    helpWidget.h \
                    ioObj.h \
                    lineList.h \
                    linePlotter.h \
                    linePlotter_inline.h \
                    mainWindow.h \
                    node.h \
                    polygonList.h \
                    rectList.h \
                    s2d.h \
                    standardTypes.h \
                    textList.h \
                    triangleList.h \
                    windowListItemModel.h \
                    windowListView.h 

SOURCES =           clock.cpp \
                    clockHandler.cpp \
                    clockTreeDlg.cpp \
                    clockTreeItemModel.cpp \
                    clockTreeNode.cpp \
                    clockTreeView.cpp \
                    colorEncoding.cpp \
                    colors.cpp \
                    displayCEImageList.cpp \
                    display.cpp \
                    displayImageList.cpp \
                    displayTreeDlg.cpp \
                    displayTreeItemModel.cpp \
                    displayTreeNode.cpp \
                    displayTreeView.cpp \
                    displayWidget.cpp \
                    drawingList.cpp \
                    drawingListHandler.cpp \
                    drawingListPreview.cpp \
                    ellipseList.cpp \
                    eventHandler.cpp \
                    helpWidget.cpp \
                    lineList.cpp \
                    mainWindow.cpp \
                    node.cpp \
                    polygonList.cpp \
                    rectList.cpp \
                    textList.cpp \
                    triangleList.cpp \
                    windowListItemModel.cpp \
                    windowListView.cpp

# OpenCV (recommend to run "pkg-config --libs opencv" and "pkg-config --cflags opencv" to find out the correct values).
# OPENCV_PATH       = 
OPENCV_INC         = /usr/local/include/opencv -I/usr/local/include
OPENCV_LIB         = -L/usr/local/lib.a -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann

INCLUDEPATH   += $${OPENCV_INC}
LIBS          += $${OPENCV_LIB}

LIBS          +=  -lglut

RESOURCES     = res/symbols.qrc

QT            *= opengl xml

LIBS 	         += -lgomp
QMAKE_CXXFLAGS   += -fopenmp
QMAKE_LFLAGS     += -fopenmp

# install

target.path = lib
sources.files = $$SOURCES $$HEADERS
sources.path = $$[LOCAL_PATH]
INSTALLS += target sources

TARGET = qcv
