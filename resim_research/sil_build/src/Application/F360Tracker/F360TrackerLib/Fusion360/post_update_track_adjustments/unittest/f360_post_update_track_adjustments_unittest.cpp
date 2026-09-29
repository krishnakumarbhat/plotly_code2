/** \file
 * This file contains unit tests for content of f360_post_update_track_adjustments.cpp file
 */

#include "f360_post_update_track_adjustments.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_post_update_track_adjustments
 *  @{
 */

/** \brief
*  Test group containing definition of all input parameters to function Post_Update_Track_Adjustments
*  Test group initializes tracker calibrations and sets 1 active object in tracker info
*  The input parameters are tweaked in the individual test cases.
**/
TEST_GROUP(f360_post_update_track_adjustments)
{
   F360_Calibrations_T calibs = {};
   F360_Globals_T globals = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};

   /** \setup
   * Initialize tracker calibrations
   * Set number of active objects to 1 as default.
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      globals.f_single_front_center_radar_only = false;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      object_tracks[0].id = 1;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Verify that objects status gets updated by call to Post_Update_Track_Adjustments()
*          through subfunction Obj_Trk_Status_Book_Keeping()
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Status_Change)
{

   /** \precond
   * Set object status to updated
   * Set object time since initalization to 1 second
   * Set no associated detections
   **/
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_initialization = 1.0F;
   object_tracks[0].ndets = 0;

   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that object status have changed status to COASTING
   **/
   CHECK_EQUAL(F360_OBJECT_STATUS_COASTED, object_tracks[0].status);
}

/**
*\purpose  Verify that objects filter type gets updated by call to Post_Update_Track_Adjustments()
*          through subfunction Adjust_Fltr_Type_Dependent_Params()
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Filter_Type_Change)
{

   /** \precond
   * Set object filter type to CCV
   * Set moving flag to true
   * Set object speed greater than threshold
   * Set curvature to 0
   * Set time_since_initialization to larger than 2.0F
   **/
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].f_moving = true;
   object_tracks[0].speed = calibs.fast_moving_thresh + 1.0F;
   object_tracks[0].curvature = 0.0F;
   object_tracks[0].time_since_initialization = 2.0F + 1e-3F; 


   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that object filter type have changed to CTCA
   **/
   CHECK_EQUAL(F360_TRACKER_TRKFLTR_CTCA, object_tracks[0].trk_fltr_type);
}

/**
*\purpose  Verify that new object that overlaps with existing object is terminated by call to Post_Update_Track_Adjustments()
*          through subfunction Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks().
*          This test also verifies that objects are re-sorted in longitudinal order.
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Terminate_New_Object_Overlapping_With_Existing_Object)
{

   /** \precond
   * Create two objects, one new and one old at same position (0,0)
   * Set tracker info
   * Set old object id
   * Set old objects status
   * Set old objects size
   * Set old object pointing
   * Set old object moving flag
   * Set old object time since cluster created
   * Set old object confidence level high
   * Set new object status
   * Set new object size
   * Set new object pointing
   * Set new object moving flag
   **/
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &object_tracks[0];
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].Update_Bbox_Size(4.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[0].f_moving = true;
   object_tracks[0].time_since_cluster_created = calibs.k_puta_min_object_time + 1.0F;
   object_tracks[0].confidenceLevel = calibs.k_puta_min_object_confidence + 0.1F;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].Update_Bbox_Size(2.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[1].f_moving = true;

   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that new object have been terminated
   * Verify that vcs longitudinal sorted list have been updated
   **/
   CHECK_EQUAL(1, tracker_info.num_active_objs);
   POINTERS_EQUAL(&object_tracks[0], tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[0]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[1]);
}

/**
*\purpose  Verify that object that overlaps with existing object is flagged by call to Post_Update_Track_Adjustments()
*          through subfunction Adjust_Overlapping_Confirmed_Tracks().
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Object_Overlapping_With_Other_Object)
{

   /** \precond
   * Create two objects, one new and one old at same position (0,0)
   * Set tracker info
   * Set first object id
   * Set first objects status
   * Set first objects size
   * Set first object pointing
   * Set first object moving flag
   * Set first object time since cluster created
   * Set first object confidence level high
   * Set second object id
   * Set second object status
   * Set second object size
   * Set second object pointing
   * Set second object moving flag
   * Set second object moveable flag
   * Set second object confidence low
   **/
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &object_tracks[0];
   object_tracks[0].id = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].Update_Bbox_Size(4.0F, 2.0F);
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[0].f_moving = true;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].time_since_cluster_created = calibs.k_puta_min_object_time + 1.0F;
   object_tracks[0].confidenceLevel = calibs.k_puta_overlapping_tracks_high_conf_thr + 0.1F;
   object_tracks[1].id = 2;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].Update_Bbox_Size(2.0F, 1.0F);
   object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[1].f_moving = true;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].confidenceLevel = calibs.low_confidence_level_thresh + 0.1F;

   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that second object is flagged as overlapping with first object
   **/
   CHECK_TRUE(object_tracks[1].f_overlapping_with_object);
}

/**
*\purpose  Verify that objects are re-sorted in longitudinal order by call to Post_Update_Track_Adjustments()
*          through subfunction Sorted_Tracks_Update_List().
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Objects_Resorted)
{

   /** \precond
   * Create two objects with different vcs longitudinal position
   * Set sorted objects information in tracker info so that the order is wrong
   **/
   F360_Object_Track_T object_1 = {};
   object_1.id = 1;
   object_1.vcs_position.x = 10.0F;
   F360_Object_Track_T object_2 = {};
   object_2.id = 2;
   object_2.vcs_position.x = 0.0F;

   // Initialize object_tracks IDs to prevent out-of-bounds access in the loop
   object_tracks[0].id = 1;
   object_tracks[1].id = 2;

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.vcslong_sorted_start = &object_1;
   tracker_info.vcslong_sorted_next_track[0] = &object_2;
   tracker_info.vcslong_sorted_prev_track[1] = &object_1;

   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that objects have been re-sorted
   **/
   POINTERS_EQUAL(&object_2, tracker_info.vcslong_sorted_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[0]);
   POINTERS_EQUAL(&object_1, tracker_info.vcslong_sorted_next_track[1]);
   POINTERS_EQUAL(&object_2, tracker_info.vcslong_sorted_prev_track[0]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[1]);
}

/**
*\purpose  Verify that clusters are re-sorted in longitudinal order by call to Post_Update_Track_Adjustments()
*          through subfunction Sorted_Clusters_Update_List().
*\req    NA
*/
TEST(f360_post_update_track_adjustments, Post_Update_Track_Adjustments__Clusters_Resorted)
{

   /** \precond
   * Create two clusters with different vcs longitudinal position
   * Set sorted cluster information in tracker info so that the order is wrong
   **/
   F360_Cluster_T cluster_1 = {};
   cluster_1.id = 1;
   cluster_1.vcs_position_x = 10.0F;
   F360_Cluster_T cluster_2 = {};
   cluster_2.id = 2;
   cluster_2.vcs_position_x = 0.0F;

   tracker_info.num_active_clusters = 2;
   tracker_info.vcslong_sorted_cluster_start = &cluster_1;
   tracker_info.vcslong_sorted_cluster_next[0] = &cluster_2;
   tracker_info.vcslong_sorted_cluster_prev[1] = &cluster_1;
   tracker_info.vcslong_sorted_cluster_list[0] = 1;
   tracker_info.vcslong_sorted_cluster_list[1] = 2;

   /** \action
   * Call Post_Update_Track_Adjustments
   **/
   Post_Update_Track_Adjustments(
      calibs,
      globals,
      raw_detect_list,
      host,
      sensors,
      tracker_info,
      det_props,
      object_tracks,
      timing_info);

   /** \result
   * Verify that clusters have been re-sorted
   **/
   POINTERS_EQUAL(&cluster_2, tracker_info.vcslong_sorted_cluster_start);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_cluster_next[0]);
   POINTERS_EQUAL(&cluster_1, tracker_info.vcslong_sorted_cluster_next[1]);
   POINTERS_EQUAL(&cluster_2, tracker_info.vcslong_sorted_cluster_prev[0]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_cluster_prev[1]);
   CHECK_EQUAL(2, tracker_info.vcslong_sorted_cluster_list[0]);
   CHECK_EQUAL(1, tracker_info.vcslong_sorted_cluster_list[1]);
}

/** @}*/
