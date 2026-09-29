/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_object_track_management_internals.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  Is_Enough_Valid_Dets_For_Update
*  @{
*/

/** \brief
* Test group for testing function Is_Enough_Valid_Dets_For_Update()
*/
TEST_GROUP(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T object_track;

   /** \setup
   * Fill detection properties and object data with initial information
   **/
   TEST_SETUP()
   {
      object_track.ndets = 3;
      object_track.num_rr_inlier_dets = 2;
      object_track.detids[0] = 1U;
      object_track.detids[1] = 2U;
      object_track.detids[2] = 3U;
      det_props[0].f_rr_inlier = true;
      det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[1].f_rr_inlier = true;
      det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[2].f_rr_inlier = false;
      det_props[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   }
};

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return true if CTCA object has number of associated detections greater than zero
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Enough_Detections_Status_Returned_For_CTCA_With_Any_Associated_Detections)
{
   /** \precond
   * Set object filter type to CTCA
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return enough detections status
   */
   CHECK_TRUE(f_enough_dets);
}

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return false if CTCA object has number of associated detections equal to zero
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Not_Enough_Detections_Status_Returned_For_CTCA_With_No_Associated_Detections)
{
   /** \precond
   * Set object filter type to CTCA
   * Set number of associated detecions to zero
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_track.ndets = 0;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return not enough detections status
   */
   CHECK_FALSE(f_enough_dets);
}

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return false if CCV object has number of associated and reduced detections equal to zero
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Not_Enough_Detections_Status_Returned_For_CCV_With_No_Associated_Detections)
{
   /** \precond
   * Set object filter type to CCV
   * Set number of associated detecions to zero
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_track.ndets = 0;
   object_track.num_rr_inlier_dets = 0;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return not enough detections status
   */
   CHECK_FALSE(f_enough_dets);
}

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return false if CCV object has number of associated detections greater than zero, but no reduced detections
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Not_Enough_Detections_Status_Returned_For_CCV_With_No_Reduced_Detections)
{
   /** \precond
   * Set object filter type to CCV
   * Set number of reduced detections to zero
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   det_props[0].f_rr_inlier = false;
   det_props[1].f_rr_inlier = false;
   det_props[2].f_rr_inlier = false;
   object_track.num_rr_inlier_dets = 0;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return not enough detections status
   */
   CHECK_FALSE(f_enough_dets);
}

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return false if CCV object has number of associated and reduced detections greater than zero, but all are wheespins
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Not_Enough_Detections_Status_Returned_For_CCV_With_All_Associated_Detecions_Wheel_Spins)
{
   /** \precond
   * Set object filter type to CCV
   * Set all associated, reduced detections as wheel spins
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return not enough detections status
   */
   CHECK_FALSE(f_enough_dets);
}

/** \purpose
* Is_Enough_Valid_Dets_For_Update should return true if CCV object has associated, reduced detections that are not wheel spins
* \req
* NA
*/
TEST(f360_object_track_management_internals__Is_Enough_Valid_Dets_For_Update, Check_If_Enough_Detections_Status_Returned_For_CCV_More_Than_Zero_Valid_Detections)
{
   /** \precond
   * Set object filter type to CCV
   * Remaining states set already in test group to make detections valid
   */
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   /** \action
   * Call Is_Enough_Valid_Dets_For_Update()
   */
   const bool f_enough_dets = Is_Enough_Valid_Dets_For_Update(det_props, object_track);

   /** \result
   * Function should return enough detections status
   */
   CHECK_TRUE(f_enough_dets);
}

/** \defgroup  f360_Change_Object_To_Updated_State
 *  @{
 */

 /** \brief
  *  Test group for Change_Object_To_Updated_State()
  */

TEST_GROUP(f360_Change_Object_To_Updated_State)
{
   F360_Object_Track_T object_track;
   float32_t check_epsilon = 0.00001F;
   F360_Calibrations_T calibs;

   /** \setup
   */
   TEST_SETUP()
   {
      calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C = 50.0F;
      calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C = 30.0F;

      object_track.status = F360_OBJECT_STATUS_INVALID;
      object_track.time_since_stage_start = 0.05F;
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.f_ghost_NU_2_C = false;
      object_track.num_updates_since_init = 5U;
   }
};

/**
*\purpose  Test if Change_Object_To_Updated_State() reset time i.e. time since stage start when object has COASTED status
*\req   NA
*/
TEST(f360_Change_Object_To_Updated_State, Change_Object_To_Updated_State__reset_time_if_object_COASTED)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_COASTED;
   uint8_t exp_num_updates_since_init = 6U;

   /** \action
    */
   Change_Object_To_Updated_State(object_track);

   /** \result
    */
   DOUBLES_EQUAL(object_track.time_since_stage_start, 0.0F, check_epsilon);
   CHECK_EQUAL(object_track.status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track.time_since_track_updated, 0.0F, check_epsilon);
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
   CHECK_EQUAL(exp_num_updates_since_init, object_track.num_updates_since_init);
}

/**
*\purpose  Test if Change_Object_To_Updated_State() saturates num_updates_since_init correctly
*\req   NA
*/
TEST(f360_Change_Object_To_Updated_State, Change_Object_To_Updated_State__test_saturation_num_updates)
{
   /** \precond
    */
   uint8_t uint8_max = 255U;
   object_track.status = F360_OBJECT_STATUS_COASTED;
   object_track.num_updates_since_init = uint8_max;

   uint16_t exp_num_updates_since_init = uint8_max;

   /** \action
    */
   Change_Object_To_Updated_State(object_track);

   /** \result
    */
   DOUBLES_EQUAL(object_track.time_since_stage_start, 0.0F, check_epsilon);
   CHECK_EQUAL(object_track.status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track.time_since_track_updated, 0.0F, check_epsilon);
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
   CHECK_EQUAL(exp_num_updates_since_init, object_track.num_updates_since_init);
}

/**
*\purpose  Test if Change_Object_To_Updated_State() reset time i.e. time since stage start when object has NEW COASTED status
*\req   NA
*/
TEST(f360_Change_Object_To_Updated_State, Change_Object_To_Updated_State__reset_time_if_object_NEW_COASTED)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_COASTED;

   /** \action
    */
   Change_Object_To_Updated_State(object_track);

   /** \result
    */
   DOUBLES_EQUAL(object_track.time_since_stage_start, 0.0F, check_epsilon);
   CHECK_EQUAL(object_track.status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track.time_since_track_updated, 0.0F, check_epsilon);
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Updated_State() reset time i.e. time since stage start when object has NEW status
*\req   NA
*/
TEST(f360_Change_Object_To_Updated_State, Change_Object_To_Updated_State__reset_time_if_object_NEW)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW;

   /** \action
    */
   Change_Object_To_Updated_State(object_track);

   /** \result
    */
   DOUBLES_EQUAL(object_track.time_since_stage_start, 0.0F, check_epsilon);
   CHECK_EQUAL(object_track.status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track.time_since_track_updated, 0.0F, check_epsilon);
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Updated_State() DOES NOT reset time i.e. time since stage start when object has INVALID status
*\req   NA
*/
TEST(f360_Change_Object_To_Updated_State, Change_Object_To_Updated_State__dont_reset_time_if_object_INVALID)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_INVALID;
   float32_t expected = object_track.time_since_stage_start;

   /** \action
    */
   Change_Object_To_Updated_State(object_track);

   /** \result
    */
   DOUBLES_EQUAL(object_track.time_since_stage_start, expected, check_epsilon);
   CHECK_EQUAL(object_track.status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track.time_since_track_updated, 0.0F, check_epsilon);
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/** @}*/


/** \defgroup  f360_Change_Object_To_Coasted_State
 *  @{
 */

 /** \brief
  *  Test group for Change_Object_To_Coasted_State()
  */

TEST_GROUP(f360_Change_Object_To_Coasted_State)
{
   F360_Object_Track_T object_track;
   float32_t check_epsilon = 0.00001F;
   F360_Calibrations_T calibs;

   /** \setup
   */
   TEST_SETUP()
   {
      calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C = 50.0F;
      calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C = 30.0F;

      object_track.status = F360_OBJECT_STATUS_INVALID;
      object_track.time_since_stage_start = 0.05F;
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.f_ghost_NU_2_C = false;
   }
};

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag to false
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_not_det_due_to_object_status)
{
   /** \precond
    */

    /** \action
     */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag to false due too high value of object's long position
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_not_det_due_to_too_high_object_long_position)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track.vcs_position.x = calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C + 0.001F;

   /** \action
    */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag to false due too low value of object's long position
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_not_det_due_to_too_low_object_long_position)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track.vcs_position.x = -(calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C + 0.001F);

   /** \action
    */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag to false due too high value of object's lateral position
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_not_det_due_to_too_high_object_lat_position)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track.vcs_position.y = calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C + 0.001F;;

   /** \action
    */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag to false due too low value of object's lateral position
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_not_det_due_to_too_low_objec_lat_position)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track.vcs_position.y = -(calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C + 0.001F);

   /** \action
    */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_FALSE(object_track.f_ghost_NU_2_C);
}

/**
*\purpose  Test if Change_Object_To_Coasted_State() properly set f_ghost_NU_2_C flag
*\req   NA
*/
TEST(f360_Change_Object_To_Coasted_State, Change_Object_To_Coasted_State__flag_is_set)
{
   /** \precond
    */
   object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action
    */
   Change_Object_To_Coasted_State(object_track, calibs);

   /** \result
    */
   CHECK_TRUE(object_track.f_ghost_NU_2_C);
}

/** @}*/