/** \file
 * This file contains unit tests for content of f360_determine_final_vel_estimate.cpp file
 */

#include "f360_determine_final_vel_estimate.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_determine_final_vel_estimate
 *  @{
 */

/** \brief
 * This test group includes tests for the function Determine_Final_Vel_Estimate() defined in
 * f360_determine_final_vel_estimate.cpp which determines the lateral velocity and longitudinal
 * velocity estimate of the initialized object based on the velocities extracted from cloud estimate
 * and position difference using the respective estimate confidences and also outputs the weighting
 * used between the position difference and cloud estimate velocities for calculation.
 */
TEST_GROUP(f360_determine_final_vel_estimate)
{
   // Declare common variables used within all tests in this test group.
   float32_t posdiff_longvel;
   float32_t posdiff_latvel;
   float32_t cloud_longvel;
   float32_t cloud_latvel;
   CONF3_T posdiff_confidence;
   CONF3_T cloud_confidence;
   float32_t longvel_estimate;
   float32_t latvel_estimate;
   /** \setup
    * Set up default values of longitudinal and lateral velocities from cloud velocity estimate and position difference estimate
    */
   TEST_SETUP()
   {
      posdiff_longvel = 2.0F;
      posdiff_latvel = 1.0F;
      cloud_longvel = 4.0F;
      cloud_latvel = 2.0F;
   }
};

/** \purpose
 * Test that the velocity used for track initialization is equally weighted when both cloud estimate and pos diff have high
 * confidence and cloud estimate confidence is not being adjusted inside the function.
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, HighConfidenceCloud_HighConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_HIGH;
   cloud_confidence = CONF3_HIGH;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to weighted track init
    */
   CHECK_TEXT(F360_TRACK_INIT_WEIGHTED == result, "Final Velocity Estimate for track is not classified as weighted between cloud estimate and pos diff when it should be");
}

/** \purpose
 * Test that the track initialization is set to invalid when both cloud estimate and pos diff velocities have low confidence
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, LowConfidenceCloud_LowConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_LOW;
   cloud_confidence = CONF3_LOW;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to invalid init
    */
   CHECK_TEXT(F360_TRACK_INIT_INVALID == result, "Final Velocity Estimate for track is not classified as invalid when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to pos diff only when cloud estimate has low confidence
 * and pos diff has high confidence
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, LowConfidenceCloud_HighConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_HIGH;
   cloud_confidence = CONF3_LOW;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to pos diff track init
    */
   CHECK_TEXT(F360_TRACK_INIT_POSDIFF == result, "Final Velocity Estimate for track is not classified as based on pos diff only when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to cloud estimate only when cloud estimate has high confidence
 * and pos diff has low confidence
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, HighConfidenceCloud_LowConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_LOW;
   cloud_confidence = CONF3_HIGH;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to cloud estimate track init
    */
   CHECK_TEXT(F360_TRACK_INIT_CLOUD == result, "Final Velocity Estimate for track is not classified as based on cloud estimate only when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to equally weighted when cloud estimate has high confidence
 * and pos diff has medium confidence
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, HighConfidenceCloud_MedConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_MED;
   cloud_confidence = CONF3_HIGH;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to weighted track init
    */
   CHECK_TEXT(F360_TRACK_INIT_WEIGHTED == result, "Final Velocity Estimate for track is not classified as weighted between cloud estimate and pos diff when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to pos diff only when cloud estimate has medium confidence
 * and pos diff has high confidence
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, MedConfidenceCloud_HighConfidencePosDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_HIGH;
   cloud_confidence = CONF3_MED;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to pos diff track init
    */
   CHECK_TEXT(F360_TRACK_INIT_POSDIFF == result, "Final Velocity Estimate for track is not classified as based on pos diff only when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to equally weighted when both cloud estimate and pos diff have
 * medium confidence and the lateral and longitudinal velocity difference between cloud and pos diff is below a threshold velocity
 * difference (k_max_vel_diff)
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, MedConfidenceCloud_MedConfidencePosDiff_SmallVelDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose
    */
   posdiff_confidence = CONF3_MED;
   cloud_confidence = CONF3_MED;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to weighted track init
    */
   CHECK_TEXT(F360_TRACK_INIT_WEIGHTED == result, "Final Velocity Estimate for track is not classified as weighted between cloud estimate and pos diff when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to invalid when both cloud estimate and pos diff have medium
 * confidence and only the lateral velocity difference between cloud and pos diff is above a threshold velocity
 * difference (k_max_vel_diff)
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, MedConfidenceCloud_MedConfidencePosDiff_LargeLatVelDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose and set cloud lateral velocity such that the difference
    * in lateral velocity between cloud and pos diff is larger than the threshold velocity difference (k_max_vel_diff)
    */
   cloud_latvel = 5.0F;

   posdiff_confidence = CONF3_MED;
   cloud_confidence = CONF3_MED;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to invalid track init
    */
   CHECK_TEXT(F360_TRACK_INIT_INVALID == result, "Final Velocity Estimate for track is not classified as invalid when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to invalid when both cloud estimate and pos diff have medium
 * confidence and only the longitudinal velocity difference between cloud and pos diff is above a threshold velocity
 * difference (k_max_vel_diff)
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, MedConfidenceCloud_MedConfidencePosDiff_LargeLongVelDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose and set cloud lateral velocity such that the difference
    * in longitudinal velocity between cloud and pos diff is larger than the threshold velocity difference (k_max_vel_diff)
    */
   cloud_longvel = 6.0F;

   posdiff_confidence = CONF3_MED;
   cloud_confidence = CONF3_MED;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to invalid track init
    */
   CHECK_TEXT(F360_TRACK_INIT_INVALID == result, "Final Velocity Estimate for track is not classified as invalid when it should be");
}

/** \purpose
 * Test that the velocity used for track initialization is set to invalid when both cloud estimate and pos diff have medium
 * confidence and both lateral and longitudinal velocity difference between cloud and pos diff is above a threshold velocity
 * difference (k_max_vel_diff)
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, MedConfidenceCloud_MedConfidencePosDiff_LargeLatLongVelDiff)
{
   /** \precond
    * Set appropriate cloud and pos diff confidences as per test purpose and set cloud lateral and longitudinal velocity such that
    * the difference in lateral velocity between cloud and pos diff is larger than the threshold velocity difference (k_max_vel_diff)
    */
   cloud_longvel = 6.0F;
   cloud_latvel = 5.0F;

   posdiff_confidence = CONF3_MED;
   cloud_confidence = CONF3_MED;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to invalid track init
    */
   CHECK_TEXT(F360_TRACK_INIT_INVALID == result, "Final Velocity Estimate for track is not classified as invalid when it should be");
}

/** \purpose
 * Test that the final velocity estimate is set to posdiff after adjusting cloud_confidence to MED. Longvel difference between posdiff and cloud is greater than k_max_vel_diff.
 * \req
 * NA
 */
TEST(f360_determine_final_vel_estimate, Cloud_Confidence_Adjustment_Med_And_Longvel_Diff_Above_Threshold)
{
   /** \precond
    * posdiff_longvel set to 2.0F
    * posdiff_latvel set to 1.0F
    * cloud_longvel set to 6.0F
    * cloud_latvel set to 2.0F
    * posdiff_confidence set to high
    * cloud_confidence set to high
    */
   posdiff_longvel = 2.0F;
   posdiff_latvel = 1.0F;
   cloud_longvel = 6.0F;
   cloud_latvel = 2.0F;

   posdiff_confidence = CONF3_HIGH;
   cloud_confidence = CONF3_HIGH;

   /** \action
    * call Determine_Final_Vel_Estimate().
    */
   F360_Track_Init_T result = Determine_Final_Vel_Estimate(
      posdiff_longvel, posdiff_latvel, cloud_longvel, cloud_latvel, posdiff_confidence, cloud_confidence, longvel_estimate, latvel_estimate);

   /** \result
    * Check that the track initialization enum is set to posdiff track init
    */
   CHECK_TEXT(F360_TRACK_INIT_POSDIFF == result, "Final Velocity Estimate for track is not classified as posdiff when it should be");
}

/** @}*/
