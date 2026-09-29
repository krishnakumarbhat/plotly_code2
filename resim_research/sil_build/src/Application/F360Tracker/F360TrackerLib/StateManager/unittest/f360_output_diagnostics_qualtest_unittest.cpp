/** \file
   This file contains unit-tests for Output Diagnostics class.
*/

#include "f360_output_diagnostics.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>



/** \defgroup  f360_output_diagnostics
 *  @{
 */

/** \brief
 *  Test group of Output_Diagnostics class.
 */
using namespace f360_variant_A;

TEST_GROUP(f360_output_diagnostics)
{
   /** \setup
   * Create object tracks structure.
   * Create Output_Diagnostics instance.
   */
   F360_Object_Log_Output_T obj_log = {};
   Tracker_Info_Log_T r_tracker_info = {};

   Output_Diagnostics output_diagnostics;
};

/**
*\purpose  Purpose of this test is to verify whether sets fault when lateral position is too high.
*\req    CPR-3903
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_lateral_pos_too_high)
{

   /** \precond
   * Set lateral position of selected track to be too high.
   */
   obj_log.object[0].vcs_yposn = output_diagnostics.Get_Calib().max_allowed_lateral_position + 0.01F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether sets fault when lateral position is too low.
*\req    CPR-3903
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_lateral_pos_too_low)
{

   /** \precond
   * Set lateral position of selected track to be too low.
   */
   obj_log.object[0].vcs_yposn = -(output_diagnostics.Get_Calib().max_allowed_lateral_position + 0.01F);
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether sets fault when longitudinal position is too high
*\req    CPR-3902
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_longitudinal_pos_too_high)
{

   /** \precond
   * Set longitudinal position of selected track to be too high.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = output_diagnostics.Get_Calib().max_allowed_longitudinal_position + 0.01F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether sets fault when longitudinal position is too low
*\req    CPR-3902
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_longitudinal_pos_too_low)
{

   /** \precond
   * Set longitudinal position of selected track to be too low.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = -(output_diagnostics.Get_Calib().max_allowed_longitudinal_position + 0.01F);
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether when tangential acceleration is too high
*           error is set
*\req    CPR-3904
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_acceleration_is_too_high)
{

   /** \precond
   * Set acceleration of selected track to be too high.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = output_diagnostics.Get_Calib().max_allowed_acceleration + 0.01F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_TRUE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether when tangential acceleration is too low
*           error is set
*\req    CPR-3904
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_acceleration_is_too_low)
{

   /** \precond
   * Set acceleration of selected track to be too low.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = output_diagnostics.Get_Calib().min_allowed_acceleration - 0.01F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether position fault is set
   * rest of the faults should not be set.
   */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_TRUE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether when OTG speed of track is too high
*           error is set
*\req    CPR-3905
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_otg_speed_is_too_high)
{

   /** \precond
   * Set speed of selected track to be too high.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = output_diagnostics.Get_Calib().max_allowed_speed + 0.01F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether velocity fault is set
   * rest of the faults should not be set.
   */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_TRUE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify whether when OTG speed of track is too low
*           error is set
*\req    CPR-3905
*/
TEST(f360_output_diagnostics, Execute__check_whether_fault_is_set_when_otg_speed_is_too_low)
{

   /** \precond
   * Set speed of selected track to be too low.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = output_diagnostics.Get_Calib().min_allowed_speed - 0.01F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether velocity fault is set
   * rest of the faults should not be set.
   */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_TRUE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}
/**
*\purpose  Purpose of this test is to verify whether when f_severe_angle_jump_presence_fault in tracker info log is true
*           error is set
*\req    CPR-3905
*/
TEST(f360_output_diagnostics, Execute__check_whether_tracker_info_fault_is_set)
{

   /** \precond
   * Set speed of selected track to be too low.
   */
   obj_log.object[0].vcs_yposn = 0.0F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);
   obj_log.f360header.num_elements = 1;
   r_tracker_info.f_severe_angle_jump_detected = true;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check whether velocity fault is set
   * rest of the faults should not be set.
   */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_TRUE(output_faults.f_severe_angle_jump_presence_fault);
}