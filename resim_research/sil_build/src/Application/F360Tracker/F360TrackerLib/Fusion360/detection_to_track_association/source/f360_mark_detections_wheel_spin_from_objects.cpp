/*===========================================================================*\
* FILE: f360_mark_detections_wheel_spin_from_objects.cpp
*============================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains funcions to find and mark detections as wheel spin detections from objects. This is done by identifying if there
*   is a substantial discrepancy between the object's predicted range rate and the measured range rate of the detection.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/


#include "f360_mark_detections_wheel_spin_from_objects.h"
#include "f360_math.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_calc_predicted_range_rate.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_associate_detection_to_object.h"

namespace f360_variant_A {



   /*===========================================================================*\
   * FUNCTION: Mark_Detections_Wheel_Spin_From_Objects()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS]
   * const float32_t dist_rear_axle_to_vcs_m
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Calibrations_T & calibrations
   * F360_Object_Track_T & object_track
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
   * This function marks associated detections that substantially disagrees with the object's motion as
   * object wheel spin detections.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Detections_Wheel_Spin_From_Objects(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t nr_valid_dets,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calibrations,
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Only try to mark detections as wheel spin detections for objects that are within 30m of host
      if (object_track.f_moving && (std::abs(object_track.vcs_position.x) < calibrations.k_max_abs_vcs_long_posn_for_wheelspin))
      {
         // Determine bounding box around object to check for wheelspin detections
         BoundingBox box{ object_track.bbox };
         Determine_Bounding_Box_Extension(object_track, calibrations, box);

         // Detections which are inside the (non-extended) bounding box around the object, to be
         // used for (type OBJECT) wheel spin detector algorithm
         for (uint32_t det_idx = 0U; det_idx < nr_valid_dets; det_idx++)
         {
            F360_Detection_Props_T& det_prop = detection_props[det_idx];
            /* This is a coarse filter to avoid checking each and every detection.
             * f_low_az_conf_det flag is for detections with low azimuth confidence and such dets do not need to be considered for wheelspin classification
             * f_inside_gate does not necessarily mean it's in the gate of the current object, but it is inside the gate of some object
               which makes the detection a more likely candidate for wheel spin.*/
            if ((det_prop.f_ok_to_use) &&
               (!det_prop.f_low_az_conf_det) &&
               (det_prop.f_inside_gate) &&
               ((det_prop.object_track_id == object_track.id) || (0 == det_prop.object_track_id)))
            {
               if (Is_Det_Wheel_Spin_From_Object(object_track, dets[det_idx], sensors, calibrations, box, det_prop))
               {
                  det_prop.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
                  // If detection is not associated to any object already and classified as moving, associate it to the current object
                  // Even if range rate information of detection is incorrect we can still use it's position
                  if ((0 == det_prop.object_track_id) &&
                     ((F360_TRACKER_TRKFLTR_CTCA == object_track.trk_fltr_type) || (std::abs(object_track.speed) > calibrations.fast_moving_thresh)) &&
                     (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status))
                  {
                     (void)Associate_Detection_To_Object(tracker_info, object_track, det_prop, (det_idx + 1U));
                  }
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Bounding_Box_Extension()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const float32_t dist_rear_axle_to_vcs_m
   * const F360_Calibrations_T & calibrations
   * BoundingBox & box
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
   * This function creates an extended bounding box around an object.
   * The extension depends on track speed.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Determine_Bounding_Box_Extension(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibrations,
      BoundingBox & box)
   {
      const bool f_fast_moving = object_track.speed > calibrations.k_min_speed_fast_moving; //flag may later be set by new track filter
      
      // Extend bounding box only for fast moving objects
      if (f_fast_moving)
      {
         // Determine extension of bounding box in lateral direction
         float32_t ws_lat_buffer_zone_variable;
         if (object_track.f_oncoming)
         {
            // Extension of bounding box in lateral direction for a fast, oncoming, track with a parallel motion to host
            ws_lat_buffer_zone_variable = calibrations.k_ws_lat_buffer_zone_oncoming;
         }
         else
         {
            // Extension of bounding box in lateral direction for a fast track with a parallel motion to host
            ws_lat_buffer_zone_variable = calibrations.k_ws_lat_buffer_zone;
         }
         // Track is fast moving and has a parallel motion to host. Extend bounding box with buffers.
         box.Extend_Boundaries(
            ws_lat_buffer_zone_variable,
            ws_lat_buffer_zone_variable,
            calibrations.k_ws_long_buffer_zone,
            calibrations.k_ws_long_buffer_zone);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Wheel_Spin_From_Object()
   *===========================================================================
   * RETURN VALUE:
   * bool - is detection a wheelspin status flag
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const rspp_variant_A::RSPP_Detection_T & det
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Calibrations_T & calibrations
   * const BoundingBox & wheel_spin_zone
   * const F360_Detection_Props_T & det_prop
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
   * This function checks if a detection that is within an (extended) bounding box around the target has a range rate that
   * substantially differs from the predicted range rate. If so, marks the detection as a wheel spin detection of type "object".
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Wheel_Spin_From_Object(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calibrations,
      const BoundingBox & wheel_spin_zone,
      const F360_Detection_Props_T & det_prop)
   {
      const bool f_det_inside_ws_box = wheel_spin_zone.Contains(det_prop.vcs_position);

      bool f_is_object_wheel_spin = false;
      if (f_det_inside_ws_box)
      {
         // Calculate predicted range rate
         const int32_t sensor_idx = det.raw.sensor_id - 1;
         const float32_t predicted_range_rate = Calc_Predicted_Range_Rate(det_prop, det, object_track, sensors[sensor_idx]);
         const float32_t rng_rate = det_prop.range_rate_dealiased;

         // Check the discrepancy between predicted and detection range rate
         if (std::abs(predicted_range_rate - rng_rate) > calibrations.k_min_rr_diff_wheelspin)
         {
            f_is_object_wheel_spin = true;
         }
      }
      return f_is_object_wheel_spin;
   }
}
