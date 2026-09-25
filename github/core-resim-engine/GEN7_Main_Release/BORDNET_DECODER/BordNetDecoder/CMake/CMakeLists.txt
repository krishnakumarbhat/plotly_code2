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
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseScan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanLow.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanEcu.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanBpilLeft.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanBpilRight.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseLow.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseMid.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSomip.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanBpil.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanLow.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanLowRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanLowRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanLowFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanLowFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanMid.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanMidFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanMidFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanMidRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BasePcanMidRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanMANFc.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanMANLeft.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseVcanMANRight.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseNissan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseNissanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseNissanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseNissanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseNissanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaPcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaPcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaPcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaVcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaVcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaVcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseHondaVcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLASmallVcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLASmallVcanFC.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLASmallVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLASmallVcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAFLR4Ethernet.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAFLR4PSomeip.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAFLR4PADCAM.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2SomeIp.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2SomeIpSRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2SomeIpFLR7.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2SomeIpIPF_FAR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2DLT.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseBMWL2DLTIPNEXT.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseTratonVcan.cpp")  #TRATON
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseTRATONVcanFC.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseTRATONVcanRR.cpp") 
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseTRATONVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseTRATONRTRange.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNAVcan.cpp")		#RNA
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNAVcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNAVcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNAVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNAVcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseRNARTRange.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcan.cpp") #CEER
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERPcanFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7Someip.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7SomeipRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7SomeipRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7SomeipFL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7SomeipFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseCEERFLR7SomeipFLR.cpp")


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
#Traton VCAN
if(TARGET TRATON_SRR6P_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE TRATON_SRR6P_VCAN_Extractor)
endif(TARGET TRATON_SRR6P_VCAN_Extractor)
if(TARGET PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE PCAN_Extractor)
endif(TARGET PCAN_Extractor)

if(TARGET SCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE SCAN_Extractor)
endif(TARGET SCAN_Extractor)

if(TARGET VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE VCAN_Extractor)
endif(TARGET VCAN_Extractor)

if(TARGET SOME_IP_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE SOME_IP_Extractor) 
endif(TARGET SOME_IP_Extractor)

if(TARGET VCAN_MAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE VCAN_MAN_Extractor)
endif(TARGET VCAN_MAN_Extractor)

if(TARGET NISSAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE NISSAN_Extractor)
endif(TARGET NISSAN_Extractor)

if(TARGET HONDA_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE HONDA_PCAN_Extractor)
endif(TARGET HONDA_PCAN_Extractor)
if(TARGET HONDA_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE HONDA_VCAN_Extractor)
endif(TARGET HONDA_VCAN_Extractor)

if(TARGET RNA_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE RNA_VCAN_Extractor)
endif(TARGET RNA_VCAN_Extractor)

if(TARGET CEER_PCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_PCAN_Extractor)
endif(TARGET CEER_PCAN_Extractor)

if(TARGET Auto_Gen_Files)
	target_link_libraries (${PROJECTNAME} PRIVATE Auto_Gen_Files)
endif(TARGET Auto_Gen_Files)

if(TARGET STLA_SRR6P_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_SRR6P_VCAN_Extractor)
endif(TARGET STLA_SRR6P_VCAN_Extractor)

if(TARGET STLA_SMALL_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_SMALL_VCAN_Extractor)
endif(TARGET STLA__SMALL_SRR7P_VCAN_Extractor)
if(TARGET STLA_FLR4_ETH_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_FLR4_ETH_Extractor)
endif(TARGET STLA_FLR4_ETH_Extractor)

if(TARGET STLA_FLR4P_SOMEIP_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_FLR4P_SOMEIP_Extractor)
endif(TARGET STLA_FLR4P_SOMEIP_Extractor)

if(TARGET CEER_FLR7_SOMEIP_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE CEER_FLR7_SOMEIP_Extractor)
endif(TARGET CEER_FLR7_SOMEIP_Extractor)
if(TARGET STLA_FLR4P_ADCAM_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_FLR4P_ADCAM_Extractor)
endif(TARGET STLA_FLR4P_ADCAM_Extractor)

if(TARGET BMWL2_SOMEIP_Decoder_Wrapper)
	target_link_libraries (${PROJECTNAME} PRIVATE BMWL2_SOMEIP_Decoder_Wrapper)
endif(TARGET BMWL2_SOMEIP_Decoder_Wrapper)

if(TARGET BMWL2_DLT_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE BMWL2_DLT_Extractor)
endif(TARGET BMWL2_DLT_Extractor)

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
