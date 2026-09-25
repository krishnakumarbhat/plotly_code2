#This CMake is created for linking autogen_smc as a static library.

set(sil_smc "autogen_smc")
add_library(${sil_smc} STATIC "")

# add hdr files
target_sources(${sil_smc} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSMCParameter.h)
								   
# add src files
target_sources(${sil_smc} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenBMWFLR7SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenBMWMRR3SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenBMWSRR5PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenBMWSRR7PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenCEERFLR7SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenCEERSRR7PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenCHANGANSRR5SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenFORDMRR3SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenGPOSRR7PlusUWBSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenHKMCSRR5SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenHONDASRR6PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenMTNLFLR4PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenNISSANSRR6SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenRNASRR5SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSCANIASRR3SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSTLAFLR4PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSTLAFLR4SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSTLAFLR7SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSTLASRR6PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenSTLASRR7PlusSMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenTMLSRR5SMCParameter.cpp
								  ${CMAKE_CURRENT_SOURCE_DIR}/AutoGenTRATONSRR6PlusSMCParameter.cpp)
								   
								   
target_include_directories(${sil_smc} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)   
