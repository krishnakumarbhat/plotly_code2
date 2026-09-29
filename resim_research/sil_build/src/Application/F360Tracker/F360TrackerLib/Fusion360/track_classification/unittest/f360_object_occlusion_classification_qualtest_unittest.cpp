/** \file
 * This file contains qualification tests for content of the f360_object_occlusion_classification.cpp file
 */

#include "f360_object_occlusion_classification.h"
#include <CppUTest/TestHarness.h>

#include "f360_clear_object_track.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  f360_object_occlusion_classification
 *  @{
 */

/** \brief
 * Test group dedicated to verify the functionality of the function Object_Occlusion_Classification().
 * It is done by setting up a scenario with a single forward looking sensor and 2 objects. 
 * 1 object is intended to be tested and while the other object is placed between the test object and 
 * the sensor, occluding the test object. This scenario is varied slightly to achieve different levels
 * of occlusion.
 */
TEST_GROUP(f360_object_occlusion_classification)
{	
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   const uint32_t obj_id_occluding = 3U;
   const uint32_t obj_id_to_be_tested = 6U;

   // For occlusion
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   /** \setup
    * Initialize tracker calibrations.
    * Setup a single forward looking sensor such that
    * - its mounting position is in vcs origo.
    * - its boresight is vcs forward
    * - is valid
    * - has look id F360_DET_LOOK_ID_0
    * - for that look id, has a fov_min_az_rad of -1.5F
    * - for that look id, has a fov_max_az_rad of 1.5F
    * - for that look id, has a range limit of 150.0F
    * Setup tracker_info to contain 2 active objects and object ids
    * Setup the occluding object to
    * - have vcs position (25,0)
    * - be f_moving
    * - be f_moveable
    * - have status updated
    * - have confidence_level 1
    * - have reference point rear
    * - have heading and pointing 0
    * - have a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));

      Setup_Front_Center_Sensor_For_Occlusion(sensors[0]);

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj_id_occluding;
      tracker_info.active_obj_ids[1] = obj_id_to_be_tested;
      
      object_tracks[obj_id_occluding - 1].id = obj_id_occluding;
      object_tracks[obj_id_to_be_tested - 1].id = obj_id_to_be_tested;

      for (F360_Object_Track_T& obj : object_tracks)
      {
         Clear_Object_Track(obj);
      }

      const Point occluding_obj_position = Point{25.0F, 0.0F};
      Set_Object_Parameters_To_Be_Valid_For_Occlusion(occluding_obj_position, object_tracks[obj_id_occluding - 1]);
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
      object.reference_point = F360_REFERENCE_POINT_REAR;
      object.vcs_position = vcs_pos;

      object.vcs_heading = Angle{0.0F};
      object.bbox.Set_Orientation(object.vcs_heading);

      object.bbox.Set_Length(2.0F);
      object.bbox.Set_Width(10.0F);
      object.Update_Bbox_Center();
   }
   
   void Setup_Front_Center_Sensor_For_Occlusion(
      F360_Radar_Sensor_T& sensor)
   {
      sensor.constant.mounting_position.vcs_position.lateral = 0.0F;
      sensor.constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

      sensor.variable.is_valid = true;
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensor.constant.fov_min_az_rad[sensor.variable.look_id] = -1.5F;
      sensor.constant.fov_max_az_rad[sensor.variable.look_id] = 1.5F;
      sensor.constant.range_limits[sensor.variable.look_id] = 150.0F;
   }
};

/** \purpose  
 * Verify that an object whose reference point is positioned behind another object is marked as occluded.
 * \req
 * CPR-3830, CPR-3831
 */
TEST(f360_object_occlusion_classification, Verify_Single_Object_Status_Occluded)
{
   /** \precond
    * Setup the test object to be positioned at (37.0), i.e. behind the occluding object
    * Set the test object to also
    * - be f_moving
    * - be moveable
    * - have status updated
    * - have confidence_level 1
    * - have reference point rear
    * - have heading and pointing 0
    * - have a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{37.0F, 0.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
	
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Object_Occlusion_Classification().
    */
   Object_Occlusion_Classification(occlusion_data, sensors, tracker_info, object_tracks);

   /** \result
    * Verify the test object occlusion_status.at_vcs_position have status OCCLUDED
    */
   const F360_Object_Track_T& test_obj = object_tracks[obj_id_to_be_tested - 1];
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, test_obj.occlusion_status);
}

/** \purpose  
 * Verify that an object whose reference point is positioned behind another object corner is marked as on edge.
 * \req
 * CPR-3830, CPR-3831
 */
TEST(f360_object_occlusion_classification, Verify_Single_Object_Status_On_Edge)
{
   /** \precond
    * Setup the test object to be positioned at (28,2), i.e. behind the occluding object close to the right corner
    * of the occluding object.
    * Set the test object to also
    * - be f_moving
    * - movable_prob be 1.0
    * - status updated
    * - confidence_level 1
    * - reference point rear
    * - heading and pointing -15 deg
    * - a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{28.0F, 2.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
   object_tracks[obj_id_to_be_tested - 1].Set_Bbox_Orientation(Angle{F360_DEG2RAD(-15.0F)});
	
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Object_Occlusion_Classification().
    */
   Object_Occlusion_Classification(occlusion_data, sensors, tracker_info, object_tracks);

   /** \result
    * Verify the test object occlusion_status.at_vcs_position have status ON_EDGE
    */
   const F360_Object_Track_T& test_obj = object_tracks[obj_id_to_be_tested - 1];
   CHECK_EQUAL(OCCLUSION_STATUS_ON_EDGE, test_obj.occlusion_status);
}

/** \purpose  
 * Verify that an object whose reference point is not positioned behind another objectis marked as visible.
 * \req
 * CPR-3830, CPR-3831
 */
TEST(f360_object_occlusion_classification, Verify_Single_Object_Status_Visible)
{
   /** \precond
    * Setup the test object to be positioned at (28,6), i.e. there is a free line of sight between the sensor
    * and the test object.
    * Set the test object to also
    * - be f_moving
    * - be moveable
    * - status updated
    * - confidence_level 1
    * - reference point rear
    * - heading and pointing 0
    * - a length of 2 and width of 10
    * - bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{25.0F, 5.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
	
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Object_Occlusion_Classification().
    */
   Object_Occlusion_Classification(occlusion_data, sensors, tracker_info, object_tracks);

   /** \result
    * Verify the test object occlusion_status.at_vcs_position have status VISIBLE
    */
   const F360_Object_Track_T& test_obj = object_tracks[obj_id_to_be_tested - 1];
   CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, test_obj.occlusion_status);
}

/** \purpose  
 * Verify that an object's occlusion status is marked as UNDEFINED if the object is not moving
 * \req
 * CPR-3830, CPR-3831
 */
TEST(f360_object_occlusion_classification, Verify_Single_Object_Status_Undefined_Not_Moving)
{
   /** \precond
    * Setup the test object to be positioned at (28,0), i.e. behind the occluding object
    * Set the test object to also
    * - be moveable
    * - status updated
    * - confidence_level 1
    * - reference point rear
    * - heading and pointing 0
    * - a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{28.0F, 0.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
   object_tracks[obj_id_to_be_tested - 1].f_moving = false;
	
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Object_Occlusion_Classification().
    */
   Object_Occlusion_Classification(occlusion_data, sensors, tracker_info, object_tracks);

   /** \result
    * Verify the test object occlusion_status.at_vcs_position have status UNDEFINED
    */
   const F360_Object_Track_T& test_obj = object_tracks[obj_id_to_be_tested - 1];
   CHECK_EQUAL(OCCLUSION_STATUS_UNDEFINED, test_obj.occlusion_status);
}

/** \purpose  
 * Verify that all active moving objects have their occlusion status evaluated
 * \req
 * CPR-3830, CPR-3831
 */
TEST(f360_object_occlusion_classification, Verify_All_Active_Objects_Are_Considered)
{
   /** \precond
    * Setup the test object to be positioned at (28,6), i.e. there is a free line of sight between the sensor
    * and the test object.
    * Set the test object to also
    * - be moving
    * - be moveable
    * - status updated
    * - confidence_level 1
    * - reference point rear
    * - heading and pointing 0
    * - a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{25.0F, 5.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
	
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Object_Occlusion_Classification().
    */
   Object_Occlusion_Classification(occlusion_data, sensors, tracker_info, object_tracks);

   /** \result
    * Verify the test object occlusion_status.at_vcs_position have status VISIBLE
    */
   for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
   {
      const int32_t obj_id = tracker_info.active_obj_ids[obj_idx];
      const F360_Object_Track_T& obj = object_tracks[obj_id - 1];

      CHECK_EQUAL(OCCLUSION_STATUS_VISIBLE, obj.occlusion_status)
   };
}
/** @}*/
