/** \file
 * This file contains unit tests for content of f360_update_longi_stat_curves.cpp file
 */

#include "f360_update_longi_stat_curves.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

#include "f360_lsc_data_generator.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup f360_update_longi_stat_curves_Arrange_First_Iteration
 *  @{
 */

/** \brief
 * This test group defines a set of objects to be used to test function Arrange_First_Iteration().
 */
TEST_GROUP(f360_update_longi_stat_curves_Arrange_First_Iteration)
{
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Calibrations_T calibs;
   uint16_t nr_next_ids_of_interest;
   uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   F360_Host_T host;

   bool f_is_data_remaining;

   F360_LSC_Object_Group_Settings_T group_A;
   
   /** \setup
    * Initialize default calibrations and create a set of objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      Initialize_Tracker_Info(tracker_info);
      host.curvature_rear = 0.0F;
      group_A = Add_LSC_Group_A(tracker_info, objects);
   }

};

/** \purpose  
 * Purpose is to verify that all objects have been correctly selected as they are within
 * the valid interval in longitudinal direction for relevant objects. We also verify
 * that the function returns true which indicates that there are enough relevant object to
 * run LSC algorithm.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Arrange_First_Iteration, Arrange_First_Iteration_All_Objects_Relevant)
{
   /** \precond
    * Modify interval in calibration to guarantee all objects are within relevant interval
    */
   uint16_t first_idx = group_A.ids[0] - 1U;
   calibs.k_lsc_min_long_pos = objects[first_idx].vcs_position.x - 1.0F;
   uint16_t last_index = group_A.ids[group_A.nr_objects - 1U] - 1U;
   calibs.k_lsc_max_long_pos = objects[last_index].vcs_position.x + 1.0F;

   /** \action
    * Call function
    */
   f_is_data_remaining = Arrange_First_Iteration(tracker_info, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify all objects have been selected and that function has returned true
    */
   CHECK_TRUE(f_is_data_remaining);
   CHECK_EQUAL(group_A.nr_objects, nr_next_ids_of_interest);
   for (uint16_t i = 0U; i < nr_next_ids_of_interest; i++)
   {
      CHECK_EQUAL(group_A.ids[i], next_ids_of_interest[i]);
   }
}

/** \purpose
 * Purpose is to verify that all objects except for the first and the last one have been correctly selected as they are within
 * the valid interval in longitudinal direction for relevant objects. The first one should not be selected as it is CTCA and the last one 
 * should not be selected as it is moveable. 
 * We also verify that the function returns true which indicates that there are enough relevant object to run LSC algorithm.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Arrange_First_Iteration, Arrange_First_Iteration_CTCA_and_Moveable_Objects_Excluded)
{
   /** \precond
    * Modify interval in calibration to guarantee all objects are within relevant interval
    * Set filter type of first object to CTCA
    * Set last object to moveable
    */
   uint16_t first_idx = group_A.ids[0] - 1U;
   calibs.k_lsc_min_long_pos = objects[first_idx].vcs_position.x - 1.0F;
   uint16_t last_index = group_A.ids[group_A.nr_objects - 1U] - 1U;
   calibs.k_lsc_max_long_pos = objects[last_index].vcs_position.x + 1.0F;
   
   objects[first_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   objects[last_index].movable_prob = 1.0F;

   /** \action
    * Call function
    */
   f_is_data_remaining = Arrange_First_Iteration(tracker_info, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify all objects have been selected and that function has returned true
    */
   CHECK_TRUE(f_is_data_remaining);
   CHECK_EQUAL(group_A.nr_objects - 2U, nr_next_ids_of_interest);
   for (uint16_t i = 1U; i < nr_next_ids_of_interest - 1; i++)
   {
      CHECK_EQUAL(group_A.ids[i], next_ids_of_interest[i-1U]);
   }
}

/** \purpose
 * Purpose is to verify that all objects except the first and last have been correctly selected as they are not
 * within the valid interval in longitudinal direction for relevant objects. We also verify that the function 
 * returns true which indicates that there are enough relevant object to run LSC algorithm.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Arrange_First_Iteration, Arrange_First_Iteration_First_And_Last_Object_Excluded)
{
   /** \precond
    * Modify interval in calibration so that first and last object are outside valid interval
    */
   uint16_t first_idx = group_A.ids[0] - 1U;
   calibs.k_lsc_min_long_pos = objects[first_idx].vcs_position.x + 1.0F;
   uint16_t last_index = group_A.ids[group_A.nr_objects - 1U] - 1U;
   calibs.k_lsc_max_long_pos = objects[last_index].vcs_position.x - 1.0F;

   /** \action
    * Call function
    */
   f_is_data_remaining = Arrange_First_Iteration(tracker_info, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify that correct objects have been selected and that function has returned true
    */
   CHECK_TRUE(f_is_data_remaining);
   CHECK_EQUAL(group_A.nr_objects - 2U, nr_next_ids_of_interest);
   for (uint16_t i = 0U; i < nr_next_ids_of_interest; i++)
   {
      CHECK_EQUAL(group_A.ids[i + 1U], next_ids_of_interest[i]);
   }
}

/** \purpose
 * Purpose is to verify that only one object is selected when only one object is within valid interval.
 * One object is not enough to fit a second order polynomial so we also expect that the function returns false
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Arrange_First_Iteration, Arrange_First_Iteration_One_Relevant_Object)
{
   /** \precond
    * Modify interval in calibration so that only one object is in valid interval
    */
   uint16_t expected_id = 10U;

   uint16_t object_idx = group_A.ids[expected_id - 1U] - 1U;
   calibs.k_lsc_min_long_pos = objects[object_idx].vcs_position.x - 1.0F;
   calibs.k_lsc_max_long_pos = objects[object_idx].vcs_position.x + 1.0F;

   /** \action
    * Call function
    */
   f_is_data_remaining = Arrange_First_Iteration(tracker_info, calibs, nr_next_ids_of_interest, next_ids_of_interest);

   /** \result
    * Verify that correct object have been selected and that function has returned false
    */
   CHECK_FALSE(f_is_data_remaining);
   CHECK_EQUAL(1U, nr_next_ids_of_interest);
   CHECK_EQUAL(expected_id, next_ids_of_interest[0]);
}

/** @}*/

/** \defgroup  f360_update_longi_stat_curves_Sanity_Check_And_Populate_LSC_Output
 *  @{
 */

 /** \brief
  * This test group defines a set of new longi stat curves to test functionality
  * of function Sanity_Check_And_Populate_LSC_Output()
  */
TEST_GROUP(f360_update_longi_stat_curves_Sanity_Check_And_Populate_LSC_Output)
{
   F360_Calibrations_T calibs;
   uint16_t nr_downselected_clusters;
   F360_Longi_Stat_Curve_T new_longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];
   F360_Longi_Stat_Curve_T longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];

   /** \setup
    * Initialize default calibrations and create a set of new longi stat curves.
    * The "a" coefficient of the second curve is too large for it to be valid
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      nr_downselected_clusters = 3U;

      new_longi_stat_curves[0].f_valid = true;
      new_longi_stat_curves[0].x_min = -25.0F;
      new_longi_stat_curves[0].x_max = -15.0F;
      new_longi_stat_curves[0].a = 0.5F * calibs.k_lsc_max_a_coeff;
      new_longi_stat_curves[0].b = 0.1F;
      new_longi_stat_curves[0].c = 1.0F;
      new_longi_stat_curves[0].mean_lat_pos = -25.0F;

      new_longi_stat_curves[1].f_valid = true;
      new_longi_stat_curves[1].x_min = -25.0F;
      new_longi_stat_curves[1].x_max = -15.0F;
      new_longi_stat_curves[1].a = 1.5F * calibs.k_lsc_max_a_coeff;
      new_longi_stat_curves[1].b = 0.1F;
      new_longi_stat_curves[1].c = 1.0F;
      new_longi_stat_curves[1].mean_lat_pos = -25.0F;

      new_longi_stat_curves[2].f_valid = true;
      new_longi_stat_curves[2].x_min = -25.0F;
      new_longi_stat_curves[2].x_max = -15.0F;
      new_longi_stat_curves[2].a = 0.0F * calibs.k_lsc_max_a_coeff;
      new_longi_stat_curves[2].b = 0.5F;
      new_longi_stat_curves[2].c = -1.0F;
      new_longi_stat_curves[2].mean_lat_pos = -25.0F;
   }

};

/** \purpose
 * Purpose is to verify that curve 0 and 2 have passed the sanity check and is populated
 * to the LSC output structure.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Sanity_Check_And_Populate_LSC_Output, Sanity_Check_And_Populate_LSC_Output_Verify_LSC_Sanity_Check)
{

   /** \action
    * Call function
    */
   Sanity_Check_And_Populate_LSC_Output(calibs, nr_downselected_clusters, new_longi_stat_curves, longi_stat_curves);

   /** \result
    * Verify that curve 0 and 2 are valid and that curve 1 is invalid.
    * Also check that the LSC output have been filled correctly
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   CHECK_TRUE(longi_stat_curves[2].f_valid);
   CHECK_FALSE(longi_stat_curves[1].f_valid);

   for (uint32_t i = 0U; i < nr_downselected_clusters; i++)
   {
      DOUBLES_EQUAL(new_longi_stat_curves[i].x_min, longi_stat_curves[i].x_min, F360_EPSILON);
      DOUBLES_EQUAL(new_longi_stat_curves[i].x_max, longi_stat_curves[i].x_max, F360_EPSILON);
      DOUBLES_EQUAL(new_longi_stat_curves[i].a, longi_stat_curves[i].a, F360_EPSILON);
      DOUBLES_EQUAL(new_longi_stat_curves[i].b, longi_stat_curves[i].b, F360_EPSILON);
      DOUBLES_EQUAL(new_longi_stat_curves[i].c, longi_stat_curves[i].c, F360_EPSILON);
      DOUBLES_EQUAL(new_longi_stat_curves[i].mean_lat_pos, longi_stat_curves[i].mean_lat_pos, F360_EPSILON);
   }
   

}
/** @}*/

/** \defgroup  f360_update_longi_stat_curves_Fit_Second_Order_Polynomials_To_Clusters
 *  @{
 */

 /** \brief
  * This test group defines two LSC clusters. These clusters are then passed to the
  * function Fit_Second_Order_Polynomials_To_Clusters() to verify that fitted curves are populated correctly.
  * Note that this is not a test for the actual polynomial fit which is tested separately but rather a test
  * to verify that the population from the polynomial fit is assigned correctly. The objects are thus,
  * for simplicity placed on a line in longitudinal order and should yield a polynomial where both a and b
  * coefficients are 0.
  */
TEST_GROUP(f360_update_longi_stat_curves_Fit_Second_Order_Polynomials_To_Clusters)
{
   F360_Tracker_Info_T tracker_info;

   uint16_t nr_valid_clusters;
   F360_Longi_Stat_Cluster_T valid_clusters[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Longi_Stat_Curve_T longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];

   F360_LSC_Object_Group_Settings_T group_A;
   F360_LSC_Object_Group_Settings_T group_B;

   // These variables are used to derive the expected lateral mean position of the curve.
   // Since objects are placed on a line in longitudinal order we also expect the c coefficent
   // to be equal to the lateral mean
   float32_t exp_lat_mean_A = 0.0F;
   float32_t exp_lat_mean_B = 0.0F;

   float32_t test_pass_thres = 1.0e-5;

   /** \setup
    * Create two groups of objects and assign each of them to a cluster.
    * Also derive a clusters lateral mean
    */
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
      Initialize_Tracker_Info(tracker_info);

      nr_valid_clusters = 2U;

      group_A = Add_LSC_Group_A(tracker_info, objects);
      
      valid_clusters[0].nr_objects = group_A.nr_objects;
      uint32_t first_idx = group_A.ids[0] - 1U;
      valid_clusters[0].first_object = &objects[first_idx];
      uint32_t last_idx = group_A.ids[group_A.nr_objects - 1U] - 1U;
      valid_clusters[0].last_object = &objects[last_idx];

      uint32_t second_index = group_A.ids[1] - 1U;
      objects[first_idx].lsc_next_in_cluster = &objects[second_index];
      objects[first_idx].lsc_prev_in_cluster = NULL;
      exp_lat_mean_A += objects[first_idx].vcs_position.y;
      for (uint32_t i = 1U; i < (group_A.nr_objects - 1U); i++)
      {
         uint32_t curr_obj_idx = group_A.ids[i] - 1;
         objects[curr_obj_idx].lsc_next_in_cluster = &objects[group_A.ids[i + 1U] - 1U];
         objects[curr_obj_idx].lsc_prev_in_cluster = &objects[group_A.ids[i - 1U] - 1U];
         exp_lat_mean_A += objects[group_A.ids[i] - 1U].vcs_position.y;
      }
      objects[last_idx].lsc_next_in_cluster = NULL;
      uint32_t second_last_index = group_A.ids[group_A.nr_objects - 2U] - 1U;
      objects[last_idx].lsc_prev_in_cluster = &objects[second_last_index];
      exp_lat_mean_A += objects[last_idx].vcs_position.y;

      exp_lat_mean_A /= static_cast<float32_t>(group_A.nr_objects);
      valid_clusters[0].lat_mean = exp_lat_mean_A;


      group_B = Add_LSC_Group_B(tracker_info, objects);

      valid_clusters[1].nr_objects = group_B.nr_objects;
      first_idx = group_B.ids[0] - 1U;
      valid_clusters[1].first_object = &objects[first_idx];
      last_idx = group_B.ids[group_B.nr_objects - 1U] - 1U;
      valid_clusters[1].last_object = &objects[last_idx];

      second_index = group_B.ids[1] - 1U;
      objects[first_idx].lsc_next_in_cluster = &objects[second_index];
      objects[first_idx].lsc_prev_in_cluster = NULL;
      exp_lat_mean_B += objects[first_idx].vcs_position.y;
      for (uint32_t i = 1U; i < (group_B.nr_objects - 1U); i++)
      {
         uint32_t curr_obj_idx = group_B.ids[i] - 1;
         objects[curr_obj_idx].lsc_next_in_cluster = &objects[group_B.ids[i + 1U] - 1U];
         objects[curr_obj_idx].lsc_prev_in_cluster = &objects[group_B.ids[i - 1U] - 1U];
         exp_lat_mean_B += objects[group_B.ids[i] - 1U].vcs_position.y;
      }
      objects[last_idx].lsc_next_in_cluster = NULL;
      second_last_index = group_B.ids[group_B.nr_objects - 2U] - 1U;
      objects[last_idx].lsc_prev_in_cluster = &objects[second_last_index];
      exp_lat_mean_B += objects[last_idx].vcs_position.y;

      exp_lat_mean_B /= static_cast<float32_t>(group_B.nr_objects);
      valid_clusters[1].lat_mean = exp_lat_mean_B;
   }

};

/** \purpose
 * Purpose is to verify that both clusters have generated a curve and inherited the clusters properties.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Fit_Second_Order_Polynomials_To_Clusters, Fit_Second_Order_Polynomials_To_Clusters_Verify_Curves_Have_Inherited_Cluster_Props)
{

   /** \action
    * Call function
    */
   Fit_Second_Order_Polynomials_To_Clusters(nr_valid_clusters, valid_clusters, longi_stat_curves);

   /** \result
    * Verify that curve 0 and 1 are valid 
    * Also check that the LSC output have been filled correctly
    * Note that c coefficient is expected to be equal to the lateral mean since a and b coefficient should be 0.
    */
   for (uint16_t i = 0U; i < nr_valid_clusters; i++)
   {
      CHECK_TRUE(longi_stat_curves[i].f_valid);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].a, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].b, test_pass_thres);
   }
   
   DOUBLES_EQUAL(valid_clusters[0].first_object->vcs_position.x, longi_stat_curves[0].x_min, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[0].last_object->vcs_position.x, longi_stat_curves[0].x_max, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[0].lat_mean, longi_stat_curves[0].c, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[0].lat_mean, longi_stat_curves[0].mean_lat_pos, test_pass_thres);

   DOUBLES_EQUAL(valid_clusters[1].first_object->vcs_position.x, longi_stat_curves[1].x_min, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[1].last_object->vcs_position.x, longi_stat_curves[1].x_max, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[1].lat_mean, longi_stat_curves[1].c, test_pass_thres);
   DOUBLES_EQUAL(valid_clusters[1].lat_mean, longi_stat_curves[1].mean_lat_pos, test_pass_thres);
}

/** \purpose
 * Purpose of this is to check if when f_poly_fit_ok flag is false, then lsc properties are cleared to 0 and lsc_valid set to false.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Fit_Second_Order_Polynomials_To_Clusters, Fit_Second_Order_Polynomials_To_Clusters_Poly_Fit_Flag_Not_Ok)
{

   /** \action
    * Set f_poly_fit_ok to false by setting nr_objects to less than LSC_NR_POLY_COEFF_SLOTS (3),
    * which causes the matrix A to be rank deficient and thus causes the polynomial fit to fail.
    * Then call the function.
    */
   valid_clusters[0].nr_objects = 2U;
   valid_clusters[1].nr_objects = 2U;

   Fit_Second_Order_Polynomials_To_Clusters(nr_valid_clusters, valid_clusters, longi_stat_curves);

   /** \result
    * Verify that curve 0 and 1 are invalid since the polynomial fit failed.
    * Also check that the LSC output have been cleared to 0 and f_valid set to false.
    */
   for (uint16_t i = 0U; i < nr_valid_clusters; i++)
   {
      CHECK_FALSE(longi_stat_curves[i].f_valid);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].a, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].b, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].c, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].x_min, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].x_max, test_pass_thres);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].mean_lat_pos, test_pass_thres);
   }
}
/** @}*/

/** \defgroup  f360_update_longi_stat_curves_Update_Longi_Stat_Curves
 *  @{
 */

 /** \brief
  * This test group defines 4 groups of objects.
  * Objects are created on 4 lines in longitudinal direction with various lateral position.
  * These groups of objects are then checked that they have been used to create LSC's as expected.
  */
TEST_GROUP(f360_update_longi_stat_curves_Update_Longi_Stat_Curves)
{
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Longi_Stat_Curve_T longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];
   F360_Calibrations_T calibs;
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Host_T host;
   F360_Host_Props_T host_props = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detections = {};
   F360_Globals_T globals = {};

   F360_LSC_Object_Group_Settings_T group_A;
   F360_LSC_Object_Group_Settings_T group_B;
   F360_LSC_Object_Group_Settings_T group_C;
   F360_LSC_Object_Group_Settings_T group_D;
   F360_LSC_Object_Group_Settings_T group_E;

   // These variables are used to derive the expected lateral mean position of the curves.
   // Since objects are placed on a line in longitudinal order we also expect the c coefficent
   // to be equal to the lateral mean
   float32_t exp_lat_mean_A = 0.0F;
   float32_t exp_lat_mean_B = 0.0F;

   float32_t test_pass_thres = 1.0e-5;

   /** \setup
    * Create 4 groups of objects and assign each of them to a cluster.
    * Also derive a clusters lateral mean
    * Initialize globals with front sensor configuration (to avoid Extend_LSC_With_Detections being called)
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      Initialize_Tracker_Info(tracker_info);

      host.curvature_rear = 0.0F;
      host.speed = 3.1F; // Default speed above 3.0F threshold
      group_A = Add_LSC_Group_A(tracker_info, objects);
      group_B = Add_LSC_Group_B(tracker_info, objects);
      group_C = Add_LSC_Group_C(tracker_info, objects);
      group_D = Add_LSC_Group_D(tracker_info, objects);
      group_E = Add_LSC_Group_E(tracker_info, objects); // To form LSC behind host for rear only sensor LSC extension logic check

      exp_lat_mean_A = objects[group_A.ids[0] - 1U].vcs_position.y;
      exp_lat_mean_B = objects[group_B.ids[0] - 1U].vcs_position.y;

      // Initialize globals with front sensor available to prevent Extend_LSC_With_Detections from being called
      globals = F360_Globals_T{};
      globals.f_front_or_front_corner_sensor_available = true;
      globals.f_rear_sensor_available = false;

      // Initialize raw detections
      raw_detections = rspp_variant_A::RSPP_Detection_List_T{};
      raw_detections.number_of_valid_detections = 0U;
      raw_detections.vcslong_det_idx_min = F360_INVALID_ID;
      raw_detections.vcslong_det_idx_max = F360_INVALID_ID;

      // Initialize all reference indices to invalid
      for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
      {
         raw_detections.vcslong_sorted_ref_det_idx[i] = F360_INVALID_ID;
      }
   }

   /** \brief Helper function to add a detection to the list */
   void Add_Detection(uint32_t det_idx, float32_t vcs_x, float32_t vcs_y, bool f_ok_to_use,
                      rspp_variant_A::RSPP_Detection_Motion_Status_T motion_status, int32_t next_sorted_idx)
   {
      raw_detections.detections[det_idx].processed.vcs_position_x = vcs_x;
      raw_detections.detections[det_idx].processed.vcs_position_y = vcs_y;
      raw_detections.detections[det_idx].processed.f_ok_to_use = f_ok_to_use;
      raw_detections.detections[det_idx].processed.motion_status = motion_status;
      raw_detections.detections[det_idx].processed.next_sorted_idx = next_sorted_idx;
      raw_detections.number_of_valid_detections++;
   }

};

/** \purpose
 * Purpose is to verify that group A and B have generated two LSC's as expected.
 * Also verify that curve 2 and 3 are invalid.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Verify_LSC_Created_Group_A_And_B)
{

   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that curve 0 is valid and that properties of the curve is as expected from group A
    * Verify that curve 1 is valid and that properties of the curve is as expected from group B
    * Verify that curve 2 and 3 are invalid with default properties
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   uint32_t first_obj_idx_A = group_A.ids[0] - 1U;
   uint32_t last_obj_idx_A = group_A.ids[group_A.nr_objects - 1U] - 1U;
   DOUBLES_EQUAL(objects[first_obj_idx_A].vcs_position.x, longi_stat_curves[0].x_min, test_pass_thres);
   DOUBLES_EQUAL(objects[last_obj_idx_A].vcs_position.x, longi_stat_curves[0].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].b, test_pass_thres);
   DOUBLES_EQUAL(exp_lat_mean_A, longi_stat_curves[0].c, test_pass_thres);
   DOUBLES_EQUAL(exp_lat_mean_A, longi_stat_curves[0].mean_lat_pos, test_pass_thres);

   CHECK_TRUE(longi_stat_curves[1].f_valid);
   uint32_t first_obj_idx_B = group_B.ids[0] - 1U;
   uint32_t last_obj_idx_B = group_B.ids[group_B.nr_objects - 1U] - 1U;
   DOUBLES_EQUAL(objects[first_obj_idx_B].vcs_position.x, longi_stat_curves[1].x_min, test_pass_thres);
   DOUBLES_EQUAL(objects[last_obj_idx_B].vcs_position.x, longi_stat_curves[1].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].b, test_pass_thres);
   DOUBLES_EQUAL(exp_lat_mean_B, longi_stat_curves[1].c, test_pass_thres);
   DOUBLES_EQUAL(exp_lat_mean_B, longi_stat_curves[1].mean_lat_pos, test_pass_thres);

   CHECK_FALSE(longi_stat_curves[2].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[2].mean_lat_pos, test_pass_thres);

   CHECK_FALSE(longi_stat_curves[3].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[3].mean_lat_pos, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that group A have generated LSC and that it is extended by history.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Verify_LSC_Created_Group_A_And_B_highway)
{

   tracker_info.f_highway_suspected = true;
   host_props.delta_position_x = 5.0F;
   /** \action
    * Call function twice
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that curve 0 is valid and that properties of the curve is as expected from group A
    * Verify that curve 1 is valid and that properties of the curve is as expected from group B
    * Verify that curve 2 and 3 are invalid with default properties
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   uint32_t first_obj_idx_A = group_A.ids[0] - 1U;
   DOUBLES_EQUAL(objects[first_obj_idx_A].vcs_position.x - 5.0F, longi_stat_curves[0].x_min, test_pass_thres);
   
}


/** \purpose
 * Purpose is to verify that no LSCs are generated when all tracked objects are moveable
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Verify_No_LSCs_Are_Created)
{
   /** \precond
    * Set moveable flag for all active objects to true
    */
   for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_objs); i++)
   {
      objects[i].movable_prob = 1.0F;
   }
   
   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that curve 0 is valid and that properties of the curve is as expected from group A
    * Verify that curve 1 is valid and that properties of the curve is as expected from group B
    * Verify that curve 2 and 3 are invalid with default properties
    */
   CHECK_FALSE(longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[0].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[0].mean_lat_pos, test_pass_thres);

   CHECK_FALSE(longi_stat_curves[1].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[1].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[1].mean_lat_pos, test_pass_thres);

   CHECK_FALSE(longi_stat_curves[2].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[2].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[2].mean_lat_pos, test_pass_thres);

   CHECK_FALSE(longi_stat_curves[3].f_valid);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].x_min, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].x_max, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].a, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].b, test_pass_thres);
   DOUBLES_EQUAL(0.0F, longi_stat_curves[3].c, test_pass_thres);
   DOUBLES_EQUAL(INFTY, longi_stat_curves[3].mean_lat_pos, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that Extend_LSC_With_Detections is called and LSC x_max is extended
 * when only rear sensors are present and host speed is above 3.0 m/s.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Only_Rear_Sensors_High_Speed_Extends_LSC)
{
   /** \precond
    * Set up globals with only rear sensors available
    * Set host speed above 3.0 m/s threshold [Already set to 3.1m/s in test setup]
    * Modify object positions for Group D to set LSC range to x_min=-30, x_max=-14
    * Make all other objects movable so as to not create any other LSCs
    * Add detections that would extend the LSC
    */
   globals.f_rear_sensor_available = true;
   globals.f_front_or_front_corner_sensor_available = false;

   // Set all objects in group A and B to be moving so that only Group E creates LSC (This LSC fulfills the position criteria for extension)
   for (uint32_t i = 0; i < group_A.nr_objects; ++i) {
      objects[group_A.ids[i] - 1U].movable_prob = 1.0F;
   }
   for (uint32_t i = 0; i < group_B.nr_objects; ++i) {
      objects[group_B.ids[i] - 1U].movable_prob = 1.0F;
   }

   // Get x_max for Group D objects
   uint32_t last_obj_idx = group_E.ids[group_E.nr_objects - 1U] - 1U;
   const float32_t original_x_max = objects[last_obj_idx].vcs_position.x;

   // Get lateral mean position of Group D objects
   float32_t exp_lat_mean_E = objects[group_E.ids[0] - 1U].vcs_position.y;

   // Add detections to extend LSC
   raw_detections.vcslong_sorted_ref_det_idx[0] = 0U;
   raw_detections.vcslong_sorted_ref_det_idx[1] = 1U;  // Reference to second detection
   Add_Detection(0U, original_x_max + 2.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, 1);
   Add_Detection(1U, original_x_max + 3.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, F360_INVALID_ID);

   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that LSC is created and x_max is extended by detection
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(original_x_max + 3.0F, longi_stat_curves[0].x_max, test_pass_thres); // Expected: -13.0F
}

/** \purpose
 * Purpose is to verify that Extend_LSC_With_Detections is NOT called and LSC x_max is NOT extended
 * when only rear sensors are present but host speed is at or below 3.0 m/s.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Only_Rear_Sensors_Low_Speed_No_Extension)
{
   /** \precond
    * Set up globals with only rear sensors available
    * Set host speed at 3.0 m/s threshold (should NOT trigger extension)
    * Add detections that would extend the LSC if extension was triggered
    */
   globals.f_rear_sensor_available = true;
   globals.f_front_or_front_corner_sensor_available = false;
   host.speed = 3.0F;

   // Set all objects in group A and B to be moving so that only Group E creates LSC (This LSC fulfills the position criteria for extension)
   for (uint32_t i = 0; i < group_A.nr_objects; ++i) {
      objects[group_A.ids[i] - 1U].movable_prob = 1.0F;
   }
   for (uint32_t i = 0; i < group_B.nr_objects; ++i) {
      objects[group_B.ids[i] - 1U].movable_prob = 1.0F;
   }

   // Get x_max for Group D objects
   uint32_t last_obj_idx = group_E.ids[group_E.nr_objects - 1U] - 1U;
   const float32_t original_x_max = objects[last_obj_idx].vcs_position.x;

   // Get lateral mean position of Group D objects
   float32_t exp_lat_mean_E = objects[group_E.ids[0] - 1U].vcs_position.y;

   // Add detections to extend LSC
   raw_detections.vcslong_sorted_ref_det_idx[0] = 0U;
   raw_detections.vcslong_sorted_ref_det_idx[1] = 1U;  // Reference to second detection
   Add_Detection(0U, original_x_max + 2.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, 1);
   Add_Detection(1U, original_x_max + 3.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, F360_INVALID_ID);

   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that LSC is created but x_max is NOT extended (speed too low)
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that Extend_LSC_With_Detections is NOT called and LSC x_max is NOT extended
 * when front sensors are available, regardless of host speed.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_Front_Sensors_Available_No_Extension)
{
   /** \precond
    * Set up globals with front sensors available (even if rear is also available)
    * Set host speed above 3.0 m/s threshold [Already set to 3.1m/s in test setup]
    * Add detections that would extend the LSC if extension was triggered
    */
   globals.f_rear_sensor_available = true;
   globals.f_front_or_front_corner_sensor_available = true;

   // Set all objects in group A and B to be moving so that only Group E creates LSC (This LSC fulfills the position criteria for extension)
   for (uint32_t i = 0; i < group_A.nr_objects; ++i) {
      objects[group_A.ids[i] - 1U].movable_prob = 1.0F;
   }
   for (uint32_t i = 0; i < group_B.nr_objects; ++i) {
      objects[group_B.ids[i] - 1U].movable_prob = 1.0F;
   }

   // Get x_max for Group D objects
   uint32_t last_obj_idx = group_E.ids[group_E.nr_objects - 1U] - 1U;
   const float32_t original_x_max = objects[last_obj_idx].vcs_position.x;

   // Get lateral mean position of Group D objects
   float32_t exp_lat_mean_E = objects[group_E.ids[0] - 1U].vcs_position.y;

   // Add detections to extend LSC
   raw_detections.vcslong_sorted_ref_det_idx[0] = 0U;
   raw_detections.vcslong_sorted_ref_det_idx[1] = 1U;  // Reference to second detection
   Add_Detection(0U, original_x_max + 2.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, 1);
   Add_Detection(1U, original_x_max + 3.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, F360_INVALID_ID);

   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that LSC is created but x_max is NOT extended (front sensors available)
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that Extend_LSC_With_Detections is NOT called and LSC x_max is NOT extended
 * when no rear sensor is available, regardless of host speed.
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Update_Longi_Stat_Curves, Update_Longi_Stat_Curves_No_Rear_Sensor_No_Extension)
{
   /** \precond
    * Set up globals with no rear sensor available
    * Set host speed above 3.0 m/s threshold [Already set to 3.1m/s in test setup]
    * Add detections that would extend the LSC if extension was triggered
    */
   globals.f_rear_sensor_available = false;
   globals.f_front_or_front_corner_sensor_available = false;

   // Set all objects in group A and B to be moving so that only Group E creates LSC (This LSC fulfills the position criteria for extension)
   for (uint32_t i = 0; i < group_A.nr_objects; ++i) {
      objects[group_A.ids[i] - 1U].movable_prob = 1.0F;
   }
   for (uint32_t i = 0; i < group_B.nr_objects; ++i) {
      objects[group_B.ids[i] - 1U].movable_prob = 1.0F;
   }

   // Get x_max for Group D objects
   uint32_t last_obj_idx = group_E.ids[group_E.nr_objects - 1U] - 1U;
   const float32_t original_x_max = objects[last_obj_idx].vcs_position.x;

   // Get lateral mean position of Group D objects
   float32_t exp_lat_mean_E = objects[group_E.ids[0] - 1U].vcs_position.y;

   // Add detections to extend LSC
   raw_detections.vcslong_sorted_ref_det_idx[0] = 0U;
   raw_detections.vcslong_sorted_ref_det_idx[1] = 1U;  // Reference to second detection
   Add_Detection(0U, original_x_max + 2.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, 1);
   Add_Detection(1U, original_x_max + 3.0F, exp_lat_mean_E, true, rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, F360_INVALID_ID);

   /** \action
    * Call function
    */
   Update_Longi_Stat_Curves(tracker_info, calibs, host, host_props, raw_detections, globals, objects, longi_stat_curves, static_env_polys, timing_info);

   /** \result
    * Verify that LSC is created but x_max is NOT extended (no rear sensor)
    */
   CHECK_TRUE(longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}
/** @}*/

/** \defgroup f360_update_longi_stat_curves_Arrange_First_Iteration_Track_In_Host_Path
 *  @{
 */

/** \brief
 * This test group defines a set of objects to be used to test function Arrange_First_Iteration_Track_In_Host_Path().
 */
TEST_GROUP(f360_update_longi_stat_curves_Arrange_First_Iteration_Track_In_Host_Path)
{
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Calibrations_T calibs;
   uint16_t nr_next_ids_of_interest;
   uint16_t next_ids_of_interest[NUMBER_OF_OBJECT_TRACKS];
   F360_Host_T host;

   bool f_is_data_remaining;

   F360_LSC_Object_Group_Settings_T group_A_modified;

   /** \setup
    * Initialize default calibrations and create a set of objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      Initialize_Tracker_Info(tracker_info);
      host.curvature_rear = 0.0F;
      group_A_modified = Add_LSC_Group_A_modified(tracker_info, objects);
   }

};
/** @}*/

/** \defgroup  f360_update_longi_stat_curves_Old_LSC
 *  @{
 */
/** \brief
 * This test group checks if the logic of extending LSC based on history works correctly.
 */
TEST_GROUP(f360_update_longi_stat_curves_Old_LSC)
{
   
   /**
    * Initialize array of new and old LSC
    * Initialize host_props
    */
   F360_Longi_Stat_Curve_T old_longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};
   F360_Longi_Stat_Curve_T new_longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};
   F360_Host_Props_T host_props = {};

   /** \setup
    * host moved 5m in logidutinal direction from last scan
    */
   TEST_SETUP()
   {
      host_props.delta_position_x = 5.0;
   }
};

/** \purpose
 * Purpose is to verify that the function Remember_Old_LSC works as designed - under certain conditions it translates 
 * old LSC by host's longitudinal movement and saves them 
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Old_LSC, Remember_Old_LSC_)
{
   /** \precond
    * Set up four valid LSCs
    * Each of them will start at -20.0F and end at 20.0F.
    * First one is a straight line
    * Second one will have b parameter too high
    * Third one will have a parameter too high
    * Fourth one will have both a & b parameters too high
    * High value of those parameters indicates that the LSC is skewed and we do not want to save it
    * 
    */
   for (uint16_t i = 0U; i < 4; i++)
   {
      new_longi_stat_curves[i].f_valid = true;
      new_longi_stat_curves[i].x_min = -20.0F;
      new_longi_stat_curves[i].x_max = 20.0F;
   }

   new_longi_stat_curves[1].b = 0.2F;

   new_longi_stat_curves[2].a = 0.002F;

   new_longi_stat_curves[3].a = 0.002F;
   new_longi_stat_curves[3].b = 0.2F;

   /** \action
    * Call Remember_Old_LSC()
    */
   Remember_Old_LSC(new_longi_stat_curves, host_props, old_longi_stat_curves);

   /** \result
    * Verify that first LSC is correctly translated and saved
    * Verify that remaining LSCs are not saved
    */
   CHECK_TRUE(old_longi_stat_curves[0].f_valid);
   DOUBLES_EQUAL(old_longi_stat_curves[0].x_min, -25.0F, 1e-4);
   DOUBLES_EQUAL(old_longi_stat_curves[0].x_max , 15.0F, 1e-4);

   for (uint8_t i = 1U; i < 4; i++)
   {
      CHECK_FALSE(old_longi_stat_curves[i].f_valid)
   }
}

/** \purpose
 * Purpose is to verify that the function Extend_Old_LSC extends LSC to behind under certain conditions - they are straight and previously there was a similar LSC 
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Old_LSC, Extend_Old_LSC_Extension)
{
   /** \precond
    * Set up four valid old LSCs
    * All of them will be a straight line 
    * First one will have c parameter set to 0.0F, will start at -60.0F and end at -50.0F 
    * Second one will have c parameter set to 5.0F, will start at -60.0F and end at -50.0F
    * Third one will have c parameter set to 0.0F, will start at -30.0F and end at -20.0F
    * Fourth one will have c parameter set to 0.0F, will start at -40.0F and end at -30.0F
    * Set up one valid new LSCs that will be similar to last LSC, but translated by host's movement
    * 
    */

   old_longi_stat_curves[0].f_valid = true;
   old_longi_stat_curves[0].x_min = -60.0F;
   old_longi_stat_curves[0].x_max = -50.0F;
   old_longi_stat_curves[0].c = 0.0F;
   
   old_longi_stat_curves[1].f_valid = true;
   old_longi_stat_curves[1].x_min = -60.0F;
   old_longi_stat_curves[1].x_max = -50.0F;
   old_longi_stat_curves[1].c = 5.0F;
   
   old_longi_stat_curves[2].f_valid = true;
   old_longi_stat_curves[2].x_min = -30.0F;
   old_longi_stat_curves[2].x_max = -20.0F;
   old_longi_stat_curves[2].c = 0.0F;

   old_longi_stat_curves[3].f_valid = true;
   old_longi_stat_curves[3].x_min = -40.0F;
   old_longi_stat_curves[3].x_max = -30.0F;
   old_longi_stat_curves[3].c = 0.0F;

   new_longi_stat_curves[0].f_valid = true;
   new_longi_stat_curves[0].x_min = -35.0F;
   new_longi_stat_curves[0].x_max = -25.0F;
   new_longi_stat_curves[0].c = 0.0F;
   
   /** \action
    * Call Extend_Old_LSC()
    */
   Extend_Old_LSC(old_longi_stat_curves, new_longi_stat_curves);

   /** \result
    * Verify that new LSC is correctly extended by its old version
    */

   DOUBLES_EQUAL(new_longi_stat_curves[0].x_min, -40.0F, 1e-4);
}

/** \purpose
 * Purpose is to verify that the function Extend_Old_LSC does not extend LSC under certain conditions - they are skewd or start not far behind 
 * \req
 * NA
 */
TEST(f360_update_longi_stat_curves_Old_LSC, Extend_Old_LSC_No_Extension)
{
   /** \precond
    * Set up four valid new LSCs
    * All of them will end at 20.0F
    * First one will have b parameter too high
    * Second one will have a parameter too high
    * Third one will have both a & b parameters too high
    * Three of them of them of them will start at -20.0F
    * Fourth one will start at -10.0F 
    * High value of those parameters indicates that the LSC is skewed and we do not want to extend it
    * Additionally, set up four old LSC similar to new LSC but translated to behind by host movement
    * 
    */
   for (uint16_t i = 0U; i < 3; i++)
   {
      new_longi_stat_curves[i].f_valid = true;
      new_longi_stat_curves[i].x_min = -20.0F;
      new_longi_stat_curves[i].x_max = 20.0F;
      old_longi_stat_curves[i].f_valid = true;
      old_longi_stat_curves[i].x_min = -25.0F;
      old_longi_stat_curves[i].x_max = 15.0F;
   }

   new_longi_stat_curves[0].b = 0.2F;
   old_longi_stat_curves[0].b = 0.2F;

   new_longi_stat_curves[1].a = 0.002F;
   old_longi_stat_curves[1].a = 0.002F;

   new_longi_stat_curves[2].a = 0.002F;
   new_longi_stat_curves[2].b = 0.2F;
   old_longi_stat_curves[2].a = 0.002F;
   old_longi_stat_curves[2].b = 0.2F;
   
   new_longi_stat_curves[3].f_valid = true;
   new_longi_stat_curves[3].x_min = -10.0F;
   new_longi_stat_curves[3].x_max = 20.0F;
   old_longi_stat_curves[3].f_valid = true;
   old_longi_stat_curves[3].x_min = -15.0F;
   old_longi_stat_curves[3].x_max = 15.0F;

   /** \action
    * Call Extend_Old_LSC()
    */
   Extend_Old_LSC(old_longi_stat_curves, new_longi_stat_curves);

   /** \result
    * Verify that all LSCs are not extended
    */
   for (uint8_t i = 0U; i < 3; i++)
   {
      DOUBLES_EQUAL(new_longi_stat_curves[i].x_min, -20.0F, 1e-4);
   }
   DOUBLES_EQUAL(new_longi_stat_curves[3].x_min, -10.0F, 1e-4);
}
/** @}*/

/** \defgroup f360_update_longi_stat_curves_Extend_LSC_With_Detections
 *  @{
 */

/** \brief
 * This test group defines tests for function Extend_LSC_With_Detections().
 * The function extends LSC x_max using stationary detections when only rear sensors are present.
 */
TEST_GROUP(f360_update_longi_stat_curves_Extend_LSC_With_Detections)
{
   F360_Calibrations_T calibs;
   F360_Host_T host;
   rspp_variant_A::RSPP_Detection_List_T raw_detections;
   F360_Longi_Stat_Curve_T longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES];

   float32_t test_pass_thres = 1.0e-5F;

   /** \setup
    * Initialize default calibrations, host, detections, and LSCs.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      // Initialize host with valid speed
      host = F360_Host_T{};
      host.speed = 30.0F; // Above 3.0F threshold and higher still to allow lsc starting upto 35m behind host.

      // Initialize raw detections
      raw_detections = rspp_variant_A::RSPP_Detection_List_T{};
      raw_detections.number_of_valid_detections = 0U;
      raw_detections.vcslong_det_idx_min = F360_INVALID_ID;
      raw_detections.vcslong_det_idx_max = F360_INVALID_ID;

      // Initialize all reference indices to invalid
      for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
      {
         raw_detections.vcslong_sorted_ref_det_idx[i] = F360_INVALID_ID;
      }

      // Initialize LSCs to default invalid state
      for (uint8_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         longi_stat_curves[i] = F360_Longi_Stat_Curve_T{};
         longi_stat_curves[i].f_valid = false;
         longi_stat_curves[i].x_min = 0.0F;
         longi_stat_curves[i].x_max = 0.0F;
         longi_stat_curves[i].a = 0.0F;
         longi_stat_curves[i].b = 0.0F;
         longi_stat_curves[i].c = 0.0F;
         longi_stat_curves[i].mean_lat_pos = 0.0F;
      }
      
      // Set up lsc that is valid for extension
      Setup_Valid_LSC_For_Extension(0U, -15.0F, 3.0F);

      // Set up detections such that they would extend above LSC
      Set_Three_Sorted_Equidistant_Stationary_Detections(-14.0F, -11.0F, 3.0F);
   }

   /** \brief Helper function to set up a valid LSC for extension testing */
   void Setup_Valid_LSC_For_Extension(uint8_t lsc_idx, float32_t x_max_val, float32_t mean_lat)
   {
      longi_stat_curves[lsc_idx].f_valid = true;
      longi_stat_curves[lsc_idx].x_min = x_max_val - 15.0F; // Length > 10m
      longi_stat_curves[lsc_idx].x_max = x_max_val;
      longi_stat_curves[lsc_idx].a = 0.0F; // Linear (< 0.01)
      longi_stat_curves[lsc_idx].b = 0.0F; // Parallel to host (< 0.27)
      longi_stat_curves[lsc_idx].c = mean_lat;
      longi_stat_curves[lsc_idx].mean_lat_pos = mean_lat;
   }

   /** \brief Helper function to add 3 sorted colinear equidistant stationary detections to the list */
   void Set_Three_Sorted_Equidistant_Stationary_Detections(float32_t vcs_x_min, float32_t vcs_x_max, float32_t vcs_y)
   {
      // If vcs_x_min is greater than vcs_x_max due to user error, swap them to ensure correct ordering
      if (vcs_x_min > vcs_x_max)
      {
         const float32_t vcs_x_max_temp = vcs_x_max;
         vcs_x_max = vcs_x_min;
         vcs_x_min = vcs_x_max_temp;
      }
      raw_detections.vcslong_sorted_ref_det_idx[0] = 0U;
      raw_detections.vcslong_sorted_ref_det_idx[1] = 1U;
      raw_detections.vcslong_sorted_ref_det_idx[2] = 2U;

      raw_detections.detections[0].processed.vcs_position_x = vcs_x_min;
      raw_detections.detections[0].processed.vcs_position_y = vcs_y;
      raw_detections.detections[0].processed.f_ok_to_use = true;
      raw_detections.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      raw_detections.detections[0].processed.next_sorted_idx = 1;

      raw_detections.detections[1].processed.vcs_position_x = (vcs_x_max + vcs_x_min) / 2.0F;
      raw_detections.detections[1].processed.vcs_position_y = vcs_y;
      raw_detections.detections[1].processed.f_ok_to_use = true;
      raw_detections.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      raw_detections.detections[1].processed.next_sorted_idx = 2;

      raw_detections.detections[2].processed.vcs_position_x = vcs_x_max;
      raw_detections.detections[2].processed.vcs_position_y = vcs_y;
      raw_detections.detections[2].processed.f_ok_to_use = true;
      raw_detections.detections[2].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      raw_detections.detections[2].processed.next_sorted_idx = F360_INVALID_ID;
      
      raw_detections.number_of_valid_detections = 3U;
   }
};

/** \purpose
 * Purpose is to verify that LSC is extended when it is valid and appropriate detections are present.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Valid_Extension)
{
   /** \precond
    * No change from TEST_SETUP
    */

    /** \action
     * Call function
     */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended
    */
   DOUBLES_EQUAL(-11.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is extended when it is valid and appropriate detections are present even when lsc is curved
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Valid_Extension_Curved)
{
   /** \precond
    * Set up a curving LSC and corresponing detections that would extend it
    */
   Setup_Valid_LSC_For_Extension(0U, -15.0F, 16.0F);
   longi_stat_curves[0].a = 0.01F;
   longi_stat_curves[0].b = -0.25F;
   longi_stat_curves[0].c= 3.0F;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -11.1F, 7.0F);

    /** \action
     * Call function
     */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended
    */
   DOUBLES_EQUAL(-11.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when it is invalid.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Invalid_No_Extension)
{
   /** \precond
    * Set up invalid LSC
    */
   longi_stat_curves[0].f_valid = false;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when x_max is in front of host (>= 0).
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_X_Max_In_Front_Of_Host_No_Extension)
{
   /** \precond
 * Set up LSC with x_max >= 0 (in front of host)
 * Set up detections in front of host origin but fulfilling all other lsc extension criteria
 */
   Setup_Valid_LSC_For_Extension(0U, 5.0F, 3.0F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(5.0F, 10.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when x_max is too far behind host.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_X_Max_Too_Far_Behind_Host_No_Extension)
{
   /** \precond
    * Set up LSC with x_max too far behind host (beyond speed-dependent limit)
    * Set up detection that would extend the LSC if LSC was valid
    */
   Setup_Valid_LSC_For_Extension(0U, -40.0F, 3.0F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-35.0F, -30.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when lateral position of last object in lsc is too far from host.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Lat_Pos_Too_Far_No_Extension)
{
   /** \precond
    * Set up last object y position to beyond 10m from host
    * Set up detection that would extend the LSC if LSC was valid
    */
   Setup_Valid_LSC_For_Extension(0U, -15.0F, 11.1F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -5.0F, 11.1F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when lateral position of last object in lsc is too close to host.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Lat_Pos_Too_Close_No_Extension)
{
   /** \precond
    * Set up last object y position to closer than 1.5m to host
    * Set up detection that would extend the LSC if LSC was valid
    */
   Setup_Valid_LSC_For_Extension(0U, -15.0F, 1.4F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -5.0F, 1.4F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when curvature coefficient 'a' is too high.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Curvature_Too_High_No_Extension)
{
   /** \precond
    * Set up LSC with |a| > 0.012
    * Set up detection that would extend the LSC if LSC was valid
    */
   longi_stat_curves[0].a = 0.02F;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when slope coefficient 'b' is too high.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Slope_Too_High_No_Extension)
{
   /** \precond
    * Set up LSC with |b| >= 0.27
    * Set up detection that would extend the LSC if LSC was valid
    */
   longi_stat_curves[0].b = 0.28F;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when intercept coefficient 'c' is too high.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Intercept_Too_High_No_Extension)
{
   /** \precond
    * Set up LSC with c > 10m
    * Set up detection that would extend the LSC if LSC was valid
    */
   longi_stat_curves[0].c = 11.0F;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -5.0F, 11.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when intercept coefficient 'c' is too low.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Intercept_Too_Low_No_Extension)
{
   /** \precond
    * Set up LSC with c less than 1.5m
    * Set up detection that would extend the LSC if LSC was valid
    */
   longi_stat_curves[0].c = 1.4F;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -5.0F, 1.4F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when LSC Y intercept is on the opposite side of the host compared to the lat mean position of the lsc
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Intercept_Opposite_Side_No_Extension)
{
   /** \precond
    * Set up sloping LSC with position of Y intercept "c" being on the opposite side of host compared to mean lateral position of LSC
    * Set up detection that would extend the LSC if LSC was valid
    */
   Setup_Valid_LSC_For_Extension(0U, -10.0F, 2.0F);
   longi_stat_curves[0].a = 0.0F;
   longi_stat_curves[0].b = -0.2F;
   longi_stat_curves[0].c = -2.0F;
   longi_stat_curves[0].x_min = -30.0F;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-8.0F, -7.0F, -0.4F);

   const float32_t original_x_max = longi_stat_curves[0].x_max;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that LSC is not extended when length is less than 10m.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, LSC_Length_Too_Short_No_Extension)
{
   /** \precond
    * Set up LSC with length <= 10m
    * Set up detection that would extend the LSC if LSC was valid
    */
   longi_stat_curves[0].f_valid = true;
   longi_stat_curves[0].x_min = -20.0F;
   longi_stat_curves[0].x_max = -15.0F; // Length = 5m
   longi_stat_curves[0].a = 0.0F;
   longi_stat_curves[0].b = 0.0F;
   longi_stat_curves[0].mean_lat_pos = 3.0F;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   Set_Three_Sorted_Equidistant_Stationary_Detections(-11.0F, -5.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that when no valid detections are found, no LSC extension happens
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, No_Valid_Detections_No_Extension)
{
   /** \precond
    * Set up valid LSC and no other valid detections
    */
   raw_detections.number_of_valid_detections = 0U;
   const float32_t original_x_max = longi_stat_curves[0].x_max;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that detection with f_ok_to_use = false is ignored.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Detection_Not_Ok_To_Use_Ignored)
{
   /** \precond
    * Set up valid LSC and detection with f_ok_to_use = false
    */
   const float32_t original_x_max = longi_stat_curves[0].x_max;
   
   Set_Three_Sorted_Equidistant_Stationary_Detections(-14.0F, -11.0F, 3.0F);
   raw_detections.detections[0].processed.f_ok_to_use = false;
   raw_detections.detections[1].processed.f_ok_to_use = false;
   raw_detections.detections[2].processed.f_ok_to_use = false;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that moving detection is ignored (not AMBIGUOUS motion status).
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Moving_Detection_Ignored)
{
   /** \precond
    * Set up valid LSC and moving detection
    */
   Setup_Valid_LSC_For_Extension(0U, -15.0F, 3.0F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;
   
   Set_Three_Sorted_Equidistant_Stationary_Detections(-14.0F, -11.0F, 3.0F);
   raw_detections.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detections.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detections.detections[2].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that detection outside lateral gate is ignored.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Detection_Outside_Lateral_Gate_Ignored)
{
   /** \precond
    * Set up valid LSC and detection with lateral position outside gate
    * Set up LSC last object lateral position to 1m and detection at 2m lateral position(gate is 0.975m by default)
    */
   const float32_t original_x_max = longi_stat_curves[0].x_max;
   
   // Detection lateral position more than 0.975m from LSC last object lat pos
   Set_Three_Sorted_Equidistant_Stationary_Detections(-14.0F, -11.0F, 2.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that detection beyond host origin (x > 0) stops processing.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Detection_Beyond_Host_Origin_Stops_Processing)
{
   /** \precond
    * Set up valid LSC and detection beyond host origin which would otherwise have extended the LSC
    */
   Setup_Valid_LSC_For_Extension(0U, -2.0F, 3.0F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;
   
   // Detection in front of host
   Set_Three_Sorted_Equidistant_Stationary_Detections(2.0F, 4.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that detection exceeding longitudinal gate stops processing.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Detection_Exceeds_Long_Gate_Stops_Processing)
{
   /** \precond
    * Set up valid LSC and detection that exceeds longitudinal gate
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);
   const float32_t original_x_max = longi_stat_curves[0].x_max;
   
   // Detection gap exceeds longitudinal gate
   Set_Three_Sorted_Equidistant_Stationary_Detections(-5.0F, -3.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is unchanged (detection gap too large)
    */
   DOUBLES_EQUAL(original_x_max, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify multiple detections are processed and max x is found.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Max_X_Used)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);
   
   // Add chained detections (sorted order)
   Set_Three_Sorted_Equidistant_Stationary_Detections(-18.0F, -12.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection
    */
   DOUBLES_EQUAL(-12.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that when detections extend LSC to a position closer than -10m from host,
 * the LSC x_max is extended by an additional 5m beyond the furthest detection.
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Max_X_Below_Minus_10m_Extended_By_5m)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections where the furthest detection is closer than -10m to host
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);

   // Add chained detections (sorted order) where the last detection is at -8m (closer than -10m threshold)
   Set_Three_Sorted_Equidistant_Stationary_Detections(-16.0F, -8.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection (-8m) plus 5m = -3m
    * This is because max_det_x (-8m) is >= -10m, so the logic adds 5m extension
    */
   DOUBLES_EQUAL(-3.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that when detection to extend LSC is too far in longitudinal gate
 * the extension stops and previous detection is used to extend LSC. It also checks that
 * when f_ok_to_use flag is set to false, then that detection is not used
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Det_X_Gate_Not_Met_and_Not_Ok_To_Use)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections where one detection is more than 5m away
    * in longitudinal direction from the previous detection so as to stop the extension process
    * Also set one det as not ok to use which should then not be used for extension
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);

   // Add chained detections (sorted order) where the last detection is at more than 5m away from the previous det and second last det is not ok to use
   Set_Three_Sorted_Equidistant_Stationary_Detections(-16.0F, -8.0F, 3.0F);

   raw_detections.detections[1].processed.f_ok_to_use = false; // This detection should be ignored and not used for extension as flag not ok to use is set to false
   raw_detections.detections[2].processed.vcs_position_x = -6.9F; // This detection should be ignored for extension as it is more than 5m away from previous valid detection at -12m


   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection which will be -12m
    * in this case since the detection after that is not ok to use and the one after 
    * that is more than 5m away longitudinally
    */
   DOUBLES_EQUAL(-16.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that when detection to extend LSC is too far in lateral gate
 * that det is not considered for LSC extension
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Det_Y_Gate_Not_Met)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections where one detection is more than 0.975m away
    * in lateral direction (i.e. size of lat pos gate) from the previous detection so that it is not
    * considered for LSC extension
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);

   // Add chained detections (sorted order) where the last detection is at more than 0.975m away from the previous det
   Set_Three_Sorted_Equidistant_Stationary_Detections(-16.0F, -8.0F, 3.0F);

   raw_detections.detections[2].processed.vcs_position_y = 2.0F; // This detection should be ignored for extension as it is more than 0.975m away laterally from previous det at 3.0F

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection which will be -12m
    * in this case since the detection after that is more than 0.975m away laterally and should
    * not be considered for lsc extension.
    */
   DOUBLES_EQUAL(-12.0F, longi_stat_curves[0].x_max, test_pass_thres);
}

/** \purpose
 * Purpose is to verify that when detection to extend LSC is moving det, it is ignored for lsc extension
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Moving_Det_Not_Used)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections where one detection is set as moving
    * det so that it is not considered for LSC extension
    */
   Setup_Valid_LSC_For_Extension(0U, -20.0F, 3.0F);

   // Add chained detections (sorted order) where the last detection is a moving det
   Set_Three_Sorted_Equidistant_Stationary_Detections(-16.0F, -8.0F, 3.0F);
   raw_detections.detections[2].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection which will be -12m
    * in this case since the detection after that is moving det and should
    * not be considered for lsc extension.
    */
   DOUBLES_EQUAL(-12.0F, longi_stat_curves[0].x_max, test_pass_thres);
}


/** \purpose
 * Purpose is to verify that when detection to extend LSC is ahead of host origin
 * it is not used for extension
 * \req NA
 */
TEST(f360_update_longi_stat_curves_Extend_LSC_With_Detections, Multiple_Detections_Det_In_Front_Of_Host_Not_Used_For_Extension)
{
   /** \precond
    * Set up valid LSC and multiple sorted detections where last detection is set to be
    * ahead of the host origin point. This detection should thus not be used for lsc extension
    */
   Setup_Valid_LSC_For_Extension(0U, -10.0F, 3.0F);

   // Add chained detections (sorted order) where the last detection is ahead of the host origin and thus should not be considered for lsc extension
   Set_Three_Sorted_Equidistant_Stationary_Detections(-5.0F, 1.0F, 3.0F);

   /** \action
    * Call function
    */
   Extend_LSC_With_Detections(calibs, host, raw_detections, longi_stat_curves);

   /** \result
    * Verify LSC x_max is extended to furthest valid detection which will be -2m (+5m since
    * it is within -10m of host triggering the +5m extra extension) and the det in front of
    * host origin is ignored
    */
   DOUBLES_EQUAL(3.0F, longi_stat_curves[0].x_max, test_pass_thres);
}
/** @}*/
