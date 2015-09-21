#####################################################################
#
# Find QCV
#
# 1: Output Variables
#
# The following are set after configuration is done:
#
#  QCV_FOUND
#  QCV_INCLUDE_DIRS
#  QCV_LIBRARIES
#  QCV_IDLPP
#
#  Author: Hernan Badino
#
#####################################################################

if (QCV_LIBRARIES AND QCV_INCLUDE_DIRS)
    # in cache already
    set (QCV_FOUND TRUE)
else (QCV_LIBRARIES AND QCV_INCLUDE_DIRS)

    set (QCV_LIBRARIES_OPTIONAL "") 
    set (QCV_INCLUDE_DIRS_OPTIONAL "") 

    # find include paths
    find_path (QCV_INC_DIR
               NAMES drawingList.h
               PATHS "$ENV{QCV_DIR}"
                     "$ENV{QCV_DIR}/include/qcv"
                     "${CMAKE_SOURCE_DIR}/external/install/qcv/include/qcv"
                     "${CMAKE_SOURCE_DIR}/../qcv-code"
                     "${CMAKE_SOURCE_DIR}/../qcv"
    )
  message(STATUS "QCV_INC_DIR is ${QCV_INC_DIR}")
    find_path (QCV_OP_DIR
               NAMES imgScalerOp.h
	       PATHS "$ENV{QCV_DIR}"
                     "$ENV{QCV_DIR}/modules/operators"
                     "$ENV{QCV_DIR}/include/qcv/operators"
                     "${CMAKE_SOURCE_DIR}/external/install/qcv/include/qcv/operators"
                     "${CMAKE_SOURCE_DIR}/../qcv-code/modules/operators"
                     "${CMAKE_SOURCE_DIR}/../qcv/modules/operators"
    )
  message(STATUS "QCV_OP_DIR is ${QCV_OP_DIR}")

    find_path (QCV_SEQ_DIR
               NAMES seqController.h
	       PATHS "$ENV{QCV_DIR}"
                     "$ENV{QCV_DIR}/modules/sequencer"
                     "$ENV{QCV_DIR}/include/qcv/sequencer"
                     "${CMAKE_SOURCE_DIR}/external/install/qcv/include/qcv/sequencer"
                     "${CMAKE_SOURCE_DIR}/../qcv-code/modules/sequencer"
                     "${CMAKE_SOURCE_DIR}/../qcv/modules/sequencer"
    )
  message(STATUS "QCV_SEQ_DIR is ${QCV_SEQ_DIR}")

    find_path (QCV_PEDT_DIR
               NAMES parameter.h
	       PATHS "$ENV{QCV_DIR}"
                     "$ENV{QCV_DIR}/modules/paramEditor"
                     "$ENV{QCV_DIR}/include/qcv/paramEditor"
                     "${CMAKE_SOURCE_DIR}/external/install/qcv/include/qcv/paramEditor"
                     "${CMAKE_SOURCE_DIR}/../qcv-code/modules/paramEditor"
                     "${CMAKE_SOURCE_DIR}/../qcv/modules/paramEditor"
    )
  message(STATUS "QCV_PEDT_DIR is ${QCV_PEDT_DIR}")

    find_path (QCV_MISC_DIR
               NAMES camera.h
	       PATHS "$ENV{QCV_DIR}"
                     "$ENV{QCV_DIR}/modules/misc"
                     "$ENV{QCV_DIR}/include/qcv/misc"
                     "${CMAKE_SOURCE_DIR}/external/install/qcv/include/qcv/misc"
                     "${CMAKE_SOURCE_DIR}/../qcv-code/modules/misc"
                     "${CMAKE_SOURCE_DIR}/../qcv/modules/misc"
    )

  message(STATUS "QCV_MISC_DIR is ${QCV_MISC_DIR}")

    # find library paths
    find_library (QCV_MAIN_LIB
        NAMES qcv
        PATHS "$ENV{QCV_DIR}" "$ENV{QCV_DIR}/build/" "$ENV{QCV_DIR}/lib" "$ENV{QCV_DIR}/build/lib" "$ENV{QCV_DIR}/build/lib" "${CMAKE_SOURCE_DIR}/external/install/qcv/lib" "${CMAKE_SOURCE_DIR}/../qcv-code/build/"
    )
    # find library paths
    find_library (QCV_OP_LIB
        NAMES qcvoperators
        PATHS "$ENV{QCV_DIR}" "$ENV{QCV_DIR}/build/modules/operators" "$ENV{QCV_DIR}/build/lib" "$ENV{QCV_DIR}/lib" "${CMAKE_SOURCE_DIR}/external/install/qcv/lib"  "${CMAKE_SOURCE_DIR}/../qcv-code/build/modules/operators"
    )
    # find library paths
    find_library (QCV_SEQ_LIB
        NAMES qcvsequencer
        PATHS "$ENV{QCV_DIR}" "$ENV{QCV_DIR}/build/modules/sequencer" "$ENV{QCV_DIR}/lib" "$ENV{QCV_DIR}/build/lib" "${CMAKE_SOURCE_DIR}/external/install/qcv/lib"  "${CMAKE_SOURCE_DIR}/../qcv-code/build/modules/sequencer"
    )
    # find library paths
    find_library (QCV_PEDT_LIB
        NAMES qcvpeditor
        PATHS "$ENV{QCV_DIR}" "$ENV{QCV_DIR}/build/modules/paramEditor" "$ENV{QCV_DIR}/lib" "$ENV{QCV_DIR}/build/lib" "${CMAKE_SOURCE_DIR}/external/install/qcv/lib"  "${CMAKE_SOURCE_DIR}/../qcv-code/build/modules/paramEditor"
    )
    # find library paths
    find_library (QCV_MISC_LIB
        NAMES qcvmisc
        PATHS "$ENV{QCV_DIR}" "$ENV{QCV_DIR}/build/modules/misc"  "$ENV{QCV_DIR}/lib" "$ENV{QCV_DIR}/build/lib" "${CMAKE_SOURCE_DIR}/external/install/qcv/lib" "${CMAKE_SOURCE_DIR}/../qcv-code/build/modules/misc"
    ) 

    # set paths
    set ( QCV_INCLUDE_DIRS 
    	  ${QCV_INC_DIR} 
	  ${QCV_OP_DIR} 
	  ${QCV_SEQ_DIR} 
	  ${QCV_PEDT_DIR}
	  ${QCV_MISC_DIR} )

    set (QCV_LIBRARIES 
         ${QCV_MAIN_LIB}
	 ${QCV_OP_LIB} 
	 ${QCV_SEQ_LIB} 
	 ${QCV_PEDT_LIB}
	 ${QCV_MISC_LIB} )


       MESSAGE(STATUS "${QCV_LIBRARIES}")
    # set FOUND variable
    if ( QCV_INC_DIR AND QCV_OP_DIR AND QCV_SEQ_DIR AND QCV_PEDT_DIR AND QCV_MISC_DIR AND 
         QCV_MAIN_LIB AND QCV_OP_LIB AND  QCV_SEQ_LIB AND QCV_PEDT_LIB AND QCV_MISC_LIB )
        set(QCV_FOUND TRUE)

        set (QCV_LIBRARIES_OPTIONAL ${QCV_LIBRARIES}) 
        set (QCV_INCLUDE_DIRS_OPTIONAL ${QCV_INCLUDE_DIRS}) 
    endif ( )

    # for debugging
    if (QCV_FOUND)
        if (NOT QCV_FIND_QUIETLY)
            message (STATUS "Found QCV:")
            message (STATUS " - Includes: ${QCV_INCLUDE_DIRS}")
            message (STATUS " - Libraries: ${QCV_LIBRARIES}")
        endif (NOT QCV_FIND_QUIETLY)
        add_definitions (-DHAVE_QCV)
    else (QCV_FOUND)
        if (QCV_FIND_REQUIRED)
            message (FATAL_ERROR "Could not find QCV")
        endif (QCV_FIND_REQUIRED)
    endif (QCV_FOUND)

    mark_as_advanced(QCV_INCLUDE_DIRS QCV_LIBRARIES)

endif (QCV_LIBRARIES AND QCV_INCLUDE_DIRS)
