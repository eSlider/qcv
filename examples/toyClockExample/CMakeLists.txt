######### ToyClockExample ###########

project(toyClockExample CXX C)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_VERBOSE_MAKEFILE true)

#################################################
#DEPENDENCIES
#################################################

#Qt
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)
find_package(Qt4 REQUIRED)

#OpenCV
find_package ( OpenCV REQUIRED )
message ( STATUS  ${OpenCV_LIBS} )

#OpenGL
find_package(GLU REQUIRED)

#Qcv
set (QCV_LIB qcv )

# Include directories before QGLViewer otherwise headers will be found first in 
include_directories( "$ENV{QCV_DIR}/" )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( "$ENV{QCV_DIR}/include" )


#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBTOYCLOCKEXAMPLE_SRC
                toyClockExample.cpp )

##########################

add_definitions(${QT_DEFINITIONS})

add_executable ( toyClockExample ${LIBTOYCLOCKEXAMPLE_SRC} )

target_link_libraries(toyClockExample ${QT_LIBRARIES} 
                                       -lglut -lGL -lGLU
                                       ${QCV_LIB} 
				       ${CMAKE_THREAD_LIBS_INIT} 
				       ${OpenCV_LIBS})

### Set binary to be installed under bin directory
install(TARGETS toyClockExample RUNTIME DESTINATION bin)

