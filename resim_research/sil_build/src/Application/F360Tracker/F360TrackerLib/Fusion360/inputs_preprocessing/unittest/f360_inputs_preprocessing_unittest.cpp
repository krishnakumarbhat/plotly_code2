/** \file
 * This file contains unit tests for content of f360_inputs_preprocessing.cpp file
 */

#include "f360_inputs_preprocessing.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_inputs_preprocessing
 *  @{
 */

TEST_GROUP(f360_inputs_preprocessing)
{	
   // Declare common variables used within all tests in this test group.
   const float32_t test_pass_th = 1e-6F;
   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
   }

};

/** @}*/
