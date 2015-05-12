
set ( StereoSFMLib_SRC
featKFOp.cpp
featVisKFOp.cpp
kf3DStereoPointCommon.cpp
kf3DStereoPoint.cpp
main.cpp
stereoEgoMotionOp.cpp
stereoSFMOp.cpp
)

set ( StereoSFMLib_HEADERS
featKFOp.h
featVisKFOp.h
kf3DStereoPointCommon.h
kf3DStereoPoint.h
kf3DStereoPointVector.h
stereoEgoMotionOp.h
stereoSFMOp.h
)

##########################

QT4_WRAP_CPP(StereoSFMLib_HEADERS_MOC
             ${StereoSFMLib_HEADERS}
            )

add_library(stereoSFMLib ${StereoSFMLib_SRC} ${StereoSFMLib__HEADERS_MOC} )
target_link_libraries(stereoSFMLib ${OpenCV_LIBRARIES} ${QGLVIEWER_LIBRARIES})

# install each directory into the include folder
install(FILES ${StereoSFMLib__HDRS} DESTINATION include/stereoSFMLib)
install(TARGETS stereoSFMLib ARCHIVE DESTINATION lib COMPONENT StereoSFMLibLib)

list(APPEND StereoVOEst_LIBRARIES stereoSFMLib)
set(StereoVOEst_LIBRARIES  ${StereoVOEst_LIBRARIES} PARENT_SCOPE)


############################

add_executable(stereoSFM
               main.cpp)

target_link_libraries(stereoSFM ${StereoVOEst_LIBRARIES}
                                ${QT_LIBRARIES} 
                                ${OPENGL_LIBRARIES} 
				${GLUT_LIBRARY}
                                ${OpenCV_LIBS} 
                                ${QCV_LIBRARIES}
				)

install(TARGETS stereoSFM RUNTIME DESTINATION bin)

############################
#Copy parameter files.

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/params_stereoSFM DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
