/** \file
 * This file contains unit tests for content of f360_overall_confidence_helpers.cpp file
 */

#include "f360_overall_confidence_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_Confidence_State_Machine_Test_Group
 *  @{
 */

 /** \brief
  * It tests functionality of Confidence_State_Machine().
  */
TEST_GROUP(f360_Confidence_State_Machine_Test_Group)
{
   /** \setup
    * Set some thresholds for the state machine.
    */
   F360_Calibrations_T calib = {};

   const float thresh_easy = 1.0F;
   const float thresh_medium = 0.5F;
   const float thresh_hard = 0.2F;
   const float thresh_very_hard = 0.15F;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Test that the confidence state machine increases the confidence when the difference is small.
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_SingleStepUp)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_NONE;
   const float difference = 0.19F;

   for (int32_t i = 0; i < 9; i++)
   {
      /** \action
       * Call the Confidence_State_Machine function.
       */
      Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

      /** \result
       * Check if the confidence level has increased by one.
       */
      CHECK_EQUAL(static_cast<CONF9_T>(i + 1), current_conf);
   }
}

/** \purpose
 * Test that the confidence state machine increases the confidence by two steps when the difference is very small and checks over multiple consecutive scans to go over all valid confidence levels.
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_TwoStepsUp)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_LOW1;
   const float difference = 0.14F;

   for (int32_t i = 1; i < 7; i+=2) // Checking between 1 and 7 with a gap in between since two step increase is only possible if CONF is atleast CONF9_LOW1(1) and at most CONF9_MED3(7)
   {
      /** \action
       * Call the Confidence_State_Machine function.
       */
      Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

      /** \result
       * Check if the confidence level has increased by two.
       */
      CHECK_EQUAL(static_cast<CONF9_T>(i + 2), current_conf);
   }
}

/** \purpose
 * Test that the confidence state machine increases the confidence by two steps when the difference is very small and more than 2 steps away from CONF9_HIGH so that it is not saturated
 * Only checks in a single scan for increment
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_TwoStepsUp_SingleScan)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_LOW1;
   const float difference = 0.14F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence level has increased by two.
    */
   CHECK_EQUAL(CONF9_LOW3, current_conf);
}

/** \purpose
 * Test that the confidence state machine increases the confidence with saturation to CONF9_HIGH when the difference is very small
 * Only checks in a single scan for increment
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_TwoStepsUp_Saturate_SingleScan)
{
   /** \precond
    * Declare thresholds, current confidence level (setting to 1 state below CONF9_HIGH) and difference.
    */
   CONF9_T current_conf = CONF9_MED4;
   const float difference = 0.14F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence level has increased by only one and saturated to CONF9_HIGH.
    */
   CHECK_EQUAL(CONF9_HIGH, current_conf);
}

/** \purpose
 * Test that the confidence state machine increases by only one step when difference is equal to very hard threshold but not below it
 * Only checks in a single scan for increment
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_SingleStepUp_EqualToThreshVeryHard)
{
   /** \precond
    * Declare thresholds, current confidence level and difference (which is equal to very_hard_thresh).
    */
   CONF9_T current_conf = CONF9_LOW1;
   const float difference = 0.15F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence level has increased by only one.
    */
   CHECK_EQUAL(CONF9_LOW2, current_conf);
}

/** \purpose
 * Check if the state machine does not increase confidence when it's already at the highest level and difference is lower than hard threshold
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_StepUpOverHigh_HardThresh)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_HIGH;
   const float difference = 0.19F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence level is unchanged.
    */
   CHECK_EQUAL(CONF9_HIGH, current_conf);
}

/** \purpose
 * Check if the state machine does not increase confidence when it's already at the highest level and difference is lower than very hard threshold
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_StepUpOverHigh_VeryHardThresh)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_HIGH;
   const float difference = 0.14F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence level is unchanged.
    */
   CHECK_EQUAL(CONF9_HIGH, current_conf);
}

/** \purpose
 * When the confidence level is HIGH and the difference only fulfills easy threshold, Check if the confidence drops to MED1.
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_StepToMed)
{
   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_HIGH;
   const float difference = 0.9F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence dropped to MED1.
    */
   CHECK_EQUAL(CONF9_MED1, current_conf);
}

/** \purpose
 * When the difference is significant, Check if the confidence drops to LOW1.
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_StepDownToLowest)
{

   /** \precond
    * Declare thresholds, current confidence level and difference.
    */
   CONF9_T current_conf = CONF9_HIGH;
   const float difference = 1.1F;


   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf, difference, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence has dropped to LOW1.
    */
   CHECK_EQUAL(CONF9_LOW1, current_conf);
}

/** \purpose
 * Check if the state machine leaves the confidence unchanged when the difference is inside an interval.
 * \req NA
 */
TEST(f360_Confidence_State_Machine_Test_Group, Confidence_State_Machine_KeepCurrentConf)
{

   /** \precond
    * Declare thresholds, current confidence levels and differences.
    */
   CONF9_T current_conf1 = CONF9_HIGH;
   CONF9_T current_conf2 = CONF9_LOW4;
   float difference1 = 0.4F;
   float difference2 = 0.9F;

   /** \action
    * Call the Confidence_State_Machine function.
    */
   Confidence_State_Machine(current_conf1, difference1, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);
   Confidence_State_Machine(current_conf2, difference2, thresh_easy, thresh_medium, thresh_hard, thresh_very_hard);

   /** \result
    * Check if the confidence is unchanged.
    */
   CHECK_EQUAL(CONF9_HIGH, current_conf1);
   CHECK_EQUAL(CONF9_LOW4, current_conf2);

}
/** @}*/


/** \defgroup  f360_Overall_Confidence_Decay_Test_Group
 *  @{
 */

 /** \brief
  * It tests functionality of Overall_Confidence_Decay().
  */
TEST_GROUP(f360_Overall_Confidence_Decay_Test_Group)
{
   /** \setup
    * Set up calibrations
    */
   F360_Calibrations_T calib;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }

   void Set_Obj_Confidence_Level(F360_Object_Track_T &object_track, const CONF9_T conf_level)
   {
      object_track.conf_longitudinal_position = conf_level;
      object_track.conf_lateral_position = conf_level;
      object_track.conf_speed = conf_level;
   }
};

/** \purpose
 * Check if the confidence does not decay for an updated track.
 * \req NA
 */
TEST(f360_Overall_Confidence_Decay_Test_Group, Overall_Confidence_Decay_UpdatedTrack)
{

   /** \precond
    * Define a track with some confidence level.
    * Set object status to UPDATED
    */
   F360_Object_Track_T object_track = {};
   Set_Obj_Confidence_Level(object_track, CONF9_LOW2);
   object_track.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call the Overall_Confidence_Decay function.
    */
   Overall_Confidence_Decay(object_track, calib.k_conf_overall_timeout_to_start_decay);

   /** \result
    * Check if the confidence is unchanged.
    */
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_speed);
}

/** \purpose
 * Check if there is a grace period before confidence starts to decay for a coasted track.
 * \req NA
 */
TEST(f360_Overall_Confidence_Decay_Test_Group, Overall_Confidence_Decay_NewlyCoastedTrack)
{

   /** \precond
    * Define a track with some confidence level.
    * Set object status to COASTED
    * Set obejct time since last status update to 0.05
    */
   F360_Object_Track_T object_track = {};
   Set_Obj_Confidence_Level(object_track, CONF9_LOW2);
   object_track.status = F360_OBJECT_STATUS_COASTED;
   object_track.time_since_stage_start = 0.05F;

   /** \action
    * Call the Overall_Confidence_Decay function.
    */
   Overall_Confidence_Decay(object_track, calib.k_conf_overall_timeout_to_start_decay);

   /** \result
    * Check if the confidence is unchanged.
    */
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW2, object_track.conf_speed);
}

/** \purpose
 * Check if the confidence decays for a track that has coasted for too long.
 * \req NA
 */
TEST(f360_Overall_Confidence_Decay_Test_Group, Overall_Confidence_Decay_CoastedTrack)
{

   /** \precond
    * Define a track with some confidence level.
    * Set object status to COASTED
    * Set obejct time since last status update to 0.2
    */
   F360_Object_Track_T object_track = {};
   Set_Obj_Confidence_Level(object_track, CONF9_LOW2);
   object_track.status = F360_OBJECT_STATUS_COASTED;
   object_track.time_since_stage_start = 0.2F;

   /** \action
    * Call the Overall_Confidence_Decay function.
    */
   Overall_Confidence_Decay(object_track, calib.k_conf_overall_timeout_to_start_decay);

   /** \result
    * Check if all confidences were reduced by one level.
    */
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_speed);
}

/** \purpose
 * Check if the confidence cannot decay past the lowest level.
 * \req NA
 */
TEST(f360_Overall_Confidence_Decay_Test_Group, Overall_Confidence_Decay_CoastedLowestConfTrack)
{

   /** \precond
    * Define a track with confidences at the lowest level.
    * Set object status to COASTED
    * Set obejct time since last status update to 0.2
    */
   F360_Object_Track_T object_track = {};
   Set_Obj_Confidence_Level(object_track, CONF9_LOW1);
   object_track.status = F360_OBJECT_STATUS_COASTED;
   object_track.time_since_stage_start = 0.2F;

   /** \action
    * Call the Overall_Confidence_Decay function.
    */
   Overall_Confidence_Decay(object_track, calib.k_conf_overall_timeout_to_start_decay);

   /** \result
    * Check if the confidence is unchanged.
    */
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW1, object_track.conf_speed);
}
/** @}*/


/** \defgroup  f360_Overall_Confidence_Update_Test_Group
 *  @{
 */

 /** \brief
  * It tests functionality of Overall_Confidence_Update().
  */
TEST_GROUP(f360_Overall_Confidence_Update_Test_Group)
{
   /** \setup
    * Set up calibrations
    */
   F360_Calibrations_T calib;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Check if the confidence increases for a CCA track with good agreement between predicted and updated state.
 * \req NA
 */
TEST(f360_Overall_Confidence_Update_Test_Group, Overall_Confidence_Update_CCATrack)
{

   /** \precond
    * Define a CCA track with some confidence and good agreement between predicted and updated state (but not fulfilling very hard threshold so it only increments by one step).
    */
   F360_Object_Track_T object_track = {};
   object_track.conf_longitudinal_position = CONF9_LOW2;
   object_track.conf_lateral_position = CONF9_LOW2;
   object_track.conf_speed = CONF9_LOW2;

   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 10.0F;
   object_track.speed = 5.0F;
   object_track.predicted_vcs_position.x = 10.31F;
   object_track.predicted_vcs_position.y = 10.31F;
   object_track.predicted_speed = 5.16F;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call the Overall_Confidence_Update function.
    */
   Overall_Confidence_Update(object_track, calib);

   /** \result
    * Check if CCA confidences are increased.
    */
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_speed);
}

/** \purpose
 * Check if the confidence increases by two stages for a CCA track with very good agreement between predicted and updated state.
 * \req NA
 */
TEST(f360_Overall_Confidence_Update_Test_Group, Overall_Confidence_Update_CCATrack_TwoStepIncrement)
{

   /** \precond
    * Define a CCA track with some confidence and very good agreement between predicted and updated state.
    */
   F360_Object_Track_T object_track = {};
   object_track.conf_longitudinal_position = CONF9_LOW2;
   object_track.conf_lateral_position = CONF9_LOW2;
   object_track.conf_speed = CONF9_LOW2;

   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 10.0F;
   object_track.speed = 5.0F;
   object_track.predicted_vcs_position.x = 10.0F;
   object_track.predicted_vcs_position.y = 10.0F;
   object_track.predicted_speed = 5.0F;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call the Overall_Confidence_Update function.
    */
   Overall_Confidence_Update(object_track, calib);

   /** \result
    * Check if CCA confidences are increased by two steps.
    */
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_speed);
}

/** \purpose
 * Check if the confidence increases for a CTCA track with good agreement between predicted and updated state.
 * \req NA
 */
TEST(f360_Overall_Confidence_Update_Test_Group, Overall_Confidence_Update_CTCATrack)
{

   /** \precond
    * Define a CTCA track with some confidence and good agreement between predicted and updated state (but not fulfilling very hard threshold so it only increments by one step).
    */
   F360_Object_Track_T object_track = {};
   object_track.conf_longitudinal_position = CONF9_LOW2;
   object_track.conf_lateral_position = CONF9_LOW2;
   object_track.conf_speed = CONF9_LOW2;

   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 10.0F;
   object_track.speed = 5.0F;
   object_track.vcs_heading = Angle{ 0.0F };
   object_track.tang_accel = 0.0F;
   object_track.predicted_vcs_position.x = 10.31F;
   object_track.predicted_vcs_position.y = 10.31F;
   object_track.predicted_speed = 5.21F;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
    * Call the Overall_Confidence_Update function.
    */
   Overall_Confidence_Update(object_track, calib);

   /** \result
    * Check if CTCA confidences are increased.
    */
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW3, object_track.conf_speed);
}

/** \purpose
 * Check if the confidence increases by two stages for a CTCA track with very good agreement between predicted and updated state.
 * \req NA
 */
TEST(f360_Overall_Confidence_Update_Test_Group, Overall_Confidence_Update_CTCATrack_TwoStepIncrement)
{

   /** \precond
    * Define a CTCA track with some confidence and very good agreement between predicted and updated state
    */
   F360_Object_Track_T object_track = {};
   object_track.conf_longitudinal_position = CONF9_LOW2;
   object_track.conf_lateral_position = CONF9_LOW2;
   object_track.conf_speed = CONF9_LOW2;

   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 10.0F;
   object_track.speed = 5.0F;
   object_track.vcs_heading = Angle{ 0.0F };
   object_track.tang_accel = 0.0F;
   object_track.predicted_vcs_position.x = 10.0F;
   object_track.predicted_vcs_position.y = 10.0F;
   object_track.predicted_speed = 5.0F;
   object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
    * Call the Overall_Confidence_Update function.
    */
   Overall_Confidence_Update(object_track, calib);

   /** \result
    * Check if CTCA confidences are increased by two steps.
    */
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_longitudinal_position);
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_lateral_position);
   CHECK_EQUAL(CONF9_LOW4, object_track.conf_speed);
}
/** @}*/


/** \defgroup  f360_overall_confidence_helpers_for_CTCA
 *  @{
 */

 /** \brief
  * It tests functionality of Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To().
  */
TEST_GROUP(f360_overall_confidence_helpers_for_CTCA)
{
   /** \setup
    * Set confidence thresholds
    * Set all object's confidence params to conf_threshold (CONF9_LOW3)
    */
   const CONF9_T conf_threshold = CONF9_LOW3;
   const CONF9_T conf_below_th = CONF9_LOW2;
   const CONF9_T conf_above_th = CONF9_LOW4;
   F360_Object_Track_T object_track;

   TEST_SETUP()
   {
      Set_All_Obj_Params_To_Be_Above_Threshold(object_track);
   }

   void Set_All_Obj_Params_To_Be_Above_Threshold(F360_Object_Track_T &object_track)
   {
      object_track.conf_longitudinal_position = conf_above_th;
      object_track.conf_lateral_position = conf_above_th;
      object_track.conf_speed = conf_above_th;
   }
};

/** \purpose
 * Check if function returns false when all parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__False_If_All_Params_Above_Th)
{
   /** \precond
    * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
    */

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return false
   */
   CHECK_FALSE_TEXT(f_result, "Function should return false");
}

/** \purpose
 * Check if function returns true when conf_longitudinal_position is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_Pos_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_longitudinal_position to be equal to threshold
   */
   object_track.conf_longitudinal_position = conf_threshold;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}

/** \purpose
 * Check if function returns true when conf_lateral_position is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Lat_Pos_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_lateral_position to be equal to threshold
   */
   object_track.conf_lateral_position = conf_threshold;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}

/** \purpose
 * Check if function returns true when conf_speed is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Speed_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_speed to be equal to threshold
   */
   object_track.conf_speed = conf_threshold;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}

/** \purpose
 * Check if function returns true when conf_longitudinal_position is below threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_Pos_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_longitudinal_position to be below threshold
   */
   object_track.conf_longitudinal_position = conf_below_th;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}

/** \purpose
 * Check if function returns true when conf_lateral_position is belowthreshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Lat_Pos_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_lateral_position to be below threshold
   */
   object_track.conf_lateral_position = conf_below_th;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}

/** \purpose
 * Check if function returns true when conf_speed is below threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Speed_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_speed to be below threshold
   */
   object_track.conf_speed = conf_below_th;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}

/** \purpose
 * Check if function returns true when all confidence parameters are set below threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CTCA, Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_All_Params_Are_Below_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be below threshold
   */
   object_track.conf_longitudinal_position = conf_below_th;
   object_track.conf_lateral_position = conf_below_th;
   object_track.conf_speed = conf_below_th;

   /** \action
   * Call the Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}
/** @}*/


/** \defgroup  f360_overall_confidence_helpers_for_CCA
 *  @{
 */

 /** \brief
  * It tests functionality of Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To().
  */
TEST_GROUP(f360_overall_confidence_helpers_for_CCA)
{
   /** \setup
    * Set confidence thresholds
    */
   const CONF9_T conf_threshold = CONF9_LOW3;
   const CONF9_T conf_below_th = CONF9_LOW2;
   const CONF9_T conf_above_th = CONF9_LOW4;
   F360_Object_Track_T object_track;

   TEST_SETUP()
   {
      Set_All_Obj_Params_To_Be_Above_Threshold(object_track);
   }

   void Set_All_Obj_Params_To_Be_Above_Threshold(F360_Object_Track_T &object_track)
   {
      object_track.conf_longitudinal_position = conf_above_th;
      object_track.conf_lateral_position = conf_above_th;
      object_track.conf_speed = conf_above_th;
   }
};

/** \purpose
 * Check if function returns false when all parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__False_If_All_Params_Above_Th)
{
   /** \precond
    * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
    */

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return false
   */
   CHECK_FALSE_TEXT(f_result, "Function should return false");
}

/** \purpose
 * Check if function returns true when conf_longitudinal_position is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_Pos_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_longitudinal_position to be equal to threshold
   */
   object_track.conf_longitudinal_position = conf_threshold;

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}

/** \purpose
 * Check if function returns true when conf_lateral_position is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Lat_Pos_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_lateral_position to be equal to threshold
   */
   object_track.conf_lateral_position = conf_threshold;

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}


/** \purpose
 * Check if function returns true when conf_speed is equal to threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_Vel_Is_Equal_To_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_speed to be equal to threshold
   */
   object_track.conf_speed = conf_threshold;

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not equal to threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not equal to threshold.");
}

/** \purpose
 * Check if function returns true when conf_longitudinal_position is below threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_Pos_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_longitudinal_position to be below threshold
   */
   object_track.conf_longitudinal_position = conf_below_th;

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}

/** \purpose
 * Check if function returns true when conf_lateral_position is below threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Lat_Pos_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_lateral_position to be below threshold
   */
   object_track.conf_lateral_position = conf_below_th;

   /** \action
   * Call the Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}


/** \purpose
 * Check if function returns true when conf_speed is below threshold and rest parameters are above threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To__True_If_Only_Conf_Long_vel_Is_Less_Than_Th)
{
   /** \precond
   * Set all parameters i.e. confidence levels to be above threshold. Done in test group setup.
   * Set conf_speed to be below threshold
   */
   object_track.conf_speed = conf_below_th;

   /** \action
   * Call the Is_CCA_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}

/** \purpose
 * Check if function returns true when all confidence parameters are set below threshold
 * \req NA
 */
TEST(f360_overall_confidence_helpers_for_CCA, Is_CCA__Obj_Confidence_Less_Than_Or_Equal_To__True_If_All_Params_Are_Below_Th)
{
   /** \precond
    * Set all parameters i.e. confidence levels to be below threshold
    */
   object_track.conf_longitudinal_position = conf_below_th;
   object_track.conf_lateral_position = conf_below_th;
   object_track.conf_speed = conf_below_th;

   /** \action
   * Call the Is_CCA_CCA_Obj_Confidence_Less_Than_Or_Equal_To() function.
   */
   bool f_result = Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_track, conf_threshold);

   /** \result
   * Function should return true. Object confidence is not less than threshold.
   */
   CHECK_TRUE_TEXT(f_result, "Function should return true. Object confidence is not less than threshold.");
}


/** \defgroup  f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence
 *  @{
 */

 /** \brief
  * It tests functionality of Update_Low_RCS_Dets_Counter_And_Block_Confidence().
  */
TEST_GROUP(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence)
{
   /** \setup
    * Declare required structs and initialize calibs
    */
   F360_Calibrations_T calib{};
   rspp_variant_A::RSPP_Detection_List_T detection_list{};
   F360_Object_Track_T object_track{};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Check that counter increases by one as expected
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_increase_by_1)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 1;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = -20.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return true. Counter should have increased by 1.
   */
   CHECK_EQUAL(2, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that counter increases by two as expected
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_increase_by_2)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 1;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = -30.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return true. Counter should have increased by 2.
   */
   CHECK_EQUAL(3, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that counter decreases by one as expected
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_decrease_by_1)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 5;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = 0.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return true. Counter should have decreased by 1.
   */
   CHECK_EQUAL(4, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}


/** \purpose
 * Check that counter decreases by one as expected
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_decrease_by_1_by_range)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 5;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.range = 11.0F;
   detection_list.detections[0].raw.rcs = -30.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return true. Counter should have decreased by 1.
   */
   CHECK_EQUAL(4, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that counter decreases by two as expected when rcs is high
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_decrease_by_2_by_rcs)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 5;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = 10.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return false. Counter should have decreased by 2.
   */
   CHECK_EQUAL(3, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that counter decreases by two as expected when more than 1 detections is associated
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_decrease_by_2_by_ndet)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 5;
   object_track.ndets = 2;
   object_track.detids[0] = 1;
   object_track.detids[1] = 2;
   detection_list.detections[0].raw.rcs = -30.0F;
   detection_list.detections[1].raw.rcs = -30.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return false. Counter should have decreased by 2.
   */
   CHECK_EQUAL(3, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}

/** \purpose
 * Check that counter does not roll over
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_to_decrease_without_rollover)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 1;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = 10.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return false. Counter should be at zero.
   */
   CHECK_EQUAL(0, object_track.low_rcs_dets_cnt);
   CHECK_FALSE(f_result);
}

/** \purpose
 * Check that counter does not exceed its maximum
 * \req NA
 */
TEST(f360_Update_Low_RCS_Dets_Counter_And_Block_Confidence, check_for_counter_hit_max)
{
   /** \precond
    * Set up paramaters consistent with a track relevant for confidence blocking
    */
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.conf_overall = CONF3_NONE;
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   object_track.low_rcs_dets_cnt = 10;
   object_track.ndets = 1;
   object_track.detids[0] = 1;
   detection_list.detections[0].raw.rcs = -30.0F;

   /** \action
   * Call the Update_Low_RCS_Dets_Counter_And_Block_Confidence function.
   */
   bool f_result = Update_Low_RCS_Dets_Counter_And_Block_Confidence(detection_list, calib, object_track);

   /** \result
   * Function should return false. Counter should be at zero.
   */
   CHECK_EQUAL(10, object_track.low_rcs_dets_cnt);
   CHECK_TRUE(f_result);
}
/** @}*/
