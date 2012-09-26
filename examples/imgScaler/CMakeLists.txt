######### ImgScaler ###########

project(imgScaler CXX C)
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

# Include directories before QGLViewer otherwise headers will be found first in 
include_directories( "$ENV{QCV_DIR}/" )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( "$ENV{QCV_DIR}/include" )
include_directories( "$ENV{QCV_DIR}/modules/paramEditor" )
include_directories( "$ENV{QCV_DIR}/modules/sequencer" )
include_directories( "$ENV{QCV_DIR}/modules/operators" )


#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBCHECKIMGSCALERPAIR_SRC
                main.cpp )

##########################

add_definitions(${QT_DEFINITIONS})

add_executable ( imgScalerExample ${LIBCHECKIMGSCALERPAIR_SRC} )

target_link_libraries(imgScalerExample ${QT_LIBRARIES} 
                             -lglut -lGL -lGLU
                             ${QCVOperators_LIB} 
                             ${QCVParamEditor_LIB} 
                             ${QCVSequencer_LIB} 
                             ${QCV_LIB}
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${OpenCV_LIBS})

### Set binary to be installed under bin directory
install(TARGETS imgScalerExample RUNTIME DESTINATION bin)

