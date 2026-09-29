/** \file
 * This file contains unit tests for content of f360_update_merged_objects_properties.cpp file
 */

#include "f360_update_merged_objects_properties.h"
#include <CppUTest/TestHarness.h>

#include "f360_trk_fltr_ccv_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_pseudo_msmt.h"
#include "f360_clear_object_track.h"
#include "f360_try_to_merge_two_objects.h"

using namespace f360_variant_A;

/** \defgroup  f360_update_merged_objects_properties
   *  @{
   */

/** \brief
*  This purpose of this test group is to test the functionality of the function Update_Merged_Objects_Properties()
**/
TEST_GROUP(f360_update_merged_objects_properties)
{
   /** \setup
   *  Set up default values for input parameters
   **/
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Calibrations_T calibs = {};
   F360_Dimensions_T dimensions = {};
   F360_Object_Track_T object_to_keep = {};
   F360_Object_Track_T object_to_kill = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   const float32_t tolerance = 1e-6F;
   Point merged_object_center = {}; 

   TEST_SETUP()
   {
      // Setup tracker calibrations
      Initialize_Tracker_Calibrations(calibs);

      globals.f_single_front_center_radar_only = false;

      // Set the distance to real axel for host
      host.dist_rear_axle_to_vcs_m = 3.0F;

      // Initialize measurement covariance matrices with non-zero values
      object_to_kill.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = 1.1F;
      object_to_kill.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = 1.2F;

      object_to_keep.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = 2.1F;
      object_to_keep.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = 2.2F;

      // Initialize error covariance matrices with non-zero values
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = 6.1F;
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = 6.2F;
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = 6.3F;
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = 6.4F;
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = 6.5F;
      object_to_kill.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = 6.6F;

      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = 4.1F;
      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = 4.2F;
      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = 4.3F;
      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = 4.4F;
      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = 4.5F;
      object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = 4.6F;

   }
};

/**
*\purpose  Check if pseudo-measurement covariance matrix is updated correctly
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Update_Pseudo_Measurement_Covariance_Matrix_After_Merge)
{
   /** \precond
    * Set object filter types
    * Set expected pseudo-measurement covariance matrix
    **/

   float32_t expected_meascov[F360_PSEUDO_MEAS_DIM][F360_PSEUDO_MEAS_DIM] = {};
   expected_meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = object_to_keep.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X];
   expected_meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = object_to_keep.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y];

   /** \action
   * Call tested function
   **/
   Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Compare expected meascov matrix with the actual one
   **/
   for (uint32_t row = 0U; row < F360_PSEUDO_MEAS_DIM; row++)
   {
      for (uint32_t col = 0U; col < F360_PSEUDO_MEAS_DIM; col++)
      {
         DOUBLES_EQUAL(expected_meascov[row][col], object_to_keep.meascov[row][col], tolerance);
      }
   }
}

/**
*\purpose  Check if error covariance matrix is updated correcty when both objects to keep are CTCA type
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Update_Error_Covariance_Matrix_After_Merge_When_Both_Objects_CTCA_Type)
{
   /** \precond
    * Set object filter types
    * Set expected errcov_matrix
    **/
   object_to_keep.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_to_kill.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X];
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y];
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H];
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C];
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S];
   expected_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = object_to_keep.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A];

   /** \action
   * Call tested function
   **/
   Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Compare expected errcov matrix with the actual one
   **/
   for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint32_t col = 0U; col < STATE_DIMENSION; col++)
      {
         DOUBLES_EQUAL(expected_errcov[row][col], object_to_keep.errcov[row][col], tolerance);
      }
   }
}

/**
*\purpose  Check if flags are correctly inherited when none of the objects has f_vehicular_trk flag set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Not_Set_Vehicular_Flag_For_Kept_Object_When_None_Of_The_Input_Objects_Has_Vehicular_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.f_vehicular_trk = false;
   object_to_kill.f_vehicular_trk = false;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has not been set
   **/
   CHECK_FALSE(object_to_keep.f_vehicular_trk);
}

/**
*\purpose  Check if flags are correctly inherited when one of the objects has f_vehicular_trk flag set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Vehicular_Flag_For_Kept_Object_When_Input_Kept_Object_Has_Vehicular_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.f_vehicular_trk = true;
   object_to_kill.f_vehicular_trk = false;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK(object_to_keep.f_vehicular_trk);
}

/**
*\purpose  Check if flags are correctly inherited when one of the objects has f_vehicular_trk flag set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Vehicular_Flag_For_Kept_Object_When_Input_Killed_Object_Has_Vehicular_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.f_vehicular_trk = false;
   object_to_kill.f_vehicular_trk = true;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK(object_to_keep.f_vehicular_trk);
}

/**
*\purpose  Check if flags are correctly inherited when both objects has f_vehicular_trk flag set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Vehicular_Flag_For_Kept_Object_When_Both_Input_Objects_Have_Vehicular_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.f_vehicular_trk = true;
   object_to_kill.f_vehicular_trk = true;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK(object_to_keep.f_vehicular_trk);
}

/**
*\purpose  Check if flags are correctly inherited when none of the objects has movable_prob set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Not_Set_Moveable_Flag_For_Kept_Object_When_None_Of_The_Input_Objects_Has_Moveable_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.movable_prob = 0.0F;
   object_to_kill.movable_prob = 0.0F;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has not been set
   **/
   CHECK_TRUE(object_to_keep.movable_prob < 0.5F);
}

/**
*\purpose  Check if flags are correctly inherited when one of the objects has movable_prob set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Moveable_Flag_For_Kept_Object_When_Input_Kept_Object_Has_Moveable_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.movable_prob = 1.0F;
   object_to_kill.movable_prob = 0.0F;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK_TRUE(object_to_keep.movable_prob > 0.5F);
}

/**
*\purpose  Check if flags are correctly inherited when one of the objects has movable_prob set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Moveable_Flag_For_Kept_Object_When_Input_Killed_Object_Has_Moveable_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.movable_prob = 0.0F;
   object_to_kill.movable_prob = 1.0F;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK_TRUE(object_to_keep.movable_prob > 0.5F);
}

/**
*\purpose  Check if flags are correctly inherited when both objects has movable_prob set
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Moveable_Flag_For_Kept_Object_When_Both_Input_Objects_Have_Moveable_Flag_Set)
{
   /** \precond
    * Set flags in both objects
    **/
   object_to_keep.movable_prob = 1.0F;
   object_to_kill.movable_prob = 1.0F;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if flag in merged object has been set
   **/
   CHECK_TRUE(object_to_keep.movable_prob > 0.5F);
}

/**
*\purpose  Check if kept object width is determined correctly when input width below max_width for vehicular track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Below_Max_Width_For_Vehicular_Track)
{
   /** \precond
    * Set input dimensions and object f_vehicular_trk flag
    * Set expected output values
    **/
   dimensions.width = 2.3F;
   dimensions.wid1 = 1.5F;
   dimensions.wid2 = 0.8F;
   object_to_keep.f_vehicular_trk = true;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width equal to max_width for vehicular track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Equal_To_Max_Width_For_Vehicular_Track)
{
   /** \precond
    * Set input dimensions and object f_vehicular_trk flag
   * Set expected output values
    **/
   dimensions.width = 2.5F;
   dimensions.wid1 = 1.5F;
   dimensions.wid2 = 1.0F;
   object_to_keep.f_vehicular_trk = true;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/

   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width greater than max_width for vehicular track
* This test is no longer valid, this needs to be tested in Try_To_Merge_Two_Objects() tests, will be handled in ICV-336
*\req  N/A
*/
IGNORE_TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Max_Value_If_Dimensions_Width_Greater_Than_Max_Width_For_Vehicular_Track)
{
   /** \precond
    * Set input dimensions and object f_vehicular_trk flag
    * Set expected output values
    **/
   dimensions.width = 2.8F;
   dimensions.wid1 = 1.8F;
   dimensions.wid2 = 1.0F;
   object_to_keep.f_vehicular_trk = true;

   const float32_t expected_width = 2.5F;

   /** \action
   * Call tested function
   **/
   Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);
   // Update_Merged_Objects_Properties does not contain Determine_Maximum_Width_And_Adapt anymore. Instead this function has been moved to Try_To_Merge_Two_Objects
   // However, Determine_Maximum_Width_And_Adapt is a static function and cannot be called from here. Also, Try_To_Merge_Two_Objects doesnt output the dimensions
   // of the merged object so we can check.
   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width below max_width for movable track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Below_Max_Width_For_Movable_Track)
{
   /** \precond
    * Set input dimensions and object movable_prob
    * Set expected output values
    **/
   dimensions.width = 1.8F;
   dimensions.wid1 = 1.0F;
   dimensions.wid2 = 0.8F;
   object_to_keep.movable_prob = 1.0F;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width equal to max_width for movable track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Equal_To_Max_Width_For_Movable_Track)
{
   /** \precond
    * Set input dimensions and object movable_prob
    * Set expected output values
    **/
   dimensions.width = 2.0F;
   dimensions.wid1 = 1.5F;
   dimensions.wid2 = 0.5F;
   object_to_keep.movable_prob = 1.0F;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width below max_width for non-movable track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Below_Max_Width_For_Non_Movable_Track)
{
   /** \precond
    * Set input dimensions and objects f_vehicular_trk and movable_prob
    * Set expected output values
    **/
   dimensions.width = 1.8F;
   dimensions.wid1 = 1.0F;
   dimensions.wid2 = 0.8F;
   object_to_keep.movable_prob = 0.0F;
   object_to_keep.f_vehicular_trk = false;
   object_to_kill.movable_prob = 0.0F;
   object_to_kill.f_vehicular_trk = false;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if kept object width is determined correctly when input width equal to max_width for non-movable track
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Should_Set_Width_To_Given_In_Dimensions_Input_If_Value_Equal_To_Max_Width_For_Non_Movable_Track)
{
   /** \precond
    * Set input dimensions and objects f_vehicular_trk and movable_prob
    * Set expected output values
    **/
   dimensions.width = 2.0F;
   dimensions.wid1 = 1.5F;
   dimensions.wid2 = 0.5F;
   object_to_keep.movable_prob = 0.0F;
   object_to_keep.f_vehicular_trk = false;
   object_to_kill.movable_prob = 0.0F;
   object_to_kill.f_vehicular_trk = false;

   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
   Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check if object width is set the same as in input dimensions structure
   **/
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), F360_EPSILON);
}

/**
*\purpose  Check if merged object is flagged as on SEP correctly
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Update_Merged_Objects_Properties__Merged_Object_On_SEP)
{
   /** \precond
    * Set SEP id 2 to valid status
    * Set SEP id 2 interval limits
    **/
   sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[1].lower_limit = -10.0F;
   sep[1].upper_limit = 10.0F;

   /** \action
   * Call tested function
   **/
      Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check that object have been flagged as on SEP id 2 and
   * that is not flagged as behind any SEP
   **/
   CHECK_EQUAL(2U, object_to_keep.on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, object_to_keep.behind_sep_id);
}

/**
*\purpose  Check if merged object is flagged as behind SEP correctly
* This test is no longer valid, this needs to be tested in Try_To_Merge_Two_Objects() tests, will be handled in ICV-336
*\req  N/A
*/
TEST(f360_update_merged_objects_properties, Update_Merged_Objects_Properties__Merged_Object_Behind_SEP)
{
   /** \precond
    * Set SEP id 2 to valid status
    * Set SEP id 2 interval limits
    * Set SEP id 2 as a straight line on right side of host
    * Set object to keep after merge lateral position behind SEP id 2
    * Set object movable_prob to 1.0 as only moveable objects are flagged as behind SEPs
    * Set the center_vcs_position same as object vcs_position, since the default refpoint is center
    **/
   sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[1].lower_limit = -10.0F;
   sep[1].upper_limit = 10.0F;
   sep[1].p0 = 2.0F;
   sep[1].p1 = 0.0F;
   sep[1].p2 = 0.0F;

   object_to_keep.vcs_position.y = sep[1].p0 + 5.0F;
   object_to_keep.movable_prob = 1.0F;
   object_to_keep.bbox.Set_Center(object_to_keep.vcs_position);
   // Calculate_Merged_Obj_Center_And_Update_Width needs to be called before Update_Merged_Objects_Properties
   // to calculate new updated center of the object after merge

   /** \action
   * Call tested function
   **/
   merged_object_center = Calculate_Merged_Obj_Center_And_Update_Width(object_to_keep, dimensions);
   Update_Merged_Objects_Properties(object_to_kill, calibs, sep, host, sensors, globals, merged_object_center, dimensions, object_to_keep);

   /** \result
   * Check that object have been flagged as behind SEP id 2 and
   * that is not flagged as on any SEP
   **/
   CHECK_EQUAL(2U, object_to_keep.behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, object_to_keep.on_sep_id);
}
/** @}*/

/** \defgroup  f360_adjust_obj_states_after_merge
   *  @{
   */

/** \brief
*  This purpose of this test group is for testing the functionality of the function Adjust_Obj_States_After_Merge()
**/
TEST_GROUP(f360_adjust_obj_states_after_merge)
{
   /** \setup
   * Set up default values for input parameters
   **/
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Dimensions_T dimensions = {};
   F360_Object_Track_T object_to_keep = {};
   F360_Calibrations_T calibs = {};
   Point merged_object_center = {};
   const float32_t tolerance = 1e-4F;

   TEST_SETUP()
   {
      // Clear Object Properties
      Clear_Object_Track(object_to_keep);

      // Setup tracker calibrations
      Initialize_Tracker_Calibrations(calibs);

      globals.f_single_front_center_radar_only = false;

      // Set the distance to real axel for host
      host.dist_rear_axle_to_vcs_m = 3.0F;

      // Set values for selected input parameters
      object_to_keep.curvature = 0.2F;
      object_to_keep.speed = 5.0F;
      object_to_keep.vcs_velocity.longitudinal = 4.0F;
      object_to_keep.vcs_velocity.lateral = 3.0F;
      object_to_keep.vcs_position.x = 30.0F;
      object_to_keep.vcs_position.y = 10.0F;
      object_to_keep.reference_point = F360_REFERENCE_POINT_LEFT;

      object_to_keep.Set_Bbox_Orientation(Angle{ 0.1F });
      object_to_keep.bbox.Set_Width(1.8F);

      // Set the object's f_movable flag to true, so that derive_ref_point algorithm returns a new ref point
      object_to_keep.movable_prob = 1.0F;
      object_to_keep.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

      dimensions.len1 = 2.5F;
      dimensions.len2 = 1.5F;
      dimensions.wid1 = 0.4F;
      dimensions.wid2 = 1.4F;
      dimensions.length = 4.0F;
      dimensions.width = 1.8F;

      const float32_t max_width = calibs.k_movable_max_target_width;

      // Update width/wid1/wid2 if previous width is larger than maximum allowed width
      if (dimensions.width > max_width)
      {
         const float32_t factor = 1.0F / dimensions.width;
         dimensions.wid1 = dimensions.wid1 * factor * max_width;
         dimensions.wid2 = dimensions.wid2 * factor * max_width;
         dimensions.width = max_width;
      }

      merged_object_center = Calculate_Merged_Obj_Center_And_Update_Width(object_to_keep, dimensions);

   }

};

/**
*\purpose  Check if object length and width is set to a correct value
*\req  N/A
*/
TEST(f360_adjust_obj_states_after_merge, Should_Set_New_Object_Length_And_Width)
{
   /** \precond
    * Set expected length and width
    **/
   const float32_t expected_length = dimensions.length;
   const float32_t expected_width = dimensions.width;

   /** \action
   * Call tested function
   **/
   Adjust_Obj_States_After_Merge(dimensions, calibs, host, sensors, globals, merged_object_center, object_to_keep);

   /** \result
   * Check if length and width values has been set correctly
   **/
   DOUBLES_EQUAL(expected_length, object_to_keep.bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), tolerance);
}

/**
*\purpose  Check if kept object vcs position is set to a new value
*\req  N/A
*/
TEST(f360_adjust_obj_states_after_merge, Should_Set_New_Object_VCS_Position)
{
   /** \precond
    * Set expected position
    **/
   const float32_t expected_long_position = 27.5524F;
   const float32_t expected_lat_position = 9.3524F;

   /** \action
   * Call tested function
   **/
   Adjust_Obj_States_After_Merge(dimensions, calibs, host, sensors, globals, merged_object_center, object_to_keep);

   /** \result
   * Check if position value has been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, object_to_keep.vcs_position.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, object_to_keep.vcs_position.y, tolerance);
}

/**
*\purpose  Check if kept object vcs position and width is set to a new value. Let object's width be smaller than the merged
*          object's width. Verify that when front or rear is not visible, position and width is based on the merged object's
*          dimension.
*\req  N/A
*/
TEST(f360_adjust_obj_states_after_merge, Should_Set_New_Object_VCS_Position_Smaller_Width_Ref_Point_Left)
{
   /** \precond
    * Set keep object's width to different value from merged object's width
    * Set expected position
    **/
   object_to_keep.bbox.Set_Width(dimensions.width - 0.5F);
   const float32_t expected_long_position = 27.5524F;
   const float32_t expected_lat_position = 9.3524F;

   /** \action
   * Call tested function
   **/
   Adjust_Obj_States_After_Merge(dimensions, calibs, host, sensors, globals, merged_object_center, object_to_keep);

   /** \result
   * Check if position value has been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, object_to_keep.vcs_position.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, object_to_keep.vcs_position.y, tolerance);
}

/**
*\purpose   Check if kept object vcs position and width is set to a new value. Verify that the width of the kept object is used
*           and the updated position is set accordingly.
*\req  N/A
*/
TEST(f360_adjust_obj_states_after_merge, Should_Set_New_Object_VCS_Position_Use_Keep_Objs_Width)
{
   /** \precond
    * Set reference point of keep object to REAR LEFT such that object's width kept
    * Set keep object's width to a smaller value than the merged object's
    * Set expected position
    **/
   object_to_keep.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   object_to_keep.bbox.Set_Width(dimensions.width - 0.5F);
   const float32_t expected_long_position = 27.5774F;
   const float32_t expected_lat_position = 9.1037F;
   const float32_t expected_width = object_to_keep.bbox.Get_Width();


   /** \action
   * Call tested function
   **/
   // Calculate_Merged_Obj_Center_And_Update_Width needs to be called before Update_Merged_Objects_Properties
   // to calculate new updated center of the object after merge due to the change of reference point and width
   merged_object_center = Calculate_Merged_Obj_Center_And_Update_Width(object_to_keep, dimensions);
   Adjust_Obj_States_After_Merge(dimensions, calibs, host, sensors, globals, merged_object_center, object_to_keep);

   /** \result
   * Check if position value has been set correctly
   **/
   DOUBLES_EQUAL(expected_long_position, object_to_keep.vcs_position.x, tolerance);
   DOUBLES_EQUAL(expected_lat_position, object_to_keep.vcs_position.y, tolerance);
   DOUBLES_EQUAL(expected_width, object_to_keep.bbox.Get_Width(), tolerance);
}

/** @}*/
