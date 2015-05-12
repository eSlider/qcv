
cmake_minimum_required(VERSION 2.8.6)
project(StereoVisOdom CXX C)

##################################################################################
#  PROJECT OPTIONS
##################################################################################

##################################################################################
# Needed dependencies
##################################################################################

#OpenCV
if (NOT OpenCV_INCLUDE_DIRS OR NOT OpenCV_LIBS)
  message(FATAL_ERROR "OpenCV_INCLUDE_DIRS and/or OpenCV_LIBS are not defined")
endif()

# QGlViewer
if (NOT QGLVIEWER_INCLUDE_DIR OR NOT QGLVIEWER_LIBRARIES)
  message(STATUS "QGLVIEWER_INCLUDE_DIR or QGLVIEWER_LIBRARIES not defined")
endif()

# QT
if (NOT QT_FOUND OR NOT QT_INCLUDE_DIR OR NOT QT_LIBRARIES)
  message(STATUS "QT_FOUND = ${QT_FOUND} QT_INCLUDE_DIR = ${QT_INCLUDE_DIR} QT_LIBRARIES= ${QT_LIBRARIES}")
  message(FATAL_ERROR "QT_FOUND or QT_INCLUDE_DIR or QT_LIBRARIES not defined")
endif()

# OPENGL
if (NOT OPENGL_INCLUDE_DIR OR NOT OPENGL_LIBRARIES OR NOT OPENGL_gl_LIBRARY)
  message(FATAL_ERROR "OPENGL_INCLUDE_DIR or OPENGL_LIBRARIES or OPENGL_gl_LIBRARY not defined")
endif()

# QCV
if (NOT QCV_INCLUDE_DIRS OR NOT QCV_LIBRARIES)
  message(FATAL_ERROR "QCV_INCLUDE_DIRS or QCV_LIBRARIES not defined")
endif()

##################################################################################
# Add compilation directories
##################################################################################
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${QT_INCLUDE_DIRS} )
include_directories( ${OPENGL_INCLUDE_DIR} )
include_directories( ${QCV_INCLUDE_DIRS} )

if(QGLVIEWER_FOUND)
  include_directories( ${QGLVIEWER_INCLUDE_DIR} )
endif()

add_subdirectory(stereoSFM)

##################################################################################
# Exports
##################################################################################
if(NOT ${CMAKE_PROJECT_NAME} STREQUAL StereoVisOdo)
  set(StereoVisOdo_LIBRARIES      ${StereoVisOdo_LIBRARIES} PARENT_SCOPE)
  set(StereoVisOdo_INCLUDE_DIRS   ${CMAKE_CURRENT_SOURCE_DIR}    PARENT_SCOPE)
endif()
