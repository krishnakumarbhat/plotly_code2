/** \file
 * This file contains unit tests for content of f360_det_is_in_bistatic_fp_area.cpp file
 */

#include "f360_det_is_in_bistatic_fp_area.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_det_is_in_bistatic_fp_area
 *  @{
 */

/** \brief
 * Test group of Det_Is_In_Bistatic_FP_Area function. Tests verify whether function properly
 * determines whether detection is placed inside FP area.
 */
TEST_GROUP(f360_det_is_in_bistatic_fp_area)
{
   const float tolerance = 0.0001F;
   F360_Calibrations_T calib{};
   Point det_pos{};

   /** \setup
    * Initialize tracker calibrations
    * set point inside area
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      det_pos.y = calib.k_bistatic_cond_assoc_area_min_lat + tolerance;
      det_pos.x = calib.k_bistatic_cond_assoc_area_max_lon - tolerance;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether when detection is placed in bistatic FP area - function returns true
 * \req
 * NA.
 */
TEST(f360_det_is_in_bistatic_fp_area, Det_Is_In_Bistatic_FP_Area__Det_Is_In_FP_Area)
{
   /** \precond
    * Same as setup
    */
	
   /** \action
    * Call tested function
    */
   const bool f_is_in_fp_area = Is_Det_In_Bistatic_FP_Area(det_pos, calib);

   /** \result
    * Check whether function returned true
    */	
   CHECK_TRUE(f_is_in_fp_area);

}

/** \purpose
* Purpose of this test is to verify whether when detection is placed too far laterally towards left
* function returns false
* \req
* NA.
*/
TEST(f360_det_is_in_bistatic_fp_area, Det_Is_In_Bistatic_FP_Area__Det_Is_Not_In_FP_Area_Too_Low_Lat_Pos)
{
   /** \precond
   * Put detection outside area
   */
   det_pos.y = calib.k_bistatic_cond_assoc_area_min_lat - tolerance;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fp_area = Is_Det_In_Bistatic_FP_Area(det_pos, calib);

   /** \result
   * Check whether function returned false
   */
   CHECK_FALSE(f_is_in_fp_area);

}

/** \purpose
* Purpose of this test is to verify whether when detection is placed too far laterally towards right
* function returns false
* \req
* NA.
*/
TEST(f360_det_is_in_bistatic_fp_area, Det_Is_In_Bistatic_FP_Area__Det_Is_Not_In_FP_Area_Too_High_Lat_Pos)
{
   /** \precond
   * Put detection outside area
   */
   det_pos.y = calib.k_bistatic_cond_assoc_area_max_lat + tolerance;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fp_area = Is_Det_In_Bistatic_FP_Area(det_pos, calib);

   /** \result
   * Check whether function returned false
   */
   CHECK_FALSE(f_is_in_fp_area);

}

/** \purpose
* Purpose of this test is to verify whether when detection is placed too far longitudinally towards front
* function returns false
* \req
* NA.
*/
TEST(f360_det_is_in_bistatic_fp_area, Det_Is_In_Bistatic_FP_Area__Det_Is_Not_In_FP_Area_Too_High_Long_Pos)
{
   /** \precond
   * Put detection outside area
   */
   det_pos.x = calib.k_bistatic_cond_assoc_area_max_lon + tolerance;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fp_area = Is_Det_In_Bistatic_FP_Area(det_pos, calib);

   /** \result
   * Check whether function returned false
   */
   CHECK_FALSE(f_is_in_fp_area);

}

/** \purpose
* Purpose of this test is to verify whether when detection is placed too far longitudinally towards rear
* function returns false
* \req
* NA.
*/
TEST(f360_det_is_in_bistatic_fp_area, Det_Is_In_Bistatic_FP_Area__Det_Is_Not_In_FP_Area_Too_Low_Long_Pos)
{
   /** \precond
   * Put detection outside area
   */
   det_pos.x = calib.k_bistatic_cond_assoc_area_min_lon - tolerance;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fp_area = Is_Det_In_Bistatic_FP_Area(det_pos, calib);

   /** \result
   * Check whether function returned false
   */
   CHECK_FALSE(f_is_in_fp_area);
}
/** @}*/
