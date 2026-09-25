##########################
### Project definition ###
##########################
cmake_minimum_required(VERSION 3.2)

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
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcan.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanRL.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanRR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanFR.cpp")
list(APPEND SOURCE_ALL "${BORDNET_DECODER_PROJ}/BaseSTLAVcanFL.cpp")


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
	SET(CONF_DIR "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
	SET(CONF_DIR "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8) 
add_executable (${PROJECTNAME} "") 


set(BOOST_INC "${BORDNET_DECODER_PROJ}/../BOOST/Include")


target_link_libraries(${PROJECTNAME} PRIVATE CrossPlatform)   

#link libraries
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

if(TARGET STLA_SRR6P_VCAN_Extractor)
	target_link_libraries (${PROJECTNAME} PRIVATE STLA_SRR6P_VCAN_Extractor)
endif(TARGET STLA_SRR6P_VCAN_Extractor)

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


if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")

		set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS15")		
		find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug")
		find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release")
		
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015")
	
		set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS15")		
		find_library(pugixml_debug pugixml_debug1 HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug")		
		find_library(pugixml_release pugixml_Release HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release")		
	
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
	
		set(PUGIXML_LIB_PATH "${COMMON_INC}/PUGI_VS19")		
		find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Debug")		
		find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}/Release")		
	
	else()

		set(PUGIXML_LIB_PATH "${COMMON_INC}/pugixml/binaries")
		find_library(pugixml_debug pugixml_dbg HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
		find_library(pugixml_release pugixml HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")

	endif()
else()

	set(PUGIXML_LIB_PATH "${COMMON_INC}/pugixml/binaries")
	find_library(pugixml_debug libpugixml_dbg.a HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")
	find_library(pugixml_release libpugixml.a HINTS "${PUGIXML_LIB_PATH}/${CONF_DIR}")

endif(MSVC)

# Structure Alignment
if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 16 2019")
		target_compile_definitions(${PROJECTNAME} PRIVATE  WINDOWS_IGNORE_PACKING_MISMATCH) 
	endif()
endif(MSVC)

target_link_libraries(${PROJECTNAME} PRIVATE debug "${pugixml_debug}"
									 optimized "${pugixml_release}"
									 )

									 									 