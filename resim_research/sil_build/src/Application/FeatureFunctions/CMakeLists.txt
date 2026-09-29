# Set required environmental variable
cmake_minimum_required(VERSION 3.16.3)
project(FF)

# options
option(BINARY_DEBUG_ACTIVE "Write *-bin logs" "OFF")


set(SFL_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")


# Assume that everything is located within the repository

set(CUSTOMER Generic)
set(MODULE_PROJECT_VARIANT "Generic" CACHE STRING "Tag to identify the customer the module should be build for.")

set_property(
   CACHE MODULE_PROJECT_VARIANT
   PROPERTY STRINGS
            "Generic"
            )
			
# Set features to Rivian variant
set(Feature_Building_Kit_PROJECT_VARIANT ${CUSTOMER} CACHE STRING "Build FBK for Generic.")
set(PT_PROJECT_VARIANT                   ${CUSTOMER} CACHE STRING "Build PT for Generic.")
set(CED_PROJECT_VARIANT                  ${CUSTOMER} CACHE STRING "Build CED for Generic.")
set(CTA_PROJECT_VARIANT                  ${CUSTOMER} CACHE STRING "Build CTA for Generic.")
set(LCDA_PROJECT_VARIANT                 ${CUSTOMER} CACHE STRING "Build LCDA for Generic.")
set(LTB_PROJECT_VARIANT                  ${CUSTOMER} CACHE STRING "Build LTB for Generic.")
set(RECW_PROJECT_VARIANT                 ${CUSTOMER} CACHE STRING "Build RECW for Generic.")
set(SCW_PROJECT_VARIANT                  ${CUSTOMER} CACHE STRING "Build SCW for Generic.")
set(TA_PROJECT_VARIANT                   ${CUSTOMER} CACHE STRING "Build TA for Generic.")
set(INTERFACE_PROJECT_VARIANT            ${CUSTOMER} CACHE STRING "Build Interface for Generic.")
set(ESA_PROJECT_VARIANT                  ${CUSTOMER} CACHE STRING "Build ESA for Generic.")

add_compile_definitions(FEATURE_CALS_INITIALIZED)

# Set options for Math Library
set(MLMathLibrary_fast_math_exp_table "use_table" CACHE STRING "Set the method for exponential table calculation")
set(MLMathLibrary_fast_math_trig_table "use_table" CACHE STRING "Set the method for trigonometric table calculation")
set(MODULE_PA_VARIANT "GDSR")
set(GDSR_IFACE_ROOT_PATH "${SFL_ROOT_PATH}/Mock_Files/tracker_interfaces/gdsr")
set(BINARY_DEBUG_ACTIVE ON)
add_definitions(-DBINARY_DEBUG)


add_subdirectory("${GDSR_IFACE_ROOT_PATH}/Tracker_Out_Iface/Tracker_output_interface"
                       "${CMAKE_CURRENT_BINARY_DIR}/GDSR_Tracker_API")
add_subdirectory("${GDSR_IFACE_ROOT_PATH}/Z2_SRR" "${CMAKE_CURRENT_BINARY_DIR}/GDSR_Z2_SRR_API")
	  
# Include mock files for SRR_CORE_LIB (reuse.h)
add_subdirectory("${SFL_ROOT_PATH}/Mock_Files/SRR_CORE_LIB" "${CMAKE_CURRENT_BINARY_DIR}/SRR_CORE_LIB")

# Include mock files for SIL_Library (Radar_Config.h, Tracker_Wrapper.h)
add_subdirectory("${SFL_ROOT_PATH}/Mock_Files/SIL_Library" "${CMAKE_CURRENT_BINARY_DIR}/SIL_Library")

add_subdirectory(Math_Library)
add_subdirectory(Calibration_Tool/c_src)
add_subdirectory(Feature_Building_Kit)
add_subdirectory(TA)
add_subdirectory(PT)
add_subdirectory(CED)
add_subdirectory(CTA)
add_subdirectory(LCDA)
add_subdirectory(RECW)
add_subdirectory(SCW)
add_subdirectory(ESA)
add_subdirectory(LTB)

 #set_source_files_properties(
  #  ${CFILES}
   #    DIRECTORY  Interface Feature_Building_Kit  PT LCDA TA CTA CED RECW SCW
    #   PROPERTIES LANGUAGE CXX
 #)
 
 
