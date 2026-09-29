/** \file
 * This file contains unit tests for content of f360_populate_objects_log.cpp file
 */

#include "f360_populate_objects_log.h"
#include "f360_reference_point_support_functions.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_objects_log
 *  @{
 */

/** \brief
 * Test suit for functions in f360_populate_objects_log.cpp
 * Verify those functions return the value as expected.
 */
TEST_GROUP(f360_populate_objects_log)
{	
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS] {};
   F360_Object_Log_T objects_log[NUMBER_OF_OBJECT_TRACKS] {};
   int32_t num_active_objects{};
   int32_t active_obj_ids[NUMBER_OF_OBJECT_TRACKS]{};
   /** \setup
    * Set up the default variable value for all test cases
    */
   TEST_SETUP()
   {
      num_active_objects = 3;
      active_obj_ids[0] = 5;
      active_obj_ids[1] = 10;
      active_obj_ids[2] = 11;
      objects[4].unique_id = 100;
      objects[4].id = 5;
      objects[4].reduced_id = 1;
      objects[4].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[4].vcs_position.x = 10.0F;
      objects[4].vcs_position.y = 0.5F;
      objects[4].vcs_velocity.longitudinal = 22.0F;
      objects[4].vcs_velocity.lateral = 0.2F;
      objects[4].vcs_accel.longitudinal = 0.03F;
      objects[4].vcs_accel.lateral = 0.01F;
      objects[4].vcs_heading.Value(0.01F);
      objects[4].bbox.Set_Orientation(0.01F);
      objects[4].speed = 22.1F;
      objects[4].curvature = 0.01F;
      objects[4].tang_accel = 0.03F;
      objects[4].exist_prob = 1.0F;
      objects[4].otg_height = 1.2F;
      for (uint8_t j = 0U; j < STATE_DIMENSION; j++)
      {
         objects[4].errcov[j][j] = 0.02;
      }
      objects[4].errcov[0][1] = 0.01;
      objects[4].errcov[2][4] = 0.01;
      objects[4].errcov[3][5] = 0.01;
      objects[4].confidenceLevel = 0.9F;
      objects[4].time_since_measurement = 2.0F;
      objects[4].time_since_cluster_created = 3.0F;
      objects[4].time_since_track_updated = 2.5F;
      objects[4].time_since_stage_start = 2.5F;
      objects[4].average_rcs = 8.5F;
      objects[4].bbox.Set_Length(5.0F);
      objects[4].bbox.Set_Width(1.8F);
      objects[4].length_processed = 5.0F;
      objects[4].width_processed = 1.8F;
      objects[4].reference_point = F360_REFERENCE_POINT_REAR;
      objects[4].probability_pedestrian = 0.0F;
      objects[4].probability_car = 1.0F;
      objects[4].probability_motorcycle = 0.0F;
      objects[4].probability_bicycle = 0.0F;
      objects[4].probability_truck = 0.1F;
      objects[4].probability_undet = 0.0F;
      objects[4].probability_underdrivable_ocg = 0.0F;
      objects[4].ndets = 5U;
      objects[4].num_rr_inlier_dets = 4U;
      objects[4].num_dets_used_in_rr_msmt_update = 4U;
      objects[4].status = F360_OBJECT_STATUS_UPDATED;
      objects[4].reduced_status = F360_OBJECT_STATUS_UPDATED;
      objects[4].init_scheme = F360_TRACK_INIT_POSDIFF;
      objects[4].object_class = F360_OBJ_CLASS_CAR;
      objects[4].f_moving = true;
      objects[4].f_moveable = true;
      objects[4].movable_prob = 1.0F;
      objects[4].f_oncoming = false;
      objects[4].f_vehicular_trk = true;
      objects[4].on_sep_id = 0;
      objects[4].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      objects[4].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
      objects[4].drivable_confidence_sg = 100;


      objects[9].unique_id = 200;
      objects[9].id = 10;
      objects[9].reduced_id= 2;
      objects[9].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      objects[9].vcs_position.x = 2.0F;
      objects[9].vcs_position.y = 5.0F;
      objects[9].vcs_velocity.longitudinal = 0.0F;
      objects[9].vcs_velocity.lateral = 0.0F;
      objects[9].vcs_accel.longitudinal = 0.0F;
      objects[9].vcs_accel.lateral = 0.0F;
      objects[9].vcs_heading.Value(0.0F);
      objects[9].bbox.Set_Orientation(0.0F);
      objects[9].speed = 0.0F;
      objects[9].curvature = 0.0F;
      objects[9].tang_accel = 0.0F;
      objects[9].exist_prob = 0.8F;
      objects[9].otg_height = 0.3F;
      for (uint8_t j = 0U; j < STATE_DIMENSION; j++)
      {
         objects[9].errcov[j][j] = 0.02;
      }
      objects[9].errcov[0][3] = 0.01;
      objects[9].errcov[1][4] = 0.01;
      objects[9].errcov[2][5] = 0.01;
      objects[9].confidenceLevel = 0.9F;
      objects[9].time_since_measurement = 1.0F;
      objects[9].time_since_cluster_created = 1.50F;
      objects[9].time_since_track_updated = 1.5F;
      objects[9].time_since_stage_start = 1.5F;
      objects[9].average_rcs = 1.5F;
      objects[9].bbox.Set_Length(1.0F);
      objects[9].bbox.Set_Width(1.0F);
      objects[9].length_processed = 1.0F;
      objects[9].width_processed = 1.0F;
    
      objects[9].reference_point = F360_REFERENCE_POINT_CENTER;
      objects[9].probability_pedestrian = 0.4F;
      objects[9].probability_car = 0.1F;
      objects[9].probability_motorcycle = 0.2F;
      objects[9].probability_bicycle = 0.1F;
      objects[9].probability_truck = 0.0F;
      objects[9].probability_undet = 0.1F;
      objects[9].probability_underdrivable_ocg = 0.0F;
      objects[9].ndets = 2U;
      objects[9].num_rr_inlier_dets = 2U;
      objects[9].num_dets_used_in_rr_msmt_update = 2U;
      objects[9].status = F360_OBJECT_STATUS_UPDATED;
      objects[9].reduced_status = F360_OBJECT_STATUS_UPDATED;
      objects[9].init_scheme = F360_TRACK_INIT_STATIONARY;
      objects[9].object_class = F360_OBJ_CLASS_UNDETERMINED;
      objects[9].f_moving = false;
      objects[9].f_moveable = false;
      objects[9].movable_prob = 0.0F;
      objects[9].f_oncoming = false;
      objects[9].f_vehicular_trk = false;
      objects[9].on_sep_id = 1;
      objects[9].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      objects[9].drivable_status_sg = sg::SG_Drivability_Class_T::OVERDRIVABLE;
      objects[9].drivable_confidence_sg = 90;

      objects[10].unique_id = 300;
      objects[10].id = 11;
      objects[10].reduced_id= 3;
      objects[10].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
      objects[10].vcs_position.x = 2.0F;
      objects[10].vcs_position.y = 5.0F;
      objects[10].vcs_velocity.longitudinal = 0.0F;
      objects[10].vcs_velocity.lateral = 0.0F;
      objects[10].vcs_accel.longitudinal = 0.0F;
      objects[10].vcs_accel.lateral = 0.0F;
      objects[10].vcs_heading.Value(0.0F);
      objects[10].bbox.Set_Orientation(0.0F);
      objects[10].speed = 0.0F;
      objects[10].curvature = 0.0F;
      objects[10].tang_accel = 0.0F;
      objects[10].exist_prob = 0.8F;
      objects[10].otg_height = 0.3F;
      for (uint8_t j = 0U; j < STATE_DIMENSION; j++)
      {
         objects[10].errcov[j][j] = 0.02F;
      }
      objects[10].errcov[0][2] = 0.01F;
      objects[10].errcov[1][3] = 0.01F;
      objects[10].errcov[4][5] = 0.01F;
      objects[10].confidenceLevel = 0.9F;
      objects[10].time_since_measurement = 1.0F;
      objects[10].time_since_cluster_created = 1.50F;
      objects[10].time_since_track_updated = 1.5F;
      objects[10].time_since_stage_start = 1.5F;
      objects[10].average_rcs = 1.5F;
      objects[10].bbox.Set_Length(1.0F);
      objects[10].bbox.Set_Width(1.0F);
      objects[10].length_processed = 1.0F;
      objects[10].width_processed = 1.0F;
    
      objects[10].reference_point = F360_REFERENCE_POINT_CENTER;
      objects[10].probability_pedestrian = 0.4F;
      objects[10].probability_car = 0.1F;
      objects[10].probability_motorcycle = 0.2F;
      objects[10].probability_bicycle = 0.1F;
      objects[10].probability_truck = 0.0F;
      objects[10].probability_undet = 0.1F;
      objects[10].probability_underdrivable_ocg = 0.0F;
      objects[10].ndets = 2U;
      objects[10].num_rr_inlier_dets = 2U;
      objects[10].num_dets_used_in_rr_msmt_update = 2U;
      objects[10].status = F360_OBJECT_STATUS_UPDATED;
      objects[10].reduced_status = F360_OBJECT_STATUS_UPDATED;
      objects[10].init_scheme = F360_TRACK_INIT_STATIONARY;
      objects[10].object_class = F360_OBJ_CLASS_UNDETERMINED;
      objects[10].f_moving = false;
      objects[10].f_moveable = false;
      objects[10].movable_prob = 0.0F;
      objects[10].f_oncoming = true;
      objects[10].f_vehicular_trk = false;
      objects[10].on_sep_id = 1;
      objects[10].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      objects[10].drivable_status_sg = sg::SG_Drivability_Class_T::OVERDRIVABLE;
      objects[10].drivable_confidence_sg = 90;
   }
};

/** \purpose  
 * Verify Populate_Objects_Log_Data returns the value as expected
 * \req NA
 */
TEST(f360_populate_objects_log, Test_Populate_Objects_Log_Data)
{
   /** \precond
    * Using the test group default values
    */
	
   /** \action
    * call Populate_Objects_Log_Data().
    */
   Populate_Objects_Log_Data(objects_log,objects,num_active_objects,active_obj_ids);

   /** \result
    * check that the output match expected data.
    */
   CHECK_EQUAL_TEXT(100, objects_log[0].unique_id, "Object log expected unique id is not 100");
   CHECK_EQUAL_TEXT(5, objects_log[0].trkID, "Object log expected track id is not 5");
   CHECK_EQUAL_TEXT(1, objects_log[0].reducedID, "Object log expected reduced object id is not 1");
   CHECK_EQUAL_TEXT(2, objects_log[0].trk_fltr_type, "Object log expected track model type is not 2");
   DOUBLES_EQUAL_TEXT(10.0F, objects_log[0].vcs_xposn, F360_EPSILON, "Object log expected vcs x position is not 10.0F");
   DOUBLES_EQUAL_TEXT(0.5F, objects_log[0].vcs_yposn, F360_EPSILON, "Object log expected vcs y position is not 0.5F");
   DOUBLES_EQUAL_TEXT(22.0F, objects_log[0].vcs_xvel, F360_EPSILON, "Object log expected longitudinal velocity is not 22.0F");
   DOUBLES_EQUAL_TEXT(0.2F, objects_log[0].vcs_yvel, F360_EPSILON, "Object log expected lateral velcoity is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.03F, objects_log[0].vcs_xaccel, F360_EPSILON, "Object log expected longitudinal acceleration is not 0.03");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].vcs_yaccel, F360_EPSILON, "Object log expected lateral acceleration is not 0.01");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].vcs_heading, F360_EPSILON, "Object log vcs heading id is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].vcs_pointing, F360_EPSILON, "Object log vcs pointing angle is not 0.01");
   DOUBLES_EQUAL_TEXT(22.1F, objects_log[0].speed, F360_EPSILON, "Object log expected speed is not 22.1F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].curvature, F360_EPSILON, "Object log expected curvature is not 0.01");
   DOUBLES_EQUAL_TEXT(0.03F, objects_log[0].tang_accel, F360_EPSILON, "Object log expected accleartion is not 0.03F");
   DOUBLES_EQUAL_TEXT(1.0F, objects_log[0].existence_probability, F360_EPSILON, "Object log expected existence_probability is not 1.0F");
   DOUBLES_EQUAL_TEXT(1.2F, objects_log[0].otg_height, F360_EPSILON, "Object log expected over the ground height is not 1.2F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[0], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[1], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[2], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[3], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[4], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[0].state_variance[5], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].supplemental_state_covariance[0], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].supplemental_state_covariance[1], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[0].supplemental_state_covariance[2], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.9F, objects_log[0].confidenceLevel, F360_EPSILON, "Object log expected confidenceLevel is not 0.9F");
   DOUBLES_EQUAL_TEXT(2.0F, objects_log[0].time_since_measurement, F360_EPSILON, "Object log expected time since measurement is not 2.0F");
   DOUBLES_EQUAL_TEXT(3.0F, objects_log[0].time_since_cluster_created, F360_EPSILON, "Object log expected time_since_cluster_created is not 3.0F");
   DOUBLES_EQUAL_TEXT(2.5F, objects_log[0].time_since_track_updated, F360_EPSILON, "Object log expected time_since_track_updated is not 2.5F");
   DOUBLES_EQUAL_TEXT(2.5F, objects_log[0].time_since_stage_start, F360_EPSILON, "Object log expected time_since_stage_start is not 2.5F");
   DOUBLES_EQUAL_TEXT(8.5F, objects_log[0].radar_cross_section, F360_EPSILON, "Object log expected radar_cross_section is not 8.5F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].len1, F360_EPSILON, "Object log expected len1 is not 0.0F");
   DOUBLES_EQUAL_TEXT(5.0F, objects_log[0].len2, F360_EPSILON, "Object log expected len2 is not 5.0F");
   DOUBLES_EQUAL_TEXT(0.9F, objects_log[0].wid1, F360_EPSILON, "Object log expected wid1 is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.9F, objects_log[0].wid2, F360_EPSILON, "Object log expected wid2 is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].probability_pedestrian, F360_EPSILON, "Object log expectedprobability_pedestrian is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.0F, objects_log[0].probability_car, F360_EPSILON, "Object log expected probability_car is not 1.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].probability_motorcycle, F360_EPSILON, "Object log expected probability_motorcycle is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].probability_bicycle, F360_EPSILON, "Object log expected probability_bicycle is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, objects_log[0].probability_truck, F360_EPSILON, "Object log expected probability_truck is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].probability_undet, F360_EPSILON, "Object log expected probability_undet is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[0].probability_underdrivable_ocg, F360_EPSILON, "Object log expected probability_underdrivable_ocg is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.0F, objects_log[0].movable_prob, F360_EPSILON, "Object log expected movable_prob is not 1.0F");
   CHECK_EQUAL_TEXT(5U, objects_log[0].ndets, "Object log expected number of detection is not 5U");
   CHECK_EQUAL_TEXT(4U, objects_log[0].num_rr_inlier_dets, "Object log expected num_rr_inlier_dets is not 4U");
   CHECK_EQUAL_TEXT(4U, objects_log[0].num_dets_used_in_rr_msmt_update, "Object log expected num_dets_used_in_rr_msmt_update is not 4U");
   CHECK_EQUAL_TEXT(6U, objects_log[0].reference_point, "Object log expected reference_point is not 6U");
   CHECK_EQUAL_TEXT(4U, objects_log[0].status, "Object log expected status is not 4U");
   CHECK_EQUAL_TEXT(4U, objects_log[0].reducedStatus, "Object log expected reducedStatus is not 4U");
   CHECK_EQUAL_TEXT(2U, objects_log[0].init_scheme, "Object log expected init_scheme is not 2U");
   CHECK_EQUAL_TEXT(1U, objects_log[0].object_class, "Object log expected object_class is not 1");
   CHECK_EQUAL_TEXT(1U, objects_log[0].f_moving, "Object log expected f_moving is not 1");
   CHECK_EQUAL_TEXT(1U, objects_log[0].f_moveable, "Object log expected f_moveable is not 1");
   CHECK_EQUAL_TEXT(0U, objects_log[0].f_oncoming, "Object log expected f_oncoming is not 0U");
   CHECK_EQUAL_TEXT(1U, objects_log[0].f_vehicular_trk, "Object log expected f_vehicular_trk is not 1");
   CHECK_EQUAL_TEXT(0U, objects_log[0].f_onguardrail, "Object log expected f_onguardrail is not 0");
   CHECK_EQUAL_TEXT(0U, objects_log[0].underdrivable_status_ocg, "Object log expected underdrivable_status_ocg is not 0");
   CHECK_EQUAL_TEXT(2U, objects_log[0].drivable_status_sg, "Object log expected drivable_status_sg is not 2U");
   CHECK_EQUAL_TEXT(100U, objects_log[0].drivable_confidence_sg, "Object log expected drivable_confidence_sg is not 100F");

   CHECK_EQUAL_TEXT(200, objects_log[1].unique_id, "Object log expected unique id is not 200");
   CHECK_EQUAL_TEXT(10, objects_log[1].trkID, "Object log expected track id is not 10");
   CHECK_EQUAL_TEXT(2, objects_log[1].reducedID, "Object log expected reduced object id is not 2");
   CHECK_EQUAL_TEXT(3, objects_log[1].trk_fltr_type, "Object log expected track model type is not 3");
   DOUBLES_EQUAL_TEXT(2.0F, objects_log[1].vcs_xposn, F360_EPSILON, "Object log expected vcs x position is not 2.0F");
   DOUBLES_EQUAL_TEXT(5.0F, objects_log[1].vcs_yposn, F360_EPSILON, "Object log expected vcs y position is not 5.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_xvel, F360_EPSILON, "Object log expected longitudinal velocity is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_yvel, F360_EPSILON, "Object log expected lateral velcoity is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_xaccel, F360_EPSILON, "Object log expected longitudinal acceleration is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_yaccel, F360_EPSILON, "Object log expected lateral acceleration is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_heading, F360_EPSILON, "Object log vcs heading id is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].vcs_pointing, F360_EPSILON, "Object log vcs pointing angle is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].speed, F360_EPSILON, "Object log expected speed is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].curvature, F360_EPSILON, "Object log expected curvature is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].tang_accel, F360_EPSILON, "Object log expected accleartion is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.8F, objects_log[1].existence_probability, F360_EPSILON, "Object log expected existence_probability is not 0.8F");
   DOUBLES_EQUAL_TEXT(0.3F, objects_log[1].otg_height, F360_EPSILON, "Object log expected over the ground height is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[0], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[1], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[2], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[3], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[4], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.02F, objects_log[1].state_variance[5], F360_EPSILON, "Object log expected state_variance is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[1].supplemental_state_covariance[0], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[1].supplemental_state_covariance[1], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.01F, objects_log[1].supplemental_state_covariance[2], F360_EPSILON, "Object log expected supplemental_state_covariance is not 0.01F");
   DOUBLES_EQUAL_TEXT(0.9F, objects_log[1].confidenceLevel, F360_EPSILON, "Object log expected confidenceLevel is not 0.9F");
   DOUBLES_EQUAL_TEXT(1.0F, objects_log[1].time_since_measurement, F360_EPSILON, "Object log expected time since measurement is not 1.0F");
   DOUBLES_EQUAL_TEXT(1.5F, objects_log[1].time_since_cluster_created, F360_EPSILON, "Object log expected time_since_cluster_created is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.5F, objects_log[1].time_since_track_updated, F360_EPSILON, "Object log expected time_since_track_updated is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.5F, objects_log[1].time_since_stage_start, F360_EPSILON, "Object log expected time_since_stage_start is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.5F, objects_log[1].radar_cross_section, F360_EPSILON, "Object log expected radar_cross_section is not 1.5F");
   DOUBLES_EQUAL_TEXT(0.5F, objects_log[1].len1, F360_EPSILON, "Object log expected len1 is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.5F, objects_log[1].len2, F360_EPSILON, "Object log expected len2 is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.5F, objects_log[1].wid1, F360_EPSILON, "Object log expected wid1 is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.5F, objects_log[1].wid2, F360_EPSILON, "Object log expected wid2 is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.4F, objects_log[1].probability_pedestrian, F360_EPSILON, "Object log expectedprobability_pedestrian is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.1F, objects_log[1].probability_car, F360_EPSILON, "Object log expected probability_car is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.2F, objects_log[1].probability_motorcycle, F360_EPSILON, "Object log expected probability_motorcycle is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, objects_log[1].probability_bicycle, F360_EPSILON, "Object log expected probability_bicycle is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].probability_truck, F360_EPSILON, "Object log expected probability_truck is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, objects_log[1].probability_undet, F360_EPSILON, "Object log expected probability_undet is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects_log[1].probability_underdrivable_ocg, F360_EPSILON, "Object log expected probability_underdrivable_ocg is not 0.0F");
   CHECK_EQUAL_TEXT(2U, objects_log[1].ndets, "Object log expected number of detection is not 2U");
   CHECK_EQUAL_TEXT(2U, objects_log[1].num_rr_inlier_dets, "Object log expected num_rr_inlier_dets is not 2U");
   CHECK_EQUAL_TEXT(2U, objects_log[1].num_dets_used_in_rr_msmt_update, "Object log expected num_dets_used_in_rr_msmt_update is not 2U");
   CHECK_EQUAL_TEXT(0U, objects_log[1].reference_point, "Object log expected reference_point is not 0U");
   CHECK_EQUAL_TEXT(4U, objects_log[1].status, "Object log expected status is not 4U");
   CHECK_EQUAL_TEXT(4U, objects_log[1].reducedStatus, "Object log expected reducedStatus is not 4U");
   CHECK_EQUAL_TEXT(0U, objects_log[1].init_scheme, "Object log expected init_scheme is not 8U");
   CHECK_EQUAL_TEXT(0U, objects_log[1].object_class, "Object log expected object_class is not 0");
   CHECK_EQUAL_TEXT(0U, objects_log[1].f_moving, "Object log expected f_moving is not 0");
   CHECK_EQUAL_TEXT(0U, objects_log[1].f_moveable, "Object log expected f_moveable is not 0");
   CHECK_EQUAL_TEXT(0.0F, objects_log[1].movable_prob, "Object log expected movable_prob is not 0.0F");
   CHECK_EQUAL_TEXT(0U, objects_log[1].f_oncoming, "Object log expected f_oncoming is not 0U");
   CHECK_EQUAL_TEXT(0U, objects_log[1].f_vehicular_trk, "Object log expected f_vehicular_trk is not 0");
   CHECK_EQUAL_TEXT(1U, objects_log[1].f_onguardrail, "Object log expected f_onguardrail is not 1");
   CHECK_EQUAL_TEXT(0U, objects_log[1].underdrivable_status_ocg, "Object log expected underdrivable_status_ocg is not 0");
   CHECK_EQUAL_TEXT(1U, objects_log[1].drivable_status_sg, "Object log expected drivable_status_sg is not 1U");
   CHECK_EQUAL_TEXT(90U, objects_log[1].drivable_confidence_sg, "Object log expected drivable_confidence_sg is not 90F");
   CHECK_EQUAL_TEXT(1U, objects_log[2].f_oncoming, "Object log expected f_oncoming is not 1U");



}

/** \purpose  
 * Verify Populate_Objects_Data returns the values as expected
 * \req NA
 */
TEST(f360_populate_objects_log, Test_Populate_Objects_Data)
{
   /** \precond
    * Describe the extra varibles that needs by Populate_Objects_Data()
    */
   F360_Tracker_Info_T tracker_info{};
   F360_Host_T host{};

   // Using the default data from test group then call Populate_Objects_Log_Data to get the objects_log first;
   Populate_Objects_Log_Data(objects_log,objects,num_active_objects,active_obj_ids);

   // Clear default objects values and use the generated objects_log to test Populate_Objects_Data
   memset(&objects[0], 0, sizeof(F360_Object_Track_T) * NUMBER_OF_OBJECT_TRACKS);
   Populate_Objects_Data(objects,tracker_info, objects_log, host);

   /** \result
    * check that the output match expected data.
    */
   CHECK_EQUAL_TEXT(100U, objects[4].unique_id, "Object log expected unique id is not 100");
   CHECK_EQUAL_TEXT(5U, objects[4].id, "Object log expected track id is not 5");
   CHECK_EQUAL_TEXT(1U, objects[4].reduced_id, "Object log expected reduced object id is not 1");
   CHECK_EQUAL_TEXT(2, objects[4].trk_fltr_type, "Object log expected track model type is not 2")
   DOUBLES_EQUAL_TEXT(10.0F, objects[4].vcs_position.x, F360_EPSILON, "Object log expected vcs x position is not 10.0F")
   DOUBLES_EQUAL_TEXT(0.5F, objects[4].vcs_position.y, F360_EPSILON, "Object log expected vcs y position is not 0.5F")
   DOUBLES_EQUAL_TEXT(22.0F, objects[4].vcs_velocity.longitudinal, F360_EPSILON, "Object log expected longitudinal velocity is not 22.0F")
   DOUBLES_EQUAL_TEXT(0.2F, objects[4].vcs_velocity.lateral, F360_EPSILON, "Object log expected lateral velcoity is not 0.2F")
   DOUBLES_EQUAL_TEXT(0.03F, objects[4].vcs_accel.longitudinal, F360_EPSILON, "Object log expected longitudinal acceleration is not 0.03")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].vcs_accel.lateral, F360_EPSILON, "Object log expected lateral acceleration is not 0.01")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].vcs_heading.Value(), F360_EPSILON, "Object log vcs heading id is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].bbox.Get_Orientation().Value(), F360_EPSILON, "Object log vcs pointing angle is not 0.01")
   DOUBLES_EQUAL_TEXT(22.1F, objects[4].speed, F360_EPSILON, "Object log expected speed is not 22.1F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].curvature, F360_EPSILON, "Object log expected curvature is not 0.01")
   DOUBLES_EQUAL_TEXT(0.03F, objects[4].tang_accel, F360_EPSILON, "Object log expected accleartion is not 0.03F")
   DOUBLES_EQUAL_TEXT(F360_EPSILON, objects[4].exist_prob, 1.0F, "Object log expected existence_probability is not 1.0F")
   DOUBLES_EQUAL_TEXT(1.2F, objects[4].otg_height, F360_EPSILON, "Object log expected over the ground height is not 1.2F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[0][0], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[1][1], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[2][2], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[3][3], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[4][4], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[4].errcov[5][5], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[0][1], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[1][0], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[2][4], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[4][2], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[3][5], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[4].errcov[5][3], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.9F, objects[4].confidenceLevel, F360_EPSILON, "Object log expected confidenceLevel is not 0.9F");
   DOUBLES_EQUAL_TEXT(2.0F, objects[4].time_since_measurement, F360_EPSILON, "Object log expected time since measurement is not 2.0F");
   DOUBLES_EQUAL_TEXT(3.0F, objects[4].time_since_cluster_created, F360_EPSILON, "Object log expected time_since_cluster_created is not 3.0F");
   DOUBLES_EQUAL_TEXT(2.5F, objects[4].time_since_track_updated, F360_EPSILON, "Object log expected time_since_track_updated is not 2.5F");
   DOUBLES_EQUAL_TEXT(2.5F, objects[4].time_since_stage_start, F360_EPSILON, "Object log expected time_since_stage_start is not 2.5F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[4].probability_pedestrian, F360_EPSILON, "Object log expectedprobability_pedestrian is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.0F, objects[4].probability_car, F360_EPSILON, "Object log expected probability_car is not 1.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[4].probability_motorcycle, F360_EPSILON, "Object log expected probability_motorcycle is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[4].probability_bicycle, F360_EPSILON, "Object log expected probability_bicycle is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[4].probability_truck, F360_EPSILON, "Object log expected probability_truck is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[4].probability_undet, F360_EPSILON, "Object log expected probability_undet is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[4].probability_underdrivable_ocg, F360_EPSILON, "Object log expected probability_underdrivable_ocg is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.0F, objects[4].movable_prob, F360_EPSILON, "Object log expected movable_prob is not 1.0F");
   CHECK_EQUAL_TEXT(5U, objects[4].ndets, "Object log expected number of detection is not 5U");
   CHECK_EQUAL_TEXT(4U, objects[4].num_rr_inlier_dets, "Object log expected num_rr_inlier_dets is not 4U");
   CHECK_EQUAL_TEXT(4U, objects[4].num_dets_used_in_rr_msmt_update, "Object log expected num_dets_used_in_rr_msmt_update is not 4U");
   CHECK_EQUAL_TEXT(6U, objects[4].reference_point, "Object log expected reference_point is not 6U");
   CHECK_EQUAL_TEXT(4U, objects[4].status, "Object log expected status is not 4U");
   CHECK_EQUAL_TEXT(4U, objects[4].reduced_status, "Object log expected reduced_status is not 4U");
   CHECK_EQUAL_TEXT(2U, objects[4].init_scheme, "Object log expected init_scheme is not 2U");
   CHECK_EQUAL_TEXT(1U, objects[4].object_class, "Object log expected object_class is not 1");
   CHECK_EQUAL_TEXT(1U, objects[4].f_moving, "Object log expected f_moving is not 1");
   CHECK_EQUAL_TEXT(1U, objects[4].f_moveable, "Object log expected f_moveable is not 1");
   CHECK_EQUAL_TEXT(0U, objects[4].f_oncoming, "Object log expected f_oncoming is not 0U");
   CHECK_EQUAL_TEXT(1U, objects[4].f_vehicular_trk, "Object log expected f_vehicular_trk is not 1");
   CHECK_EQUAL_TEXT(0U, objects[4].underdrivable_status_ocg, "Object log expected underdrivable_status_ocg is not 0");

   CHECK_EQUAL_TEXT(200U, objects[9].unique_id, "Object log expected unique id is not 200");
   CHECK_EQUAL_TEXT(10U, objects[9].id, "Object log expected track id is not 10");
   CHECK_EQUAL_TEXT(2U, objects[9].reduced_id, "Object log expected reduced object id is not 2");
   CHECK_EQUAL_TEXT(3, objects[9].trk_fltr_type, "Object log expected track model type is not 2")
   DOUBLES_EQUAL_TEXT(2.0F, objects[9].vcs_position.x, F360_EPSILON, "Object log expected vcs x position is not 2.0F")
   DOUBLES_EQUAL_TEXT(5.0F, objects[9].vcs_position.y, F360_EPSILON, "Object log expected vcs y position is not 5.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].vcs_velocity.longitudinal, F360_EPSILON, "Object log expected longitudinal velocity is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].vcs_velocity.lateral, F360_EPSILON, "Object log expected lateral velcoity is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].vcs_accel.longitudinal, F360_EPSILON, "Object log expected longitudinal acceleration is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].vcs_accel.lateral, F360_EPSILON, "Object log expected lateral acceleration is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].vcs_heading.Value(), F360_EPSILON, "Object log vcs heading id is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].bbox.Get_Orientation().Value(), F360_EPSILON, "Object log vcs pointing angle is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].speed, F360_EPSILON, "Object log expected speed is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].curvature, F360_EPSILON, "Object log expected curvature is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].tang_accel, F360_EPSILON, "Object log expected accleartion is not 0.0F");
   DOUBLES_EQUAL_TEXT(F360_EPSILON, objects[9].exist_prob, 0.8F, "Object log expected existence_probability is not 0.8F")
   DOUBLES_EQUAL_TEXT(0.3F, objects[9].otg_height, F360_EPSILON, "Object log expected over the ground height is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[0][0], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[1][1], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[2][2], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[3][3], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[4][4], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[9].errcov[5][5], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[0][3], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[3][0], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[1][4], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[4][1], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[2][5], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[9].errcov[5][2], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.9F, objects[9].confidenceLevel, F360_EPSILON, "Object log expected confidenceLevel is not 0.9F");
   DOUBLES_EQUAL_TEXT(1.0F, objects[9].time_since_measurement, F360_EPSILON, "Object log expected time since measurement is not 1.0F");
   DOUBLES_EQUAL_TEXT(1.5F, objects[9].time_since_cluster_created, F360_EPSILON, "Object log expected time_since_cluster_created is not 10.5")
   DOUBLES_EQUAL_TEXT(1.5F, objects[9].time_since_track_updated, F360_EPSILON, "Object log expected time_since_track_updated is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.5F, objects[9].time_since_stage_start, F360_EPSILON, "Object log expected time_since_stage_start is not 1.5F");
   DOUBLES_EQUAL_TEXT(0.4F, objects[9].probability_pedestrian, F360_EPSILON, "Object log expectedprobability_pedestrian is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[9].probability_car, F360_EPSILON, "Object log expected probability_car is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[9].probability_motorcycle, F360_EPSILON, "Object log expected probability_motorcycle is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[9].probability_bicycle, F360_EPSILON, "Object log expected probability_bicycle is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].probability_truck, F360_EPSILON, "Object log expected probability_truck is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[9].probability_undet, F360_EPSILON, "Object log expected probability_undet is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[9].probability_underdrivable_ocg, F360_EPSILON, "Object log expected probability_underdrivable_ocg is not 0.0F");
   CHECK_EQUAL_TEXT(2U, objects[9].ndets, "Object log expected number of detection is not 2U");
   CHECK_EQUAL_TEXT(2U, objects[9].num_rr_inlier_dets, "Object log expected num_rr_inlier_dets is not 2U");
   CHECK_EQUAL_TEXT(2U, objects[9].num_dets_used_in_rr_msmt_update, "Object log expected num_dets_used_in_rr_msmt_update is not 2U");
   CHECK_EQUAL_TEXT(0U, objects[9].reference_point, "Object log expected reference_point is not 0U");
   CHECK_EQUAL_TEXT(4U, objects[9].status, "Object log expected status is not 4U");
   CHECK_EQUAL_TEXT(4U, objects[9].reduced_status, "Object log expected reduced_status is not 4U");
   CHECK_EQUAL_TEXT(0U, objects[9].init_scheme, "Object log expected init_scheme is not 0U");
   CHECK_EQUAL_TEXT(0U, objects[9].object_class, "Object log expected object_class is not 0");
   CHECK_EQUAL_TEXT(0U, objects[9].f_moving, "Object log expected f_moving is not 0");
   CHECK_EQUAL_TEXT(0U, objects[9].f_moveable, "Object log expected f_moveable is not 0");
   CHECK_EQUAL_TEXT(0U, objects[9].f_oncoming, "Object log expected f_oncoming is not 0U");
   CHECK_EQUAL_TEXT(0U, objects[9].f_vehicular_trk, "Object log expected f_vehicular_trk is not 0");
   CHECK_EQUAL_TEXT(0U, objects[9].underdrivable_status_ocg, "Object log expected underdrivable_status_ocg is not 0");
   
   CHECK_EQUAL_TEXT(300U, objects[10].unique_id, "Object log expected unique id is not 200");
   CHECK_EQUAL_TEXT(11U, objects[10].id, "Object log expected track id is not 10");
   CHECK_EQUAL_TEXT(3U, objects[10].reduced_id, "Object log expected reduced object id is not 3");
   CHECK_EQUAL_TEXT(1, objects[10].trk_fltr_type, "Object log expected track model type is not 1")
   DOUBLES_EQUAL_TEXT(2.0F, objects[10].vcs_position.x, F360_EPSILON, "Object log expected vcs x position is not 2.0F")
   DOUBLES_EQUAL_TEXT(5.0F, objects[10].vcs_position.y, F360_EPSILON, "Object log expected vcs y position is not 5.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].vcs_velocity.longitudinal, F360_EPSILON, "Object log expected longitudinal velocity is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].vcs_velocity.lateral, F360_EPSILON, "Object log expected lateral velcoity is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].vcs_accel.longitudinal, F360_EPSILON, "Object log expected longitudinal acceleration is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].vcs_accel.lateral, F360_EPSILON, "Object log expected lateral acceleration is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].vcs_heading.Value(), F360_EPSILON, "Object log vcs heading id is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].bbox.Get_Orientation().Value(), F360_EPSILON, "Object log vcs pointing angle is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].speed, F360_EPSILON, "Object log expected speed is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].curvature, F360_EPSILON, "Object log expected curvature is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].tang_accel, F360_EPSILON, "Object log expected accleartion is not 0.0F")
   DOUBLES_EQUAL_TEXT(F360_EPSILON, objects[10].exist_prob, 0.8F, "Object log expected existence_probability is not 0.8F")
   DOUBLES_EQUAL_TEXT(0.3F, objects[10].otg_height, F360_EPSILON, "Object log expected over the ground height is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.02F, objects[10].errcov[0][0], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[10].errcov[1][1], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(1.00F, objects[10].errcov[2][2], F360_EPSILON, "Object log expected state_variance is not 1.00F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[10].errcov[3][3], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(0.02F, objects[10].errcov[4][4], F360_EPSILON, "Object log expected state_variance is not 0.02F")
   DOUBLES_EQUAL_TEXT(1.00F, objects[10].errcov[5][5], F360_EPSILON, "Object log expected state_variance is not 1.00F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[10].errcov[0][3], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[10].errcov[3][0], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[10].errcov[1][4], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.01F, objects[10].errcov[4][1], F360_EPSILON, "Object log expected state_variance is not 0.01F")
   DOUBLES_EQUAL_TEXT(0.00F, objects[10].errcov[2][5], F360_EPSILON, "Object log expected state_variance is not 0.00F")
   DOUBLES_EQUAL_TEXT(0.00F, objects[10].errcov[5][2], F360_EPSILON, "Object log expected state_variance is not 0.00F")
   DOUBLES_EQUAL_TEXT(0.9F, objects[10].confidenceLevel, F360_EPSILON, "Object log expected confidenceLevel is not 0.9F");
   DOUBLES_EQUAL_TEXT(1.0F, objects[10].time_since_measurement, F360_EPSILON, "Object log expected time since measurement is not 1.0F");
   DOUBLES_EQUAL_TEXT(1.5F, objects[10].time_since_cluster_created, F360_EPSILON, "Object log expected time_since_cluster_created is not 10.5");
   DOUBLES_EQUAL_TEXT(1.5F, objects[10].time_since_track_updated, F360_EPSILON, "Object log expected time_since_track_updated is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.5F, objects[10].time_since_stage_start, F360_EPSILON, "Object log expected time_since_stage_start is not 1.5F");
   DOUBLES_EQUAL_TEXT(0.4F, objects[10].probability_pedestrian, F360_EPSILON, "Object log expectedprobability_pedestrian is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[10].probability_car, F360_EPSILON, "Object log expected probability_car is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[10].probability_motorcycle, F360_EPSILON, "Object log expected probability_motorcycle is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[10].probability_bicycle, F360_EPSILON, "Object log expected probability_bicycle is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].probability_truck, F360_EPSILON, "Object log expected probability_truck is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[10].probability_undet, F360_EPSILON, "Object log expected probability_undet is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].probability_underdrivable_ocg, F360_EPSILON, "Object log expected probability_underdrivable_ocg is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].movable_prob, F360_EPSILON, "Object log expected movable_prob is not 0.0F");
   CHECK_EQUAL_TEXT(2U, objects[10].ndets, "Object log expected number of detection is not 2U");
   CHECK_EQUAL_TEXT(2U, objects[10].num_rr_inlier_dets, "Object log expected num_rr_inlier_dets is not 2U");
   CHECK_EQUAL_TEXT(2U, objects[10].num_dets_used_in_rr_msmt_update, "Object log expected num_dets_used_in_rr_msmt_update is not 2U");
   CHECK_EQUAL_TEXT(0U, objects[10].reference_point, "Object log expected reference_point is not 0U");
   CHECK_EQUAL_TEXT(4U, objects[10].status, "Object log expected status is not 4U");
   CHECK_EQUAL_TEXT(4U, objects[10].reduced_status, "Object log expected reduced_status is not 4U");
   CHECK_EQUAL_TEXT(0U, objects[10].init_scheme, "Object log expected init_scheme is not 8U");
   CHECK_EQUAL_TEXT(0U, objects[10].object_class, "Object log expected object_class is not 0U");
   CHECK_EQUAL_TEXT(0U, objects[10].f_moving, "Object log expected f_moving is not 0U");
   CHECK_EQUAL_TEXT(0U, objects[10].f_moveable, "Object log expected f_moveable is not 0U");
   CHECK_EQUAL_TEXT(1U, objects[10].f_oncoming, "Object log expected f_oncoming is not 1U");
   CHECK_EQUAL_TEXT(0U, objects[10].f_vehicular_trk, "Object log expected f_vehicular_trk is not 0U");
   CHECK_EQUAL_TEXT(0U, objects[10].underdrivable_status_ocg, "Object log expected underdrivable_status_ocg is not 0U");

   /** \action
    * update objects_log property and call Populate_Objects_Data() again
    * this is to verify another scenario
    */
   objects_log[1].movable_prob = 1.0F;
   objects_log[1].f_moving = true;
   objects_log[1].speed = 5.0F;
   objects_log[1].curvature = 0.01F;
   objects_log[2].movable_prob = 1.0F;
   objects_log[6].unique_id = 100U; // create a duplicated unique ID to test branch coverage
   objects_log[6].trkID = 100U;
   objects_log[6].status = 4U;
   objects_log[6].reducedStatus = 0U;  
   Populate_Objects_Data(objects,tracker_info, objects_log, host);

   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(0.05F, objects[9].heading_rate, F360_EPSILON,"Object log expected heading rate is not 0.0F")
  
  /** \action
    * update two objects_log property and call Populate_Objects_Data() again
    */
   objects_log[1].trkID = 0;
   objects_log[2].trk_fltr_type = 0;
   objects[10].errcov[0][1] = 0.0F;
   objects[10].errcov[0][3] = 0.0F;
   Populate_Objects_Data(objects,tracker_info, objects_log, host);
   
    /** \result
    * check that the output match expected data.
    * objects_log[1] is not valid anymore, objects_log[6] is added
    * objects[10].errcov[0][1] objects[10].errcov[0][3] 
    * will not be updated due to trk filter type is invalid
    * 
    */
   CHECK_EQUAL_TEXT(3U, tracker_info.num_active_objs, "Expected number of active objects is not 2")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].errcov[0][1], F360_EPSILON, "Object log expected state_variance is not 0.0F")
   DOUBLES_EQUAL_TEXT(0.0F, objects[10].errcov[0][3], F360_EPSILON, "Object log expected state_variance is not 0.0F")
}


/** @}*/
