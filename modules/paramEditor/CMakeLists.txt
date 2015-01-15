######### CVFRAMEWORK ###########

cmake_minimum_required(VERSION 2.6)
project (QCVParamEditor)

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

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../.." )
include_directories( "$ENV{QCV_DIR}/" )
include_directories( "$ENV{QCV_DIR}/include" )

#################################################

include(${QT_USE_FILE})

##### SOURCE FILES

set ( LIBQCVParamEditor_SRC
        3DVectorParam.cpp
        3DVectorParamEditor.cpp
        boolParam.cpp
        boolParamEditor.cpp
        buttonParam.cpp
        buttonParamEditor.cpp
        colorParam.cpp
        colorParamEditor.cpp
        dbl2DParam.cpp
        dbl2DParamEditor.cpp
        dirPathParam.cpp
        displayStateParam.cpp
        displayStateParamEditor.cpp
        doubleEditor.cpp
        doubleParam.cpp
        doubleParamEditor.cpp
        enumParamBase.cpp
        enumParamEditor.cpp
        filePathParam.cpp
        filePathParamEditor.cpp
        floatParam.cpp
        floatParamEditor.cpp
        flt2DParam.cpp
        flt2DParamEditor.cpp
        int2DParam.cpp
        int2DParamEditor.cpp
        intEditor.cpp
        intParam.cpp
        intParamEditor.cpp
        paramEditorDlg.cpp
        parameter.cpp
        parameterSet.cpp
        paramGroup.cpp
        paramGroupEnd.cpp
        paramIOPFile.cpp
        paramIOXmlFile.cpp
        paramLineSeparator.cpp
        paramTreeItemModel.cpp
        stringOp.cpp
        stringParam.cpp
        stringParamEditor.cpp
        uint2DParam.cpp
        uint2DParamEditor.cpp
        uintEditor.cpp
        uintParam.cpp
        uintParamEditor.cpp
)

set ( LIBQCVParamEditor_HEADERS 
        3DVectorParamEditor.h
        3DVectorParam.h
        baseParamEditorWidget.h
        boolParamEditor.h
        boolParam.h
        buttonParamEditor.h
        buttonParam.h
        ceParameter.h
	colorParamEditor.h
        colorParam.h
        dbl2DParamEditor.h
        dbl2DParam.h
        dirPathParam.h
        displayStateParamEditor.h
        displayStateParam.h
        doubleEditor.h
        doubleParamEditor.h
        doubleParam.h
        enumParamBase.h
        enumParamEditor.h
        enumParam.h
        enumParam_inline.h
        filePathParamEditor.h
        filePathParam.h
        floatParamEditor.h
        floatParam.h
        flt2DParamEditor.h
        flt2DParam.h
        int2DParamEditor.h
        int2DParam.h
        intEditor.h
        intParamEditor.h
        intParam.h
	    my_unordered_map.h
        paramBaseConnector.h
        paramEditorDlg.h
        parameter.h
        parameterSet.h
        paramGroupEnd.h
        paramGroup.h
        paramIOPFile.h
        paramIOXmlFile.h
        paramIOHandling.h
        paramLineSeparator.h
        paramMacros.h
        paramTreeItemModel.h
        stringOp.h
        stringParamEditor.h
        stringParam.h
        uint2DParamEditor.h
        uint2DParam.h
        uintEditor.h
        uintParamEditor.h
        uintParam.h
)  

set ( LIBQCVParamEditor_MOC_HEADERS 
    3DVectorParamEditor.h
    baseParamEditorWidget.h
    baseParamEditorWidget.h
    boolParamEditor.h
    buttonParamEditor.h
    colorParamEditor.h
    dbl2DParamEditor.h
    displayStateParamEditor.h
    doubleEditor.h
    doubleParamEditor.h
    enumParamEditor.h
    filePathParamEditor.h
    floatParamEditor.h
    flt2DParamEditor.h
    int2DParamEditor.h
    intEditor.h
    intParamEditor.h
    paramEditorDlg.h
    paramTreeItemModel.h
    stringParamEditor.h
    uint2DParamEditor.h
    uintEditor.h
    uintParamEditor.h
 )

# Wrap ui files.
QT4_WRAP_UI ( LIBQCVParamEditor_FORMS_HEADERS
              ${LIBQCVParamEditor_FORMS} )

# Mocify files.
QT4_WRAP_CPP ( LIBQCVParamEditor_HEADERS_MOC
               ${LIBQCVParamEditor_MOC_HEADERS} )


############################ 

add_definitions(${QT_DEFINITIONS})

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

### Generated library
add_library ( qcvpeditor SHARED ${LIBQCVParamEditor_SRC}
                         ${LIBQCVParamEditor_HEADERS_MOC}
                          )

target_link_libraries(qcvpeditor  ${QT_LIBRARIES} 
                                      ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY} ${OpenCV_LIBS})

### Set library to be installed under lib directory
install ( TARGETS qcvpeditor LIBRARY DESTINATION lib)

### Set header to be installed under include/qcv
install (FILES ${LIBQCVParamEditor_HEADERS} DESTINATION include/qcv/paramEditor )

#####################################

install(FILES DESTINATION bin/ PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
