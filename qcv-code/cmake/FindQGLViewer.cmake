#  Simple QGLViewer finder
#
#  Output will be
#  QGLVIEWER_FOUND
#  QGLVIEWER_INCLUDE_DIR
#  QGLVIEWER_LIBRARIES

find_path(QGLVIEWER_INCLUDE_DIR 
         NAMES QGLViewer/qglviewer.h
         PATHS ${QGLVIEWERROOT}
         $ENV{QGLVIEWERROOT}
         /usr/include
         /usr/local/include
         $ENV{HOME}/local/include
         )

if(QGLVIEWER_INCLUDE_DIR)
  message( "Found QGLViewer include-dir: ${QGLVIEWER_INCLUDE_DIR}")
else()
  message( "Could NOT find QGLViewer include-dir.")
endif()


find_library(QGLVIEWER_LIBRARY_RELEASE 
             NAMES qglviewer-qt4 qglviewer QGLViewer QGLViewer2
             PATHS 
             ${QGLVIEWERROOT}
             $ENV{QGLVIEWERROOT}
             ENV LD_LIBRARY_PATH
             ENV LIBRARY_PATH
             /usr/lib
             /usr/local/lib
             PATH_SUFFIXES QGLViewer QGLViewer/release
  )

if(QGLVIEWER_LIBRARY_RELEASE)
  message( "Found QGLViewer library: ${QGLVIEWER_LIBRARY_RELEASE}")
else()
  message( "Could NOT find QGLViewer library.")
endif()

find_library(QGLVIEWER_LIBRARY_DEBUG
             NAMES dqglviewer dQGLViewer dQGLViewer2
             PATHS ${QGLVIEWERROOT}
             $ENV{QGLVIEWERROOT}
             ENV LD_LIBRARY_PATH
             ENV LIBRARY_PATH
             /usr/lib
             /usr/local/lib
             PATH_SUFFIXES QGLViewer QGLViewer/debug      
  )

if(QGLVIEWER_LIBRARY_DEBUG)
  message( "Found QGLViewer library (debug): ${QGLVIEWER_LIBRARY_DEBUG}")
else()
  message( "Could NOT find QGLViewer library (debug).")
endif()

if(QGLVIEWER_LIBRARY_RELEASE)
  if(QGLVIEWER_LIBRARY_DEBUG)
    set(QGLVIEWER_LIBRARIES_ optimized ${QGLVIEWER_LIBRARY_RELEASE} debug ${QGLVIEWER_LIBRARY_DEBUG})
  else()
    set(QGLVIEWER_LIBRARIES_ ${QGLVIEWER_LIBRARY_RELEASE})
  endif()
  
  set(QGLVIEWER_LIBRARIES ${QGLVIEWER_LIBRARIES_} CACHE FILEPATH "The QGLViewer library")
endif()

IF(QGLVIEWER_INCLUDE_DIR AND QGLVIEWER_LIBRARIES)
  SET(QGLVIEWER_FOUND TRUE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_QGLVIEWER")
ENDIF(QGLVIEWER_INCLUDE_DIR AND QGLVIEWER_LIBRARIES)

IF(QGLVIEWER_FOUND)
  IF(NOT QGLViewer_FIND_QUIETLY)
    message( "QGLViewer found: ${QGLVIEWER_LIBRARIES}")
  ENDIF(NOT QGLViewer_FIND_QUIETLY)
ELSE(QGLVIEWER_FOUND)
  IF(QGLViewer_FIND_REQUIRED)
    message(FATAL_ERROR "QGLViewer could not be found")
  ENDIF(QGLViewer_FIND_REQUIRED)
ENDIF(QGLVIEWER_FOUND)

