/** \file
 * This file contains unit tests for content of f360_update_exist_prob_helpers.cpp file
 */

#include "f360_update_exist_prob_helpers.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ccv_states.h"
#include <CppUTest/TestHarness.h>
#include "f360_tracker_info.h"

using namespace f360_variant_A;

/** \defgroup  f360_update_exist_prob_helpers_functions
 *  @{
 */

 /** \brief
 * Test group of f360_update_exist_prob_helpers_functions. Tests verify sub functions used inside
 * Update_Existence_Probability().
 **/
TEST_GROUP(f360_update_exist_prob_helper_functions)
{
   /** \setup
   * Set up common variables
   **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Object_Track_T& object = object_tracks[0];

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
   }
};


/**
*\purpose  Purpose of this test is to verify whether new updated object track has p_track_state equal to calibration value.
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Calculate_P_Track_State__if_p_track_state_set_to_calib_value)
{
   /** \precond
   * Set object status to F360_OBJECT_STATUS_NEW_UPDATED
   **/
   const float32_t expected_p_track_state = calibrations.k_ep_prob_track_state_init_value;
   object.status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action
   * Call Calculate_P_Track_State
   **/
   const float32_t p_track_state = Calculate_P_Track_State(object, calibrations);

   /** \result
   * Check whether object p_track_state is equal to calibration value
   **/
   DOUBLES_EQUAL(expected_p_track_state, p_track_state, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether a CTCA track has the correct p_track_state
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Calculate_P_Track_State__CTCA_object)
{
   /** \precond
   * Set object status to F360_OBJECT_STATUS_UPDATED
   * Set object filter typ to CTCA
   * Fill errcov with 1s
   **/
   const float32_t expected_p_track_state = 0.583599091F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
      {
         object.errcov[i][j] = 1.0F;
      }
   }

   /** \action
   * Call Calculate_P_Track_State
   **/
   const float32_t p_track_state = Calculate_P_Track_State(object, calibrations);

   /** \result
   * Check whether object p_track_state is equal to the expected value
   **/
   DOUBLES_EQUAL(expected_p_track_state, p_track_state, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether CCA track has the correct p_track_state
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Calculate_P_Track_State__CCA_object)
{
   /** \precond
   * Set object status to F360_OBJECT_STATUS_UPDATED
   * Set object filter typ to CCA
   * Fill errcov with 1s
   **/
   const float32_t expected_p_track_state = 0.6345649F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
      {
         object.errcov[i][j] = 1.0F;
      }
   }

   /** \action
   * Call Calculate_P_Track_State
   **/
   const float32_t p_track_state = Calculate_P_Track_State(object, calibrations);

   /** \result
   * Check whether object p_track_state is equal to the correct value
   **/
   DOUBLES_EQUAL(expected_p_track_state, p_track_state, F360_EPSILON)
}

/**
*\purpose  Purpose of this test is to verify whether state variance is normalized for CCA model.
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Normalize_State_Variances__if_state_variance_is_calculated_for_CCA)
{
   /** \precond
   * Set object motion model to CCA and provide other data necessary for calculatins.
   **/
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   float32_t s2[STATE_DIMENSION] = {};

   object.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] = 4.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 2.25F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = 1.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] = 4.0F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 2.25F;
   object.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = 1.0F;
   const uint32_t actual_state_dimension = static_cast<uint32_t>(STATE_DIMENSION);

   /** \action
   * Call Normalize_State_Variances
   **/
   Normalize_State_Variances(object, calibrations, actual_state_dimension, s2);

   /** \result
   * Check whether object state variance values are qual to expected.
   **/
   DOUBLES_EQUAL(1.0F, s2[0], F360_EPSILON);
   DOUBLES_EQUAL(1.0F, s2[1], F360_EPSILON);
   DOUBLES_EQUAL(1.0F, s2[2], F360_EPSILON);
   DOUBLES_EQUAL(1.0F, s2[3], F360_EPSILON);
   DOUBLES_EQUAL(1.0F, s2[4], F360_EPSILON);
   DOUBLES_EQUAL(1.0F, s2[5], F360_EPSILON); 
}

/**
*\purpose  Purpose of this test is to verify whether low denominator value in function Normalize_State() is handled.
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Normalize_State_Variances__if_low_denominator_is_handle)
{
   /** \precond
   * Set the data to get the branch wherein denominator values colse to zero are handled.
   **/
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   float32_t s2[STATE_DIMENSION] = {}; // vector of state variances
   float32_t s2_th[STATE_DIMENSION] = { 1.0F,1.0F,1.0F,1.0F,1.0F,1.0F }; // initial state variance thresholds
   const uint32_t actual_state_dimension = static_cast<uint32_t>(STATE_DIMENSION);

   s2_th[0] = F360_EPSILON * 0.5F;
   s2_th[1] = F360_EPSILON * 0.5F;
   s2_th[2] = F360_EPSILON * 0.5F;
   s2_th[3] = F360_EPSILON * 0.5F;
   s2_th[4] = F360_EPSILON * 0.5F;
   s2_th[5] = F360_EPSILON * 0.5F;

   /** \action
   * Call Normalize_State
   **/
   Normalize_State(s2, s2_th, actual_state_dimension);

   /** \result
   * Check whether object state variance values are set to large  values.
   **/
   DOUBLES_EQUAL(INFTY, s2[0], F360_EPSILON);
   DOUBLES_EQUAL(INFTY, s2[1], F360_EPSILON);
   DOUBLES_EQUAL(INFTY, s2[2], F360_EPSILON);
   DOUBLES_EQUAL(INFTY, s2[3], F360_EPSILON);
   DOUBLES_EQUAL(INFTY, s2[4], F360_EPSILON);
   DOUBLES_EQUAL(INFTY, s2[5], F360_EPSILON);
}


/**
*\purpose  Purpose of this test is to verify whether Normalize_Information() is handled in the normal
* case where denominator is large enough so that we don't have to worry about division with zero
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Normalize_Information_default)
{
   /** \precond
   * Set up an array representing the information. Let it be larger than 
   * calibs.k_ep_bottom_saturation_of_normalized_variance for all elements except the last.
   * the last element  is smaller than calibs.k_ep_bottom_saturation_of_normalized_variance so that
   * we can test the denominator saturation case.
   **/
   const float32_t s2[STATE_DIMENSION] = {0.5F, 0.1F, 1.0F, 2.0F, 5.0F, 0.5F * calibrations.k_ep_bottom_saturation_of_normalized_variance};
   const uint32_t actual_state_dimension = static_cast<uint32_t>(STATE_DIMENSION);

   /** \action
   * Call Normalize_State
   **/
  float32_t output_normalized_information = 0.0F;
   Normalize_Information(s2, calibrations, actual_state_dimension, output_normalized_information);

   /** \result
   * Check whether object state variance values are set to large  values.
   **/
  const float32_t expected_normalized_information = 1.0F / s2[0] + 1.0F / s2[1] + 1.0F / s2[2] + 1.0F / s2[3] + 1.0F / s2[4] + 1.0F / calibrations.k_ep_bottom_saturation_of_normalized_variance;    
  DOUBLES_EQUAL(expected_normalized_information, output_normalized_information, F360_EPSILON);
}

/**
*\purpose  Purpose of this test is to verify whether Normalize_Information() is handled for the case
* case where denominator is very small so that have to protect ourselves aginst division with zero.
*\req    NA.
*/
TEST(f360_update_exist_prob_helper_functions, Normalize_Information_protection_against_zero_division)
{
   /** \precond
   * Change calibs.k_ep_bottom_saturation_of_normalized_variance such that it is smaller than F360_EPSILON
   * Set up an array representing the information. Let it be larger than 
   * calibs.k_ep_bottom_saturation_of_normalized_variance for all elements except the last.
   * the last element is smaller than calibs.k_ep_bottom_saturation_of_normalized_variance so that
   * we can test the case of a very small denominator.
   **/
  calibrations.k_ep_bottom_saturation_of_normalized_variance = F360_EPSILON * 0.5F;
  const float32_t s2[STATE_DIMENSION] = {0.5F, 0.1F, 1.0F, 2.0F, 5.0F, 0.5F * calibrations.k_ep_bottom_saturation_of_normalized_variance};
  const uint32_t actual_state_dimension = static_cast<uint32_t>(STATE_DIMENSION);

   /** \action
   * Call Normalize_State
   **/
  float32_t output_normalized_information = 0.0F;
   Normalize_Information(s2, calibrations, actual_state_dimension, output_normalized_information);

   /** \result
   * Check whether object state variance values are set to large  values.
   **/ 
  DOUBLES_EQUAL(INFTY, output_normalized_information, F360_EPSILON);
}

/** @}*/

/** \defgroup f360_update_exist_prob_helpers_low_rcs
 *  @{ 
 */

/** \brief
* Test group for Decrease_EP_If_Low_RCS. Verifies existence probability adjustments under different RCS, age, max RCS and threat proximity conditions.
**/
TEST_GROUP(f360_update_exist_prob_helpers_low_rcs)
{
   F360_Calibrations_T calib = {};
   F360_Host_T host = {};
   F360_Object_Track_T obj = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      host.speed = 5.0F; // default >1 m/s
      host.vcs_sideslip = 0.0F;
      obj.exist_prob = 0.9F;
      obj.vcs_position.x = 12.0F; // default outside close threat range (>10)
      obj.vcs_position.y = 0.0F;
      obj.time_since_initialization = 2.0F; // immature (<4)
      obj.average_rcs = -17.5F; // below threshold (-16)
      obj.maximum_rcs = -12.0F; // not high enough (> -10 would be high)
   }
};

/**
 *\purpose
   Low average RCS object with insufficient age and low max RCS is limited.
   *\req    NA.
   */
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__immature_low_max_rcs_decreases)
{
   /** \precond
    * Baseline setup: average_rcs < -16, age <4, max_rcs <= -10 not satisfied, range >10, ttc unspecified -> reduction path.
    */
   const float32_t starting_ep = obj.exist_prob;

   /** \action
    * Call Decrease_EP_If_Low_RCS.
    */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result
    * EP reduced to 0.65F upper limit.
    */
   DOUBLES_EQUAL(0.65F, obj.exist_prob, F360_EPSILON);
   CHECK_TRUE(starting_ep > obj.exist_prob);
}

/**
 *\purpose  Mature track with high max RCS decreased EP, because of far range and high ttc.
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__mature_high_max_rcs_range_ttc)
{
   /** \precond
    * Adjust age >4 and maximum_rcs > -10 so initial exclusion criteria satisfied, but range remains >10 and ttc is high, so EP will be decreased.
    */
   obj.time_since_initialization = 5.0F; // mature
   obj.maximum_rcs = -5.0F; // high
   obj.exist_prob = 0.8F; // below cap already

   /** \action */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result
    * EP reduced to 0.65F.
    */

   DOUBLES_EQUAL(0.65F, obj.exist_prob, F360_EPSILON);
}

/**
 *\purpose  Mature track near host (range <10) excluded from reduction (close threat).
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__mature_close_range_excluded)
{
   /** \precond
    * Set age >4, high max_rcs, and range <10.
    */
   obj.time_since_initialization = 6.0F;
   obj.maximum_rcs = -8.0F;
   obj.vcs_position.x = 5.0F; // close
   obj.exist_prob = 0.75F;

   /** \action */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result */
   DOUBLES_EQUAL(0.75F, obj.exist_prob, F360_EPSILON);
}

/**
 *\purpose  Mature track with high max RCS but host almost stationary still excluded.
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__mature_host_slow_excluded)
{
   /** \precond
    * Age >4, host.speed <1, average_rcs low, max_rcs may remain low but host slow condition triggers max_rcs branch.
    */
   host.speed = 0.5F; // host slow
   obj.time_since_initialization = 7.0F;
   obj.maximum_rcs = -12.0F; // still low but host slow acts like high max_rcs branch
   obj.exist_prob = 0.7F;

   /** \action */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result */
   DOUBLES_EQUAL(0.65F, obj.exist_prob, F360_EPSILON);
}

/**
 *\purpose  Low average RCS object with age >4 but exclusion not satisfied (max_rcs low, range >10 and ttc <=3) gets reduced.
*\req    NA.*/
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__mature_low_max_rcs_reduction)
{
   /** \precond
    * Age >4, maximum_rcs remains low (<= -10), range >10; expect reduction.
    */
   obj.time_since_initialization = 5.0F; // mature
   obj.maximum_rcs = -12.0F; // low
   obj.vcs_position.x = 15.0F; // range >10
   obj.exist_prob = 0.85F;

   /** \action */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result */
   DOUBLES_EQUAL(0.65F, obj.exist_prob, F360_EPSILON);
}

/**
 *\purpose  Average RCS above threshold leaves EP unchanged.
*\req    NA.*/
TEST(f360_update_exist_prob_helpers_low_rcs, Decrease_EP_If_Low_RCS__average_rcs_above_threshold_no_change)
{
   /** \precond
    * average_rcs >= -16 so function should not modify EP.
    */
   obj.average_rcs = -15.0F; // above threshold
   obj.exist_prob = 0.82F;

   /** \action */
   Decrease_EP_If_Low_RCS(host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, obj);

   /** \result */
   DOUBLES_EQUAL(0.82F, obj.exist_prob, F360_EPSILON);
}
/** @}*/

/** \defgroup f360_update_exist_prob_helpers_idm
 *  @{ 
 */

/** \brief
* Test group for Decrease_EP_Based_On_IDM_Detections. Verifies existence probability adjustment driven by fraction of IDM detections.
**/
TEST_GROUP(f360_update_exist_prob_helpers_idm)
{
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object = {};

   TEST_SETUP()
   {
      // Common baseline: object has 5 associated detections with sequential IDs, none marked IDM.
      object.exist_prob = 1.0F;
      object.ndets = 5U;
      for(uint32_t i=0;i<object.ndets;i++)
      {
         object.detids[i] = i+1U;
         detections[i].raw.f_idm_det = false;
      }
      object.idm_det_fraction = 0.0F;
   }
};

/**
 *\purpose  Validate EP unchanged when object has zero detections.
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_idm, Decrease_EP_Based_On_IDM_Detections__no_change_with_zero_detections)
{
   /** \precond
    * Object has zero detections (ndets = 0). EP initialized to 1.0.
    */
   // Override baseline: zero detections
   object.exist_prob = 1.0F;
   object.ndets = 0U; // no detids used

   /** \action
    * Call Decrease_EP_Based_On_IDM_Detections().
    */
   Decrease_EP_Based_On_IDM_Detections(detections, object);

   /** \result
    * EP remains unchanged and idm_det_fraction is zero.
    */
   DOUBLES_EQUAL(1.0F, object.exist_prob, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object.idm_det_fraction, F360_EPSILON);
}

/**
 *\purpose  Validate EP unchanged when IDM fraction exactly equals threshold.
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_idm, Decrease_EP_Based_On_IDM_Detections__equals_threshold_no_change)
{
   /** \precond
    * Configure 5 detections with exactly 2 IDM (fraction = 0.4).
    */
   object.exist_prob = 0.9F;
   // Baseline already has 5 dets; mark first two as IDM
   detections[0].raw.f_idm_det = true;
   detections[1].raw.f_idm_det = true;

   /** \action */
   Decrease_EP_Based_On_IDM_Detections(detections, object);

   /** \result */
   DOUBLES_EQUAL(0.9F, object.exist_prob, F360_EPSILON);
   DOUBLES_EQUAL(0.4F, object.idm_det_fraction, F360_EPSILON);
}

/**
 *\purpose  Validate EP reduced when IDM fraction just above threshold.
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_idm, Decrease_EP_Based_On_IDM_Detections__just_above_threshold_decreases)
{
   /** \precond
    * Configure 5 detections with 3 IDM (fraction = 0.6).
    */
   object.exist_prob = 1.0F;
   // Mark first three as IDM
   detections[0].raw.f_idm_det = true;
   detections[1].raw.f_idm_det = true;
   detections[2].raw.f_idm_det = true;

   /** \action
    * Call Decrease_EP_Based_On_IDM_Detections().
    */
   Decrease_EP_Based_On_IDM_Detections(detections, object);

   /** \result
    * EP reduced to 0.825F for fraction 0.6; fraction stored.
    */
   DOUBLES_EQUAL(0.825F, object.exist_prob, F360_EPSILON);
   DOUBLES_EQUAL(0.6F, object.idm_det_fraction, F360_EPSILON);
}

/**
 *\purpose  Validate EP capped by saturation at high IDM fraction (all detections IDM).
 *\req    NA.*/
TEST(f360_update_exist_prob_helpers_idm, Decrease_EP_Based_On_IDM_Detections__high_fraction_capped)
{
   /** \precond
    * Configure 5 detections all IDM (fraction = 1.0).
    */
   object.exist_prob = 0.95F;
   // Mark all detections as IDM
   for(uint32_t i=0;i<object.ndets;i++){ detections[i].raw.f_idm_det = true; }

   /** \action
    * Call Decrease_EP_Based_On_IDM_Detections().
    */
   Decrease_EP_Based_On_IDM_Detections(detections, object);

   /** \result
    *  EP decreased to 0.65F; fraction stored.
    */
   DOUBLES_EQUAL(0.65F, object.exist_prob, F360_EPSILON);
   DOUBLES_EQUAL(1.0F, object.idm_det_fraction, F360_EPSILON);
}
/** @}*/
