#include "CppUTest/TestHarness.h"
#include "f360_identify_fast_approach_ghost_behind_host.h"
#include "f360_occlusion.h"
#include "f360_clear_object_track.h"

using namespace f360_variant_A;

/** \defgroup  F360_Identify_Fast_Approach_Ghost_Behind_HostTest
 *  @{
 */

 /** \brief
  * Test group for validating Identify_Fast_Approach_Ghost_Behind_Host
  */
TEST_GROUP(F360_Identify_Fast_Approach_Ghost_Behind_Host)
{

   F360_Host_T host{};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Tracker_Info_T tracker_info{};
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Calibrations_T calib{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T test_object = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];
   const uint32_t obj_id_occluding = 1U;
   const uint32_t obj_id_to_be_tested = 2U;

   /** \setup
    * Initialize tracker calibrations.
    * Setup a single rear right radar sensor
    * Setup tracker_info to contain 2 active objects and object ids
    * Setup the occluding object to occlude the test object.
    */
   void setup()
   {
      Initialize_Tracker_Calibrations(calib);

      test_object.id = obj_id_to_be_tested;
      Setup_Rear_Right_Sensor_For_Occlusion(sensors[0]);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj_id_occluding;
      tracker_info.active_obj_ids[1] = obj_id_to_be_tested;
      object_tracks[obj_id_occluding - 1].id = obj_id_occluding;

      for (F360_Object_Track_T& obj : object_tracks)
      {
         Clear_Object_Track(obj);
      }

      const Point occluding_obj_position = Point{-13.45F, -0.138F};
      Set_Object_Parameters_To_Be_Valid_For_Occlusion(occluding_obj_position, object_tracks[obj_id_occluding - 1]);
      Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_object.vcs_position, test_object);
   }

   // Helper functions for occlusion initialization
   void Set_Object_Parameters_To_Be_Valid_For_Occlusion(
      const Point& vcs_pos,
      F360_Object_Track_T& object)
   {
      object.f_moving = true;
      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      object.confidenceLevel = 1.0F;
      object.reference_point = F360_REFERENCE_POINT_FRONT;
      object.vcs_position = vcs_pos;

      object.vcs_heading = Angle{0.0F};
      object.bbox.Set_Orientation(object.vcs_heading);

      object.bbox.Set_Length(4.24F);
      object.bbox.Set_Width(1.48F);
      object.Update_Bbox_Center();
   }
   
   // Helper functions for occlusion initialization
   void Setup_Rear_Right_Sensor_For_Occlusion(
      F360_Radar_Sensor_T& sensor)
   {
      sensor.constant.mounting_position.vcs_position.lateral = 0.775F;
      sensor.constant.mounting_position.vcs_position.longitudinal = -4.547F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 2.3562F;

      sensor.variable.is_valid = true;
      sensor.variable.look_id = F360_DET_LOOK_ID_2;
      sensor.constant.fov_min_az_rad[sensor.variable.look_id] = -1.5F;
      sensor.constant.fov_max_az_rad[sensor.variable.look_id] = 1.5F;
      sensor.constant.range_limits[sensor.variable.look_id] = 250.0F;
   } 
};

/** \purpose  
 * Verify that a fast approaching object is identified as a potential ghost when all
 * associated inlier detections are determined as occluded from sensor's point of view.
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Fast_Approach_Ghost_Identified)
{
   /** \precond
    * Setup the test object to be positioned at (-39.5,-3.57), i.e. occluded by the occluding object
    * Set the test object to have
    * - speed larger than the host
    * - -10 deg heading
    * - 1 associated inlier detection that is occluded by an object from sensor point of view
    * Initialize occlusion
    */
   host.speed = 10.0F;
   test_object.vcs_position.x = -39.5F;
   test_object.vcs_position.y = -3.57F;
   test_object.speed = 22.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(-10.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -39.65F;
   det_props[0].vcs_position.y = -3.49F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = false;
   test_object.cnt_consecutive_visible_from_rear = 0;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);

   /** \result
    * Verify the f_hide_occluded_track_behind_host is set to True
    */
   CHECK_TRUE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when an associated inlier detection 
 * is found visible from sensor's point of view given prior conditions
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Ghost_Flag_Reset_When_Visible)
{
   /** \precond
    * Setup the test object to be positioned at (-39.5,-3.57) with no occluded moving object
    * Set the test object to have
    * - speed larger than the host
    * - -10 deg heading
    * - 1 associated inlier detection that is occluded by an object from sensor point of view
    * Initialize occlusion
    */
   host.speed = 10.0F;
   test_object.vcs_position.x = -39.5F;
   test_object.vcs_position.y = -3.57F;
   test_object.speed = 22.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(-10.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   object_tracks[obj_id_occluding - 1].f_moving = false;
   object_tracks[obj_id_occluding - 1].movable_prob = 0.0F;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -39.65F;
   det_props[0].vcs_position.y = -3.49F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset 
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when entry condition position x
 * is positive
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Reset_Positive_Position_x)
{
   /** \precond
    * Setup the test object to be at positive x position such that it is the only 
    * entry contion not met
    * Set the test object to have
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equal 1
    * Initialize occlusion
    */
   host.speed = 0.0F;
   test_object.vcs_position.x = 0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 2.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(0.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = 0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset 
    * since it was set before but now object is not meeting entry condition
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when entry condition object
 * speed is below host speed
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Reset_When_Speed_Below_Host)
{
   /** \precond
    * Setup the test object to be positioned at (-39.5,-3.57), occluded by the occluding object
    * Set the test object to have
    * - speed smaller than the host
    * - -10 deg heading
    * - 1 associated inlier detection that is occluded by an object from sensor point of view
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 10.0F;
   test_object.vcs_position.x = -39.5F;
   test_object.vcs_position.y = -3.57F;
   test_object.speed = 9.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(-10.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -39.65F;
   det_props[0].vcs_position.y = -3.49F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset 
    * since it was set before but now object is not meeting entry condition
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when entry condition speed 
 * is below 1 meter per second
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Reset_Obj_Speed_Too_Low)
{
   /** \precond
    * Setup object to have:
    * - Negative x position
    * - Host speed < Object speed
    * - Object speed < 1 m/s
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 0.5F;
   test_object.vcs_position.x = -0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 0.9F;
   test_object.vcs_heading.Value(F360_DEG2RAD(0.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset 
    * since it was set before but now object is not meeting entry condition
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when entry condition heading
 * is greater than threshold
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Reset_Obj_Heading_Too_High)
{
   /** \precond
    * Setup object to have:
    * - Negative x position
    * - Host speed < Object speed
    * - Object speed > 1 m/s
    * - Object heading  > 60 deg
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 0.5F;
   test_object.vcs_position.x = -0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 2.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(60.1F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset 
    * since it was set before but now object is not meeting entry condition
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is reset when entry condition heading
 * is greater than threshold but negative
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Reset_Obj_Heading_Too_High_Negative)
{
   /** \precond
    * Setup object to have:
    * - Negative x position
    * - Host speed < Object speed
    * - Object speed > 1 m/s
    * - Object heading  < -60 deg
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 0.5F;
   test_object.vcs_position.x = -0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 2.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(-60.1F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset
    * since it was set before but now object is not meeting entry condition
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is not reset when entry condition num_rr_inlier_dets
 * is not above threshold
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_No_Reset_Obj_Num_RR_Inlier_Dets_Too_Low)
{
   /** \precond
    * Set the test object to have 0 associated inlier detections
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 0.5F;
   test_object.vcs_position.x = -0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 2.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(0.0F));
   test_object.num_rr_inlier_dets = 0;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = false;
   det_props[0].vcs_position.x = -0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = true;
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear are not changed
    * since num rr inlier dets are 0
    */
   CHECK_TRUE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(1, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that a detection occluded by the associated object itself is not considered as occlusion and thus
 * f_hide_occluded_track_behind_host shall be reset
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_No_Ghost_Identified_Given_Det_In_BBox)
{
   /** \precond
    * Set the occluding object (the test target in this test case) to have
    * - speed larger than the host
    * - 0 deg heading
    * - 1 associated inlier detection that is visible to the sensor but positioned in the rear part of the object
    * - f_hide_occluded_track_behind_host set to True
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   F360_Object_Track_T& occluding_object = object_tracks[obj_id_occluding - 1];
   host.speed = 10.0F;
   occluding_object.speed = 22.0F;
   occluding_object.num_rr_inlier_dets = 1;
   occluding_object.ndets = 1;
   occluding_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = -16.9F;
   det_props[0].vcs_position.y = -0.9F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   occluding_object.f_hide_occluded_track_behind_host = true;
   occluding_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, occluding_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is reset
    */
   CHECK_FALSE(occluding_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, occluding_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that f_hide_occluded_track_behind_host is not changed when f_hide_occluded_track_behind_host 
 * is not set prior to function call and there is no occlusion detected
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_No_Reset_Flag_Not_Set_No_Occlusion)
{
   /** \precond
    * Set the test object to not meet entry condition to no trigger occlusion check
    * - f_hide_occluded_track_behind_host set to false
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 0.5F;
   // Position in front of host, no occlusion check triggered
   test_object.vcs_position.x = 0.1F;
   test_object.vcs_position.y = 0.0F;
   test_object.speed = 2.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(0.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 1;
   test_object.detids[0] = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].vcs_position.x = 0.1F;
   det_props[0].vcs_position.y = 0.0F;

   raw_detect_list.detections[0].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = false;
   // Set cnt to non-zero to verify it is not changed
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host and cnt_consecutive_visible_from_rear is not changed
    */
   CHECK_FALSE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(1, test_object.cnt_consecutive_visible_from_rear);
}

/** \purpose  
 * Verify that detections that do not have f_rr_inlier set will be skipped in occlusion check
 * and f_hide_occluded_track_behind_host is still set due to another detection being occluded
 * \req NA.
 */
TEST(F360_Identify_Fast_Approach_Ghost_Behind_Host, Test_Set_One_Inlier_One_Outlier_Det)
{
   /** \precond
    * Object that is satisfying all entry conditions but one detection is an rr outlier
    * - f_hide_occluded_track_behind_host set to false
    * - cnt_consecutive_visible_from_rear equals 1
    * Initialize occlusion
    */
   host.speed = 10.0F;
   test_object.vcs_position.x = -39.5F;
   test_object.vcs_position.y = -3.57F;
   test_object.speed = 22.0F;
   test_object.vcs_heading.Value(F360_DEG2RAD(-10.0F));
   test_object.num_rr_inlier_dets = 1;
   test_object.ndets = 2;
   test_object.detids[0] = 1;
   test_object.detids[1] = 2;

   // First detection rr outlier
   det_props[0].f_rr_inlier = false;
   det_props[0].vcs_position.x = -39.65F;
   det_props[0].vcs_position.y = -3.49F;
   // Second detection rr inlier
   det_props[1].f_rr_inlier = true;
   det_props[1].vcs_position.x = -39.65F;
   det_props[1].vcs_position.y = -3.49F;

   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[1].raw.sensor_id = 1;

   test_object.f_hide_occluded_track_behind_host = false;
   // Non-zero to verify it is reset later
   test_object.cnt_consecutive_visible_from_rear = 1;

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   
   /** \action
    * Call Identify_Fast_Approach_Ghost_Behind_Host().
    */
   Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, test_object);
   
   /** \result
    * Verify that f_hide_occluded_track_behind_host is set and cnt_consecutive_visible_from_rear is reset 
    * since one inlier detection is occluded 
    */
   CHECK_TRUE(test_object.f_hide_occluded_track_behind_host);
   CHECK_EQUAL(0, test_object.cnt_consecutive_visible_from_rear);
}

/** @}*/
