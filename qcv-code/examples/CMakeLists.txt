add_subdirectory ( helloWorld )
add_subdirectory ( toyClockExample )
add_subdirectory ( checkStereoPair )
add_subdirectory ( sobelExample )
add_subdirectory ( imgPlayer )
add_subdirectory ( anaglyphStereo )
add_subdirectory ( stereoExample )
add_subdirectory ( imgScaler )
add_subdirectory ( sobelExample2 )
add_subdirectory ( surfExample )
add_subdirectory ( houghTransformExample )
add_subdirectory ( gfttFreakExample )
add_subdirectory ( stereoTrackerExample )

#add_subdirectory ( histogram )
#add_subdirectory ( voExample )
#add_subdirectory ( nriProject )

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/imgs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgs COPYONLY)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/imgs DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

