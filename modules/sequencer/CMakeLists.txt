######### CVFRAMEWORK ###########

cmake_minimum_required(VERSION 2.6)
project (QCVSequencer)

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

#Qt
find_package(Qt4 REQUIRED)

#OpenCV
find_package ( OpenCV REQUIRED )

# Include directories before QGLViewer otherwise headers will be found first in 
# QGLViewer directory (for example camera.h).

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/paramEditor")

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCVSequencer_SRC
     mainWindow.cpp
     operatorBase.cpp
     seqControlDlg.cpp
     seqControler.cpp
     seqDevHDImg.cpp
     seqDevVideoCapture.cpp
)

set ( LIBQCVSequencer_HEADERS 
     deviceOpConnector.h
     imageFromFile.h
     mainWindow.h
     matVector.h
     operatorBase.h
     operator.h
     qcvVector.h
     seqControlDlg.h
     seqControler.h
     seqDevHDImg.h
     seqDeviceControl.h
     seqDevVideoCapture.h
)  

set ( LIBQCVSequencer_MOC_HEADERS 
     mainWindow.h
     seqControlDlg.h
     seqControler.h
     seqDeviceControl.h
     seqDevHDImg.h
     seqDevVideoCapture.h
 )

set ( LIBQCVSequencer_FORMS
  gui/seqControlDlg.ui )

# Wrap ui files.
QT4_WRAP_UI ( LIBQCVSequencer_FORMS_HEADERS
              ${LIBQCVSequencer_FORMS} )

# Mocify files.
QT4_WRAP_CPP ( LIBQCVSequencer_HEADERS_MOC
               ${LIBQCVSequencer_MOC_HEADERS} )


############################ 

add_definitions(${QT_DEFINITIONS})

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

### Generated library
add_library ( qcvsequencer SHARED ${LIBQCVSequencer_SRC}
                           ${LIBQCVSequencer_FORMS_HEADERS}
                           ${LIBQCVSequencer_HEADERS_MOC} )

target_link_libraries(qcvsequencer  ${QT_LIBRARIES} 
                                      -lglut -lGL ${OpenCV_LIBS})

### Set library to be installed under lib directory
install ( TARGETS qcvsequencer LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCVSequencer_HEADERS} DESTINATION include/qcv/sequencer )

#####################################

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
