/*===================================================================================*\
* FILE: f360_msmt_update_object_timestamp.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions to generate and update object timestamp in time update step.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_msmt_update_object_timestamp.h"
namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Msmt_Update_Object_Timestamps()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t elapsed_time_s
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T & object_track
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
   * This function updates object timestamp in measurement update step.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Msmt_Update_Object_Timestamp(
      const float32_t elapsed_time_s,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & object_track)
   {
      if (object_track.ndets > 0U)
      {
         Msmt_Update_Object_Timestamp_Newest_Det(elapsed_time_s, sensors, raw_detect_list, det_props, object_track);
      }
      else
      {
         object_track.time_since_measurement += elapsed_time_s;
      }
   }


   /*===========================================================================*\
   * FUNCTION: Msmt_Update_Object_Timestamp_Newest_Det()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t elapsed_time_s
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T & object_track
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
   * This function updates object timestamp in measurement update step by the
   * timestamp of newest from all updating detections. If there is no valid
   * detection the elapsed time is added to the timestamp.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Msmt_Update_Object_Timestamp_Newest_Det(
      const float32_t elapsed_time_s,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object_track)
   {
      float32_t timestamp_newest;
      bool f_valid_det_found;
      Find_Newest_Det_Timestamp(object_track, sensors, raw_detect_list, det_props, timestamp_newest, f_valid_det_found);

      if (f_valid_det_found)
      {
         object_track.time_since_measurement = timestamp_newest;
      }
      else
      {
         object_track.time_since_measurement += elapsed_time_s;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Newest_Det_Timestamp()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * float32_t & timestamp_newest
   * bool & f_valid_det_found
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
   * This function finds the smallest time since measurement out of all the object's
   * associated detections that are not marked as wheelspin.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Find_Newest_Det_Timestamp(
      const F360_Object_Track_T & object_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t & timestamp_newest,
      bool & f_valid_det_found)
   {
      timestamp_newest = INFTY;
      f_valid_det_found = false;
      for (uint32_t j = 0U; j < object_track.ndets; j++)
      {
         const uint32_t det_idx = static_cast<uint32_t>(object_track.detids[j]) - 1U;
         if ((det_props[det_idx].f_rr_inlier)
            && (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_props[det_idx].wheel_spin_type))
         {
            f_valid_det_found = true;
            const int32_t sensor_idx = raw_detect_list.detections[det_idx].raw.sensor_id - 1;

            if (sensors[sensor_idx].refined.time_since_measurement_s < timestamp_newest)
            {
               timestamp_newest = sensors[sensor_idx].refined.time_since_measurement_s;
            }
         }
      }
   }
}
