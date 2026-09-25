##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 4.0)

set(PROJECTNAME  "BordNetDecoder")
project(${PROJECTNAME})
#Linking directories
#link_directories(${OUTPUT_PATH})

#include custom specific
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/Source.cpp")
#list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/Source_old.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/Decoder.h")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/Decoder.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/Resource.rc")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/ReleaseNotes_BordnetTool.txt")
list(APPEND SOURCE_ALL "${CROSS_PLATFORM_PROJ}/genutil.h")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVariant.h")
#CEER
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcan.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanFLR.cpp")
#MCIP
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcan.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFLR.cpp")


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (UNIX)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_SKIP_INSTALL_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'\$ORIGIN'")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,'\$ORIGIN'")
endif()


add_executable (${PROJECTNAME} "") 


set(BOOST_INC "${BORDNET_DECODER_PROJ}/../BOOST/Include")


target_link_libraries(${PROJECTNAME} PRIVATE CrossPlatform)  

# ======================== HDF5 Integration =========================
if(UNIX)
  target_link_libraries(${PROJECTNAME} PRIVATE "${OUTPUT_PATH}/libhdf5_cpp.so")  
  target_link_libraries(${PROJECTNAME} PRIVATE "${OUTPUT_PATH}/libhdf5.so")   
endif()


#link libraries
if(TARGET CEER_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_PCAN_Extractor)
endif(TARGET CEER_PCAN_Extractor)

if(TARGET MCIP_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE MCIP_PCAN_Extractor)
endif(TARGET MCIP_PCAN_Extractor)

if(TARGET Auto_Gen_Files)
	#target_link_libraries (${PROJECTNAME} PRIVATE Auto_Gen_Files)
endif(TARGET Auto_Gen_Files)

#link include directories
target_include_directories(${PROJECTNAME} PUBLIC ${COMMON_INC})


target_include_directories(${PROJECTNAME} PRIVATE ${COMMON_INC}/pugixml ${BOOST_INC})




target_sources(${PROJECTNAME} PRIVATE ${SOURCE_ALL})

if(OUTPUT_PATH)
set_target_properties(${PROJECTNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
												ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PATH}") 				
endif(OUTPUT_PATH)


# Multi Byte Character Set
if(MSVC)
	target_compile_definitions(${PROJECTNAME} PRIVATE _MBCS) 
endif(MSVC)

if(UNIX)
	#For Debugging in Linux...
	target_compile_options(${PROJECTNAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-g>")
endif(UNIX)


# MSVC-specific configurations
if(MSVC)
    # Set Windows SDK version
    if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")

        set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS15")
       
        find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug")
        find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release")
        find_library(hdf_lib hdf5_cpp HINTS "${OUTPUT_PATH}${CMAKE_BUILD_TYPE}")

    elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")

        set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS19")
      
        find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug")
        find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release")
		find_library(hdf_lib hdf5_cpp HINTS "${OUTPUT_PATH}${CMAKE_BUILD_TYPE}")

        # Add preprocessor definition to ignore packing mismatch warnings
        target_compile_definitions(${PROJECTNAME} PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH)

    else()
        set(PUGIXML_LIB_PATH "${COMMON_INC}/pugixml/binaries")
        find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
        find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
    endif()

    # Apply default packing alignment (/Zp8) globally
    add_compile_options(/Zp8)

else()
    # Non-MSVC configuration
    set(PUGIXML_LIB_PATH "${COMMON_INC}/pugixml/binaries")
    
    find_library(pugixml_debug libpugixml_dbg.a HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
    find_library(pugixml_release libpugixml.a HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
endif()

# Structure Alignment for Windows SDK
if(MSVC)
    if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
        target_compile_definitions(${PROJECTNAME} PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH)
    endif()
endif()

# Link Libraries
if(MSVC)
    target_link_libraries(${PROJECTNAME} PRIVATE
        debug "${pugixml_debug}" "${hdf_lib}"
        optimized "${pugixml_release}")
else()
    target_link_libraries(${PROJECTNAME} PRIVATE
        debug "${pugixml_debug}"
        optimized "${pugixml_release}")
endif()
