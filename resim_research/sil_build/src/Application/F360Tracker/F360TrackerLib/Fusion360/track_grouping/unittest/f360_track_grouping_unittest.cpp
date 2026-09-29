/** \file
 * This file contains unit tests for content of f360_track_grouping.cpp file
 */

#include "f360_track_grouping.h"
#include <CppUTest/TestHarness.h>

#include "f360_static_env_helpers.h"
#include "f360_set_variant.h"

using namespace f360_variant_A;

/** \defgroup  f360_track_grouping
 *  @{
 */

/** \brief
 *  Test group for unit testing Track_Grouping function.
 */
TEST_GROUP(f360_track_grouping_ut)
{
   //Initialize common variables used within all tests in this test group.
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Calibrations_T calib;
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
      
   /** \setup
    * Initialize calibrations, globals and common tracker_info and objects properties.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      for (uint32_t idx = 0U; idx < F360_NUM_OF_STATIC_ENV_POLYS; idx++)
      {
         Reset_Single_Static_Env_Poly(static_env_polys[idx]);
      }

      //globals.obj_mov_stat_spd_thresh = 4.0F;
      globals.f_single_front_center_radar_only = false;

      object_tracks[20].id = 21;
      object_tracks[20].vcs_position.x = 5.0F;
      object_tracks[20].vcs_position.y = 5.0F;
      object_tracks[20].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[20].Update_Bbox_Size(3.0F, 1.0F);
      object_tracks[20].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[20].Update_Bbox_Center();

      object_tracks[25].id = 26;
      object_tracks[25].vcs_position.x = 5.1F;
      object_tracks[25].vcs_position.y = 5.2F;
      object_tracks[25].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[25].Update_Bbox_Size(3.0F, 1.0F);
      object_tracks[25].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[25].Update_Bbox_Center();

      const int32_t active_obj_id_1 = 21;
      const int32_t active_obj_id_2 = 26;
      tracker_info.num_active_objs = 0;
      int32_t num_inactive_objs = 0;
      for (int32_t obj_id = 1; obj_id <= static_cast<int32_t>(tracker_info.variant.num_tracks); obj_id++)
      {
         if((obj_id == active_obj_id_1) || (obj_id == active_obj_id_2))
         {
            tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_id;
            tracker_info.num_active_objs++;
         }
         else
         {
            tracker_info.inactive_obj_ids[num_inactive_objs] = obj_id;
            num_inactive_objs++;
         }

      }
      tracker_info.active_obj_ids[0] = active_obj_id_1;
      tracker_info.active_obj_ids[1] = active_obj_id_2;
      tracker_info.vcslong_sorted_start = &(object_tracks[20]);
      tracker_info.vcslong_sorted_prev_track[active_obj_id_1 - 1] = NULL;
      tracker_info.vcslong_sorted_next_track[active_obj_id_1 - 1] = &(object_tracks[active_obj_id_2 -1]);
      tracker_info.vcslong_sorted_prev_track[active_obj_id_2 - 1] = &(object_tracks[active_obj_id_1 - 1]);
      tracker_info.vcslong_sorted_next_track[active_obj_id_2 - 1] = NULL;
   }
};

/** \purpose  
 *  Test checks whether for loop will be break in first iteration when vcslong sorted list
 *  is not filled properly.
 *
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Invalid_Vcslong_Sorted_List)
{
   /** \precond
    * Vcslong_sorted list starts from NULL pointer.
    * Several objects are active.
    */
   tracker_info.vcslong_sorted_start = NULL;
   tracker_info.num_active_objs = 2;

   int32_t exp_num_active_obj = tracker_info.num_active_objs;

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Check if object vcs_long_soretd list and number of active objects have not been changed.
    */
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_start);
   CHECK_EQUAL(exp_num_active_obj, tracker_info.num_active_objs);
}

/** \purpose
 *  Test checks whether two objects merge fails when first movable object has too small speed for merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Speed_Diff_Is_Below_Thr_For_Movable_Object)
{
   /** \precond
    * Define two active, movable objects and first object speed is below minimum speed threshold for object merge.
    */
   tracker_info.num_active_objs = 2;

   object_tracks[20].movable_prob = 1.0F;
   object_tracks[20].speed = globals.obj_mov_stat_spd_thresh - 1.0F;
   
   object_tracks[25].movable_prob = 1.0F;
   object_tracks[25].speed = globals.obj_mov_stat_spd_thresh + 1.0F;
   
   int32_t exp_num_active_obj = tracker_info.num_active_objs;

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Check if object vcs_long_soretd list and number of active objects have not been changed.
    */
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[20]);
   POINTERS_EQUAL((&object_tracks[25]), tracker_info.vcslong_sorted_next_track[20]);
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_prev_track[25]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[25]);
   CHECK_EQUAL(exp_num_active_obj, tracker_info.num_active_objs);
}

/** \purpose
 *  Test checks whether two non movable objects does not merge when their are to far away from each other.
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Too_Big_Long_Distance_Between_Non_Movable_Objects)
{
   /** \precond
    * Define two active not movable objects, their longitudinal distance is above maximal threshold for merging two objects.
    */
   tracker_info.num_active_objs = 2;

   int32_t exp_num_active_obj = tracker_info.num_active_objs;

   object_tracks[20].movable_prob = 0.0F;
   object_tracks[20].speed = 1.0F;

   object_tracks[25].movable_prob = 0.0F;
   object_tracks[25].speed = 1.0F;
   object_tracks[25].vcs_position.x = object_tracks[20].vcs_position.x + (4.0F * object_tracks[20].bbox.Get_Length());
   object_tracks[25].Update_Bbox_Center();

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Check if object vcs_long_soretd list and number of active objects have not been changed.
    */
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[20]);
   POINTERS_EQUAL((&object_tracks[25]), tracker_info.vcslong_sorted_next_track[20]);
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_prev_track[25]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[25]);
   CHECK_EQUAL(exp_num_active_obj, tracker_info.num_active_objs);
}


/** \purpose
 *  Test checks whether two movable objects does not merge when their are to far away from each other.
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Movable_Objects_Should_Not_Be_Merged_Too_Far_Away)
{
   /** \precond
    * Define two active movable objects, their longitudinal distance is above maximal threshold for merging two objects.
    */
   tracker_info.num_active_objs = 2;

   int32_t exp_num_active_obj = tracker_info.num_active_objs;

   object_tracks[20].movable_prob = 1.0F;
   object_tracks[20].f_moving = true;
   object_tracks[20].speed = globals.obj_mov_stat_spd_thresh + 1.0F;

   object_tracks[25].movable_prob = 1.0F;
   object_tracks[25].f_moving = true;
   object_tracks[25].speed = globals.obj_mov_stat_spd_thresh + 1.0F;
   object_tracks[25].vcs_position.x = object_tracks[20].vcs_position.x + 2.0F * object_tracks[20].bbox.Get_Length() + 0.01F;
   object_tracks[25].Update_Bbox_Center();

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Check if object vcs_long_soretd list and number of active objects have been properly updated.
    */
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[20]);
   POINTERS_EQUAL((&object_tracks[25]), tracker_info.vcslong_sorted_next_track[20]);
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_prev_track[25]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[25]);
   CHECK_EQUAL(exp_num_active_obj, tracker_info.num_active_objs);
}

/** \purpose
 *  Test checks that two objects are not merged when one is not moving.
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Movable_Objects_Should_Not_Be_Merged_One_Not_Moving)
{
   /** \precond
    * Define two active movable objects, their longitudinal distance is below maximal threshold for merging two objects.
    * Set first object to moving and the second to not moving
    */
   tracker_info.num_active_objs = 2;

   int32_t exp_num_active_obj = tracker_info.num_active_objs;

   object_tracks[20].movable_prob = 1.0F;
   object_tracks[20].f_moving = true;
   object_tracks[20].speed = globals.obj_mov_stat_spd_thresh + 1.0F;

   object_tracks[25].movable_prob = 0.0F;
   object_tracks[25].f_moving = false;
   object_tracks[25].speed = globals.obj_mov_stat_spd_thresh + 1.0F;

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Check if object vcs_long_soretd list and number of active objects have been properly updated.
    */
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[20]);
   POINTERS_EQUAL((&object_tracks[25]), tracker_info.vcslong_sorted_next_track[20]);
   POINTERS_EQUAL((&object_tracks[20]), tracker_info.vcslong_sorted_prev_track[25]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[25]);
   CHECK_EQUAL(exp_num_active_obj, tracker_info.num_active_objs);
}

/** \purpose
 *  Test checks that split logic is called as expected from
 *  top level function Track_Grouping()
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Track_Grouping_Verify_Split_Logic)
{
   /** \precond
    * Set tracker info to have 1 active object 
    * Initialize an object that should trigger a split.
    */
   tracker_info.num_active_objs = 1;
   tracker_info.inactive_obj_ids[tracker_info.variant.num_tracks  - 2] = tracker_info.active_obj_ids[1];
   tracker_info.active_obj_ids[1] = 0;

   object_tracks[20].speed = calib.k_orth_split_min_speed + 1.0F;
   object_tracks[20].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[20].orth_gap_filtered = calib.k_orth_split_min_orth_gap_for_split_high + 1.0F;
   object_tracks[20].orth_delta_filtered = object_tracks[20].orth_gap_filtered + 1.0F;

   /** \action
    *  call Track_Grouping().
    */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

   /** \result
    *  Verify that split happened by checking that the number of
    *  active objects have increased by 1
    */
   CHECK_EQUAL(2, tracker_info.num_active_objs);
}

/** \purpose
 *  Test checks if calibration values are correctly set up.
 * \req  NA.
 */
TEST(f360_track_grouping_ut, Verify_Track_Grouping_Calibration_Values)
{
   /** \precond
   * Set test tolerance.
   */
   const float32_t test_tolerance = 1e-5F;

   /** \result
    */
   DOUBLES_EQUAL(0.523599F, calib.k_track_grouping_hdg_gate, test_tolerance);
   DOUBLES_EQUAL(1.0F, calib.k_track_grouping_speed_gate, test_tolerance);
   DOUBLES_EQUAL(2.2F, calib.merging_m2m_distance_threshold, test_tolerance);
   DOUBLES_EQUAL(4.0F, calib.merging_lateral_det_spread_threshold, test_tolerance);
   DOUBLES_EQUAL(6.94444F, calib.merging_m2m_max_obj_speed, test_tolerance);
}

/** @}*/

/** \brief
 *  Test group for unit testing Track_Grouping function, specifically the vcs sorted list.
 */
TEST_GROUP(f360_track_grouping_ut_sorted_list)
{
   //Initialize common variables used within all tests in this test group.
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Calibrations_T calib;
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
      
   /** \setup
    * Initialize calibrations, globals and common tracker_info and objects properties. The object properties were defined in that
    * way to follow and represent each scenario of the presentation attached to the ticket DFD-2173.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      for (uint32_t idx = 0U; idx < F360_NUM_OF_STATIC_ENV_POLYS; idx++)
      {
         Reset_Single_Static_Env_Poly(static_env_polys[idx]);
      }

      globals.f_single_front_center_radar_only = false;

      tracker_info.num_active_objs = 7;
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         tracker_info.active_obj_ids[i] = i + 1;
      }
      
      // Object 1 
      object_tracks[0].id = 1;
      object_tracks[0].vcs_position.x = 1.0F;
      object_tracks[0].vcs_position.y = 5.0F;
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[0].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[0].Update_Bbox_Center();
      object_tracks[0].speed = 10.0F;
      object_tracks[0].hdg_ptng_disagmt = 0.0F;
      object_tracks[0].ndets = 1U;
      object_tracks[0].detids[0] = 1U;
      detection_props[object_tracks[0].detids[0]-1U].vcs_position.x = object_tracks[0].vcs_position.x;
      detection_props[object_tracks[0].detids[0]-1U].vcs_position.y = object_tracks[0].vcs_position.y;
      raw_detection_list.detections[object_tracks[0].detids[0]-1U].raw.sensor_id = 1U;
      
      // Object 2 
      object_tracks[1].id = 2;
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[1].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[1].vcs_position.x = 8.0F;
      object_tracks[1].vcs_position.y = 1.0F; 
      object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[1].Update_Bbox_Center();
      object_tracks[1].f_moving = true;
      object_tracks[1].curvature = 0.0F;
      object_tracks[1].speed = 10.0F;
      object_tracks[1].hdg_ptng_disagmt = 0.0F;
      object_tracks[1].ndets = 1U;
      object_tracks[1].detids[0] = 2U;
      detection_props[object_tracks[1].detids[0]-1U].vcs_position.x = object_tracks[1].vcs_position.x;
      detection_props[object_tracks[1].detids[0]-1U].vcs_position.y = object_tracks[1].vcs_position.y;
      raw_detection_list.detections[object_tracks[1].detids[0]-1U].raw.sensor_id = 1U;

      // Object 3 
      object_tracks[2].id = 3;
      object_tracks[2].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[2].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[2].vcs_position.x = 8.1F;
      object_tracks[2].vcs_position.y = 1.0F;
      object_tracks[2].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[2].Update_Bbox_Center();
      object_tracks[2].f_moving = true;
      object_tracks[2].speed = 10.0F;
      object_tracks[2].curvature = 0.0F;
      object_tracks[2].hdg_ptng_disagmt = 0.0F;
      object_tracks[2].ndets = 1U;
      object_tracks[2].detids[0] = 3U;
      detection_props[object_tracks[2].detids[0]-1U].vcs_position.x = object_tracks[2].vcs_position.x;
      detection_props[object_tracks[2].detids[0]-1U].vcs_position.y = object_tracks[2].vcs_position.y;
      raw_detection_list.detections[object_tracks[2].detids[0]-1U].raw.sensor_id = 1U;

      // Object 4 
      object_tracks[3].id = 4;
      object_tracks[3].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[3].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[3].vcs_position.x = 10.0F;
      object_tracks[3].vcs_position.y = 10.0F;
      object_tracks[3].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[3].Update_Bbox_Center();
      object_tracks[3].f_moving = true;
      object_tracks[3].speed = 10.0F;
      object_tracks[3].curvature = 0.0F;
      object_tracks[3].time_since_cluster_created = 1.7F;
      object_tracks[3].time_since_initialization = object_tracks[3].time_since_cluster_created + 0.1F;
      object_tracks[3].hdg_ptng_disagmt = 0.0F;
      object_tracks[3].ndets = 1U;
      object_tracks[3].detids[0] = 4U;
      detection_props[object_tracks[3].detids[0]-1U].vcs_position.x = object_tracks[3].vcs_position.x;
      detection_props[object_tracks[3].detids[0]-1U].vcs_position.y = object_tracks[3].vcs_position.y;
      raw_detection_list.detections[object_tracks[3].detids[0]-1U].raw.sensor_id = 1U;

      // Object 5 
      object_tracks[4].id = 5;
      object_tracks[4].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[4].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[4].vcs_position.x = 10.1F;
      object_tracks[4].vcs_position.y = 10.0F;
      object_tracks[4].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[4].Update_Bbox_Center();
      object_tracks[4].f_moving = true;
      object_tracks[4].speed = 10.0F;
      object_tracks[4].curvature = 0.0F;
      object_tracks[4].time_since_cluster_created = 1.6F;
      object_tracks[4].time_since_initialization = object_tracks[4].time_since_cluster_created + 0.1F;
      object_tracks[4].hdg_ptng_disagmt = 0.0F;
      object_tracks[4].ndets = 1U;
      object_tracks[4].detids[0] = 5U;
      detection_props[object_tracks[4].detids[0]-1U].vcs_position.x = object_tracks[4].vcs_position.x;
      detection_props[object_tracks[4].detids[0]-1U].vcs_position.y = object_tracks[4].vcs_position.y;
      raw_detection_list.detections[object_tracks[4].detids[0]-1U].raw.sensor_id = 1U;

      // Object 6
      object_tracks[5].id = 6;
      object_tracks[5].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[5].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[5].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[5].vcs_position.x = 100.0F;
      object_tracks[5].vcs_position.y = 100.0F;
      object_tracks[5].Update_Bbox_Center();
      object_tracks[5].f_moving = true;
      object_tracks[5].speed = 10.0F;
      object_tracks[5].curvature = 0.0F;
      object_tracks[5].time_since_cluster_created = 2.8F;
      object_tracks[5].time_since_initialization = object_tracks[5].time_since_cluster_created + 0.1F;
      object_tracks[5].hdg_ptng_disagmt = 0.0F;
      object_tracks[5].ndets = 1U;
      object_tracks[5].detids[0] = 6U;
      detection_props[object_tracks[5].detids[0]-1U].vcs_position.x = object_tracks[5].vcs_position.x;
      detection_props[object_tracks[5].detids[0]-1U].vcs_position.y = object_tracks[5].vcs_position.y;
      raw_detection_list.detections[object_tracks[5].detids[0]-1U].raw.sensor_id = 1U;

      // Object 7
      object_tracks[6].id = 7;
      object_tracks[6].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object_tracks[6].Update_Bbox_Size(1.0F, 1.0F);
      object_tracks[6].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[6].vcs_position.x = 800.0F;
      object_tracks[6].vcs_position.y = 100.0F;
      object_tracks[6].Update_Bbox_Center();
      object_tracks[6].f_moving = true;
      object_tracks[6].speed = 10.0F;
      object_tracks[6].curvature = 0.0F;
      object_tracks[6].time_since_cluster_created = 2.1F;
      object_tracks[6].time_since_initialization = object_tracks[6].time_since_cluster_created + 0.1F;
      object_tracks[6].hdg_ptng_disagmt = 0.0F;
      object_tracks[6].ndets = 1U;
      object_tracks[6].detids[0] = 7U;
      detection_props[object_tracks[6].detids[0]-1U].vcs_position.x = object_tracks[6].vcs_position.x;
      detection_props[object_tracks[6].detids[0]-1U].vcs_position.y = object_tracks[6].vcs_position.y;
      raw_detection_list.detections[object_tracks[6].detids[0]-1U].raw.sensor_id = 1U;

      tracker_info.vcslong_sorted_start = &(object_tracks[0]);
      tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
      tracker_info.vcslong_sorted_next_track[1] = &object_tracks[2];
      tracker_info.vcslong_sorted_next_track[2] = &object_tracks[3];
      tracker_info.vcslong_sorted_next_track[3] = &object_tracks[4];
      tracker_info.vcslong_sorted_next_track[4] = &object_tracks[5];
      tracker_info.vcslong_sorted_next_track[5] = &object_tracks[6];
      tracker_info.vcslong_sorted_next_track[6] = NULL;

      tracker_info.vcslong_sorted_prev_track[0] = NULL;
      tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
      tracker_info.vcslong_sorted_prev_track[2] = &object_tracks[1];
      tracker_info.vcslong_sorted_prev_track[3] = &object_tracks[2];
      tracker_info.vcslong_sorted_prev_track[4] = &object_tracks[3];
      tracker_info.vcslong_sorted_prev_track[5] = &object_tracks[4];
      tracker_info.vcslong_sorted_prev_track[6] = &object_tracks[5];

   }
};

/** \purpose
 *  Test that killing the first object of the first merge doesn't lead to breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_First_Object_Of_The_First_Merge)
{
   /** \precond
   * Set positions and time since they were created for all objects in order to create the first wanted scenario.
   */
   // Object 2 - Merge with object 3 - Kill this one
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

   // Object 3 - Merge with object 2 - Keep this one
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is correct and that the expected object IDs are kept
    */
   CHECK_EQUAL(5 ,tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(3, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
}

/** \purpose
 *  Test that killing the second object of the first merge doesn't lead to breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_Second_Object_Of_The_First_Merge)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the second wanted scenario.
    */
    // Object 2 - Merge with object 3 - Keep this one
    object_tracks[1].time_since_cluster_created = 1.9F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;


    // Object 3 - Merge with object 2 - Kill this one
    object_tracks[2].time_since_cluster_created = 1.8F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept
     */
   CHECK_EQUAL(5 ,tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(2, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
}


/** \purpose
 *  Test that killing the first object of the first merge, while they are not consecutive in the list 
    of sorted objects and there is a second merge happening, doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_First_Object_Of_The_First_Merge_While_They_Are_Not_Consecutive_In_The_List)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the third wanted scenario.
    */
    // Object 2 - Merge with object 3 - Kill this one
    object_tracks[1].time_since_cluster_created = 1.6F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Keep this one
    object_tracks[2].time_since_cluster_created = 1.8F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

    // Object 4 - Merges with object 5 - Keep this one
    object_tracks[3].vcs_position.x = 8.05F;
    object_tracks[3].vcs_position.y = 10.0F;
    object_tracks[3].Update_Bbox_Center();
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.x = object_tracks[3].vcs_position.x;
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.y = object_tracks[3].vcs_position.y;

    // Object 5 - Merges with object 4 - Kill this one
    object_tracks[4].vcs_position.x = 8.2F; 
    object_tracks[4].vcs_position.y = 10.0F;
    object_tracks[4].Update_Bbox_Center();
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.x = object_tracks[4].vcs_position.x;
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.y = object_tracks[4].vcs_position.y;

    tracker_info.vcslong_sorted_start = &(object_tracks[0]);
    tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
    tracker_info.vcslong_sorted_next_track[1] = &object_tracks[3];
    tracker_info.vcslong_sorted_next_track[3] = &object_tracks[2];
    tracker_info.vcslong_sorted_next_track[2] = &object_tracks[4];
    tracker_info.vcslong_sorted_next_track[4] = &object_tracks[5];
    tracker_info.vcslong_sorted_next_track[5] = &object_tracks[6];
    tracker_info.vcslong_sorted_next_track[6] = NULL;

    tracker_info.vcslong_sorted_prev_track[0] = NULL;
    tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
    tracker_info.vcslong_sorted_prev_track[3] = &object_tracks[1];
    tracker_info.vcslong_sorted_prev_track[2] = &object_tracks[3];
    tracker_info.vcslong_sorted_prev_track[4] = &object_tracks[2];
    tracker_info.vcslong_sorted_prev_track[5] = &object_tracks[4];
    tracker_info.vcslong_sorted_prev_track[6] = &object_tracks[5];

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept 
     */
   CHECK_EQUAL(5, tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(3, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
}

/** \purpose
 *  Test that killing the second object of the first merge, while they are not consecutive in the list 
    of sorted objects and there is a second merge happening, doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_Second_Object_Of_The_First_Merge_While_They_Are_Not_Consecutive_In_The_List)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the fourth wanted scenario.
    */
    // Object 2 - Merge with object 3 - Keep this one
    object_tracks[1].time_since_cluster_created = 1.8F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Kill this one
    object_tracks[2].time_since_cluster_created = 1.6F;
    object_tracks[2].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 4 - Merges with object 5 - Keep this one
    object_tracks[3].vcs_position.x = 8.05F;
    object_tracks[3].vcs_position.y = 10.0F;
    object_tracks[3].Update_Bbox_Center();
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.x = object_tracks[3].vcs_position.x;
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.y = object_tracks[3].vcs_position.y;

    // Object 5 - Merges with object 4 - Kill this one
    object_tracks[4].vcs_position.x = 8.2F; 
    object_tracks[4].vcs_position.y = 10.0F;
    object_tracks[4].Update_Bbox_Center();
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.x = object_tracks[4].vcs_position.x;
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.y = object_tracks[4].vcs_position.y;

    tracker_info.vcslong_sorted_start = &(object_tracks[0]);
    tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
    tracker_info.vcslong_sorted_next_track[1] = &object_tracks[3];
    tracker_info.vcslong_sorted_next_track[3] = &object_tracks[2];
    tracker_info.vcslong_sorted_next_track[2] = &object_tracks[4];
    tracker_info.vcslong_sorted_next_track[4] = &object_tracks[5];
    tracker_info.vcslong_sorted_next_track[5] = &object_tracks[6];
    tracker_info.vcslong_sorted_next_track[6] = NULL;

    tracker_info.vcslong_sorted_prev_track[0] = NULL;
    tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
    tracker_info.vcslong_sorted_prev_track[3] = &object_tracks[1];
    tracker_info.vcslong_sorted_prev_track[2] = &object_tracks[3];
    tracker_info.vcslong_sorted_prev_track[4] = &object_tracks[2];
    tracker_info.vcslong_sorted_prev_track[5] = &object_tracks[4];
    tracker_info.vcslong_sorted_prev_track[6] = &object_tracks[5];

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept 
     */
   CHECK_EQUAL(5 ,tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(2, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
}

/** \purpose
 *  Test that  killing the first object of the first merge, while they are not consecutive in the list
    of objects and the first merge contains 3 objects, doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_first_Object_Of_The_First_Merge_Which_Consists_Of_Three_Objects)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the fifth wanted scenario.
    */
    // Object 2 - Merge with object 3 - Kill this one
    object_tracks[1].time_since_cluster_created = 1.6F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Kill this one
    object_tracks[2].time_since_cluster_created = 1.8F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

    // Object 7
    object_tracks[6].vcs_position.x = 8.12F; 
    object_tracks[6].vcs_position.y = 1.0F;
    object_tracks[6].Update_Bbox_Center();
    detection_props[object_tracks[6].detids[0]-1U].vcs_position.x = object_tracks[6].vcs_position.x;
    detection_props[object_tracks[6].detids[0]-1U].vcs_position.y = object_tracks[6].vcs_position.y;
    object_tracks[6].time_since_cluster_created = 1.4F;
    object_tracks[6].time_since_initialization = object_tracks[6].time_since_cluster_created + 0.1F;

    tracker_info.vcslong_sorted_start = &(object_tracks[0]);
    tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
    tracker_info.vcslong_sorted_next_track[1] = &object_tracks[2];
    tracker_info.vcslong_sorted_next_track[2] = &object_tracks[6];
    tracker_info.vcslong_sorted_next_track[6] = &object_tracks[3];
    tracker_info.vcslong_sorted_next_track[3] = &object_tracks[4];
    tracker_info.vcslong_sorted_next_track[4] = &object_tracks[5];
    tracker_info.vcslong_sorted_next_track[5] = NULL;

    tracker_info.vcslong_sorted_prev_track[0] = NULL;
    tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
    tracker_info.vcslong_sorted_prev_track[2] = &object_tracks[1];
    tracker_info.vcslong_sorted_prev_track[6] = &object_tracks[2];
    tracker_info.vcslong_sorted_prev_track[3] = &object_tracks[6];
    tracker_info.vcslong_sorted_prev_track[4] = &object_tracks[3];
    tracker_info.vcslong_sorted_prev_track[5] = &object_tracks[4];

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept 
     */
   CHECK_EQUAL(4 ,tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(3, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);

}

/** \purpose
 *  Test that  killing the second object of the first merge, while they are not consecutive in the list
    of objects and the first merge should merge again with a third object in the next scan, doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_Second_Object_Of_The_First_Merge_While_A_Third_Object_Fills_The_Conditions_To_Be_Included_In_The_First_Merge)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the sixth wanted scenario.
    */
    // Object 2 - Merge with object 3 - Keep this one
    object_tracks[1].time_since_cluster_created = 1.8F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Kill this one
    object_tracks[2].vcs_position.x = 8.03F;
    object_tracks[2].vcs_position.y = 1.0F;
    object_tracks[2].Update_Bbox_Center();
    object_tracks[2].time_since_cluster_created = 1.6F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;
    
    // Object 7
    object_tracks[6].vcs_position.x = 8.05F; 
    object_tracks[6].vcs_position.y = 1.0F;
    object_tracks[6].Update_Bbox_Center();
    object_tracks[6].time_since_cluster_created = 1.4F;
    object_tracks[6].time_since_initialization = object_tracks[6].time_since_cluster_created + 0.1F;

    tracker_info.vcslong_sorted_start = &(object_tracks[0]);
    tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
    tracker_info.vcslong_sorted_next_track[1] = &object_tracks[2];
    tracker_info.vcslong_sorted_next_track[2] = &object_tracks[6];
    tracker_info.vcslong_sorted_next_track[6] = &object_tracks[3];
    tracker_info.vcslong_sorted_next_track[3] = &object_tracks[4];
    tracker_info.vcslong_sorted_next_track[4] = &object_tracks[5];
    tracker_info.vcslong_sorted_next_track[5] = NULL;

    tracker_info.vcslong_sorted_prev_track[0] = NULL;
    tracker_info.vcslong_sorted_prev_track[1] = &(object_tracks[0]);
    tracker_info.vcslong_sorted_prev_track[2] = &object_tracks[1];
    tracker_info.vcslong_sorted_prev_track[6] = &object_tracks[2];
    tracker_info.vcslong_sorted_prev_track[3] = &object_tracks[6];
    tracker_info.vcslong_sorted_prev_track[4] = &object_tracks[3];
    tracker_info.vcslong_sorted_prev_track[5] = &object_tracks[4];

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept
     */
   CHECK_EQUAL(5 ,tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
   CHECK_EQUAL(2, tracker_info.active_obj_ids[1U]);
   CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
   CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
   CHECK_EQUAL(7, tracker_info.active_obj_ids[4U]);
}

/** \purpose
 * Test that killing the first object of the second merge which the last 2 objects are merged from a list of sorted objects, while there is an object between this and previous merge, 
  doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_First_Object_Of_The_First_Merge_And_The_First_Object_of_The_Sexond_Merge)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the seventh wanted scenario.
    */
    // Object 2 - Merge with object 3 - Kill this one
    object_tracks[1].time_since_cluster_created = 1.6F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Keep this one
    object_tracks[2].time_since_cluster_created = 1.8F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

    // Object 4 - Merges with object 5 
    object_tracks[3].vcs_position.x = 8.3F;
    object_tracks[3].vcs_position.y = 10.0F;
    object_tracks[3].Update_Bbox_Center();
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.x = object_tracks[3].vcs_position.x;
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.y = object_tracks[3].vcs_position.y;
    object_tracks[3].time_since_cluster_created = 10.8F;
    object_tracks[3].time_since_initialization = object_tracks[3].time_since_cluster_created + 0.1F;

    // Object 5 - Merges with object 6 - Kill this one
    object_tracks[4].vcs_position.x = 20.0F;
    object_tracks[4].vcs_position.y = 15.0F;
    object_tracks[4].Update_Bbox_Center();
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.x = object_tracks[4].vcs_position.x;
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.y = object_tracks[4].vcs_position.y;
    object_tracks[4].time_since_cluster_created = 1.6F;
    object_tracks[4].time_since_initialization = object_tracks[4].time_since_cluster_created + 0.1F;

    // Object 6
    object_tracks[5].vcs_position.x = 20.1F;
    object_tracks[5].vcs_position.y = 15.0F;
    object_tracks[5].Update_Bbox_Center();
    detection_props[object_tracks[5].detids[0]-1U].vcs_position.x = object_tracks[5].vcs_position.x;
    detection_props[object_tracks[5].detids[0]-1U].vcs_position.y = object_tracks[5].vcs_position.y;
    object_tracks[5].time_since_cluster_created = 1.8F;
    object_tracks[5].time_since_initialization = object_tracks[5].time_since_cluster_created + 0.1F;

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are kept
     */
    CHECK_EQUAL(5 ,tracker_info.num_active_objs); 
    CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
    CHECK_EQUAL(3, tracker_info.active_obj_ids[1U]);
    CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
    CHECK_EQUAL(6, tracker_info.active_obj_ids[3U]);
    CHECK_EQUAL(7, tracker_info.active_obj_ids[4U]);
}

/** \purpose
 * Test that killing the second object of the second merge which the last 2 objects are merged from a list of sorted objects, while there is an object between this and previous merge, 
  doesn't lead breaking early and not checking every potential merge.
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, Killing_The_First_Object_Of_The_First_Merge_And_The_Second_Object_of_The_Sexond_Merge)
{
    /** \precond
    * Set positions and time since they were created for all objects in order to create the eighth wanted scenario.
    */
    // Object 2 - Merge with object 3 - Kill this one
    object_tracks[1].time_since_cluster_created = 1.6F;
    object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

    // Object 3 - Merge with object 2 - Keep this one
    object_tracks[2].time_since_cluster_created = 1.8F;
    object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

    // Object 4
    object_tracks[3].vcs_position.x = 8.3F;
    object_tracks[3].vcs_position.y = 10.0F;
    object_tracks[3].Update_Bbox_Center();
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.x = object_tracks[3].vcs_position.x;
    detection_props[object_tracks[3].detids[0]-1U].vcs_position.y = object_tracks[3].vcs_position.y;
    object_tracks[3].time_since_cluster_created = 10.8F;
    object_tracks[3].time_since_initialization = object_tracks[3].time_since_cluster_created + 0.1F;

    // Object 5 - Merges with object 6 - Keep this one
    object_tracks[4].vcs_position.x = 20.0F; 
    object_tracks[4].vcs_position.y = 15.0F;
    object_tracks[4].Update_Bbox_Center();
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.x = object_tracks[4].vcs_position.x;
    detection_props[object_tracks[4].detids[0]-1U].vcs_position.y = object_tracks[4].vcs_position.y;
    object_tracks[4].time_since_cluster_created = 1.8F;
    object_tracks[4].time_since_initialization = object_tracks[4].time_since_cluster_created + 0.1F;

    // Object 6 - Merges with object 5 - Kill this one
    object_tracks[5].vcs_position.x = 20.1F; 
    object_tracks[5].vcs_position.y = 15.0F;
    object_tracks[5].Update_Bbox_Center();
    detection_props[object_tracks[5].detids[0]-1U].vcs_position.x = object_tracks[5].vcs_position.x;
    detection_props[object_tracks[5].detids[0]-1U].vcs_position.y = object_tracks[5].vcs_position.y;
    object_tracks[5].time_since_cluster_created = 1.6F;
    object_tracks[5].time_since_initialization = object_tracks[5].time_since_cluster_created + 0.1F;

    /** \action
    *  call Track_Grouping().
    */
    Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);

    /** \result
     * Check that the number of active objects is correct and that the expected object IDs are keptt 
     */
    CHECK_EQUAL(5, tracker_info.num_active_objs);
    CHECK_EQUAL(1, tracker_info.active_obj_ids[0U]);
    CHECK_EQUAL(3, tracker_info.active_obj_ids[1U]);
    CHECK_EQUAL(4, tracker_info.active_obj_ids[2U]);
    CHECK_EQUAL(5, tracker_info.active_obj_ids[3U]);
    CHECK_EQUAL(7, tracker_info.active_obj_ids[4U]);
}

/** \purpose
 *  Test that merge of two objects are being blocked if first merge candidate object is not moving
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_First_Obj_Not_Moving)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should be kept but set f_moving for second object to false such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;
   object_tracks[1].f_moving = false;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if second merge candidate object is not moving
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_Second_Obj_Not_Moving)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should be kept but set f_moving for third object to false such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;
   object_tracks[2].f_moving = false;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if first merge candidate object has large heading pointing disagreement
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_First_Obj_Large_Hdg_Pntg_Disagreement)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set heading pointing disagreement for second object to be large such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;
   object_tracks[1].hdg_ptng_disagmt = F360_DEG2RAD(10.1F);

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if second merge candidate object has large heading ointing disagreement
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_Second_Obj_Large_Hdg_Pntg_Disagreement)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set heading pointing diasagreement for third object to be large such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;
   object_tracks[2].hdg_ptng_disagmt = F360_DEG2RAD(10.1F);

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if first merge candidate object has small speed
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_First_Obj_Small_Speed)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set speed for second object to be small such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;
   object_tracks[1].speed = 0.9F;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if second merge candidate object has small speed
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_Second_Obj_Small_Speed)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set speed for third object to be small such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;
   object_tracks[2].speed = 0.9F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if first merge candidate object has no associated detections
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_First_Obj_No_Assoc_Dets)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set ndets for second object to be 0 such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;
   object_tracks[1].ndets = 0U;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** \purpose
 *  Test that merge of two objects are being blocked if second merge candidate object has no associated detections
 * \req  NA.
 */
TEST(f360_track_grouping_ut_sorted_list, No_Merge_Second_Obj_No_Assoc_Dets)
{
   /** \precond
   * Set number of active objects to 3 
   * Set properties for second and third object such that they should merged and such that third object should
   * be kept but set ndets for third object to 0 such that the merge is blocked.
   */
  tracker_info.num_active_objs = 3;

   // Object 2
   object_tracks[1].time_since_cluster_created = 1.7F;
   object_tracks[1].time_since_initialization = object_tracks[1].time_since_cluster_created + 0.1F;

   // Object 3
   object_tracks[2].time_since_cluster_created = 1.8F;
   object_tracks[2].time_since_initialization = object_tracks[2].time_since_cluster_created + 0.1F;
   object_tracks[2].ndets = 0U;

   /** \action
   *  call Track_Grouping().
   */
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, tracker_info, timing_info);
   
   /** \result
    * Check that the number of active objects is not reduced (i.e. that no objects has been merged) 
    */
   CHECK_EQUAL(3 ,tracker_info.num_active_objs);
}

/** @}*/
