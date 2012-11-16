######### MISCELANOUS ###########

cmake_minimum_required(VERSION 2.6)
project (QCVMisc)

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

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCVMisc_SRC
  camera.cpp
  stereoCamera.cpp
)

set ( LIBQCVMisc_HEADERS 
  camera.h
  stereoCamera.h
)  

set ( LIBQCVMisc_MOC_HEADERS  "" )

set ( LIBQCVMisc_FORMS "" )

# Wrap ui files.
QT4_WRAP_UI ( LIBQCVMisc_FORMS_HEADERS
              ${LIBQCVMisc_FORMS} )

# Mocify files.
QT4_WRAP_CPP ( LIBQCVMisc_HEADERS_MOC
               ${LIBQCVMisc_MOC_HEADERS} )


############################ 

add_definitions(${QT_DEFINITIONS})

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

### Generated library
add_library ( qcvmisc SHARED ${LIBQCVMisc_SRC}
                           ${LIBQCVMisc_FORMS_HEADERS}
                           ${LIBQCVMisc_HEADERS_MOC} )

target_link_libraries(qcvmisc  ${QT_LIBRARIES} 
                                    -lglut -lGL ${OpenCV_LIBS})

### Set library to be installed under lib directory
install ( TARGETS qcvmisc LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCVMisc_HEADERS} DESTINATION include/qcv/misc )

#####################################

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
