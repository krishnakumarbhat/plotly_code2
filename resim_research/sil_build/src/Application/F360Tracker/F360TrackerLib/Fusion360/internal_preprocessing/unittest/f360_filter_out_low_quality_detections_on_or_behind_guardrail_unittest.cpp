/** \file
 * This file contains unit tests for content of f360_filter_out_low_quality_detections_on_or_behind_guardrail.cpp file
 */

#include "f360_filter_out_low_quality_detections_on_or_behind_guardrail.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_filter_out_low_quality_detections_on_or_behind_guardrail
 *  @{
 */

 /** \brief
  * Tests using this test group will verify the functionality of function Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail() used to filter out detections with low quality.
  */
TEST_GROUP(f360_filter_out_low_quality_detections_on_or_behind_guardrail)
{
   F360_Calibrations_T calib;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Initialize tracker calibrations
    * Set number of valid detections to 3
    * Set (azimuth confidence, rcs) for the 3 detections to (1, -5), (2, -5), (3, -11)
    * Set the three detections to ok to use
    * Mark the third detection as on guardrail (set on_sep_id > F360_INVALID_UNSIGNED_ID)
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      raw_detection_list.number_of_valid_detections = 3U;

      raw_detection_list.detections[0U].raw.confid_azimuth = 1;
      raw_detection_list.detections[0U].raw.rcs = -5.0F;

      raw_detection_list.detections[1U].raw.confid_azimuth = 2;
      raw_detection_list.detections[1U].raw.rcs = -5.0F;

      raw_detection_list.detections[2U].raw.confid_azimuth = 3;
      raw_detection_list.detections[2U].raw.rcs = -11.0F;

      det_props[0U].f_ok_to_use = true;
      det_props[1U].f_ok_to_use = true;
      det_props[2U].f_ok_to_use = true;

      det_props[2U].on_sep_id = 1;
   }
};

/** \purpose
 * Verify that a detection with poor azimuth quality, low rcs and flagged as on guardrail is correctly marked as not ok to use.
 * \req
 * NA.
 */
TEST(f360_filter_out_low_quality_detections_on_or_behind_guardrail, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail_Detection_On_Guardrail_With_Poor_Azimuth_Quality_And_Low_rcs)
{
   /** \precond
    * In test group setup the following have been set:
    * - Tracker calibrations were initialized.
    * - Number of valid detections was set 3
    * - Detection azimuth confidence and rcs for the 3 detections were set to: (1, -5), (2, -5), (3, -11)
    * - All three detections were marked as ok to use
    * - The third detection was marked as on guardrail
    */

    /** \action
     * Call Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail
     */
   Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(raw_detection_list, calib, det_props);

   /** \result
    * Check that the first two detections are still marked as ok to use
    * Check that the third detection was marked as not ok to use
    */
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The second detection was marked as not ok to use when it should not have been.");
   CHECK_FALSE_TEXT(det_props[2U].f_ok_to_use, "The third detection is still marked as ok to use when it should not be.");
}

/** \purpose
 * Verify that a detection with poor azimuth quality, low rcs and flagged as behind guardrail is correctly marked as not ok to use.
 * \req
 * NA.
 */
TEST(f360_filter_out_low_quality_detections_on_or_behind_guardrail, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail_Detection_Behind_Guardrail_With_Poor_Azimuth_Quality_And_Low_rcs)
{
   /** \precond
    * In test group setup the following have been set:
    * - Tracker calibrations were initialized.
    * - Number of valid detections was set 3
    * - Detection azimuth confidence and rcs for the 3 detections were set to: (1, -5), (2, -5), (3, -11)
    * - All three detections were marked as ok to use
    * Set behind sep id to 1 and on sep id to F360_INVALID_UNSIGNED_ID for the third detection
    */
   det_props[2U].behind_sep_id = 1;
   det_props[2U].on_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action
    * Call Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail
    */
   Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(raw_detection_list, calib, det_props);

   /** \result
    * Check that the first two detections are still marked as ok to use
    * Check that the third detection was marked as not ok to use
    */
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The second detection was marked as not ok to use when it should not have been.");
   CHECK_FALSE_TEXT(det_props[2U].f_ok_to_use, "The third detection is still marked as ok to use when it should not be.");
}

/** \purpose
 * Verify that a detection with poor azimuth quality and low rcs that is not flagged as behind or on guardrail is not filtered out by function.
 * \req
 * NA.
 */
TEST(f360_filter_out_low_quality_detections_on_or_behind_guardrail, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail_Detection_Not_On_Or_Behind_Guardrail_But_Poor_Azimuth_Quality_And_Low_rcs)
{
   /** \precond
    * In test group setup the following have been set:
    * - Tracker calibrations were initialized.
    * - Number of valid detections was set 3
    * - Detection azimuth confidence and rcs for the 3 detections were set to: (1, -5), (2, -5), (3, -11)
    * - All three detections were marked as ok to use
    * Set on sep id to F360_INVALID_UNSIGNED_ID for the third detection
    */
   det_props[2U].on_sep_id = F360_INVALID_UNSIGNED_ID;

   /** \action
    * Call Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail
    */
   Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(raw_detection_list, calib, det_props);

   /** \result
    * Check that all three detections are still marked as ok to use
    */
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The second detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[2U].f_ok_to_use, "The third detection was marked as not ok to use when it should not have been.");
}

/** \purpose
 * Verify that a detection with low rcs that is flagged as on guardrail is not filtered out by function if azimuth confidence is not poor.
 * \req
 * NA.
 */
TEST(f360_filter_out_low_quality_detections_on_or_behind_guardrail, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail_Detection_On_Guardrail_Low_rcs_But_Not_Poor_Azimuth_Confidence)
{
   /** \precond
    * In test group setup the following have been set:
    * - Tracker calibrations were initialized.
    * - Number of valid detections was set 3
    * - Detection azimuth confidence and rcs for the 3 detections were set to: (1, -5), (2, -5), (3, -11)
    * - All three detections were marked as ok to use
    * - The third detection was marked as on guardrail
    * Set azimuth confidence for the third detection to 2
    */
   raw_detection_list.detections[2U].raw.confid_azimuth = 2;

   /** \action
    * Call Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail
    */
   Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(raw_detection_list, calib, det_props);

   /** \result
    * Check that all three detections are still marked as ok to use
    */
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The second detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[2U].f_ok_to_use, "The third detection was marked as not ok to use when it should not have been.");
}

/** \purpose
 * Verify that a detection with poor azimuth confidence that is flagged as on guardrail is not filtered out by function if rcs of detection is not low enough.
 * \req
 * NA.
 */
TEST(f360_filter_out_low_quality_detections_on_or_behind_guardrail, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail_Detection_On_Guardrail_Poor_Azimuth_Confidence_But_Not_Low_rcs)
{
   /** \precond
    * In test group setup the following have been set:
    * - Tracker calibrations were initialized.
    * - Number of valid detections was set 3
    * - Detection azimuth confidence and rcs for the 3 detections were set to: (1, -5), (2, -5), (3, -11)
    * - All three detections were marked as ok to use
    * - The third detection was marked as on guardrail
    * Set rcs for the third detection to -9
    */
   raw_detection_list.detections[2U].raw.rcs = -9.0F;

   /** \action
    * Call Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail
    */
   Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(raw_detection_list, calib, det_props);

   /** \result
    * Check that all three detections are still marked as ok to use
    */
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The second detection was marked as not ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[2U].f_ok_to_use, "The third detection was marked as not ok to use when it should not have been.");
}
/** @}*/
