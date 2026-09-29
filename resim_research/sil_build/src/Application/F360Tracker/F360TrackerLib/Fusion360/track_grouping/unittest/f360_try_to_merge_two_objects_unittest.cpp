/** \file
 * This file contains unit tests for content of f360_try_to_merge_two_objects.cpp file
 */

#include "f360_try_to_merge_two_objects.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_try_to_merge_two_objects
 *  @{
 */

/** \brief
 * Test group designed for testing Try_To_Merge_Two_Objects function
 */
TEST_GROUP(f360_try_to_merge_two_objects)
{
   F360_Host_T host = {};
   F360_Globals_T globals = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   int32_t kill_idx;
   int32_t idx1;
   int32_t idx2;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   
   /** \setup
    * Set up object list with two active objects, that should be merged
    */
   TEST_SETUP()
   {
      // Set up tracker calibrations
      Initialize_Tracker_Calibrations(calib);
      host.dist_rear_axle_to_vcs_m = 3.0F;

      globals.f_single_front_center_radar_only = false;

      // Set up properties of the first object
      idx1 = 10;
      object_tracks[idx1].id = idx1;
      object_tracks[idx1].vcs_position.x = 20.0F;
      object_tracks[idx1].vcs_position.y = -5.0F;
      object_tracks[idx1].speed = 10.0F;
      object_tracks[idx1].vcs_heading = Angle{ 0.2F };
      object_tracks[idx1].movable_prob = 1.0F;
      object_tracks[idx1].f_moving = true;
      object_tracks[idx1].time_since_initialization = 11.0F;
      object_tracks[idx1].bbox.Set_Orientation(object_tracks[idx1].vcs_heading);
      object_tracks[idx1].Update_Bbox_Size(3.8F, 2.0F);
      object_tracks[idx1].hdg_ptng_disagmt = 0.0F;
      object_tracks[idx1].ndets = 1U;
      object_tracks[idx1].detids[0] = 1U;

      detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.x = object_tracks[idx1].vcs_position.x;
      detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
      raw_detection_list.detections[object_tracks[idx1].detids[0]-1U].raw.sensor_id = 1U;

      tracker_info.active_obj_ids[tracker_info.num_active_objs] = idx1 + 1;
      tracker_info.num_active_objs++;

      // Set up properties of the second object
      idx2 = 20;
      object_tracks[idx2].id = idx2;
      object_tracks[idx2].vcs_position.x = 20.5F;
      object_tracks[idx2].vcs_position.y = -5.2F;
      object_tracks[idx2].speed = 10.1F;
      object_tracks[idx2].vcs_heading = Angle{ 0.2F };
      object_tracks[idx2].movable_prob = 1.0F;
      object_tracks[idx2].f_moving = true;
      object_tracks[idx2].time_since_initialization = 15.0F;
      object_tracks[idx2].bbox.Set_Orientation(object_tracks[idx2].vcs_heading);
      object_tracks[idx2].Update_Bbox_Size(3.1F, 1.7F);
      object_tracks[idx2].hdg_ptng_disagmt = 0.0F;
      object_tracks[idx2].ndets = 1U;
      object_tracks[idx2].detids[0] = 2U;

      detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.x = object_tracks[idx2].vcs_position.x;
      detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
      raw_detection_list.detections[object_tracks[idx2].detids[0]-1U].raw.sensor_id = 1U;

      tracker_info.active_obj_ids[tracker_info.num_active_objs] = idx2 + 1;
      tracker_info.num_active_objs++;
      Set_Tracker_Variant(tracker_info.variant);
   }

};

/** \purpose  
 * Check if objects are merged when all merging conditions fulfilled
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Merge_Objects_When_All_Conditions_Fulfilled)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected number of objects after merge operation
    */
   const int32_t expected_num_active_objs = 1;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has decreased which indicates a merge operation
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are not merged when some of conditions fails
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Not_Merge_Objects_When_Any_Condition_Failed)
{
   /** \precond
    * Modify second object heading to do not match the first object
    * Set expected number of objects if no merges are done
    */
   object_tracks[idx2].vcs_heading = Angle{ 1.0F };
   const int32_t expected_num_active_objs = 2;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has remained the same - no merge operations
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if younger object is killed when passed as idx2
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Kill_Younger_Object_When_Passed_As_Idx2)
{
   /** \precond
    * Set active objects lifespan, idx1 older than idx2
    * Set expected kill idx
    */
   object_tracks[idx1].time_since_initialization = 2.0F;
   object_tracks[idx2].time_since_initialization = 1.0F;

   const int32_t expected_kill_idx = 20;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if younger object has been killed
    */
   CHECK_EQUAL(expected_kill_idx, kill_idx);

}

/** \purpose
 * Check if younger object is killed when passed as idx1
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Kill_Younger_Object_When_Passed_As_Idx1)
{
   /** \precond
    * Set active objects lifespan, idx2 older than idx1
    * Set expected kill idx
    */
   object_tracks[idx1].time_since_initialization = 1.0F;
   object_tracks[idx2].time_since_initialization = 2.0F;

   const int32_t expected_kill_idx = 10;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if younger object has been killed
    */
   CHECK_EQUAL(expected_kill_idx, kill_idx);

}

/** \purpose
 * Check if objects are not merged when pre-merge size condition not fulfilled
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Not_Merge_Objects_When_Size_Condition_Not_Fulfilled)
{
   /** \precond
    * Set first object width to big value (> 3.9m) and time since split to negative values to enforce fail in size check
    * Set expected number of objects - no merge operations
    */
   object_tracks[idx1].Update_Bbox_Size(4.0F, 10.8F);
   object_tracks[idx1].time_since_split = -1.0F;

   const int32_t expected_num_active_objs = 2;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has remained the same - indicates no merge operations
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}


/** \purpose
 * Check if after merging, vehicular class freeze timer is set correctly when both object have different freeze timers > 0.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Object_Inherits_Lower_Veh_Class_Freeze_Timer_After_Merge)
{
    /** \precond
     * All object properites set in test setup section
     * Set different freeze timers (> 0) for both objects
     */
    object_tracks[idx1].time_since_obj_considered_veh_for_class_freeze = 5.0F;
    object_tracks[idx2].time_since_obj_considered_veh_for_class_freeze = 3.0F;
    const float32_t expected_freeze_timer = 3.0F; // minimum of time_since_obj_considered_veh_for_class_freeze from objects[idx1] and object[idx2]

    /** \action
     * Run Try_To_Merge_Two_Objects()
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if merged objects' freeze timer is set to the lower of the two
     */
    const int32_t kept_obj_idx = (kill_idx == idx1) ? idx2 : idx1;
    DOUBLES_EQUAL(expected_freeze_timer, object_tracks[kept_obj_idx].time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Check if after merging, vehicular class freeze timer is set correctly when one object has its freeze timer not initialized (=-1).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Object_Inherits_Higher_Veh_Class_Freeze_Timer_After_Merge)
{
    /** \precond
     * All object properites set in test setup section
     * Set freeze timer for one object and uninitialized value for the other
     */
    object_tracks[idx1].time_since_obj_considered_veh_for_class_freeze = 5.0F;
    object_tracks[idx2].time_since_obj_considered_veh_for_class_freeze = -1.0F;
    const float32_t expected_freeze_timer = 5.0F;

    /** \action
     * Run Try_To_Merge_Two_Objects()
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if merged objects' freeze timer is set to the higher of the two
     */
    const int32_t kept_obj_idx = (kill_idx == idx1) ? idx2 : idx1;
    DOUBLES_EQUAL(expected_freeze_timer, object_tracks[kept_obj_idx].time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Check if after merging, vehicular class freeze timer is set correctly when both object has their freeze timer not initialized (=-1).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Object_Inherits_Higher_Veh_Class_Freeze_Timer_After_Merge_When_Timer_For_Both_Is_Invalid)
{
    /** \precond
     * All object properites set in test setup section
     * Set freeze timer  uninitialized value for both objects
     */
    object_tracks[idx1].time_since_obj_considered_veh_for_class_freeze = -1.0F;
    object_tracks[idx2].time_since_obj_considered_veh_for_class_freeze = -1.0F;
    const float32_t expected_freeze_timer = -1.0F;

    /** \action
     * Run Try_To_Merge_Two_Objects()
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if merged objects' freeze timer is unchaged
     */
    const int32_t kept_obj_idx = (kill_idx == idx1) ? idx2 : idx1;
    DOUBLES_EQUAL(expected_freeze_timer, object_tracks[kept_obj_idx].time_since_obj_considered_veh_for_class_freeze, 1e-6F);
}

/** \purpose
 * Check if objects are merged when detections fall within the extended merged bounding box
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Objects_Should_Merge_When_Detections_Within_Extended_Merged_BBox)
{
   /** \precond
    * Reposition detections to be within extended bounding box but outside the non-extended merged bbox
    * Detections are placed in the buffer zone (extended area)
    */
   object_tracks[idx1].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx1].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx1].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx1].long_buffer_zone_len2 = 1.0F;
   object_tracks[idx2].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx2].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx2].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx2].long_buffer_zone_len2 = 1.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.x = 20.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.y = -7.0F; // Outside core bbox, in lateral buffer zone
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.x = 20.5F;
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.y = -7.2F; // Outside core bbox, in lateral buffer zone

   /** \action
    * Run tested function with detections inside extended merged bbox but outside non-extended bbox
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has decreased which indicates a merge operation
    */
   const int32_t expected_num_active_objs = 1;
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are NOT merged when no detections fall within the extended merged bounding box
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Objects_Should_Not_Merge_When_No_Detections_Within_Extended_Merged_BBox)
{
   /** \precond
    * Move both detections far away from the merged bounding box (far forward)
    */
   object_tracks[idx1].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx1].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx1].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx1].long_buffer_zone_len2 = 1.0F;
   object_tracks[idx2].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx2].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx2].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx2].long_buffer_zone_len2 = 1.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.x = 50.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.y = 0.0F;
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.x = 50.5F;
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.y = 0.0F;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has remained the same - no merge operations
    */
   const int32_t expected_num_active_objs = 2;
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged when at least one detection falls within the extended merged bounding box
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Objects_Should_Merge_When_One_Detection_Within_Extended_Merged_BBox)
{
   /** \precond
    * Move the first detection far away outside the extended merged bbox
    * Keep the second detection in the buffer zone (extended bbox but outside non-extended bbox)
    */
   object_tracks[idx1].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx1].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx1].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx1].long_buffer_zone_len2 = 1.0F;
   object_tracks[idx2].lat_buffer_zone_wid1 = 2.0F;
   object_tracks[idx2].lat_buffer_zone_wid2 = 2.0F;
   object_tracks[idx2].long_buffer_zone_len1 = 1.0F;
   object_tracks[idx2].long_buffer_zone_len2 = 1.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.x = 50.0F;
   detection_props[object_tracks[idx1].detids[0]-1U].vcs_position.y = 0.0F; // Far outside
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.x = 20.5F;
   detection_props[object_tracks[idx2].detids[0]-1U].vcs_position.y = -7.2F; // In buffer zone

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has decreased which indicates a merge operation
    */
   const int32_t expected_num_active_objs = 1;
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose  
 * Check if objects are merged when all merging conditions fulfilled and the keep object
 * has low speed and needs detection support for merging.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Merge_Objects_When_Detection_Support_Required)
{
   /** \precond
    * All object properties set in test setup section except
    * speed which is set to below 5.0m/s for first keep object.
    * Set kill object to have different speed and set object heading 
    * to differ from each other to generate high coarse_gate_score.
    * 
    * Set expected number of objects after merge operation
    */
   const int32_t expected_num_active_objs = 1;
   object_tracks[idx1].speed = 4.9F;
   object_tracks[idx2].speed = 3.95F;
   object_tracks[idx1].vcs_heading = Angle{ 0.0F };
   object_tracks[idx2].vcs_heading = Angle{ F360_DEG2RAD(29.0F) };

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has decreased which indicates a merge operation
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose  
 * Check if objects are merged when all merging conditions fulfilled and the keep object
 * has low speed and does not need detection support for merging, due to coarse gate score low enough.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Merge_Objects_When_Detection_Support_Not_Required_Speed_Low_Too_Low_Coarse_Gate)
{
   /** \precond
    * All object properties set in test setup section except
    * speed which is set to below 5.0m/s for both objects.
    * 
    * Set expected number of objects after merge operation
    */
   const int32_t expected_num_active_objs = 1;
   object_tracks[idx1].speed = 4.9F;
   object_tracks[idx2].speed = 4.9F;

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has decreased which indicates a merge operation
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose  
 * Check that objects are not merged when all merging conditions fulfilled and the keep object
 * has low speed and does not need detection support for merging, due to coarse gate score low enough,
 * but the total size of the merged object would be too large.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects, Should_Not_Merge_Objects_When_Det_Support_Ok_But_Size_Too_Large)
{
   /** \precond
    * All object properties set in test setup section except
    * speed which is set to below 5.0m/s for both objects so the coarse gate score is low.
    * Change obj2's size such that the total object size exceeds the threshold for merging.
    * 
    * Set expected number of objects after merge operation
    */
   const int32_t expected_num_active_objs = 2;
   object_tracks[idx1].speed = 4.9F;
   object_tracks[idx2].speed = 4.9F;
   object_tracks[idx2].Update_Bbox_Size(7.0F, 2.0F);

   /** \action
    * Run tested function
    */
   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

   /** \result
    * Check if overall number of objects has not decreased which indicates no merge operation
    */
   CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** @}*/

/** \defgroup  f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept 
 *  @{
 */

/** \brief
 * Test group designed for testing Choose_Obj_Idx_To_Be_Kept function
 */
TEST_GROUP(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept)
{
   F360_Globals_T globals = {};
   F360_Object_Track_T obj_1 = {};
   F360_Object_Track_T obj_2 = {};
   int32_t idx1;
   int32_t idx2;
   int32_t keep_idx;
   int32_t kill_idx;

   /** \setup
    * Set up two objects with
    *   - Filter type CTCA
    *   - Time since initialization > 2 s
    *   - obj1 closer to host than obj2
    */
   TEST_SETUP()
   {
      idx1 = 1;
      idx2 = 2;
      obj_1.id = idx1;
      obj_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_1.time_since_initialization = 3.0F;
      obj_1.vcs_position.x = 5.0F;
      obj_1.vcs_position.y = -3.0F;
      obj_1.speed = 3.1F;

      obj_2.id = idx2;
      obj_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj_2.time_since_initialization = 2.5F;
      obj_2.vcs_position.x = 8.0F;
      obj_2.vcs_position.y = -3.0F;
      obj_2.speed = 3.2F;

      globals.average_sensor_position_x = 0.0F;
      globals.average_sensor_position_y = 0.0F;
   }

};

/** \purpose  
 * Check that when both objects are old enough and CTCA, the idx of the closest one is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CTCA)
{
   /** \precond
    * All object properites set in test setup section
    */
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose
 * Check that object 2 is retained when object 1 is closer to VCS origin but object 2 is closer to host center.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Keep_Obj2_When_Closer_To_Host_Center_But_Farther_From_VCS_Origin)
{
   /** \precond
    * All object properites set in test setup section.
    * Set host-center offset and object positions such that:
    * - obj1 is closer to VCS origin
    * - obj2 is closer to host center
    */
   obj_1.vcs_position.x = 0.0F;
   obj_1.vcs_position.y = -2.5F;
   obj_2.vcs_position.x = -2.5F;
   obj_2.vcs_position.y = -2.5F;
   globals.average_sensor_position_x = -4.0F;
   globals.average_sensor_position_y = 0.0F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function.
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that object closer to host center is retained.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough, CTCA and slow moving, the idx of the closest one is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CTCA_Slow_Moving)
{
   /** \precond
    * All object properites set in test setup section
    * Set both object's speed below slow moving threshold
    */
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough, CTCA, slow moving and close enough, 
 * the idx of the one with a larger length is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Larger_Size_When_Both_Are_Closest_former_larger)
{
   /** \precond
    * All object properites set in test setup section
    * Set both object's speed below slow moving threshold
    */
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   obj_1.vcs_position.x = 5.0F;
   obj_1.vcs_position.y = -3.0F;
   obj_2.vcs_position.x = 5.01F;
   obj_2.vcs_position.y = -3.0F;
   obj_1.bbox.Set_Length(1.0F);
   obj_2.bbox.Set_Length(5.0F);
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough, CTCA, slow moving and close enough, 
 * the idx of the one with a larger length is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Larger_Size_When_Both_Are_Closest_latter_larger)
{
   /** \precond
    * All object properites set in test setup section
    * Set both object's speed below slow moving threshold
    */
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   obj_1.vcs_position.x = 5.0F;
   obj_1.vcs_position.y = -3.0F;
   obj_2.vcs_position.x = 5.01F;
   obj_2.vcs_position.y = -3.0F;
   obj_1.bbox.Set_Length(5.0F);
   obj_2.bbox.Set_Length(1.0F);
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough and CTCA, the idx of the closest one is selected as the keep idx. Object 1 is moved farther away.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CTCA_Obj1_Far)
{
   /** \precond
    * All object properites set in test setup section
    * Move obj1 farther away than obj2
    * Set expected keep idx to idx2 since it's closer.
    */
   obj_1.vcs_position.y = 10.0F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough, CTCA and slow moving, the idx of the closest one is selected as the keep idx. Object 1 is moved farther away.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CTCA_Slow_Moving_Obj1_Far)
{
   /** \precond
    * All object properites set in test setup section
    * Move obj1 farther away than obj2
    * Set both object's speed below slow moving threshold
    * Set expected keep idx to idx2 since it's closer.
    */
   obj_1.vcs_position.y = 10.0F;
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough and CCA, the idx of the closest one is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CCA)
{
   /** \precond
    * All object properites set in test setup section
    * Set filter type to CCA for both objects
    */
   obj_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough, CCA and slow moving, the idx of the closest one is selected as the keep idx.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Closest_When_Both_CCA_Slow_Moving)
{
   /** \precond
    * All object properites set in test setup section
    * Set both object's speed below slow moving threshold
    * Set filter type to CCA for both objects
    */
   obj_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough and slow moving and closest object is CCA while the other is CTCA, the idx of the object farthest away is selected.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Far_Obj_When_Closest_CCA)
{
   /** \precond
    * All object properites set in test setup section
    * Set filter type to CCA for the closest object
    * Set speed of both objects below slow moving threshold
    * Set expected keep id to idx2 since it's CTCA
    */
   obj_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are old enough and slow moving and obj1 is farther away and CTCA while obj2 is closer and CCA, the CTCA object is selected.
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Select_Far_Obj_When_Closest_CCA_Flipped_Idx)
{
   /** \precond
    * All object properites set in test setup section
    * Change placement of obj1 and obj2
    * Set filter type to CCA for the closest object
    * Set expected keep id to idx2 since it's CTCA
    */
   obj_1.vcs_position.y = 8.0F;
   obj_2.vcs_position.y = 5.0F;
   obj_1.speed = 2.9F;
   obj_2.speed = 2.85F;
   obj_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are newly created, the oldest is selected (Here object 1).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Both_New_Obj1_Older)
{
   /** \precond
    * All object properites set in test setup section
    * Set time_since_initialization below 2 seconds for both objects
    * Set time_since_initialization larger for object 1
    */
   obj_1.time_since_initialization = 1.5F;
   obj_2.time_since_initialization = 1.0F;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when both objects are newly created, the oldest is selected (Here object 2).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_Both_New_Obj2_Older)
{
   /** \precond
    * All object properites set in test setup section
    * Set time_since_initialization below 2 seconds for both objects
    * Set time_since_initialization larger for object 1
    */
   obj_1.time_since_initialization = 1.5F;
   obj_2.time_since_initialization = 1.9F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when one objects is newly created and the other not, the oldest is selected (Here object 2).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_One_New_Obj2_Older)
{
   /** \precond
    * All object properites set in test setup section
    * Set time_since_initialization below 2 seconds for both objects
    * Set time_since_initialization larger for object 1
    */
   obj_1.time_since_initialization = 1.0F;
   obj_2.time_since_initialization = 2.9F;
   const int32_t exp_keep_idx = idx2;
   const int32_t exp_kill_idx = idx1;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** \purpose  
 * Check that when one objects is newly created and the other not, the oldest is selected (Here object 1).
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_Choose_Obj_Idx_To_Be_Kept, Choose_Obj_Idx_To_Be_Kept_One_New_Obj1_Older)
{
   /** \precond
    * All object properites set in test setup section
    * Set time_since_initialization below 2 seconds for both objects
    * Set time_since_initialization larger for object 1
    */
   obj_1.time_since_initialization = 2.1F;
   obj_2.time_since_initialization = 1.9F;
   const int32_t exp_keep_idx = idx1;
   const int32_t exp_kill_idx = idx2;

   /** \action
    * Run tested function
    */
   Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, obj_1, obj_2, keep_idx, kill_idx);

   /** \result
    * Check that the correct index is selected.
    */
   CHECK_EQUAL(exp_keep_idx, keep_idx);
   CHECK_EQUAL(exp_kill_idx, kill_idx);
}

/** @}*/

/** \defgroup  f360_try_to_merge_two_objects_into_host_path
 *  @{
 */

 /** \brief
  * Test group designed for testing Try_To_Merge_Two_Objects function when the marge happen into or very close to host path
  */
TEST_GROUP(f360_try_to_merge_two_objects_into_host_path)
{
    F360_Host_T host = {};
    F360_Globals_T globals = {};
    F360_Calibrations_T calib = {};
    F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
    F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
    F360_Tracker_Info_T tracker_info = {};
    F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
    rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
    int32_t kill_idx;
    int32_t idx1;
    int32_t idx2;
    Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};

    /** \setup
     * Set up object list with two active objects, that should be merged
     */
    TEST_SETUP()
    {
        // Set up tracker calibrations
        Initialize_Tracker_Calibrations(calib);
        host.dist_rear_axle_to_vcs_m = 3.0F;

        globals.f_single_front_center_radar_only = false;

        // Set up properties of the first object
        idx1 = 10;
        object_tracks[idx1].id = idx1;
        object_tracks[idx1].vcs_position.x = 20.0F;
        object_tracks[idx1].vcs_position.y = 3.0F;
        object_tracks[idx1].speed = 10.0F;
        object_tracks[idx1].vcs_heading = Angle{ -0.4F };
        object_tracks[idx1].movable_prob = 1.0F;
        object_tracks[idx1].f_moving = true;
        object_tracks[idx1].time_since_initialization = 11.0F;
        object_tracks[idx1].bbox.Set_Orientation(object_tracks[idx1].vcs_heading);
        object_tracks[idx1].Update_Bbox_Size(1.5F, 2.0F);
        object_tracks[idx1].hdg_ptng_disagmt = 0.0F;
        object_tracks[idx1].ndets = 1U;
        object_tracks[idx1].detids[0] = 1U;

        detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.x = object_tracks[idx1].vcs_position.x;
        detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
        raw_detection_list.detections[object_tracks[idx1].detids[0] - 1U].raw.sensor_id = 1U;

        tracker_info.active_obj_ids[tracker_info.num_active_objs] = idx1 + 1;
        tracker_info.num_active_objs++;

        // Set up properties of the second object
        idx2 = 20;
        object_tracks[idx2].id = idx2;
        object_tracks[idx2].vcs_position.x = 20.5F;
        object_tracks[idx2].vcs_position.y = 3.0F;
        object_tracks[idx2].speed = 10.1F;
        object_tracks[idx2].vcs_heading = Angle{ -0.4F };
        object_tracks[idx2].movable_prob = 1.0F;
        object_tracks[idx2].f_moving = true;
        object_tracks[idx2].time_since_initialization = 15.0F;
        object_tracks[idx2].bbox.Set_Orientation(object_tracks[idx2].vcs_heading);
        object_tracks[idx2].Update_Bbox_Size(2.3F, 2.0F);
        object_tracks[idx2].hdg_ptng_disagmt = 0.0F;
        object_tracks[idx2].ndets = 1U;
        object_tracks[idx2].detids[0] = 2U;

        detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.x = object_tracks[idx2].vcs_position.x;
        detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
        raw_detection_list.detections[object_tracks[idx2].detids[0] - 1U].raw.sensor_id = 1U;

        tracker_info.active_obj_ids[tracker_info.num_active_objs] = idx2 + 1;
        tracker_info.num_active_objs++;
        Set_Tracker_Variant(tracker_info.variant);
    }
};

/** \purpose
 * Check if objects are not merged since none of them is close within 1.8m 0 in y axis
 * but the (future) merged object it is. The objects are on the right side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Not_Merge_Into_Host_Path_Right_Side)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after no merge operation
     * Modify the objects' positions so their closest point is further than 1.8m and
     * the closest point of the potentially merged object out of them is less than 1.8m
     * Adjust detection positions to match modified object positions
     */
    const int32_t expected_num_active_objs = 2;
    object_tracks[idx2].vcs_position.y = 3.2F;
    object_tracks[idx2].Update_Bbox_Size(object_tracks[idx2].bbox.Get_Length(), object_tracks[idx2].bbox.Get_Width());
    object_tracks[idx1].vcs_position.y = 3.2F;
    object_tracks[idx1].Update_Bbox_Size(object_tracks[idx1].bbox.Get_Length(), object_tracks[idx1].bbox.Get_Width());
    detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
    detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;


    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a non merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since the front object out of 2 is close within 1.8m 0 in y axis
 * and the (future) merged object it is. The objects are on the right side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Right_Side_Front_Object_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the object with idx2 to make it close enough to host path (1.8m) in y axis
     */
    object_tracks[idx2].Update_Bbox_Size(2.5F, 2.0F);
    const int32_t expected_num_active_objs = 1;

    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since one of them is close within 1.8m 0 in y axis
 * and the (future) merged object it is. The objects are on the right side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Right_Side_Back_Object_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the object with idx1 to make it close enough to host path (1.8m) in y axis
     */
    object_tracks[idx1].Update_Bbox_Size(2.5F, 2.0F);
    const int32_t expected_num_active_objs = 1;

    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since both of them are close within 1.8m 0 in y axis
 * and the (future) merged object as well. The objects are on the right side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Right_Side_Both_Objects_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the both objects to make them close enough to host path (1.8m) in y axis
     */
    object_tracks[idx1].Update_Bbox_Size(2.5F, 2.0F);
    object_tracks[idx2].Update_Bbox_Size(2.5F, 2.0F);
    const int32_t expected_num_active_objs = 1;

    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are not merged since none of them is close within 1.8m 0 in y axis
 * but the (future) merged object it is. The objects are on the left side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Not_Merge_Into_Host_Path_Left_Side)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after no merge operation
     * Adjust detection positions to match modified object positions
     * Modify the objects' dimensions so their closest point is further than 1.8m and
     * the closest point of the potentially merged object out of them is less than 1.8m
     */
    const int32_t expected_num_active_objs = 2;
    object_tracks[idx1].vcs_position.y = -object_tracks[idx1].vcs_position.y - 0.2F;
    object_tracks[idx2].vcs_position.y = -object_tracks[idx2].vcs_position.y - 0.24F;
    detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
    detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
    object_tracks[idx2].Update_Bbox_Size(2.5F, 2.0F);
    object_tracks[idx1].Update_Bbox_Size(object_tracks[idx1].bbox.Get_Length() + 0.9F, object_tracks[idx1].bbox.Get_Width());


    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has remained the same which indicates a non merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since the front object out of 2 is close within 1.8m 0 in y axis
 * and the (future) merged object it is. The objects are on the left side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Left_Side_Front_Object_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the object with idx2 to make it close enough to host path (1.8m) in y axis
     * Set the y position of the objects to negative value to place them on the left side of host path
     * Update the detection positions to match the negated object positions
     * Modify the objects with id = idx2 dimensions so it can make the merge possible
     * Update both objects' center by update_bbox_size
     */
    object_tracks[idx1].vcs_position.y = -object_tracks[idx1].vcs_position.y;
    object_tracks[idx2].vcs_position.y = -object_tracks[idx2].vcs_position.y;
    detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
    detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
    object_tracks[idx2].Update_Bbox_Size(2.5F, 2.0F);
    object_tracks[idx1].Update_Bbox_Size(object_tracks[idx1].bbox.Get_Length(), object_tracks[idx1].bbox.Get_Width());

    const int32_t expected_num_active_objs = 1;


    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since one of them is close within 1.8m 0 in y axis
 * and the (future) merged object it is. The objects are on the left side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Left_Side_Back_Object_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the object with idx1 to make it close enough to host path (1.8m) in y axis
     * Set the y position of the objects to negative value to place them on the left side of host path
     * Update the detection positions to match the negated object positions
     */
    object_tracks[idx1].vcs_position.y = -object_tracks[idx1].vcs_position.y;
    object_tracks[idx2].vcs_position.y = -object_tracks[idx2].vcs_position.y;
    detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
    detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
    object_tracks[idx1].Update_Bbox_Size(2.5F, 2.0F);
    object_tracks[idx2].Update_Bbox_Size(object_tracks[idx1].bbox.Get_Length(), object_tracks[idx1].bbox.Get_Width());

    const int32_t expected_num_active_objs = 1;

    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** \purpose
 * Check if objects are merged since both of them are close within 1.8m 0 in y axis
 * and the (future) merged object as well. The objects are on the left side of host path
 * and on going
 * \req
 * NA
 */
TEST(f360_try_to_merge_two_objects_into_host_path, Objects_Should_Merge_Into_Host_Path_Left_Side_Both_Objects_Into_Host_Path)
{
    /** \precond
     * All object properites set in test setup section
     * Set expected number of objects after merge operation
     * Update the length of the both objects to make them close enough to host path (1.8m) in y axis
     * Set the y position of the objects to negative value to place them on the left side of host path
     * Update the detection positions to match the negated object positions
     */
    object_tracks[idx1].vcs_position.y = -object_tracks[idx1].vcs_position.y;
    object_tracks[idx2].vcs_position.y = -object_tracks[idx2].vcs_position.y;
    detection_props[object_tracks[idx1].detids[0] - 1U].vcs_position.y = object_tracks[idx1].vcs_position.y;
    detection_props[object_tracks[idx2].detids[0] - 1U].vcs_position.y = object_tracks[idx2].vcs_position.y;
    object_tracks[idx1].Update_Bbox_Size(2.5F, 2.0F);
    object_tracks[idx2].Update_Bbox_Size(2.5F, 2.0F);
    const int32_t expected_num_active_objs = 1;


    /** \action
     * Run tested function
     */
    Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, sep, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);

    /** \result
     * Check if overall number of objects has decreased which indicates a merge operation
     */
    CHECK_EQUAL(expected_num_active_objs, tracker_info.num_active_objs);
}

/** @}*/

/** \defgroup  f360_any_detection_within_merged_extended_bbox
 *  @{
 */

/** \brief
 * Test group designed for testing Any_Detection_Within_Merged_Extended_BBox function
 */
TEST_GROUP(f360_any_detection_within_merged_extended_bbox)
{
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T keep_obj = {};
   F360_Object_Track_T kill_obj = {};
   BoundingBox merged_bbox = {};

   /** \setup
    * Set up detection ids and a merged bounding box
    */
   TEST_SETUP()
   {
      keep_obj.ndets = 1U;
      kill_obj.ndets = 1U;
      keep_obj.detids[0] = 1U;
      kill_obj.detids[0] = 2U;

      merged_bbox.Set_Center(Point(10.0F, 0.0F));
      merged_bbox.Set_Length(4.0F);
      merged_bbox.Set_Width(2.0F);
      merged_bbox.Set_Orientation(Angle{ 0.0F });

      detection_props[0].vcs_position.x = 20.0F;
      detection_props[0].vcs_position.y = 20.0F;
      detection_props[1].vcs_position.x = -20.0F;
      detection_props[1].vcs_position.y = -20.0F;
   }
};

/** \purpose
 * Check if function returns true when kept object detection is inside merged bbox
 * \req
 * NA
 */
TEST(f360_any_detection_within_merged_extended_bbox, Should_Return_True_When_Keep_Detection_Is_Inside)
{
   /** \precond
    * Move keep detection inside merged bbox
    */
   detection_props[0].vcs_position.x = 10.5F;
   detection_props[0].vcs_position.y = 0.5F;

   /** \action
    * Call tested function
    */
   const bool f_any_detection_inside = Any_Detection_Within_Merged_Extended_BBox(
      detection_props, kill_obj, keep_obj, merged_bbox);

   /** \result
    * Check if detection is confirmed inside merged bbox
    */
   CHECK_TRUE(f_any_detection_inside);
}

/** \purpose
 * Check if function returns true when killed object detection is inside merged bbox
 * \req
 * NA
 */
TEST(f360_any_detection_within_merged_extended_bbox, Should_Return_True_When_Kill_Detection_Is_Inside)
{
   /** \precond
    * Move keep detection outside and kill detection inside merged bbox
    */
   detection_props[1].vcs_position.x = 9.5F;
   detection_props[1].vcs_position.y = -0.5F;

   /** \action
    * Call tested function
    */
   const bool f_any_detection_inside = Any_Detection_Within_Merged_Extended_BBox(
      detection_props, kill_obj, keep_obj, merged_bbox);

   /** \result
    * Check if detection is confirmed inside merged bbox
    */
   CHECK_TRUE(f_any_detection_inside);
}

/** \purpose
 * Check if function returns false when no detections are inside merged bbox
 * \req
 * NA
 */
TEST(f360_any_detection_within_merged_extended_bbox, Should_Return_False_When_No_Detection_Is_Inside)
{
   /** \action
    * Call tested function with both detections outside merged bbox
    */
   const bool f_any_detection_inside = Any_Detection_Within_Merged_Extended_BBox(
      detection_props, kill_obj, keep_obj, merged_bbox);

   /** \result
    * Check if no detection is confirmed inside merged bbox
    */
   CHECK_FALSE(f_any_detection_inside);
}

/** @}*/

/** \defgroup  f360_calculate_merged_obj_center_and_update_width
 *  @{
 */

 /** \brief
  * Test group designed for testing Calculate_Merged_Obj_Center_And_Update_Width function to check that
  * the position and the width of the potential merged object are updated correctly
  */
TEST_GROUP(f360_calculate_merged_obj_center_and_update_width)
{
    F360_Calibrations_T calib = {};
    F360_Object_Track_T obj;
    F360_Dimensions_T dimensions = {};
    float32_t tolerance;

    /** \setup
     * Set up object list with two active objects, that should be merged
     */
    TEST_SETUP()
    {
      // Set up tracker calibrations
      Initialize_Tracker_Calibrations(calib);

      // Set up dimensions 
      dimensions.len1 = 1.0F;
      dimensions.len2 = 2.0F;
      dimensions.wid1 = 1.0F;
      dimensions.wid2 = 2.0F;
      dimensions.length = 3.0F;
      dimensions.width = 3.0F;

      // Set up the object
      obj.vcs_position.x = 5.0F;
      obj.vcs_position.y = 5.0F;
      obj.reference_point = F360_REFERENCE_POINT_LEFT;

      obj.bbox.Set_Orientation(Angle{ 0.0F });
      obj.bbox.Set_Center(Point(obj.vcs_position.x + dimensions.len1 * 0.5F, obj.vcs_position.y + dimensions.wid1 * 0.5F ));
      obj.bbox.Set_Width(1.0F);
      obj.reference_point = F360_REFERENCE_POINT_REAR;

      // Set tolerance for floating point comparisons
      tolerance = 0.001F;

    }
};

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is rear referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Rear_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is front referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Front_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to front
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is front left referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Front_Left_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to front left
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is front right referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Front_Right_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to front right
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is rear left referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Rear_Left_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to rear left
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs is modified correctly and if the width remains the same when the keep object's width is used.
* Given that the object is rear right referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Rear_Right_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to rear right
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y;
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, dimensions.width, tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs  and width are modified correctly.
* Given that the object is left referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Left_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to left
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y + 0.5F * std::abs(dimensions.wid2 - dimensions.wid1);
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, obj.bbox.Get_Width(), tolerance);
}

/**
*\purpose  
* Check if kept object's center in vcs and width are modified correctly.
* Given that the object is right referenced and oriented with 0 degree heading.
*\req  N/A
*/
TEST(f360_calculate_merged_obj_center_and_update_width, Should_Set_New_Object_VCS_Center_Use_Keep_Objs_Width_Right_Point_Reference)
{
   /** \precond
    * All object properites set in test setup section
    * Set expected values for center and width
    * Set reference point to right
    **/
   const float expected_long_position = obj.bbox.Get_Center().x + 0.5F * std::abs(dimensions.len2 - dimensions.len1);
   const float expected_lat_position = obj.bbox.Get_Center().y + 0.5F * std::abs(dimensions.wid2 - dimensions.wid1);
   const float expected_width = obj.bbox.Get_Width();
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Call tested function
   **/
   Point obj_vcs_center = Calculate_Merged_Obj_Center_And_Update_Width(obj, dimensions);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, obj_vcs_center.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, obj_vcs_center.y, tolerance);
   DOUBLES_EQUAL(expected_width, obj.bbox.Get_Width(), tolerance);
}

/** @}*/

/** \defgroup  F360_Get_Closest_Corner_VCS_Y 
 *  @{
 */

/** \brief
 * Test group designed for testing Get_Closest_Corner_VCS_Y function
 */
TEST_GROUP(f360_Get_Closest_Corner_VCS_Y)
{
   BoundingBox bbox = {};
   Point obj_center = {};
   float32_t bbox_length;
   float32_t bbox_width;
   Angle bbox_orientation;
   float32_t tolerance;
   float32_t expected_closest_y_abs_vcs;

   /** \setup
    * Nothing to set up
    */
   TEST_SETUP()
   {
      tolerance = 1e-4F;
      bbox_length = 1.0F;
      bbox_width = 1.0F;
      bbox.Set_Width(bbox_width);
      bbox.Set_Length(bbox_length);
      expected_closest_y_abs_vcs = 4.452581F;
   }
};

/**
*\purpose  
* Check if the distance to the closest corner in y axis is calculated correctly when the object is on going
* in front and right of the host. The closest edge of the object is the left rear corner since the orientation is 0.1 rad.
*\req  N/A
*/
TEST(f360_Get_Closest_Corner_VCS_Y, f360_Get_Closest_Corner_VCS_Y_Rear_Left_Corner_Closest)
{
   /** \precond
    * Set center of the object to (5,5)
    * Set length to 2m, width to 1m and orientation to 0.1 rad
    * fill the bounding box of the object with the above values
    **/
   obj_center.x = 5.0F;
   obj_center.y = 5.0F;
   bbox_orientation = Angle{ 0.1F };
   bbox.Set_Center(obj_center);
   bbox.Set_Orientation(bbox_orientation);
  
   /** \action
   * Call tested function
   **/
   float32_t closest_y_abs_vcs = Get_Closest_Corner_VCS_Y(bbox);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_closest_y_abs_vcs, closest_y_abs_vcs, tolerance);
}

/**
*\purpose  
* Check if the distance to the closest corner in y axis is calculated correctly when the object is on going
* in back and right of the host. The closest edge of the object is the front left corner since the orientation is -0.1 rad.
*\req  N/A
*/
TEST(f360_Get_Closest_Corner_VCS_Y, f360_Get_Closest_Corner_VCS_Y_Front_Left_Corner_Closest)
{
   /** \precond
    * Set center of the object to (5,5)
    * Set length to 2m, width to 1m and orientation to -0.1 rad
    * fill the bounding box of the object with the above values
    **/
   obj_center.x = -5.0F;
   obj_center.y = 5.0F;
   bbox_orientation = Angle{ -0.1F };
   bbox.Set_Center(obj_center);;
   bbox.Set_Orientation(bbox_orientation);

   /** \action
   * Call tested function
   **/
   float32_t closest_y_abs_vcs = Get_Closest_Corner_VCS_Y(bbox);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_closest_y_abs_vcs, closest_y_abs_vcs, tolerance);
}

/**
*\purpose  
* Check if the distance to the closest corner in y axis is calculated correctly when the object is on going
* in front and left of the host. The closest edge of the object is the rear right corner since the orientation is -0.1 rad.
*\req  N/A
*/
TEST(f360_Get_Closest_Corner_VCS_Y, f360_Get_Closest_Corner_VCS_Y_Rear_Right_Corner_Closest)
{
   /** \precond
    * Set center of the object to (5,5)
    * Set length to 2m, width to 1m and orientation to -0.1 rad
    * fill the bounding box of the object with the above values
    **/
   obj_center.x = 5.0F;
   obj_center.y = -5.0F;
   bbox_orientation = Angle{ -0.1F };
   bbox.Set_Center(obj_center);
   bbox.Set_Orientation(bbox_orientation);
   
   /** \action
   * Call tested function
   **/
   float32_t closest_y_abs_vcs = Get_Closest_Corner_VCS_Y(bbox);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(std::abs(expected_closest_y_abs_vcs), closest_y_abs_vcs, tolerance);
}

/**
*\purpose  
* Check if the distance to the closest corner in y axis is calculated correctly when the object is on going
* in rear and left of the host. The closest edge of the object is the front right corner since the orientation is 0.1 rad.
*\req  N/A
*/
TEST(f360_Get_Closest_Corner_VCS_Y, f360_Get_Closest_Corner_VCS_Y_Front_Right_Corner_Closest)
{
   /** \precond
    * Set center of the object to (5,5)
    * Set length to 2m, width to 1m and orientation to 0.1 rad
    * fill the bounding box of the object with the above values
    **/
   obj_center.x = -5.0F;
   obj_center.y = -5.0F;
   bbox_orientation = Angle{ 0.1F };
   bbox.Set_Center(obj_center);
   bbox.Set_Orientation(bbox_orientation);

   /** \action
   * Call tested function
   **/
   float32_t closest_y_abs_vcs = Get_Closest_Corner_VCS_Y(bbox);

   /** \result
   * Check if center and width have been set correctly
   **/
   DOUBLES_EQUAL(expected_closest_y_abs_vcs, closest_y_abs_vcs, tolerance);
}
/** @}*/

/** \defgroup  F360_Check_If_Detection_Gaps_Support_Merge 
 *  @{
 */

/** \brief
 * Test group designed for testing Check_If_Detection_Gaps_Support_Merge function
 */
TEST_GROUP(f360_Check_If_Detection_Gaps_Support_Merge)
{
   F360_Object_Track_T obj1 = {};
   F360_Object_Track_T obj2 = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up two objects with three detections each
    */
   TEST_SETUP()
   {
      // obj1: 1 detection, bbox at origin with zero orientation
      obj1.ndets = 3U;
      obj1.detids[0] = 1U;
      obj1.detids[1] = 2U;
      obj1.detids[2] = 3U;
      obj1.bbox.Set_Center(Point(5.0F, 5.0F));
      obj1.bbox.Set_Orientation(Angle{ 0.0F });

      // obj2: 1 detection
      obj2.ndets = 3U;
      obj2.detids[0] = 4U;
      obj2.detids[1] = 5U;
      obj2.detids[2] = 6U;
      obj2.bbox.Set_Center(Point(4.0F, 5.0F));
      obj2.bbox.Set_Orientation(Angle{ 0.0F });

      // Detections placed such that the total spread is 2m in x and 1.2m in y.
      // Biggest gap is 0.4m in x and 0.3m in y
      detection_props[0].vcs_position.x = 5.0F;
      detection_props[0].vcs_position.y = 4.8F;

      detection_props[1].vcs_position.x = 5.5F;
      detection_props[1].vcs_position.y = 5.0F;

      detection_props[2].vcs_position.x = 5.0F;
      detection_props[2].vcs_position.y = 5.2F;

      detection_props[3].vcs_position.x = 4.6F;
      detection_props[3].vcs_position.y = 4.0F;

      detection_props[4].vcs_position.x = 3.5F;
      detection_props[4].vcs_position.y = 4.0F;

      detection_props[5].vcs_position.x = 4.0F;
      detection_props[5].vcs_position.y = 4.5F;
   }
};

/** \purpose
 * Check that function returns true when the detections of the two merge candidates are close enough in both longitudinal and orthogonal directions,
 * with gap ratios below the defined thresholds.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, True_When_Object_Gaps_Below_Thresholds)
{
   /** \precond
    * Use default setup
    */

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Overlapping clusters produce zero gap ratios - merge should be supported
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that function returns true when there is no detection gap in the longitudinal direction
 * (clusters fully overlap in x axis) and the orthogonal gap is below threshold.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, True_When_Both_Object_Detections_Longitudinal_Overlap)
{
   /** \precond
    * Move one detection of the rear object to be infront of the other object's detections, creating an overlap in the longitudinal direction (x axis).
    */
   detection_props[3].vcs_position.x = 5.1F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Overlapping clusters produce zero gap ratios - merge should be supported
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that function returns true when the detection clusters overlap in the orthogonal (lateral/y) direction.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, True_When_Both_Object_Detections_Lateral_Overlap)
{
   /** \precond
    * Move one detection of the second object to be at the same y as the first object's detections, creating overlap in the y direction.
    */
   detection_props[3].vcs_position.y = detection_props[0].vcs_position.y;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Overlapping clusters in y produce zero gap ratios - merge should be supported
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that function returns false when the longitudinal gap between detection clusters exceeds the threshold.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, False_When_Longitudinal_Gap_Exceeds_Threshold)
{
   /** \precond
    * Move obj2's detections far enough in x so gap ratio exceeds threshold.
    */
   detection_props[3].vcs_position.x = 4.4F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio exceeds threshold, should return false.
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check that function returns false when the orthogonal gap between detection clusters exceeds the threshold.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, False_When_Lateral_Gap_Exceeds_Threshold)
{
   /** \precond
    * Move obj2's detections far enough in y so gap ratio exceeds threshold.
    */
   detection_props[5].vcs_position.y = 4.1F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio exceeds threshold, should return false.
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check that function returns false when the longitudinal gap ratio is exactly at the threshold.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, False_When_Longitudinal_Gap_At_Threshold)
{
   /** \precond
    * Move obj2's detections so gap ratio is exactly at threshold.
    */
   detection_props[3].vcs_position.x = 4.5F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio at threshold, should return false.
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check that function returns false when the lateral gap ratio is exactly at the threshold.
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, False_When_Lateral_Gap_At_Threshold)
{
   /** \precond
    * Move obj2's detections so lateral gap ratio is exactly at threshold.
    */
   detection_props[5].vcs_position.y = 4.2F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio at threshold, should return false.
    */
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check that function returns true when the longitudinal spread of detections is less than 1m (gap ratio = 0).
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, True_When_Longitudinal_Gap_Less_Than_1m)
{
   /** \precond
    * Move obj2's detection so longitudinal spread is <1m.
    */
   detection_props[4].vcs_position.x = 4.51F;
   detection_props[5].vcs_position.x = 4.51F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio is 0, should return true.
    */
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that function returns true when the lateral spread of detection is less than 1m (gap ratio = 0).
 * \req
 * NA
 */
TEST(f360_Check_If_Detection_Gaps_Support_Merge, True_When_Lateral_Spread_Less_Than_1m)
{
   /** \precond
    * Move obj2's detection so lateral spread is <1m.
    */
   detection_props[3].vcs_position.y = 4.21F;
   detection_props[4].vcs_position.y = 4.21F;

   /** \action
    * Call tested function
    */
   const bool f_result = Check_If_Detection_Gaps_Support_Merge(obj1, obj2, detection_props);

   /** \result
    * Gap ratio is 0, should return true.
    */
   CHECK_TRUE(f_result);
}

/** @}*/

