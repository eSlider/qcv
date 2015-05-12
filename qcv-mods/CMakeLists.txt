
cmake_minimum_required(VERSION 2.8)
project(QCVAPPS CXX C)

##################################################################################
# set cmake find path (default)
##################################################################################
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR})

##################################################################################
# set PROJECT OPTIONS
##################################################################################

#Set default install directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#set default build type
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  message(STATUS "Setting build type to: ${CMAKE_BUILD_TYPE}")
endif()

#This line forces all binaries to be in a bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##################################################################################
# finding externals
##################################################################################
#include(ExternalProject)

#################################################
#DEPENDENCIES
#################################################

#Qt
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)
find_package(OpenGL)  
add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})
link_directories(${QT_LIBRARY_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#OpenCV
find_package ( OpenCV REQUIRED )

#Qcv
if (NOT ENV{QCV_DIR})
  set(ENV{QCV_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../qcv-code")
  message(STATUS "Setting QCV_DIR to $ENV{QCV_DIR}")
endif()

find_package ( QCV REQUIRED )

#QGLViewer
find_package( QGLViewer  )

##################################################################################
# include directories (order matters!)
##################################################################################

# Stereo visual odometry
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stereoMotionEst AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stereoMotionEst)
  add_subdirectory(stereoMotionEst)
endif()

# Monocular visual odometry
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/monoMotionEst AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/monoMotionEst)
  add_subdirectory(monoMotionEst)
endif()

# NRI project
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nri AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/nri)
  add_subdirectory(nri)
endif()

# Green quality yield estimation
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gqYield AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gqYield)
  add_subdirectory(gqYield)
endif()

# Green quality yield estimation
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stereoSFM AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stereoSFM)
  add_subdirectory(stereoSFM)
endif()
