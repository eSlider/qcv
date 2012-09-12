######### CVFRAMEWORK ###########

cmake_minimum_required(VERSION 2.6)
project (QCV)
if (POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_VERBOSE_MAKEFILE true)

#################################################
#DEPENDENCIES
#################################################


#Qt
find_package(Qt4 REQUIRED)

#OpenGL
find_package(GLU REQUIRED)
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)

#OpenCV
find_package ( OpenCV REQUIRED )

# Include directories before QGLViewer otherwise headers will be found first in 
# QGLViewer directory (for example camera.h).

include_directories( "$ENV{QCV_DIR}/" )
include_directories( "$ENV{QCV_DIR}/include" )

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCV_SRC
     clock.cpp
     clock.cpp
     clockHandler.cpp
     clockTreeDlg.cpp
     clockTreeItemModel.cpp
     clockTreeNode.cpp
     clockTreeView.cpp
     colorEncoding.cpp
     colors.cpp
     displayCEImageList.cpp
     display.cpp
     displayImageList.cpp
     displayTreeDlg.cpp
     displayTreeItemModel.cpp
     displayTreeNode.cpp
     displayTreeView.cpp
     displayWidget.cpp
     drawingList.cpp
     drawingListHandler.cpp
     drawingListPreview.cpp
     ellipseList.cpp
     eventHandler.cpp
     helpWidget.cpp
     lineList.cpp
     mainWindow.cpp
     node.cpp
     polygonList.cpp
     qrc_symbols.cpp
     rectList.cpp
     textList.cpp
     triangleList.cpp
     windowListItemModel.cpp
     windowListView.cpp
)
set ( LIBQCV_HEADERS 
     clock.h
     cinterface.h
     clipLine.h
     clock.h
     clockHandler.h
     clockTreeDlg.h
     clockTreeItemModel.h
     clockTreeNode.h
     clockTreeView.h
     colorEncoding.h
     colors.h
     displayCEImageList.h
     display.h
     displayImageList.h
     displayTreeDlg.h
     displayTreeItemModel.h
     displayTreeNode.h
     displayTreeView.h
     displayWidget.h
     drawingElementList.h
     drawingList.h
     drawingListHandler.h
     drawingListPreview.h
     ellipseList.h
     eventHandler.h
     events.h
     helpWidget.h
     glheader.h
     ioObj.h
     lineList.h
     linePlotter.h
     linePlotter_inline.h
     mainWindow.h
     node.h
     polygonList.h
     rectList.h
     standardTypes.h
     textList.h
     triangleList.h
     windowListItemModel.h
     windowListView.h
 )


set ( LIBQCV_MOC_HEADERS 
     clockTreeDlg.h
     clockTreeItemModel.h
     clockTreeView.h
     display.h
     displayTreeDlg.h
     displayTreeItemModel.h
     displayTreeView.h
     displayWidget.h
     drawingListPreview.h
     eventHandler.h
     helpWidget.h
     mainWindow.h
     windowListView.h
     windowListItemModel.h
 )

# Wrap ui files.
QT4_WRAP_UI ( LIBQCV_FORMS_HEADERS
              ${LIBQCV_FORMS} )

# Mocify files.
QT4_WRAP_CPP ( LIBQCV_HEADERS_MOC
               ${LIBQCV_MOC_HEADERS} )

SET(QT_RCCS res/symbols.qrc)
QT4_ADD_RESOURCES(QT_RCC_SRCS ${QT_RCCS})

############################ 

add_definitions(${QT_DEFINITIONS})

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

### Generated library
add_library ( qcv SHARED ${LIBQCV_SRC}
                         ${LIBQCV_HEADERS_MOC}
                          )

target_link_libraries(qcv  ${QT_LIBRARIES} 
                           -lglut -lGL -lGLU ${OpenCV_LIBS})

### Set library to be installed under lib directory
install ( TARGETS qcv LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCV_HEADERS} DESTINATION include/qcv )

#####################################

add_subdirectory ( examples )

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
