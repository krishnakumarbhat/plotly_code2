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
   TEST_SETUP()
   {

   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose  Purpose of this test is to verify whether flags indicating that there are errors are cleared when all params are okay.
*\req    NA
*/
TEST(f360_output_diagnostics, Execute__check_whether_returns_false_when_all_params_are_okay)
{

   /** \precond
    * All tracks parameters have to be valid
    */

   /** \action
    * Call Execute function of output validity
    */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
    * Check whether all faults are false
    */
   CHECK_FALSE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_FALSE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
}

/**
*\purpose  Purpose of this test is to verify that the output faults corretly indicates that enough objects are flagged as faulty.
*\req    NA
*/
TEST(f360_output_diagnostics, Execute__check_enough_faulty_objects_is_set)
{

   /** \precond
   * Set lateral position of first track to be too high.
   * Set lateral position of second track to be too high.
   * Set lateral acceleration of third track to be too high.
   */
   obj_log.f360header.num_elements = 3;

   obj_log.object[0].vcs_yposn = output_diagnostics.Get_Calib().max_allowed_lateral_position + 0.01F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   obj_log.object[1].vcs_yposn = output_diagnostics.Get_Calib().max_allowed_lateral_position + 0.01F;
   obj_log.object[1].vcs_xposn = 5.0F;
   obj_log.object[1].tang_accel = 0.0F;
   obj_log.object[1].speed = 0.0F;
   obj_log.object[1].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   obj_log.object[2].vcs_yposn = 3.01F;
   obj_log.object[2].vcs_xposn = 5.0F;
   obj_log.object[2].tang_accel = output_diagnostics.Get_Calib().max_allowed_acceleration + 0.01F;
   obj_log.object[2].speed = 0.0F;
   obj_log.object[2].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check that position and acceleration faults are set as well as the flag indicating enough objects are faulty.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_TRUE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
   CHECK_TRUE(output_faults.f_enough_faulty_unique_objs);
}

/**
*\purpose  Purpose of this test is to verify that the output faults corretly indicates that not enough objects are flagged as faulty.
*\req   NA
*/
TEST(f360_output_diagnostics, Execute__check_not_enough_faulty_objects_is_set)
{

   /** \precond
   * Set lateral position of first track to be too high.
   * Set lateral position of second track to be within ok limits.
   * Set lateral acceleration of third track to be too high.
   */
   obj_log.f360header.num_elements = 3;

   obj_log.object[0].vcs_yposn = output_diagnostics.Get_Calib().max_allowed_lateral_position + 0.01F;
   obj_log.object[0].vcs_xposn = 0.0F;
   obj_log.object[0].tang_accel = 0.0F;
   obj_log.object[0].speed = 0.0F;
   obj_log.object[0].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   obj_log.object[1].vcs_yposn = output_diagnostics.Get_Calib().max_allowed_lateral_position - 0.01F;
   obj_log.object[1].vcs_xposn = 5.0F;
   obj_log.object[1].tang_accel = 0.0F;
   obj_log.object[1].speed = 0.0F;
   obj_log.object[1].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   obj_log.object[2].vcs_yposn = 3.01F;
   obj_log.object[2].vcs_xposn = 5.0F;
   obj_log.object[2].tang_accel = output_diagnostics.Get_Calib().max_allowed_acceleration + 0.01F;
   obj_log.object[2].speed = 0.0F;
   obj_log.object[2].status = static_cast<uint8_t>(F360_OBJ_STATUS_NEW);

   r_tracker_info.f_severe_angle_jump_detected = false;

   /** \action
   * Call Execute function of output validity
   */
   Output_Faults_T output_faults = output_diagnostics.Execute(obj_log, r_tracker_info);
   /** \result
   * Check that position and acceleration faults are set as well as the flag indicating enough objects are faulty.
   */
   CHECK_TRUE(output_faults.f_track_positions_faulty);
   CHECK_FALSE(output_faults.f_track_velocities_faulty);
   CHECK_TRUE(output_faults.f_track_accelerations_faulty);
   CHECK_FALSE(output_faults.f_severe_angle_jump_presence_fault);
   CHECK_FALSE(output_faults.f_enough_faulty_unique_objs);
}

