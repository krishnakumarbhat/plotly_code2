/** \file
   Unit tests for update_object_average_rcs.cpp file
*/

#include "f360_update_object_average_rcs.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  Update_Object_Average_Rcs
*  @{
*/

/** \brief
* Test group for testing function Update_Object_Average_Rcs()
*/
TEST_GROUP(f360_update_object_average_rcs)
{
   F360_Object_Track_T object_track = {};
   rspp_variant_A::RSPP_Detection_T raw_dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Calibrations_T calibs = {};
   const float32_t tolerance = 1e-4;

   /** \setup
   * Fill detection properties and object data with initial information
   **/
   TEST_SETUP()
   {

      Initialize_Tracker_Calibrations(calibs);

      object_track.ndets = 5;
      object_track.detids[0] = 1U;
      object_track.detids[1] = 2U;
      object_track.detids[2] = 20U;
      object_track.detids[3] = 35U;
      object_track.detids[4] = 37U;
      object_track.status = F360_OBJECT_STATUS_NEW_UPDATED;
      object_track.average_rcs = 10.0F;

      raw_dets[0].raw.rcs = 0.0F;
      raw_dets[1].raw.rcs = -4.0F;
      raw_dets[19].raw.rcs = 2.0F;
      raw_dets[34].raw.rcs = 8.0F;
      raw_dets[36].raw.rcs = -30.0F;
   }
};

/** \purpose
* Update_Object_Average_Rcs should calculate and update average rcs based on associated detections
*  rcs when object status is new updated
* \req
* NA
*/
TEST(f360_update_object_average_rcs, Check_If_Average_RCS_Updated_Correctly_When_Object_Status_New_Updated)
{
   /** \precond
   * Set expected average_rcs_value
   */
   const float32_t expected_average_rcs = 0.722021F;

   /** \action
   * Call Update_Object_Average_Rcs()
   */
   Update_Object_Average_Rcs(raw_dets, calibs, object_track);

   /** \result
   * Function should update average_rcs to expected value
   */

   DOUBLES_EQUAL(expected_average_rcs, object_track.average_rcs, tolerance);
}

/** \purpose
* Update_Object_Average_Rcs should calculate and update average rcs based on associated detections
*  rcs and previous rcs value when object status is updated
* \req
* NA
*/
TEST(f360_update_object_average_rcs, Check_If_Average_RCS_Updated_Correctly_When_Object_Status_Updated)
{
   /** \precond
   * Set object status to updated
   * Set expected average_rcs_value
   */
   object_track.status = F360_OBJECT_STATUS_UPDATED;
   const float32_t expected_average_rcs = 8.608303F;

   /** \action
   * Call Update_Object_Average_Rcs()
   */
   Update_Object_Average_Rcs(raw_dets, calibs, object_track);

   /** \result
   * Function should update average_rcs to expected value
   */

   DOUBLES_EQUAL(expected_average_rcs, object_track.average_rcs, tolerance);
}

/** \purpose
* Update_Object_Average_Rcs should calculate and update average rcs based on associated detections
*  rcs and previous rcs value when object status is coasted
* \req
* NA
*/
TEST(f360_update_object_average_rcs, Check_If_Average_RCS_Updated_Correctly_When_Object_Status_Coasted)
{
   /** \precond
   * Set object status to coasted
   * Set expected average_rcs_value
   */
   object_track.status = F360_OBJECT_STATUS_COASTED;
   const float32_t expected_average_rcs = 8.608303F;

   /** \action
   * Call Update_Object_Average_Rcs()
   */
   Update_Object_Average_Rcs(raw_dets, calibs, object_track);

   /** \result
   * Function should update average_rcs to expected value
   */

   DOUBLES_EQUAL(expected_average_rcs, object_track.average_rcs, tolerance);
}

/** \purpose
* Update_Object_Average_Rcs should not modify average rcs value when no associated detecions
* \req
* NA
*/
TEST(f360_update_object_average_rcs, Check_If_Average_RCS_Not_Updated_When_No_Associated_Detections)
{
   /** \precond
   * Set number of associated detections to zero
   * Set expected average_rcs_value
   */
   object_track.ndets = 0;
   const float32_t expected_average_rcs = object_track.average_rcs;

   /** \action
   * Call Update_Object_Average_Rcs()
   */
   Update_Object_Average_Rcs(raw_dets, calibs, object_track);

   /** \result
   * Function should update average_rcs to expected value
   */

   DOUBLES_EQUAL(expected_average_rcs, object_track.average_rcs, tolerance);
}


/** \purpose
* Update_Object_Average_Rcs should calculate and update average rcs based on associated detections
*  rcs when object status is new updated when all rcss equal to zero
* \req
* NA
*/
TEST(f360_update_object_average_rcs, Check_If_Average_RCS_Updated_Correctly_When_Object_Status_New_Updated_And_rcss_Equal_To_Zero)
{
   /** \precond
   * Set all rcss to zero
   * Set expected average_rcs_value
   */
   raw_dets[0].raw.rcs = 0.0F;
   raw_dets[1].raw.rcs = 0.0F;
   raw_dets[19].raw.rcs = 0.0F;
   raw_dets[34].raw.rcs = 0.0F;
   raw_dets[36].raw.rcs = 0.0F;
   object_track.average_rcs = 0.0F;
   const float32_t expected_average_rcs = 0.0F;

   /** \action
   * Call Update_Object_Average_Rcs()
   */
   Update_Object_Average_Rcs(raw_dets, calibs, object_track);

   /** \result
   * Function should update average_rcs to expected value
   */
   DOUBLES_EQUAL(expected_average_rcs, object_track.average_rcs, tolerance);
}

