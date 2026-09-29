#define F360_LIB_INC
/*=========================================================================
*  FILE: VSE_Core.cpp
*=========================================================================
* Copyright © 2020 Aptiv. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
*
*  DESCRIPTION:
*    This file contains VSE Core class methods definitions.
*
*
*=========================================================================
*------------------------------------------------------------------------------
*
* class:        VSE_CORE
*
* Description:  Wrapper for the Core VSE
*
*========================================================================*/
#include "stdint.h"
#include "VSE_Core.h"
#include "vse_utilities.h"
#include "f360_math.h"
#include "f360_math_func.h"
/*===========================================================================*\
 * Public Function
\*===========================================================================*/

namespace vse_core
{

#ifndef DEG2RADF
#define DEG2RADF ((float)0.0174532925199433F)
#define RAD2DEGF ((float)1.0F/DEG2RADF)
#endif

	VSE_CORE::VSE_CORE()
	{}

	/*===========================================================================*/
	VSE_CORE::~VSE_CORE()
	{}
	/*===========================================================================*/
	void VSE_CORE::Initialize(const f360_variant_A::F360_Host_Calib_T& r_host_calib)
	{
		// Map Vehicle_Parameters input
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_WheelBase = r_host_calib.wheelbase_m;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_DistRearAxleToVCS = r_host_calib.dist_rear_axle_to_vcs_m;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_LongDistRadarToRearAxle = 0.0F; // Currently not used
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_LatDistRadarToRearAxle = 0.0F; // Currently not used
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_SteeringGearRatio = r_host_calib.steer_gear_ratio;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_UndersteeringCoeff = r_host_calib.understeer_coefficient;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_RearCorneringCompliance = r_host_calib.rear_cornering_compliance;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_VehWidth = r_host_calib.vehicle_width_m;
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_COGX = r_host_calib.cog_x; // Currently not used
		this->BMW_VSE.VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_COGY = r_host_calib.cog_y; // Currently not used

		this->vehicle_index = 0;

		this->raw_host_signal_latency_us = static_cast<uint64_t>(r_host_calib.raw_host_signal_latency_ms * 1000U);

		this->BMW_VSE.initialize();
	}

	/*===========================================================================*/
	void VSE_CORE::Step(const uint64_t timestamp_us, const float speed_correction_factor, const f360_variant_A::F360_Host_Raw_T& r_host_raw)
	{
		// Map VCAN_VSE input
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_RawVehSpeed = (std::abs)(r_host_raw.raw_speed);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawVehSpeedQF = Map_QF_F360_VSE((F360_QUALITY_FACTOR)r_host_raw.speed_qf);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rps_RawYawRate = r_host_raw.raw_yaw_rate_rad;
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawYawRateQF = Map_QF_F360_VSE((F360_QUALITY_FACTOR)r_host_raw.yaw_rate_qf);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehStationary = (0.0F == r_host_raw.raw_speed);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehReverse = (0.0F > r_host_raw.raw_speed);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_deg_RawSteeringAngle = RAD2DEGF * r_host_raw.steering_wheel_angle_rad;
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawSteeringAngleQF = Map_QF_F360_VSE((F360_QUALITY_FACTOR)r_host_raw.steering_wheel_angle_qf);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps2_RawLatAccel = r_host_raw.lat_accel;
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLatAccelQF = Map_QF_F360_VSE((F360_QUALITY_FACTOR)r_host_raw.lat_accel_qf);
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps2_RawLongAccel = r_host_raw.long_accel;
		this->BMW_VSE.VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLongAccelQF = Map_QF_F360_VSE((F360_QUALITY_FACTOR)r_host_raw.long_accel_qf);

		//// Map Tracker_VSE input
		this->BMW_VSE.VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_5 = speed_correction_factor;
		this->BMW_VSE.VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_5_QF = ACCURATED; // Not available. Setting to accurate

		// Map System_Time_Micro_Sec input
		// Saturate value at zero when calculating difference to avoid uint value wrap around
		this->BMW_VSE.VSE_Master_Model_L2_U.Vs_us_SystemTime = (timestamp_us > this->raw_host_signal_latency_us) ?
			(timestamp_us - this->raw_host_signal_latency_us) : 0ULL;

		// Run VSE
		this->BMW_VSE.step();

		// Increment vehicle index
		this->vehicle_index++;
		this->BMW_VSE.VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex = this->vehicle_index;

		// Add VSE Output to buffer
		this->Update_VSE_Buffer(this->BMW_VSE.VSE_Master_Model_L2_Y.VSE_Output);
	}

	/*===========================================================================*/
	VSE_OUT VSE_CORE::Get_VSE_Output(void)
	{
		return ((0U < this->vse_output_buffer.size()) ?
			this->vse_output_buffer[this->vse_output_buffer.size() - 1U] :
			this->BMW_VSE.VSE_Master_Model_L2_Y.VSE_Output);
	}

	VSE_OUT VSE_CORE::Get_VSE_Output(const uint64_t timestamp_us)
	{
		const uint32_t closest_timestamp_idx = F360_Get_Closest_VSE_Output_Index(timestamp_us, this->vse_output_buffer);
		return (this->vse_output_buffer[closest_timestamp_idx]);
	}

	/*===========================================================================*/
	void VSE_CORE::Log_VSE_Output(VSE_Output_Log_T& r_VSE_Output_log)
	{
		Map_VSE_OUT_To_VSE_Output_Log(this->BMW_VSE.VSE_Master_Model_L2_Y.VSE_Output, r_VSE_Output_log);
	}

	void VSE_CORE::Log_Internals(VSE_Internal_Data_Log_T& r_VSE_internals_log)
	{
		r_VSE_internals_log.k_host_signal_latency_ms = static_cast<uint32_t>(this->raw_host_signal_latency_us / 1000);

		// Log VSE_RESIM internals
		VSE_RESIM_Internals_T& VSE_resim_log = r_VSE_internals_log.VSE_internals;
		VSE_RESIM_T& VSE_Resim = this->BMW_VSE.VSE_Master_Model_L2_Y.VSE_Resim_Output;
		VSE_resim_log.yaw_rate_bias1 = VSE_Resim.VsVSE_rps_YawRateBias1;
		VSE_resim_log.yaw_rate_bias2 = VSE_Resim.VsVSE_rps_YawRateBias2;
		VSE_resim_log.yaw_rate_bias_fast_bias1 = VSE_Resim.VsVSE_rps_YawRateBiasFast1;
		VSE_resim_log.yaw_rate_bias_fast_bias2 = VSE_Resim.VsVSE_rps_YawRateBiasFast2;
		VSE_resim_log.comp_yaw_rate_diff_filt = VSE_Resim.VsVSE_rps_CompYawRateDiffFilt;
		VSE_resim_log.yaw_rate_bias_diff = VSE_Resim.VsVSE_rps_YawRateBiasDiff;
		VSE_resim_log.ignition_time = VSE_Resim.VsVSE_s_IgnitionTime;
		VSE_resim_log.CurvKalmanFilterC0 = VSE_Resim.VsVSE_CurvKalmanFilterC0;
		VSE_resim_log.CurvKalmanFilterC1 = VSE_Resim.VsVSE_CurvKalmanFilterC1;
		VSE_resim_log.road_type = VSE_Resim.VeVSE_RoadType;
		VSE_resim_log.f_yaw_rate_bias_converged = VSE_Resim.VsVSE_b_YawRateBiasConverged;
		VSE_resim_log.f_stop_bias_converged = VSE_Resim.VsVSE_b_YawRateStopBiasConverged;
		VSE_resim_log.f_yaw_rate_bias_shift = VSE_Resim.VsVSE_b_YawRateBiasShift;
		VSE_resim_log.f_yaw_rate_steady = VSE_Resim.VsVSE_b_YawRateSteady;
		VSE_resim_log.f_input_invalid_persistent = VSE_Resim.VsVSE_b_InputInvalidPersistent;
		VSE_resim_log.f_execution_period_error_persistent = VSE_Resim.VsVSE_b_ExecutionPeriodErrorPresistent;
		VSE_resim_log.f_bias_was_accurate = VSE_Resim.VsVSE_b_BiasWasAccurate;

		// Log VSE output buffer in order
		r_VSE_internals_log.buffer_size = static_cast<uint32_t>(this->vse_output_buffer.size());
		for (uint32_t i = 0U; i < r_VSE_internals_log.buffer_size; i++)
		{
			Map_VSE_OUT_To_VSE_Output_Log(this->vse_output_buffer[i], r_VSE_internals_log.VSE_output_buffer[i]);
		}
	}

	/*===========================================================================*/
	void VSE_CORE::Init_State_From_Log(const VSE_Internal_Data_Log_T& r_VSE_internals_log)
	{
		// TODO: Not possible to initialize the VSE_RESIM internals. New VSE update required

		// Insert VSE output into buffer. (Logged Data assumed to be in order)
		for (uint32_t i = 0U; i < r_VSE_internals_log.buffer_size; i++)
		{
			VSE_OUT VSE_Output_local = {};
			Map_VSE_Output_Log_To_VSE_OUT(r_VSE_internals_log.VSE_output_buffer[i], VSE_Output_local);
			this->Update_VSE_Buffer(VSE_Output_local);
		}
	}

	void VSE_CORE::Update_VSE_Buffer(const VSE_OUT& r_VSE_Output)
	{
		// Add VSE Output to buffer
		this->vse_output_buffer.push(r_VSE_Output);
	}
}
