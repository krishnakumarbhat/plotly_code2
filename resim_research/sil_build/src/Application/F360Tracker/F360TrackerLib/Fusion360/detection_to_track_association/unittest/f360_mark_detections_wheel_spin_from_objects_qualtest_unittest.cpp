/** \file
   Qualification tests for CPR-3858 requirement. Tests check if detections are marked correctly as 
   wheel spin object type i.e. detections which are likely a result of reflections from a spinning wheel.
*/


#include "f360_mark_detections_wheel_spin_from_objects.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/** \defgroup f360_detect_wheelspin_object_qualtest
*  @{
*/

using namespace f360_variant_A;

/** \brief
   Group of tests checking if detections are correctly marked as object wheel spins
 */
TEST_GROUP(f360_detect_wheelspin_object_qualtest)
{
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets = 0;
   F360_Calibrations_T calibs = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_track = {};

   /** \setup
   * Init calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Checks if detections are correctly marked as OBJECT wheel spins
*\req      CPR-3858
*/
TEST(f360_detect_wheelspin_object_qualtest, f360_detect_wheelspin_object_qualtest__correctly_marked_as_object_wheel_spin)
{
   /** \precond
   * Setup object and two dets close to each other
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_track.vcs_velocity.longitudinal = 2.0F;
   object_track.vcs_velocity.lateral = 0.0F;
   object_track.vcs_position.x = 29.9F;
   object_track.vcs_position.y = 1.0F;
   object_track.f_moving = true;
   object_track.Set_Bbox_Orientation(Angle{ 0.0F });
   object_track.Update_Bbox_Size(4.0F, 2.0F);

   uint32_t sensor_idx = 0;
   sensors[sensor_idx].variable.vcs_velocity.longitudinal = 1.0F;
   sensors[sensor_idx].variable.vcs_velocity.lateral = 0.0F;

   object_track.id = 1;
   num_dets = 2U;
   det_props[0].object_track_id = 1;
   det_props[1].object_track_id = 1;
   det_props[0].f_inside_gate = true;
   det_props[1].f_inside_gate = true;
   det_props[0].f_ok_to_use = true;
   det_props[1].f_ok_to_use = true;
   det_props[0].range_rate_dealiased = 10.0F;
   det_props[1].range_rate_dealiased = -10.0F;

   det_props[0].vcs_position.x = object_track.vcs_position.x - 0.1F;
   det_props[0].vcs_position.y = object_track.vcs_position.y + 0.1F;
   dets[0].processed.cos_vcs_az = 1.0F;
   dets[0].processed.sin_vcs_az = 0.0F;

   det_props[1].vcs_position.x = object_track.vcs_position.x + 0.1F;
   det_props[1].vcs_position.y = object_track.vcs_position.y - 0.1F;
   dets[1].processed.cos_vcs_az = 1.0F;
   dets[1].processed.sin_vcs_az = 0.0F;

   dets[0].raw.sensor_id = sensor_idx + 1;
   dets[1].raw.sensor_id = sensor_idx + 1;

   /** \action
    * Call Mark_Detections_Wheel_Spin_From_Objects()
    */
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, num_dets, sensors, calibs, object_track, det_props);

   /** \result
   * Detections should be marked as OBJECT WHEEL SPIN
   */
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_OBJECT, det_props[0].wheel_spin_type, "Detection is not marked as a OBJECT wheel spin");
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_OBJECT, det_props[1].wheel_spin_type, "Detection is not marked as a OBJECT wheel spin");
}

/**
*\purpose  Checks if detections are correctly marked as OBJECT wheel spins
*\req      CPR-3858
*/
TEST(f360_detect_wheelspin_object_qualtest, f360_detect_wheelspin_object_qualtest__correctly_not_marked_as_object_wheel_spin)
{
   /** \precond
   * Setup object and two dets
   * Fisrt det is not associated to object
   * Second has small range rate
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_track.vcs_velocity.longitudinal = 2.0F;
   object_track.vcs_velocity.lateral = 0.0F;
   object_track.vcs_position.x = 29.9F;
   object_track.vcs_position.y = 1.0F;
   object_track.f_moving = true;
   object_track.Set_Bbox_Orientation(Angle{ 0.0F });
   object_track.Update_Bbox_Size(4.0F, 2.0F);

   uint32_t sensor_idx = 0;
   sensors[sensor_idx].variable.vcs_velocity.longitudinal = 1.0F;
   sensors[sensor_idx].variable.vcs_velocity.lateral = 0.0F;

   object_track.id = 1;
   num_dets = 2U;
   det_props[0].object_track_id = 1;
   det_props[1].object_track_id = F360_INVALID_ID;
   det_props[0].f_inside_gate = true;
   det_props[1].f_inside_gate = true;
   det_props[0].range_rate_dealiased = 1.0F;
   det_props[1].range_rate_dealiased = -10.0F;

   det_props[0].vcs_position.x = object_track.vcs_position.x - 0.1F;
   det_props[0].vcs_position.y = object_track.vcs_position.y + 0.1F;
   dets[0].processed.cos_vcs_az = 1.0F;
   dets[0].processed.sin_vcs_az = 0.0F;

   det_props[1].vcs_position.x = object_track.vcs_position.x + 0.1F;
   det_props[1].vcs_position.y = object_track.vcs_position.y - 0.1F;
   dets[1].processed.cos_vcs_az = 1.0F;
   dets[1].processed.sin_vcs_az = 0.0F;

   dets[0].raw.sensor_id = sensor_idx + 1;
   dets[1].raw.sensor_id = sensor_idx + 1;

   dets[0].raw.sensor_id = 1;
   dets[1].raw.sensor_id = 1;

   /** \action
    * Call Mark_Detections_Wheel_Spin_From_Objects()
    */
   Mark_Detections_Wheel_Spin_From_Objects(tracker_info, dets, num_dets, sensors, calibs, object_track, det_props);

   /** \result
   * Detections should not be marked as OBJECT WHEEL SPIN
   */
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det_props[0].wheel_spin_type, "Detection is marked as a OBJECT wheel spin");
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det_props[1].wheel_spin_type, "Detection is marked as a OBJECT wheel spin");
}

/** @}*/
