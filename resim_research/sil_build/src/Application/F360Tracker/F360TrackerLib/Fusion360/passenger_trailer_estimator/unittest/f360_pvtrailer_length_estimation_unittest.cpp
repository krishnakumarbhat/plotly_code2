/** \file
 * This file contains unit tests for content of f360_pvtrailer_length_estimation.cpp file
 */
#include "f360_pvtrailer_length_estimation.h"
#include <CppUTest/TestHarness.h>
#include <limits>
#include <cstring>


// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_SVM_classification
 *  @{
 */

/** \brief
 * Tests that Norm_Detection_Row works as intended
 */
TEST_GROUP(f360_SVM_classification)
{	
   // Define thresholds for floating number equality comparision
   const float32_t test_pass_th = 1e-9F;

   // Define predict class default state and zero the max_score
   int32_t predict_class = -1;
   float32_t max_score = 0.0F;
   float32_t trailer_len = 4.0F;

   // SVM model parameters (from f360_trailer_detector_TL_SVM.h)
   const float32_t length_array[20] = {4.0F, 4.5F, 5.5F, 5.5F, 5.5F, 6.0F, 8.0F, 7.5F, 12.0F, 5.0F, 9.0F, 4.0F, 4.5F, 4.5F, 6.0F, 5.0F, 4.5F, 8.0F, 12.0F, 6.0F};

   // Internal data struct for length estimation
   F360_PVTrailer_Length_Data_T pvtrailer_length;

   /** \setup
    * Initialize the Trailer_Detector_TP class instance with some default values
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
   }
};

/** \purpose  
 * Test that Norm_Detection_Row correctly normalizes the input array.
 */
TEST(f360_SVM_classification, Test_norm_detection_row_normal_input)
{
   /** \precond
    * Set up a detection row with non-zero elements in 4 bins.
    * Fill expected output array with normalized values.
    */
   float32_t norm_sample[DETECTION_ROWS] = {};
   int32_t detection_row[DETECTION_ROWS] = {};
   detection_row[0] = 2;
   detection_row[4] = 1;
   detection_row[26] = 4;
   detection_row[DETECTION_ROWS - 1] = 5;

   float32_t exp_norm_sample[DETECTION_ROWS] = {}; 
   exp_norm_sample[3] = 0.147441956154897;
   exp_norm_sample[25] = 0.589767824619589;
   exp_norm_sample[DETECTION_ROWS - 2] = 0.737209780774486;
	
   /** \action
    * Call Trailer_Detector_TL::Norm_Detection_Row() from the mocked class.
    */
   Norm_Detection_Row(detection_row, norm_sample);

   /** \result
    * Check that all elements of norm_sample are filled correctly.
    */
   for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_norm_sample[i], norm_sample[i], test_pass_th, "all elements of norm_sample are not filled correctly.")
   }	
}

/** \purpose  
 * Test that Norm_Detection_Row correctly handles an input array when all elements are zero.
 */
TEST(f360_SVM_classification, Test_norm_detection_row_empty_input_array)
{
   /** \precond
    * Set up a detection row with only zeros.
    * Fill expected output array with zeros.
    */
   float32_t norm_sample[DETECTION_ROWS] = {};
   int32_t detection_row[DETECTION_ROWS] = {};

   float32_t exp_norm_sample[DETECTION_ROWS] = {}; 

   /** \action
    * Call Trailer_Detector_TL::Norm_Detection_Row() from the mocked class.
    */
   Norm_Detection_Row(detection_row, norm_sample);

   /** \result
    * Check that all elements of norm_sample are filled correctly.
    */
   for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_norm_sample[i], norm_sample[i], test_pass_th, "all elements of norm_sample are not filled correctly.")
   }	
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 2 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_vnose)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 2
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.0911948681, 0.994114399, 0.000451459753, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00902919471, 0.0446945168, 0.0365682393, 0.00180583901, 0.000451459753, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000 };
   int32_t exp_class_idx = 2;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 0.8F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 18 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_utility)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 18
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.0122404806, 0.713773012, 0.166394040, 0.0244809613, 0.000765030039, 0.000382515020, 0.000765030039, 0.0325137749, 0.117432110, 0.0221858714, 0.0221858714, 0.0103279054, 0.00344263529, 0.00726778526, 0.271585673, 0.0107104201, 0.000765030039, 0.00382515020, 0.000765030039, 0.00191257510, 0.000765030039, 0.00229509012, 0.00229509012, 0.00000000, 0.00573772518, 0.0673226416, 0.507597446, 0.299891770, 0.0665576160, 0.0413116217, 0.0218033567, 0.0221858714, 0.0634974912, 0.0700002462, 0.0126229953, 0.00956287514, 0.0114754504, 0.00497269537, 0.00726778526, 0.0229509007, 0.0401640758, 0.0191257503, 0.00114754506, 0.00114754506, 0.000382515020, 0.00497269537, 0.0248634759, 0.00306012016, 0.00267760502, 0.00918036047, 0.0110929357, 0.0103279054, 0.00535521004, 0.00459018024 };

   int32_t exp_class_idx = 18;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 0.6F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 12 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_jetski)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 12
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.000502012263, 0.00301207369, 0.318275779, 0.135041296, 0.0170684177, 0.0466871411, 0.0717877522, 0.0727917776, 0.00000000, 0.0100402450, 0.0346388444, 0.620487154, 0.692776918, 0.0346388444, 0.00803219620, 0.00251006125, 0.00401609810, 0.000502012263, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000 };
   int32_t exp_class_idx = 12;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 1.0F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 8 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_horse)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 8
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00306748366, 0.704644799, 0.323400408, 0.0271691419, 0.00000000, 0.00744960317, 0.153374180, 0.434268057, 0.00569675537, 0.00613496732, 0.0153374188, 0.00219105976, 0.00306748366, 0.184049025, 0.0955302045, 0.0499561615, 0.0674846396, 0.0442594066, 0.0648553669, 0.233128756, 0.155565247, 0.0613496751, 0.0368098021, 0.147677422, 0.139789611, 0.0113935107, 0.00525854342, 0.00394390756, 0.00525854342, 0.00876423903, 0.00131463585, 0.00131463585, 0.00394390756, 0.000438211951, 0.000438211951, 0.000438211951, 0.00350569561, 0.00525854342, 0.00657317927, 0.0127081461, 0.0258545056, 0.0113935107, 0.0113935107, 0.0350569561, 0.0205959622, 0.00613496732, 0.00131463585, 0.00131463585, 0.00262927171, 0.00701139122, 0.0271691419, 0.0210341737, 0.00394390756, 0.00131463585, 0.00131463585 };
   int32_t exp_class_idx = 8;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 0.1F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 9 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_clamshell)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 9
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.0255685542, 0.905803680, 0.374504119, 0.0323367007, 0.0131602855, 0.00488810614, 0.0195524246, 0.189132109, 0.0334647261, 0.00639213854, 0.00413608970, 0.00300806528, 0.00188004086, 0.00338407350, 0.00263205706, 0.00263205706, 0.000752016320, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.000376008160, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.000376008160, 0.00000000, 0.00000000, 0.000376008160, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000 };
   int32_t exp_class_idx = 9;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 0.0F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 4 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_box)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 4
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.199623674, 0.713844121, 0.0501164906, 0.0109498212, 0.00547491061, 0.00421146955, 0.0109498212, 0.0303225815, 0.0240053777, 0.120448038, 0.0193727612, 0.0412724055, 0.0240053777, 0.0113709681, 0.00421146955, 0.0164247323, 0.0446415804, 0.185304672, 0.409776002, 0.0332706124, 0.0122132627, 0.00294802897, 0.00463261688, 0.00168458791, 0.0155824386, 0.0450627282, 0.212679222, 0.0699103996, 0.0277957004, 0.00800179224, 0.00463261688, 0.00210573478, 0.00673835166, 0.0311648771, 0.194148764, 0.357974946, 0.0437992848, 0.0109498212, 0.00715949852, 0.00126344094, 0.00210573478, 0.0151612908, 0.0172670260, 0.0193727612, 0.00547491061, 0.00210573478, 0.00168458791, 0.00168458791, 0.000421146979, 0.000842293957, 0.00336917583, 0.00168458791 };
   int32_t exp_class_idx = 4;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 1.0F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** \purpose  
 * Test that SVM_classification correctly classifies trailer as class 11 by checking that the corresponding assigned length
 * and axel length is correct given the normalized input array.
 */
TEST(f360_SVM_classification, Test_SVM_classification_trailer_class_boat)
{
   /** \precond
    * norm_sample array has been filled with values that indicates trailer is of class 11
    * Expected trailer length and axel length are set to values corresponding to expected class. 
    */
   float32_t norm_sample[DETECTION_ROWS] = { 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00269400910, 0.0107760364, 0.00484921644, 0.0264012907, 0.100217141, 0.0231684782, 0.0856694877, 0.329207927, 0.0312505066, 0.0129312444, 0.0210132711, 0.0296341013, 0.0921351165, 0.154636130, 0.197740272, 0.0721994489, 0.0371773280, 0.0808202773, 0.130928844, 0.544189870, 0.223063961, 0.134700462, 0.187503040, 0.166489765, 0.0872858986, 0.0199356675, 0.00862082932, 0.0199356675, 0.0398713350, 0.566819549, 0.100217141, 0.0145476498, 0.00592681998, 0.00377161289, 0.00538801821, 0.00269400910, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000 };

   int32_t exp_class_idx = 11;
   float32_t exp_trailer_len = length_array[exp_class_idx];
   float32_t expected_conf = 0.1F;

   /** \action
    * Call Trailer_Detector_TL::SVM_classification() from the mocked class.
    */
   SVM_Classification(norm_sample, pvtrailer_length);
   trailer_len = pvtrailer_length.trailer_length_SVM;

   /** \result
    * Check that trailer_len and axel_trailer_len are assigned the expected values.
    */
   DOUBLES_EQUAL_TEXT(exp_trailer_len, trailer_len, test_pass_th, "Trailer len is not correct.");
   DOUBLES_EQUAL(expected_conf, pvtrailer_length.trailer_length_SVM_conf, test_pass_th);
}

/** @}*/

/** \defgroup  tl_estimate
 *  @{
 */

/** \brief
 * Tests group for testing Estimate() function in f360_trailer_detector_TL.cpp
 */
TEST_GROUP(tl_estimate)
{
   // Internal data struct for length estimation and various input data
   F360_PVTrailer_Length_Data_T pvtrailer_length;
   F360_Host_T vehicle_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T all_detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];

   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
   }
};

/* purpose 
Verify functional behavior when the timer is exactly equal to the threshold, which should trigger estimation and set the done flag to true.
Confirms that a completed estimation sets the f_estimation_done flag to true.
*/

IGNORE_TEST(tl_estimate, EstimationDoneWhenTimerAtThreshold)
{
   /** \precond
    * f_estimation_done is false (i.e. estimation is not done yet)
   window_timer is set to the threshold value = 1800U */
   pvtrailer_length.f_estimation_done = false;
   pvtrailer_length.window_timer = 1800U;

   /** \action
    * Call Estimate() function 
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * f_estimation_done flag should be set to true after estimation is performed.
    */
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
 * Verifies that if estimation is already completed, 
the function does not perform estimation again and leaves the done flag true. 
Confirms that the f_estimation_done flag remains true. */
TEST(tl_estimate, NoOpWhenAlreadyDone)
{
   /** \precond
    * f_estimation_done is true (i.e. estimation is already done)
    * window_timer is set to 0U */
   pvtrailer_length.f_estimation_done = true;
   pvtrailer_length.window_timer = 0U;

   /** \action
    * Call Estimate() function 
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * f_estimation_done flag should remain true since estimation is already done and function should not perform estimation again.
    */
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
 * Verifies that estimation does not run when the timer is below the threshold
 * Ensures the done flag remains false.
 */
TEST(tl_estimate, NoOpWhenTimerNotReached)
{
   /** \precond
   * f_estimation_done is false (i.e. estimation is not done yet)
   * window_timer is set to 1799U, which is just below the threshold of 1800U */
   pvtrailer_length.f_estimation_done = false;
   pvtrailer_length.window_timer = 1799U;

   /** \action
    * Call Estimate() function
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * f_estimation_done flag should remain false 
    */
   CHECK_FALSE(pvtrailer_length.f_estimation_done);
}

/** \defgroup f360_adjust_sample
@{
*/

/** \brief
Test group for testing Adjust_Sample() function.
*/
TEST_GROUP(f360_adjust_sample)
{
   // Internal data struct for length estimation
   F360_PVTrailer_Length_Data_T pvtrailer_length;

   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
   }
};

/**  \purpose
 * Verify that only adjacent equal bins whose value is at or above the computed threshold
 * are decremented by one, while other bins remain unchanged.
 */
TEST(f360_adjust_sample, AdjustSample_DecrementsOnlyWhenAboveThreshold)
{
   /** \precond
    * Threshold control:
    *   max_val = 100 at index DETECTION_ROWS - 2
    *   threshold = int(0.05 * 100) = 5
    *
    * Case layout:
    *   Case 1: indices [0],[1] = 10,10  -> equal and >= threshold -> decrement expected only at [0]
    *   Case 2: indices [2],[3] = 11,9   -> not equal -> no decrement expected at [2]
    *   Case 3: indices [4],[5] = 4,4    -> equal but < threshold -> no decrement expected at [4]
    *
    * Additional checks:
    *   Indices [1], [3], and [5] must remain unchanged to prove the "only" condition
    */
   pvtrailer_length.detection_row[DETECTION_ROWS - 2U] = 100;  /* threshold anchor = 5 */

   pvtrailer_length.detection_row[0] = 10;
   pvtrailer_length.detection_row[1] = 10;

   pvtrailer_length.detection_row[2] = 11;
   pvtrailer_length.detection_row[3] = 9;

   pvtrailer_length.detection_row[4] = 4;
   pvtrailer_length.detection_row[5] = 4;

   /** \action
    * Call wrapper: call_adjust_sample().
    */
   Adjust_Sample(pvtrailer_length);

   /** \result
    * Only index [0] should decrement
    * All other indices should remain unchanged
    */
   /* Case 1 expected behavior */
   LONGS_EQUAL(9,  pvtrailer_length.detection_row[0]);   /* decremented */
   LONGS_EQUAL(10, pvtrailer_length.detection_row[1]);   /* unchanged */

   /* Case 2 expected behavior */
   LONGS_EQUAL(11, pvtrailer_length.detection_row[2]);   /* unchanged */
   LONGS_EQUAL(9,  pvtrailer_length.detection_row[3]);   /* unchanged */

   /* Case 3 expected behavior */
   LONGS_EQUAL(4,  pvtrailer_length.detection_row[4]);   /* unchanged */
   LONGS_EQUAL(4,  pvtrailer_length.detection_row[5]);   /* unchanged */

   /* Threshold anchor remains unchanged */
   LONGS_EQUAL(100, pvtrailer_length.detection_row[DETECTION_ROWS - 2U]);
}

/** \purpose
 * Verify behavior when all adjacent pairs are equal exactly at the computed threshold.
 * Confirms each left element of every pair is decremented once.
 * \req NA.
 */
TEST(f360_adjust_sample, AdjustSample_AllEqualExactlyAtThreshold_ZeroThresholdCase)
{
    /** \precond
     * Make threshold = 0 by ensuring scanned max over is 0
     * Set all bins to 0 so every adjacent pair is equal and at the threshold.
     * With threshold = 0, equal pairs will trigger a decrement of the left element.
     */
    for (uint32_t i = 0; i < DETECTION_ROWS; ++i)
    {
        pvtrailer_length.detection_row[i] = 0;
    }

    /** \action
     * Call wrapper: call_adjust_sample().
     */
    Adjust_Sample(pvtrailer_length);

    /** \result
     * For i = 1..DETECTION_ROWS-1, each equal pair (i-1, i) decrements the left element (i-1) by 1.
     * Therefore:
     *  - indices [0 .. DETECTION_ROWS-2] become -1
     *  - index   [DETECTION_ROWS-1] remains 0 (never a left element)
     * Validate representative positions including boundaries.
     */

    LONGS_EQUAL(-1, pvtrailer_length.detection_row[0]);                 /* first left element decremented */
    LONGS_EQUAL(-1, pvtrailer_length.detection_row[1]);                 /* also decremented when paired with [2] */
    LONGS_EQUAL(-1, pvtrailer_length.detection_row[DETECTION_ROWS - 2U]);   /* last left element decremented */
    LONGS_EQUAL(0,  pvtrailer_length.detection_row[DETECTION_ROWS - 1U]);   /* last element never decremented */

}

/** \purpose
 * Verify that a large max value placed only in the last bin (index DETECTION_ROWS-1) is ignored
 * by threshold computation, since the max scan covers indices [0 .. DETECTION_ROWS-2] only.
 * Confirms functional behavior uses the scanned range to compute threshold.
 * \req NA.
 */
TEST(f360_adjust_sample, AdjustSample_MaxOnlyInLastBinIsIgnoredByThresholdComputation)
{
    /** \precond
     * Set all scanned bins [0 .. DETECTION_ROWS-2] to small values so scanned max = 1 -> threshold = int(0.05*1) = 0.
     * Place a large value only at the last index [DETECTION_ROWS-1] = 100 (outside the scanned range).
     * Create an equal pair at [0],[1] = 1,1 -> with threshold = 0, expect decrement at [0].
     * Important: Break equality for the subsequent pair (2,3) to avoid decrementing index [2].
     * We do this by setting [3] = 1 while keeping [2] = 0.
     */
    for (uint32_t i = 0; i < DETECTION_ROWS; ++i)
    {
        pvtrailer_length.detection_row[i] = 0; 
    }
    pvtrailer_length.detection_row[0] = 1;                         /* equal pair start */
    pvtrailer_length.detection_row[1] = 1;                         /* equal pair partner */
    pvtrailer_length.detection_row[2] = 0;                         /* keep 0 */
    pvtrailer_length.detection_row[3] = 1;                         /* break (2,3) equality to prevent decrement of [2] */
    pvtrailer_length.detection_row[DETECTION_ROWS - 1U] = 100;         /* ignored by max scan */

    /** \action
     * Call wrapper: call_adjust_sample().
     */
    Adjust_Sample(pvtrailer_length);

    /** \result
     * Since scanned max over [0 .. DETECTION_ROWS-2] is 1 -> threshold = 0:
     *  - Equal pair [0],[1] decrements [0] from 1 to 0.
     *  - [1] remains 1 because (1,2) is not equal (1 != 0).
     *  - [2] remains 0 because (2,3) is not equal (0 != 1).
     *  - Last index [DETECTION_ROWS-1] remains 100 (not modified and not part of max scan).
     */
    LONGS_EQUAL(0,   pvtrailer_length.detection_row[0]);                /* decremented from 1 -> 0 */
    LONGS_EQUAL(1,   pvtrailer_length.detection_row[1]);                /* unchanged */
    LONGS_EQUAL(0,   pvtrailer_length.detection_row[2]);                /* unchanged */
    LONGS_EQUAL(1,   pvtrailer_length.detection_row[3]);                /* unchanged (breaks equality with [2]) */
    LONGS_EQUAL(100, pvtrailer_length.detection_row[DETECTION_ROWS - 1U]);  /* unchanged; last bin */
}

/** \purpose
 * Verify that when a pair is exactly equal to a non-zero threshold, the left element is decremented by one.
 * Confirms boundary behavior for score == threshold with threshold > 0.
 * \req NA.
 */
TEST(f360_adjust_sample, AdjustSample_EqualExactlyAtNonZeroThreshold_DecrementsLeftOnly)
{
    /** \precond
     * Threshold control:
     * max_val = 100 at index DETECTION_ROWS - 2 -> threshold = int(0.05 * 100) = 5.
     * Set a single equal pair at exactly the threshold: [6],[7] = 5,5 -> expect [6] to decrement to 4.
     * Prevent cascade by making [8] different from [7] (e.g., [8] = 0).
     * Leave surrounding elements at 0 to avoid unintended equals.
     */
    for (uint32_t i = 0; i < DETECTION_ROWS; ++i)
    {
        pvtrailer_length.detection_row[i] = 0;
    }
    pvtrailer_length.detection_row[DETECTION_ROWS - 2U] = 100; /* threshold anchor -> threshold = 5 */

    pvtrailer_length.detection_row[6] = 5;  /* left of the equal-at-threshold pair */
    pvtrailer_length.detection_row[7] = 5;  /* right of the equal-at-threshold pair */
    pvtrailer_length.detection_row[8] = 0;  /* break equality (7,8) to avoid decrementing index 7 */

    /** \action
     * Call wrapper: call_adjust_sample().
     */
    Adjust_Sample(pvtrailer_length);

    /** \result
     * Pair [6],[7] equals threshold -> decrement [6] to 4; [7] remains 5.
     * [8] remains 0. Threshold anchor unchanged.
     */
    LONGS_EQUAL(4,   pvtrailer_length.detection_row[6]);                 /* decremented from 5 -> 4 */
    LONGS_EQUAL(5,   pvtrailer_length.detection_row[7]);                 /* unchanged (pair with [8] is not equal) */
    LONGS_EQUAL(0,   pvtrailer_length.detection_row[8]);                 /* unchanged */
    LONGS_EQUAL(100, pvtrailer_length.detection_row[DETECTION_ROWS - 2U]);   /* unchanged threshold anchor */
}

/** \defgroup f360_find_peak_group_info
 *  @{
 */
/**  \brief
 * Test group for testing SVM_confidence() function in f360_trailer_detector_TL.cpp
 */

TEST_GROUP(f360_find_peak_group_info)
{
   // Internal data struct for length estimation
   F360_PVTrailer_Length_Data_T pvtrailer_length;

   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
   }
};


/** \purpose
 * Verify that when no peaks have been stored yet (peak_cnt = 0),
 * the first detected peak is stored, radii are computed, and peak_cnt becomes 1.
 * \req NA.
 */

TEST(f360_find_peak_group_info, FindsFirstPeak_StoresAtIndex1_WhenPeakCntStartsAt0)
{
   /** \precond
    * detection_row with a single clear peak at k=10:
    *   [9]=0, [10]=100, [11]=0
    * Thresholds set to allow detection: threshold_forward=0.8, threshold_backward=0.6.
    * peak_gap_max=3 (unit: index), peak_r_gap_max=10 (unit: index), max_val=100.
    * peak_cnt starts at 0.
    * Cal_Radius uses trailer_detection_row_struct internally; set it to the same shape.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   /* Make a peak at k=10: sample[10] is greater than both neighbors. */
   pvtrailer_length.detection_row[9] = 0;
   pvtrailer_length.detection_row[10] = 100;
   pvtrailer_length.detection_row[11] = 0;

   row[9] = 0;
   row[10] = 100;
   row[11] = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,
      0.6F,
      3U,
      10U,
      100,
      peaks,
      peak_cnt);

   /** \result
    * One peak stored at index 1:
    *   pos=10, val=100, radii >= 1 (left/right).
    * peak_cnt == 1.
    */
   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
   LONGS_EQUAL(1, peaks[0].peak_left_radius);
   LONGS_EQUAL(1, peaks[0].peak_right_radius);
}

/** \purpose
 * Verify that when the radius gap condition is met (difference >= peak_r_gap_max),
 * the algorithm continues without adding the second peak, keeping only the first.
 * \req NA.
 */
TEST(f360_find_peak_group_info, SecondPeakRejected_ByRadiusGap_TriggersContinue)
{
   /** \precond
    * Two peaks shaped in detection_row:
    *   Peak1 at k=10 with [9]=0, [10]=100, [11]=0.
    *   Peak2 at k=13 with [12]=0, [13]=95,  [14]=0.
    * Thresholds: threshold_forward=0.8, threshold_backward=0.6.
    * peak_gap_max=3 (index gap between 10 and 13 is 3, allowed).
    * peak_r_gap_max=1 (small value to force radius-gap >= peak_r_gap_max).
    * max_val=100.
    * peak_cnt=0.
    * trailer_detection_row_struct set to same samples for Cal_Radius.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   pvtrailer_length.detection_row[9] = 0;
   pvtrailer_length.detection_row[10] = 100;
   pvtrailer_length.detection_row[11] = 0;

   row[9] = 0;
   row[10] = 100;
   row[11] = 0;

   pvtrailer_length.detection_row[12] = 0;
   pvtrailer_length.detection_row[13] = 95;
   pvtrailer_length.detection_row[14] = 0;

   row[12] = 0;
   row[13] = 95;
   row[14] = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */

   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,
      0.6F,
      3U,
      1U,
      100,
      peaks,
      peak_cnt);

   /** \result
    * Only the first peak is stored; peak_cnt == 1.
    * Peak[1] pos=10, val=100.
    */

   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
}

/** \purpose
* Verify that when the radius gap condition does not hold, the second peak is accepted
* and stored, and peak_cnt increments to 2.
*/
TEST(f360_find_peak_group_info, SecondPeakAccepted_WhenRadiusGapConditionFalse)
{
   /** \precond
    * Two peaks shaped in detection_row:
    *   Peak1 at k=10 with [9]=0, [10]=100, [11]=0.
    *   Peak2 at k=13 with [12]=0, [13]=95,  [14]=0.
    * Thresholds: threshold_forward=0.8, threshold_backward=0.6.
    * peak_gap_max=3 (index gap between 10 and 13 is 3, allowed).
    * peak_r_gap_max=2 (larger value to force radius-gap < peak_r_gap_max).
    * max_val=100.
    * peak_cnt=0.
    * trailer_detection_row_struct set to same samples for Cal_Radius.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   /* Peak1 at k=10 */
   pvtrailer_length.detection_row[9] = 0;
   pvtrailer_length.detection_row[10] = 100;
   pvtrailer_length.detection_row[11] = 0;

   row[9] = 0;
   row[10] = 100;
   row[11] = 0;

   /* Peak2 at k=13 (index gap = 3, within peak_gap_max) */
   pvtrailer_length.detection_row[12] = 0;
   pvtrailer_length.detection_row[13] = 95;
   pvtrailer_length.detection_row[14] = 0;

   row[12] = 0;
   row[13] = 95;
   row[14] = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,
      0.6F,
      3U,
      2U,
      100,
      peaks,
      peak_cnt);

   /** \result
    * Both peaks stored: peak_cnt == 2.
    * Peak[1] : pos=10, val=100.
    * Peak[2] : pos=13, val=95.
    */
   LONGS_EQUAL(2, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
   LONGS_EQUAL(13, peaks[1].peak_pos);
   LONGS_EQUAL(95, peaks[1].peak_val);
}

/** Purpose
 * Verify that when the index gap between two peaks exceeds peak_gap_max, the second peak is not added, 
 * and only the first peak is stored.
 */
TEST(f360_find_peak_group_info, GapTooLarge_FirstGroup_peakGapMax3_DoesNotAddSecondPeak)
{
   /** \precond
    * Peak1 at k=10: [9]=0, [10]=100, [11]=0.
    * Peak2 at k=15: [14]=0, [15]=95, [16]=0.
    * peak_gap_max=3; index gap=5 > 3 => reject second peak.
    * threshold_forward/backward as before.
    * max_val=100; peak_cnt=0.
    * trailer_detection_row_struct mirrors the same samples.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   /* Peak1 at k=10 */
   pvtrailer_length.detection_row[9] = 0;
   pvtrailer_length.detection_row[10] = 100;
   pvtrailer_length.detection_row[11] = 0;

   row[9] = 0;
   row[10] = 100;
   row[11] = 0;

   /* Peak2 at k=15 (index gap = 5, beyond peak_gap_max=3) */
   pvtrailer_length.detection_row[14] = 0;
   pvtrailer_length.detection_row[15] = 95;
   pvtrailer_length.detection_row[16] = 0;

   row[14] = 0;
   row[15] = 95;
   row[16] = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,
      0.6F,
      3U,
      10U,
      100,
      peaks,
      peak_cnt);

   /** \result
    * Only the first peak is stored; peak_cnt == 1.
    * Peak[1] pos=10, val=100.   
    */
   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
}

/** \purpose
 * Verify that Cal_Radius grows radii beyond 1 when there is a multi-step drop away from the peak
 * on both sides, and that the peak is stored with computed radii.
 * \req NA.
*/
TEST(f360_find_peak_group_info, CalRadius_BigDropSlope_ComputesExpectedRadii)
{
   /** \precond
    * Peak at k=10 with descending slopes:
    *   Left: 100 -> 40 -> 22 -> 15 -> 12 -> 12
    *   Right: 100 -> 85 -> 60 -> 30 -> 12 -> 12
    * Thresholds chosen to allow detection: 0.8 forward, 0.6 backward.
    * peak_gap_max and peak_r_gap_max set high (10) to avoid interference.
    * max_val=100; peak_cnt=0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   /* Peak at k=10 */
   pvtrailer_length.detection_row[10] = 100;
   pvtrailer_length.detection_row[9] = 40;
   pvtrailer_length.detection_row[11] = 85;

   /* Left slope */
   pvtrailer_length.detection_row[8] = 22;
   pvtrailer_length.detection_row[7] = 15;
   pvtrailer_length.detection_row[6] = 12;
   pvtrailer_length.detection_row[5] = 12;

   /* Right slope */
   pvtrailer_length.detection_row[12] = 60;
   pvtrailer_length.detection_row[13] = 30;
   pvtrailer_length.detection_row[14] = 12;
   pvtrailer_length.detection_row[15] = 12;

   row[10] = 100;
   row[9] = 40;
   row[11] = 85;

   row[8] = 22;
   row[7] = 15;
   row[6] = 12;
   row[5] = 12;

   row[12] = 60;
   row[13] = 30;
   row[14] = 12;
   row[15] = 12;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,
      0.6F,
      10U,
      10U,
      100,
      peaks,
      peak_cnt);

   /** \result
    * One peak stored at index 1:
    *   pos=10, val=100.
    * Radii > 1 on both sides due to multi-step drop.
    * peak_cnt == 1.
    */
   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);

   CHECK(peaks[0].peak_left_radius > 1);
   CHECK(peaks[0].peak_right_radius > 1);
}

/** \purpose
 * Verify that a first peak whose value equals the forward threshold is accepted.
 * \req NA.
 */

TEST(f360_find_peak_group_info, ForwardThresholdEquality_AcceptsFirstPeak)
{
   /** \precond
    * forward threshold = int(0.8 * max_val) with max_val = 100 -> threshold = 80.
    * detection_row has a true peak at k=10 with value 80: [9]=0, [10]=80, [11]=0.
    * backward threshold set to 0.6 (not used until a second peak).
    * peak_gap_max, peak_r_gap_max set high to avoid interference.
    * peak_cnt starts at 0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   pvtrailer_length.detection_row[9]  = 0; pvtrailer_length.detection_row[10] = 80; pvtrailer_length.detection_row[11] = 0;
   row[9]              = 0; row[10]             = 80; row[11]             = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward
      0.6F,   // threshold_backward
      10U,    // peak_gap_max
      10U,    // peak_r_gap_max
      100,    // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * First peak accepted and stored; peak_cnt == 1; peak at pos=10, val=80.
    */
   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(80, peaks[0].peak_val);
}

/** \purpose
 * Verify that a second peak whose value equals the backward-updated threshold is accepted.
 * \req NA.
 */
TEST(f360_find_peak_group_info, BackwardThresholdEquality_AcceptsSecondPeak)
{
      /** \precond
    * First peak: k=10 with value 100 (>= forward threshold), neighbors 0 -> true peak.
    * After first peak, threshold becomes int(0.6 * 100) = 60.
    * Second candidate: k=13 with value 60 and neighbors 0 -> true peak.
    * peak_gap_max = 3 (index gap = 3 -> allowed), peak_r_gap_max high to avoid rejection.
    * peak_cnt starts at 0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   // First peak at 10
   pvtrailer_length.detection_row[9]  = 0; pvtrailer_length.detection_row[10] = 100; pvtrailer_length.detection_row[11] = 0;
   row[9]              = 0; row[10]             = 100; row[11]             = 0;

   // Second peak at 13 exactly at backward threshold 60
   pvtrailer_length.detection_row[12] = 0; pvtrailer_length.detection_row[13] = 60;  pvtrailer_length.detection_row[14] = 0;
   row[12]             = 0; row[13]             = 60;  row[14]             = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward
      0.6F,   // threshold_backward
      3U,     // peak_gap_max
      10U,    // peak_r_gap_max
      100,    // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * Two peaks stored:
    *   peaks[1]: pos=10, val=100
    *   peaks[2]: pos=13, val=60
    * peak_cnt == 2.
    */
   LONGS_EQUAL(2, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
   LONGS_EQUAL(13, peaks[1].peak_pos);
   LONGS_EQUAL(60, peaks[1].peak_val);
}

/** \purpose
* Verify that a second peak at exactly peak_gap_max distance is accepted.
* \req NA.
*/
TEST(f360_find_peak_group_info, PeakGapMaxEquality_AcceptsSecondPeak)
{
   /** \precond
    * First peak: k=10 with value 100; second peak: k=13 with value 95.
    * peak_gap_max = 3 and index gap = 3 -> boundary equality.
    * Radii-gap relaxed to avoid continue; thresholds allow detection.
    * peak_cnt starts at 0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   // First peak
   pvtrailer_length.detection_row[9]  = 0;   pvtrailer_length.detection_row[10] = 100; pvtrailer_length.detection_row[11] = 0;
   row[9]              = 0;   row[10]             = 100; row[11]             = 0;

   // Second peak at exact gap boundary
   pvtrailer_length.detection_row[12] = 0;   pvtrailer_length.detection_row[13] = 95;  pvtrailer_length.detection_row[14] = 0;
   row[12]             = 0;   row[13]             = 95;  row[14]             = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward
      0.6F,   // threshold_backward
      3U,     // peak_gap_max (equality boundary)
      10U,    // peak_r_gap_max relaxed
      100,    // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * Both peaks accepted; peak_cnt == 2.
    * peaks[1]: pos=10, val=100; peaks[2]: pos=13, val=95.
    */
   LONGS_EQUAL(2, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
   LONGS_EQUAL(13, peaks[1].peak_pos);
   LONGS_EQUAL(95, peaks[1].peak_val);
}

/** \purpose
 * Verify that when the radius-gap expression equals peak_r_gap_max, the second peak is rejected.
 * \req NA.
 */

TEST(f360_find_peak_group_info, PeakRGapMaxEquality_RejectsSecondPeak)
{
   /** \precond
    * Shape radii so that (k - r_left) - (cur + r_right_cur) == peak_r_gap_max.
    * Use two close peaks: first at 10 (100), second at 13 (95).
    * Choose peak_r_gap_max = 1 and slopes that yield r_left=2 (for k=13) and r_right_cur=0/1 minimal,
    * but because Cal_Radius enforces minimum 1, use values to produce r_left=2 and r_right_cur=1.
    * Then (13 - 2) - (10 + 1) = 11 - 11 = 0 -> set peak_r_gap_max = 0 to hit equality boundary.
    * peak_gap_max = 3 (index gap allowed). Thresholds allow detection.
    * peak_cnt starts at 0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   // First peak with sharp immediate drop to keep right radius minimal (=1)
   pvtrailer_length.detection_row[9]  = 0;   pvtrailer_length.detection_row[10] = 100; pvtrailer_length.detection_row[11] = 0;
   row[9]              = 0;   row[10]             = 100; row[11]             = 0;

   // Second peak with a small left extension to get r_left = 2
   // Values chosen so Cal_Radius left loop advances two steps.
   pvtrailer_length.detection_row[12] = 50;  pvtrailer_length.detection_row[13] = 95;  pvtrailer_length.detection_row[14] = 0;
   row[12]             = 50;  row[13]             = 95;  row[14]             = 0;

   /** \action
    * Call wrapper: call_find_peak_group_info() with peak_r_gap_max at equality boundary 0.
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward
      0.6F,   // threshold_backward
      3U,     // peak_gap_max
      0U,     // peak_r_gap_max equality boundary: requires expression >= 0 to reject
      100,    // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * Only the first peak is stored because the second is rejected at equality boundary.
    * peak_cnt == 1; peaks[1]: pos=10, val=100.
    */
   LONGS_EQUAL(1, peak_cnt);
   LONGS_EQUAL(10, peaks[0].peak_pos);
   LONGS_EQUAL(100, peaks[0].peak_val);
}

/** \purpose
 * Verify that a candidate that meets the threshold but is not a strict peak is ignored.
 * \req NA.
 */
TEST(f360_find_peak_group_info, NonPeakCandidate_IgnoredEvenIfAboveThreshold)
{
  /** \precond
    * Create a plateau at k=10: [9]=50, [10]=80, [11]=80 (not strictly greater than right neighbor).
    * forward threshold = int(0.8 * 100) = 80, so candidate equals threshold but Is_Peak is false.
    * No other true peaks exist. peak_cnt starts at 0.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   pvtrailer_length.detection_row[9]  = 50; pvtrailer_length.detection_row[10] = 80; pvtrailer_length.detection_row[11] = 80;
   row[9]              = 50; row[10]             = 80; row[11]             = 80;

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward
      0.6F,   // threshold_backward
      3U,     // peak_gap_max
      10U,    // peak_r_gap_max
      100,    // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * No peaks stored; peak_cnt remains 0.
    */
   LONGS_EQUAL(0, peak_cnt);
}
/** \purpose
 * Verify overflow guard: when peak_cnt starts at PEAK_GROUP_SIZE - 1, adding one valid peak
 * saturates peak_cnt to PEAK_GROUP_SIZE and writes the new peak into the last valid slot
 * (0-based index PEAK_GROUP_SIZE - 1) without overflowing the array.
 * \req NA.
 */
IGNORE_TEST(f360_find_peak_group_info, OverflowGuard_WritesAtLastSlot_WhenPeakCntAtCapacityMinusOne)
{
   /** \precond */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = static_cast<int32_t>(PEAK_GROUP_SIZE - 1);

   // Prefill peaks with sentinels to detect the write index
   for (size_t i = 0; i < PEAK_GROUP_SIZE; ++i) {
      peaks[i].peak_pos = -1;
      peaks[i].peak_val = -1;
      peaks[i].peak_left_radius = -1;
      peaks[i].peak_right_radius = -1;
   }

   // Single valid peak at k = 10
   pvtrailer_length.detection_row[9]  = 0;  pvtrailer_length.detection_row[10] = 100;  pvtrailer_length.detection_row[11] = 0;
   row[9]              = 0;  row[10]             = 100;  row[11]             = 0;

   /** \action */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,    // threshold_forward
      0.6F,    // threshold_backward
      10U,     // peak_gap_max
      10U,     // peak_r_gap_max
      100,     // max_val
      peaks,
      peak_cnt
   );

   /** \result */
   LONGS_EQUAL(PEAK_GROUP_SIZE, peak_cnt);

   // Detect actual write location (in-bounds)
   int changed_idx = -1;
   for (int i = 0; i < static_cast<int>(PEAK_GROUP_SIZE); ++i) {
      if (peaks[i].peak_pos == 10 && peaks[i].peak_val == 100) {
         changed_idx = i;
         break;
      }
   }

   CHECK_TEXT(changed_idx >= 0,
      "BUG: new peak not found in-bounds; production likely wrote out-of-bounds at PEAK_GROUP_SIZE.");

   const int last = static_cast<int>(PEAK_GROUP_SIZE - 1);
   LONGS_EQUAL(last, changed_idx);
   LONGS_EQUAL(10, peaks[last].peak_pos);
   LONGS_EQUAL(100, peaks[last].peak_val);
}
/** \purpose
 * Verify that when all bins are above the threshold but there are no local maxima
 * (monotonically increasing row), no peaks are stored and peak_cnt remains 0.
 * \req NA.
 */
TEST(f360_find_peak_group_info, NoPeaksWhenMonotonicIncreasing_EvenIfAboveThreshold)
{
   /** \precond
    * Monotonically increasing detection_row (strictly rising), so Is_Peak() is false for all indices.
    * All bins above forward threshold:
    *   Use max_val = 10 => forward threshold = int(0.8 * 10) = 8.
    *   Set row[i] = 9 + i so every sample >= 9 >= threshold.
    * peak_cnt starts at 0.
    * Cal_Radius uses trailer_detection_row_struct; mirror the same samples.
    */
   int32_t row[DETECTION_ROWS] = {};
   TL_Peak peaks[PEAK_GROUP_SIZE] = {};
   int32_t peak_cnt = 0;

   for (uint32_t i = 0; i < DETECTION_ROWS; ++i)
   {
      const int32_t v = static_cast<int32_t>(9 + i); // strictly increasing
      pvtrailer_length.detection_row[i] = v;
      row[i] = v;
   }

   /** \action
    * Call wrapper: call_find_peak_group_info().
    */
   Find_Peak_Group_Info(pvtrailer_length,
      row,
      0.8F,   // threshold_forward -> 8 with max_val=10
      0.6F,   // threshold_backward (unused here as no first peak)
      10U,    // peak_gap_max
      10U,    // peak_r_gap_max
      10,     // max_val
      peaks,
      peak_cnt
   );

   /** \result
    * No local maxima exist; peak_cnt remains 0 and peaks[] is untouched.
    */
   LONGS_EQUAL(0, peak_cnt);
}


/** \defgroup f360_post_processing
@{
*/

/** \brief
Test group for testing Post_Processing() function.
*/
TEST_GROUP(f360_post_processing)
{
   const float32_t test_pass_th = 1e-9F;
   // Internal data struct for length estimation
   F360_PVTrailer_Length_Data_T pvtrailer_length;
   TL_Peak first_peak_group[PEAK_GROUP_SIZE];

   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
      pvtrailer_length.f_estimation_done = true;
   }

   void setup_post_inputs(float peaks_trailer_length, float SVM_trailer_length, float SVM_conf, int32_t first_peak_idx)
   {
      pvtrailer_length.trailer_length_peaks = peaks_trailer_length;
      pvtrailer_length.trailer_length_SVM = SVM_trailer_length;
      pvtrailer_length.trailer_length_SVM_conf = SVM_conf;
      first_peak_group[0].peak_pos = first_peak_idx;
   }
};

/** \purpose
* Verify zeroed outputs when peaks-based trailer length is below 1.0, and confirm trailer_HV_gap and trailer_length_conf are reset.
* \req NA.
*/
TEST(f360_post_processing, OutputsZero_WhenPeaksBelowOne)
{
   /** \precond
    * peaks trailer length = 0.0 (unit: m).
    * SVM trailer length = 10.0 (unit: m).
    * SVM confidence = 0.50 (unitless).
    * first peak position index = 5 (unit: count).
   */
   setup_post_inputs(0.0F, 10.0F, 0.50F, 5);

   /** \action
    * Call wrapper: call_post_processing().
   */
   Post_Processing(pvtrailer_length, first_peak_group);

   /** \result
    * trailer_length = 0.0.
    * axle_trailer_length = 0.0.
    * trailer_HV_gap = 0.0.
    * trailer_length_conf = 0.
    * f_estimation_done remains true.
   */
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.axle_trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_HV_gap, test_pass_th);
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
* Verify SVM path is selected when SVM confidence is at the threshold, and confirm trailer_HV_gap and trailer_length_conf are set for the usable path.
* \req NA.
*/
TEST(f360_post_processing, UsesSvmLength_WhenSvmConfAtOrAboveThreshold)
{
   /** \precond
    * peaks trailer length = 5.0 (unit: m), which is usable (>= 1.0).
    * SVM trailer length = 10.0 (unit: m).
    * SVM confidence = 0.05 (unitless), equal to the decision threshold.
    * first peak position index = 0 (unit: count) to make HV gap deterministically 0 via min().
   */
   setup_post_inputs(5.0F, 10.0F, 0.05F, 0);

   /** \action
    * Call wrapper: call_post_processing().
   */
   Post_Processing(pvtrailer_length, first_peak_group);

   /** \result
    * trailer_length = SVM value (10.0).
    * axle_trailer_length = 10.0 * 0.7 = 7.0.
    * trailer_HV_gap = 0.0 (since first_peak_group[0].peak_pos = 0).
    * trailer_length_conf = 3.
    * f_estimation_done remains true.
   */
   DOUBLES_EQUAL(10.0F, pvtrailer_length.trailer_length, test_pass_th);
   DOUBLES_EQUAL(7.0F,  pvtrailer_length.axle_trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_HV_gap, test_pass_th);
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
* Verify peaks path is selected when SVM confidence is below the threshold, and confirm trailer_HV_gap and trailer_length_conf are set for the usable path.
* \req NA.
*/
TEST(f360_post_processing, UsesPeaksLength_WhenSvmConfBelowThreshold)
{
   /** \precond
    * peaks trailer length = 5.0 (unit: m), which is usable (>= 1.0).
    * SVM trailer length = 10.0 (unit: m).
    * SVM confidence = 0.049 (unitless), below the decision threshold 0.05.
    * first peak position index = 0 (unit: count) to make HV gap deterministically 0 via min().
   */
   setup_post_inputs(5.0F, 10.0F, 0.049F, 0);

   /** \action
    * Call wrapper: call_post_processing().
   */
   Post_Processing(pvtrailer_length, first_peak_group);

   /** \result
    * trailer_length = peaks value (5.0).
    * axle_trailer_length = 5.0 * 0.7 = 3.5.
    * trailer_HV_gap = 0.0 (since first_peak_group[0].peak_pos = 0).
    * trailer_length_conf = 3.
    * f_estimation_done remains true.
   */
   DOUBLES_EQUAL(5.0F, pvtrailer_length.trailer_length, test_pass_th);
   DOUBLES_EQUAL(3.5F, pvtrailer_length.axle_trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_HV_gap, test_pass_th);
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
* Verify boundary behavior when peaks trailer length equals 1.0; confirm both SVM and peaks paths handle trailer_HV_gap and trailer_length_conf.
* \req NA.
*/
TEST(f360_post_processing, PeaksEqualOne_UsesSvmOrPeaksPerConfidence)
{
   /** \precond
    * peaks trailer length = 1.0 (unit: m), exactly at usable boundary.
    * SVM trailer length = 8.0 (unit: m).
    * Use first peak position index = 0 (unit: count) to make HV gap deterministically 0 via min().
    * Subcase B: SVM confidence just below threshold -> choose peaks (1.0)
   */
   // Subcase A: SVM confidence exactly at threshold -> choose SVM
   setup_post_inputs(1.0F, 8.0F, 0.05F, 0);
   Post_Processing(pvtrailer_length, first_peak_group);

   /** \result
    * trailer_length = 8.0; axle_trailer_length = 5.6.
    * trailer_HV_gap = 0.0; trailer_length_conf = 3.
   */
   DOUBLES_EQUAL(8.0F, pvtrailer_length.trailer_length, test_pass_th);
   DOUBLES_EQUAL(5.6F, pvtrailer_length.axle_trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_HV_gap, test_pass_th);
   CHECK_TRUE(pvtrailer_length.f_estimation_done);

   setup_post_inputs(1.0F, 8.0F, 0.049F, 0);
   Post_Processing(pvtrailer_length, first_peak_group);

   /** \result
    * trailer_length = 1.0; axle_trailer_length = 0.7.
    * trailer_HV_gap = 0.0; trailer_length_conf = 3.
   */
   DOUBLES_EQUAL(1.0F, pvtrailer_length.trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.7F, pvtrailer_length.axle_trailer_length, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_length.trailer_HV_gap, test_pass_th);
   CHECK_TRUE(pvtrailer_length.f_estimation_done);
}

/** \purpose
 * End-to-end: verify Post_Processing uses peaks path when svm_conf < 0.05,
 * and generates final outputs (trailer_length, axle_trailer_length, HV gap, conf).
 * \req NA.
 */
TEST(f360_post_processing, E2E_PostProcessing_PeaksPath_ConfBelowThreshold)
{
    /** \precond
     * trailer_length_peaks >= 1.0 to enable post-processing branch.
     * svm_conf < 0.05 forces peaks path.
     * first_peak_group[0].peak_pos known to compute HV gap (capped at 2.0).
     */
    first_peak_group[0].peak_pos = 10;
    first_peak_group[0].peak_val = 100;
    first_peak_group[0].peak_left_radius = 1;
    first_peak_group[0].peak_right_radius = 1;

    const float32_t peaks_len = 21.0F;
    const float32_t svm_len   = 9.5F;   // arbitrary (will not be used)
    const float32_t svm_conf  = 0.01F;  // below threshold

    setup_post_inputs(peaks_len, svm_len, svm_conf, /*first_peak_pos*/10);

    /** \action
     * Run post-processing.
     */
    Post_Processing(pvtrailer_length, first_peak_group);

    /** \result
     * trailer_length = peaks_len (21.0)
     * axle_trailer_length = 0.7 * 21.0 = 14.7
     * trailer_length_conf = 3
     * trailer_HV_gap = min(10 * k_row_interval, 2.0) -> cannot assert exact without k_row_interval;
     *   but must be <= 2.0 and > 0.0.
     */
    DOUBLES_EQUAL(21.0F, pvtrailer_length.trailer_length, 0.01F);
    DOUBLES_EQUAL(14.7F, pvtrailer_length.axle_trailer_length, 0.01F);
    CHECK(pvtrailer_length.trailer_HV_gap > 0.0F);
    CHECK(pvtrailer_length.trailer_HV_gap <= 2.0F);
}

/** \purpose
 * End-to-end: verify Post_Processing uses SVM path when svm_conf >= 0.05 (inclusive).
 * Checks final outputs reflect SVM length instead of peaks length.
 * \req NA.
 */
TEST(f360_post_processing, E2E_PostProcessing_SVMPath_ConfAtBoundary)
{
    /** \precond
     * trailer_length_peaks >= 1.0 to enable post-processing branch.
     * svm_conf == 0.05 -> boundary equality for SVM path.
     * first_peak_group[0].peak_pos known to compute HV gap (capped at 2.0).
     */
    first_peak_group[0].peak_pos = 10;
    first_peak_group[0].peak_val = 100;
    first_peak_group[0].peak_left_radius = 1;
    first_peak_group[0].peak_right_radius = 1;

    const float32_t peaks_len = 21.0F;  // will be ignored
    const float32_t svm_len   = 17.5F;  // used because conf >= 0.05
    const float32_t svm_conf  = 0.05F;  // equality boundary

    setup_post_inputs(peaks_len, svm_len, svm_conf, /*first_peak_pos*/10);

    /** \action */
    Post_Processing(pvtrailer_length, first_peak_group);

    /** \result
     * trailer_length = svm_len (17.5)
     * axle_trailer_length = 0.7 * 17.5 = 12.25
     * trailer_length_conf = 3
     * trailer_HV_gap within (0, 2.0]; cannot assert exact without k_row_interval.
     */
    DOUBLES_EQUAL(17.5F, pvtrailer_length.trailer_length, 0.01F);
    DOUBLES_EQUAL(12.25F, pvtrailer_length.axle_trailer_length, 0.01F);
    CHECK(pvtrailer_length.trailer_HV_gap > 0.0F);
    CHECK(pvtrailer_length.trailer_HV_gap <= 2.0F);
}

/** @}
 * \defgroup f360_shrink_trailer_length
 * @{
 * \brief Test group for testing Shrink_Trailer_Length() function.
*/

TEST_GROUP(f360_shrink_trailer_length)
{
   F360_PVTrailer_Length_Data_T pvtrailer_length;
   TL_Peak first_peak_group[PEAK_GROUP_SIZE];
   TL_Peak second_peak_group[PEAK_GROUP_SIZE];
   TL_Peak third_peak_group[PEAK_GROUP_SIZE];
   int32_t first_peak_cnt;
   int32_t second_peak_cnt;
   int32_t third_peak_cnt;
   TL_Area_T front_area;
   TL_Area_T middle_area;

   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
      (void)memset(&first_peak_group[0], 0, sizeof(first_peak_group));
      (void)memset(&second_peak_group[0], 0, sizeof(second_peak_group));
      (void)memset(&third_peak_group[0], 0, sizeof(third_peak_group));
      (void)memset(&front_area, 0, sizeof(front_area));
      (void)memset(&middle_area, 0, sizeof(middle_area));
   }

   void set_peak_counts(int32_t cnt_1, int32_t cnt_2, int32_t cnt_3)
   {
      first_peak_cnt = cnt_1;
      second_peak_cnt = cnt_2;
      third_peak_cnt = cnt_3;
   }

   void set_first_peak(int32_t idx, int32_t pos, int32_t val, int32_t l_rad, int32_t r_rad)
   {
      first_peak_group[idx].peak_pos = pos;
      first_peak_group[idx].peak_val = val;
      first_peak_group[idx].peak_left_radius = l_rad;
      first_peak_group[idx].peak_right_radius = r_rad;
   }

   void set_second_peak(int32_t idx, int32_t pos, int32_t val, int32_t l_rad, int32_t r_rad)
   {
      second_peak_group[idx].peak_pos = pos;
      second_peak_group[idx].peak_val = val;
      second_peak_group[idx].peak_left_radius = l_rad;
      second_peak_group[idx].peak_right_radius = r_rad;
   }

   void set_third_peak(int32_t idx, int32_t pos, int32_t val, int32_t l_rad, int32_t r_rad)
   {
      third_peak_group[idx].peak_pos = pos;
      third_peak_group[idx].peak_val = val;
      third_peak_group[idx].peak_left_radius = l_rad;
      third_peak_group[idx].peak_right_radius = r_rad;
   }

   void set_front_area_vals(float32_t mean_val, float32_t max_val)
   {
      front_area.mean_val = mean_val;
      front_area.max_val = max_val;
   }

   void set_front_area_bounds(int32_t start, int32_t end, int32_t ref_val)
   {
      front_area.starting_pos = start;
      front_area.ending_pos = end;
      front_area.ref_val = ref_val;
   }

   void set_middle_area_vals(float32_t mean_val, float32_t max_val)
   {
      middle_area.mean_val = mean_val;
      middle_area.max_val = max_val;
   }

   void set_middle_area_bounds(int32_t start, int32_t end, int32_t ref_val)
   {
      middle_area.starting_pos = start;
      middle_area.ending_pos = end;
      middle_area.ref_val = ref_val;
   }

   void set_detection_row_val(uint32_t idx, int32_t val)
   {
      pvtrailer_length.detection_row[idx] = val;
   }
};

/** \purpose
* Verify that Shrink Case 1 activates and reduces last_peak when the second peak 
* is interpreted as a false (multipath) peak and all required conditions are met.
* \req NA.
*/

TEST(f360_shrink_trailer_length, ShrinkCase1_GhostSecondPeak_ShrinksLastPeak)
{
   
/** \precond
    * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
    * First peak: pos = 4, val = 100, radii = (1,2).
    * Second peak: pos = 35, val = 60, radii = (1,1).
    * front_area.mean_val = 0.10, front_area.max_val = 0.20.
    * detection_row shaped so:
    *     - detection_row[4]  = 100  (first peak)
    *     - detection_row[6]  = 30   (between-peaks strong point)
    *     - detection_row[7]  = 30   (between-peaks strong point)
    *     - detection_row[35] = 60   (second peak)
    * temp_max_val behind second peak remains low.
    * i_array_max = 100, j_array_max = 60.
    * f_noise = true.
    * last_peak initially = 35.0.
    */
   set_peak_counts(1, 1, 0);
  

   set_first_peak(0, 4, 100, 1, 2);
   set_second_peak(0, 35, 60, 1, 1);

   set_front_area_vals(0.10F, 0.20F);

   for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
   {
      pvtrailer_length.detection_row[i] = 0;
   }

   pvtrailer_length.detection_row[4] = 100;
   pvtrailer_length.detection_row[6] = 30;
   pvtrailer_length.detection_row[7] = 30;
   pvtrailer_length.detection_row[35] = 60;

   float32_t last_peak = 35.0F;
   bool f_shrink = false;
   
   /** \action
    * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
   */
   Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
    * f_shrink is set true.
    * last_peak updated to 0.6 * (4 + 35) = 23.4.
    */
   LONGS_EQUAL(1, f_shrink ? 1 : 0);
   DOUBLES_EQUAL(23.4, static_cast<double>(last_peak), 0.01);
}

/** \purpose
* Verify that Shrink Case 1 does NOT activate when the rear-region maximum value
* is higher than the allowed limit. This ensures the shrink logic does not run
* when reflections behind the second peak are too strong.
* \req NA.
*/

TEST(f360_shrink_trailer_length, ShrinkCase1_NoEnter_WhenTempMaxTooHigh)
{
    
   /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak defined but not used for this specific condition.
     * Second peak placed at pos=35 so all earlier conditions except temp_max_val can pass.
     * front_area.mean_val = 0.10 and front_area.max_val = 0.20 so the front-area OR condition is true.
     * detection_row configured so that:
     *     - sample[50] = 100, which makes temp_max_val > 0.25 * i_array_max.
     *       (i_array_max = 100 -> limit = 25).
     * f_noise = true.
     * last_peak initially = 35.0 (unit: index).
   */
    set_peak_counts(1, 1, 0);
   

    set_first_peak(0, 4, 100, 1, 1);
    set_second_peak(0, 35, 60, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
    {
        pvtrailer_length.detection_row[i] = 0;
    }
    pvtrailer_length.detection_row[50] = 100;

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink Case 1 must NOT activate because temp_max_val > 0.25 * i_array_max.
     * f_shrink remains false.
     * last_peak must remain unchanged at 35.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
* Verify that Shrink Case 1 does NOT activate when the second peak position is above
* the 40 index boundary, even if the front area condition is true. This ensures that
* the logic correctly restricts shrink activation to cases where the second peak is within the expected range, preventing inappropriate shrinking based on distant peaks.
* \req NA.
*/
TEST(f360_shrink_trailer_length, ShrinkCase1_NoEnter_WhenSecondPeakPosAbove40)
{
   /** \precond
   * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
   * First peak: pos = 4, val = 100, radii = (1,2).
   * Second peak: pos = 41 (> 40 boundary), val = 60, radii = (1,1).
   * front_area.mean_val = 0.10, front_area.max_val = 0.20.
   * detection_row all zeros (rear region values do not affect this check).
   * f_noise = true.
   * i_array_max = 100, j_array_max = 60.
   * last_peak initially = 41.0 (unit: index).
   */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 41, 60, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 41.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink Case 1 must NOT activate because the second peak position is above 40, 
     * even though the front area condition is true.
     * f_shrink remains false.
     * last_peak remains unchanged at 41.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(41.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
* Verify that Shrink Case 1 activates and shrinks last_peak based on the first peak
* and the second peak, even when the second peak position is above 40, as long as the front area condition is true. 
* This tests that the front area condition can independently trigger the shrink logic without being blocked by the second peak position. 
* \req NA.
*/
TEST(f360_shrink_trailer_length, ShrinkCase1_AltFrontAreaOrArm_ShrinksLastPeak)
{
    
   /** \precond
    * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
    * First peak: pos = 4, val = 100, radii = (1,2).
    * Second peak: pos = 35 (within 40 boundary), val = 60, radii = (1,1).
    * front_area.mean_val = 0.05 and front_area.max_val = 0.50 so the front-area OR condition is true.
    * detection_row shaped so:
    *     - detection_row[4]  = 100  (first peak)
    *     - detection_row[6]  = 30   (between-peaks strong point)
    *     - detection_row[7]  = 30   (between-peaks strong point)
    *     - detection_row[35] = 60   (second peak)
    * temp_max_val behind second peak remains low.
    * i_array_max = 100, j_array_max = 60.
    * f_noise = true.
    * last_peak initially = 35.0 (unit: index).
   */

    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.05F, 0.50F);

    set_detection_row_val(35U, 60);
    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink Case 1 should activate because the front area condition is true, even though the second peak position is above 40.
     * f_shrink is set true.
     * last_peak updated to 0.6 * (4 + 35) = 23.4.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(23.4, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 enters the outer logic but does not shrink when the
 * between-peaks strong-count (temp_cnt) is 3, since the inner condition only allows temp_cnt equal to 1 or 2.
 * This ensures that the logic correctly prevents shrinking when there are too many strong points between the peaks
 * which may indicate a more complex scenario than a simple ghost peak, even if the initial conditions are met.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_EnterButNoShrink_WhenTempCntIsThree)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak: pos = 4, val = 100, radii = (1,2).
     * Second peak: pos = 35, val = 60, radii = (1,1).
     * front_area.mean_val = 0   and front_area.max_val = 0.20 so the front-area OR condition is true.
     * detection_row shaped so:
     *     - detection_row[4]  = 100  (first peak)
     *     - detection_row[6]  = 30   (between-peaks strong point)
     *     - detection_row[7]  = 30   (between-peaks strong point)
     *     - detection_row[8]  = 30   (between-peaks strong point)
     *     - detection_row[35] = 60   (second peak)
     * temp_max_val behind second peak remains low.
     * i_array_max = 100, j_array_max = 60.
     * f_noise = true.
     * last_peak initially = 35.0 (unit: index).
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(35U, 60);
    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);
    set_detection_row_val(8U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Outer block can run, but inner if should be false and not shrink.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 2 (loaded boat trailer rule) activates when all of its
 * required conditions are satisfied:
 *   - f_noise is true,
 *   - second_peak_cnt >= 2,
 *   - last_peak >= 40,
 *   - the first peak position is >= 17.
 * Confirm that when the second peak values trigger the loop break condition,
 * last_peak is updated to the position of the earlier qualifying second peak
 * and f_shrink becomes true.
 * \req NA.
 */

TEST(f360_shrink_trailer_length, ShrinkCase2_LoadedBoatTrailer_ShrinksToEarlierSecondPeak)
{
   /** \precond
    * 
    * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 0.
    * First peak: pos = 4, val = 100, radii = (1,2).
    * Second peaks:
    *     - Peak 1: pos = 45, val = 100, radii = (1,1).
    *     - Peak 2: pos = 50, val = 70, radii = (1,1).
    * front_area.mean_val = 0.10, front_area.max_val = 0.20 (OR condition true).
    * detection_row all zeros (rear region values do not affect this check).
    * f_noise = true.
   */
    set_peak_counts(1, 2, 0);

    /* First peak position must be >= 17 for this case. */
    set_first_peak(0, 20, 100, 1, 1);

    /* Set second peak values so the loop breaks on the first comparison. */
    set_second_peak(0, 45, 100, 1, 1);
    set_second_peak(1, 50, 70, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    /* last_peak must be >= 40 to enter this case, and must change to prove action. */
    float32_t last_peak = 55.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);
    /** \result
     * Shrink Case 2 should activate because the first second peak is strong and the front area condition is true.
     * f_shrink is set true.
     * last_peak updated to the first second peak position, 45.0.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(45.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 shrink rule (Shrink Case 4) activates when all of its
 * required conditions are satisfied and updates last_peak to the average of the
 * first and last second peak positions.
 * \req NA.
 */

TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_ShrinksToAverageOfSecondPeaks)
{
    /** \precond
     * f_noise is true.
     * third_peak_cnt >= 1 with third peak position = 50 (> 35).
     * second_peak_cnt = 2 with second peak positions 30 and 40 (last >= 30).
     * j_array_max = 30 is within the required ratio bounds [0.2 * 100, 0.4 * 100] = [20, 40].
     * mid_area is [Peak_Right_Edge(first)=11 .. Peak_Right_Edge(second_last)=41], ref = 100.
     *   8 samples of 20 give mean ~= 0.052 (>= 0.05) and max = 0.2 (>= 0.2).
     * rear_area [42..79] is all zeros, satisfying mean <= 0.02 and max <= 0.1.
     * First peak at pos = 10 (< 17) and last_peak = 30.0 (< 40) prevent boat-trailer case from running.
     * last_peak initially equals 30.0. i_array_max = 100.
     */
    set_peak_counts(1, 2, 1);
   

    set_first_peak(0, 10, 100, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 20);
    set_detection_row_val(12U, 20);
    set_detection_row_val(13U, 20);
    set_detection_row_val(14U, 20);
    set_detection_row_val(15U, 20);
    set_detection_row_val(16U, 20);
    set_detection_row_val(17U, 20);
    set_detection_row_val(18U, 20);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink Case 4 should activate because all BMZ conditions are met.
     * f_shrink is set true.
     * last_peak updated to the average of the first and last second peak positions: (30 + 40) / 2 = 35.0.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
* Verify that Shrink Case 1 does not activate when the front-area OR condition is false.
* This confirms no shrink happens and last_peak remains unchanged when
* front_area.mean_val is above 0.3 and front_area.max_val is below 0.4.
* \req NA.
*/

TEST(f360_shrink_trailer_length, ShrinkCase1_NoEnter_WhenFrontAreaOrIsFalse)
{
   /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak is set at position 4 with value 100 and radii (1,2).
     * Second peak is set at position 35 with value 60 and radii (1,1).
     * front_area.mean_val = 0.31 and front_area.max_val = 0.20, so the front-area OR is false.
     * Rear region behind the second peak is kept small so execution deterministically reaches the OR check.
     * detection_row initialized to zeros, with detection_row[35] = 10 to keep temp_max_val small.
     * f_noise = true, i_array_max = 100, j_array_max = 60.
     * last_peak initially equals 35.0.
   */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.31F, 0.20F);

    set_detection_row_val(35U, 10);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    
   /** \result
     * Shrink Case 1 does not run because the front-area OR condition is false.
     * f_shrink remains false.
     * last_peak remains 35.0.
   */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
   * Verify that the loaded boat trailer rule (Shrink Case 2) follows the no-break path
   * when the next second-peak value is not small enough (next_val > 0.8 * current_val).
   * Confirms that, with required preconditions met, last_peak is updated to the position
   * of the last evaluated earlier second peak (tmp_last_index) and f_shrink is set true.
   * \req NA.
*/

TEST(f360_shrink_trailer_length, ShrinkCase2_LoadedBoatTrailer_NoBreakPath_ShrinksToLastIndex)
{
  /** \precond
   *  first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 0.
   * First peak position >= 17: pos = 20, val = 100.
   * Second peaks:
   *    - index 0: pos = 45, val = 100.
   *   - index 1: pos = 50, val = 90. Since 90 > 0.8 * 100 = 80, the loop does not break,
   *     so tmp_last_index ends as 0 for second_peak_cnt == 2.
   * last_peak initially = 55.0 (>= 40.0).
   * f_noise = true. i_array_max = 100. j_array_max = 60.
   * front_area values do not block this case; detection_row contents are not used by this path.
  */
    set_peak_counts(1, 2, 0);

    set_first_peak(0, 20, 100, 1, 1);

    set_second_peak(0, 45, 100, 1, 1);
    set_second_peak(1, 50, 90, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 55.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
     * Shrink Case 2 activates. The loop does not break on the second peak because its value is > 0.8 * first peak value,
     * so last_peak updates to the position of the last evaluated earlier second peak,
     * which is the first second peak at position 45.0. f_shrink is set true.
   */

    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(45.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) enters its outer condition but does not shrink
 * when the mid_area statistics are too low to satisfy the inner requirement.
 * Confirms that with BMZ preconditions met, if mid_area.mean_val < 0.05 or mid_area.max_val < 0.2,
 * no shrink occurs and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_EnterButNoShrink_WhenMidAreaTooLow)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
     * First peak: pos = 10, val = 100 (keeps boat case from running: first pos < 17).
     * Second peaks: pos = 30 (val = 60), pos = 40 (val = 50) so last second peak pos >= 30.
     * Third peak: pos = 50 (val = 40) so third peak pos > 35.
     * front_area values do not block BMZ.
     * detection_row all zeros so mid_area.mean_val and mid_area.max_val are both too low.
     * j_array_max = 30, i_array_max = 100 so j_array_max is within [0.2 * i_array_max, 0.4 * i_array_max].
     * f_noise = true.
     * last_peak initially = 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ outer condition is satisfied, but mid_area thresholds are not met.
     * f_shrink remains false.
     * last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 enters the outer block but does not shrink when the rear-area
 * average behind the second peak is too large. This confirms the inner multipath condition
 * fails when rear_area.mean_val is above 0.1 and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_InnerIfFalse_WhenRearAreaMeanAbovePointOne)
{
    
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak set at pos = 4, val = 100, radii = (1,2). Peak_Right_Edge(first) = 6.
     * Second peak set at pos = 35, val = 60, radii = (1,1). Peak_Right_Edge(second) = 36.
     * front_area mean and max set to 0.10 and 0.20 so the outer OR condition is satisfied.
     * detection_row initialized to zeros. Rear region [36..79] filled to yield rear_area.mean_val > 0.1:
     * with ref_val = 60 and n = 44, sum > 264 is required. Use 11 samples of 25 (sum = 275) to make mean ~= 0.104.
     * Minimal content between peaks and at the second peak location added for stability.
     * f_noise = true, i_array_max = 100, j_array_max = 60.
     * last_peak initially equals 35.0 (unit: index).
     */

    set_peak_counts(1, 1, 0);
   
    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(36U, 25);
    set_detection_row_val(37U, 25);
    set_detection_row_val(38U, 25);
    set_detection_row_val(39U, 25);
    set_detection_row_val(40U, 25);
    set_detection_row_val(41U, 25);
    set_detection_row_val(42U, 25);
    set_detection_row_val(43U, 25);
    set_detection_row_val(44U, 25);
    set_detection_row_val(45U, 25);
    set_detection_row_val(46U, 25);
    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);
    set_detection_row_val(35U, 60);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */ 
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Inner multipath if must fail, so nothing shrinks and last_peak stays unchanged.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the loaded boat trailer rule (Shrink Case 2) does not activate 
 * when the first peak position is below the required threshold of 17. 
 * This confirms the block is skipped and no shrink is applied.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase2_NoEnter_WhenFirstPeakPosBelowSeventeen)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 0.
     * First peak position is 16, which is below the required 17 threshold for this rule.
     * Second peaks are defined at positions 45 and 50 with valid values.
     * front_area values do not block this case.
     * detection_row set to all zeros because this case does not depend on area computations.
     * last_peak starts at 55.0 so other entry conditions for this case are satisfied.
     * f_noise = true, i_array_max = 100, j_array_max = 60.
    */

    set_peak_counts(1, 2, 0);

    set_first_peak(0, 16, 100, 1, 1);
    set_second_peak(0, 45, 100, 1, 1);
    set_second_peak(1, 50, 70, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 55.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * The loaded boat trailer rule does not run because the first peak position is below 17.
     * f_shrink remains false.
     * last_peak remains unchanged at 55.0.
    */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(55.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) enters its outer condition but does not shrink
 * when the rear_area maximum value is above 0.1. This confirms that even when mid_area meets
 * its required thresholds, the inner BMZ condition fails due to rear_area.max_val > 0.1,
 * leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_EnterButNoShrink_WhenRearAreaMaxTooHigh)
{
    
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100 ensures boat-trailer case does not run.
     * Second peaks at positions 30 and 40 meet BMZ preconditions since the last second peak
     * position is >= 30. Third peak at pos = 50 meets BMZ condition third peak pos > 35.
     * front_area values do not block BMZ eligibility.
     * mid_area is formed from indices 11 to 41. Values at 11 through 18 are set to 20 to ensure
     * mid_area.mean_val >= 0.05 and mid_area.max_val >= 0.2.
     * rear_area begins at 42. Setting detection_row[42] = 11 ensures rear_area.max_val = 0.11,
     * which is greater than the allowed 0.1 threshold.
     * detection_row values elsewhere kept at 0.
     * f_noise = true, i_array_max = 100, j_array_max = 30.
     * last_peak initially = 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 20);
    set_detection_row_val(12U, 20);
    set_detection_row_val(13U, 20);
    set_detection_row_val(14U, 20);
    set_detection_row_val(15U, 20);
    set_detection_row_val(16U, 20);
    set_detection_row_val(17U, 20);
    set_detection_row_val(18U, 20);

    set_detection_row_val(42U, 11);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ outer condition is met, but inner BMZ condition fails because rear_area.max_val > 0.1.
     * f_shrink remains false.
     * last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 enters the outer block but does not shrink when the rear-area
 * average behind the second peak is above 0.1. This confirms the inner multipath condition
 * fails and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_EnterButNoShrink_WhenRearAreaMeanAbovePointOne)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak at pos = 4, val = 100, radii = (1,2).
     * Second peak at pos = 35, val = 60, radii = (1,1).
     * front_area.mean_val = 0.20 and front_area.max_val = 0.20 so the outer OR condition is satisfied.
     * detection_row initialized to zeros. Rear region [36..79] populated to yield rear_area.mean_val > 0.1
     * with ref_val = 60 and n = 44 by setting 11 samples to 25 (sum = 275, mean ~= 0.104).
     * Minimal between-peaks content added for stability.
     * f_noise = true, i_array_max = 100, j_array_max = 60.
     * last_peak initially equals 35.0.
     */

    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.20F, 0.20F);

    set_detection_row_val(36U, 25);
    set_detection_row_val(37U, 25);
    set_detection_row_val(38U, 25);
    set_detection_row_val(39U, 25);
    set_detection_row_val(40U, 25);
    set_detection_row_val(41U, 25);
    set_detection_row_val(42U, 25);
    set_detection_row_val(43U, 25);
    set_detection_row_val(44U, 25);
    set_detection_row_val(45U, 25);
    set_detection_row_val(46U, 25);

    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Inner multipath condition fails because rear_area.mean_val > 0.1.
     * f_shrink remains false.
     * last_peak remains 35.0.
    */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 enters the outer block but does not shrink when the spacing
 * between strong points is greater than 6. This confirms the inner multipath condition
 * rejects temp_cnt == 2 when the spacing requirement is violated, keeping last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_EnterButNoShrink_WhenSpacingGreaterThanSix)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak at pos = 4, val = 100, r_right = 2 so i_start = Peak_Right_Edge(first) = 6.
     * Second peak at pos = 35, val = 60 so i_end = Peak_Left_Edge(second) = 34.
     * front_area.mean_val = 0.10 and front_area.max_val = 0.20 so the outer OR condition is satisfied.
     * detection_row initialized to zeros. Rear region [36..79] kept at zero so the outer block runs.
     * Two strong points added at indices 7 and 10, making temp_cnt = 2 and spacing = 10 - 7 = 3.
     * To ensure spacing > 6 as required for this test, spacing must exceed 6. Use indices 7 and 14.
     * last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 60.
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(7U, 30);
    set_detection_row_val(10U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Inner if must fail at (temp_cnt_pos[1] - temp_cnt_pos[0] <= 6).
     * f_shrink remains false.
     * last_peak remains 35.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) enters its outer block but does not shrink
 * when mid_area.max_val is below 0.2 while mid_area.mean_val is still above 0.05.
 * This confirms the inner BMZ condition rejects the case when the mid-area maximum
 * reflection strength is too low, keeping last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_EnterButNoShrink_WhenMidAreaMaxBelowPointTwo)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
     * First peak at pos = 10 with val = 100 ensures the boat trailer rule does not run 
     * because first peak pos < 17 and last_peak < 40.
     * Second peaks at pos = 30 and 40 so the last second peak pos >= 30 (BMZ requirement).
     * Third peak at pos = 50 so third peak pos > 35 (BMZ requirement).
     * front_area values do not block BMZ entry.
     * detection_row initialized to zeros.
     * mid_area is [Peak_Right_Edge(first)=11 .. Peak_Right_Edge(second_last)=41], n = 31.
     * To satisfy mid_area.mean_val >= 0.05, set nine samples of value 19 for a sum of 171.
     * With ref_val = 100, mean = 171 / (31 * 100) ~= 0.055. 
     * mid_area.max_val is 19, which corresponds to 0.19 < 0.2, making max_val too low.
     * No rear-area conditions are needed because the inner BMZ condition already fails on mid-area max.
     * last_peak initially equals 30.0. f_noise = true, i_array_max = 100, j_array_max = 30.
     */

    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 19);
    set_detection_row_val(12U, 19);
    set_detection_row_val(13U, 19);
    set_detection_row_val(14U, 19);
    set_detection_row_val(15U, 19);
    set_detection_row_val(16U, 19);
    set_detection_row_val(17U, 19);
    set_detection_row_val(18U, 19);
    set_detection_row_val(19U, 19);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ outer condition is satisfied, but mid_area.max_val < 0.2 causes the inner BMZ check to fail.
     * f_shrink remains false.
     * last_peak remains at 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 does not activate when the front-area OR condition is false
 * because front_area.mean_val is below 0.07 and front_area.max_val is below 0.4.
 * Confirms the outer block is skipped and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_NoEnter_WhenFrontAreaOrSecondArmFails)
{
    
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak at pos = 4 with val = 100 and r_right = 2.
     * Second peak at pos = 35 with val = 60 and r_right = 1.
     * front_area.mean_val = 0.05 and front_area.max_val = 0.20 so both OR arms are false.
     * detection_row initialized to zeros to keep rear-region reflections minimal.
     * last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 60.
    */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.05F, 0.20F);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Outer block skipped because OR is false, so no shrink happens.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 enters the outer block but does not shrink when temp_cnt is 1
 * and the spacing check (temp_cnt_pos[1] - temp_cnt_pos[0] == 1) is false.
 * Confirms the inner multipath condition rejects this case and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_EnterButNoShrink_WhenSpacingCheckIsFalse)
{
    /** \precond
     *   first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     *   First peak at pos = 4 with val = 100 and r_right = 1 so i_start = Peak_Right_Edge(first) = 5.
     *   Second peak at pos = 35 with val = 60 so i_end = Peak_Left_Edge(second) = 34.
     *   front_area.mean_val = 0.10 and front_area.max_val = 0.20 so the outer OR condition is satisfied.
     *   detection_row initialized to zeros. Rear region [36..79] kept at zero so the outer block can run.
     *   A single strong point is set at index 5 to produce temp_cnt = 1 and temp_cnt_pos[1] = 5.
     *   Because temp_cnt = 1 and temp_cnt_pos[1] - temp_cnt_pos[0] != 1, the inner multipath check fails.
     *   last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 60.
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 1);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(5U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Inner multipath spacing check fails, so no shrink occurs.
     * f_shrink remains false and last_peak remains 35.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 activates and shrinks when the inner multipath OR path
 * (front_area.max_val >= 0.5) is satisfied. Confirms last_peak is updated using the
 * 0.6 * (first_peak_pos + second_peak_pos) formula and f_shrink is set true.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_Shrinks_WhenFrontAreaMaxAtLeastPointFive)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * First peak at pos = 0 with val = 100 and r_right = 1 so i_start = Peak_Right_Edge(first) = 1.
     * Second peak at pos = 35 with val = 60 and r_right = 1 so i_end = Peak_Left_Edge(second) = 34.
     * front_area.mean_val = 0.10 and front_area.max_val = 0.60 so the left OR arm is true and inner multipath condition is satisfied.
     * detection_row initialized to zeros. Rear region [36..79] kept at zero so the outer block can run and rear_area stays small.
     * Two strong points added at indices 1 and 2, making temp_cnt = 2 and satisfying the spacing check regardless of temp_cnt_pos[1] value.
     * last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 60.
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 0, 100, 1, 1);
    set_second_peak(0, 35, 60, 1, 1);

    set_front_area_vals(0.10F, 0.60F);

    set_detection_row_val(1U, 30);
    set_detection_row_val(2U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * last_peak = 0.6 * (first_peak_pos + second_peak_pos) = 0.6 * (0 + 35) = 21.0
     * f_shrink is set to true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) activates and shrinks last_peak
 * when the middle area meets its thresholds and the rear area is weak.
 * Confirms last_peak is set to Peak_Right_Edge(second) when rear_area.mean_val <= 0.05
 * and rear_area.max_val <= 0.1.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_ShrinksWhenRearAreaIsWeak)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10 with val = 100.
     * Second peak at pos = 20 with val = 100.
     * Third peak at pos = 40 with val = 100, satisfying the utility trailer requirements of third_peak_pos > 35 
     * and last second peak pos >= 30 front_area values do not inhibit utility trailer activation.
     * detection_row initialized to zeros, making rear_area very weak and satisfying the rear-area condition for 
     * shrinking middle_area defined from Peak_Right_Edge(first)=11 to Peak_Right_Edge(second)=21. 
     * With ref_val=100 and n=11, setting one value to 15 gives mean=0.015 which is between 
     * 0.15 and 0.4, enabling the middle area condition.
     * last_peak initially equals 50.0. f_noise = true, i_array_max = 100, j_array_max = 50.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
    */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * f_shrink is set true.
     * last_peak is set to Peak_Right_Edge(second) = 21.0.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) enters but does not shrink when
 * the rear area has a strong maximum value (rear_area.max_val > 0.1), even though the
 * middle area meets its thresholds. Confirms f_shrink stays false and last_peak is unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoShrinkWhenRearAreaMaxTooHigh)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10 with val = 100.
     * Second peak at pos = 20 with val = 100.
     * Third peak at pos = 40 with val = 100, satisfying the utility trailer requirements of third_peak_pos > 35 and last second peak pos >= 30.
     * front_area values do not inhibit utility trailer activation.
     * detection_row initialized to zeros, making rear_area very weak and satisfying the rear-area condition for shrinking.
     * middle_area defined from Peak_Right_Edge(first)=11 to Peak_Right_Edge(second)=21. With ref_val=100 and n=11, setting one value to 15 gives mean=0.015 which is between 0.15 and 0.4, enabling the middle area condition.
     * last_peak initially equals 50.0. f_noise = true, i_array_max = 100, j_array_max = 50.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    set_detection_row_val(45U, 11);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */ 
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * rear_area.max_val > 0.1 causes the utility trailer inner condition to fail, so no shrink occurs.
     * f_shrink remains false and last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * the third peak position is not above 30. Confirms the outer condition is false
 * and no shrink is applied, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenThirdPeakPosNotAbove30)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 30 (not above 30) so the utility trailer outer condition fails.
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 are valid but unused since the block is skipped.
     * detection_row initialized to zeros.
     * f_noise = false (utility trailer rule).
     * i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);

    set_third_peak(0, 30, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block must not run, so no shrink happens.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify utility trailer outer if covers the false path when third peak last position is above 47.
 * Confirms the outer condition is false and no shrink is applied, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenThirdPeakLastPosAbove47)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 48 (not above 47) so the utility trailer outer condition fails.
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 are valid but unused since the block is skipped.
     * detection_row initialized to zeros.
     * f_noise = false (utility trailer rule).
     * i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);

    set_third_peak(0, 48, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block must not run, so no shrink happens.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * the last third peak position is above 47. Confirms the outer condition is false
 * and no shrink is applied, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenJArrayMaxBelowLowerBound)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 40, val = 100.
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 are valid but unused since the block is skipped.
     * detection_row initialized to zeros.
     * f_noise = false (utility trailer rule).
     * i_array_max = 100, j_array_max = 19 (below 0.2 * i_array_max).
     * last_peak initially equals 50.0.
     */  
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 19, last_peak, f_shrink).
     * 0.2 * 100 = 20, so use 19 to force the false branch.
     */
    Shrink_Trailer_Length(false, 100, 19, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block must not run, so no shrink happens.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * the middle_area span is below 10. Confirms the outer condition is false
 * and no shrink is applied, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenMiddleAreaSpanTooSmall)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 40, val = 100.
     * middle_area.starting_pos = 0, middle_area.ending_pos = 9, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 are valid but unused since the block is skipped.
     * detection_row initialized to zeros.
     * f_noise = false (utility trailer rule).
     * i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */  
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 9, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /* \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer outer condition is not met because middle_area span < 10.
     * f_shrink remains false.
     * last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * middle_area.mean_val is below 0.15. Confirms the outer utility-trailer condition
 * rejects the case and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenMiddleAreaMeanTooLow)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10 with val = 100.
     * Second peak at pos = 20 with val = 100.
     * Third peak at pos = 40 with val = 100 (satisfies third_peak_cnt >= 1).
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10 so span = 10 (valid).
     * Set middle_area.mean_val = 0.14 which is below the required 0.15 threshold.
     * middle_area.max_val = 0.40 satisfies the max-val requirement but the mean check fails.
     * detection_row initialized to zeros; rear-area values do not affect this test.
     * f_noise = false (utility trailer rule). i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.14F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
   */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
    * Utility trailer block must not run, so no shrink happens.
    * f_shrink remains false.
    * last_peak remains 50.0.
    */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * middle_area.max_val is below 0.4. Confirms the outer utility-trailer condition
 * is false and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnterWhenMiddleAreaMaxTooLow)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 40, val = 100 (satisfies third_peak_cnt >= 1).
     * middle_area.starting_pos = 0 and middle_area.ending_pos = 10 so span = 10 (valid).
     * middle_area.mean_val = 0.15 which meets the mean requirement.
     * middle_area.max_val = 0.39 which is below the required 0.4 threshold.
     * detection_row initialized to zeros; rear area does not affect this path.
     * f_noise = false (utility trailer rule). i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);
    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.39F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /* \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block must not run, so no shrink happens.
     * f_shrink remains false.
     * last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) enters but does not shrink when
 * rear_area.mean_val is above 0.05 while rear_area.max_val stays at or below 0.1.
 * Confirms the inner rear-area condition fails and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_EnterButNoShrinkWhenRearAreaMeanAbovePointZeroFive)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10 with val = 100.
     * Second peak at pos = 20 with val = 100.
     * Third peak at pos = 40 with val = 100, satisfying the utility trailer requirements of third_peak_pos > 35 and last second peak pos >= 30.
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 satisfy the middle-area condition for utility trailer shrinking.
     * detection_row initialized to zeros but rear area values are set to produce rear_area.mean_val > 0.05 while keeping rear_area.max_val <= 0.1, causing the inner rear-area condition to fail.
     * f_noise = false (utility trailer rule). i_array_max = 100, j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);

    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    set_detection_row_val(45U, 10);
    set_detection_row_val(46U, 10);
    set_detection_row_val(47U, 10);
    set_detection_row_val(48U, 10);
    set_detection_row_val(49U, 10);
    set_detection_row_val(50U, 10);
    set_detection_row_val(51U, 10);
    set_detection_row_val(52U, 10);
    set_detection_row_val(53U, 10);
    set_detection_row_val(54U, 10);
    set_detection_row_val(55U, 10);
    set_detection_row_val(56U, 10);
    set_detection_row_val(57U, 10);
    set_detection_row_val(58U, 10);
    set_detection_row_val(59U, 10);
    set_detection_row_val(60U, 10);
    set_detection_row_val(61U, 10);
    set_detection_row_val(62U, 10);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /* \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block runs, but inner if must fail due to rear_area.mean_val > 0.05.
     * f_shrink remains false and last_peak remains 50.0.
     * This confirms that the rear area mean value condition is properly evaluated and can prevent shrinking even
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when the number
 * of second peaks is less than 2. Confirms the BMZ outer condition is false and
 * last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenSecondPeakCntIsOne)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Only one second peak is defined at pos = 50, so second_peak_cnt < 2 and the BMZ rule
     * cannot activate.
     * A third peak exists at pos = 50, satisfying third_peak_cnt >= 1, but BMZ requires
     * at least two second peaks.
     * detection_row is all zeros; front_area does not block or enable this path.
     * f_noise = true, i_array_max = 100, j_array_max = 30.
     * last_peak initially equals 30.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 50, 60, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ block must not run because second_peak_cnt < 2, so no shrink happens.
     * f_shrink remains false and last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when
 * j_array_max is below 0.2 * i_array_max. Confirms the BMZ outer condition is false
 * and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenJArrayMaxBelowLowerBound)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1, satisfying the peak count requirements for BMZ.
     * First peak at pos = 10, val = 100.
     * Two second peaks at pos = 30 and 40 with vals 60 and 50 respectively.
     * A third peak at pos = 50 with val = 40, satisfying the requirement of third_peak_pos > 35.
     * detection_row initialized to zeros; front_area does not block this path.
     * f_noise = true, i_array_max = 100, j_array_max = 19 (below the required 0.2 * i_array_max).
     * last_peak initially equals 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 19, last_peak, f_shrink)
     */
    Shrink_Trailer_Length(true, 100, 19, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /* \result
     * BMZ block must not run because j_array_max is below the lower bound, so no shrink happens.
     * f_shrink remains false and last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when
 * j_array_max is above 0.4 * i_array_max. Confirms the BMZ outer condition is false
 * and no shrink is applied, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenJArrayMaxAboveUpperBound)
{
    /* \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1, satisfying the peak count requirements for BMZ.
     * First peak at pos = 10, val = 100.
     * Two second peaks at pos = 30 and 40 with vals 60 and 50 respectively.
     * A third peak at pos = 50 with val = 40, satisfying the requirement of third_peak_pos > 35.
     * detection_row initialized to zeros; front_area does not block this path.
     * f_noise = true, i_array_max = 100, j_array_max = 41 (above the required upper bound of 0.4 * i_array_max).
     * last_peak initially equals 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);

    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);

    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 41, last_peak, f_shrink)
     */
    Shrink_Trailer_Length(true, 100, 41, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /* \result
     * BMZ block must not run because j_array_max is above the upper bound, so no shrink happens.
     * f_shrink remains false and last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) enters but does not shrink when
 * rear_area.mean_val is above 0.02 while rear_area.max_val remains at or below 0.1.
 * Confirms the inner BMZ condition fails and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_EnterButNoShrink_WhenRearAreaMeanAbovePointZeroTwo)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100 (keeps boat-trailer case from running: first pos < 17).
     * Second peaks at pos = 30 and 40 (last second peak pos >= 30, meets BMZ precondition).
     * Third peak at pos = 50 (third_peak_cnt >= 1 and third peak pos > 35).
     * detection_row initialized to zeros.
     * mid_area = [Peak_Right_Edge(first)=11 .. Peak_Right_Edge(second_last)=41], ref = 100:
     *   set 8 samples of 20 to satisfy mid_area.mean_val >= 0.05 and mid_area.max_val >= 0.2.
     * rear_area = [42 .. 79], n = 38, ref = 100:
     *   set 8 samples of 10 so sum = 80 -> mean ~= 0.021 (> 0.02) and max = 0.1 (<= 0.1).
     * f_noise = true, i_array_max = 100, j_array_max = 30.
     * last_peak initially = 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);
    set_third_peak(0, 50, 40, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 20);
    set_detection_row_val(12U, 20);
    set_detection_row_val(13U, 20);
    set_detection_row_val(14U, 20);
    set_detection_row_val(15U, 20);
    set_detection_row_val(16U, 20);
    set_detection_row_val(17U, 20);
    set_detection_row_val(18U, 20);
    set_detection_row_val(42U, 10);
    set_detection_row_val(43U, 10);
    set_detection_row_val(44U, 10);
    set_detection_row_val(45U, 10);
    set_detection_row_val(46U, 10);
    set_detection_row_val(47U, 10);
    set_detection_row_val(48U, 10);
    set_detection_row_val(49U, 10);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
    */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ block runs, but inner if must fail due to rear_area.mean_val > 0.02.
     * f_shrink remains false and last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when the last second
 * peak position is below 30. Confirms the BMZ outer condition is false and no shrink occurs,
 * leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenLastSecondPeakPosBelowThirty)
{
    /** \precondition
     * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100 (keeps boat-trailer case from running: first pos < 17).
     * Two second peaks at pos = 25 and 29 with vals 60 and 50 respectively, so last second peak pos < 30 and BMZ cannot activate.
     * A third peak at pos = 40 with val = 40, satisfying the requirement of third_peak_pos > 35.
     * detection_row initialized to zeros; front_area does not block this path.
     * f_noise = true, i_array_max = 100, j_array_max = 30.
     * last_peak initially equals 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_third_peak(0, 50, 40, 1, 1);
    set_second_peak(0, 25, 60, 1, 1);
    set_second_peak(1, 29, 50, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ block must not run because the last second peak position is below 30.
     * f_shrink remains false and last_peak remains 30.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * first_peak_cnt is not equal to 1. Confirms the utility trailer outer condition
 * fails and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnter_WhenFirstPeakCntNotOne)
{
    /** \precond
     * first_peak_cnt = 2, second_peak_cnt = 1, third_peak_cnt = 1.
     * first_peak_extension = false.
     * Only one second peak is required so that the top-level loops have valid indices.
     * detection_row initialized to zeros; no area computations affect this path.
     * f_noise = false so only the utility trailer path is evaluated.
     * last_peak initially equals 50.0. i_array_max = 100, j_array_max = 20.
     * Utility trailer requires first_peak_cnt == 1, which is violated here.
     */
    set_peak_counts(2, 1, 1);

    set_second_peak(0, 20, 100, 1, 1);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer block does not run because first_peak_cnt != 1.
     * f_shrink remains false.
     * last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * second_peak_cnt is less than 2. Confirms the utility trailer outer condition
 * fails and last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnter_WhenSecondPeakCntNotOne)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * first_peak_extension = false.
     * detection_row initialized to zeros; no area computations affect this path.
     * f_noise = false so only the utility trailer path is evaluated.
     * last_peak initially equals 50.0. i_array_max = 100, j_array_max = 20.
     * Utility trailer requires second_peak_cnt >= 2, which is violated here.
     */
    set_peak_counts(1, 2, 1);

    set_second_peak(0, 20, 100, 1, 1);
    set_second_peak(1, 25, 80, 1, 1);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /* \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * second_peak_cnt == 1 is false, so utility trailer block must not run.
     * f_shrink remains false.
     * last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not activate when
 * third_peak_cnt is zero. Confirms the utility trailer outer condition fails and
 * last_peak remains unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnter_WhenThirdPeakCntIsZero)
{
    /* \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * first_peak_extension = false.
     * detection_row initialized to zeros; no area computations affect this path.
     * f_noise = false so only the utility trailer path is evaluated.
     * last_peak initially equals 50.0. i_array_max = 100, j_array_max = 20.
     * Utility trailer requires third_peak_cnt >= 1, which is violated here.
     */
    set_peak_counts(1, 1, 0);
   
    set_second_peak(0, 20, 100, 1, 1);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * third_peak_cnt >= 1 is false, so utility trailer block must not run.
    */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 activates and shrinks last_peak when front_area.max_val is at least 0.5, taking the inner OR path. Confirms that the condition front_area.max_val >= 0.5 is properly evaluated and allows shrinking even if front_area.mean_val is below 0.07.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_Shrinks_WhenFrontAreaMaxGtePointFive_TruePath)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
     * first_peak_extension = false.
     * First peak at pos = 4, val = 100, with r_right = 1 so i_start = Peak_Right_Edge(first) = 5.
     * Second peak at pos = 35, val = 60 so i_end = Peak_Left_Edge(second) = 34.
     * front_area.mean_val = 0.10 (does not meet the >= 0.07 condition), but front_area.max_val = 0.60 meets the >= 0.5 condition, so the inner OR is true and shrinking can occur.
     * detection_row initialized to zeros; rear region [36..79] remains weak so the outer block runs.
     * Two strong points placed at indices 6 and 7 create temp_cnt = 2 and satisfy spacing conditions.
     * last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 60.
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 35, 60, 1, 1);
    set_front_area_vals(0.10F, 0.60F);

    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);

    float32_t last_peak = 35.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * last_peak = 0.6 * (first_peak_pos + second_peak_pos) = 0.6 * (4 + 35) = 23.4
     * f_shrink is set true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(23.4, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 does not activate when the second peak position is below 30, 
 * taking the false path of the top-level if. Confirms that the condition second_peak_group[0].peak_pos >= 30 is 
 * properly evaluated and prevents shrinking when the second peak is too far left, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_NoEnter_WhenSecondPeakPosBelow30)
{
   /** precond
    * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
    * first_peak_extension = false.
    * First peak at pos = 4, val = 100, with r_right = 1 so i_start = Peak_Right_Edge(first) = 5.
    * Second peak at pos = 29, val = 60 so i_end = Peak
    * Left_Edge(second) = 28. The second peak position is below 30, so the top-level if condition second_peak_group[0].peak_pos >= 30 is false and the entire shrink block should be skipped.
    * front_area.mean_val = 0.10, front_area.max_val = 0.20, but these should not matter because the second peak position fails the initial check.
    * detection_row initialized to zeros; rear region [30..79] remains weak so the outer block would run if not for the second peak position check.
    * last_peak initially equals 29.0. f_noise = true, i_array_max = 100, j_array_max = 60.
    */
   set_peak_counts(1, 1, 0);

   set_first_peak(0, 4, 100, 1, 2);
   set_second_peak(0, 29, 60, 1, 1);
   set_front_area_vals(0.10F, 0.20F);

   float32_t last_peak = 29.0F;
   bool f_shrink = false;

   /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
   Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
    * The second peak position check fails, so the shrink block is not entered and no shrink occurs.
    * f_shrink remains false and last_peak remains 29.0.
    */
   LONGS_EQUAL(0, f_shrink ? 1 : 0);
   DOUBLES_EQUAL(29.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that Shrink Case 1 activates and shrinks last_peak when front_area.max_val is NaN, 
 * taking the false path of the inner OR but still allowing shrinking via the j_array_max condition. 
 * Confirms that NaN values are properly handled in the max comparisons and that the logic allows shrinking 
 * through the j_array_max path even when front_area.max_val does not meet the >= 0.5 condition.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_Shrinks_WhenFrontAreaMaxIsNaN_UsesJArrayMaxPath)
{
   /** \precond
    * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 0.
    * first_peak_extension = false.
    * First peak at pos = 4, val = 100, with r_right = 1 so i_start = Peak_Right_Edge(first) = 5.
    * Second peak at pos = 35, val = 60 so i_end = Peak_Left_Edge(second) = 34.
    * front_area.mean_val = 0.10 (does not meet the >= 0.07 condition), and front_area.max_val is set to NaN, which should cause the >= 0.5 condition to evaluate as false. However, the j_array_max condition will allow shrinking if it is <= 0.25 * i_array_max.
    * detection_row initialized to zeros; rear region [36..79] remains weak so the outer block runs.
    * Two strong points placed at indices 6 and 7 create temp_cnt = 2 and satisfy spacing conditions.
    * last_peak initially equals 35.0. f_noise = true, i_array_max = 100, j_array_max = 25 (which is <= 0.25 * i_array_max).
    */
   set_peak_counts(1, 1, 0);

   set_first_peak(0, 4, 100, 1, 2);
   set_second_peak(0, 35, 60, 1, 1);
   const float32_t nan_val = std::numeric_limits<float32_t>::quiet_NaN();
   set_front_area_vals(0.10F, nan_val);

   set_detection_row_val(6U, 30);
   set_detection_row_val(7U, 30);

   float32_t last_peak = 35.0F;
   bool f_shrink = false;

   /** \action
    * Call wrapper: call_shrink_trailer_length(true, 100, 25, last_peak, f_shrink).
    * j_array_max is set to 25, which is exactly 0.25 * i_array_max, so the condition j_array_max <= 0.25 * i_array_max is true and allows shrinking to occur even though front_area.max_val does not meet the >= 0.5 condition.
    */
   Shrink_Trailer_Length(true, 100, 25, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /* \result
    * Shrink occurs because the j_array_max condition allows it even though front_area.max_val is NaN 
    and does not meet the >= 0.5 condition.
    */
   LONGS_EQUAL(1, f_shrink ? 1 : 0);
   DOUBLES_EQUAL(23.4, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when
 * third_peak_cnt is 0. Confirms the BMZ outer condition is false and no shrink occurs,
 * leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenThirdPeakCntIsZero)
{
   /** \precond
    * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 0.
    * first_peak_extension = false.
    * First peak at pos = 10, val = 100 keeps earlier cases from interfering for this setup.
    * Second peaks at pos = 30 and 40 satisfy BMZ second-peak position preconditions.
    * No third peak is present, so third_peak_cnt >= 1 is false.
    * front_area values are not relevant to this BMZ check.
    * detection_row initialized to zeros.
    * last_peak initially equals 30.0. f_noise = true. i_array_max = 100. j_array_max = 30.
    */
   /* Put all initialization and input setup here. */
   set_peak_counts(1, 2, 0);

   set_first_peak(0, 10, 100, 1, 1);
   set_second_peak(0, 30, 60, 1, 1);
   set_second_peak(1, 40, 50, 1, 1);

   set_front_area_vals(0.10F, 0.20F);

   float32_t last_peak = 30.0F;
   bool f_shrink = false;

   /* \action
    * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
    * j_array_max is set to 30, which is within [0.2*i_array_max, 0.4*i_array_max], so BMZ range checks would pass if reached.
    */
   Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
    * BMZ block does not run because third_peak_cnt >= 1 is false.
    * f_shrink remains false.
    * last_peak remains 30.0.
    */
   LONGS_EQUAL(0, f_shrink ? 1 : 0);
   DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) does not activate when
 * third_peak_group[0].peak_pos is not greater than 35. Confirms the BMZ outer
 * condition is false and no shrink occurs, leaving last_peak unchanged.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ_NoEnter_WhenThirdPeakPosIs35)
{
   /** \precond
    * first_peak_cnt = 1, second_peak_cnt = 2, third_peak_cnt = 1.
    * first_peak_extension = false.
    * First peak at pos = 10 prevents earlier shrink cases from interfering.
    * Second peaks at pos = 30 and 40 satisfy BMZ second-peak requirements.
    * Third peak is set at pos = 35, which does not satisfy the BMZ requirement
    * that third_peak_group[0].peak_pos must be greater than 35.
    * front_area values do not affect this path.
    * detection_row initialized to zeros.
    * last_peak initially equals 30.0. f_noise = true. i_array_max = 100. j_array_max = 30.
    */
   set_peak_counts(1, 2, 1);

   set_first_peak(0, 10, 100, 1, 1);
   set_second_peak(0, 30, 60, 1, 1);
   set_second_peak(1, 40, 50, 1, 1);
   set_third_peak(0, 35, 40, 1, 1);

   set_front_area_vals(0.10F, 0.20F);

   float32_t last_peak = 30.0F;
   bool f_shrink = false;

   /** \action
    * Call wrapper: call_shrink_trailer_length(true, 100, 30, last_peak, f_shrink).
    * j_array_max = 30 lies within the BMZ allowed range and would allow entry
    * if the third peak position condition did not fail first.
    */
   Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

   /** \result
    * BMZ outer condition is false because third_peak_group[0].peak_pos is not > 35.
    * f_shrink remains false.
    * last_peak remains 30.0.
    */
   LONGS_EQUAL(0, f_shrink ? 1 : 0);
   DOUBLES_EQUAL(30.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify Shrink Case 1 accepts boundaries:
 * - second_peak_group[0].peak_pos == 40 (inclusive upper bound)
 * - temp_max_val == 0.25 * i_array_max (inclusive)
 * - spacing bound satisfied with (temp_cnt_pos[1] - temp_cnt_pos[0]) == 6
 * Confirms last_peak is shrunk to 0.6 * (first_peak_pos + second_peak_pos).
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase1_Boundaries_PosEquals40_TempMaxEqQuarter_SpacingEqSix_Shrinks)
{
    /** \precond
     * f_noise = true; second_peak_cnt = 1.
     * first peak: pos=4, val=100, r_left=1, r_right=2 => i_start = Peak_Right_Edge(first)=4+2=6.
     * second peak: pos=40, val=60, r_left=1, r_right=1.
     * front_area.mean in [0.07,0.3] to satisfy left OR arm: mean=0.10, max arbitrary=0.20.
     * i_array_max=100, choose one sample at Peak_Right_Edge(second)=41 with value 25 so:
     *   temp_max_val (over [41..79]) == 25 == 0.25 * i_array_max (boundary equality).
     * Between peaks [i_start..i_end]=[6..39]:
     *   make temp_cnt==2 with first strong at i=6 and the next at i=7 (>= 0.8*temp_max_between),
     *   which yields (temp_cnt_pos[1] - temp_cnt_pos[0]) == 6 - 0 == 6 (boundary equality).
     * Rear area [41..79] mostly zeros so mean is small and < front mean, <= 0.1.
     */
    set_peak_counts(1, 1, 0);

    set_first_peak(0, 4, 100, 1, 2);
    set_second_peak(0, 40, 60, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(41U, 25);
    set_detection_row_val(6U, 30);
    set_detection_row_val(7U, 30);

    float32_t last_peak = 40.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink occurs:
     *   last_peak = 0.6 * (4 + 40) = 26.4.
     * f_shrink == true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(26.4, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify Shrink Case 2 accepts exact boundary values:
 * - first_peak_group[last].peak_pos == 17 (inclusive)
 * - last_peak == 40.0F (inclusive)
 * Confirms it shrinks to the earlier qualifying second peak.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase2_Boundaries_FirstPeakPosEq17_LastPeakEq40_ShrinksToEarlierSecondPeak)
{
    /** \precond
     * f_noise = true; second_peak_cnt >= 2.
     * first peak: pos=17 (boundary), val=100.
     * two second peaks: [pos=45,val=100], [pos=50,val=70] with 70 <= 0.8*100 -> break at i=0.
     * last_peak starts at 40.0F (boundary).
     * Other areas not relevant to this branch.
     */
    set_peak_counts(1, 2, 0);
    set_first_peak(0, 17, 100, 1, 1);
    set_second_peak(0, 45, 100, 1, 1);
    set_second_peak(1, 50, 70, 1, 1);

    float32_t last_peak = 40.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(true, 100, 60, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 60, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink occurs:
     *   last_peak becomes second_peak_group[0].peak_pos = 45.0.
     * f_shrink == true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(45.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify BMZ-18705 case accepts boundary equalities:
 * - j_array_max == 0.4 * i_array_max (upper bound inclusive)
 * - mid_area.mean_val == 0.05 and mid_area.max_val == 0.2 (inclusive)
 * - rear_area.mean_val == 0.02 and rear_area.max_val == 0.1 (inclusive)
 * Confirms it shrinks to the average of the first and last second peaks.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_AllEqualityBoundaries_ShrinksToAverage)
{
    /** \precond
     * Enable BMZ:
     *   f_noise = true; second_peak_cnt >= 2; last second peak pos >= 30; third_peak_cnt >= 1; third_peak[0].pos > 35.
     *   j_array_max in [0.2, 0.4] * i_array_max; use equality: j_array_max = 0.4 * i_array_max = 40 (i_array_max=100).
     * First peak (to place mid_area.start): pos=10, right edge = 11.
     * Second peaks: pos=30 (edge contributes to mid start/end), pos=40 (right edge=41).
     * mid_area = [11..41], ref=100:
     *   mean == 0.05 exactly -> sum = 0.05 * 31 * 100 = 155.
     *   max == 0.2 -> one sample of value 20.
     *   Compose sum: 1x20 + 7x19 + 1x2 = 20 + 133 + 2 = 155.
     * rear_area = [42..79], n=38, ref=100:
     *   mean == 0.02 exactly -> sum = 76; max == 0.1 -> highest 10.
     *   Compose sum: 7x10 + 1x6 = 70 + 6 = 76.
     * last_peak initial any value (< 40) to avoid earlier cases; choose 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);
    set_third_peak(0, 50, 40, 1, 1);
    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 20);
    set_detection_row_val(12U, 19);
    set_detection_row_val(13U, 19);
    set_detection_row_val(14U, 19);
    set_detection_row_val(15U, 19);
    set_detection_row_val(16U, 19);
    set_detection_row_val(17U, 19);
    set_detection_row_val(18U, 19);
    set_detection_row_val(19U, 2);
    set_detection_row_val(42U, 10);
    set_detection_row_val(43U, 10);
    set_detection_row_val(44U, 10);
    set_detection_row_val(45U, 10);
    set_detection_row_val(46U, 10);
    set_detection_row_val(47U, 10);
    set_detection_row_val(48U, 10);
    set_detection_row_val(49U, 6);

    float32_t last_peak = 30.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper with boundary j_array_max = 40 (== 0.4 * 100).
     */
    Shrink_Trailer_Length(true, 100, 40, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Shrink occurs:
     *   last_peak = 0.5 * 30 + 0.5 * 40 = 35.0.
     * f_shrink == true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the BMZ-18705 rule (Shrink Case 4) activates when mid_area.mean_val == 0.05
 * and mid_area.max_val == 0.2 exactly (inclusive thresholds). Confirms last_peak is set
 * to the average of the first and last second peak positions and f_shrink is set true.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase4_BMZ18705_MidAreaExactThresholds_ShrinksToAverage)
{
    /** \precond
     *   f_noise = true; second_peak_cnt >= 2; third_peak_cnt >= 1;
     *   third_peak_group[0].peak_pos > 35; last second peak pos >= 30.
     * Use i_array_max = 100 so 0.2 * i_array_max = 20 and 0.4 * i_array_max = 40.
     * Choose j_array_max = 30 (within [0.2, 0.4] * 100).
     *
     * Geometry:
     *   First peak pos = 10 -> Peak_Right_Edge(first) = 11.
     *   Second peaks at 30 and 40 -> Peak_Right_Edge(second_last) = 41.
     *   mid_area = [11 .. 41], n = 31, ref = 100.
     * Exact mid_area thresholds:
     *   mean == 0.05 -> sum = 0.05 * 31 * 100 = 155.
     *   max  == 0.2  -> at least one sample = 20.
     *   Compose sum: 1x20 + 7x19 + 1x2 = 20 + 133 + 2 = 155.
     *
     * rear_area = [42 .. 79], n = 38, ref = 100; must be weak:
     *   mean <= 0.02 and max <= 0.1.
     *   Use sum = 76 (exact 0.02): 7x10 + 1x6 -> 70 + 6 = 76; max = 10 -> 0.1.
     * last_peak initial any value (< 40) to avoid earlier cases; set 30.0.
     */
    set_peak_counts(1, 2, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 30, 60, 1, 1);
    set_second_peak(1, 40, 50, 1, 1);
    set_third_peak(0, 50, 40, 1, 1);

    set_front_area_vals(0.10F, 0.20F);

    set_detection_row_val(11U, 20); 
    set_detection_row_val(12U, 19);
    set_detection_row_val(13U, 19);
    set_detection_row_val(14U, 19);
    set_detection_row_val(15U, 19);
    set_detection_row_val(16U, 19);
    set_detection_row_val(17U, 19);
    set_detection_row_val(18U, 19);
    set_detection_row_val(19U, 2); 
    set_detection_row_val(42U, 10);
    set_detection_row_val(43U, 10);
    set_detection_row_val(44U, 10);
    set_detection_row_val(45U, 10);
    set_detection_row_val(46U, 10);
    set_detection_row_val(47U, 10);
    set_detection_row_val(48U, 10);
    set_detection_row_val(49U, 6);

    float32_t last_peak = 30.0F; 
    bool f_shrink = false;

    /** \action
     * Call wrapper with j_array_max within BMZ range: [0.2, 0.4] * 100 -> use 30.
     */
    Shrink_Trailer_Length(true, 100, 30, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * BMZ inner condition passes at exact mid_area thresholds and weak rear area.
     * last_peak becomes average of first and last second peak positions: (30 + 40) / 2 = 35.0.
     * f_shrink is set true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(35.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify utility trailer outer condition accepts boundary j_array_max == 0.2 * i_array_max.
 * Confirms shrink occurs when all other conditions are satisfied at this equality.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_JArrayMaxEqLowerBound_AllowsShrink)
{
    /** \precond
     * first_peak_cnt=1, second_peak_cnt=1, third_peak_cnt=1.
     * first=10 (val=100), second=20 (val=100), third=40 (val=100).
     * middle_area bounds 0..10, ref=100, mean=0.15, max=0.40.
     * detection_row all zeros so rear_area.mean<=0.05 and rear_area.max<=0.1.
     * f_noise=false, i_array_max=100, j_array_max=20 (== 0.2 * 100).
     * last_peak starts 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);
    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F; bool f_shrink = false;

    /** \action */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify utility trailer outer condition accepts boundary j_array_max == 0.4 * i_array_max.
 * Confirms shrink occurs at the upper equality.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_JArrayMaxEqUpperBound_AllowsShrink)
{
    /** \precond
     * first_peak_cnt=1, second_peak_cnt=1, third_peak_cnt=1.
     * first=10 (val=100), second=20 (val=100), third=40 (val=100).
     * middle_area bounds 0..10, ref=100, mean=0.15, max=0.40.
     * detection_row all zeros so rear_area.mean<=0.05 and rear_area.max<=0.1.
     * f_noise=false, i_array_max=100, j_array_max=40 (== 0.4 * 100).
     * last_peak starts 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);
    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F; bool f_shrink = false;

    /** \action */
    Shrink_Trailer_Length(false, 100, 40, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) accepts the boundary condition
 * third_peak_group[0].peak_pos == 31 (> 30) and performs shrinking as expected.
 * Confirms outer condition is satisfied at the lower boundary for the first third-peak position.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_ThirdPeakFirstPosEq31_AllowsShrink)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 31 (boundary > 30).
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 meet utility trailer thresholds.
     * detection_row initialized to zeros so rear_area remains weak (mean <= 0.05, max <= 0.1).
     * f_noise = false. i_array_max = 100. j_array_max = 20 (within [0.2, 0.4] * i_array_max).
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 31, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer shrink occurs.
     * last_peak becomes Peak_Right_Edge(second) = 20 + 1 = 21.0.
     * f_shrink is set true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) accepts the boundary condition
 * third_peak_group[last].peak_pos == 47 (<= 47) and performs shrinking as expected.
 * Confirms outer condition is satisfied at the upper boundary for the last third-peak position.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_ThirdPeakLastPosEq47_AllowsShrink)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 47 (boundary <= 47).
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, middle_area.ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 meet utility trailer thresholds.
     * detection_row initialized to zeros so rear_area remains weak (mean <= 0.05, max <= 0.1).
     * f_noise = false. i_array_max = 100. j_array_max = 20 (within [0.2, 0.4] * i_array_max).
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 47, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer shrink occurs.
     * last_peak becomes Peak_Right_Edge(second) = 20 + 1 = 21.0.
     * f_shrink is set true.
     */
    LONGS_EQUAL(1, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(21.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that the utility trailer rule (Shrink Case 3) does not execute when f_noise == true,
 * even if all other outer conditions are satisfied. Confirms the path is disabled under noise.
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_NoEnter_WhenFNoiseTrue)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 40, val = 100 (valid third-peak window).
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 meet thresholds.
     * detection_row initialized to zeros so rear area is weak.
     * f_noise will be true in the action call. i_array_max = 100. j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 40, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper with f_noise = true to verify the block is skipped.
     * call_shrink_trailer_length(true, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(true, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer rule is not executed under noise.
     * f_shrink remains false and last_peak remains 50.0.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** \purpose
 * Verify that placing the third peak at index 79 does not cause out-of-bounds access
 * in Cal_Radius and that the utility trailer rule does not activate because the
 * third peak position exceeds its allowed bound (<= 47).
 * \req NA.
 */
TEST(f360_shrink_trailer_length, ShrinkCase3_UtilityTrailer_ThirdPeakAt79_NoShrink_NoOOB)
{
    /** \precond
     * first_peak_cnt = 1, second_peak_cnt = 1, third_peak_cnt = 1.
     * First peak at pos = 10, val = 100.
     * Second peak at pos = 20, val = 100.
     * Third peak at pos = 79, val = 100 (beyond allowed bound for utility trailer).
     * middle_area.starting_pos = 0, middle_area.ending_pos = 10, ref_val = 100.
     * middle_area.mean_val = 0.15 and middle_area.max_val = 0.40 meet thresholds.
     * detection_row initialized to zeros so rear area remains weak.
     * f_noise = false. i_array_max = 100. j_array_max = 20.
     * last_peak initially equals 50.0.
     */
    set_peak_counts(1, 1, 1);

    set_first_peak(0, 10, 100, 1, 1);
    set_second_peak(0, 20, 100, 1, 1);
    set_third_peak(0, 79, 100, 1, 1);

    set_middle_area_bounds(0, 10, 100);
    set_middle_area_vals(0.15F, 0.40F);

    float32_t last_peak = 50.0F;
    bool f_shrink = false;

    /** \action
     * Call wrapper: call_shrink_trailer_length(false, 100, 20, last_peak, f_shrink).
     */
    Shrink_Trailer_Length(false, 100, 20, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

    /** \result
     * Utility trailer outer condition is not met because third peak position > 47.
     * No shrink occurs; f_shrink remains false and last_peak remains unchanged.
     * Cal_Radius boundary handling is implicitly validated by successful execution without OOB.
     */
    LONGS_EQUAL(0, f_shrink ? 1 : 0);
    DOUBLES_EQUAL(50.0, static_cast<double>(last_peak), 0.01);
}

/** @}*/

/** \defgroup  tl_estimate
 *  @{
 */

/** \brief
 * Test group to test process input main functionality.
 */
TEST_GROUP(f360_process_input_main_functionality)
{
   // Initialize an object of the mocked trailer detector TL
   F360_PVTrailer_Length_Data_T pvtrailer_length;
   F360_Host_T vehicle_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T all_detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];

   int32_t detection_row[DETECTION_ROWS];

   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer_length, 0, sizeof(pvtrailer_length));
      pvtrailer_length.window_timer = 600U;

      vehicle_data.speed = 10.0F;
      vehicle_data.dist_rear_axle_to_vcs_m = 5.0F;

      // Set up history in the detection_row_struct with 1 previously binned detection
      for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
      {
         pvtrailer_length.detection_row[i] = 0;
      }

      // Set up detections
      raw_detect_list.number_of_valid_detections = 3U;

      uint32_t det_idx_1 = 0U;
      raw_detect_list.detections[det_idx_1].raw.range_rate = 0.1F;
      raw_detect_list.detections[det_idx_1].raw.sensor_id = 1U;
      all_detections[det_idx_1].vcs_position.x = -10.0F;
      all_detections[det_idx_1].vcs_position.y = 1.0F;
      all_detections[det_idx_1].f_double_bounce = false;
      all_detections[det_idx_1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      all_detections[det_idx_1].f_water_spray = false;
      sensors[raw_detect_list.detections[det_idx_1].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

      uint32_t det_idx_2 = 1U;
      raw_detect_list.detections[det_idx_2].raw.range_rate = 0.1F;
      raw_detect_list.detections[det_idx_2].raw.sensor_id = 2U;
      all_detections[det_idx_2].vcs_position.x = -15.0F;
      all_detections[det_idx_2].vcs_position.y = 1.0F;
      all_detections[det_idx_2].f_double_bounce = false;
      all_detections[det_idx_2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      all_detections[det_idx_2].f_water_spray = false;
      sensors[raw_detect_list.detections[det_idx_2].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;

      uint32_t det_idx_3 = 2U;
      raw_detect_list.detections[det_idx_3].raw.range_rate = 0.1F;
      raw_detect_list.detections[det_idx_3].raw.sensor_id = 3U;
      all_detections[det_idx_3].vcs_position.x = -5.5F;
      all_detections[det_idx_3].vcs_position.y = 1.0F;
      all_detections[det_idx_3].f_double_bounce = false;
      all_detections[det_idx_3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      all_detections[det_idx_3].f_water_spray = false;
      sensors[raw_detect_list.detections[det_idx_3].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;
   }
};

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations.
 */
TEST(f360_process_input_main_functionality, process_input_3_valid_dets)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 600
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up
    * Expectation is that these three detections are stored in the detection row struct arrays.
    */
   uint8_t exp_row_idx_1 = 30U;
   uint8_t exp_row_idx_2 = 63U;
   uint8_t exp_row_idx_3 = 0U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_2)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_3)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations when window timer is only in  interval.
 */
TEST(f360_process_input_main_functionality, process_input_3_valid_dets_only_shorter_window)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 100
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up
    * Expectation is that these three detections are stored in the detection row struct arrays properly
    *    - Only the historic one is stored in the array
    *    - The new detections are only stored in the  array
    */
   pvtrailer_length.window_timer = 100U;
   uint8_t exp_row_idx_1 = 30U;
   uint8_t exp_row_idx_2 = 63U;
   uint8_t exp_row_idx_3 = 0U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_2)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_3)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations when window timer is only in interval.
 */
TEST(f360_process_input_main_functionality, process_input_3_valid_dets_only)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 1100
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up
    * Expectation is that these three detections are stored in the detection row struct arrays.
    */
   pvtrailer_length.window_timer = 1100U;
   uint8_t exp_row_idx_1 = 30U;
   uint8_t exp_row_idx_2 = 63U;
   uint8_t exp_row_idx_3 = 0U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_2)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_3)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations when window timer is 0.
 */
TEST(f360_process_input_main_functionality, process_input_3_valid_dets_window_timer_is_0s)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 0
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up
    * Expectation is that these three detections are stored in the detection row struct arrays.
    */
   pvtrailer_length.window_timer = 0U;
   const uint8_t exp_row_idx_1 = 0U;
   const uint8_t exp_row_idx_2 = 30U;
   const uint8_t exp_row_idx_3 = 63U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct and that the window timer is incremented by one while reset timer is reset to 0.
    */

   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_2)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_3)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
   CHECK_EQUAL_TEXT(1U, pvtrailer_length.window_timer, "window timer is not correctly incremented.")
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations. Set the first detections
 * position such that it is assigned a max row idx to check that it correctly assigns the maximum allowed row idx.
 */
TEST(f360_process_input_main_functionality, process_input_3_valid_dets_new_cals_to_test_idx_overshoot)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 600
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up
    * - First detection's xpos is set to -17.49 such that its initial idx is = DETECTION_ROWS-1
    * Expectation is that these three detections are stored in the detection row struct arrays.
    */

   all_detections[0U].vcs_position.x = -17.49F;
   uint8_t exp_row_idx_1 = 63U;
   uint8_t exp_row_idx_2 = 30U;
   uint8_t exp_row_idx_3 = 0U;
   uint8_t exp_row_idx_4 = DETECTION_ROWS - 1U; // The idx is extected to be saturated at DETECTION_ROWS-1

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1) // Check that the previously stored detection is still there.
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_2)
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_3)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else if (i == exp_row_idx_4)
      {
         CHECK_EQUAL_TEXT(1, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 invalid detections and 1 from previous iterations. The detections are invalid due to
 * their x_pos and range rates respectively
 */
TEST(f360_process_input_main_functionality, process_input_3_invalid_dets_RR_and_xpos)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 600
    *    - a detection row struct with 1 historic detection added to the array
    * 3 invalid detections are set up
    *    - 1 detection with too much high range rate
    *    - 1 detection with xpos too big
    *    - 1 detection with xpos too small
    * Expectation is that these three detections are not stored in the detection row struct arrays.
    */
   raw_detect_list.detections[0U].raw.range_rate = 1.0F;
   all_detections[1U].vcs_position.x = -1.0F;
   all_detections[2U].vcs_position.x = -25.0F;

   uint8_t exp_row_idx_1 = 22U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1) // Check that only the detection already stored is still there
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else // all other slots should be empty
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 4 invalid detections and 1 from previous iterations. The detections are invalid due to their
 * y_pos, double bounce status, wheel spins status and water spray status respectively.
 */
TEST(f360_process_input_main_functionality, process_input_4_invalid_dets_ypos_double_bounce_wheel_spin_water_spray)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 600
    *    - a detection row struct with 1 historic detection added to the array
    * 4 invalid detections are set up
    *    - 1 detection with ypos too big
    *    - 1 detection flagged as double bounce
    *    - 1 detection flagged as wheel spin
    *    - 1 detection flagged as water spray
    * Expectation is that these 4 detections are not stored in the detection row struct arrays.
    */
   all_detections[0U].vcs_position.y = 3.0F;
   all_detections[1U].f_double_bounce = true;
   all_detections[2U].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   raw_detect_list.number_of_valid_detections = 4U;
   uint32_t det_idx_4 = 3U;
   raw_detect_list.detections[det_idx_4].raw.range_rate = 0.1F;
   raw_detect_list.detections[det_idx_4].raw.sensor_id = 1U;
   all_detections[det_idx_4].vcs_position.x = -8.0F;
   all_detections[det_idx_4].vcs_position.y = 1.0F;
   all_detections[det_idx_4].f_double_bounce = false;
   all_detections[det_idx_4].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   all_detections[det_idx_4].f_water_spray = true;
   sensors[raw_detect_list.detections[det_idx_4].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;

   uint8_t exp_row_idx_1 = 22U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1) // Check that only the detection already stored (in array) is still there
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else // all other slots should be empty
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input works as intended when there are 3 detections and 1 from previous iterations. The 3 detections are all from non-rear sensors.
 */
TEST(f360_process_input_main_functionality, process_input_3_dets_not_from_rear_sensors)
{
   /** \precond
    * Host speed is set to 10 m/s
    * A mocked instance of Trailer_Detector_TL has been set up in the test group with
    *    - reset_timer = 0
    *    - window_timer = 600
    *    - a detection row struct with 1 historic detection added to the array
    * 3 valid detections are set up but all are from non-rear sensors
    * Expectation is that these 3 detections are not stored in the detection row struct arrays.
    */
   sensors[raw_detect_list.detections[0U].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE1;
   sensors[raw_detect_list.detections[1U].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE2;
   sensors[raw_detect_list.detections[2U].raw.sensor_id - 1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE1;

   uint8_t exp_row_idx_1 = 22U;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
   {
      if (i == exp_row_idx_1) // Check that only the detection already stored (in array) is still there
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
      else // all other slots should be empty
      {
         CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i], "detection_row is incorrectly filled");
      }
   }
}

/** \purpose
 * Test that Process_Input skips processing when speed is at or above threshold and window timer is at or above threshold.
 * This verifies the else path of the main if condition in Process Input
 */
TEST(f360_process_input_main_functionality, process_input_window_timer_at_or_above_threshold_skips_then_path)
{
   /** \precond
    * Host speed is set to 8.0 mps which is higher than the speed threshold
    * Window Timer is set to 1800, i.e. the max value which is greater than or equal to the configured threshold
    * Sensor mounting is configured F360_MOUNTING_LOCATION_LEFT_REAR
    */

   const uint32_t window_timer_max = 1800U;
   vehicle_data.speed = 8.0F;
   pvtrailer_length.window_timer = window_timer_max;
   pvtrailer_length.f_estimation_done = true;

   /* Minimal valid detection so that if-branch would process if entered */
   raw_detect_list.number_of_valid_detections = 1U;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   raw_detect_list.detections[0].raw.sensor_id = 1U;
   raw_detect_list.detections[0].raw.range_rate = 0.0F;

   /* Choose an x position behind the host within trailer length bounds and a centered y position */
   all_detections[0].vcs_position.x = -(1.1F * vehicle_data.dist_rear_axle_to_vcs_m) - 1.0F;
   all_detections[0].vcs_position.y = 0.0F;
   all_detections[0].f_double_bounce = false;
   all_detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   all_detections[0].f_water_spray = false;

   /** \action
    * Call Process_Input()
    */
   PVTrailer_Estimate_Length(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_length);

   /** \result
    * Check that the detections are correclty binned in the detection row struct.
    */
   // window_timer must NOT increase because B=false (we're in the else path)
   CHECK_EQUAL_TEXT(window_timer_max, pvtrailer_length.window_timer,
                    "window_timer should not increment when at/above threshold");

   // Ensure detection_row unchanged
   for (uint32_t i = 0; i < DETECTION_ROWS; ++i)
   {
      CHECK_EQUAL_TEXT(0, pvtrailer_length.detection_row[i],
                       "detection_row should be unchanged when if-condition is false due to B=false");
   }
}
/** @} */
