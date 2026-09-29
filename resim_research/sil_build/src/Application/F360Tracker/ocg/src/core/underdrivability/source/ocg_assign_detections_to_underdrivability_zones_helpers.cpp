/*===================================================================================*\
* FILE: ocg_assign_detections_to_underdrivability_zones_helpers.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Detections_To_Underdrivability_Zones() helper functions definitions
*
*   Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include <cmath>
#include "cmn_math_func.h"
#include "cmn_math_constants.h"
#include "ocg_underdrivability_states.h"
#include "cmn_calc_circular_zone_idx.h"
#include "ocg_assign_detections_to_underdrivability_zones_helpers.h"
#include "cmn_utilities.h"


namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Compensate_Ground_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib
   * float& det_height
   * float& det_elev
   * bool& is_ground_detection
   * const float& det_rng
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
   * This function verifies whether detection can be ground detection and if yes
   * inverts its height and elevation.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Compensate_Ground_Detection(
       const OCG_Calibrations_T &calib,
       float &det_height,
       float &det_elev,
       bool &is_ground_detection,
       const float &det_rng)
   {
      if (calib.underdrive_hypothesis_height_can_not_pass_lower > det_height)
      {
         det_height = -det_height;
         if (det_rng > ocg::cmn::OCG_MIN_DENOMINATOR)
         {
            det_elev = asinf(det_height / det_rng);
         }
         else
         {
            det_elev = 0.0F;
         }
         is_ground_detection = true;
      }
      else
      {
         is_ground_detection = false;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_In_Which_Zone_Det_Is_Located()
   *===========================================================================
   * RETURN VALUE:
   * uint32_t
   *
   * PARAMETERS:
   * const OCG_Underdrivability_Internal_T& underdrivability
   * const float det_vcs_long_posn
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
   * This function calculates in which zone detection is located.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   uint32_t Calc_In_Which_Zone_Det_Is_Located(
       const OCG_Underdrivability_Internal_T &in_underdrivability,
       const float det_vcs_long_posn)
   {
      uint32_t zone_idx = (static_cast<uint32_t>(ceilf((det_vcs_long_posn - GRID_MIN_X_DIST + in_underdrivability.props.host_travel_distance) / CELL_LENGTH)) - 1U);

      zone_idx = ((static_cast<uint32_t>(NUM_CELLS_X) <= zone_idx) ? (static_cast<uint32_t>(NUM_CELLS_X - 1U)) : zone_idx);
      return cmn::Calc_Circular_Zone_Idx(in_underdrivability.props.circular_buffer_idx, zone_idx, NUM_CELLS_X);
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Det_Elevation_Params()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T& det_props
   * const F360_Detection_T& det_raw
   * const F360_Radar_Sensor_Calib_T(&sensor_calibs)[MAX_NUMBER_OF_SENSORS]
   * float& det_vcs_rng
   * float& det_vcs_height
   * float& det_vcs_elev
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
   * This function calculates detection position in VCS:
   * - height
   * - elevation
   * - range
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   
   void Calc_Det_Elevation_Params(
       const rspp_variant_A::RSPP_Detection_T &det,
       float &det_vcs_rng,
       float &det_vcs_height,
       float &det_vcs_elev)
   {
      // Measurement info from detection
      det_vcs_height = -det.processed.vcs_position_z; //z coordinate in Vehicle Coordinate System(VCS)[m], Note: The zero plane is defined at ground leveland negative above ground

      // Calculate detection range in VCS
      det_vcs_rng = sqrtf((det.processed.vcs_position_y * det.processed.vcs_position_y) + (det.processed.vcs_position_x * det.processed.vcs_position_x) + (det_vcs_height * det_vcs_height));
      // Calculate detection elevation in VCS (from ground plane)
      if (det_vcs_rng > ocg::cmn::OCG_MIN_DENOMINATOR)
      {
         det_vcs_elev = asinf(det_vcs_height / det_vcs_rng);
      }
      else
      {
         det_vcs_elev = 0.0F;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Detection_Valid()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Detection_Props_T& det_props
   * const F360_Detection_T& det_raw
   * const F360_Host_T& vehicle_data
   * const OCG_Calibrations_T& calib
   * const F360_Radar_Sensor_Calib_T(&sensor_calibs)[MAX_NUMBER_OF_SENSORS]
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
   * This function checks whether detection meets conditions to be used for
   * underdrivability classification.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Detection_Valid(
       const rspp_variant_A::RSPP_Detection_T &det,
       const OCG_Calibrations_T &calib,
       const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS],
       const float closest_dist_to_host_path,
       const float max_lat_offset_from_host_curv)
   {

      bool f_valid = false;
      if (use_this_sensor(sensors[det.raw.sensor_id - 1].constant.mounting_location, calib))
      {
         f_valid = ((det.processed.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS) || (det.processed.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY)) &&
                   (GRID_MIN_X_DIST < det.processed.vcs_position_x) &&
                   (det.processed.vcs_position_x < GRID_MAX_X_DIST);

      }
         
      if (f_valid)
      {

         f_valid = (closest_dist_to_host_path < max_lat_offset_from_host_curv) &&
                   (std::abs(det.processed.vcs_position_y) < calib.underdrive_max_lat_posn) &&
                   (((det.raw.f_super_res) && (det.raw.confid_azimuth < calib.underdrive_az_conf_th)) || (!det.raw.f_super_res)) &&
                   (((det.raw.f_super_res) && (det.raw.confid_elevation < calib.underdrive_el_conf_th)) || (!det.raw.f_super_res)) &&
                   (std::abs(det.processed.range_rate_compensated) < calib.underdrive_max_comp_range_rate);
      }
      return f_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Zone_Innovation()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Zones_Innovation_T& zone_innovation
   * const OCG_Calibrations_T& calib
   * const float& det_vcs_rng
   * const float& det_vcs_elev
   * const F360_Detection_T& det_raw
   * const uint32_t& det_zone
   * const bool& is_ground_detection
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
   * This function updates state innovation of zones.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Update_Zone_Innovation(
       OCG_Zones_Innovation_T &zone_innovation,
       const OCG_Calibrations_T &calib,
       const float &det_vcs_rng,
       const float &det_vcs_elev,
       const rspp_variant_A::Raw_Detection_T &det_raw,
       const bool &is_ground_detection,
       const float lateral_weight)
   {
      const float norm_det_elev_can_pass = (det_vcs_elev - asinf(calib.underdrive_hypothesis_height_can_pass / det_vcs_rng));
      zone_innovation.height_can_pass[UD_HEIGHT_STATE_DET_COUNT] += lateral_weight;
      zone_innovation.height_can_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE] += norm_det_elev_can_pass * lateral_weight;
      zone_innovation.height_can_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] += norm_det_elev_can_pass * norm_det_elev_can_pass * lateral_weight;

      const float norm_det_elev_is_likely_to_pass = (det_vcs_elev - asinf(calib.underdrive_hypothesis_height_is_likely_to_pass / det_vcs_rng));
      zone_innovation.height_is_likely_to_pass[UD_HEIGHT_STATE_DET_COUNT] += lateral_weight;
      zone_innovation.height_is_likely_to_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE] += norm_det_elev_is_likely_to_pass * lateral_weight;
      zone_innovation.height_is_likely_to_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] += norm_det_elev_is_likely_to_pass * norm_det_elev_is_likely_to_pass * lateral_weight;

      const float norm_det_elev_can_not_pass_upper = (det_vcs_elev - asinf(calib.underdrive_hypothesis_height_can_not_pass_upper / det_vcs_rng));
      zone_innovation.height_can_not_pass_upper[UD_HEIGHT_STATE_DET_COUNT] += lateral_weight;
      zone_innovation.height_can_not_pass_upper[UD_HEIGHT_STATE_ELEVATION_ANGLE] += norm_det_elev_can_not_pass_upper * lateral_weight;
      zone_innovation.height_can_not_pass_upper[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] += norm_det_elev_can_not_pass_upper * norm_det_elev_can_not_pass_upper * lateral_weight;

      const float norm_det_elev_can_not_pass_lower = (det_vcs_elev - asinf(calib.underdrive_hypothesis_height_can_not_pass_lower / det_vcs_rng));
      zone_innovation.height_can_not_pass_lower[UD_HEIGHT_STATE_DET_COUNT] += lateral_weight;
      zone_innovation.height_can_not_pass_lower[UD_HEIGHT_STATE_ELEVATION_ANGLE] += norm_det_elev_can_not_pass_lower * lateral_weight;
      zone_innovation.height_can_not_pass_lower[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] += norm_det_elev_can_not_pass_lower * norm_det_elev_can_not_pass_lower * lateral_weight;

      if ((0.0F < norm_det_elev_can_pass) && (!is_ground_detection))
      {
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_DET_COUNT] += 1.0F;
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_RANGE] += det_vcs_rng;
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RANGE] += det_vcs_rng * det_vcs_rng;
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_RCS] += det_raw.rcs;
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RCS] += det_raw.rcs * det_raw.rcs;
         zone_innovation.RCS_slope_can_pass[UD_RCS_STATE_RANGE_RCS_PROD] += det_vcs_rng * det_raw.rcs;
      }
      if ((0.0F < norm_det_elev_is_likely_to_pass) && (!is_ground_detection))
      {
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_DET_COUNT] += 1.0F;
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_RANGE] += det_vcs_rng;
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_SQUARED_RANGE] += det_vcs_rng * det_vcs_rng;
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_RCS] += det_raw.rcs;
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_SQUARED_RCS] += det_raw.rcs * det_raw.rcs;
         zone_innovation.RCS_slope_is_likely_to_pass[UD_RCS_STATE_RANGE_RCS_PROD] += det_vcs_rng * det_raw.rcs;
      }
      if ((0.0F < norm_det_elev_can_not_pass_lower) && (0.0F > norm_det_elev_can_not_pass_upper) && (!is_ground_detection))
      {
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_DET_COUNT] += 1.0F;
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_RANGE] += det_vcs_rng;
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_SQUARED_RANGE] += det_vcs_rng * det_vcs_rng;
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_RCS] += det_raw.rcs;
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_SQUARED_RCS] += det_raw.rcs * det_raw.rcs;
         zone_innovation.RCS_slope_can_not_pass[UD_RCS_STATE_RANGE_RCS_PROD] += det_vcs_rng * det_raw.rcs;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Lateral_Distance_To_Curved_Host_Path()
   *===========================================================================
   * RETURN VALUE:
   * float
   *
   * PARAMETERS:
   * const F360_Host_T& vehicle_data,
   * const OCG_Calibrations_T& calibrations,
   * const float& det_vcs_long_posn,
   * const float& det_lat_posn
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
   * Function calculates lateral distance of detection to curved host path.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   float Calc_Lateral_Distance_To_Curved_Host_Path(
       const float curvature_rear,
       const float small_curvature_th,
       const float vcs_position_x,
       const float vcs_position_y)
   {
      float closest_dist_to_host_path;
      if ((small_curvature_th < std::abs(curvature_rear)))
      {
         const float host_path_radius = 1.0F / curvature_rear;
         const float lat_diff = vcs_position_y - host_path_radius;
         const float long_diff = vcs_position_x;
         closest_dist_to_host_path = std::abs(sqrtf(lat_diff * lat_diff + long_diff * long_diff) - std::abs(host_path_radius));
      }
      else
      {
         closest_dist_to_host_path = std::abs(vcs_position_y);
      }
      return closest_dist_to_host_path;
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
