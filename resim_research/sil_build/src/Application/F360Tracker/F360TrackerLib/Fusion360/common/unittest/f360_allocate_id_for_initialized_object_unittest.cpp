/** \file
 * This file contains unit tests for content of f360_allocate_id_for_initialized_object.cpp file
 */

#include "f360_allocate_id_for_initialized_object.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_allocate_id_for_initialized_object
 *  @{
 */

/** \brief
 * Add brief description of test group, i.e. describe what functionality is tested.
 * When using multiple test groups, make sure to write a brief description for each test group.
 * The description should be unique and describe the specific scenario that is tested in that group.
 */
TEST_GROUP(f360_allocate_id_for_initialized_object)
{	
   // Declare common variables used within all tests in this test group.
   
   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
   }

   /** \teardown
    * Describe what is done in test teardown. Remove test teardown function and this tag if it is not used.
    */
   TEST_TEARDOWN()
   {
      // Perform any necessary clean up. E.g. mock().clear().
   }
   
   // Define helper functions used in this test group here. E.g. Add_Detection_To_Detection_List().
};

/** \purpose  
 * Describe purpose of test on a high level. The description should be unique and distinguishable from other tests' purposes.
 * E.g. test that function Some_Function() works as intended given some specific preconditions.
 * \req
 * Put in requirement tag if any otherwise set to NA.
 */
TEST(f360_allocate_id_for_initialized_object, FunctionToTest_Descriptive_Tag)
{
   /** \precond
    * Describe prepared input. E.g. describe relevant set up from test group and what variables are changed or added for this specific test.
    */
	
   /** \action
    * Describe the action of the test. E.g. call Some_Function().
    */

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */	
}
/** @}*/
