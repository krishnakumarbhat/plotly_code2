/** \file
 * This file contains unit tests for content of f360_update_object_maximum_rcs.cpp file
 */

#include "f360_update_object_maximum_rcs.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_update_object_maximum_rcs
 *  @{
 */

/** \brief
 * Add brief description of test group, i.e. describe what functionality is tested.
 * When using multiple test groups, make sure to write a brief description for each test group.
 * The description should be unique and describe the specific scenario that is tested in that group.
 */
TEST_GROUP(f360_update_object_maximum_rcs)
{	
   F360_Object_Track_T obj_trk;
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
};

/** \purpose  
 * Check that maximum_rcs does not go lower than -50
 * \req
 * NA
 */
TEST(f360_update_object_maximum_rcs, f360_update_object_maximum_rcs_saturation)
{
   obj_trk.maximum_rcs = -50.0F;
   obj_trk.ndets = 0;
   obj_trk.status = F360_OBJECT_STATUS_COASTED;
	
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */
   Update_Object_Maximum_Rcs(detections, obj_trk);

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */	
   DOUBLES_EQUAL(-50.0F, obj_trk.maximum_rcs, 0.01F);	
}

/** \purpose  
 * Check that maximum_rcs updates to higher rcs value from detections
 * \req
 * NA
 */
TEST(f360_update_object_maximum_rcs, f360_update_object_maximum_rcs_update_to_higher_value)
{
   obj_trk.maximum_rcs = -10.0F;
   obj_trk.ndets = 2;
   obj_trk.detids[0] = 1;
   obj_trk.detids[1] = 2;
   obj_trk.status = F360_OBJECT_STATUS_UPDATED;
   detections[0].raw.rcs = -5.0F;
   detections[1].raw.rcs = -20.0F;
	
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */
   Update_Object_Maximum_Rcs(detections, obj_trk);

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */	
   DOUBLES_EQUAL(-5.0F, obj_trk.maximum_rcs, 0.01F);	
}

/** \purpose  
 * Check that maximum_rcs does not go lower than -50
 * \req
 * NA
 */
TEST(f360_update_object_maximum_rcs, f360_update_object_maximum_rcs__update_to_lower_value)
{
   obj_trk.maximum_rcs = -5.0F;
   obj_trk.ndets = 1;
   obj_trk.detids[0] = 1;
   obj_trk.status = F360_OBJECT_STATUS_UPDATED;
   detections[0].raw.rcs = -6.0F;
	
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */
   Update_Object_Maximum_Rcs(detections, obj_trk);

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */	
   DOUBLES_EQUAL(-5.01F, obj_trk.maximum_rcs, 0.001F);	
}
/** @}*/
