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
#CEER_SOMEIP
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeip.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeipFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeipFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeipRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeipRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeipFLR.cpp")
#MCIP
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcan.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMCIPPcanFLR.cpp")

list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeIpFLR7.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERSomeIpSRR.cpp")

#AL
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseALPcan.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseALPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseALPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseALPcanFLR.cpp")

#GPO_GEN8
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_Pcan.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_PcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_PcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_PcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_PcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseGPO_GEN8_PcanFLR.cpp")



if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (UNIX)
add_definitions(${CMAKE_CFLAGS} "-O2")
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




# ======================== HDF5 Portable Integration =========================

# ✅ FIXED ROOT PATH (important)
set(HDF_ROOT ${CMAKE_SOURCE_DIR}/../Common_Headers/HDF)

# Common include
target_include_directories(${PROJECTNAME}
    PRIVATE
    ${HDF_ROOT}/include
)

# -------------------- Linux --------------------
if(UNIX)
    target_link_directories(${PROJECTNAME}
        PRIVATE
        ${HDF_ROOT}/linux/lib
    )

    target_link_libraries(${PROJECTNAME}
        PRIVATE
        CrossPlatform

        # ✅ wrap EVERYTHING together
        -Wl,--start-group

        # ✅ your static libs (VERY IMPORTANT)
        CEER_PCAN_Extractor
        CEER_SOMEIP_Decoder_Wrapper

        # ✅ HDF5 libs
        hdf5_cpp
        hdf5
        z
        sz

        -Wl,--end-group
    )

    set_target_properties(${PROJECTNAME} PROPERTIES
        BUILD_RPATH "${HDF_ROOT}/linux/lib"
    )
endif()

# -------------------- Windows --------------------
if(WIN32)
    target_link_libraries(${PROJECTNAME}
        PRIVATE
        CrossPlatform
        ${HDF_ROOT}/windows/lib/RELEASE/hdf5_cpp.lib
        ${HDF_ROOT}/windows/lib/RELEASE/hdf5.lib
        ${HDF_ROOT}/windows/lib/RELEASE/zlib-static.lib
    )

    # Copy DLL directory safely
    add_custom_command(TARGET ${PROJECTNAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${HDF_ROOT}/windows/lib/DLL"
            $<TARGET_FILE_DIR:${PROJECTNAME}>
    )
endif()



#link libraries
if(TARGET CEER_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_PCAN_Extractor)
endif(TARGET CEER_PCAN_Extractor)

if(TARGET MCIP_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE MCIP_PCAN_Extractor)
endif(TARGET MCIP_PCAN_Extractor)

if(TARGET CEER_SOMEIP_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_SOMEIP_Extractor)
endif(TARGET CEER_SOMEIP_Extractor)

if(TARGET CEER_SOMEIP_Decoder_Wrapper)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_SOMEIP_Decoder_Wrapper)
endif(TARGET CEER_SOMEIP_Decoder_Wrapper)

if(UNIX AND BORDNET_HDF5_LINK_LIBS)
    # Keep HDF5 after static decoder libs to satisfy Linux link-order resolution.
    target_link_libraries(${PROJECTNAME} PRIVATE ${BORDNET_HDF5_LINK_LIBS})
endif()

if(TARGET GPO_GEN8_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE GPO_GEN8_PCAN_Extractor)
endif(TARGET GPO_GEN8_PCAN_Extractor)

if(TARGET CEER_SOMEIP_Decoder_Wrapper)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_SOMEIP_Decoder_Wrapper)
endif(TARGET CEER_SOMEIP_Decoder_Wrapper)

if(TARGET AL_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE AL_PCAN_Extractor)
endif(TARGET AL_PCAN_Extractor)

#target_link_libraries(BordNetDecoder PRIVATE AL_PCAN_Extractor)

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
        set(PUGIXML_FALLBACK_PATH "${COMMON_INC}/pugixml/binaries/${CONF_DIR}")
        set(HDF5_LIB_PATH "${COMMON_INC}/HDF/lib")

        set(PUGIXML_DEBUG_CANDIDATE "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug/pugixml_dbg.lib")
        set(PUGIXML_RELEASE_CANDIDATE "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release/pugixml.lib")
        if(EXISTS "${PUGIXML_DEBUG_CANDIDATE}")
            set(pugixml_debug "${PUGIXML_DEBUG_CANDIDATE}")
        else()
            find_library(pugixml_debug NAMES pugixml_dbg HINTS "${PUGIXML_FALLBACK_PATH}")
        endif()
        if(EXISTS "${PUGIXML_RELEASE_CANDIDATE}")
            set(pugixml_release "${PUGIXML_RELEASE_CANDIDATE}")
        else()
            find_library(pugixml_release NAMES pugixml HINTS "${PUGIXML_FALLBACK_PATH}")
        endif()
       
        find_library(hdf_cpp_debug hdf5_cpp HINTS "${HDF5_LIB_PATH}/DEBUG")
        find_library(hdf_cpp_release hdf5_cpp HINTS "${HDF5_LIB_PATH}/RELEASE")

    elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")

        set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS19")
        set(PUGIXML_FALLBACK_PATH "${COMMON_INC}/pugixml/binaries/${CONF_DIR}")
        set(HDF5_LIB_PATH "${COMMON_INC}/HDF/lib")

        set(PUGIXML_DEBUG_CANDIDATE "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug/pugixml_dbg.lib")
        set(PUGIXML_RELEASE_CANDIDATE "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release/pugixml.lib")
        if(EXISTS "${PUGIXML_DEBUG_CANDIDATE}")
            set(pugixml_debug "${PUGIXML_DEBUG_CANDIDATE}")
        else()
			find_library(pugixml_debug NAMES pugixml_dbg HINTS "${PUGIXML_FALLBACK_PATH}")
        endif()
        if(EXISTS "${PUGIXML_RELEASE_CANDIDATE}")
            set(pugixml_release "${PUGIXML_RELEASE_CANDIDATE}")
        else()
			find_library(pugixml_release NAMES pugixml HINTS "${PUGIXML_FALLBACK_PATH}")
        endif()
      
		find_library(hdf_cpp_debug hdf5_cpp HINTS "${HDF5_LIB_PATH}/DEBUG")
		find_library(hdf_cpp_release hdf5_cpp HINTS "${HDF5_LIB_PATH}/RELEASE")

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
    if(NOT pugixml_debug)
        message(FATAL_ERROR "Missing pugixml debug library: expected pugixml_dbg.lib in ${COMMON_INC}/PUGI_VS19/${CONF_DIR}/Debug or ${COMMON_INC}/pugixml/binaries/${CONF_DIR}")
    endif()
    if(NOT pugixml_release)
        message(FATAL_ERROR "Missing pugixml release library: expected pugixml.lib in ${COMMON_INC}/PUGI_VS19/${CONF_DIR}/Release or ${COMMON_INC}/pugixml/binaries/${CONF_DIR}")
    endif()
    if(NOT hdf_cpp_debug)
        message(FATAL_ERROR "Missing HDF5 debug C++ library: expected hdf5_cpp.lib in ${COMMON_INC}/HDF/lib/DEBUG")
    endif()
    if(NOT hdf_cpp_release)
        message(FATAL_ERROR "Missing HDF5 release C++ library: expected hdf5_cpp.lib in ${COMMON_INC}/HDF/lib/RELEASE")
    endif()

    target_link_libraries(${PROJECTNAME} PRIVATE
        debug "${pugixml_debug}" "${hdf_cpp_debug}"
        optimized "${pugixml_release}" "${hdf_cpp_release}")
else()
    target_link_libraries(${PROJECTNAME} PRIVATE
        debug "${pugixml_debug}"
        optimized "${pugixml_release}")
endif()
