/*===================================================================================*\
* FILE:  f360_detection_association_countermeasures.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions for different countermeasures active related to detection
* to track association.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_detection_association_countermeasures.h"
#include "f360_mark_detections_wheel_spin_from_objects.h"
#include "f360_mark_dets_as_close_target_and_farside.h"
#include "f360_find_detection_inliers.h"
#include "f360_nearby_wheel_spins.h"
#include "f360_mark_azimuth_range_rate_outliers.h"
#include "f360_cond_deassoc_low_rr_dets.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_detection_association_support_functions.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Detection_Association_Countermeasures()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Calibrations_T & calibrations
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * This function collects and calls all countermeasures active for detection
   * association, both detection-based and object-based.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Detection_Association_Countermeasures(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Track-based countermeasures
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         F360_Object_Track_T& object = object_tracks[obj_idx];

         Deassociate_And_Count_Double_Bounce_Detections(object, detection_props);

         Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

         Deassociate_Suspected_Ground_Detections(raw_detection_list.detections, object, detection_props);

         Deassociate_Ambiguous_Detections(object, detection_props);

         Mark_Detections_Wheel_Spin_From_Objects(
            tracker_info,
            raw_detection_list.detections,
            raw_detection_list.number_of_valid_detections,
            sensors,
            calibrations,
            object,
            detection_props);

         Mark_Azimuth_Range_Rate_Outliers(
            object,
            calibrations,
            host.dist_rear_axle_to_vcs_m,
            raw_detection_list,
            detection_props);

         Mark_Dets_As_Close_Target_And_Farside(
            raw_detection_list.number_of_valid_detections,
            object,
            detection_props,
            calibrations);

         Cond_Deassoc_Low_RR_Dets(
            calibrations,
            detection_props,
            object);

         Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(
            raw_detection_list,
            sensors,
            calibrations.k_min_range_rate_error_threshold, 
            detection_props, 
            object);

         Calc_Percentage_Of_Assoc_Dets(object);
      }
      // Detection-based countermeasure
      Detect_Nearby_Wheel_Spins(raw_detection_list, calibrations, detection_props);
   }
   /*===========================================================================*\
   * FUNCTION: Deassociate_And_Count_Double_Bounce_Detecions()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  F360_Object_Track_T& object
   *  F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Deassociate and count detections that are double bounce.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Deassociate_And_Count_Double_Bounce_Detections(
       F360_Object_Track_T& object,
       F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
       object.num_db_dets = 0U;
       if (object.ndets > 0U)
       {
           uint32_t ok_dets_ids[MAX_DETS_IN_OBJ_TRK] = {};
           uint32_t num_ok_dets = 0U;
           for (uint32_t j = 0U; j < object.ndets; j++)
           {
               const uint32_t det_idx = object.detids[j] - 1U;
               if (detection_props[det_idx].f_double_bounce)
               {
                   // deassociate double bounce detection
                   object.num_db_dets++;
                   detection_props[det_idx].f_ok_to_use = false;
                   detection_props[det_idx].object_track_id = 0;

                   if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == detection_props[det_idx].motion_status)
                   {
                       object.num_types_of_dets[0]--;
                   }
                   else
                   {
                       object.num_types_of_dets[1]--;
                   }
               }
               else
               {
                   ok_dets_ids[num_ok_dets] = det_idx + 1U;
                   num_ok_dets++;
               }
           }

           object.ndets = num_ok_dets;
           (void)std::copy(cmn::begin(ok_dets_ids), cmn::end(ok_dets_ids), cmn::begin(object.detids));
           //saturate to 20 to avoid too big values
           object.historic_num_db_dets_with_forgetting_factor = std::min(object.historic_num_db_dets_with_forgetting_factor + static_cast<float32_t>(object.num_db_dets), 20.0F);
           // forgetting factor to avoid dependency on the ancient history
           const float32_t historic_num_db_dets_forgetting_factor = 0.97F;
           object.historic_num_db_dets_with_forgetting_factor = historic_num_db_dets_forgetting_factor * object.historic_num_db_dets_with_forgetting_factor;
       }
   }
   /*===========================================================================*\
   * FUNCTION: Deassociate_Stationary_Range_Rate_Outlier_Dets()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS]
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   *  F360_Object_Track_T& obj
   *  F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]
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
   * Deassociate detections that seem to be stationary range rate outliers.
   * Detections with small compensated predicted range rate are excluded to not risk discarding detections from
   * where the sensor can't measure non-zero range rates.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Deassociate_Stationary_Range_Rate_Outlier_Dets(
      const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS])
   {

      if ((obj.movable_prob > 0.5F) && (std::abs(obj.speed) < 2.5F))
      {
         uint32_t num_moving_dets = 0U;
         uint32_t moving_dets_ids[MAX_DETS_IN_OBJ_TRK]{};
         uint32_t num_amb_dets = 0U;
         uint32_t amb_dets_idx[MAX_DETS_IN_OBJ_TRK]{};
         for (uint32_t i = 0U; i < obj.ndets; i++)
         {
            const uint32_t det_idx = obj.detids[i] - 1U;
            const uint32_t sensor_idx = static_cast<uint32_t>(dets_raw[det_idx].raw.sensor_id) - 1U;
            const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];
            const float32_t predicted_compensated_range_rate = dets_prop[det_idx].range_rate_predicted + sensor.variable.vcs_velocity.longitudinal * dets_raw[det_idx].processed.cos_vcs_az
               + sensor.variable.vcs_velocity.lateral * dets_raw[det_idx].processed.sin_vcs_az;

            if ((rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == dets_prop[det_idx].motion_status) || (std::abs(predicted_compensated_range_rate) < 0.2F))
            {
               moving_dets_ids[num_moving_dets] = det_idx + 1U;
               num_moving_dets++;
            }
            else
            {
               amb_dets_idx[num_amb_dets] = det_idx;
               num_amb_dets++;
            }
         }

         if ((num_moving_dets > 2U) && (num_amb_dets > 0U))
         {
            (void)std::copy(cmn::begin(moving_dets_ids), cmn::end(moving_dets_ids), cmn::begin(obj.detids));
            obj.ndets = num_moving_dets;

            for (uint32_t i = 0U; i < num_amb_dets; i++)
            {
               dets_prop[amb_dets_idx[i]].f_ok_to_use = false;
               dets_prop[amb_dets_idx[i]].object_track_id = 0;
               obj.num_types_of_dets[1]--;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Deassociate_Suspected_Ground_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T& obj
   * F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]
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
   * Deassociate stationary detections that are suspected to be from the ground from slow moving objects in
   * an area close to host.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Deassociate_Suspected_Ground_Detections(
      const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (obj.f_moving && (std::abs(obj.speed) < 2.5F))
      {
         uint32_t num_ok_dets = 0U;
         uint32_t ok_dets_ids[MAX_DETS_IN_OBJ_TRK]{};
         uint32_t num_not_ok_dets = 0U;
         uint32_t not_ok_dets_indices[MAX_DETS_IN_OBJ_TRK]{};

         for (uint32_t i = 0U; i < obj.ndets; i++)
         {
            const uint32_t det_idx = obj.detids[i] - 1U;
            // Note that the z axis is negative upwards from the ground.
            const bool f_low_z_det_close_to_host = ((std::abs(dets_raw[det_idx].processed.vcs_position_z) < 0.2F) && (std::abs(dets_prop[det_idx].vcs_position.x) < 15.0F) && (std::abs(dets_prop[det_idx].vcs_position.y) < 4.0F));
            const bool f_ambiguous_low_rcs_det = ((dets_prop[det_idx].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS) && (dets_raw[det_idx].raw.rcs < -10.0F));
            if (f_low_z_det_close_to_host && f_ambiguous_low_rcs_det)
            {
               not_ok_dets_indices[num_not_ok_dets] = det_idx;
               num_not_ok_dets++;
            }
            else
            {
               ok_dets_ids[num_ok_dets] = det_idx + 1U;
               num_ok_dets++;
            }
         }

         if (num_not_ok_dets > 0U)
         {
            (void)std::copy(cmn::begin(ok_dets_ids), cmn::end(ok_dets_ids), cmn::begin(obj.detids));
            obj.ndets = num_ok_dets;

            for (uint32_t i = 0U; i < num_not_ok_dets; i++)
            {
               dets_prop[not_ok_dets_indices[i]].f_ok_to_use = false;
               dets_prop[not_ok_dets_indices[i]].object_track_id = 0;
               obj.num_types_of_dets[1]--;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Deassociate_Ambiguous_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& obj,
   * F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]
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
   * Deassociate detections that are ambigous
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Deassociate_Ambiguous_Detections(
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS])
   {
      uint32_t num_ok_dets = 0U;
      uint32_t ok_dets_ids[MAX_DETS_IN_OBJ_TRK]{};
      uint32_t num_not_ok_dets = 0U;
      uint32_t not_ok_dets_indices[MAX_DETS_IN_OBJ_TRK]{};

      for (uint32_t i = 0U; i < obj.ndets; i++)
      {
         const uint32_t det_idx = obj.detids[i] - 1U;
         // Note that the z axis is negative upwards from the ground.
         if (dets_prop[det_idx].f_rr_ambiguity)
         {
            not_ok_dets_indices[num_not_ok_dets] = det_idx;
            num_not_ok_dets++;
         }
         else
         {
            ok_dets_ids[num_ok_dets] = det_idx + 1U;
            num_ok_dets++;
         }
      }

      if (num_not_ok_dets > 0U)
      {
         (void)std::copy(cmn::begin(ok_dets_ids), cmn::end(ok_dets_ids), cmn::begin(obj.detids));
         obj.ndets = num_ok_dets;

         for (uint32_t i = 0U; i < num_not_ok_dets; i++)
         {
            dets_prop[not_ok_dets_indices[i]].object_track_id = 0;
            obj.num_types_of_dets[1]--;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const float32_t k_min_range_rate_error_threshold,
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& object
   *
   * EXTERNAL REFERENCES:
   * Convert_VCS_Posn_To_TCS_Posn()
   * F360_Sqrtf()
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Deassociates detections that are range rate outliers and are
   * positioned laterally far from the object center. This countermeasure only
   * applies to large, fast-moving objects with rear reference points and are
   * moving in a straight.
   *
   * The function performs check the detections in two stages:
   * 1. Evaluates range rate error against a dynamic threshold
   * 2. For detections range rate above the threshold, check if they are
   *    laterally distant from the object (1m from object bounding box edge)
   *
   * Only detections that fail BOTH criteria are deassociated
   *
   * PRECONDITIONS:
   * - Object must have rear reference point (rear, rear-left, or rear-right)
   * - Object length > 7.0m
   * - Object speed > 10.0 m/s  
   * - Object heading rate < 0.1 rad/s
   * - Object heading deviation < 5 degrees
   * - Object lateral association gates > 1.0m
   *
   * POSTCONDITIONS:
   * - Range rate outliers that are laterally far are deassociated
   * - Object detection count and IDs are updated
   * - Deassociated detections have object_track_id set to 0
   *
   \*===========================================================================*/
   void Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t k_min_range_rate_error_threshold,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object)
   {  
      constexpr float32_t LATERAL_THRESHOLD_M = 1.0F;
      constexpr float32_t MIN_OBJECT_LENGTH_M = 7.0F;
      constexpr float32_t MAX_HEADING_RATE_RAD_S = 0.1F;
      constexpr float32_t MIN_SPEED_MS = 10.0F;
      constexpr float32_t MAX_HEADING_DEV_RAD = F360_DEG2RAD(5.0F);

      // Only consider objects that have a rear reference point and are sufficiently large, fast and straight moving
      const bool f_valid_ref_point = (
         (F360_REFERENCE_POINT_REAR == object.reference_point) ||
         (F360_REFERENCE_POINT_REAR_RIGHT == object.reference_point) ||
         (F360_REFERENCE_POINT_REAR_LEFT == object.reference_point)
      );

      const bool f_preconditions = (
         (object.bbox.Get_Length() > MIN_OBJECT_LENGTH_M) && 
         (std::abs(object.heading_rate) < MAX_HEADING_RATE_RAD_S) && 
         (object.speed > MIN_SPEED_MS) && 
         (std::abs(object.vcs_heading.Value()) < MAX_HEADING_DEV_RAD) &&
         (object.lat_buffer_zone_wid1 > LATERAL_THRESHOLD_M) &&
         (object.lat_buffer_zone_wid2 > LATERAL_THRESHOLD_M) &&
         f_valid_ref_point
      );

      if (f_preconditions)
      {
         uint32_t ok_dets_ids[MAX_DETS_IN_OBJ_TRK] = {};
         uint32_t num_ok_dets = 0U;
         int32_t num_moving_dets = 0; 
         int32_t num_other_dets = 0;

         // Calculate range rate error threshold
         float32_t filtered_hist_assoc_det_rr_err_std = 0.0F;
         if (object.filtered_hist_assoc_det_rr_err_var > 0.0F)
         {
            filtered_hist_assoc_det_rr_err_std = F360_Sqrtf(object.filtered_hist_assoc_det_rr_err_var);
         }
         else
         {
            filtered_hist_assoc_det_rr_err_std = 0.0F;
         }
         float32_t range_rate_error_threshold = std::max(object.filtered_hist_assoc_det_rr_err_mean + filtered_hist_assoc_det_rr_err_std, k_min_range_rate_error_threshold);
         // Increase threshold to avoid too aggressive deassociations
         constexpr float32_t RANGE_RATE_ERROR_MULTIPLIER = 2.0F;
         range_rate_error_threshold *= RANGE_RATE_ERROR_MULTIPLIER;

         // Pre calculate object properties
         const Point obj_center = object.bbox.Get_Center();
         const float32_t object_width_half = object.bbox.Get_Width() * 0.5F;
         const Angle object_orientation = object.bbox.Get_Orientation();
         const float32_t lateral_limit = object_width_half + LATERAL_THRESHOLD_M;

         // Check each detection associated to object
         for (uint32_t i = 0U; i < object.ndets; i++)
         {
            const uint32_t det_idx = object.detids[i] - 1U;
            F360_Detection_Props_T& det_prop = detection_props[det_idx];

            const float32_t predicted_rdot = det_prop.range_rate_predicted;
            const float32_t det_rdot = det_prop.range_rate_dealiased; // Detection should always be dealiased at this stage since it is associated to an object
            const float32_t abs_rdot_diff = std::abs(predicted_rdot - det_rdot);
            // Check range rate error first (cheaper operation)
            const bool det_rr_ok = abs_rdot_diff < range_rate_error_threshold;
            bool det_lateral_ok = true;
            // Only check lateral distance if range rate is not OK
            if (!det_rr_ok)
            {
               float32_t dets_tcs_x = 0.0F;
               float32_t dets_tcs_y = 0.0F;
               Convert_VCS_Posn_To_TCS_Posn(
                  det_prop.vcs_position.x, det_prop.vcs_position.y, 
                  obj_center.x, obj_center.y, 
                  object_orientation, dets_tcs_x, dets_tcs_y);

               det_lateral_ok = std::abs(dets_tcs_y) < lateral_limit;
            }
            if (det_rr_ok || det_lateral_ok)
            {
               // Add to temporary array of OK detections
               ok_dets_ids[num_ok_dets] = det_idx + 1U;
               num_ok_dets++; 
               if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status)
               {
                  //Count number of moving dets that remain associated
                  num_moving_dets++;
               }
               else
               {
                  //Count number of "other" dets that remain associated
                  num_other_dets++;
               }
            }
            else
            {
               const rspp_variant_A::RSPP_Detection_T& detection = raw_detect_list.detections[det_idx];
               const F360_Radar_Sensor_T& sensor = sensors[detection.raw.sensor_id - 1];
               Deassociate_Detection(detection, sensor, det_prop);
            }
         }
         if (num_ok_dets != object.ndets)
         {  
            // Detections has been deassociated
            // Update object with the remaining associated detections
            object.ndets = num_ok_dets;
            object.num_types_of_dets[0] = num_moving_dets;
            object.num_types_of_dets[1] = num_other_dets;
            (void)std::copy_n(cmn::begin(ok_dets_ids), num_ok_dets, cmn::begin(object.detids));
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Percentage_Of_Assoc_Dets()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& obj,
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
   * Calculate percentage of associated detections to the number of all the detections
   * in object's extended bbox
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Percentage_Of_Assoc_Dets(
      F360_Object_Track_T& object)
   {
      float32_t assoc_dets_pct = 1.0F;
      if (object.num_dets_in_ext_bbox != 0U)
      {
         assoc_dets_pct = static_cast<float32_t>(object.ndets) / static_cast<float32_t>(object.num_dets_in_ext_bbox);
      }
      constexpr float32_t alpha = 0.1F;
      object.assoc_dets_pct_filtered = alpha * assoc_dets_pct + (1.0F - alpha) * object.assoc_dets_pct_filtered;
   }
}
