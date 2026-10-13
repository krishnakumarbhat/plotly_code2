
/*===========================================================================*/
/**
 * @file f360_motion_status_classification.cpp
 *
 * @brief Detection motion status classification implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of detection motion status classification using statistical
 * hypothesis testing on compensated range rate. Determines if detections are
 * moving or stationary based on adaptive thresholds considering host vehicle
 * dynamics, detection quality, and measurement uncertainties.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/52-SoftwareArchitecture/CMP_SAD_Interface_Library_TrackerCore
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "f360_motion_status_classification.h"
#include "f360_detection_motion_status.h"
#include "f360_mark_detection_with_low_detection_conf.h"
#include "f360_math_func.h"
#include "f360_uncertainty_calculation.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/
namespace f360_variant_A
{
/******************************************************************************
 * Name:  Tune_Moving_Threshold
 *   This function adjusts the base moving threshold based on detection quality
 *   factors to account for azimuth uncertainty affecting compensated range
 *   rate. Applies penalties for low azimuth confidence and azimuth error
 *   conditions, returning the maximum of confidence and error-adjusted
 *   thresholds.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   conf_az - Detection azimuth confidence level
 *   f_azimuth_error_stat_mov - Flag indicating azimuth error affects motion classification
 *   host_speed - Host vehicle speed for threshold calculation
 *
 * Return Value:
 *   Tuned moving threshold value
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
static float32_t Tune_Moving_Threshold(const int8_t conf_az, const bool f_azimuth_error_stat_mov, const float32_t host_speed);
} // namespace f360_variant_A

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
{
/*===========================================================================*
 * Local Enum Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Local Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Static Variable Definitions
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/
static float32_t Tune_Moving_Threshold(const int8_t conf_az, const bool f_azimuth_error_stat_mov, const float32_t host_speed)
{
   const float32_t k_det_mov_slope                 = 0.04F;
   const float32_t k_det_mov_intercept             = 0.18F;
   const float32_t k_det_mov_min                   = 0.3F;
   const float32_t k_det_mov_max                   = 1.5F;
   const float32_t k_det_mov_low_az_conf_penalty   = 1.0F;
   const float32_t k_det_az_error_stat_mov_penalty = 3.0F;

   const float32_t initial_moving_threshold =
      F360_Saturate(k_det_mov_slope * std::abs(host_speed) + k_det_mov_intercept, k_det_mov_min, k_det_mov_max);
   const float32_t move_th_conf_az =
      (F360_CONF_AZIMUTH_LOW == conf_az) ? initial_moving_threshold + k_det_mov_low_az_conf_penalty : initial_moving_threshold;
   const float32_t move_th_err_az =
      (f_azimuth_error_stat_mov) ? initial_moving_threshold + k_det_az_error_stat_mov_penalty : initial_moving_threshold;

   return (move_th_conf_az > move_th_err_az) ? move_th_conf_az : move_th_err_az;
}

void Reset_Motion_Status_Cache(void)
{
   // Reset Uncertainty Calculation Cache
   Reset_Uncertainty_Cache();
}

float32_t Calculate_Moving_Threshold(const Point vcs_pos, const int8_t conf_az, const F360_Host_T &host,
                                     const bool f_azimuth_error_stat_mov)
{
   const float32_t k_dmc_base_thr                    = 1.5F;
   const float32_t k_dmc_host_speed_offset           = 15.0F;
   const float32_t k_dmc_host_speed_scale_factor     = 0.02F;
   const float32_t k_dmc_host_curvature_scale_factor = 500.0F;
   const float32_t k_dmc_bypass_det_range_thr_sq     = 25.0F;

   const float32_t moving_threshold = Tune_Moving_Threshold(conf_az, f_azimuth_error_stat_mov, host.vcs_speed);

   const float32_t host_speed_offset       = std::max(0.0F, host.vcs_speed - k_dmc_host_speed_offset);
   const float32_t host_speed_th_component = host_speed_offset * host_speed_offset * k_dmc_host_speed_scale_factor;
   const float32_t curvature_th_component  = host.curvature_rear * host.curvature_rear * k_dmc_host_curvature_scale_factor;

   const float32_t threshold_limit = k_dmc_base_thr + host_speed_th_component + curvature_th_component;

   const bool f_bypass_limitation =
      (F360_CONF_AZIMUTH_LOW == conf_az) && (vcs_pos.Distance_To_Origin_Squared() > k_dmc_bypass_det_range_thr_sq);

   const float32_t corrected_moving_threshold =
      f_bypass_limitation ? moving_threshold : std::min(threshold_limit, moving_threshold);

   return corrected_moving_threshold;
}

bool Check_Moving_Hypothesis(const float32_t abs_range_rate_comp, const float32_t range_rate_comp_std,
                             const float32_t range_rate_comp_th, const float32_t moving_sigma_th)
{
   const float32_t range_rate_comp_std_tmp = range_rate_comp_std < F360_EPSILON ? F360_EPSILON : range_rate_comp_std;
   const float32_t moving_sigma            = abs_range_rate_comp / range_rate_comp_std_tmp;
   const bool f_moving_detected            = ((moving_sigma > moving_sigma_th) && (abs_range_rate_comp > range_rate_comp_th));

   return f_moving_detected;
}

void Calculate_Motion_Status(const F360_Detection_T &raw_detection, const F360_Radar_Sensor_T &sensor_data,
                             const F360_Radar_Sensor_Props_T &sensor_props_data, const F360_Host_T &host_veh,
                             F360_Detection_Props_T &detection_props)
{
   // Calculate Detections Uncertainties
   float32_t std_range_rate_compensated_scm = 0.0F;
   const float32_t k_range_rate_std         = 0.06F; // Detection range rate std, taken from radar data sheets.
   Calculate_Detection_Uncertainty(raw_detection, detection_props, host_veh, sensor_data, sensor_props_data, k_range_rate_std,
                                   std_range_rate_compensated_scm);

   // Check if low conf moving detections
   const bool f_azimuth_error_stat_mov = Mark_Detection_With_Low_Detection_Confidence(
      sensor_data.sensor_type, host_veh.vcs_speed, raw_detection, detection_props.range_rate_compensated);

   // Calculate statistical threshold
   const float32_t moving_threshold =
      Calculate_Moving_Threshold(detection_props.vcs_position, raw_detection.confid_azimuth, host_veh, f_azimuth_error_stat_mov);

   // Check moving hypothesis
   const float32_t k_det_motion_sigma_th = 3.0F;
   const bool f_moving_detected          = Check_Moving_Hypothesis(std::abs(detection_props.range_rate_compensated),
                                                                   std_range_rate_compensated_scm, moving_threshold, k_det_motion_sigma_th);

   // Set motion status based on moving hypothesis result
   detection_props.motion_status =
      f_moving_detected ? F360_DETECTION_MOTION_STATUS_MOVING : F360_DETECTION_MOTION_STATUS_AMBIGUOUS;
}

} // namespace f360_variant_A

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
