/** \file
 * This file contains unit tests for content of f360_predict_exist_prob.cpp file
 */

#include "f360_predict_exist_prob.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

#include "f360_radar_sensor.h"

using namespace f360_variant_A;

/** \defgroup  calculate_p_persist
 *  @{
 */

/** \brief
 * Test group of Calculate_P_Persist() function. Tests verify whether
 * function properly distinguishes objects that are inside and outside sensor FOV.
 */
TEST_GROUP(calculate_p_persist)
{	
   /** \setup
    * Set up common variables used in tests
    */

   F360_Object_Track_T obj;
   F360_Radar_Sensor_T sens_cal[MAX_NUMBER_OF_SENSORS];
   F360_Calibrations_T calibs;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      sens_cal[0].variable.is_valid = true;
      sens_cal[0].constant.range_limits[F360_DET_LOOK_ID_0] = 50.0F;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether objects outside sensor fov p_persist is equal to p_persist_outside.
 * \req
 * NA.
 */
TEST(calculate_p_persist, calculate_p_persist__obj_outside_fov)
{
   /** \precond
    * Set up object position to be outside sensors FOV
    */
   obj.vcs_position.y = 100.0F;
   obj.vcs_position.x = 10.0F;

   /** \action
    * Call tested function
    */
   const float32_t p_persist = Calculate_P_Persist(obj, sens_cal, calibs);

   /** \result
    * Check whether returned value is equal to 0.66F
    */

   DOUBLES_EQUAL(0.66F, p_persist, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify whether objects outside sensor fov p_persist is equal to p_persist_inside.
* \req
* NA.
*/
TEST(calculate_p_persist, calculate_p_persist__obj_inside_fov)
{
   /** \precond
   * Set up object position to be inside sensors FOV
   */
   obj.vcs_position.y = 0.0F;
   obj.vcs_position.x = 10.0F;

   /** \action
   * Call tested function
   */
   const float32_t p_persist = Calculate_P_Persist(obj, sens_cal, calibs);

   /** \result
   * Check whether returned value is equal to 0.99F
   */

   DOUBLES_EQUAL(0.99F, p_persist, F360_EPSILON);
}
/** @}*/

/** \defgroup  predict_existence_probability
 *  @{
 */

 /** \brief
  * Test group of Calculate_P_Persist() function. Tests verify whether
  * function properly distinguishes objects that are inside and outside sensor FOV.
  */
TEST_GROUP(predict_existence_probability)
{
   /** \setup
    * Set up common variables used in tests
    */

   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Radar_Sensor_T sens_cal[MAX_NUMBER_OF_SENSORS];
   F360_Calibrations_T calibs;
   F360_TRKR_TIMING_INFO_T timing_info;


   const float32_t tess_pass_thr = 1e-6F;

   TEST_SETUP()
   {
      
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      sens_cal[0].variable.is_valid = true;
      sens_cal[0].constant.range_limits[F360_DET_LOOK_ID_0] = 50.0F;
   }
};


/** \purpose
* Purpose of this test is to verify that the prediction of object existence probability works as intended.
* \req NA.
*/
TEST(predict_existence_probability, predict_existence_probability_object_inside_sens_FOV)
{
   /** \precond
   * Set up an object inside sensor FOV with existence probability of 1.0
   */
   const uint32_t obj_idx = 10;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = obj_idx + 1;

   object_tracks[obj_idx].vcs_position.x = 10.0F;
   object_tracks[obj_idx].vcs_position.y = 10.0F;
   object_tracks[obj_idx].exist_prob = 0.82F;

   const float32_t expected_exist_prob = 0.8118F;

   /** \action
   * Call Predict_Existence_Probability
   */
   Predict_Existence_Probability(tracker_info, sens_cal, calibs, object_tracks, timing_info);

   /** \result
   * Verify that existence probability and probability of persistent were set correctly.
   */
   DOUBLES_EQUAL_TEXT(expected_exist_prob, object_tracks[obj_idx].exist_prob, tess_pass_thr, "The predicted existence probability did not match the expected value");

}

/** \purpose
* Purpose of this test is to verify that objects with invalid indexes and inactive objects does not get updated
* \req NA.
*/
TEST(predict_existence_probability, predict_existence_probability_invalid_obj_idx)
{
   /** \precond
   * Set number of active objs to 2 and set first two active obj ids to an invalid ID
   * Set existence probability and probability of persistence to 0.5 for all objects
   */

   // Invalid idx as that will be out of bounds of object track array
   const int32_t invalid_obj_idx = NUMBER_OF_OBJECT_TRACKS; 
   const int32_t invalid_obj_idx2 = -1;
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = invalid_obj_idx + 1;
   tracker_info.active_obj_ids[1] = invalid_obj_idx2 + 1;

   const float32_t expected_exist_prob = 0.5F;
   for (unsigned int i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
   {
      object_tracks[i].exist_prob = expected_exist_prob;
   }

   /** \action
   * Call Predict_Existence_Probability
   */
   Predict_Existence_Probability(tracker_info, sens_cal, calibs, object_tracks, timing_info);

   /** \result
   * Verify that existence probability and probability of persistent were not modified when object ids are invalid or object is inactive
   */
   for (unsigned int i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_exist_prob, object_tracks[i].exist_prob, tess_pass_thr, "The predicted existence probability was updated when it should not have been");
   }
}
/** @}*/
