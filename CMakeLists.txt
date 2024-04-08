PROJECT( VascuSynth )
cmake_minimum_required(VERSION 3.0)

SET (ITK_DIR "/local-scratch/localhome/asa409/projects/VascuSynth/ITK-4.13.3/build/bin")
FIND_PACKAGE( ITK )
IF( ITK_FOUND )
	INCLUDE( ${ITK_USE_FILE} )
ENDIF (ITK_FOUND )

ADD_EXECUTABLE(VascuSynth VascuSynth.cpp SupplyMap.cpp OxygenationMap.cpp NodeTable.cpp VascularTree.cpp TreeDrawer.cpp)

TARGET_LINK_LIBRARIES( VascuSynth ${ITK_LIBRARIES} )
