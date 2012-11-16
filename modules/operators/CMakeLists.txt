######### CVFRAMEWORK ###########

cmake_minimum_required(VERSION 2.6)
project (QCVOperators)

if (POLICY CMP0017)
  cmake_policy(SET CMP0017 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_VERBOSE_MAKEFILE true)

#################################################
#DEPENDENCIES
#################################################

#Qcv
set (QCV_LIB qcv )
set (QCVParamEditor_LIB qcvpeditor )
set (QCVSequencer_LIB qcvsequencer )

#Qt
find_package(Qt4 REQUIRED)

#OpenCV
find_package ( OpenCV REQUIRED )

# Include directories before QGLViewer otherwise headers will be found first in 
# QGLViewer directory (for example camera.h).

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/paramEditor")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/sequencer")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/misc" )

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCVOperators_SRC
     linearHoughTransform.cpp
     houghTransformOp.cpp
     imgScalerOp.cpp
     sobelOp.cpp
     stereoOp.cpp
     surfOp.cpp
)

set ( LIBQCVOperators_HEADERS 
     linearHoughTransform.h
     houghTransformOp.h
     imgScalerOp.h
     sobelOp.h
     stereoOp.h
     surfOp.h
)  

set ( LIBQCVOperators_MOC_HEADERS  "" )

set ( LIBQCVOperators_FORMS "" )

# Wrap ui files.
QT4_WRAP_UI ( LIBQCVOperators_FORMS_HEADERS
              ${LIBQCVOperators_FORMS} )

# Mocify files.
QT4_WRAP_CPP ( LIBQCVOperators_HEADERS_MOC
               ${LIBQCVOperators_MOC_HEADERS} )


############################ 

add_definitions(${QT_DEFINITIONS})

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

### Generated library
add_library ( qcvoperators SHARED ${LIBQCVOperators_SRC}
                           ${LIBQCVOperators_FORMS_HEADERS}
                           ${LIBQCVOperators_HEADERS_MOC} )

target_link_libraries(qcvoperators  ${QT_LIBRARIES} 
                                    -lglut -lGL ${OpenCV_LIBS})

### Set library to be installed under lib directory
install ( TARGETS qcvoperators LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCVOperators_HEADERS} DESTINATION include/qcv/operators )

#####################################

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
