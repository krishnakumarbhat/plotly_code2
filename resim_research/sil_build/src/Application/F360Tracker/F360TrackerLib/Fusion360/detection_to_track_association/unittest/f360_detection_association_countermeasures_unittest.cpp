/** \file
 * This file contains unit tests for content of f360_detection_association_countermeasures.cpp file
 */

#include "f360_detection_association_countermeasures.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_detection_association_countermeasures
 *  @{
 */

/** \brief
* This test group includes test of the function Detection_Association_Countermeasures() defined in
* f360_detection_association_countermeasures.cpp.
 */
TEST_GROUP(f360_detection_association_countermeasures)
{	
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calibrations = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up a default scenario where the preconditions are set to that all countermeasures are called.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      // initialization input data
      object_tracks[0].ndets = 3;
      object_tracks[0].detids[0] = 1;
      object_tracks[0].detids[1] = 2;
      object_tracks[0].detids[2] = 3;

      detection_props[0].object_track_id = 1;
      detection_props[1].object_track_id = 1;
      detection_props[2].object_track_id = 1;
   }
};

/** \purpose  
 * Test that Detection_Association_Countermeasures() calls both the detection_based as well as the track-based countermeasures.
 * \req
 * NA
 */
TEST(f360_detection_association_countermeasures, DetectionAssociationCountermeasures_AllCM)
{
   /** \precond
    * No changes need to be made to the prepared input in setup.
    */
	
   /** \action
    * Call Detection_Association_Countermeasures().
    */
    Detection_Association_Countermeasures(tracker_info, raw_detection_list, calibrations, host, sensors, object_tracks, detection_props);

   /** \result
    * Check that both if clauses have been entered to detection and track-based countermeasures by:
    */	
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not correctly associated in an expected way.")
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not correctly associated in an expected way.")
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not correctly associated in an expected way.")
}

/** \purpose
* Test that Detection_Association_Countermeasures() calls none of the detection_based as well as the track-based countermeasures 
* when obj idx is, by mistake somewhere else, less than 0..
* \req
* NA
*/
TEST(f360_detection_association_countermeasures, DetectionAssociationCountermeasures_NoCM)
{
   /** \precond
   * Set first element of active objext IDs to 0.
   */
   tracker_info.num_active_objs = 0;
   tracker_info.active_obj_ids[0] = 0;

   /** \action
   * Call Detection_Association_Countermeasures().
   */
   Detection_Association_Countermeasures(tracker_info, raw_detection_list, calibrations, host, sensors, object_tracks, detection_props);

   /** \result
   * Check that the value of the detections' fields object_track_id have not been modified.
   */
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not correctly associated in an expected way.")
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not correctly associated in an expected way.")
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not correctly associated in an expected way.")
}

/** \purpose
* Test that function that conditionally deassociates detections from objects is called in Detection_Association_Countermeasures().
* \req
* NA
*/
TEST(f360_detection_association_countermeasures, DetectionAssociationCountermeasures_Cond_Deassoc)
{
   /** \precond
   * For first object in object list set:
   * - position to (0,7)
   * - wid1 and wid2 to 0.5m and width to 1m
   * - speed to 5m/s
   * - visible edge to left edge
   * - pointing to 0 
   * Set position of first detection in detection list to (0,8)
   * Set range rate compensated for the first detection to 0
   */
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = 7.0F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 1.0F });
   object_tracks[0].Update_Bbox_Size(1.0F, 1.0F);
   object_tracks[0].speed = 5.0F;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_LEFT;

   detection_props[0].vcs_position.x = 0.0F;
   detection_props[0].vcs_position.y = 8.0F;
   detection_props[0].range_rate_compensated = 0.0F;


   /** \action
   * Call Detection_Association_Countermeasures().
   */
   Detection_Association_Countermeasures(tracker_info, raw_detection_list, calibrations, host, sensors, object_tracks, detection_props);

   /** \result
   * Check the first detection was deassociated from the object
   */
   CHECK_EQUAL_TEXT(2, object_tracks[0].ndets, "The detection was not deassociated from the object as expected.");
   CHECK_EQUAL_TEXT(0, detection_props[0].object_track_id, "Detection with ID 1 was not deassociated as expected.");
}


/** @}*/

/** \defgroup  f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets
 *  @{
 */

/** \brief
* This test group includes test of the function Deassociate_Stationary_Range_Rate_Outlier_Dets
 */
TEST_GROUP(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets)
{	
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};

   /** \setup
    * Set up a default scenario where the preconditions are set to that all countermeasures are called.
    */
   TEST_SETUP()
   {
      // Set up an object with 5 detections
      // Set predicted range rate above threshold for all detections
      // Set all except one detection's motion status to moving
      // Set up sensor motion properties to calculate compensated predicted rang rates of detections
      sensors[0].variable.vcs_velocity.longitudinal = 3.0F;
      sensors[0].variable.vcs_velocity.lateral = 0.0F;

      object.movable_prob = 1.0F;
      object.speed = 1.0F;
      object.ndets = 5;
      
      object.detids[0] = 1;
      object.detids[1] = 2;
      object.detids[2] = 3;
      object.detids[3] = 4;
      object.detids[4] = 5;

      detection_props[0].object_track_id = 1;
      detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[0].range_rate_predicted = - 3.19F;
      detection_props[0].f_ok_to_use = true;
      float32_t det_az = 0.1F;
      raw_detection_list.detections[0].processed.cos_vcs_az = F360_Cosf(det_az);
      raw_detection_list.detections[0].processed.sin_vcs_az = F360_Sinf(det_az);
      raw_detection_list.detections[0].raw.sensor_id = 1U;

      detection_props[1].object_track_id = 1;
      detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[1].range_rate_predicted = - 2.3F;
      detection_props[1].f_ok_to_use = true;
      det_az = 0.5F;
      raw_detection_list.detections[1].processed.cos_vcs_az = F360_Cosf(det_az);
      raw_detection_list.detections[1].processed.sin_vcs_az = F360_Sinf(det_az);
      raw_detection_list.detections[1].raw.sensor_id = 1U;

      detection_props[2].object_track_id = 1;
      detection_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      detection_props[2].range_rate_predicted = - 3.0F;
      detection_props[2].f_ok_to_use = true;
      det_az = 0.5F;
      raw_detection_list.detections[2].processed.cos_vcs_az = F360_Cosf(det_az);
      raw_detection_list.detections[2].processed.sin_vcs_az = F360_Sinf(det_az);
      raw_detection_list.detections[2].raw.sensor_id = 1U;

      detection_props[3].object_track_id = 1;
      detection_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[3].range_rate_predicted = - 2.9F;
      detection_props[3].f_ok_to_use = true;
      det_az = 0.6F;
      raw_detection_list.detections[3].processed.cos_vcs_az = F360_Cosf(det_az);
      raw_detection_list.detections[3].processed.sin_vcs_az = F360_Sinf(det_az);
      raw_detection_list.detections[3].raw.sensor_id = 1U;

      detection_props[4].object_track_id = 1;
      detection_props[4].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[4].range_rate_predicted = - 4.0F;
      detection_props[4].f_ok_to_use = true;
      det_az = 0.0F;
      raw_detection_list.detections[4].processed.cos_vcs_az = F360_Cosf(det_az);
      raw_detection_list.detections[4].processed.sin_vcs_az = F360_Sinf(det_az);
      raw_detection_list.detections[4].raw.sensor_id = 1U;
   }
};

/** \purpose
* Test that the detection with motion status ambiguous is de-associated when there are enough moving detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_1_Deassociated)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The expectation is that the detection with motion status ambiguous shall be de-associated
   */


   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check the ambiguous motion status detection was deassociated from the object
   */
   CHECK_EQUAL_TEXT(4, object.ndets, "The detection was not deassociated from the object as expected.");
   CHECK_EQUAL_TEXT(0, detection_props[2].object_track_id, "Detection with ID 2 was not deassociated as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[3], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(0, detection_props[2].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_FALSE_TEXT( detection_props[2].f_ok_to_use, "The deassociated detection should not be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the detection with motion status ambiguous and low predicted range rate is not de-associated when there are enough moving detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_Not_Deassociated_Pred_RR)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * Change the predicted range rate for the ambiguous detection to be below the threshold
   * The expectation is that the detection with motion status ambiguous shall not be de-associated
   */
  detection_props[2].range_rate_predicted = - 2.5F;

   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check the ambiguous motion status detection was not deassociated from the object
   */
   CHECK_EQUAL_TEXT(5, object.ndets, "The detection was not deassociated from the object as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(3, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[3], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[4], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[2].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that no detections are de-associated when they all have motion status moving.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_All_Moving)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * Change the motion status for the ambiguous detection to moving
   * The expectation is that all detections shall be associated
   */
  detection_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check that all detections remain associated
   */
   CHECK_EQUAL_TEXT(5, object.ndets, "The detection was not deassociated from the object as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(3, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[3], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[4], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[2].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that no detections are de-associated when there are less than 3 moving detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_Less_Than_3_Moving)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * Set motion status of the detections such that only two are moving
   * The expectation is that no detections are de-associated.
   */
  detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
  detection_props[4].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check that all detections remain associated
   */
   CHECK_EQUAL_TEXT(5, object.ndets, "The detection was not deassociated from the object as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(3, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[3], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[4], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[2].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that no detections are de-associated when the object is not movable.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_Obj_Non_Movable)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * Set object to non movable
   * The expectation is that no detections are de-associated.
   */
  object.movable_prob = 0.0F;

   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check that all detections remain associated
   */
   CHECK_EQUAL_TEXT(5, object.ndets, "The detection was not deassociated from the object as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(3, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[3], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[4], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[2].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that no detections are de-associated when the object is driving faster than threshold value.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets, Deassociate_Stationary_Range_Rate_Outlier_Dets_Fast_Obj)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * Set object speed above threshold
   * The expectation is that no detections are de-associated.
   */
  object.speed = 2.6F;

   /** \action
   * Call Deassociate_Stationary_Range_Rate_Outlier_Dets().
   */
   Deassociate_Stationary_Range_Rate_Outlier_Dets(raw_detection_list.detections, sensors, object, detection_props);

   /** \result
   * Check that all detections remain associated
   */
   CHECK_EQUAL_TEXT(5, object.ndets, "The detection was not deassociated from the object as expected.");

   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(3, object.detids[2], "Detection is not associated.");
   CHECK_EQUAL_TEXT(4, object.detids[3], "Detection is not associated.");
   CHECK_EQUAL_TEXT(5, object.detids[4], "Detection is not associated.");

   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "Detection with ID 1 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "Detection with ID 2 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[2].object_track_id, "Detection with ID 3 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[3].object_track_id, "Detection with ID 4 was not associated as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[4].object_track_id, "Detection with ID 5 was not associated as expected.");

   CHECK_TRUE_TEXT( detection_props[0].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[2].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[3].f_ok_to_use, "The associated detection should be ok to use.");
   CHECK_TRUE_TEXT( detection_props[4].f_ok_to_use, "The associated detection should be ok to use.");
}

/** @}*/

/** \defgroup  f360_deassociate_suspected_ground_detections
 *  @{
 */

/** \brief
* This test group includes test of the function Deassociate_Suspected_Ground_Detections
 */
TEST_GROUP(f360_Deassociate_Suspected_Ground_Detections)
{	
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_T dets_raw[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up a default scenario where the preconditions are set to that all countermeasures are called.
    */
   TEST_SETUP()
   {
      // Set up an object with 2 detections
      // Set the first detection such that it has f_low_z_det_close_to_host = true and f_ambiguous_low_rcs_det = true
      // Set the second detection such that it has f_low_z_det_close_to_host = false and f_ambiguous_low_rcs_det = false
      object.movable_prob = 1.0F;
      object.f_moving = true;
      object.speed = 1.5F;
      object.ndets = 2;
      object.num_types_of_dets[0] = 1;
      object.num_types_of_dets[1] = 1;
      object.id = 1;
      
      object.detids[0] = 1;
      object.detids[1] = 2;

      detection_props[0].object_track_id = 1;
      detection_props[0].vcs_position.x = 14.0F;
      detection_props[0].vcs_position.y = 3.0F;
      detection_props[0].f_ok_to_use = true;
      detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      dets_raw[0].processed.vcs_position_z = 0.1F;
      dets_raw[0].raw.rcs = -11.0F;
      

      detection_props[1].object_track_id = 1;
      detection_props[1].vcs_position.x = 16.0F;
      detection_props[1].vcs_position.y = 5.0F;
      detection_props[1].f_ok_to_use = true;
      detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      dets_raw[1].processed.vcs_position_z = -0.3F;
      dets_raw[1].raw.rcs = -9.0F;
   }
};

/** \purpose
* Test that the suspected ground detection is deassociated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Deassociate_Suspected_Ground_Detection_And_Keep_Non_Ground_Detection)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is set up such that it qualifies for f_low_z_det_close_to_host = true and f_ambiguous_low_rcs_det = true
   * The second detection is set up such that it qualifies for f_low_z_det_close_to_host = false and f_ambiguous_low_rcs_det = false
   */

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is deassociated from the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(1, object.ndets, "The detection was not deassociated from the object as expected.");
   CHECK_EQUAL_TEXT(2, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(0, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(0, detection_props[0].object_track_id, "The detection should be deassociated from the object.");
   CHECK_EQUAL_TEXT(0, detection_props[0].f_ok_to_use, "The deassociated detection should not be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the suspected ground detection is not deassociated when it's longitudinally too far away
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Ground_Detection_Longitudinally_Far_Away)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is outside longitudinal threshold
   */
   detection_props[0].vcs_position.x = 15.1F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is deassociated from the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the suspected ground detection is not deassociated when it's laterally too far away
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Ground_Detection_Laterally_Far_Away)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is outside lateral threshold
   */
   detection_props[0].vcs_position.y = -4.1F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is deassociated from the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that a stationary detection inside the vcs zone is not deassociated when its z position is too high
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Close_Det_Z_Pos_High)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection's z position is too high
   */
   dets_raw[0].processed.vcs_position_z = -0.3F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is deassociated from the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the detections that are non suspected to be ground detections are still associated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Non_Suspected_Detections)
{
   /** \precond
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is set up such that it qualifies for f_low_z_det_close_to_host = true and f_ambiguous_low_rcs_det = false
   * The second detection is set up such that it qualifies for f_low_z_det_close_to_host = false and f_ambiguous_low_rcs_det = true
   */
   // First associated detection
   detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   dets_raw[0].raw.rcs = -9.0F;

   // Second associated detection
   detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   dets_raw[1].raw.rcs = -11.0F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is associated to the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the detections that are non suspected due to being moving to be ground detections are still associated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Non_Suspected_Detections_Motion_Status_Moving)
{
   /** \precond
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is set up such that it qualifies for f_low_z_det_close_to_host = true and f_ambiguous_low_rcs_det = false
   * The second detection is set up such that it qualifies for f_low_z_det_close_to_host = false and f_ambiguous_low_rcs_det = true
   */
   // First associated detection
   detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   dets_raw[0].raw.rcs = -9.0F;

   // Second associated detection
   detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   dets_raw[1].raw.rcs = -11.0F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is associated to the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that the detections that are non suspected, due to high enough RCS, to be ground detections are still associated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Keep_Non_Suspected_Detections_Not_Low_RCS)
{
   /** \precond
   * The test is set up with a object with 2 associated detections, that is slow moving (f_moving = true)
   * The first detection is set up such that it qualifies for f_low_z_det_close_to_host = true and f_ambiguous_low_rcs_det = false
   * The second detection is set up such that it qualifies for f_low_z_det_close_to_host = false and f_ambiguous_low_rcs_det = true
   */
   // First associated detection
   detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   dets_raw[0].raw.rcs = -9.0F;

   // Second associated detection
   detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   dets_raw[1].raw.rcs = -9.0F;

   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that the first detection is associated to the object
   * Check that the second detection is associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that when an object has f_moving = false, then no detections are deassociated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Check_Detections_Are_Not_Deassociated_from_Non_Moving_Object)
{
   /** \precond
   * The test is set up with a object with 2 associated detections, that is slow moving, yet has f_moving = false
   * The detection setup is same as the Test setup
   */
   object.f_moving = false;
   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that both detections are still associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}

/** \purpose
* Test that when an object has f_moving = true and moving at higher speed than threshold (2.5m/s), then no detections are deassociated
* \req
* NA
*/
TEST(f360_Deassociate_Suspected_Ground_Detections, Check_Detections_Are_Not_Deassociated_from_Fast_Moving_Object)
{
   /** \precond
   * The test is set up with a non-movable object with 2 associated detections, that is moving at speed higher than threshold f_moving = true
   * The detection setup is same as the Test setup
   */
   object.speed = 10.0F;
   /** \action
   * Call Deassociate_Suspected_Ground_Detections().
   */
   Deassociate_Suspected_Ground_Detections(dets_raw, object, detection_props);

   /** \result
   * Check that both detections are still associated to the object
   * Check that related detection and object properties are as expected
   */
   CHECK_EQUAL_TEXT(2, object.ndets, "The detections should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, object.detids[0], "Detection is not associated.");
   CHECK_EQUAL_TEXT(2, object.detids[1], "Detection is not associated.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[0], "The number of associated moving dets is not as expected.");
   CHECK_EQUAL_TEXT(1, object.num_types_of_dets[1], "The number of associated other dets is not as expected.");
   CHECK_EQUAL_TEXT(1, detection_props[0].object_track_id, "The detection should not be deassociated from the object.");
   CHECK_EQUAL_TEXT(1, detection_props[0].f_ok_to_use, "The deassociated detection should be ok to use.");
   CHECK_EQUAL_TEXT(1, detection_props[1].object_track_id, "The detection should be associated to the object.");
   CHECK_EQUAL_TEXT(1, detection_props[1].f_ok_to_use, "The associated detection should be ok to use.");
}
/** @}*/

/** \defgroup  f360_detection_association_countermeasures_Deassociate_Stationary_Range_Rate_Outlier_Dets
 *  @{
 */

/** \brief
* This test group includes test of the function Deassociate_Ambiguous_Detections
 */
TEST_GROUP(f360_detection_association_countermeasures_Deassociate_Ambiguous_Detections)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up a default scenario where the preconditions are set to that all countermeasures are called.
    */
   TEST_SETUP()
   {
      // Set up an object with 10 detections
      // 5 are marked as range_rate ambiguous
      // 5 are not marked as such
      object.ndets = 5U;
      object.id = 1;
      for(uint8_t i = 1U; i < 6U; i++) object.detids[i-1U] = i;
      for(uint8_t i = 0U; i < 6U; i++)
      {
         detection_props[i].object_track_id = 1;
         detection_props[i].f_rr_ambiguity = true;
         detection_props[i].f_ok_to_use = true;
      }
   }
};

/** \purpose
* Test that the detection with motion status ambiguous is de-associated when there are enough moving detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Ambiguous_Detections, Deassociate_Ambiguous_Range_Rate_Dets_Deassociated)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The expectation is that the detection with ambiguous range rate shall be de-associated
   */

   /** \action
   * Call Deassociate_Ambiguous_Detections().
   */
   Deassociate_Ambiguous_Detections(object, detection_props);

   /** \result
   * Check the ambiguous range rate detection was deassociated from the object
   */
   for(uint8_t i = 1U; i < 6; i++) 
   {
      CHECK_EQUAL_TEXT(0, detection_props[i-1U].object_track_id, "Detection was not associated as expected.");
      CHECK_TRUE_TEXT(detection_props[i-1U].f_ok_to_use, "The associated detection should be ok to use.");
   }
}

TEST(f360_detection_association_countermeasures_Deassociate_Ambiguous_Detections, Deassociate_Ambiguous_Range_Rate_Dets_Not_Deassociated)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The expectation is that the detection without ambiguous range rate shall not be de-associated
   */
   for(uint8_t i = 3U; i < 6U; i++) detection_props[i].f_rr_ambiguity = false;
   /** \action
   * Call Deassociate_Ambiguous_Detections().
   */
   Deassociate_Ambiguous_Detections(object, detection_props);

   /** \result
   * Check the ambiguous range rate detection was deassociated from the object
   */
   for(uint8_t i = 0U; i < 3U; i++)
   {
      CHECK_EQUAL_TEXT(0, detection_props[i].object_track_id, "Detection was not associated as expected.");
      CHECK_TRUE_TEXT(detection_props[i].f_ok_to_use, "The associated detection should be ok to use.");
   }
   for(uint8_t i = 3U; i < 6U; i++)
   {
      CHECK_EQUAL_TEXT(1, detection_props[i].object_track_id, "Detection was not associated as expected.");
      CHECK_TRUE_TEXT(detection_props[i].f_ok_to_use, "The associated detection should be ok to use.");
   }
}
/** @}*/
/** \defgroup  f360_detection_association_countermeasures_Deassociate_And_Count_Double_Bounce_Detections
 *  @{
 */

/** \brief
* This test group includes test of the function Deassociate_And_Count_Double_Bounce_Detections
 */
TEST_GROUP(f360_detection_association_countermeasures_Deassociate_And_Count_Double_Bounce_Detections)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up a default scenario where the preconditions are set to that all countermeasures are called.
    */
   TEST_SETUP()
   {
      // Set up an object with 3 detections
      // 2 are marked as double_bounce, one of them has motion status moving, one of them has ambigous
      // 1 is not marked as double bounce, has motion status moving
      object.ndets = 3U;
      object.id = 1;
      object.num_types_of_dets[0] = 2;
      object.num_types_of_dets[1] = 1;
      for(uint8_t i = 1U; i < 4U; i++) object.detids[i-1U] = i;
      for(uint8_t i = 0U; i < 3U; i++)
      {
         detection_props[i].object_track_id = 1;
         detection_props[i].f_ok_to_use = true;
      }
      detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[1].f_double_bounce = true;
      detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      detection_props[2].f_double_bounce = true;
      detection_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   }
};

/** \purpose
* Test that the double-bounce detections are de-associated from the object.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_And_Count_Double_Bounce_Detections, Deassociate_And_Count_Double_Bounce_Detections_)
{
   /** \precond
   * A default scenario has been set up in the TEST_GROUP
   * The expectation is that the detection with ambiguous range rate shall be de-associated
   */

   /** \action
   * Call Deassociate_Ambiguous_Detections().
   */
   Deassociate_And_Count_Double_Bounce_Detections(object, detection_props);

   /** \result
   * Check that the  double bounce detections are not assocated to the object, not double bounce is asociated and that all parameters are updated accordingly
   */

   CHECK_TRUE(detection_props[0].f_ok_to_use);
   CHECK_EQUAL(1, detection_props[0].object_track_id);
   CHECK_FALSE(detection_props[1].f_ok_to_use);
   CHECK_EQUAL(0, detection_props[1].object_track_id);
   CHECK_FALSE(detection_props[2].f_ok_to_use);
   CHECK_EQUAL(0, detection_props[2].object_track_id);

   CHECK_EQUAL(1U, object.ndets);
   CHECK_EQUAL(1, object.num_types_of_dets[0]);
   CHECK_EQUAL(0, object.num_types_of_dets[1]);
   CHECK_EQUAL(2U, object.num_db_dets);
   DOUBLES_EQUAL(1.94, object.historic_num_db_dets_with_forgetting_factor, 1e-4);

}
/** @}*/


/** @}*/
/** \defgroup  f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object
 *  @{
 */

/** \brief
* This test group includes test of the function Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object
 */
TEST_GROUP(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object)
{	
   // Declare common variables used within all tests in this test group.
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
    * Set up a default scenario where the preconditions are met for the countermeasure to be active.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      
      // Set up object that meets all preconditions
      object.reference_point = F360_REFERENCE_POINT_REAR;
      object.bbox.Set_Length(8.0F); // > 7.0m
      object.heading_rate = 0.05F; // < 0.1 rad/s
      object.speed = 15.0F; // > 10.0 m/s
      object.vcs_heading = Angle(F360_DEG2RAD(3.0F)); // < 5 degrees
      object.lat_buffer_zone_wid1 = 1.5F; // > 1.0m
      object.lat_buffer_zone_wid2 = 1.5F; // > 1.0m
      
      // Set up bounding box
      object.bbox.Set_Center(Point(10.0F, 0.0F));
      object.bbox.Set_Width(2.0F);
      object.bbox.Set_Orientation(Angle(0.0F));
      
      // Set up range rate error statistics
      object.filtered_hist_assoc_det_rr_err_mean = 1.0F;
      object.filtered_hist_assoc_det_rr_err_var = 0.25F; // std = 0.5
      calibrations.k_min_range_rate_error_threshold = 2.0F;
      
      // Set up 3 detections associated to the object
      object.ndets = 3U;
      object.detids[0] = 1U;
      object.detids[1] = 2U;
      object.detids[2] = 3U;
      
      // Detection 1: Good range rate, close laterally
      detection_props[0].object_track_id = 1U;
      detection_props[0].range_rate_predicted = 5.0F;
      detection_props[0].range_rate_dealiased = 5.2F; // Small error
      detection_props[0].vcs_position.x = 10.0F;
      detection_props[0].vcs_position.y = 0.5F; // Close to center
      detection_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[0].raw.sensor_id = 1U;
      
      // Detection 2: Good range rate, far laterally
      detection_props[1].object_track_id = 1U;
      detection_props[1].range_rate_predicted = 3.0F;
      detection_props[1].range_rate_dealiased = 3.1F; // Small error
      detection_props[1].vcs_position.x = 10.0F;
      detection_props[1].vcs_position.y = 2.5F; // Far laterally
      detection_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[1].raw.sensor_id = 1U;

      // Detection 3: Bad range rate, far laterally
      detection_props[2].object_track_id = 1U;
      detection_props[2].range_rate_predicted = 4.0F;
      detection_props[2].range_rate_dealiased = 8.5F; // Large error 
      detection_props[2].vcs_position.x = 10.0F;
      detection_props[2].vcs_position.y = 2.5F; // Far laterally
      detection_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      raw_detection_list.detections[2].raw.sensor_id = 1U;

      // Set up sensor velocity for the associated sensor
      sensors[0].variable.vcs_velocity.lateral = 0.01F; // Some small lateral velocity
      sensors[0].variable.vcs_velocity.longitudinal = 0.02F;
   }
};

/** \purpose
* Test that function deassociates range rate outliers that are laterally far from the object.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Deassociate_RR_Outliers_Laterally_Far)
{
   /** \precond
   * Default setup with one detection that has both bad range rate (4.5 error > 4.0 threshold) and is laterally far.
   */

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that only the detection with bad range rate and far laterally is deassociated.
   */
   CHECK_EQUAL_TEXT(2U, object.ndets, "Wrong number of detections remaining associated.");
   CHECK_EQUAL_TEXT(1U, object.detids[0], "First detection should remain associated.");
   CHECK_EQUAL_TEXT(2U, object.detids[1], "Second detection should remain associated.");
   
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(0U, detection_props[2].object_track_id, "Third detection should be deassociated.");
}

/** \purpose
* Test that function does not deassociate range rate outliers that are laterally close to the object.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Keep_RR_Outliers_Laterally_Close)
{
   /** \precond
   * Modify the third detection to be laterally close to the object center,
   * but increase the range rate error to ensure it triggers the lateral check.
   * Threshold = max(1.0 + 0.5, 2.0) * 2.0 = 4.0
   * Setting error to 4.5 to ensure it exceeds threshold.
   */
   detection_props[2].range_rate_dealiased = 8.5F; // Large error (4.5 difference)
   detection_props[2].vcs_position.y = 0.5F; // Close to center

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that all detections remain associated since the outlier is laterally close.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, object.detids[0], "First detection should remain associated.");
   CHECK_EQUAL_TEXT(2U, object.detids[1], "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(3U, object.detids[2], "Third detection should remain associated.");
   
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object has invalid reference point.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_Invalid_Ref_Point)
{
   /** \precond
   * Set object reference point to something other than rear, rear-left, or rear-right.
   */
   object.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to invalid reference point.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object is too small.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_Small_Object)
{
   /** \precond
   * Set object length to be below the minimum threshold.
   */
   object.bbox.Set_Length(6.0F); // < 7.0m

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to small object size.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object is too slow.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_Slow_Object)
{
   /** \precond
   * Set object speed to be below the minimum threshold.
   */
   object.speed = 8.0F; // < 10.0 m/s

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to low speed.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object has high heading rate (not moving straight).
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_High_Heading_Rate)
{
   /** \precond
   * Set object heading rate to be above the maximum threshold.
   */
   object.heading_rate = 0.15F; // > 0.1 rad/s

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to high heading rate.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object has large heading deviation.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_Large_Heading_Dev)
{
   /** \precond
   * Set object heading deviation to be above the maximum threshold.
   */
   object.vcs_heading = Angle(F360_DEG2RAD(7.0F)); // > 5 degrees

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to large heading deviation.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object has narrow lateral buffer zones.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_Narrow_Buffer_Zones)
{
   /** \precond
   * Set object lateral buffer zones to be below the threshold.
   */
   object.lat_buffer_zone_wid1 = 0.8F; // < 1.0m
   object.lat_buffer_zone_wid2 = 0.9F; // < 1.0m

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to narrow buffer zones.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that function does not run when object has one lateral buffer too narrow.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassoc_One_Narrow_Buffer_Zone)
{
   /** \precond
   * Set object lateral buffer zones to one of them to be below the threshold.
   */
   object.lat_buffer_zone_wid1 = 1.8F; // < 1.0m
   object.lat_buffer_zone_wid2 = 0.5F; // > 1.0m

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that no detections are deassociated due to one narrow buffer zones.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test range rate error threshold calculation with zero variance.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, RR_Threshold_Zero_Variance)
{
   /** \precond
   * Set object range rate error variance to zero.
   * Reset the detection position and make smaller range rate error to test threshold calculation.
   */
   object.filtered_hist_assoc_det_rr_err_var = 0.0F;
   
   // Reset the third detection and make smaller range rate error
   detection_props[2].range_rate_dealiased = 6.0F; // 2.0 difference instead of 4.5
   detection_props[2].vcs_position.y = 2.5F; // Reset to far laterally

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that detection is still deassociated since threshold uses minimum calibration value.
   * Threshold = max(1.0 + 0.0, 2.0) * 2.0 = 4.0, error = 2.0 < 4.0, so should not deassociate
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated with zero variance.");
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test that rear-left and rear-right reference points are valid.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Valid_Rear_Left_Right_Ref_Points)
{
   /** \precond
   * Test with rear-left reference point and reset detection properties.
   */
   object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   // Reset detection 3 to original bad range rate and far lateral position
   detection_props[2].range_rate_dealiased = 8.5F;
   detection_props[2].vcs_position.y = 2.5F;
   object.filtered_hist_assoc_det_rr_err_var = 0.25F; // Reset variance

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that the function runs and deassociates the outlier.
   */
   CHECK_EQUAL_TEXT(2U, object.ndets, "Function should run with rear-left reference point.");
   CHECK_EQUAL_TEXT(0U, detection_props[2].object_track_id, "Third detection should be deassociated.");
   
   // Reset for rear-right test
   object.ndets = 3U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;
   detection_props[2].object_track_id = 1U;
   
   object.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   
   // Reset detection 3 to original bad range rate and far lateral position
   detection_props[2].range_rate_dealiased = 8.5F;
   detection_props[2].vcs_position.y = 2.5F;
   object.filtered_hist_assoc_det_rr_err_var = 0.25F; // Reset variance

   /** \action
   * Call function again with rear-right reference point.
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that the function runs and deassociates the outlier.
   */
   CHECK_EQUAL_TEXT(2U, object.ndets, "Function should run with rear-right reference point.");
   CHECK_EQUAL_TEXT(0U, detection_props[2].object_track_id, "Third detection should be deassociated.");
}

/** \purpose
* Test that function handles objects with no detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Handle_Object_With_No_Detections)
{
   /** \precond
   * Set object to have no detections associated.
   */
   object.ndets = 0U;

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that function handles zero detections gracefully.
   */
   CHECK_EQUAL_TEXT(0U, object.ndets, "Object should still have zero detections.");
}

/** \purpose
* Test that no detections are deassociated when all pass both range rate and lateral checks.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, No_Deassociation_All_Good)
{
   /** \precond
   * Reset all detections to have good range rates.
   */
   detection_props[0].range_rate_dealiased = 5.1F; // Small error
   detection_props[1].range_rate_dealiased = 3.05F; // Small error  
   detection_props[2].range_rate_dealiased = 4.1F; // Small error
   detection_props[2].vcs_position.y = 2.5F; // Reset to far laterally
   object.filtered_hist_assoc_det_rr_err_var = 0.25F; // Reset variance

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that all detections remain associated and num_ok_dets == object.ndets path is taken.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated.");
   CHECK_EQUAL_TEXT(1U, object.detids[0], "First detection should remain associated.");
   CHECK_EQUAL_TEXT(2U, object.detids[1], "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(3U, object.detids[2], "Third detection should remain associated.");
   
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[1].object_track_id, "Second detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test multiple detections being deassociated.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Multiple_Detections_Deassociated)
{
   /** \precond
   * Set up multiple detections with bad range rates and far lateral positions.
   */
   detection_props[1].range_rate_dealiased = 8.0F; // Large error (5.0 difference)
   detection_props[1].vcs_position.y = 3.0F; // Far laterally
   detection_props[2].range_rate_dealiased = 9.0F; // Large error (5.0 difference)
   detection_props[2].vcs_position.y = 2.8F; // Far laterally
   object.filtered_hist_assoc_det_rr_err_var = 0.25F; // Reset variance

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that multiple detections are deassociated, leaving only the first one.
   */
   CHECK_EQUAL_TEXT(1U, object.ndets, "Only one detection should remain associated.");
   CHECK_EQUAL_TEXT(1U, object.detids[0], "First detection should remain associated.");
   
   CHECK_EQUAL_TEXT(1U, detection_props[0].object_track_id, "First detection should remain associated.");
   CHECK_EQUAL_TEXT(0U, detection_props[1].object_track_id, "Second detection should be deassociated.");
   CHECK_EQUAL_TEXT(0U, detection_props[2].object_track_id, "Third detection should be deassociated.");
}

/** \purpose
* Test range rate threshold calculation when mean+std is greater than minimum threshold.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, RR_Threshold_MeanStd_Greater_Than_Min)
{
   /** \precond
   * Set up object with high mean and variance so mean+std > k_min_range_rate_error_threshold.
   */
   object.filtered_hist_assoc_det_rr_err_mean = 3.0F;
   object.filtered_hist_assoc_det_rr_err_var = 4.0F; // std = 2.0
   calibrations.k_min_range_rate_error_threshold = 1.0F; // Lower than mean+std
   
   // Set detection with error that would pass min threshold but fail mean+std threshold
   // Threshold = max(3.0 + 2.0, 1.0) * 2.0 = 10.0
   detection_props[2].range_rate_dealiased = 12.0F; // Error = 8.0 < 10.0, should not deassociate
   detection_props[2].vcs_position.y = 2.5F; // Far laterally

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that detection is not deassociated due to higher threshold from mean+std.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "All detections should remain associated with higher mean+std threshold.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test the logical OR condition where only lateral_ok is true but range_rate is not ok.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, Lateral_OK_RR_Not_OK)
{
   /** \precond
   * Set up detection with bad range rate but good lateral position.
   * Reset calibrations and object parameters.
   */
   object.filtered_hist_assoc_det_rr_err_mean = 1.0F;
   object.filtered_hist_assoc_det_rr_err_var = 0.25F; // std = 0.5
   calibrations.k_min_range_rate_error_threshold = 2.0F;
   
   // Set detection with bad range rate but close laterally
   detection_props[2].range_rate_dealiased = 8.5F; // Large error (4.5 > 4.0 threshold)
   detection_props[2].vcs_position.y = 0.3F; // Close laterally

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that detection remains associated due to good lateral position (lateral_ok || rr_ok).
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "Detection should remain associated due to good lateral position.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** \purpose
* Test the logical OR condition where only range_rate is ok but lateral is not ok.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object, RR_OK_Lateral_Not_OK)
{
   /** \precond
   * Set up detection with good range rate but bad lateral position.
   */
   // Set detection with good range rate but far laterally
   detection_props[2].range_rate_dealiased = 4.1F; // Small error (0.1 < 4.0 threshold)
   detection_props[2].vcs_position.y = 3.5F; // Very far laterally

   /** \action
   * Call Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object().
   */
   Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(raw_detection_list, sensors, calibrations.k_min_range_rate_error_threshold, detection_props, object);

   /** \result
   * Check that detection remains associated due to good range rate (lateral_ok || rr_ok).
   * Lateral check should not even be performed since range rate is OK.
   */
   CHECK_EQUAL_TEXT(3U, object.ndets, "Detection should remain associated due to good range rate.");
   CHECK_EQUAL_TEXT(1U, detection_props[2].object_track_id, "Third detection should remain associated.");
}

/** @}*/

/** \defgroup  f360_detection_association_countermeasures_Calc_Percentage_Of_Assoc_Dets
 *  @{
 */

/** \brief
* This test group includes test of the function Calc_Percentage_Of_Assoc_Dets
 */
TEST_GROUP(f360_detection_association_countermeasures_Calc_Percentage_Of_Assoc_Dets)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object = {};

   TEST_SETUP()
   {
      /** Set up one object with:
       * - 2 associated detections
       * - 4 detections in the extender bbox
       * - previous associated percentage filtered equal to 0.8
       */
      object.num_dets_in_ext_bbox = 4U;
      object.ndets = 2U;
      object.assoc_dets_pct_filtered = 0.8F;
   }
};

/** \purpose
* Test that function properly calculates percentage of all associated detections.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Calc_Percentage_Of_Assoc_Dets, Calc_Percentage_Of_Assoc_Dets)
{
   /** \precond
   * Same as in setup
   */

   /** \action
   * Call Calc_Percentage_Of_Assoc_Dets().
   */
   Calc_Percentage_Of_Assoc_Dets(object);

   /** \result
   * Check the filtered percentage of associated detections
   */
   constexpr float32_t exp_assoc_dets_pct = 0.77;
   DOUBLES_EQUAL_TEXT(exp_assoc_dets_pct, object.assoc_dets_pct_filtered, 1E-5,  "Wrong filtered percentage of associated detections.");
}

/** \purpose
* Test what happens if object has 0 detections in it's extended bbox.
* \req
* NA
*/
TEST(f360_detection_association_countermeasures_Calc_Percentage_Of_Assoc_Dets, Calc_Percentage_Of_Assoc_Dets_Zero_Dets)
{
   /** \precond
   * Set object's:
   * - number of detections in it's extended bbox to 0
   * - number of associated detections to 0
   */
   object.num_dets_in_ext_bbox = 0U;
   object.ndets = 0U;

   /** \action
   * Call Calc_Percentage_Of_Assoc_Dets().
   */
   Calc_Percentage_Of_Assoc_Dets(object);

   /** \result
   * Check the filtered percentage of associated detections
   */
   constexpr float32_t exp_assoc_dets_pct = 0.82;
   DOUBLES_EQUAL_TEXT(exp_assoc_dets_pct, object.assoc_dets_pct_filtered, 1E-5,  "Wrong filtered percentage of associated detections.");
}
