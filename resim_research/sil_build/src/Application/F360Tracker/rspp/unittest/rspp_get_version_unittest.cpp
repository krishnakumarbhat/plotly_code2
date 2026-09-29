/** \file
 * This file contains unit tests for content of rspp_get_version.cpp file
 */

#include "rspp_get_version.h"
#include "rspp_version.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_Get_Version
 *  @{
 */

/** \brief
 * Tests for RSPP_Get_Version function which retrieves the RSPP software version information
 * including major, minor, patch version numbers and build identifier.
 */
TEST_GROUP(test_RSPP_Get_Version){
    // No common variables required - each test declares its own version output variables.
};

/** \purpose
 * Test that RSPP_Get_Version correctly returns the expected version information
 * matching the defined version constants.
 * \req
 * NA
 */
TEST(test_RSPP_Get_Version, RSPP_Get_Version_TC_RSPP_Get_Version_Output_Test)
{
   /** \step{1}
    * Verify RSPP_Get_Version returns correct version information.
    */

   /** \precond
    * Declare variables to receive version information output.
    */
   int8_t major;
   int8_t minor;
   int8_t patch;
   uint64_t build_id;

   /** \action
    * Call RSPP_Get_Version() to retrieve version information.
    */
   RSPP_Get_Version(&major, &minor, &patch, &build_id);

   /** \result
    * Verify returned values match expected version constants (RSPP_Version_Major,
    * RSPP_Version_Minor, RSPP_Version_Patch, RSPP_VERSION_BUILD_ID).
    */
   CHECK_EQUAL(RSPP_Version_Major, major);
   CHECK_EQUAL(RSPP_Version_Minor, minor);
   CHECK_EQUAL(RSPP_Version_Patch, patch);
   CHECK_EQUAL(RSPP_VERSION_BUILD_ID, build_id);
}
/** @}*/
