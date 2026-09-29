#define F360_LIB_INC
/*===================================================================================*\
* FILE: vse_utilities.cpp
*====================================================================================
* Copyright (c) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   VSE buffering helper functions
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include <algorithm>
#include "vse_utilities.h"
// #include "f360_math_func.h"

namespace vse_core
{
/*================================================================================================================*/
F360_FPN_T F360_Get_Hypotenuse(
    const F360_FPN_T a,
    const F360_FPN_T b) {

   const F360_FPN_T hypot = static_cast<F360_FPN_T>(sqrt((a * a) + (b * b)));

   return hypot;
}
enum_quality_factor_T Map_QF_F360_VSE(const F360_QUALITY_FACTOR qf) {
   enum_quality_factor_T out_qf = UNDEFINED;
   switch (qf) {
   case (F360_QF_UNDEFINED): {
      out_qf = UNDEFINED;
      break;
   }
   case (F360_QF_TEMP_UNDEFINED): {
      out_qf = TEMP_UNDEFINED;
      break;
   }
   case (F360_QF_INACCURATE): {
      out_qf = NOT_ACCURATED;
      break;
   }
   case (F360_QF_ACCURATE): {
      out_qf = ACCURATED;
      break;
   }
   default: {
      out_qf = UNDEFINED;
      break;
   }
   }

   return out_qf;
}

/*================================================================================================================*/
F360_QUALITY_FACTOR Map_QF_VSE_F360(const enum_quality_factor_T qf) {
   F360_QUALITY_FACTOR out_qf = F360_QF_UNDEFINED;
   switch (qf) {
   case (UNDEFINED): {
      out_qf = F360_QF_UNDEFINED;
      break;
   }
   case (TEMP_UNDEFINED): {
      out_qf = F360_QF_TEMP_UNDEFINED;
      break;
   }
   case (NOT_ACCURATED): {
      out_qf = F360_QF_INACCURATE;
      break;
   }
   case (ACCURATED): {
      out_qf = F360_QF_ACCURATE;
      break;
   }
   default: {
      out_qf = F360_QF_UNDEFINED;
      break;
   }
   }

   return out_qf;
}

/*================================================================================================================*/
float Map_VCS_Sideslip(const float sideslip) {
   float mapped_sideslip;

   // Sideslip mapping from [-pi, pi] to [-pi/2, pi/2] by adding/subtracting pi from sideslip when it is outside of the range
   if (std::abs(sideslip) > f360_variant_A::F360_PI_2) {
      const float sign_sideslip = (sideslip < 0.0F) ? -1.0F : 1.0F;
      mapped_sideslip           = sideslip - sign_sideslip * f360_variant_A::F360_PI;
   } else {
      mapped_sideslip = sideslip;
   }
   return mapped_sideslip;
}

/*================================================================================================================*/
void Map_VSE_OUT_To_VSE_Output_Log(const VSE_OUT &r_VSE_Output, VSE_Output_Log_T &r_VSE_Output_log) {
   r_VSE_Output_log.timestamp_us                      = r_VSE_Output.VsVSE_us_Timestamp;
   r_VSE_Output_log.veh_index                         = r_VSE_Output.VsVSE_VehIndex;
   r_VSE_Output_log.raw_speed_mps                     = r_VSE_Output.VsVSE_mps_VehRawSpd;
   r_VSE_Output_log.speed_compensation_factor         = r_VSE_Output.VsVSE_SpdCompFactor;
   r_VSE_Output_log.filt_veh_speed_over_ground        = r_VSE_Output.VsVSE_mps_VehFiltSpdOverGround;
   r_VSE_Output_log.signed_filt_veh_speed_over_ground = r_VSE_Output.VsVSE_mps_VehFiltSignedSpdOverGround;
   r_VSE_Output_log.raw_lat_accel                     = r_VSE_Output.VsVSE_mps2_RawLatAccel;
   r_VSE_Output_log.raw_long_accel                    = r_VSE_Output.VsVSE_mps2_RawLongAccel;
   r_VSE_Output_log.raw_yaw_rate_rps                  = r_VSE_Output.VsVSE_rps_RawYawRate;
   r_VSE_Output_log.raw_steering_angle_deg            = r_VSE_Output.VsVSE_deg_RawSteeringAngle;
   r_VSE_Output_log.road_wheel_angle_deg              = r_VSE_Output.VsVSE_deg_RoadWhlAngle;
   r_VSE_Output_log.yaw_rate_sa                       = r_VSE_Output.VsVSE_rps_YawRateSA;
   r_VSE_Output_log.yaw_rate_raw_bias                 = r_VSE_Output.VsVSE_rps_YawRateBias;
   r_VSE_Output_log.comp_yaw_rate_unfiltered          = r_VSE_Output.VsVSE_rps_CompYawRateUnfilt;
   r_VSE_Output_log.comp_yaw_rate_filtered            = r_VSE_Output.VsVSE_rps_CompYawRateFilt;
   r_VSE_Output_log.curvature_rear_axle               = r_VSE_Output.VsVSE_CurvatureRearAxle;
   r_VSE_Output_log.sideslip_rear_axle                = r_VSE_Output.VsVSE_rad_SideslipRearAxle;
   r_VSE_Output_log.vcs_sideslip                      = r_VSE_Output.VsVSE_rad_VCSSideslip;
   r_VSE_Output_log.vcs_long_velocity                 = r_VSE_Output.VsVSE_mps_VCSLongVel;
   r_VSE_Output_log.vcs_lat_velocity                  = r_VSE_Output.VsVSE_mps_VCSLatVel;
   r_VSE_Output_log.sensor_sideslip                   = r_VSE_Output.VsVSE_rad_SensorSideslip;
   r_VSE_Output_log.sensor_long_velocity              = r_VSE_Output.VsVSE_mps_SensorLongVel;
   r_VSE_Output_log.sensor_lat_velocity               = r_VSE_Output.VsVSE_mps_SensorLatVel;
   r_VSE_Output_log.k_dist_rear_axle_to_vcs           = r_VSE_Output.KsVSE_m_DistRearAxleToVCS;
   r_VSE_Output_log.vcs_lat_accel                     = r_VSE_Output.VsVSE_mps2_VCSLatAccel;
   r_VSE_Output_log.vcs_long_accel                    = r_VSE_Output.VsVSE_mps2_VCSLongAccel;
   r_VSE_Output_log.accel_rear_axle                   = r_VSE_Output.VsVSE_mps2_COGLongAccel;
   r_VSE_Output_log.raw_speed_qf                      = r_VSE_Output.VeVSE_VehRawSpdQF;
   r_VSE_Output_log.speed_compensation_factor_qf      = r_VSE_Output.VeVSE_SpdCompFactorQF;
   r_VSE_Output_log.filt_veh_speed_over_ground_qf     = r_VSE_Output.VeVSE_VehFiltSpdOverGroundQF;
   r_VSE_Output_log.raw_lat_accel_qf                  = r_VSE_Output.VeVSE_RawLatAccelQF;
   r_VSE_Output_log.raw_long_accel_qf                 = r_VSE_Output.VeVSE_RawLongAccelQF;
   r_VSE_Output_log.raw_yaw_rate_qf                   = r_VSE_Output.VeVSE_RawYawRateQF;
   r_VSE_Output_log.raw_steering_angle_qf             = r_VSE_Output.VeVSE_RawSteeringAngleQF;
   r_VSE_Output_log.road_wheel_angle_qf               = r_VSE_Output.VeVSE_RoadWhlAngleQF;
   r_VSE_Output_log.yaw_rate_sa_qf                    = r_VSE_Output.VeVSE_YawRateSAQF;
   r_VSE_Output_log.yaw_rate_bias_qf                  = r_VSE_Output.VeVSE_YawRateBiasQF;
   r_VSE_Output_log.comp_yaw_rate_qf                  = r_VSE_Output.VeVSE_CompYawRateQF;
   r_VSE_Output_log.vcs_lat_accel_qf                  = r_VSE_Output.VeVSE_VCSLatAccelQF;
   r_VSE_Output_log.vcs_long_accel_qf                 = r_VSE_Output.VeVSE_VCSLongAccelQF;
   r_VSE_Output_log.stationary                        = r_VSE_Output.VsVSE_b_VehStationary;
}

/*================================================================================================================*/
void Map_VSE_Output_Log_To_VSE_OUT(const VSE_Output_Log_T &r_VSE_Output_log, VSE_OUT &r_VSE_Output) {
   r_VSE_Output.VsVSE_us_Timestamp                   = r_VSE_Output_log.timestamp_us;
   r_VSE_Output.VsVSE_VehIndex                       = r_VSE_Output_log.veh_index;
   r_VSE_Output.VsVSE_mps_VehRawSpd                  = r_VSE_Output_log.raw_speed_mps;
   r_VSE_Output.VsVSE_SpdCompFactor                  = r_VSE_Output_log.speed_compensation_factor;
   r_VSE_Output.VsVSE_mps_VehFiltSpdOverGround       = r_VSE_Output_log.filt_veh_speed_over_ground;
   r_VSE_Output.VsVSE_mps_VehFiltSignedSpdOverGround = r_VSE_Output_log.signed_filt_veh_speed_over_ground;
   r_VSE_Output.VsVSE_mps2_RawLatAccel               = r_VSE_Output_log.raw_lat_accel;
   r_VSE_Output.VsVSE_mps2_RawLongAccel              = r_VSE_Output_log.raw_long_accel;
   r_VSE_Output.VsVSE_rps_RawYawRate                 = r_VSE_Output_log.raw_yaw_rate_rps;
   r_VSE_Output.VsVSE_deg_RawSteeringAngle           = r_VSE_Output_log.raw_steering_angle_deg;
   r_VSE_Output.VsVSE_deg_RoadWhlAngle               = r_VSE_Output_log.road_wheel_angle_deg;
   r_VSE_Output.VsVSE_rps_YawRateSA                  = r_VSE_Output_log.yaw_rate_sa;
   r_VSE_Output.VsVSE_rps_YawRateBias                = r_VSE_Output_log.yaw_rate_raw_bias;
   r_VSE_Output.VsVSE_rps_CompYawRateUnfilt          = r_VSE_Output_log.comp_yaw_rate_unfiltered;
   r_VSE_Output.VsVSE_rps_CompYawRateFilt            = r_VSE_Output_log.comp_yaw_rate_filtered;
   r_VSE_Output.VsVSE_CurvatureRearAxle              = r_VSE_Output_log.curvature_rear_axle;
   r_VSE_Output.VsVSE_rad_SideslipRearAxle           = r_VSE_Output_log.sideslip_rear_axle;
   r_VSE_Output.VsVSE_rad_VCSSideslip                = r_VSE_Output_log.vcs_sideslip;
   r_VSE_Output.VsVSE_mps_VCSLongVel                 = r_VSE_Output_log.vcs_long_velocity;
   r_VSE_Output.VsVSE_mps_VCSLatVel                  = r_VSE_Output_log.vcs_lat_velocity;
   r_VSE_Output.VsVSE_rad_SensorSideslip             = r_VSE_Output_log.sensor_sideslip;
   r_VSE_Output.VsVSE_mps_SensorLongVel              = r_VSE_Output_log.sensor_long_velocity;
   r_VSE_Output.VsVSE_mps_SensorLatVel               = r_VSE_Output_log.sensor_lat_velocity;
   r_VSE_Output.KsVSE_m_DistRearAxleToVCS            = r_VSE_Output_log.k_dist_rear_axle_to_vcs;
   r_VSE_Output.VsVSE_mps2_VCSLatAccel               = r_VSE_Output_log.vcs_lat_accel;
   r_VSE_Output.VsVSE_mps2_VCSLongAccel              = r_VSE_Output_log.vcs_long_accel;
   r_VSE_Output.VsVSE_mps2_COGLongAccel              = r_VSE_Output_log.accel_rear_axle;
   r_VSE_Output.VeVSE_VehRawSpdQF                    = r_VSE_Output_log.raw_speed_qf;
   r_VSE_Output.VeVSE_SpdCompFactorQF                = r_VSE_Output_log.speed_compensation_factor_qf;
   r_VSE_Output.VeVSE_VehFiltSpdOverGroundQF         = r_VSE_Output_log.filt_veh_speed_over_ground_qf;
   r_VSE_Output.VeVSE_RawLatAccelQF                  = r_VSE_Output_log.raw_lat_accel_qf;
   r_VSE_Output.VeVSE_RawLongAccelQF                 = r_VSE_Output_log.raw_long_accel_qf;
   r_VSE_Output.VeVSE_RawYawRateQF                   = r_VSE_Output_log.raw_yaw_rate_qf;
   r_VSE_Output.VeVSE_RawSteeringAngleQF             = r_VSE_Output_log.raw_steering_angle_qf;
   r_VSE_Output.VeVSE_RoadWhlAngleQF                 = r_VSE_Output_log.road_wheel_angle_qf;
   r_VSE_Output.VeVSE_YawRateSAQF                    = r_VSE_Output_log.yaw_rate_sa_qf;
   r_VSE_Output.VeVSE_YawRateBiasQF                  = r_VSE_Output_log.yaw_rate_bias_qf;
   r_VSE_Output.VeVSE_CompYawRateQF                  = r_VSE_Output_log.comp_yaw_rate_qf;
   r_VSE_Output.VeVSE_VCSLatAccelQF                  = r_VSE_Output_log.vcs_lat_accel_qf;
   r_VSE_Output.VeVSE_VCSLongAccelQF                 = r_VSE_Output_log.vcs_long_accel_qf;
   r_VSE_Output.VsVSE_b_VehStationary                = r_VSE_Output_log.stationary;
}

/*================================================================================================================*/
void F360_Map_VSE_OUT_to_Host_T(const VSE_OUT &r_VSE_Ouput, const float rear_cornering_compliance, f360_variant_A::F360_Host_T &r_host) {
   // Map output from
   r_host.vehicle_index = r_VSE_Ouput.VsVSE_VehIndex;

   const float abs_speed = F360_Get_Hypotenuse(r_VSE_Ouput.VsVSE_mps_VCSLongVel, r_VSE_Ouput.VsVSE_mps_VCSLatVel);
   r_host.vcs_speed      = (0.0F > r_VSE_Ouput.VsVSE_mps_VCSLongVel) ? -1.0F * abs_speed : abs_speed;
   r_host.yaw_rate_rad   = r_VSE_Ouput.VsVSE_rps_CompYawRateFilt; // There also exist unfiltered yawrate

   // Mapping to align VSE output definition to F360 input definition (when reversing)
   r_host.vcs_sideslip = Map_VCS_Sideslip(r_VSE_Ouput.VsVSE_rad_VCSSideslip);

   r_host.curvature_rear          = r_VSE_Ouput.VsVSE_CurvatureRearAxle;
   r_host.dist_rear_axle_to_vcs_m = r_VSE_Ouput.KsVSE_m_DistRearAxleToVCS;
   r_host.vcs_lat_acceleration    = r_VSE_Ouput.VsVSE_mps2_VCSLatAccel;
   r_host.vcs_long_acceleration   = r_VSE_Ouput.VsVSE_mps2_VCSLongAccel;

   r_host.speed        = r_VSE_Ouput.VsVSE_mps_VehFiltSignedSpdOverGround;
   r_host.acceleration = r_VSE_Ouput.VsVSE_mps2_COGLongAccel;

   r_host.rear_cornering_compliance = rear_cornering_compliance;
   r_host.speed_correction_factor   = r_VSE_Ouput.VsVSE_SpdCompFactor;

   // Quality flags
   r_host.speed_qf      = Map_QF_VSE_F360(r_VSE_Ouput.VeVSE_VehFiltSpdOverGroundQF);
   r_host.yaw_rate_qf   = Map_QF_VSE_F360(r_VSE_Ouput.VeVSE_CompYawRateQF);
   r_host.lat_accel_qf  = Map_QF_VSE_F360(r_VSE_Ouput.VeVSE_VCSLatAccelQF);
   r_host.long_accel_qf = Map_QF_VSE_F360(r_VSE_Ouput.VeVSE_VCSLongAccelQF);
}

/*================================================================================================================*/
uint64_t F360_Get_Middle_Sensor_Timestamp(const f360_variant_A::F360_Radar_Sensor_T (&r_sensors)[f360_variant_A::MAX_NUMBER_OF_SENSORS]) {
   uint64_t min_sensor_timestamp_us = UINT64_MAX;
   uint64_t max_sensor_timestamp_us = 0ULL;
   bool f_any_valid_sensors         = false;
   for (int32_t sensor_idx = 0; sensor_idx < f360_variant_A::MAX_NUMBER_OF_SENSORS; sensor_idx++) {
      if ((r_sensors[sensor_idx].variable.look_id != F360_DET_LOOK_ID_INVALID) && (0U < r_sensors[sensor_idx].variable.number_of_valid_detections)) {
         min_sensor_timestamp_us = (min_sensor_timestamp_us < r_sensors[sensor_idx].variable.timestamp_us) ? min_sensor_timestamp_us : r_sensors[sensor_idx].variable.timestamp_us;
         max_sensor_timestamp_us = (max_sensor_timestamp_us > r_sensors[sensor_idx].variable.timestamp_us) ? max_sensor_timestamp_us : r_sensors[sensor_idx].variable.timestamp_us;
         f_any_valid_sensors     = true;
      }
   }

   const uint64_t middle_sensor_timestamp_us = f_any_valid_sensors ? (min_sensor_timestamp_us + ((max_sensor_timestamp_us - min_sensor_timestamp_us) / 2ULL)) : 0ULL;

   return middle_sensor_timestamp_us;
}

/*===========================================================================*\
* FUNCTION: F360_Get_Closest_VSE_Output_Index()
*===========================================================================
* RETURN VALUE:
* uint32_t
*
* PARAMETERS:
* const uint64_t timestamp_us,
* VSE_buffer& r_VSE_Output_buffer
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Find VSE buffer element with smallest timestamp difference to given timestamp.
* If multiple elements have the same timestamp use the one added to the buffer
* most recently.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
uint32_t F360_Get_Closest_VSE_Output_Index(const uint64_t timestamp_us, VSE_buffer &r_VSE_Output_buffer) {
   uint32_t closest_timestamp_idx = 0U;
   uint64_t min_timestamp_diff_us = UINT64_MAX;
   // Getting value from VSE_buffer index 0 returns element added at the earliest, getting index 'size-1' will return element added most recently
   // The loop iterates the buffer from the element added at the earliest to the element added most recently, so the newest element will be selected
   // if there are multiple elements with the same timestamp value
   for (uint32_t idx = 0U; idx < r_VSE_Output_buffer.size(); idx++) {
      const VSE_OUT &r_VSE_Output_local = r_VSE_Output_buffer[idx];

      const bool f_host_timestamp_bigger = timestamp_us < r_VSE_Output_local.VsVSE_us_Timestamp;
      const uint64_t timestamp_diff_us   = f_host_timestamp_bigger ? (r_VSE_Output_local.VsVSE_us_Timestamp - timestamp_us) : (timestamp_us - r_VSE_Output_local.VsVSE_us_Timestamp);

      // Save the element with lowest timestamp difference
      if (timestamp_diff_us <= min_timestamp_diff_us) {
         min_timestamp_diff_us = timestamp_diff_us;
         closest_timestamp_idx = idx;
      }
   }

   return closest_timestamp_idx;
}
} // namespace vse_core
