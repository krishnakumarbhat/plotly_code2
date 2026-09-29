/*===================================================================================*\
* FILE:  f360_predict_exist_prob.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential  Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains funcions to implement the prediction step of existence probability filter.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_predict_exist_prob.h"
#include "f360_visibility_info.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Predict_Existence_Probability()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T &tracker_info - reference to tracker_info struct
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to array of sensors
   * const F360_Calibrations_T& calibs - reference to calibration structure
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - reference to array of object tracks
   * F360_TRKR_TIMING_INFO_T &timing_info - reference to timing_info struct
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
   * This function calculates predicted existence probability of the track (performs time update).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Predict_Existence_Probability(
      const F360_Tracker_Info_T &tracker_info,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t trk_idx = tracker_info.active_obj_ids[i] - 1;
         if ((0 <= trk_idx) && (trk_idx < static_cast<int32_t>(tracker_info.variant.num_tracks)))
         {
            F360_Object_Track_T &obj = object_tracks[trk_idx];

            // initialize existence probability value for each track
            const float32_t ep = obj.exist_prob;

            // calculate probability of birth only for unborn tracks
            const float32_t p_persist = Calculate_P_Persist(obj, sensors, calibs);

            // predict existence probability
            obj.exist_prob = p_persist * ep;
         }
      }

      timing_info.predict_existence_probability = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_P_Persist()
   *===========================================================================
   * RETURN VALUE:
   * float32_t p_persist - probability of persistence
   *
   * PARAMETERS:
   * const F360_Object_Track_T &obj - reference to object track
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors
   * const F360_Calibrations_T& calibs - reference to calibration structure
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
   * This function calculates probability of persistence of the track.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   float32_t Calculate_P_Persist(
      const F360_Object_Track_T &obj,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibs)
   {

      float32_t p_persist = calibs.k_p_persist_outside_long_range;

      for (uint32_t sidx = 0U; sidx < MAX_NUMBER_OF_SENSORS; sidx++)
      {
         if (sensors[sidx].variable.is_valid)
         {
            const float32_t rel_posn_lon = obj.vcs_position.x - sensors[sidx].constant.mounting_position.vcs_position.longitudinal;
            const float32_t rel_posn_lat = obj.vcs_position.y - sensors[sidx].constant.mounting_position.vcs_position.lateral;

            // this simplified test based only on range to the sensor is an approximate solutions
            // it is true only with 4 same sensors mounted on corners and covering 360 deg
            // in case of using additional long range forward looking sensor or configuration
            // with less sensors a check agains interval is also needed
            const bool f_point_below_long_range = Is_Rel_Point_Below_Given_Range_Limit(
               rel_posn_lon, rel_posn_lat, sensors[sidx].constant.range_limits[F360_DET_LOOK_ID_0]);

            if (f_point_below_long_range)
            {
               p_persist = calibs.k_p_persist_inside_long_range;
               break;
            }
         }
      }
      return p_persist;
   }
}

