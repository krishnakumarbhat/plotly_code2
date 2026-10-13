#ifndef F360_INPUT_DIAGNOSTICS_VARIANT_A_H
#define F360_INPUT_DIAGNOSTICS_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_input_diagnostics.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * Declaration of a class responsible for diagnostics and fault detection of tracker input signals.
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
 *     ESGW_4-2_PE-SWx_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_constants.h"
#include "f360_core_info.h"
#include "f360_detection_list.h"
#include "f360_host.h"
#include "f360_input_faults.h"
#include "f360_previous_cycle_info.h"
#include "f360_radar_sensor.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
// LCOV_EXCL_START
// Excluded from coverage analysis, due to false alert caused by framework flaw
class Input_Diagnostics
// LCOV_EXCL_STOP
{
 public:
   const struct Calibrations_T
   {
      float32_t min_allowed_core_info_elapsed_time_s;
      float32_t max_allowed_core_info_elapsed_time_s;
      int64_t max_allowed_sensor_vs_tracker_timestamp_diff_us;
      int64_t min_allowed_sensor_vs_tracker_timestamp_diff_us;
      uint16_t max_allowed_rollover_look_index;
      uint16_t min_allowed_rollover_prev_look_index;
      float32_t max_allowed_longitudinal_mounting_position_vcs;
      float32_t min_allowed_longitudinal_mounting_position_vcs;
      float32_t max_allowed_lateral_mounting_position_abs_vcs;
      float32_t max_allowed_azimuth_boresight_angle_abs_vcs;
      int32_t valid_polarity_abs;
      float32_t min_allowed_detection_range_m;
      float32_t max_allowed_detection_range_m;
      float32_t min_allowed_range_rate_ms;
      float32_t max_allowed_range_rate_ms;
      float32_t min_allowed_raw_azimuth_angle;
      float32_t max_allowed_raw_azimuth_angle;
      float32_t min_allowed_raw_elevation_angle;
      float32_t max_allowed_raw_elevation_angle;
      int32_t min_allowed_raw_sensor_id;
      int32_t max_allowed_raw_sensor_id;
      int32_t min_allowed_raw_det_id;
      int32_t max_allowed_raw_det_id;
      float32_t min_allowed_v_wrapping;
      float32_t max_allowed_v_wrapping;
      float32_t min_allowed_aliased_range_rate;
      float32_t max_allowed_aliased_range_rate;

      Calibrations_T();
   } calib;

   Input_Diagnostics();
   ~Input_Diagnostics() = default;
   virtual Input_Faults_T Execute(const F360_Core_Info_T &core_info, const F360_Host_T &host,
                                  const F360_Detection_List_T &raw_detect_list,
                                  const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);
   virtual void Initialize();

 private:
   Previous_Cycle_Info_T previous_cycle_info;
   Core_Info_Faults_T Check_F360_Core_Info(const F360_Core_Info_T &core_info) const;
   Host_Info_Faults_T Check_F360_Host_Info(const F360_Host_T &host) const;
   Raw_Detection_Faults_T Check_F360_Radar_Raw_Detection(const F360_Detection_List_T &raw_detect_list) const;
   Radar_Sensor_Faults_T Check_F360_Radar_Sensor(const F360_Radar_Sensor_T &sensor, const F360_Core_Info_T &core_info,
                                                 const Radar_Sensor_Reduced_T &previous_cycle_sensor) const;
   Radar_Sensor_Calib_Faults_T Check_F360_Radar_Sensor_Calib(const F360_Radar_Sensor_T &sensor) const;
   bool Check_Sensor_Look_Index_Rollover(const F360_Radar_Sensor_T &sensor,
                                         const Radar_Sensor_Reduced_T &previous_cycle_sensor) const;
   bool Check_Sensor_Look_Index_No_Increase(const F360_Radar_Sensor_T &sensor,
                                            const Radar_Sensor_Reduced_T &previous_cycle_sensor) const;
   void Update_Previous_Signals(const F360_Core_Info_T &core_info, const F360_Host_T &host,
                                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);
};
} // namespace f360_variant_A

#endif // F360_INPUT_DIAGNOSTICS_H
