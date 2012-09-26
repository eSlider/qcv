######### Stereo ###########

project(stereo CXX C)
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

#OpenCV
find_package ( OpenCV REQUIRED )

#OpenGL
#find_package(GLU REQUIRED)

#Qcv
set (QCV_LIB qcv )
set (QCVParamEditor_LIB qcvpeditor )
set (QCVSequencer_LIB qcvsequencer )
set (QCVOperators_LIB qcvoperators )

# Include directories
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/paramEditor" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/sequencer" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/operators" )


#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBCHECKSTEREOPAIR_SRC
                main.cpp )

##########################

add_definitions(${QT_DEFINITIONS})

add_executable ( stereoExample ${LIBCHECKSTEREOPAIR_SRC} )

target_link_libraries(stereoExample ${QT_LIBRARIES} 
                             -lglut -lGL -lGLU
                             ${QCVOperators_LIB} 
                             ${QCVParamEditor_LIB} 
                             ${QCVSequencer_LIB} 
                             ${QCV_LIB}
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${OpenCV_LIBS})

### Set binary to be installed under bin directory
install(TARGETS stereoExample RUNTIME DESTINATION bin)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sequence.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sequence.xml COPYONLY)
