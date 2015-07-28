######### CVFRAMEWORK ###########

cmake_minimum_required(VERSION 2.6)
project (qcv)
if (POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QCV_BINARY_DIR}/bin)

#set default build type
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  message(STATUS "Setting build type to: ${CMAKE_BUILD_TYPE}")
endif()

#################################################
#PATHS
#################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#################################################
#DEPENDENCIES
#################################################

#Qt
find_package(Qt4 REQUIRED)

##################################
# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Fix OpenGL variables
if(EXISTS OPENGL_FOUND)
    set(OpenGL_FOUND ${OPENGL_FOUND})
endif(EXISTS OPENGL_FOUND)
if(EXISTS OPENGL_LIBRARIES)
    set(OpenGL_LIBRARIES ${OPENGL_LIBRARIES})
endif(EXISTS OPENGL_LIBRARIES)

set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)

##################################
# OpenCV
if("${CMAKE_SYSTEM}" MATCHES "Darwin")
      # add paths for OS X + Macports + OpenCV
      list(APPEND CMAKE_MODULE_PATH "/opt/local/lib/cmake/" "/opt/local/lib/"  "/opt/local/lib/cmake" "/opt/local/share/OpenCV")
      set(OpenCV_DIR "/opt/local/lib/cmake/")
      message(STATUS "OpenCV_DIR:${OpenCV_DIR} manually set for Darwin OpenCV dependency")
endif()

find_package ( OpenCV REQUIRED )
if(NOT EXISTS OPENCV_FOUND)
  set(OPENCV_FOUND ${OpenCV_FOUND})
endif(NOT EXISTS OPENCV_FOUND)
if(NOT EXISTS OpenCV_FOUND)
  set(OpenCV_FOUND ${OPENCV_FOUND})
endif(NOT EXISTS OpenCV_FOUND)

if(OPENCV_FOUND)
  set(OpenCV_LIBRARIES ${OpenCV_LIBS})
  include_directories(${OpenCV_INCLUDE_DIRS})
endif(OPENCV_FOUND)

##################################
# OPENMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

##################################
# QGLViewer

find_package( QGLViewer  )

# Include directories before QGLViewer otherwise headers will be found first in 
# QGLViewer directory (for example camera.h).

include_directories( "$ENV{QCV_DIR}/" )
include_directories( "$ENV{QCV_DIR}/include" )

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCV_SRC
     cinterface.cpp
     clock.cpp
     clock.cpp
     clockHandler.cpp
     clockTreeDlg.cpp
     clockTreeItemModel.cpp
     clockTreeNode.cpp
     clockTreeView.cpp
     colorEncoding.cpp
     colors.cpp
     display.cpp
     displayCEImageList.cpp
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
     eventHandlerBase.cpp
     helpWidget.cpp
     imagePyramid.cpp
     imgRemapper.cpp
     lineList.cpp
     node.cpp
     numericalSolver.cpp
     polygonList.cpp
     rectList.cpp
     simpleWindow.cpp
     textList.cpp
     triangleList.cpp
     windowListItemModel.cpp
     windowListView.cpp
)

set ( LIBQCV_HEADERS 
     3DMatrix.h
     3DMatrix_inline.h
     3DPointVector.h
     3DRowVector.h
     3DRowVector_inline.h
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
     display.h
     displayCEImageList.h
     displayImageList.h
     displayState.h
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
     eventHandlerBase.h
     events.h
     glheader.h
     helpWidget.h
     imagePyramid.h
     imgRemapper.h
     lineList.h
     linePlotter.h
     linePlotter_inline.h
     medf_inline.h
     node.h
     numericalSolver.h
     polygonList.h
     pose2ScreenMapper.h
     rectList.h
     s2d.h
     simpleWindow.h
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
     eventHandlerBase.h
     helpWidget.h
     simpleWindow.h
     windowListView.h
     windowListItemModel.h
)

if(QGLVIEWER_FOUND)
  list(APPEND LIBQCV_SRC
        glViewer.cpp
	3DPointList.cpp
	3DLineList.cpp
	3DMeshList.cpp )

  list ( APPEND LIBQCV_HEADERS
        glViewer.h
	3DDrawingElementList.h
	3DPointList.h 
	3DLineList.h
	3DMeshList.h )

  list ( APPEND LIBQCV_MOC_HEADERS 
        glViewer.h )

  add_definitions(-DHAVE_QGLVIEWER)

  list(APPEND OPT_INCLUDES  ${QGLVIEWER_INCLUDE_DIR} )
  list(APPEND OPT_LIBRARIES ${QGLVIEWER_LIBRARIES} )
endif()

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

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} ${OPT_INCLUDES})

### Generated library
add_library ( qcv SHARED ${LIBQCV_SRC} ${QT_RCC_SRCS}
                         ${LIBQCV_HEADERS_MOC}
			 )
target_link_libraries(qcv  ${QT_LIBRARIES} 
                           ${OPENGL_LIBRARIES}
                           ${GLUT_glut_LIBRARY}
                           ${OpenCV_LIBS}
                           ${OPT_LIBRARIES} )


### Set library to be installed under lib directory
install ( TARGETS qcv LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCV_HEADERS} DESTINATION include/qcv )

#####################################

add_subdirectory ( modules )
add_subdirectory ( examples )

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
