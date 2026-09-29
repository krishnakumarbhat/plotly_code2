/** \file
 * This file contains unit tests for content of f360_update_object_confidence_levels_helpers.cpp file
 */

#include "f360_update_object_confidence_levels_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  calc_average_confidence_level
 *  @{
 */

/** \brief
* Test group of Calc_Average_Confidence_Level() function. Tests verify whether average confidence level
* is properly filtered in time.
 */
TEST_GROUP(calc_average_confidence_level)
{
   float32_t elapsed_time{};
   F360_Object_Track_T object{};
   
   /** \setup
    * Set object variables that are relevant for average confidence level
    * Initialize elapsed time with base value
    */
   TEST_SETUP()
   {
      object.prev_avrg_conf_level = 0.5F;
      object.time_since_initialization = 1.0F;

      elapsed_time = 0.5F;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether average confidence level is not modified when time since initialization is equal to 0.0F
 * \req
 * NA.
 */
TEST(calc_average_confidence_level, Calc_Average_Confidence_Level__avg_conf_level_not_updated_if_time_since_init_equal_to_zero)
{
   /** \precond
    * Set object time since initialization to 0.0F
    * Save current object confidence level
    */
   object.time_since_initialization = 0.0F;
   const float32_t expected_conf_level = object.prev_avrg_conf_level;
	
   /** \action
    * Call Calc_Average_Confidence_Level
    */
   Calc_Average_Confidence_Level(elapsed_time, object);

   /** \result
    * Check whether object confidence level did not change
    */	
   CHECK_EQUAL(expected_conf_level, object.prev_avrg_conf_level);
}

/** \purpose
 * Purpose of this test is to verify whether average confidence level is properly
 * updated when time since initialziation of track is above zero
 * \req
 * NA.
 */
TEST(calc_average_confidence_level, Calc_Average_Confidence_Level__avg_conf_level_updated_if_time_since_init_above_zero)
{
   /** \precond
    * All is set in TEST_SETUP, do nothing
    */

   /** \action
    * Call Calc_Average_Confidence_Level
    */
   Calc_Average_Confidence_Level(elapsed_time, object);

   /** \result
    * Check whether returned value is equal to 0.25F with threshold
    */
   DOUBLES_EQUAL(0.25F, object.prev_avrg_conf_level, F360_EPSILON);
}
/** @}*/

/** \defgroup  calculate_conf_tau_for_coasted_objects
 *  @{
 */

/** \brief
* Test group of Calculate_Conf_Tau_For_Coasted_Objects() function. Tests verify whether filter coefficient
* is properly calculated for coasted objects.
 */
TEST_GROUP(calculate_conf_tau_for_coasted_objects)
{
   F360_Calibrations_T calib{};
   F360_Object_Track_T object{};

   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether for object that is longitudinally
 * far from host function returns coefficient for relevant tracks
 * \req
 * NA.
 */
TEST(calculate_conf_tau_for_coasted_objects, Calculate_Conf_Tau_For_Coasted_Objects__object_is_longitudinally_far_returns_coeff_for_relevant_tracks)
{
   /** \precond
    * Set object vcs longitudinal position to be within limits
    */
   object.vcs_position.x = calib.k_conf_coasted_max_long_posn_tructed_track + 0.1F;

   /** \action
    * Call Calculate_Conf_Tau_For_Coasted_Objects
    */
   const float32_t coef = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);

   /** \result
    * Check whether returned value is equal to 1.0F (with threshold)
    */
   DOUBLES_EQUAL(1.0F, coef, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether time since initialization of coasted object
 * suggest that it is trusted and its position is within limits - function returns coefficient for relevant tracks
 * \req
 * NA.
 */
TEST(calculate_conf_tau_for_coasted_objects, Calculate_Conf_Tau_For_Coasted_Objects__coasted_object_time_since_init_above_calib_value_within_pos_limits_returns_coeff_for_relevant_tracks)
{
   /** \precond
    * Set object vcs longitudinal position to be within limits
    * Set object time since initialization to be above threshold for trusted coasted track
    */
   object.vcs_position.x = calib.k_conf_coasted_min_long_posn_tructed_track + 0.1F;
   object.time_since_initialization = calib.k_conf_coasted_min_time_trusted_track + 0.1F;

   /** \action
    * Call Calculate_Conf_Tau_For_Coasted_Objects
    */
   const float32_t coef = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);

   /** \result
    * Check whether returned value is equal to 1.0F (with threshold)
    */
   DOUBLES_EQUAL(1.0F, coef, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether time since initialization of coasted object
 * suggest that it is trusted and its position is not within limits - function returns coefficient for medium relevant tracks
 * \req
 * NA.
 */
TEST(calculate_conf_tau_for_coasted_objects, Calculate_Conf_Tau_For_Coasted_Objects__coasted_object_time_since_init_above_calib_value_not_within_pos_limits_returns_coeff_for_medium_relevant_tracks)
{
   /** \precond
    * Set object vcs longitudinal position to be not within limits
    * Set object time since initialization to be above threshold for trusted coasted track
    */
   object.vcs_position.x = calib.k_conf_coasted_min_long_posn_tructed_track - 0.1F;
   object.time_since_initialization = calib.k_conf_coasted_min_time_trusted_track + 0.1F;

   /** \action
    * Call Calculate_Conf_Tau_For_Coasted_Objects
    */
   const float32_t coef = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);

   /** \result
    * Check whether returned value is equal to 0.5F (with threshold)
    */
   DOUBLES_EQUAL(0.5F, coef, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when average confidence level is above threshold
 * defined for medium relevancy tracks - function returns coefficient for medium relevancy tracks
 * \req
 * NA.
 */
TEST(calculate_conf_tau_for_coasted_objects, Calculate_Conf_Tau_For_Coasted_Objects__average_confid_level_above_threshold_returns_coef_for_medium_relevancy_tracks)
{
   /** \precond
    * Set object average confidence level to be above threshold
    */
   object.prev_avrg_conf_level = calib.k_conf_coasted_min_average_confidence_level + 0.1F;

   /** \action
    * Call Calculate_Conf_Tau_For_Coasted_Objects
    */
   const float32_t coef = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);

   /** \result
    * Check whether returned value is equal to 0.5F (with threshold)
    */
   DOUBLES_EQUAL(0.5F, coef, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when object does not meet any conditions to be relevant - function
 * returns coefficient for not relevant tracks
 * \req
 * NA.
 */
TEST(calculate_conf_tau_for_coasted_objects, Calculate_Conf_Tau_For_Coasted_Objects__not_relevant_track_returns_coefficient_for_not_relevant_tracks)
{
   /** \precond
    * Do not modify any of objects parameters - make it not relevant
    */

   /** \action
    * Call Calculate_Conf_Tau_For_Coasted_Objects
    */
   const float32_t coef = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);

   /** \result
    * Check whether returned value is equal to 0.2F (with threshold)
    */
   DOUBLES_EQUAL(0.2F, coef, F360_EPSILON);
}
/** @}*/



/** \defgroup  determine_raw_confidence_level_of_coasted_object
 *  @{
 */

/** \brief
* Test group of Determine_Raw_Confidence_Level_Of_Coasted_Object() function. Tests verify whether
* raw confidence level of coasted object is properly determined.
 */
TEST_GROUP(determine_raw_confidence_level_of_coasted_object)
{
   const float32_t time_since_coasted_track_updated_th = 0.5F;
   F360_Object_Track_T object{};

   /** \setup
    * Set object confidenceLevel to base value
    */
   TEST_SETUP()
   {
      object.confidenceLevel = 0.5F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when time since object was updated is below threshold
 * function returns current object confidence level
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level_of_coasted_object, Determine_Raw_Confidence_Level_Of_Coasted_Object__time_since_track_updated_below_threshold)
{
   /** \precond
    * Set object time since updated to be below threshold
    */
   object.time_since_track_updated = time_since_coasted_track_updated_th - 0.1F;

   /** \action
    * Call Determine_Raw_Confidence_Level_Of_Coasted_Object
    */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Coasted_Object(object, time_since_coasted_track_updated_th);

   /** \result
    * Check whether returned value is equal to object confidence level
    */
   DOUBLES_EQUAL(object.confidenceLevel, raw_conf_level, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when time since object was updated is above threshold
 * function returns current object confidence level
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level_of_coasted_object, Determine_Raw_Confidence_Level_Of_Coasted_Object__time_since_track_updated_above_threshold)
{
   /** \precond
    * Set object time since updated to be above threshold
    */
   object.time_since_track_updated = time_since_coasted_track_updated_th + 0.1F;

   /** \action
    * Call Determine_Raw_Confidence_Level_Of_Coasted_Object
    */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Coasted_Object(object, time_since_coasted_track_updated_th);

   /** \result
    * Check whether returned value is equal to zero
    */
   DOUBLES_EQUAL(0.0F, raw_conf_level, F360_EPSILON);
}
/** @}*/



/** \defgroup  determine_raw_confidence_level_of_updated_object
 *  @{
 */

/** \brief
* Test group of Determine_Raw_Confidence_Level_Of_Updated_Object() function. Tests verify whether
* raw confidence level of updated object is properly determined.
 */
TEST_GROUP(determine_raw_confidence_level_of_updated_object)
{
   const float32_t n_dets_weight = F360_Logf(0.1F);
   const float32_t conf_raw_max_value_not_reduced_dets =  1.0F - F360_Expf(n_dets_weight * 1.0F);
   const float32_t tolerance = 1e-6F;
};

/** \purpose
 * Purpose of this test is to verify whether when object does not have neither reduced dets nor associated detections
 * function returns zero
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__no_reduced_dets_no_associated_dets)
{
   /** \precond
    * Set object_num_rr_inlier_dets to zero
    * Set object_ndets to zero
    */
   const int32_t object_num_rr_inlier_dets = 0;
   const int32_t object_ndets = 0;

   /** \action
    * Call Determine_Raw_Confidence_Level_Of_Updated_Object
    */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);

   /** \result
    * Check whether returned value is equal to zero
    */
   const float32_t expected_raw_confidence_level = 0.0F;
   DOUBLES_EQUAL(expected_raw_confidence_level, raw_conf_level, tolerance);
}

/** \purpose
* Purpose of this test is to verify whether when object has reduced dets
* function returns properly calculated raw confidence level
* \req
* NA.
*/
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__has_1_reduced_det)
{
   /** \precond
   * Set object_num_rr_inlier_dets and object_ndets to 1
   */
   const int32_t object_num_rr_inlier_dets = 1;
   const int32_t object_ndets = 1;

   /** \action
   * Call Determine_Raw_Confidence_Level_Of_Updated_Object
   */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);

   /** \result
   * Check whether returned value is equal to expected value
   */
   const float32_t expected_raw_confidence_level = 0.9F;
   DOUBLES_EQUAL(expected_raw_confidence_level, raw_conf_level, tolerance);
}

/** \purpose
 * Purpose of this test is to verify whether when object has reduced dets
 * function returns properly calculated raw confidence level
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__has_5_reduced_dets)
{
   /** \precond
    * Set object_num_rr_inlier_dets and object_ndets to 5
    */
   const int32_t object_num_rr_inlier_dets = 5;
   const int32_t object_ndets = 5;

   /** \action
    * Call Determine_Raw_Confidence_Level_Of_Updated_Object
    */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);

   /** \result
    * Check whether returned value is equal to expected value
    */
   const float32_t expected_raw_confidence_level = 0.99999F;
   DOUBLES_EQUAL(expected_raw_confidence_level, raw_conf_level, tolerance);
}

/** \purpose
* Purpose of this test is to verify whether when object doesn't have reduced detections, but has associated detections
* function returns properly calculated raw confidence level
* \req
* NA.
*/
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__has_no_reduced_but_has_1_associated_dets)
{
   /** \precond
   * Set object_num_rr_inlier_dets to zero
   * Set object_ndets to 1
   */
   const int32_t object_num_rr_inlier_dets = 0;
   const int32_t object_ndets = 1;

   /** \action
   * Call Determine_Raw_Confidence_Level_Of_Updated_Object
   */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);

   /** \result
   * Check whether returned value is equal to expected value
   */
   const float32_t expected_raw_confidence_level = 0.8F;
   DOUBLES_EQUAL(expected_raw_confidence_level, raw_conf_level, tolerance);
}

/** \purpose
* Purpose of this test is to verify whether when object doesn't have reduced detections, but has associated detections
* function returns properly calculated raw confidence level
* \req
* NA.
*/
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__has_no_reduced_but_has_5_associated_dets)
{
   /** \precond
   * Set object_num_rr_inlier_dets to zero
   * Set object_ndets to 5
   */
   const int32_t object_num_rr_inlier_dets = 0;
   const int32_t object_ndets = 5;

   /** \action
   * Call Determine_Raw_Confidence_Level_Of_Updated_Object
   */
   const float32_t raw_conf_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);

   /** \result
   * Check whether returned value is equal to expected_value
   */
   const float32_t expected_raw_confidence_level = 0.89999F;
   DOUBLES_EQUAL(expected_raw_confidence_level, raw_conf_level, tolerance);
}

/** \purpose
* Purpose of this test is to compare raw confidence calculated for one reduced detection and no reduced, but max associated detections
* \req
* NA.
*/
TEST(determine_raw_confidence_level_of_updated_object, Determine_Raw_Confidence_Level_Of_Updated_Object__confidence_for_one_reduced_det_equal_to_max_associated)
{
   /** \precond
   * Set object_ndets to maximum number of associated detection
   * Set object_num_rr_inlier_dets to 1 and 0 for two separate function executions
   */
   const int32_t object_ndets = MAX_DETS_IN_OBJ_TRK;
   const int32_t object_num_rr_inlier_dets_a = 1;
   const int32_t object_num_rr_inlier_dets_b = 0;

   /** \action
   * Call Determine_Raw_Confidence_Level_Of_Updated_Object twice to calculate confidence with (a) one reduced and (b) no reduced, but max associated dets
   */
   const float32_t raw_conf_level_one_reduced_det = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets_a, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);
   const float32_t raw_conf_level_zero_reduced_max_assoc_dets = Determine_Raw_Confidence_Level_Of_Updated_Object(object_num_rr_inlier_dets_b, object_ndets, n_dets_weight, conf_raw_max_value_not_reduced_dets);


   /** \result
   * Check if raw confidence level for (a) one reduced and (b) no reduced, but max associated dets are approximately equal
   */
   DOUBLES_EQUAL(raw_conf_level_one_reduced_det, raw_conf_level_zero_reduced_max_assoc_dets, tolerance);
}
/** @}*/



/** \defgroup  reduce_confidence_based_on_object_flags
 *  @{
 */

/** \brief
* Test group of Reduce_Confidence_Based_On_Object_Flags() function. Tests verify whether
* object confidence level is reduced if object flags suggest that it should be
 */
TEST_GROUP(reduce_confidence_based_on_object_flags)
{
   const float32_t k_conf_overlapping_reduction_factor = 0.8F;
   F360_Object_Track_T object{};
};

/** \purpose
 * Purpose of this test is to verify whether object confidence level is not reduced when no flag suggest that it should be
 * \req
 * NA.
 */
TEST(reduce_confidence_based_on_object_flags, Reduce_Confidence_Based_On_Object_Flags__no_flags_set)
{
   /** \precond
    * Set up object initial confidence level to 1.0F
    * Do not modify any other object properties
    */
   object.confidenceLevel = 1.0F;

   /** \action
    * Call Reduce_Confidence_Based_On_Object_Flags
    */
   Reduce_Confidence_Based_On_Object_Flags(k_conf_overlapping_reduction_factor, object);

   /** \result
    * Check whether object confidence level was not changed
    */
   DOUBLES_EQUAL(1.0F, object.confidenceLevel, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether object confidence level is properly reduced
 * when object is overlapping with another object
 * \req
 * NA.
 */
TEST(reduce_confidence_based_on_object_flags, Reduce_Confidence_Based_On_Object_Flags__object_overlaps_with_other_object)
{
   /** \precond
    * Set up object initial confidence level to 1.0F
    * Set up object f_overlapping_with_object to true
    * Do not modify any other object properties
    */
   object.confidenceLevel = 1.0F;
   object.f_overlapping_with_object = true;

   /** \action
    * Call Reduce_Confidence_Based_On_Object_Flags
    */
   Reduce_Confidence_Based_On_Object_Flags(k_conf_overlapping_reduction_factor, object);

   /** \result
    * Check whether object confidence level was properly reduced to 0.8F
    */
   DOUBLES_EQUAL(0.8F, object.confidenceLevel, F360_EPSILON)
}
/** @}*/



/** \defgroup  determine_filter_coef
 *  @{
 */

/** \brief
* Test group of Determine_Filter_Coef() function. Tests verify whether
* function properly determines filter coefficient basing on object status and time since initialization
 */
TEST_GROUP(determine_filter_coef)
{
   F360_Object_Track_T object{};
   float32_t elapsed_time{};
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize tracker calibrations
    * Set elapsed_time to 0.05F
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      elapsed_time = 0.05F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when object status is updated and its time since initialization is below threshold
 * function calculates coefficient for recently updated tracks
 * \req
 * NA.
 */
TEST(determine_filter_coef, Determine_Filter_Coef__updated_track_recently_updated_calculates_coeff_for_recently_updated_tracks)
{
   /** \precond
    * Set up object status as updated
    * Set up object time since init to be below threshold
    */
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects - 0.1F;

   /** \action
    * Call Determine_Filter_Coef
    */
   const float32_t filter_coef = Determine_Filter_Coef(object, elapsed_time, calib);

   /** \result
    * Check whether returned filter coefficient is equal to 0.778800786F
    */
   DOUBLES_EQUAL(0.778800786F, filter_coef, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether when object status is coasted and its time since initialization is below threshold
 * function calculates coefficient for recently updated tracks
 * \req
 * NA.
 */
TEST(determine_filter_coef, Determine_Filter_Coef__coasted_track_recently_updated_calculates_coeff_for_recently_updated_tracks)
{
   /** \precond
    * Set up object status as coasted
    * Set up object time since init to be below threshold
    */
   object.status = F360_OBJECT_STATUS_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects - 0.1F;

   /** \action
    * Call Determine_Filter_Coef
    */
   const float32_t filter_coef = Determine_Filter_Coef(object, elapsed_time, calib);

   /** \result
    * Check whether returned filter coefficient is equal to 0.778800786F
    */
   DOUBLES_EQUAL(0.778800786F, filter_coef, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether when object status is coasted but its time since initialization is above threshold
 * function calculates coefficient for coasted tracks
 * \req
 * NA.
 */
TEST(determine_filter_coef, Determine_Filter_Coef__coasted_track_not_updated_recently_calculates_coeff_for_coasted_updated_tracks)
{
   /** \precond
    * Set up object status as coasted
    * Set up object time since init to be above threshold
    */
   object.status = F360_OBJECT_STATUS_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects + 0.1F;

   /** \action
    * Call Determine_Filter_Coef
    */
   const float32_t filter_coef = Determine_Filter_Coef(object, elapsed_time, calib);

   /** \result
    * Check whether returned filter coefficient is equal to 0.7788008F
    */
   DOUBLES_EQUAL(0.7788008F, filter_coef, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether when object status is updated but its time since initialization is above threshold
 * function calculates coefficient for coasted tracks
 * \req
 * NA.
 */
TEST(determine_filter_coef, Determine_Filter_Coef__updated_track_not_updated_recently_calculates_coeff_for_coasted_updated_tracks)
{
   /** \precond
    * Set up object status as updated
    * Set up object time since init to be above threshold
    */
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects + 0.1F;

   /** \action
    * Call Determine_Filter_Coef
    */
   const float32_t filter_coef = Determine_Filter_Coef(object, elapsed_time, calib);

   /** \result
    * Check whether returned filter coefficient is equal to 0.7788008F
    */
   DOUBLES_EQUAL(0.7788008F, filter_coef, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether when object status is new_coasted
 * function calculates coefficient for coasted tracks
 * \req
 * NA.
 */
TEST(determine_filter_coef, Determine_Filter_Coef__new_coasted_calculates_coeff_for_coasted_updated_tracks)
{
   /** \precond
    * Set up object status as new_coasted
    * Set up object time since init to be above threshold
    */
   object.status = F360_OBJECT_STATUS_NEW_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects + 0.1F;

   /** \action
    * Call Determine_Filter_Coef
    */
   const float32_t filter_coef = Determine_Filter_Coef(object, elapsed_time, calib);

   /** \result
    * Check whether returned filter coefficient is equal to 0.7788008F
    */
   DOUBLES_EQUAL(0.7788008F, filter_coef, F360_EPSILON)
}
/** @}*/



/** \defgroup  determine_raw_confidence_level
 *  @{
 */

/** \brief
* Test group of Determine_Raw_Confidence_Level() function. Tests verify whether
* function properly determines filter coefficient basing on object status
 */
TEST_GROUP(determine_raw_confidence_level)
{
   F360_Object_Track_T object{};
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize tracker calibrations
    * Set object number of reduced dets to base value
    * Set object time since update to base value
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      object.num_rr_inlier_dets = 10;
      object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects - 0.01F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when object status is coasted its
 * raw confidence level is properly calculated
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level, Determine_Raw_Confidence_Level__coasted_track)
{
   /** \precond
    * Set up object status as coasted
    */
   object.status = F360_OBJECT_STATUS_COASTED;

   /** \action
    * Call Determine_Raw_Confidence_Level
    */
   const float32_t raw_confidence_level = Determine_Raw_Confidence_Level(object, calib);

   /** \result
    * Check whether returned raw_confidence_level is equal to zero
    */
   DOUBLES_EQUAL(0.0F, raw_confidence_level, F360_EPSILON)
}

/** \purpose
 * Purpose of this test is to verify whether when object status is updated its
 * raw confidence level is properly calculated
 * \req
 * NA.
 */
TEST(determine_raw_confidence_level, Determine_Raw_Confidence_Level__updated_track)
{
   /** \precond
    * Set up object status as updated
    */
   object.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call Determine_Raw_Confidence_Level
    */
   const float32_t raw_confidence_level = Determine_Raw_Confidence_Level(object, calib);

   /** \result
    * Check whether returned raw_confidence_level is equal to 1.0F
    */
   DOUBLES_EQUAL(1.0F, raw_confidence_level, F360_EPSILON)
}
/** @}*/
