/*===========================================================================*\
* FILE: f360_internal_preprocessing.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains definitions of Internal_Preprocessing and support functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_internal_preprocessing.h"
#include "f360_mark_object_tracks_next_to_sensors.h"
#include "f360_clusters_preprocessing.h"
#include "f360_configure_rdot_interval_compatibility.h"
#include "f360_update_detection_property.h"
#include "f360_norm_heading_angle.h"
#include "f360_clear_detections_props.h"
#include "f360_update_global_parameters.h"
#include "f360_get_wall_time.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_check_for_low_power_clutter.h"
#include "f360_determine_cv_driving_scenario.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Internal_Preprocessing
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T &host,
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Calibrations_T &calibrations,
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const F360_Core_Info_T &core_info,
   * F360_Host_Props_T &host_props,
   * F360_Globals_T &globals,
   * F360_Detection_Hist_T &det_hist,
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
   * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
   * F360_Tracker_Info_T &tracker_info,
   * F360_TRKR_TIMING_INFO_T &timing_info
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
   * This function calls functions that mark objects next to sensors, update movement threshold parameters, perform cluster preproceesing,
   * update detections history and compute tracker-specific detection properties.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Internal_Preprocessing(
      const F360_Host_T &host,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_Props_T& host_props,
      F360_Globals_T &globals,
      F360_Detection_Hist_T &det_hist,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T &tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      Clear_Detections_Props(detection_props);

      Copy_Detections_Info(raw_detect_list, sensors, detection_props);

      Mark_Object_Tracks_Next_To_Sensors(calibrations, sensors, object_tracks, tracker_info, sensor_props, timing_info);

      Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

      Configure_Rdot_Interval_Compatibility(sensors, globals, calibrations, tracker_info, timing_info);

      Clusters_Preprocessing(host_props, clusters, det_hist, tracker_info);

      Update_Detections_History(host_props, tracker_info, det_hist);

      Update_Detection_Property(sensors, raw_detect_list, host, calibrations, static_env_polys, tracker_info, sensor_props, detection_props, timing_info);

      Check_For_Low_Power_Clutter(host, sensors, raw_detect_list, sensor_props, detection_props, tracker_info);

      tracker_info.f_highway_suspected = Determine_CV_Driving_Scenario(host);

      timing_info.internal_preprocessing= get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Detections_History
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_Props_T &host_props,
   * const F360_Tracker_Info_T& tracker_info,
   * F360_Detection_Hist_T &det_hist
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
   * Updates detections position and azimuth history to account for host motion.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Detections_History(
      const F360_Host_Props_T& host_props,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Hist_T& det_hist)
   {
      const float32_t delta_x = host_props.delta_position_x;
      const float32_t delta_y = host_props.delta_position_y;
      const float32_t rot_mat[2][2] = { { host_props.cos_delta_pointing, host_props.sin_delta_pointing },
      { -host_props.sin_delta_pointing , host_props.cos_delta_pointing } };

      for (uint32_t hist_det_idx = 0U; hist_det_idx < tracker_info.variant.num_hist_dets; hist_det_idx++)
      {
         F360_Detection_Hist_Data_T& detection_history_data = det_hist.det_data[hist_det_idx];
         if (det_hist.f_idx_occupied[hist_det_idx])
         {
            Update_Detection_Position(delta_x, delta_y, rot_mat, detection_history_data.vcs_position_x, detection_history_data.vcs_position_y);
            Update_Detection_History_Azimuth(host_props, detection_history_data);
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Detection_Position
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t delta_x - Host position change in x direction since previous iteration
   * const float32_t delta_y - Host position change in y direction since previous iteration
   * const float32_t(&rot_mat)[2][2] - Rotation matrix from previous to new VCS
   * float32_t &det_x_pos - Detection x position that is updated from previous to new VCS.
   * float32_t &det_y_pos - Detection y position that is updated from previous to new VCS.
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
   * Updates VCS position of historical detections by a translating detection by a delta position expressed in
   * previous VCS and then rotates the new position to coincide with new VCS orientation
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Detection_Position(
      const float32_t delta_x,
      const float32_t delta_y,
      const float32_t(&rot_mat)[2][2],
      float32_t &det_x_pos,
      float32_t &det_y_pos)
   {
      // This function takes a detection position (x,y) and a change in position (delta_x,delta_y) expressed in previous VCS and translates (x,y).
      // After the translation, the new (x,y) is rotated to the new VCS using given rotation matrix rot_mat to get (x,y) in new VCS.


      // Extract detection x and y in previous VCS
      float32_t detection_xy[2][1] = { 0.0F };
      detection_xy[0][0] = det_x_pos;
      detection_xy[1][0] = det_y_pos;

      // Extract delta x and y in previous VCS
      float32_t delta_xy[2][1] = { 0.0F };
      delta_xy[0][0] = delta_x;
      delta_xy[1][0] = delta_y;

      // Translate position in previous VCS with (delta_x,delta_y) in previous VCS.
      float32_t translated_xy[2][1] = { 0.0F };
      translated_xy[0][0] = detection_xy[0][0] - delta_xy[0][0];
      translated_xy[1][0] = detection_xy[1][0] - delta_xy[1][0];

      // Rotate translated coordinate clockwise with delta_pointing to get position in new VCS
      float32_t updated_xy[2][1] = { 0.0F };
      F360_matmul_2x2_2x1(rot_mat, translated_xy, updated_xy);

      // Update the position of detection
      det_x_pos = updated_xy[0][0];
      det_y_pos = updated_xy[1][0];
   }


   /*===========================================================================*\
   * FUNCTION: Update_Detection_History_Azimuth
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_Props_T &host_props - Host properties
   * F360_Detection_Hist_T &detection_hist - Historical detections
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
   * Function to update VCS azimuth for historical detections by compensating for host
   * rotation since last tracker iteration.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Detection_History_Azimuth(
      const F360_Host_Props_T &host_props,
      F360_Detection_Hist_Data_T &detection_history_data)
   {
      // Update azimuth for historical detections with pointing delta between last tracker iteration and current iteration
      detection_history_data.vcs_az = Normalize_Heading_Angle(detection_history_data.vcs_az - host_props.delta_pointing, 0.0F);
   }

   /*===========================================================================*\
    * FUNCTION: Copy_Detections_Info
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
    * Copy detections information that is extended by tracker
    *
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Copy_Detections_Info(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t k = 0U; k < raw_detect_list.number_of_valid_detections; k++)
      {
         const rspp_variant_A::RSPP_Detection_T& raw_det = raw_detect_list.detections[k];
         F360_Detection_Props_T& det = detection_props[k];

         const int32_t sensor_idx = raw_det.raw.sensor_id - 1;
         const float32_t rdot_pred = -sensors[sensor_idx].variable.vcs_velocity.longitudinal * raw_det.processed.cos_vcs_az -
            sensors[sensor_idx].variable.vcs_velocity.lateral * raw_det.processed.sin_vcs_az;

         det.motion_status = static_cast<rspp_variant_A::RSPP_Detection_Motion_Status_T>(raw_det.processed.motion_status);
         det.range_rate_compensated = raw_det.raw.range_rate - rdot_pred;
         det.range_rate_dealiased = raw_det.raw.range_rate;
         det.range_dealiased = raw_det.raw.range;
         det.f_ok_to_use = raw_det.processed.f_ok_to_use;
         det.vcs_position.Set_Position(raw_det.processed.vcs_position_x, raw_det.processed.vcs_position_y);
      }
   }
}
