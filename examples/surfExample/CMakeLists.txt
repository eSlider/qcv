######### Surf ###########

project(surf CXX C)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_BUILD_TYPE RELEASE)

#################################################
#DEPENDENCIES
#################################################

#Qt
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)
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


#Qcv
set (QCV_LIB qcv )
set (QCVParamEditor_LIB qcvpeditor )
set (QCVSequencer_LIB qcvsequencer )
set (QCVOperators_LIB qcvoperators )
set (QCVMisc_LIB        qcvmisc )

# Include directories
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/paramEditor" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/sequencer" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/operators" )


#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBCHECKIMGSCALERPAIR_SRC
                main.cpp )

##########################

add_definitions(${QT_DEFINITIONS})

add_executable ( surfExample ${LIBCHECKIMGSCALERPAIR_SRC} )

target_link_libraries(surfExample ${QT_LIBRARIES} 
                             ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
                             ${QCVOperators_LIB} 
                             ${QCVMisc_LIB} 
                             ${QCVParamEditor_LIB} 
                             ${QCVSequencer_LIB} 
                             ${QCV_LIB}
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${OpenCV_LIBS})

### Set binary to be installed under bin directory
install(TARGETS surfExample RUNTIME DESTINATION bin)

