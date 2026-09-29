/** \file
* This file contains unit tests for content of f360_is_object_suspected_stationary.cpp file
*/

#include "f360_is_object_suspected_stationary.h"
#include <CppUTest/TestHarness.h>
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  is_object_suspected_stationary__object_speed
*  @{
*/

/** \brief
* Test group of Check_If_Object_Is_Suspected_Stationary() function. Test verify whether
* stationary suspected flag is properly set based on comparsion of object speed with the moving_speed_threshold
*/
TEST_GROUP(is_object_suspected_stationary__object_speed)
{
   F360_Object_Track_T object;
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Host_T host;
   F360_Calibrations_T calib;
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   /** \setup
   * Initialize tracker calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
      calib.k_object_motion_min_speed = 1.5F;
   }
};

/** \purpose  
 * Purpose of this test is to verify that the object is suspected stationary when the object speed is lesser than calib.k_object_motion_min_speed
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__object_speed, Object_Suspected_Stationary__Based_On_moving_speed_threshold)
{
   /** \precond
    * Object speed to be lesser than calib.k_object_motion_min_speed
    */
  
   // Setup initialization
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   // Set up objects
   object.speed = calib.k_object_motion_min_speed - 0.1F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);
   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that the object is suspected stationary when the object speed is greater than calib.k_object_motion_min_speed and the filter type is INVALID
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__object_speed, Object_Suspected_Stationary__When_No_Main_Condition_Is_Met)
{
   /** \precond
    * Set Object speed to be greater than calib.k_object_motion_min_speed
    * Set Object as moveable
    * The above two conditions are chosen such that the moving_speed_threshold is equal to calib.k_object_motion_min_speed
    */
  
   // Setup initialization
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   // Set up objects
   object.movable_prob = 1.0F;
   object.speed = calib.k_object_motion_min_speed + 0.1F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_INVALID;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);
   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that the object is Not suspected stationary when the object speed is greater than 15.0 m/s
 */
TEST(is_object_suspected_stationary__object_speed, Object_Not_Suspected_Stationary__When_Speed_Larger_Than_Threshold)
{
   /** \precond
    * Set Object speed to be greater than k_max_vel_for_considering_stat_hypothesis = 15.0F
    * Then the object should not be suspicious for stationary, see test "Object_Can_Be_Suspected_Stationary__When_Speed_Smaller_Than_Threshold"
    */

   // Setup initialization - by default the object should be suspected to be stationary
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   // Set up object speed - so that the speed is higehr than the threshold. this should make the object Not suspicious for stationary
   const float32_t k_max_vel_for_considering_stat_hypothesis = 15.0F;
   object.speed = k_max_vel_for_considering_stat_hypothesis + 0.1F;
   object.vcs_velocity.longitudinal = k_max_vel_for_considering_stat_hypothesis + 0.1F;
   object.vcs_velocity.lateral = 0.0F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
    * Check the object is Not suspected to be stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that the object can be suspected stationary if the object speed is smaller than 15.0 m/s
 */
TEST(is_object_suspected_stationary__object_speed, Object_Can_Be_Suspected_Stationary__When_Speed_Smaller_Than_Threshold)
{
   /** \precond
    * Set Object speed to be lower than k_max_vel_for_considering_stat_hypothesis = 15.0F
    */

   // Setup initialization - by default the object should be eligible to be stationary-suspicious
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   // Set up object speed to be lower than the threshold to make the object suspicious for stationary
   const float32_t k_max_vel_for_considering_stat_hypothesis = 15.0F;
   object.speed = k_max_vel_for_considering_stat_hypothesis - 0.1F;
   object.vcs_velocity.longitudinal = k_max_vel_for_considering_stat_hypothesis - 0.1F;
   object.vcs_velocity.lateral = 0.0F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
    * Check the object is suspicious for stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \defgroup  is_object_suspected_stationary__CTCA
*  @{
*/

/** \brief
* Test group of Check_If_Object_Is_Suspected_Stationary() function. Test verify whether
* stationary suspected flag is properly set for CTCA tracks.
*/
TEST_GROUP(is_object_suspected_stationary__CTCA)
{
   F360_Object_Track_T object;
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Host_T host;
   F360_Calibrations_T calib;
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   /** \setup
   * Initialize tracker calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
      calib.k_object_motion_min_speed = 1.5F;
   }

   void Set_Object_Num_Moving_Dets(
      F360_Object_Track_T& object,
      bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
      else
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
   }

   void Set_Object_Nees_Params(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
      else
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 100.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 100.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
   }

   void Set_Object_Sigma_Params(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         const float32_t speed_std = calib.k_object_motion_min_speed;
         object.speed = calib.k_object_motion_sigma_ctca_th * speed_std + 1.0F;
         object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;
      }
      else
      {
         const float32_t speed_std = calib.k_object_motion_min_speed;
         object.speed = calib.k_object_motion_sigma_ctca_th * speed_std - 1.0F;
         object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;
      }
   }

   void Set_Object_Parallel_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
      else
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th + 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th + 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th + 0.1F };
         object.speed = calib.k_object_motion_min_speed - 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
   }

   void Set_Object_Cross_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.movable_prob = 1.0F;
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = -10.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
      else
      {
         object.movable_prob = 0.0F;
         object.speed = 0.0F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = 0.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
   }

   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object, bool occluding)
   {
      if (occluding)
      {
         object.Set_Bbox_Orientation(Angle{F360_PI / 4.0F});
         object.vcs_heading = Angle{F360_PI / 4.0F};

         object.bbox.Set_Length(20.0F);
      }
      else
      {
         object.Set_Bbox_Orientation(Angle{0.0F});
         object.vcs_heading = Angle{0.0F};

         object.bbox.Set_Length(2.0F);
      }
      
      object.bbox.Set_Width(2.0F);

      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      
   }

   void Set_Occluding_Objects_Parameters(bool is_object_occluding)
   {
      Set_Base_Object_Parameters(object_tracks[0], is_object_occluding);
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      Point center = {-5.0F, 5.0F};
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();
      object_tracks[0].confidenceLevel = 1.0F; 

      Set_Base_Object_Parameters(object_tracks[1], is_object_occluding);
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      center = {5.0F, 5.0F};
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].vcs_position = object_tracks[1].bbox.Get_Corners().Rear_Left();
      object_tracks[1].confidenceLevel = 1.0F;
      
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
   }
};

/** \purpose  
 * Purpose of this test is to verify that when all CTCA suspected moving conditions are set to be false, then the object is suspected to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, All_CTCA_Moving_Conditions_False__Thus_Object_Suspected_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Sigma_Params(object, false);
   Set_Object_Parallel_Moving(object,false);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;


   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when, for a CTCA object the detections are Moving and 
 * all other suspected moving conditions are set to be false, then the object is suspected not to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, Moving_Dets__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = true), so that the detections suggest that the object is not stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, true);
   Set_Object_Sigma_Params(object, false);
   Set_Object_Parallel_Moving(object,false);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;


   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when, for a CTCA object the speed sigma (speed/sqrt(speed_variance)) is high
 * and all other suspected moving conditions are set to be false, then the object is suspected not to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, Vel_Sigma_High__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = true), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Parallel_Moving(object,false);
   Set_Object_Sigma_Params(object, true);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;


   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when the CTCA object is moving parallel to host
 * and all other suspected moving conditions are set to be false, then the object is suspected not to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, obj_Parallel_Moving__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = true), so that the object is suspected to be not stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Sigma_Params(object, false);
   Set_Object_Parallel_Moving(object, true);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when the CTCA object is cross moving and non_moveable
 * and all other suspected moving conditions are set to be false, then the object is suspected not to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, Obj_Cross_Moving_NonMoveable_Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = false), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(tf_desired_moving = true), so that the object is suspected to be not stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Sigma_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, true);
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when the CTCA object is not cross moving and moveable
 * and all other suspected moving conditions are set to be false, then the object is suspected not to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CTCA, Obj_Cross_Moving_Moveable__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CTCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Sigma_Params(f_desired_moving = false), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(tf_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Sigma_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, false);
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}
/** @}*/

/** \defgroup  is_object_suspected_stationary__CCA
*  @{
*/

/** \brief
* Test group of Check_If_Object_Is_Suspected_Stationary() function. Test verify whether
* stationary suspected flag is properly set for CCA tracks.
*/
TEST_GROUP(is_object_suspected_stationary__CCA)
{
   F360_Object_Track_T object;
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Host_T host;
   F360_Calibrations_T calib;
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   /** \setup
   * Initialize tracker calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
      calib.k_object_motion_min_speed = 1.5F;
   }

   void Set_Object_Num_Moving_Dets(
      F360_Object_Track_T& object,
      bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
      else
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
   }

   void Set_Object_Nees_Params(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
      else
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 100.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 100.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
   }

   void Set_Object_Parallel_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
      else
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th + 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th + 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th + 0.1F };
         object.speed = calib.k_object_motion_min_speed - 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
   }

   void Set_Object_Cross_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.movable_prob = 1.0F;
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = -10.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
      else
      {
         object.movable_prob = 0.0F;
         object.speed = 0.0F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = 0.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
   }

   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object, bool occluding)
   {
      if (occluding)
      {
         object.Set_Bbox_Orientation(Angle{F360_PI / 4.0F});
         object.vcs_heading = Angle{F360_PI / 4.0F};

         object.bbox.Set_Length(20.0F);
      }
      else
      {
         object.Set_Bbox_Orientation(Angle{0.0F});
         object.vcs_heading = Angle{0.0F};

         object.bbox.Set_Length(2.0F);
      }
      
      object.bbox.Set_Width(2.0F);

      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      
   }

   void Set_Occluding_Objects_Parameters(bool is_object_occluding)
   {
      Set_Base_Object_Parameters(object_tracks[0], is_object_occluding);
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      Point center = {-5.0F, 5.0F};
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();
      object_tracks[0].confidenceLevel = 1.0F; 

      Set_Base_Object_Parameters(object_tracks[1], is_object_occluding);
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      center = {5.0F, 5.0F};
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].vcs_position = object_tracks[1].bbox.Get_Corners().Rear_Left();
      object_tracks[1].confidenceLevel = 1.0F;
      
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
   }
};

/** \purpose  
 * Purpose of this test is to verify that when all but cross-radial moving, then the object is suspected to be moving
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CCA, Obj_Cross_Radial_Moving__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, false);
   object.vcs_position.y = 10.0F;
   object.vcs_position.x = 10.0F;
   object.speed = 5.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.vcs_velocity.longitudinal = -6.0F;
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify that when all CCA suspected moving conditions are set to be false, then the object is suspected to be stationary
 * \req
 * NA.
 */
TEST(is_object_suspected_stationary__CCA, Obj_Not_Cross_Radial_Moving_Moveable__Thus_Object_Suspected_Not_Stationary)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, false);
   object.speed = 2.0F;
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}
/** @}*/
