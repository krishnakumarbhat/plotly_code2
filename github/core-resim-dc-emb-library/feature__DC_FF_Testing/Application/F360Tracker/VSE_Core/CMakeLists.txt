cmake_minimum_required(VERSION 3.16.3)


set(VSE_INCLUDE "include")
set(VSE_SOURCE "source")

add_library(VSE_Core)

target_include_directories(
    VSE_Core
        PUBLIC
            ${VSE_INCLUDE}
			.
		PRIVATE
			../Wrappers/Common/include
			${RSPP_IFACE_VARIANT_PATH}
			${TRACKERLIB_IFACE_VARIANT_PATH}
			${TRACKERLIB_IFACE_VARIANT_PATH}/Types
			${TRACKERLIB_IFACE_VARIANT_PATH}/core
			${TRACKERLIB_IFACE_VARIANT_PATH}/Logging
			../Wrappers/Common/include/output_adaptation
			../Wrappers/Ocg/iface
)

target_sources(
    VSE_Core 
        PRIVATE 
            ${VSE_INCLUDE}/Acceleration_VCS.h
			${VSE_INCLUDE}/Acceleration_VCS_private.h
			${VSE_INCLUDE}/Acceleration_VCS_types.h
			${VSE_INCLUDE}/BINARYSEARCH_real32_T_Aptiv.h
			${VSE_INCLUDE}/DtrmnVehStationary.h
			${VSE_INCLUDE}/DtrmnVehStationary_private.h
			${VSE_INCLUDE}/DtrmnVehStationary_types.h
			${VSE_INCLUDE}/LateralJerkEstimation.h
			${VSE_INCLUDE}/LateralJerkEstimation_private.h
			${VSE_INCLUDE}/LateralJerkEstimation_types.h
			${VSE_INCLUDE}/look1_iflf_binlx_Aptiv.h
			${VSE_INCLUDE}/LookUp_real32_T_real32_T_Aptiv.h
			${VSE_INCLUDE}/rtmodel.h
			${VSE_INCLUDE}/rtwtypes.h
			${VSE_INCLUDE}/SpeedCompensation.h
			${VSE_INCLUDE}/SpeedCompensation_private.h
			${VSE_INCLUDE}/SpeedCompensation_types.h
			${VSE_INCLUDE}/SpeedCompensationArbitration.h
			${VSE_INCLUDE}/SpeedCompensationArbitration_private.h
			${VSE_INCLUDE}/SpeedCompensationArbitration_types.h
			${VSE_INCLUDE}/SteeringWheelAngleBiasEstimation.h
			${VSE_INCLUDE}/SteeringWheelAngleBiasEstimation_private.h
			${VSE_INCLUDE}/SteeringWheelAngleBiasEstimation_types.h
			${VSE_INCLUDE}/TrailerDetection.h
			${VSE_INCLUDE}/TrailerDetection_private.h
			${VSE_INCLUDE}/TrailerDetection_types.h
			${VSE_INCLUDE}/VehicleCurvatureAndSideslipEstimation_Simulink.h
			${VSE_INCLUDE}/VehicleCurvatureAndSideslipEstimation_Simulink_private.h
			${VSE_INCLUDE}/VehicleCurvatureAndSideslipEstimation_Simulink_types.h
			${VSE_INCLUDE}/VSE_Master_Model_L2.h
			${VSE_INCLUDE}/VSE_Master_Model_L2_private.h
			${VSE_INCLUDE}/VSE_Master_Model_L2_types.h
			${VSE_INCLUDE}/YawRateCompensation.h
			${VSE_INCLUDE}/YawRateCompensation_private.h
			${VSE_INCLUDE}/YawRateCompensation_types.h
			${VSE_INCLUDE}/YawRateEstimationUsingSteeringWheelAngle.h
			${VSE_INCLUDE}/YawRateEstimationUsingSteeringWheelAngle_private.h
			${VSE_INCLUDE}/YawRateEstimationUsingSteeringWheelAngle_types.h
			${VSE_INCLUDE}/WheelRadiusSpeedAndLongSlipEstimation.h
			${VSE_INCLUDE}/VehicleMassEstimator_2018b.h
			${VSE_INCLUDE}/TrailerOscillationEstimation_2018b.h
			${VSE_INCLUDE}/mean_S0i0JSrU_Aptiv.h
			${VSE_INCLUDE}/FindDistFrom_GPSLatLongDiff.h
			${VSE_INCLUDE}/diag_OhOSGesU_Aptiv.h
			${VSE_INCLUDE}/diag_hB9Vh4NX_Aptiv.h
            ${VSE_SOURCE}/Acceleration_VCS.cpp
			${VSE_SOURCE}/BINARYSEARCH_real32_T_Aptiv.cpp
			${VSE_SOURCE}/DtrmnVehStationary.cpp
			${VSE_SOURCE}/LateralJerkEstimation.cpp
			${VSE_SOURCE}/look1_iflf_binlx_Aptiv.cpp
			${VSE_SOURCE}/LookUp_real32_T_real32_T_Aptiv.cpp
			${VSE_SOURCE}/SpeedCompensation.cpp
			${VSE_SOURCE}/SpeedCompensationArbitration.cpp
			${VSE_SOURCE}/SteeringWheelAngleBiasEstimation.cpp
			${VSE_SOURCE}/TrailerDetection.cpp
			${VSE_SOURCE}/VehicleCurvatureAndSideslipEstimation_Simulink.cpp
			${VSE_SOURCE}/VehicleCurvatureAndSideslipEstimation_Simulink_data.cpp
			${VSE_SOURCE}/VSE_Master_Model_L2.cpp
			${VSE_SOURCE}/VSE_Master_Model_L2_data.cpp
			${VSE_SOURCE}/YawRateCompensation.cpp
			${VSE_SOURCE}/YawRateEstimationUsingSteeringWheelAngle.cpp
			${VSE_SOURCE}/YawRateEstimationUsingSteeringWheelAngle_data.cpp
			${VSE_SOURCE}/WheelRadiusSpeedAndLongSlipEstimation.cpp
			${VSE_SOURCE}/VehicleMassEstimator_2018b.cpp
			${VSE_SOURCE}/TrailerOscillationEstimation_2018b.cpp
			${VSE_SOURCE}/mean_S0i0JSrU_Aptiv.cpp
			${VSE_SOURCE}/FindDistFrom_GPSLatLongDiff.cpp
			${VSE_SOURCE}/diag_OhOSGesU_Aptiv.cpp
			${VSE_SOURCE}/diag_hB9Vh4NX_Aptiv.cpp
            VSE_Core.cpp
			VSE_Core.h
			vse_utilities.cpp
			vse_utilities.h
)

target_link_libraries(VSE_Core PRIVATE sp_25_tracker_wrapper)

# Set up preprocessor to change wrt variant
if(${CUST} STREQUAL "BMWSP25")
	if(${TRACKER_TYPE} STREQUAL "SRR")
		set(VARIANT_TRACKER "f360_variant_C")
	elseif(${TRACKER_TYPE} STREQUAL "MRR")
		set(VARIANT_TRACKER "f360_variant_D")
	endif()
	add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
elseif(${CUST} STREQUAL "STLAMY24")
	if(${TRACKER_TYPE} STREQUAL "SRR" AND ${VARIANT} STREQUAL "STLA_SCALE1")
		set(VARIANT_TRACKER "f360_variant_A")
	elseif(${TRACKER_TYPE} STREQUAL "MRR" AND ${VARIANT} STREQUAL "STLA_SCALE1")
		set(VARIANT_TRACKER "f360_variant_A")
	elseif(${TRACKER_TYPE} STREQUAL "SRR" AND (${VARIANT} STREQUAL "STLA_SCALE3" OR ${VARIANT} STREQUAL "STLA_SCALE4"))
		set(VARIANT_TRACKER "f360_variant_I")
	elseif(${TRACKER_TYPE} STREQUAL "MRR" AND (${VARIANT} STREQUAL "STLA_SCALE3" OR ${VARIANT} STREQUAL "STLA_SCALE4"))
		set(VARIANT_TRACKER "f360_variant_H")
	endif()
	add_definitions(-Df360_variant_A=${VARIANT_TRACKER})
endif()

if(NOT CMAKE_CROSSCOMPILING)
	target_compile_definitions(VSE_Core PUBLIC _XKEYCHECK_H)
	target_compile_definitions(VSE_Core PRIVATE -DPC_RESIM)
endif(NOT CMAKE_CROSSCOMPILING)

get_target_property(src_files VSE_Core SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
