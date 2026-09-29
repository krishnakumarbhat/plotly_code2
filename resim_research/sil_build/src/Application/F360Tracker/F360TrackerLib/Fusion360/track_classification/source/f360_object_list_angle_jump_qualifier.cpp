/*===================================================================================*\
* FILE:  f360_object_list_angle_jump_qualifier.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of Object_List_Angle_Jump_Qualifier() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/

/******************************
* Includes
*******************************/
#include "f360_object_list_angle_jump_qualifier.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Check_Sensor_Config
   *===========================================================================
   * RETURN VALUE:
   * f_front_center_flr7_sensor_present
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
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
   * Checks the sensor config. Returns true is center forward FLR7 sensor is present
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Check_Sensor_Config(const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      bool f_front_center_flr7_sensor_present = false;
      for (uint16_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {  
         if (sensors[i].variable.is_valid)
         {
            const F360_Radar_Sensor_T& current_sensor = sensors[i];
            const F360_Mounting_Location_T current_sensor_mounting_loc = current_sensor.constant.mounting_location;
            const F360_Sensor_Type_T current_sensor_type = current_sensor.constant.sensor_type;

            const bool f_sensor_mounting_loc_front_center = (current_sensor_mounting_loc == F360_MOUNTING_LOCATION_CENTER_FORWARD) ||
               (current_sensor_mounting_loc == F360_MOUNTING_LOCATION_CENTER2_FORWARD) ||
               (current_sensor_mounting_loc == F360_MOUNTING_LOCATION_CENTER3_FORWARD);

            const bool f_sensor_type_flr7 = (current_sensor_type == F360_SENSOR_TYPE_FLR7_RADAR) ||
               (current_sensor_type == F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR) ||
               (current_sensor_type == F360_SENSOR_TYPE_FLR7_PLT_RADAR);

            f_front_center_flr7_sensor_present = ((f_sensor_mounting_loc_front_center) && (f_sensor_type_flr7));
         }
      }

      return f_front_center_flr7_sensor_present;
   }

   /*===========================================================================*\
   * FUNCTION: Object_List_Angle_Jump_Qualifier
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Tracker_Info_T& tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T& dets_raw
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Host_T& host
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * constructor
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
    void Object_List_Angle_Jump_Qualifier(
        const rspp_variant_A::RSPP_Detection_List_T& dets_raw,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const F360_Host_T& host,
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        F360_Tracker_Info_T& tracker_info)
    {
        uint16_t nr_suspected_angle_jumps_in_clustered_objects = 0U;
        
        const bool f_front_center_flr7_sensor_present = Check_Sensor_Config(sensors);

        // Logic applicable only when host is moving fast and FLR7 sensor is present
        if ((host.speed > 20.0F) && (f_front_center_flr7_sensor_present))
        {
            // initialize arrays with crossing objects and clustered crossing objects
            uint16_t ids_of_crossing_objects[NUMBER_OF_OBJECT_TRACKS] = {};
            uint16_t nr_crossing_objects = 0U;
     
            const uint16_t min_num_of_objects_for_clustering = 3U;

            // Find objects that are in front of host and crossing with significant speed
            const bool f_crossing_objects_found = Find_Crossing_Objects(tracker_info, min_num_of_objects_for_clustering, ids_of_crossing_objects, nr_crossing_objects);

            if (f_crossing_objects_found)
            {
                uint16_t ids_of_clustered_crossing_objects[NUMBER_OF_OBJECT_TRACKS] = {};
                uint16_t nr_clustered_crossing_objects = 0U;

                // Cluster crossing objects and consider only objects in clusters with at least three objects
                Cluster_Crossing_Objects(object_tracks, min_num_of_objects_for_clustering, nr_crossing_objects, ids_of_crossing_objects, ids_of_clustered_crossing_objects, nr_clustered_crossing_objects);

                // Count suspected stationary angle jump detections in clustered objects
                Count_Suspected_Stationary_Angle_Jumps_In_Objects(object_tracks, dets_raw, sensors, ids_of_clustered_crossing_objects, nr_clustered_crossing_objects, nr_suspected_angle_jumps_in_clustered_objects);
            }
        }
        // Filter number of suspected stationary angle jump detections in clustered objects through time and return the signal
        Determine_Stationary_Angle_Jump_Signal(nr_suspected_angle_jumps_in_clustered_objects, tracker_info);
    }

    /*===========================================================================*\
    * FUNCTION: Find_Crossing_Objects
    *===========================================================================
    * RETURN VALUE:
    * Bool
    *
    * PARAMETERS:
    * const F360_Tracker_Info_T& tracker_info
    * const uint16_t min_num_of_objects_for_clustering
    * uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS]
    * uint16_t& nr_crossing_objects
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
    * constructor
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    bool Find_Crossing_Objects(
        const F360_Tracker_Info_T& tracker_info,
        const uint16_t min_num_of_objects_for_clustering,
        uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t& nr_crossing_objects)
    {
        nr_crossing_objects = 0U;

        // Thresholds
        constexpr float32_t crossing_absolute_heading_rad = 1.2F;  // 68.75 degrees
        constexpr float32_t heading_diff_threshold_rad = 0.15F;    // 8.60 degrees
        constexpr float32_t min_speed_threshold = 10.0F;

        // Start with first object in front of host
        const F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start;

        for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
        {
            if (NULL == curr_trk)
            {
                break;
            }

            // Check if heading and speed satisfy crossing condition
            // Speed must be greater then 10m/s, absolute heading must be in range 60 - 77 degrees
            const float32_t heading_diff = std::abs(curr_trk->vcs_heading.Value()) - crossing_absolute_heading_rad;
            const bool heading_condition_satisfied = (std::abs(heading_diff) < heading_diff_threshold_rad);
            const bool speed_condition_satisfied = (curr_trk->speed > min_speed_threshold);
            const bool in_front_of_host = (curr_trk->vcs_position.x > 0.0F);
            if ((heading_condition_satisfied) && (speed_condition_satisfied) && (in_front_of_host))
            {
                // Fill crossing objects list
                ids_of_crossing_objects[nr_crossing_objects] = static_cast<uint16_t>(curr_trk->id);
                nr_crossing_objects++;
            }
            // Take next object from the list
            curr_trk = tracker_info.vcslong_sorted_next_track[static_cast<uint16_t>(curr_trk->id) - 1U];
        }

        // Check if we found sufficient number of objects
        const bool f_crossing_objects_found = (nr_crossing_objects >= min_num_of_objects_for_clustering);

        return f_crossing_objects_found;
    }

    /*===========================================================================*\
    * FUNCTION: Cluster_Crossing_Objects
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
    * const uint16_t min_num_of_objects_for_clustering
    * uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS]
    * uint16_t& nr_crossing_objects
    * uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS]
    * uint16_t& nr_clustered_crossing_objects
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
    * constructor
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    void Cluster_Crossing_Objects(
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        const uint16_t min_num_of_objects_for_clustering,
        const uint16_t& nr_crossing_objects,
        const uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t& nr_clustered_crossing_objects)
    {
        const uint16_t max_number_of_iterations = nr_crossing_objects;
        uint16_t number_of_checked_objects = 0U;

        // array of bools to mark whether object is clustered or not
        bool checked_objects[NUMBER_OF_OBJECT_TRACKS] = {};
        
        for (uint16_t iteration = 0U; iteration < max_number_of_iterations; iteration++)
        {
            // Break the loop if all objects are checked
            if (number_of_checked_objects == nr_crossing_objects)
            {
                break;
            }

            // Initialize cluster
            uint16_t current_cluster[NUMBER_OF_OBJECT_TRACKS] = {};
            uint16_t nr_ids_in_cluster = 0U;

            for (uint16_t i = 0U; i < nr_crossing_objects; i++)
            {
                const uint16_t current_id = ids_of_crossing_objects[i];
                if (!checked_objects[i])  // it means that the object is not clustered
                {
                    if (nr_ids_in_cluster == 0U) // empty cluster
                    {
                        // Add current object to empty cluster
                        current_cluster[0U] = current_id;
                        nr_ids_in_cluster++;
                        checked_objects[i] = true; // mark object as clustered
                        number_of_checked_objects++;
                    }
                    else   // non-empty cluster
                    {
                        // Extend cluster with current object if condition is satisfied
                        const F360_Object_Track_T& current_object = object_tracks[current_id - 1U];
                        const F360_Object_Track_T& last_object_in_cluster = object_tracks[current_cluster[nr_ids_in_cluster - 1U] - 1U];
                        if (Satisfy_Cluster_Condition(last_object_in_cluster, current_object))
                        {
                            current_cluster[nr_ids_in_cluster] = static_cast<uint16_t>(current_object.id);
                            nr_ids_in_cluster++;
                            checked_objects[i] = true; // mark object as clustered
                            number_of_checked_objects++;
                        }
                    }
                }
            }
            if (nr_ids_in_cluster >= min_num_of_objects_for_clustering)
            {
                for (uint16_t i = 0U; i < nr_ids_in_cluster; i++)
                {
                    ids_of_clustered_crossing_objects[nr_clustered_crossing_objects] = current_cluster[i];
                    nr_clustered_crossing_objects++;
                }
            }
        }
    }

    /*===========================================================================*\
    * FUNCTION: Satisfy_Cluster_Condition
    *===========================================================================
    * RETURN VALUE:
    * Bool
    *
    * PARAMETERS:
    * const F360_Object_Track_T& last_object_in_cluster
    * const F360_Object_Track_T& current_object
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
    * constructor
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    bool Satisfy_Cluster_Condition(
        const F360_Object_Track_T& last_object_in_cluster,
        const F360_Object_Track_T& current_object)
    {
        const float32_t last_object_heading = last_object_in_cluster.vcs_heading.Value();

        // Compute distance to last object
        const Point last_object_center = last_object_in_cluster.bbox.Get_Center();
        const Point current_object_center = current_object.bbox.Get_Center();
        const float32_t distance_to_last_object_sq = F360_Get_Hypotenuse_Squared(current_object_center.x - last_object_center.x, current_object_center.y - last_object_center.y);

        // Compute distance to line perpendicular to last object's heading
        const float32_t slope = - 1.0F / F360_Tanf(last_object_heading);
        const float32_t intercept = last_object_center.y - slope * last_object_center.x;
        const float32_t distance_to_line_sq = Compute_Dist_From_Point_To_Line_Squared(current_object_center.x, current_object_center.y, slope, intercept);

        // Compute heading and speed difference
        const float32_t heading_diff = std::abs(last_object_heading - current_object.vcs_heading.Value());
        const float32_t speed_diff = std::abs(last_object_in_cluster.speed - current_object.speed);

        // Thresholds
        constexpr float32_t speed_thr = 5.0F;
        constexpr float32_t distance_sq_difference_thr = 30.0F * 30.0F;
        constexpr float32_t heading_thr = 0.05F;
        constexpr float32_t distance_sq_to_line_thr = 15.0F * 15.0F;

        // Check conditions
        const bool speed_condition = (speed_diff < speed_thr);
        const bool distance_condition = (distance_to_last_object_sq < distance_sq_difference_thr);
        const bool heading_condition = (heading_diff < heading_thr);
        const bool distance_to_line_condition = (distance_to_line_sq < distance_sq_to_line_thr);

        const bool f_satisfy_cluster_condition = ((speed_condition)
                                                && (distance_condition)
                                                && (heading_condition)
                                                && (distance_to_line_condition));

        return f_satisfy_cluster_condition;
    }

    /*===========================================================================*\
    * FUNCTION: Count_Suspected_Stationary_Angle_Jumps_In_Objects
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
    * const rspp_variant_A::RSPP_Detection_List_T& dets_raw]
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
    * const uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS]
    * const uint16_t nr_clustered_crossing_objects
    * uint16_t& nr_suspected_angle_jumps_in_clustered_objects
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
    * constructor
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    void Count_Suspected_Stationary_Angle_Jumps_In_Objects(
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        const rspp_variant_A::RSPP_Detection_List_T& dets_raw,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        const uint16_t nr_clustered_crossing_objects,
        uint16_t& nr_suspected_angle_jumps_in_clustered_objects)
    {
        // FLR7 azimuth ambiguity
        constexpr float32_t flr7_angle_shift = F360_DEG2RAD(19.4712F);

        for (uint16_t i = 0U; i < nr_clustered_crossing_objects; i++) // loop over clustered objects
        {
            const uint16_t obj_idx = ids_of_clustered_crossing_objects[i] - 1U;
            const F360_Object_Track_T& object = object_tracks[obj_idx];

            for (uint16_t j = 0U; j < object.ndets; j++) // loop over detections in an object
            {
                // Extract detection from detection list
                const uint32_t det_idx = object.detids[j] - 1U;
                const rspp_variant_A::RSPP_Detection_T& rspp_detection = dets_raw.detections[det_idx];

                // Check if detection is from FLR7 sensor
                const F360_Radar_Sensor_T& sensor = sensors[static_cast<uint16_t>(rspp_detection.raw.sensor_id) - 1U];
                const bool f_flr7_sensor = ((F360_SENSOR_TYPE_FLR7_RADAR == sensor.constant.sensor_type) ||
                   (F360_SENSOR_TYPE_FLR7_PLT_RADAR == sensor.constant.sensor_type) ||
                   (F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR == sensor.constant.sensor_type));

                if (f_flr7_sensor)
                {
                    float32_t new_vcs_az_candidates[4] = {};

                    // Calculate new vcs candidates based on angle jumps
                    new_vcs_az_candidates[0] = sensor.variable.vacs_boresight_az_estimated + (rspp_detection.raw.azimuth + flr7_angle_shift) * static_cast<float32_t>(sensor.constant.polarity);
                    new_vcs_az_candidates[1] = sensor.variable.vacs_boresight_az_estimated + (rspp_detection.raw.azimuth + 2.0F * flr7_angle_shift) * static_cast<float32_t>(sensor.constant.polarity);
                    new_vcs_az_candidates[2] = sensor.variable.vacs_boresight_az_estimated + (rspp_detection.raw.azimuth - flr7_angle_shift) * static_cast<float32_t>(sensor.constant.polarity);
                    new_vcs_az_candidates[3] = sensor.variable.vacs_boresight_az_estimated + (rspp_detection.raw.azimuth - 2.0F * flr7_angle_shift) * static_cast<float32_t>(sensor.constant.polarity);

                    // Hypothesis is that the object that the angle ambiguous detections come from is stationary
                    for (uint8_t az_cand_idx = 0U; az_cand_idx < 4U; az_cand_idx++)
                    {
                        const float32_t rr_comp_new = rspp_detection.raw.range_rate
                            + sensor.variable.vcs_velocity.longitudinal * F360_Cosf(new_vcs_az_candidates[az_cand_idx])
                            + sensor.variable.vcs_velocity.lateral * F360_Sinf(new_vcs_az_candidates[az_cand_idx]);

                        constexpr float32_t range_rate_comp_diff_thres = 0.3F;

                        float32_t dealiased_rr_comp = 0.0F;
                        float32_t rr_interval = 0.0F;

                        // Dealiase against 0.0 compensated range rate
                        const bool f_suspected_angle_jump = Try_To_Dealiase_Range_Rate(
                            rr_comp_new,
                            0.0F,
                            range_rate_comp_diff_thres,
                            sensor.constant.v_wrapping[sensor.variable.look_id],
                            0.0F,
                            dealiased_rr_comp,
                            rr_interval);

                        if (f_suspected_angle_jump)
                        {
                            // Increase the counter
                            nr_suspected_angle_jumps_in_clustered_objects++;
                            break;
                        }
                    }
                }
            }
        }
    }
    /*===========================================================================*\
    * FUNCTION: Determine_Stationary_Angle_Jump_Signal
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const uint16_t nr_suspected_angle_jumps_in_clustered_objects
    * F360_Tracker_Info_T& tracker_info
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
    * constructor
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    void Determine_Stationary_Angle_Jump_Signal(
        const uint16_t nr_suspected_angle_jumps_in_clustered_objects,
        F360_Tracker_Info_T& tracker_info)
    {
        // Determine filter coefficient - slow down decrease by a significant factor
        constexpr float32_t filter_coeff_down = 0.005F;
        constexpr float32_t filter_coeff_up = 0.7F;
        const float32_t filter_coeff = (tracker_info.nr_suspected_stat_angle_jump_dets_filtered > static_cast<float32_t>(nr_suspected_angle_jumps_in_clustered_objects)) ? filter_coeff_down : filter_coeff_up;

        // Low pass filter
        tracker_info.nr_suspected_stat_angle_jump_dets_filtered = F360_Low_Pass_Filter_First_Order(static_cast<float32_t>(nr_suspected_angle_jumps_in_clustered_objects), tracker_info.nr_suspected_stat_angle_jump_dets_filtered, filter_coeff);

        constexpr float32_t stat_angle_jump_dets_thr = 1.5F;

        // Determine the signal
        const bool f_severe_angle_jump_detected = tracker_info.nr_suspected_stat_angle_jump_dets_filtered > stat_angle_jump_dets_thr;

        tracker_info.f_severe_angle_jump_detected = f_severe_angle_jump_detected;
    }
}
