set(FEATURE_BUILDING_KIT_CAL_ROOT_DIR "${Feature_Building_Kit_ROOT_PATH}/Calibration")
set(FEATURE_BUILDING_KIT_SECTION_COMPATIBILITY "Feature_Building_Kit_Core")
set(FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR
    "${FEATURE_BUILDING_KIT_CAL_ROOT_DIR}/${FEATURE_BUILDING_KIT_SECTION_COMPATIBILITY}")
set(FBK_CAL_CUSTOMER_DIR "${FEATURE_BUILDING_KIT_CAL_ROOT_DIR}/${Feature_Building_Kit_PROJECT_VARIANT}")

# Generate the calibrations for the customer if defined
option(FEATURE_BUILDING_KIT_GENERATE_CAL_FILES "Generate cal files for Feature_Building_Kit" OFF)
if(FEATURE_BUILDING_KIT_GENERATE_CAL_FILES)
   # check for Windows operating system
   if(WIN32)
      # Generate cal files using the provided batch file
      execute_process(
         COMMAND
            cmd /c
            ${FEATURE_BUILDING_KIT_CAL_ROOT_DIR}/Feature_Building_Kit_Core/calibration_tool_for_feature_building_kit.bat
      )
   else()
      message(WARNING "Calibration file generation is only supported for Windows operating systems.")
   endif()
endif()

target_include_directories(Feature_Building_Kit PUBLIC ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR})
target_sources(Feature_Building_Kit PUBLIC ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/fbk_core_calibration_t.h)
target_sources(Feature_Building_Kit PRIVATE ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/fbk_core_calibration.h)
target_sources(Feature_Building_Kit PRIVATE ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/fbk_core_calibration_check.h)
target_sources(Feature_Building_Kit PRIVATE ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/fbk_core_calibration_check.c)
target_sources(Feature_Building_Kit PRIVATE ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/feature_building_kit_cal.xml)
target_sources(Feature_Building_Kit
               PRIVATE ${FEATURE_BUILDING_KIT_CAL_HEADER_FILE_DIR}/fbk_core_calibration_print_functions.c)

if(NOT CMAKE_CROSSCOMPILING)
   target_compile_definitions(Feature_Building_Kit PUBLIC CT_ACTIVATE_CAL_PRINT)
   if(Feature_Building_Kit_UPDATE_DEFAULT_CALS)
      target_compile_definitions(Feature_Building_Kit PUBLIC SRF_UPDATE_DEFAULT_CALIBRATION)
   endif()
endif(NOT CMAKE_CROSSCOMPILING)

target_include_directories(Feature_Building_Kit PUBLIC ${FBK_CAL_CUSTOMER_DIR})
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/Customer_Specific_Cal.xml)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_core_calibration.c)

target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration_print_functions.c)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration_t.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration_check.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration_check.c)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_customer_calibration.c)

target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration_print_functions.c)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration_t.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration_check.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration_check.c)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_public_calibration.c)

target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_update_calibration.h)
target_sources(Feature_Building_Kit PRIVATE ${FBK_CAL_CUSTOMER_DIR}/fbk_update_calibration.c)
