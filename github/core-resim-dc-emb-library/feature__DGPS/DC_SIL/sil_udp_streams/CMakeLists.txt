set(udp_recu "recu_streams")
add_library(${udp_recu} STATIC "")

# add src files
target_sources(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/RECU/recu_stream_log.cpp
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/recu_stream_log.h
								   )
# add hdr files
target_sources(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_CORE0.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_CORE1.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_CORE3.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_internal_CORE1.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_vru_classifier.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/radar_ecu_calibration.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/OSI_Input_Structure.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/DSPACE_Common_Input_Structure.h
								   ${CMAKE_CURRENT_SOURCE_DIR}/RECU/udp_record.h
								   )
								   
target_compile_definitions(${udp_recu} PRIVATE _XKEYCHECK_H)
if(${BUILD_MODE} STREQUAL "mrr_dc")
    target_compile_definitions(${udp_recu} PRIVATE MRR_DC)
elseif(${BUILD_MODE} STREQUAL "srr_dc")
    target_compile_definitions(${udp_recu} PRIVATE SRR_DC)
endif()

# inc headers for other library
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/RECU)   
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../sil_engine_headers)
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../sil_smc)
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../sil_wrappers) 
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/VSE_Core/include)		
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/rspp/include)
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/OLP_Core/includes)
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/core) 		
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Logging) 		
target_include_directories(${udp_recu} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/F360Tracker/F360TrackerLib/SharedTrackerAPI/Types) 

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Interface)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc/data_ports)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Platform_Abstraction_Layer/f360/inc)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/validation)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/trajectory_prediction)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/reference_point)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Shared_Feature_Functions/zone)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Calibration/Feature_Building_Kit_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Feature_Building_Kit/Interface/index_lookup)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Calibration_Tool/c_src)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/Math_Library/ml_core/include)


target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/LCDA/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/LCDA/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/LCDA/Calibration/LCDA_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/LCDA/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/CED/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/CED/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/CED/Calibration/CED_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/CED/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/PT/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/PT/Calibration/PT_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/PT/Calibration/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Application/FeatureFunctions/PT/Customer_Adapter/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/SCW/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/SCW/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/SCW/Calibration/SCW_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/SCW/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/TA/Customer_Adapter)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/TA/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/TA/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/TA/Calibration/TA_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/TA/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/RECW/Customer_Adapter)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/RECW/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/RECW/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/RECW/Calibration/RECW_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/RECW/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/CTA/Customer_Adapter)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/CTA/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/CTA/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/CTA/Calibration/CTA_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/CTA/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/LTB/Customer_Adapter)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/LTB/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/LTB/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/LTB/Calibration/LTB_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/LTB/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/ESA/Calibration/ESA_Core)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/ESA/Customer_Adapter)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/ESA/Source)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/ESA/Customer_Adapter/Generic)
target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/ESA/Calibration/Generic)

target_include_directories(${udp_recu} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../Application/FeatureFunctions/Mock_Files/SIL_Library)

target_link_libraries(${udp_recu} PUBLIC MLMathLibrary)