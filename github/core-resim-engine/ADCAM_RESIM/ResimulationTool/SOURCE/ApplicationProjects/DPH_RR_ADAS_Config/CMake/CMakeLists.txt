#Build Steps
#Complier MSVC

#Debug Mode:
#	Step1 cmake -G "Visual Studio 11 Win64" ..\CMake
#	Step2 cmake --build . --target ALL_BUILD --config Debug

#Compiler GNU CXX
#	Step1 cmake ../CMake
#	Step2 make

##########################
### Project definition ###
##########################

cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "DPH_RR_ADAS_Config")
project(${PROJECTNAME})


#Adding required common header files
set(COMMONINC "${COMMON_INC}/inc")
set(AUDI_INC "${COMMON_INC}/inc/AUDI")
set(COMMONHEADERS "${COMMON_INC}/CommonHeaders")

#Linking directories
link_directories(${OUTPUT_PATH})

#including sources and headers
set(SourceFiles "${ADAS_CONFIG_PROJ}/")
set(AUDI_FILES "${SourceFiles}/CustomConfig/AUDI/dph_rr_adas_audi_config.h")
list(APPEND AUDI_FILES "${SourceFiles}/CustomConfig/AUDI/dph_rr_adas_audi_config.cpp")

#grouping source AUDI files
source_group(\\CustomConfig\\AUDI\\HeaderFiles FILES ${AUDI_FILES})

#custom specific sources
list(APPEND BMW_FILES "${SourceFiles}/CustomConfig/BMW/dph_rr_adas_bmw_config.h")
list(APPEND BMW_FILES "${SourceFiles}/CustomConfig/BMW/dph_rr_adas_bmw_config.cpp")

#grouping source BMW files
source_group(\\CustomConfig\\BMW\\HeaderFiles FILES ${BMW_FILES})

#include sources
list(APPEND SOURCE_ALL "${SourceFiles}/dph_rr_adas_config.cpp")
list(APPEND SOURCE_ALL "${COMMONHEADERS}/dph_rr_adas_config.h")
list(APPEND SOURCE_ALL "${COMMONINC}/pugiconfig.hpp")
list(APPEND SOURCE_ALL "${COMMONINC}/pugixml.hpp")

add_library(${PROJECTNAME} STATIC "")

#link include directories
target_include_directories(${PROJECTNAME} PRIVATE ${COMMONINC} ${AUDI_INC} ${COMMONHEADERS})

target_sources(${PROJECTNAME} PRIVATE ${AUDI_FILES} ${BMW_FILES} ${SOURCE_ALL})


					
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
		target_compile_definitions(${PROJECTNAME} PRIVATE _WIN_PLATFORM)
endif(MSVC)							

if(MSVC)
if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
	set(LIB_PATH "${ADAS_CONFIG_PROJ}/lib/PUGI_VS15")
elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
   set(LIB_PATH "${ADAS_CONFIG_PROJ}/lib/PUGI_VS19")
else()
	set(LIB_PATH "${ADAS_CONFIG_PROJ}/lib/windows")
endif()
find_library(pugixml_debug pugixml_dbg HINTS "${LIB_PATH}/Debug/")
find_library(pugixml_release pugixml HINTS "${LIB_PATH}/Release/")
target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 ) 
else()						
set(LIB_PATH "${ADAS_CONFIG_PROJ}/lib/linux")
find_library(pugixml_debug libpugixml.a HINTS "${LIB_PATH}/Debug/")
find_library(pugixml_release libpugixml.a HINTS "${LIB_PATH}/Release/")
target_link_libraries(${PROJECTNAME} debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 ) 
endif()	


if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)

target_compile_definitions(${PROJECTNAME} PUBLIC USE_PUGIXML)
macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(use_cxx11)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set(CMAKE_CXX_FLAGS "-fPIC")
	set(CFLAGS"-fPIC")
	set_target_properties(${PROJECTNAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	else()
		target_compile_options(${PROJECTNAME} PRIVATE "/Zp2")
	endif()
	
endif(MSVC)
	

if(UNIX)
	set_target_properties(${PROJECTNAME} PROPERTIES
		BUILD_WITH_INSTALL_RPATH TRUE
		INSTALL_RPATH "$ORIGIN")
	if(BUILD_FOR_32_BIT)
		set_target_properties(${PROJECTNAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
		add_compile_options("-fPIC")
	endif(BUILD_FOR_32_BIT)

	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")

endif(UNIX)