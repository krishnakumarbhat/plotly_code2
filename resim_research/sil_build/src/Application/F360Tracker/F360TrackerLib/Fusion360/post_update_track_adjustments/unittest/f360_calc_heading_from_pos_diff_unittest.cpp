/** \file
 * This file contains unit tests for content of f360_calc_heading_from_pos_diff.cpp file
 */

#include "f360_calc_heading_from_pos_diff.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_calc_heading_from_pos_diff
 *  @{
 */

/** \brief
 * Test group containing tests to verify that calculation of heading solely 
 * based on position difference between tracker iterations works as intended.
 */
TEST_GROUP(f360_calc_heading_from_pos_diff)
{
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   int32_t obj_id_A = 10;
   int32_t obj_idx_A = obj_id_A - 1;
   int32_t obj_id_B = 20;
   int32_t obj_idx_B = obj_id_B - 1;

   float32_t test_pass_thres = 0.0001F;
   
   /** \setup
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj_id_A;
      tracker_info.active_obj_ids[1] = obj_id_B;

      object_tracks[obj_idx_A].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[obj_idx_A].speed = calibs.k_min_speed_for_updating_heading + 1.0F;
      object_tracks[obj_idx_A].vcs_heading = Angle{ F360_DEG2RAD(10.0F) };
      object_tracks[obj_idx_A].vcs_position.x = 10.0F;
      object_tracks[obj_idx_A].vcs_position.y = 0.0F;
      object_tracks[obj_idx_A].prev_vcs_center_pos.x = 9.0F;
      object_tracks[obj_idx_A].prev_vcs_center_pos.y = 0.0F;
      object_tracks[obj_idx_A].filtered_pos_diff_heading = 0.0F;
      object_tracks[obj_idx_A].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = {10.0F, 0.0F};
      object_tracks[obj_idx_A].bbox.Set_Center(center);

      object_tracks[obj_idx_B].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[obj_idx_B].speed = calibs.k_min_speed_for_updating_heading + 1.0F;
      object_tracks[obj_idx_B].vcs_heading = Angle{ F360_DEG2RAD(-10.0F) };
      object_tracks[obj_idx_B].vcs_position.x = 0.0F;
      object_tracks[obj_idx_B].vcs_position.y = 9.0F;
      object_tracks[obj_idx_B].prev_vcs_center_pos.x = 0.0F;
      object_tracks[obj_idx_B].prev_vcs_center_pos.y = 10.0F;
      object_tracks[obj_idx_B].filtered_pos_diff_heading = -F360_PI_2;
      object_tracks[obj_idx_A].reference_point = F360_REFERENCE_POINT_CENTER;
      center = {0.0F, 9.0F};
      object_tracks[obj_idx_B].bbox.Set_Center(center);

   }
};

/** \purpose  
 * Purpose is to verify that heading from position differences is unchanged
 * when the raw heading measurement is equal to the already filtered heading
 * value.
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff, Calc_Heading_From_Pos_Diff__Raw_Heading_Equal_Filtered_Heading)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    */

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs );
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading haven't changed since raw measurement should be equal
    * to the previous filtered one
    */
   DOUBLES_EQUAL(0.0F, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(-F360_PI_2, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is updated
 * when the raw heading measurement is not equal to the already filtered heading
 * value.
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff, Calc_Heading_From_Pos_Diff__Raw_Heading_Not_Equal_Filtered_Heading)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    * 
    * Modify both objects filtred heading
    */
   object_tracks[obj_idx_A].filtered_pos_diff_heading = -F360_PI_2;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = 0.0F;

    /** \action
     * Call function Calc_Heading_From_Pos_Diff()
     */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading have changed since raw measurement 
    * is not equal to the previous filtered one
    */
   float32_t exp_heading_A = -1.41371691F;
   float32_t exp_heading_B = -0.157079697F;

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is updated
 * with the objects vcs_heading value since this is the first time we update 
 * the filtered heading value.
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff, Calc_Heading_From_Pos_Diff__Filtered_Heading_First_Update)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    *
    * Modify both objects filtred heading to the init value
    */
   object_tracks[obj_idx_A].filtered_pos_diff_heading = INFTY;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = INFTY;

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading have been filled with
    * the objects vcs_heading
    */
   float32_t exp_heading_A = object_tracks[obj_idx_A].vcs_heading.Value();
   float32_t exp_heading_B = object_tracks[obj_idx_B].vcs_heading.Value();

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is not
 * updated since filter object speed is smaller than calibration to freeze object
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff, Calc_Heading_From_Pos_Diff__Object_Speed_Too_Low)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    *
    * Modify both objects speed to be below calibration threshold
    * Modify both objects filtered position diff heading to something arbitrary different from 0 
    * and different from expected heading derived from position change.
    */
   object_tracks[obj_idx_A].speed = calibs.k_min_speed_for_updating_heading - 1.0F;
   object_tracks[obj_idx_B].speed = calibs.k_min_speed_for_updating_heading - 1.0F;
   object_tracks[obj_idx_A].filtered_pos_diff_heading = -F360_PI_2;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = F360_PI_2;

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading haven't been updated
    */
   float32_t exp_heading_A = -F360_PI_2;
   float32_t exp_heading_B = F360_PI_2;

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is not
 * updated since filter type is CCV
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff, Calc_Heading_From_Pos_Diff__Filtered_Heading_CCV_Type)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CTCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CTCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    *
    * Modify both objects tracker filter type to CCV
    * Modify both objects filtred heading to something arbitrary
    */
   object_tracks[obj_idx_A].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_tracks[obj_idx_B].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_tracks[obj_idx_A].filtered_pos_diff_heading = 0.0F;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = 0.0F;

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading haven't been updated
    */
   float32_t exp_heading_A = 0.0F;
   float32_t exp_heading_B = 0.0F;

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}
/** @}*/


/** \brief
 * Test group containing tests to verify that calculation of heading solely 
 * based on position difference between tracker iterations works as intended
 * for CCA objects.
 */
TEST_GROUP(f360_calc_heading_from_pos_diff_CCA)
{
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   int32_t obj_id_A = 10;
   int32_t obj_idx_A = obj_id_A - 1;
   int32_t obj_id_B = 20;
   int32_t obj_idx_B = obj_id_B - 1;

   float32_t test_pass_thres = 0.0001F;
   
   /** \setup
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj_id_A;
      tracker_info.active_obj_ids[1] = obj_id_B;

      object_tracks[obj_idx_A].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[obj_idx_A].speed = calibs.k_min_speed_for_updating_heading + 1.0F;
      object_tracks[obj_idx_A].vcs_heading = Angle{ F360_DEG2RAD(10.0F) };
      object_tracks[obj_idx_A].vcs_position.x = 10.0F;
      object_tracks[obj_idx_A].vcs_position.y = 0.0F;
      object_tracks[obj_idx_A].prev_vcs_center_pos.x = 9.0F;
      object_tracks[obj_idx_A].prev_vcs_center_pos.y = 0.0F;
      object_tracks[obj_idx_A].filtered_pos_diff_heading = 0.0F;
      object_tracks[obj_idx_A].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = {10.0F, 0.0F};
      object_tracks[obj_idx_A].bbox.Set_Center(center);

      object_tracks[obj_idx_B].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[obj_idx_B].speed = calibs.k_min_speed_for_updating_heading + 1.0F;
      object_tracks[obj_idx_B].vcs_heading = Angle{ F360_DEG2RAD(-10.0F) };
      object_tracks[obj_idx_B].vcs_position.x = 0.0F;
      object_tracks[obj_idx_B].vcs_position.y = 9.0F;
      object_tracks[obj_idx_B].prev_vcs_center_pos.x = 0.0F;
      object_tracks[obj_idx_B].prev_vcs_center_pos.y = 10.0F;
      object_tracks[obj_idx_B].filtered_pos_diff_heading = -F360_PI_2;
      object_tracks[obj_idx_A].reference_point = F360_REFERENCE_POINT_CENTER;
      center = {0.0F, 9.0F};
      object_tracks[obj_idx_B].bbox.Set_Center(center);

   }
};

/** \purpose  
 * Purpose is to verify that heading from position differences is unchanged
 * when the raw heading measurement is equal to the already filtered heading
 * value (for a CCA object).
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff_CCA, Calc_Heading_From_Pos_Diff_CCA__Raw_Heading_Equal_Filtered_Heading)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    */

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs );
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading haven't changed since raw measurement should be equal
    * to the previous filtered one
    */
   DOUBLES_EQUAL(0.0F, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(-F360_PI_2, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is updated
 * when the raw heading measurement is not equal to the already filtered heading
 * value (for a CCA object).
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff_CCA, Calc_Heading_From_Pos_Diff_CCA__Raw_Heading_Not_Equal_Filtered_Heading)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    * 
    * Modify both objects filtred heading
    */
   object_tracks[obj_idx_A].filtered_pos_diff_heading = -F360_PI_2;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = 0.0F;

    /** \action
     * Call function Calc_Heading_From_Pos_Diff()
     */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading have changed since raw measurement 
    * is not equal to the previous filtered one
    */
   float32_t exp_heading_A = -1.41371691F;
   float32_t exp_heading_B = -0.157079697F;

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is updated
 * with the objects vcs_heading value since this is the first time we update 
 * the filtered heading value (for a CCA object)..
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff_CCA, Calc_Heading_From_Pos_Diff_CCA__Filtered_Heading_First_Update)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    *
    * Modify both objects filtred heading to the init value
    */
   object_tracks[obj_idx_A].filtered_pos_diff_heading = INFTY;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = INFTY;

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading have been filled with
    * the objects vcs_heading
    */
   float32_t exp_heading_A = object_tracks[obj_idx_A].vcs_heading.Value();
   float32_t exp_heading_B = object_tracks[obj_idx_B].vcs_heading.Value();

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that heading from position differences is not
 * updated since filter object speed is smaller than calibration to freeze object (for a CCA object).
 * \req
 * NA
 */
TEST(f360_calc_heading_from_pos_diff_CCA, Calc_Heading_From_Pos_Diff_CCA__Object_Speed_Too_Low)
{
   /** \precond
    * The following have been set up in test group
    * Add two objects to tracker_info structure
    * Object A
    *    VCS position (10, 0)
    *    Type CCA
    *    Previous VCS position (9, 0)
    *    Filtered position heading 0
    * Object B
    *    VCS position (0, 9)
    *    Type CCA
    *    Previous VCS position (0, 10)
    *    Filtered position heading -pi
    *
    * Modify both objects speed to be below calibration threshold
    * Modify both objects filtered position diff heading to something arbitrary different from 0 
    * and different from expected heading derived from position change.
    */
   object_tracks[obj_idx_A].speed = calibs.k_min_speed_for_updating_heading - 1.0F;
   object_tracks[obj_idx_B].speed = calibs.k_min_speed_for_updating_heading - 1.0F;
   object_tracks[obj_idx_A].filtered_pos_diff_heading = -F360_PI_2;
   object_tracks[obj_idx_B].filtered_pos_diff_heading = F360_PI_2;

   /** \action
    * Call function Calc_Heading_From_Pos_Diff()
    */
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_A], calibs);
   Calc_Heading_From_Pos_Diff(object_tracks[obj_idx_B], calibs);

   /** \result
    * Verify that filtered heading haven't been updated
    */
   float32_t exp_heading_A = -F360_PI_2;
   float32_t exp_heading_B = F360_PI_2;

   DOUBLES_EQUAL(exp_heading_A, object_tracks[obj_idx_A].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(exp_heading_B, object_tracks[obj_idx_B].filtered_pos_diff_heading, test_pass_thres);
}
/** @}*/
