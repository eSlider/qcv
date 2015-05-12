######### QCV ###########

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


# Include directories before QGLViewer otherwise headers will be found first in 
# QGLViewer directory (for example camera.h).

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/paramEditor")

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCVSequencer_SRC
     mainWindow.cpp
     operator.cpp
     seqControlDlg.cpp
     seqController.cpp
     seqDevHDImg.cpp
     seqDevVideoCapture.cpp
)

set ( LIBQCVSequencer_HEADERS 
     imageFromFile.h
     io.h
     mainWindow.h
     matVector.h
     operator.h
     qcvVector.h
     seqControlDlg.h
     seqController.h
     seqDevHDImg.h
     seqDeviceControl.h
     seqDevVideoCapture.h
)  

set ( LIBQCVSequencer_MOC_HEADERS 
     mainWindow.h
     seqControlDlg.h
     seqController.h
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
                                      ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY} ${OpenCV_LIBS} qcv qcvmisc qcvpeditor)

### Set library to be installed under lib directory
install ( TARGETS qcvsequencer LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCVSequencer_HEADERS} DESTINATION include/qcv/sequencer )

#####################################

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
