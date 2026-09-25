##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

set(PROJECTNAME  "ORCASToolVerification")
project(${PROJECTNAME})

#include custom specific
#set(SOURCES "${PROJECTNAME}/NativeBoardNetDecoderClientAppVCANBpilLeft.cpp")

add_executable (${PROJECTNAME} "") 

set(ORCASToolVerification_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(INTERFACEFILESPATH "${CMAKE_CURRENT_LIST_DIR}/..")

set(SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppSCAN.cpp") 
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppSOMIP.cpp")
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppVCANBpilLeft.cpp")
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppVCANBpilRight.cpp")
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppVCANECU.cpp")
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppPCANLow.cpp")
list(APPEND SOURCES "${ORCASToolVerification_PATH}/NativeBoardNetDecoderClientAppPCANMid.cpp")

target_sources(${PROJECTNAME} PRIVATE ${SOURCES})
set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECTNAME} PUBLIC ${INTERFACEFILESPATH})


if(MSVC)
	target_link_libraries (${PROJECTNAME} )
else()
	target_link_libraries (${PROJECTNAME} ${CMAKE_DL_LIBS}) #${CMAKE_DL_LIBS} for dlopen and dlclose lib for Linux build
endif()


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

if(MSVC)
target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)  
endif(MSVC)

if(UNIX)
	if(BUILD_FOR_32_BIT)
		set_target_properties($${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)







