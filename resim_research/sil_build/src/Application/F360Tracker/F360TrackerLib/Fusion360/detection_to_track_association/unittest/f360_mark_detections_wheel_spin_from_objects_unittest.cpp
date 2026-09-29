/** \file
   This file contains unit tests that verifies the content of f360_mark_detections_wheel_spin_from_objects.cpp
*/

#include "f360_mark_detections_wheel_spin_from_objects.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_math.h"
#include "f360_calibrations.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

using namespace f360_variant_A;

/** \defgroup  f360_mark_detections_wheel_spin_from_objects_main_function
 *  @{
 */

/** \brief
*  Tests under this test group will test the behavior of the function Mark_Detections_Wheel_Spin_From_Objects
*  implemented in f360_mark_detections_wheel_spin_from_objects.cpp
**/
TEST_GROUP(f360_mark_detections_wheel_spin_from_objects_main_function)
{
   F360_Object_Track_T object_track = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibrations = {};
   uint32_t ndets;

   /** \setup
   * Set:
   *    track max and min para/orth to some values
   *    object track filter type to CCA
   *    object VCS longitudinal velocity is 2 m/s
   *    object VCS lateral velocity is 0 m/s
   *    object speed to 2.0m/s (below calibrations.fast_moving_thresh such that the object is considered slow moving)
   *    object longitudinal position to 29.9m
   *    object track to moving
   *    object pointing to 0 (i.e. VCS and TCS aligned)
   *    object track id to 1
   *    object id for first 3 detections to 1
   *    object id for fourth detection to 0
   *    f_inside_gate for first 4 detections to true
   *    range rate dealiased for first 4 detections to some value such that they should be marked as wheel spin detections.
   *    position of first 4 detections spread out on the object
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_track.vcs_velocity.longitudinal = 2.0F;
      object_track.vcs_velocity.lateral = 0.0F;
      object_track.speed = 2.0F;
      object_track.vcs_position.x = 29.9F;
      object_track.vcs_position.y = 1.0F;
      object_track.f_moving = true;

      object_track.ndets = 2;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.Update_Bbox_Size(4.0F, 2.0F);

      uint32_t sensor_idx = 0;
      sensors[sensor_idx].variable.vcs_velocity.longitudinal = 1.0F;
      sensors[sensor_idx].variable.vcs_velocity.lateral = 0.0F;

      object_track.id = 1;
      ndets = 4U;
      det_props[0].object_track_id = 1;
      det_props[1].object_track_id = 1;
      det_props[2].object_track_id = 0;
      det_props[3].object_track_id = 0;
      det_props[0].f_inside_gate = true;
      det_props[1].f_inside_gate = true;
      det_props[2].f_inside_gate = true;
      det_props[3].f_inside_gate = true;

      dets[0].raw.sensor_id = 1U;
      dets[1].raw.sensor_id = 1U;
      dets[2].raw.sensor_id = 1U;
      dets[3].raw.sensor_id = 1U;
      det_props[0].f_ok_to_use = true;
      det_props[1].f_ok_to_use = true;
      det_props[2].f_ok_to_use = true;
      det_props[3].f_ok_to_use = true;
      det_props[0].range_rate_dealiased = 10.0F;
      det_props[1].range_rate_dealiased = -10.0F;
      det_props[2].range_rate_dealiased = -3.0F;
      det_props[3].range_rate_dealiased = 8.0F;
      det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

      det_props[0].f_low_az_conf_det = false;
      det_props[1].f_low_az_conf_det = false;
      det_props[2].f_low_az_conf_det = false;
      det_props[3].f_low_az_conf_det = false;

      det_props[0].vcs_position.x = object_track.vcs_position.x - 0.1F;
      det_props[0].vcs_position.y = object_track.vcs_position.y + 0.1F;
      dets[0].processed.cos_vcs_az = 1.0F;
      dets[0].processed.sin_vcs_az = 0.0F;

      det_props[1].vcs_position.x = object_track.vcs_position.x + 0.1F;
      det_props[1].vcs_position.y = object_track.vcs_position.y - 0.1F;
      dets[1].processed.cos_vcs_az = 1.0F;
      dets[1].processed.sin_vcs_az = 0.0F;

      det_props[2].vcs_position.x = object_track.vcs_position.x - 1.0F;
      det_props[2].vcs_position.y = object_track.vcs_position.y + 1.0F;
      dets[2].processed.cos_vcs_az = 1.0F;
      dets[2].processed.sin_vcs_az = 0.0F;

      det_props[3].vcs_position.x = object_track.vcs_position.x + 1.0F;
      det_props[3].vcs_position.y = object_track.vcs_position.y + 1.0F;
      dets[3].processed.cos_vcs_az = 1.0F;
      dets[3].processed.sin_vcs_az = 0.0F;
   }
};

/**
*\purpose  Test that the main function that marks detections as wheel spin detections correctly marks detections as wheel spin
*\         detections when detection properties does not coincide with the object's properties in terms of position and range rate.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Detection_and_Object_Disagreement_in_Range_Rate)
{
   /** \precond
    * An object and a set of 4 detections with properties that suggest that the detections
    * are wheel spin detections have been set up in the test group.
   **/

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were marked as wheel spin detections.
   **/
   CHECK_TRUE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was not marked as a wheel spin detection from an object when it should have been.");
   CHECK_TRUE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was not marked as a wheel spin detection from an object when it should have been.");
   CHECK_TRUE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was not marked as a wheel spin detection from an object when it should have been.");
   CHECK_TRUE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was not marked as a wheel spin detection from an object when it should have been.");
}

/**
*\purpose  Test that the main function that marks detections as wheel spin detections works as intended when the object has its moving flag set to false
*\req    NA
*/

TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Not_Moving)
{
   /** \precond
    * An object and a set of 4 detections with properties that suggest that the detections
    *    are wheel spin detections have been set up in the test group.
    * Set object f_moving flag to false
   **/

   object_track.f_moving = false;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were not marked as wheel spin detections.
   **/
   CHECK_FALSE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when the object was not moving.");
   CHECK_FALSE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when the object was not moving.");
   CHECK_FALSE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when the object was not moving.");
   CHECK_FALSE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when the object was not moving.");

}

/**
*\purpose  Test that the main function that marks detections as wheel spin detections works as intended when the object is far away from host longitudinally.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Object_Far_Away)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Object longitudinal vcs position set to 80.1m.
   **/
   object_track.vcs_position.x = 80.1F;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were not marked as wheel spin detections.
   **/
   CHECK_FALSE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was wrongly marked as a wheel spin detection from an object when object was outside longitudinal "
                   "position threshold for checking wheel spin detections.");
   CHECK_FALSE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when object was outside longitudinal "
                    "position threshold for checking wheel spin detections.")
   CHECK_FALSE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when object was outside longitudinal "
                    "position threshold for checking wheel spin detections.");
   CHECK_FALSE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when object was outside longitudinal "
                    "position threshold for checking wheel spin detections.");
}

/**
*\purpose  Test that the main function that marks detections as wheel spin detections works as intended when the detections are not flagged as inside gate
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Detection_Not_Inside_Gate)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * The flag f_inside_gate is set to false for all 4 detections.
   **/
   det_props[0].f_inside_gate = false;
   det_props[1].f_inside_gate = false;
   det_props[2].f_inside_gate = false;
   det_props[3].f_inside_gate = false;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were not marked as wheel spin detections.
   **/
   CHECK_FALSE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was wrongly marked as a wheel spin detection from an object when detection was not flagged as inside gate.");
   CHECK_FALSE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was wrongly marked as a wheel spin detection from an object when detection was not flagged as inside gate.");
   CHECK_FALSE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was wrongly marked as a wheel spin detection from an object when detection was not flagged as inside gate.");
   CHECK_FALSE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                   "A detection was wrongly marked as a wheel spin detection from an object when detection was not flagged as inside gate.");
}


/**
*\purpose  Test that the main function that marks detections as wheel spin detections does not mark detections as wheel spin when the detections are far from the object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Detection_Not_On_Object)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * The position of the detections have been moved such that they are 1m away from object edges.
   **/

   det_props[0].vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F;
   det_props[0].vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width()*0.5F + 1.0F;

   det_props[1].vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F - 1.0F;
   det_props[1].vcs_position.y = object_track.vcs_position.y - object_track.bbox.Get_Width()*0.5F;

   det_props[2].vcs_position.x = object_track.vcs_position.x + object_track.bbox.Get_Length()*0.5F;
   det_props[2].vcs_position.y = object_track.vcs_position.y - object_track.bbox.Get_Width()*0.5F - 1.0F;

   det_props[3].vcs_position.x = object_track.vcs_position.x + object_track.bbox.Get_Length()*0.5F;
   det_props[3].vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width()*0.5F + 1.0F;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were not marked as wheel spin detections.
   **/
   CHECK_FALSE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when detection was outside of bounding box.");
   CHECK_FALSE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when detection was outside of bounding box.");
   CHECK_FALSE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when detection was outside of bounding box.");
   CHECK_FALSE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
                    "A detection was wrongly marked as a wheel spin detection from an object when detection was outside of bounding box.");

}

/**
*\purpose  Test that the main function that marks detections as wheel spin detections works as intended when the detections are inside gate and associated to another object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Mark_Detections_Wheel_Spin_From_Objects_Detection_Inside_Gate_Not_Associated)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * The field object_track_id is set to 10 for all 4 detections.
   **/
   det_props[0].object_track_id = 10;
   det_props[1].object_track_id = 10;
   det_props[2].object_track_id = 10;
   det_props[3].object_track_id = 10;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that the detections were not marked as wheel spin detections.
   **/
   CHECK_FALSE_TEXT(det_props[0].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
      "A detection was wrongly marked as a wheel spin detection from an object when detection was flagged as inside gate but not associated.");
   CHECK_FALSE_TEXT(det_props[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
      "A detection was wrongly marked as a wheel spin detection from an object when detection was flagged as inside gate but not associated.");
   CHECK_FALSE_TEXT(det_props[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
      "A detection was wrongly marked as a wheel spin detection from an object when detection was flagged as inside gate but not associated.");
   CHECK_FALSE_TEXT(det_props[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_OBJECT,
      "A detection was wrongly marked as a wheel spin detection from an object when detection was flagged as inside gate but not associated.");
}

/**
*\purpose  Test that the main function associates wheel spin detections to the object when detection position matches 
*\         object position, but the range rate prediction doesn't match actual range rate of the detection
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Associate_Wheel_Spin_From_Objects_Detection_When_Disagreement_in_Range_Rate_Detected_CTCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   * are wheel spin detections have been set up in the test group.
   * Set object filter type to CTCA
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that two additional moving detections were associated to the object
   * Object already has two associated detections and it should associate two more additional wheelspin detection
   **/
   const uint32_t expected_number_of_associated_detections = 4U;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should have been detected as wheel spin detection was not associated to the object");
}

/**
*\purpose  Test that the main function does NOT associate wheel spin detections to the object when detection position matches
*          object position, but the range rate prediction doesn't match actual range rate of the detection when the detections are also 
*          marked as not ok to use. 
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_From_Objects_Detection_When_Detections_Not_Ok_To_Use)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   * are wheel spin detections have been set up in the test group.
   * The detections are marked as not ok to use.
   * Set object filter type to CTCA
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   det_props[0].f_ok_to_use = false;
   det_props[1].f_ok_to_use = false;
   det_props[2].f_ok_to_use = false;
   det_props[3].f_ok_to_use = false;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Test that no additional detections were associated to the object
   * Object already has two associated detections and it should not associate any more additional wheelspin detections
   **/
   const uint32_t expected_number_of_associated_detections = 2U;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Some detection(s) that was detected as wheel spin detection was incorreclty associated to the object despite being not ok to use.");
}

/**
*\purpose  Test that the main function does NOT associate wheel spin detections to the object when detection position matches
*          object position, but the range rate prediction doesn't match actual range rate of the detection when the detections are also 
*          marked as f_low_az_conf_det = true. 
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_From_Objects_Detection_When_Detections_Are_Flagged_As_f_low_az_conf_det)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   * are wheel spin detections have been set up in the test group.
   * The detections are marked as f_low_az_conf_det = true
   * Set object filter type to CTCA
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   det_props[0].f_low_az_conf_det = true;
   det_props[1].f_low_az_conf_det = true;
   det_props[2].f_low_az_conf_det = true;
   det_props[3].f_low_az_conf_det = true;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Test that no additional detections were associated to the object
   * Object already has two associated detections and it should not associate any more additional wheelspin detections
   **/
   const uint32_t expected_number_of_associated_detections = 2U;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Some detection(s) that was detected as wheel spin detection was incorreclty associated to the object despite being not ok to use.");
}

/**
*\purpose  Test that the main function doesn't associate the wheel spin detections when the object is not moving
*\req    NA
*/

TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_For_CTCA_Objects_Not_Moving)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object f_moving flag to false
   * Set object filter type to CTCA
   * Total number of associated detections should not change, set expected value
   **/
   object_track.f_moving = false;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;


   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that additional detections were not associated to the object
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should not have been detected as wheel spin detection was associated to the object");
}

/**
*\purpose  Test that the main function doesn't associate the wheel spin detections when their motion status different than moving
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_When_Motion_Status_Different_Than_Moving_CTCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CTCA
   * Set status of all detections is different than moving
   * Set expected number of associated detections
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that two additional detections were not associated to the object due to their moving status
   * Object already has two associated detections so the total number of detections should not change
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should not have been detected as wheel spin detection was associated to the object");
}

/**
*\purpose  Test that the main function associates one wheel spin detection with moving status
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Associate_One_Wheel_Spin_Detection_When_Its_Motion_Status_Moving_CTCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CTCA
   * Set status of one detection to moving, the remaining detections have status different than moving
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that one wheel spin detection with motion status moving was associated to the object
   * Object initialy has two associated detections, so the total number of detections after executing function should be 3
   **/
   const uint32_t expected_number_of_associated_detections = 3U;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Number of detections associated to the object doesn't match expectation.");
   const uint32_t expected_associated_object_id = 1U;
   CHECK_EQUAL_TEXT(expected_associated_object_id, det_props[3].object_track_id,
      "Moving wheel spin detection was not associated to the object as expected");
}

/**
*\purpose  Test that the main function doesn't associate detections that are already associated to the object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_When_Already_Associated_CTCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CTCA
   * Associate all 4 detections to the object
   * Set expected number of associated detections - it should remain the same
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.ndets = 4;
   det_props[0].object_track_id = 1;
   det_props[1].object_track_id = 1;
   det_props[2].object_track_id = 1;
   det_props[3].object_track_id = 1;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * All detections are already associated to the object, they should not be associated again
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Number of detections associated to the object doesn't match expectation.");
}

/**
*\purpose  Test that the main function associates wheel spin detections to the object when detection position matches 
*\         object position, but the range rate prediction doesn't match actual range rate of the detection
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Associate_Wheel_Spin_From_Objects_Detection_When_Disagreement_in_Range_Rate_Detected_fast_moving_CCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   * are wheel spin detections have been set up in the test group.
   * Set object filter type to CCA and increse object speed to above calibrations.fast_moving_thresh
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.speed = calibrations.fast_moving_thresh + 0.1F;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that two additional moving detections were associated to the object
   * Object already has two associated detections and it should associate two more additional wheelspin detection
   **/
   const uint32_t expected_number_of_associated_detections = 4;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should have been detected as wheel spin detection was not associated to the object");
}

/**
*\purpose  Test that the main function does NOT associate wheel spin detections to the object when detection position matches
*          object position, but the range rate prediction doesn't match actual range rate of the detection when the detections are also 
*          marked as not ok to use. 
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_From_Objects_Detection_When_Detections_Not_Ok_To_Use_CCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   * are wheel spin detections have been set up in the test group.
   * The detections are marked as not ok to use.
   * Set object filter type to CCA
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   det_props[0].f_ok_to_use = false;
   det_props[1].f_ok_to_use = false;
   det_props[2].f_ok_to_use = false;
   det_props[3].f_ok_to_use = false;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Test that no additional detections were associated to the object
   * Object already has two associated detections and it should not associate any more additional wheelspin detections
   **/
   const uint32_t expected_number_of_associated_detections = 2;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Some detection(s) that was detected as wheel spin detection was incorreclty associated to the object despite being not ok to use.");
}


/**
*\purpose  Test that the main function doesn't associate the wheel spin detections when the object is not moving
*\req    NA
*/

TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_For_CCA_Objects_Not_Moving)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object f_moving flag to false
   * Set object filter type to CCA
   * Total number of associated detections should not change, set expected value
   **/
   object_track.f_moving = false;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;


   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that additional detections were not associated to the object
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should not have been detected as wheel spin detection was associated to the object");
}

/**
*\purpose  Test that the main function doesn't associate the wheel spin detections when their motion status different than moving
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_When_Motion_Status_Different_Than_Moving_CCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CCA
   * Set status of all detections is different than moving
   * Set expected number of associated detections
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that two additional detections were not associated to the object due to their moving status
   * Object already has two associated detections so the total number of detections should not change
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "A detection that should not have been detected as wheel spin detection was associated to the object");
}

/**
*\purpose  Test that the main function associates one wheel spin detection with moving status
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Associate_One_Wheel_Spin_Detection_When_Its_Motion_Status_Moving_CCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CCA
   * Set status of one detection to moving, the remaining detections have status different than moving
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.speed = calibrations.fast_moving_thresh + 0.1F;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * Check that one wheel spin detection with motion status moving was associated to the object
   * Object initialy has two associated detections, so the total number of detections after executing function should be 3
   **/
   const uint32_t expected_number_of_associated_detections = 3U;
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Number of detections associated to the object doesn't match expectation.");
   const uint32_t expected_associated_object_id = 1U;
   CHECK_EQUAL_TEXT(expected_associated_object_id, det_props[3].object_track_id,
      "Moving wheel spin detection was not associated to the object as expected");
}

/**
*\purpose  Test that the main function doesn't associate detections that are already associated to the object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_main_function, Do_Not_Associate_Wheel_Spin_Detections_When_Already_Associated_CCA)
{
   /** \precond
   * An object and a set of 4 detections with properties that suggest that the detections
   *    are wheel spin detections have been set up in the test group.
   * Set object filter type to CCA
   * Associate all 4 detections to the object
   * Set expected number of associated detections - it should remain the same
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_track.ndets = 4;
   det_props[0].object_track_id = 1;
   det_props[1].object_track_id = 1;
   det_props[2].object_track_id = 1;
   det_props[3].object_track_id = 1;
   const uint32_t expected_number_of_associated_detections = object_track.ndets;

   /** \action
   * Call function
   **/
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, ndets, sensors, calibrations, object_track, det_props);

   /** \result
   * All detections are already associated to the object, they should not be associated again
   **/
   CHECK_EQUAL_TEXT(expected_number_of_associated_detections, object_track.ndets,
      "Number of detections associated to the object doesn't match expectation.");
}


// ------------------------------- END CCA -------------------------------------------



/** @}*/

/** \defgroup  f360_mark_detections_wheel_spin_from_objects_determine_bounding_box_extension
 *  @{
 */

/** \brief
*  Tests under this test group will test the behavior of the function Determine_Bounding_Box_Extension
*  implemented in f360_mark_detections_wheel_spin_from_objects.cpp
**/
TEST_GROUP(f360_mark_detections_wheel_spin_from_objects_determine_bounding_box_extension)
{
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibrations = {};
   BoundingBox box = {};
   const float32_t tolerance = 0.0001F;

   /** \setup
   * Set up an object with a bounding box of 4x2 m
   * Set oncoming flag to false
   * Set track vcs heading to 0
   * Set track speed to something greater than 3.0, for track to be considered fast moving.
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      object_track.f_oncoming = false;
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_heading = Angle{ 0.0F };
      object_track.speed = 3.01F;
      object_track.Update_Bbox_Size(4.0F, 2.0F);
      box = object_track.bbox;
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Test that bounding box extension function returns correct values for a target
*          that has a small longitudinal position, a small heading angle, is not oncoming and is fast moving.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_determine_bounding_box_extension, Determine_Bounding_Box_Extension_Close_Fast_Moving_Zero_Heading)
{
   /** \precond
    * An object with oncoming flag set to false, small longitudinal vcs position,
    * zero vcs heading and speed for track to be considered fast moving has been set up in test group.
    * Expected data is set.
   **/
   // Bounding box is expected to be extended. Since oncoming is set to false,
   // extension is 0.3m in every direction.
   const BoundingBox exp_box{ Point{-2.7F, -1.3F}, Point{2.7F, 1.3F} };

   /** \action
   * Call function
   **/
   Determine_Bounding_Box_Extension(object_track, calibrations, box);

   /** \result
   * Check that the bounding box corners coincide with the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().x, box.Get_Corners().Rear_Left().x, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().y, box.Get_Corners().Rear_Left().y, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().x, box.Get_Corners().Rear_Right().x, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().y, box.Get_Corners().Rear_Right().y, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().x, box.Get_Corners().Front_Left().x, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().y, box.Get_Corners().Front_Left().y, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().x, box.Get_Corners().Front_Right().x, tolerance, "Front right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().y, box.Get_Corners().Front_Right().y, tolerance, "Front right corner was not within threshold of expected data.");
}

/**
*\purpose  Test that bounding box extension function returns correct values for a target
*\purpose  that has a small longitudinal position, a small heading angle, is not oncoming and is not fast moving.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_determine_bounding_box_extension, Determine_Bounding_Box_Extension_Slow_Moving_Target)
{
   /** \precond
    *    An object with oncoming flag set to false, small longitudinal vcs position and zero vcs heading has been set up in test group.
    *    track speed is set to something smaller than 3.0, for track to be considered slow moving.
    *    Expected data is set.
   **/
   object_track.speed = 2.99F;

   // Bounding box is not expected to be extended because of object speed < 3 m/s
   const BoundingBox exp_box = object_track.bbox;

   /** \action
   * Call function
   **/
   Determine_Bounding_Box_Extension(object_track, calibrations, box);

   /** \result
   * Check that the bounding box corners coincide with the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().x, box.Get_Corners().Rear_Left().x, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().y, box.Get_Corners().Rear_Left().y, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().x, box.Get_Corners().Rear_Right().x, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().y, box.Get_Corners().Rear_Right().y, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().x, box.Get_Corners().Front_Left().x, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().y, box.Get_Corners().Front_Left().y, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().x, box.Get_Corners().Front_Right().x, tolerance, "Front right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().y, box.Get_Corners().Front_Right().y, tolerance, "Front right corner was not within threshold of expected data.");
}

/**
*\purpose  Test that bounding box extension function returns correct values for an oncoming track.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_determine_bounding_box_extension, Determine_Bounding_Box_Extension_Oncoming_Track)
{
   /** \precond
   *     An object with a speed for track to be considered fast moving has been set up in test group.
   *     track vcs heading is set to -3.316 (~ -190 degrees)
   *     f_oncoming flag is set to true
   **/
   object_track.f_oncoming = true;

   const BoundingBox exp_box{ Point{-2.7F, -1.4F}, Point{2.7F, 1.4F} };

   /** \action
   * Call function
   **/
   Determine_Bounding_Box_Extension(object_track, calibrations, box);

   /** \result
   * Check that the bounding box corners coincide with the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().x, box.Get_Corners().Rear_Left().x, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Left().y, box.Get_Corners().Rear_Left().y, tolerance, "Rear left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().x, box.Get_Corners().Rear_Right().x, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Rear_Right().y, box.Get_Corners().Rear_Right().y, tolerance, "Rear right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().x, box.Get_Corners().Front_Left().x, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Left().y, box.Get_Corners().Front_Left().y, tolerance, "Front left corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().x, box.Get_Corners().Front_Right().x, tolerance, "Front right corner was not within threshold of expected data.");
   DOUBLES_EQUAL_TEXT(exp_box.Get_Corners().Front_Right().y, box.Get_Corners().Front_Right().y, tolerance, "Front right corner was not within threshold of expected data.");
}
/** @}*/

/** \defgroup  f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object
 *  @{
 */

/** \brief
*  Tests under this test group will test the behavior of the function Is_Det_Wheel_Spin_From_Object
*  implemented in f360_mark_detections_wheel_spin_from_objects.cpp
**/
TEST_GROUP(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object)
{
   // Instantiate common variables used for all tests in this test group
   F360_Object_Track_T object_track = {};
   rspp_variant_A::RSPP_Detection_T det = {};
   F360_Detection_Props_T det_prop = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibrations = {};
   BoundingBox box = {};

   /** \setup
   * Set:
   *  track with len1, len2, wid1 and wid2 to some values
   *  detection sensor ID to 1
   *  sensor velocity components to 0
   *  sensor position components to 0
   *  track filter type to CTCA
   *  track pointing set to 0
   *  track x and y component of VCS velocity to 1
   *  track speed to correct value
   *  track curvature to 0
   *  detection range rate to sqrt(2).
   *  detection position on object rear right corner
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      det.raw.sensor_id = 1;

      sensors[det.raw.sensor_id - 1].variable.vcs_velocity.longitudinal = 0.0F;
      sensors[det.raw.sensor_id - 1].variable.vcs_velocity.lateral = 0.0F;
      sensors[det.raw.sensor_id - 1].variable.vcs_velocity.longitudinal = 0.0F;
      sensors[det.raw.sensor_id - 1].variable.vcs_velocity.lateral = 0.0F;

      object_track.Update_Bbox_Size(4.0F, 2.0F);
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track.Set_Bbox_Orientation(Angle{ 0.0F });
      object_track.vcs_velocity.longitudinal = 1.0F;
      object_track.vcs_velocity.lateral = 1.0F;
      object_track.speed = sqrtf(object_track.vcs_velocity.longitudinal * object_track.vcs_velocity.longitudinal + object_track.vcs_velocity.lateral * object_track.vcs_velocity.lateral);
      object_track.curvature = 0.0F;

      det_prop.range_rate_dealiased = 1.41F; // Sqrt(2), sqrt(x_vel^2 + y_vel^2)
      det_prop.vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F;
      det_prop.vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width()*0.5F;
      det.processed.cos_vcs_az = 1.0F;
      det.processed.sin_vcs_az = 0.0F;

      box = object_track.bbox;
   }

};

/**
*\purpose  Check if detection is not identified as wheel spin detection when the properties of the detection
*          coincides with the object's properties for a CTCA object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Detection_And_Object_Properties_Coincide)
{
   /** \precond
    * A CTCA object with zero pointing angle and zero curvature has been set up in the test group.
    * A detection with position and range rate that coincide with object position and velocity has been set up in the test group.
   **/

   /** \action
   * Call function
   **/
	bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified as a wheel spin detection from an object.")
}

/**
*\purpose  Check if detection is not identified as wheel spin detection when the properties of the detection
*          coincides with the object's properties for a CCV object
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_CCV_Track)
{
   /** \precond
    * A object with zero pointing angle has been set up in the test group.
    * A detection with position and range rate that coincide with object position and velocity has been set up in the test group.
    * Object track filter type is set to CCV
   **/
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified as a wheel spin detection from an object.")
}

/**
*\purpose  Check if detection is not identified as wheel spin detection when the properties of the detection
*          coincides with the object's properties and target and sensor is approaching each other
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Approaching_Target)
{
   /** \precond
   *     A CTCA object has been set up in the test group.
   *     Sensor velocity components is set to 1m/s (towards target)
   *     pointing is set to -180 degrees
   *     Track x and y components of vcs velocity are set to -1.0F
   *     Track speed is set to correct value
   *     Detection range rate is set to -2*sqrt(2)
   *     Bounding box corners updated to reflect that object is rotated 180 degrees
   *     Detection position is set on object front left corner
   **/

   sensors[det.raw.sensor_id -1].variable.vcs_velocity.longitudinal = 1.0F;
   sensors[det.raw.sensor_id - 1].variable.vcs_velocity.lateral = 1.0F;

   object_track.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(-180.0F) });
   object_track.vcs_velocity.longitudinal = -1.0F;
   object_track.vcs_velocity.lateral = -1.0F;
   object_track.speed = F360_Sqrtf(object_track.vcs_velocity.longitudinal * object_track.vcs_velocity.longitudinal + object_track.vcs_velocity.lateral * object_track.vcs_velocity.lateral);

   det_prop.range_rate_dealiased = -2.0F*1.41F;

   // Detection position in VCS
   det_prop.vcs_position = box.Get_Corners().Front_Left();

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified as a wheel spin detection from an object.");
}



/**
*\purpose  Test that function identifies detections as wheel spin detections when the detection has an unreasonable range rate.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Approaching_Target_Wrong_Range_Rate)
{
   /** \precond
   *     A CTCA object has been set up in the test group.
   *     Sensor velocity components is set to 1m/s (towards target)
   *     pointing is set to -180 degrees
   *     Track x and y components of vcs velocity are set to -1.0F
   *     Track speed is set to correct value
   *     Detection range rate is set to +2*sqrt(2) (correct value is minus 2*sqrt(2))
   *     Bounding box corners updated to reflect that object is rotated 180 degrees
   *     Detection position is set on object front left corner
   **/

   sensors[det.raw.sensor_id -1].variable.vcs_velocity.longitudinal = 1.0F;
   sensors[det.raw.sensor_id - 1].variable.vcs_velocity.lateral = 1.0F;

   object_track.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(-180.0F) });
   object_track.vcs_velocity.longitudinal = -1.0F;
   object_track.vcs_velocity.lateral = -1.0F;
   object_track.speed = sqrtf(object_track.vcs_velocity.longitudinal * object_track.vcs_velocity.longitudinal + object_track.vcs_velocity.lateral * object_track.vcs_velocity.lateral);

   det_prop.range_rate_dealiased = 2.0F*1.41F;

   // Detection position in VCS
   det_prop.vcs_position = box.Get_Corners().Front_Left();

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was identified as a wheel spin detection of type object
   **/
   CHECK_TRUE_TEXT(f_is_det_wheel_spin,
                   "A detection was not identified as wheel spin detection from an object when it should have been.");
}

/**
*\purpose  Test that function does not identify a detection as a wheel spin for a detection
*          that is outside the bounding box of the object, even though other properties suggest that it is a wheel spin detection.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Detection_Outside_Bounding_Box_Right_Edge)
{
   /** \precond
   *     A CTCA object with zero pointing angle and zero curvature has been set up in the test group.
   *     Detection range rate is set to something unreasonable
   *     Detection x position is set to be on rear edge
   *     Detection y position is set to be 2m from right edge
   **/

   det_prop.range_rate_dealiased = -10.0F;
   det_prop.vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F;
   det_prop.vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width()*0.5F + 2.0F;
   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified as a wheel spin detection from an object when detection was outside of bounding box.");
}


/**
*\purpose  Check if detection is not identified as wheel spin if the detection
*          is outside the bounding box of the object, even though other properties suggest that it is a wheel spin detection.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Detection_Outside_Bounding_Box_Left_Edge)
{
   /** \precond
   *     A CTCA object with zero pointing angle and zero curvature has been set up in the test group.
   *     Detection range rate is set to something unreasonable
   *     Detection x position is set to be on rear edge
   *     Detection y position is set to be 2m from left edge
   **/

   det_prop.range_rate_dealiased = -10.0F;
   det_prop.vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F;
   det_prop.vcs_position.y = object_track.vcs_position.y - object_track.bbox.Get_Width()*0.5F - 2.0F;

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified to be a wheel spin detection from an object when detection was outside of bounding box.");
}

/**
*\purpose  Check if detection is not identified as wheel spin if the detection
*          is outside the bounding box of the object, even though other properties suggest that it is a wheel spin detection.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Detection_Outside_Bounding_Box_Rear_Edge)
{
   /** \precond
   *     A CTCA object with zero pointing angle and zero curvature has been set up in the test group.
   *     Detection range rate is set to something unreasonable
   *     Detection x position is set to be 2m from rear edge
   *     Detection y position is set to be on right edge
   **/

   det_prop.range_rate_dealiased = -10.0F;
   det_prop.vcs_position.x = object_track.vcs_position.x - object_track.bbox.Get_Length()*0.5F - 2.0F;
   det_prop.vcs_position.y = object_track.vcs_position.y + object_track.bbox.Get_Width()*0.5F;

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin detection of type object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified to be a wheel spin detection from an object when detection was outside of bounding box.");
}


/**
*\purpose  Check if detection is not identified as wheel spin if the detection
*          is outside the bounding box of the object, even though other properties suggest that it is a wheel spin detection.
*\req    NA
*/
TEST(f360_mark_detections_wheel_spin_from_objects_is_det_wheel_spin_from_object, Is_Det_Wheel_Spin_From_Object_Detection_Outside_Bounding_Box_Front_Edge)
{
   /** \precond
   *     A CTCA object with zero pointing angle and zero curvature has been set up in the test group.
   *     Detection range rate is set to something unreasonable
   *     Detection x position is set to be 2m from front edge
   *     Detection y position is set to be on left edge
   **/

   det_prop.range_rate_dealiased = -10.0F;
   det_prop.vcs_position.x = object_track.vcs_position.x + object_track.bbox.Get_Length()*0.5F + 2.0F;
   det_prop.vcs_position.y = object_track.vcs_position.y - object_track.bbox.Get_Width()*0.5F;

   /** \action
   * Call function
   **/
   bool f_is_det_wheel_spin = Is_Det_Wheel_Spin_From_Object(object_track, det, sensors, calibrations, box, det_prop);

   /** \result
   * Check that the detection was not identified as a wheel spin from the object
   **/
   CHECK_FALSE_TEXT(f_is_det_wheel_spin,
                    "A detection was wrongly identified as a wheel spin detection from an object when detection was outside of bounding box.");
}

/** @}*/
