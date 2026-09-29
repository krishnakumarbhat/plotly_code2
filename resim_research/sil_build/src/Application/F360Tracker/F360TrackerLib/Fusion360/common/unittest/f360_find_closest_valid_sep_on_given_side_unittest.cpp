/** \file
 * This file contains unit tests for content of f360_find_closest_valid_sep_on_given_side.cpp file
 */

#include "f360_find_closest_valid_sep_on_given_side.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_find_closest_valid_sep_on_side
 *  @{
 */

  /** \brief
   * Create valid SEPs that are tweaked for the various test cases
   */
TEST_GROUP(f360_Find_Closest_Valid_SEP_On_Side_Test_Group)
{
   Static_Env_Poly_T stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Side_Of_Host_T host_side = F360_HOST_LEFT_SIDE;
   float32_t long_pos = 0.0F;

   /** \setup
    * Set up default SEPs and other default parameters
    */
   TEST_SETUP()
   {
      for (uint16_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
      {
         stat_env_poly[sep_idx].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
         stat_env_poly[sep_idx].lower_limit = -20.0F;
         stat_env_poly[sep_idx].upper_limit = 20.0F;
         stat_env_poly[sep_idx].p2 = 0.0F;
         stat_env_poly[sep_idx].p1 = 0.0F;

         if (sep_idx == 0)
         {
            stat_env_poly[sep_idx].p0 = -5.0F;
         }
         else if (sep_idx == 1)
         {
            stat_env_poly[sep_idx].p0 = 5.0F;
         }
         else
         {
            stat_env_poly[sep_idx].p0 = 2.0F * stat_env_poly[sep_idx - 2].p0;
         }
      }
   }

};

/** \purpose
 * Verifies whether no SEP is found when no SEP is valid for given longitudinal position or the SEP is invalid
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__No_SEP_Valid)
{
   /** \precond
      * Set SEPs so they are not valid
   */

   stat_env_poly[0].lower_limit = -40.0F;
   stat_env_poly[0].upper_limit = -20.0F;

   stat_env_poly[1].lower_limit = 20.0F;
   stat_env_poly[1].upper_limit = 40.0F;

   for (uint16_t sep_idx = 2U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
   {
      stat_env_poly[sep_idx].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   }

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify no sep is found and that sep_lat_pos variable is unchanged
    */
   CHECK_TRUE(F360_INVALID_UNSIGNED_ID == closest_sep_info.id);
   DOUBLES_EQUAL(0.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether correct SEP is found on host left side
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__Valid_SEP_Left_Side)
{
   /** \precond
      * Defined in the test group
   */

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify that sep is found at expected lateral position
    */
   CHECK_TRUE(closest_sep_info.id > F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether correct SEP is found on host right side, closest SEP is last in array. Meaning
 * closest SEP will be replaced inside the function.
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__Valid_SEP_Right_Side)
{
   /** \precond
   * Switch lateral sign to right side
   * Switch position of the SEP's on right side
   */
   host_side = F360_HOST_RIGHT_SIDE;
   stat_env_poly[1].p0 = 10.0F;
   stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS - 1U].p0 = 5.0F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify that sep is found at expected lateral position
    */
   CHECK_TRUE(closest_sep_info.id > F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether correct SEP is found on host left side when none of the SEP coefficients are zero and
 * when the given longitudinal position is different from zero.
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__Valid_SEP_Left_Side_All_Poly_Coeffs_Long_Pos_Non_Zero)
{
   /** \precond
      * Set coefficents of the SEP's to be different from zero, c coefficents are unchanged from test group
      * Set the longitudinal position different from zero
   */
   stat_env_poly[0].p2 = 1.0F;
   stat_env_poly[0].p1 = 1.0F;
   stat_env_poly[1].p2 = 1.0F;
   stat_env_poly[1].p1 = 1.0F;
   stat_env_poly[2].p2 = 1.0F;
   stat_env_poly[2].p1 = 1.0F;
   stat_env_poly[3].p2 = 1.0F;
   stat_env_poly[3].p1 = 1.0F;
   long_pos = -2.0F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify that sep is found at expected lateral position
    * y = p2*x^2 + p1*x + p0 = 1 * (-2)^2 + 1 * (-2) + (-5) = -3
    */
   CHECK_TRUE(closest_sep_info.id > F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-3.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether no SEP is found when invalid host side is passed to function
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__Invalid_Host_Side)
{
   /** \precond
      * Set host side as invalid
   */
   host_side = F360_HOST_INVALID_SIDE;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify no sep is found and that sep_lat_pos variable is unchanged
    */
   CHECK_TRUE(closest_sep_info.id == F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(0.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether SEP is found when longitudinal margin is provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__Lower_Than_Min_Lateral_dist)
{
   /** \precond
      * Set longitudinal margin just below abs(sep lateral position)
   */

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos);

   /** \result
    * Verify sep is found and lat pos = -5.0F
    */
   CHECK_TRUE(closest_sep_info.id != F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether SEP is found when longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__within_Margin_from_up)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos just below upper limit
   */
   const float long_margin = 2.0F;
   long_pos = 22.0F - 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify sep is found and lat pos = -5.0F
    */
   CHECK_TRUE(closest_sep_info.id != F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether SEP is found and longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__within_Margin_from_down)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos just above lower limit
   */
   const float long_margin = 2.0F;
   long_pos = -22.0F + 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify sep is found and lat pos = -5.0F
    */
   CHECK_TRUE(closest_sep_info.id != F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether SEP is NOT found and longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__on_upper_limit_with_margin)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos equal to upper limit
   */
   const float long_margin = 2.0F;
   long_pos = 22.0F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify no sep is found
    */
   CHECK_TRUE(closest_sep_info.id == F360_INVALID_UNSIGNED_ID);
}

/** \purpose
 * Verifies whether SEP is NOT found longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__above_upper_limit_with_margin)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos slightly above upper limit
   */
   const float long_margin = 2.0F;
   long_pos = 22.0F + 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify no sep is found
    */
   CHECK_TRUE(closest_sep_info.id == F360_INVALID_UNSIGNED_ID);
}

/** \purpose
 * Verifies whether SEP is NOT found when longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__on_lower_limit_with_margin)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos equal to lower limit
   */
   const float long_margin = 2.0F;
   long_pos = -22.0F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify no sep is found
    */
   CHECK_TRUE(closest_sep_info.id == F360_INVALID_UNSIGNED_ID);
}

/** \purpose
 * Verifies whether SEP is NOT found when longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__below_lower_limit_with_margin)
{
   /** \precond
      * Set margin to 2.0F
      * Set long pos slightly below lower limit
   */
   const float long_margin = 2.0F;
   long_pos = -22.0F - 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin);

   /** \result
    * Verify no sep is found
    */
   CHECK_TRUE(closest_sep_info.id == F360_INVALID_UNSIGNED_ID);
}

/** \purpose
 * Verifies whether SEP is found when min lateral distance and longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__min_lateral_distance_lower_tahn_first_sep)
{
   /** \precond
      * Set min lateral distance just below first sep lateral postion
      * Set margin to 2.0F
      * Set long pos slightly above lower limit
   */
   const float long_margin = 2.0F;
   const float min_abs_lat_pos = 5.0F - 0.0001F;
   long_pos = -22.0F + 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin, min_abs_lat_pos);

   /** \result
    * Verify sep is found and lat pos = -5.0F
    */
   CHECK_TRUE(closest_sep_info.id != F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-5.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** \purpose
 * Verifies whether SEP is found when min lateral distance and longitudinal margin are provieded
 * \req
 * NA
 */
TEST(f360_Find_Closest_Valid_SEP_On_Side_Test_Group, Find_Closest_Valid_SEP_On_Side__min_lateral_distance_higher_than_first_sep)
{
   /** \precond
      * Set min lateral distance just above first sep lateral postion
      * Set margin to 2.0F
      * Set long pos slightly above lower limit
   */
   const float long_margin = 2.0F;
   const float min_abs_lat_pos = 5.0F + 0.0001F;
   long_pos = -22.0F + 0.00001F;

   /** \action
    * Call function
    */
   Closest_SEP_Info closest_sep_info = Find_Closest_SEP_On_Given_Side(stat_env_poly, host_side, long_pos, long_margin, min_abs_lat_pos);

   /** \result
    * Verify sep is found and lat pos = -10.0F
    */
   CHECK_TRUE(closest_sep_info.id != F360_INVALID_UNSIGNED_ID);
   DOUBLES_EQUAL(-10.0F, closest_sep_info.lat_pos, F360_EPSILON);
}

/** @}*/
