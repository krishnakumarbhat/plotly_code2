/** \file
 * This file contains unit tests for content of f360_update_object_confidence_levels.cpp file
 */

#include "f360_update_object_confidence_levels.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_update_object_confidence_levels
 *  @{
 */

/** \brief
 * Test group of Update_Object_Confidence_Levels() function. Those tests are high-level
 * functionality tests that verify whether specific objects are properly analysed and 
 * their confidence level is updated basing on their flags
 */
TEST_GROUP(f360_update_object_confidence_levels)
{
   F360_Tracker_Info_T tracker_info{};
   F360_Calibrations_T calib{};
   F360_Object_Track_T object{};
   
   /** \setup
    * Initialize tracker calibrations
    * Set elapsed time to 0.05
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      tracker_info.elapsed_time_s = 0.05F;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether new objects do not have their confidence level lowered if none flags suggests to
 * \req
 * NA.
 */
TEST(f360_update_object_confidence_levels, Update_Object_Confidence_Levels__new_objects_do_not_have_their_conf_level_changed)
{
   /** \precond
    * Set up object status to NEW
    * Set up object confidenceLevel
    * Reset object f_overlapping_with_object flag
    */
   object.status = F360_OBJECT_STATUS_NEW;
   object.confidenceLevel = 0.8F;
   object.f_overlapping_with_object = false;
	
   /** \action
    * Call Update_Object_Confidence_Levels
    */
   Update_Object_Confidence_Levels(tracker_info, calib, object);

   /** \result
    * Check whether object confidence level was not modified
    */	
   DOUBLES_EQUAL(0.8F, object.confidenceLevel, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether new objects have their confidence level lowered when flag suggests to
 * \req
 * NA.
 */
TEST(f360_update_object_confidence_levels, Update_Object_Confidence_Levels__new_objects_have_their_conf_level_lowered_when_object_properties_suggests_to)
{
   /** \precond
    * Set up object status to NEW
    * Set up object confidenceLevel
    * Set object f_overlapping_with_object flag
    */
   object.status = F360_OBJECT_STATUS_NEW;
   object.confidenceLevel = 0.8F;
   object.f_overlapping_with_object = true;

   /** \action
    * Call Update_Object_Confidence_Levels
    */
   Update_Object_Confidence_Levels(tracker_info, calib, object);

   /** \result
    * Check whether object confidence level was lowered to 0.72F
    */
   DOUBLES_EQUAL(0.72F, object.confidenceLevel, 1e-4);
}

/** \purpose
 * Purpose of this test is to verify whether coasted objects have their confidence level properly calculated
 * \req
 * NA.
 */
TEST(f360_update_object_confidence_levels, Update_Object_Confidence_Levels__coasted_objects_have_their_conf_level_properly_calculated)
{
   /** \precond
    * Set up object status to COASTED
    * Set up object confidenceLevel
    * Set up object time since updated to be above threshold
    */
   object.status = F360_OBJECT_STATUS_COASTED;
   object.confidenceLevel = 0.8F;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects + 0.1F;

   /** \action
    * Call Update_Object_Confidence_Levels
    */
   Update_Object_Confidence_Levels(tracker_info, calib, object);

   /** \result
    * Check whether object confidence level was set to 0.623040617
    */
   DOUBLES_EQUAL(0.623040617F, object.confidenceLevel, 1e-4);
}
/** @}*/
