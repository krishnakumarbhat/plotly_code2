/** \file
 * This file contains unit tests for content of f360_calc_obj_processed_size.cpp file
 */

#include "f360_calc_obj_processed_size.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_calc_obj_processed_size
 *  @{
 */

/** \brief
 * The purpose of this test group is to test the functionality of the function Calc_Obj_Processed_Size().
 */
TEST_GROUP(f360_calc_obj_processed_size)
{
   /** \setup
    * Set up variables called by Calc_Obj_Processed_Size function.
    */
    F360_Calibrations_T calib;
    F360_Object_Track_T  obj = {};

    float32_t tolerance = 1e-6F;
   TEST_SETUP()
   {
      // Use default tracker settings for calibrations
      Initialize_Tracker_Calibrations(calib);
      /*Set obj with:
         - CTCA
         - reference point REAR
         - length = 20 and width = 2
         - vcs position x = 50
      */
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.bbox.Set_Length(20.0F);
      obj.bbox.Set_Width(2.0F);
      obj.length_processed = 20.0F;
      obj.width_processed = 2.0F;
      obj.vcs_position.x = 50.0F;
      obj.reference_point = F360_REFERENCE_POINT_REAR;
      
   }
};

/** \purpose  
 * Check if processed length and width changes when the object properties are as in setup
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Obj_Shrink)
{
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length shouldn't change so it should be 20, but processed width should change to 1.95
    */
   float32_t expected_length = 20.0F;
   float32_t expected_width = 1.95F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}

/** \purpose  
 * Check if processed width and length are set correctly when their initial value is 0
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Initial_Processed_Values_Zero)
{
   /** \precond
    * Change length and width processed to 0
    */

    obj.length_processed = 0.0F;
    obj.width_processed = 0.0F;

    obj.bbox.Set_Length(6.0F);
	
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length should be 6 and processed width should be 2.
    */	
   float32_t expected_length = 6.0F;
   float32_t expected_width = 2.0F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}

/** \purpose  
 * Check if processed length and width changes when the object is at 10m lat position.
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Obj_Close_To_Host)
{
   /** \precond
    * Change vcs_position x to 10
    */

    obj.vcs_position.x = 10.0F;
	
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length and width shouldn't be change so their values should stay as 2 and 20.
    */	
   float32_t expected_length = 20.0F;
   float32_t expected_width = 2.0F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}

/** \purpose  
 * Check if processed length and width changes when the object is CCA.
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Obj_CCA)
{
   /** \precond
    * Change object filter type to CCA.
    */

    obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
	
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length and width shouldn't be change so their values should stay as 2 and 20.
    */
   float32_t expected_length = 20.0F;
   float32_t expected_width = 2.0F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}

/** \purpose  
 * Check if processed length and width changes when the object reference point is front.
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Obj_Ref_Point_Front)
{
   /** \precond
    * Change object reference point to front
    */

    obj.reference_point = F360_REFERENCE_POINT_FRONT;
	
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length shouldn't change so it should be 20, but processed width should change to 1.95
    */
   float32_t expected_length = 20.0F;
   float32_t expected_width = 1.95F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}

/** \purpose  
 * Check if processed length and width changes when the object reference point is rear left.
 * \req
 * NA
 */
TEST(f360_calc_obj_processed_size, Test_Obj_Ref_Point_Rear_Left)
{
   /** \precond
    * Change object reference point to rear left
    */

    obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
	
   /** \action
    * call Calc_Obj_Processed_Size().
    */
   Calc_Obj_Processed_Size(calib, obj);

   /** \result
    * Processed length and width shouldn't be change so their values should stay as 2 and 20.
    */
   float32_t expected_length = 20.0F;
   float32_t expected_width = 2.0F;
   DOUBLES_EQUAL_TEXT(expected_length, obj.length_processed, tolerance, "Unexpected value for object processed length.");
   DOUBLES_EQUAL_TEXT(expected_width, obj.width_processed, tolerance, "Unexpected value for object processed width.");
}


/** @}*/
