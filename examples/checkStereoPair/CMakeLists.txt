######### CheckStereoPair ###########

project(checkStereoPair CXX C)
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

#OpenGL
#find_package(GLU REQUIRED)

#Qcv
set (QCV_LIB qcv )

# Include directories
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBCHECKSTEREOPAIR_SRC
                checkStereoPair.cpp )

##########################

add_definitions(${QT_DEFINITIONS})

add_executable ( checkStereoPair ${LIBCHECKSTEREOPAIR_SRC} )

target_link_libraries(checkStereoPair ${QT_LIBRARIES} 
                                       -lglut -lGL -lGLU
                                       ${QCV_LIB} 
				       ${CMAKE_THREAD_LIBS_INIT} 
				       ${OpenCV_LIBS})

### Set binary to be installed under bin directory
install(TARGETS checkStereoPair RUNTIME DESTINATION bin)

