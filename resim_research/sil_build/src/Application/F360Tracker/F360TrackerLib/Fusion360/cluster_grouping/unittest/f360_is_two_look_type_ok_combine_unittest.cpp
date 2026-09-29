/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_is_two_look_type_ok_combine.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_is_two_look_type_ok_combine
 *  @{
 */

/** \brief
*  Test Group for testing 
*  Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased 
*  and Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased
*  function.
**/
TEST_GROUP(f360_is_two_look_type_ok_combine)
{
   F360_Globals_T globals = { };
   F360_Calibrations_T calib = { };
   float32_t rdot_1 = 0.0F;
   float32_t rdot_2 = 0.0F;
   float32_t tolerance = 1e-6F;
   bool f_success;

   /** \setup
   * Initialize calibrations max OTG speed.
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      globals.max_otg_speed = 70.0F;
   }
};

/**
*\purpose  Check whether function does not let combine two look types, when defined speed interval is greater than maximum speed threshold.  
*\req      NA
*/
TEST(f360_is_two_look_type_ok_combine, Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased_failing)
{
   /** \precond
   *    Define speed interval.
   **/
   float32_t index;
   float32_t interval = globals.max_otg_speed * 2.0F;

   /** \action
   * call Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased().
   **/
   Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased(globals, calib, rdot_1, rdot_2, interval, f_success, index);

   /** \result
   * Check if functions returns false and expected index value is inside boundaries.
   **/
   CHECK_FALSE(f_success);
   DOUBLES_EQUAL(0.0F, index, tolerance);
}

/**
*\purpose  Check whether function does not let combine two look types, when defined speed intervals are below max interval thresholds 
*          However, those intervals have exactly the same values.
*\req      NA
*/
TEST(f360_is_two_look_type_ok_combine, Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased_failing)
{
    /** \precond
    *    Define speed interval.
    **/
    float32_t index_1;
    float32_t index_2;
    float32_t interval_1 = (globals.max_otg_speed * 2.0F) - 1.0F;
    float32_t interval_2 = interval_1;

    /** \action
    *   call Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased().
    **/
    Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(globals, calib, rdot_1, rdot_2, interval_1, interval_2, f_success, index_1, index_2);

    /** \result
    * Check if functions returns false and expected index value is inside boundaries.
    **/
    CHECK_FALSE(f_success);
    DOUBLES_EQUAL(0.0F, index_1, tolerance);
    DOUBLES_EQUAL(0.0F, index_2, tolerance);
}

/**
*\purpose  Check whether function does not let combine two look types, when defined speed intervals are above max interval thresholds 
*          and those intervals have exactly the same values.
*\req      NA
*/
TEST(f360_is_two_look_type_ok_combine, Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased_first_interval_bigger_than_max_otg_speed)
{
    /** \precond
    *    Define speed interval.
    **/
    float32_t index_1;
    float32_t index_2;
    float32_t interval_1 = globals.max_otg_speed * 2.0F;
    float32_t interval_2 = interval_1;

    /** \action
    *   call Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased().
    **/
    Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(globals, calib, rdot_1, rdot_2, interval_1, interval_2, f_success, index_1, index_2);

    /** \result
    * Check if functions returns true and expected index value is inside boundaries.
    **/
    CHECK_TRUE(f_success);
    DOUBLES_EQUAL(0.0F, index_1, tolerance);
    DOUBLES_EQUAL(0.0F, index_2, tolerance);
}

/**
*\purpose  Check whether function lets combine two look types, when defined speed intervals are below max interval thresholds
*          and those intervals difference is within threshold.
*\req      NA
*/
TEST(f360_is_two_look_type_ok_combine, Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased_big_interval_difference)
{
    /** \precond
    *    Define speed interval.
    **/
    float32_t index_1;
    float32_t index_2;
    float32_t interval_1 = (globals.max_otg_speed * 2.0F) - 1.F;
    float32_t interval_2 = interval_1 + 1U;

    /** \action
    *   call Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased().
    **/
    Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(globals, calib, rdot_1, rdot_2, interval_1, interval_2, f_success, index_1, index_2);

    /** \result
    * Check if functions returns true and expected index value is inside boundaries.
    **/
    CHECK_TRUE(f_success);
    DOUBLES_EQUAL(0.0F, index_1, tolerance);
    DOUBLES_EQUAL(0.0F, index_2, tolerance);
}

/** @}*/
