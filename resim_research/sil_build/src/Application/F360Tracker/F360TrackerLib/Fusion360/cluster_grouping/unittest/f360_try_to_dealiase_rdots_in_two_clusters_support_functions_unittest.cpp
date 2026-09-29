/** \file
 * This file contains unit tests for content of f360_try_to_dealiase_rdots_in_two_clusters_support_functions.cpp file
 */

#include "f360_try_to_dealiase_rdots_in_two_clusters_support_functions.h"
#include "f360_cluster_grouping_data_generator.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_try_to_dealiase_rdots_in_two_clusters_support_functions
 *  @{
 */

/** \brief
 * Test group for f360_try_to_dealiase_rdots_in_two_clusters_support_functions functions
 */
TEST_GROUP(f360_try_to_dealiase_rdots_in_two_clusters_support_functions)
{
   /** \setup
    * Setting up arguments for try_to_dealiase_rdots_in_two_clusters support functions and assigning them with basic values
    */
   F360_Calibrations_T calib = {};
   F360_Cluster_T cluster_older = {};
   F360_Cluster_T cluster_newer = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Hist_T detections_hist = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   float32_t rngrate_interval_width_older;
   float32_t rngrate_interval_width_newer;
   float32_t interval_older;
   float32_t interval_newer;

   int32_t ndets_older = 0;
   float32_t rdot_width[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER] = {};
   int32_t unique_rdot_interval_ids[MAX_NUM_UNIQUE_RDOT_INTERVAL] = {};
   int32_t unique_rdot_interval_index = 0;
   int32_t rdot_interval_ids[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER] = {};
   float32_t rep_rdot_comp = 0.0;

   float32_t tolerance = 1e-6;
   float32_t rdot_interval_width_1 = 69.8359375;
   float32_t rdot_interval_width_2 = 59.6132813;

   bool f_success = false;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      globals.max_otg_speed = 70.0F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;
      sensors[0].refined.time_since_measurement_s = 0.05F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;
      sensors[1].refined.time_since_measurement_s = 0.04F;

      tracker_info.num_unique_rdot_interval_widths = 2;
      tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
      tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;

      tracker_info.rdot_interval_compatibility[0][1] = true;
      tracker_info.rdot_interval_compatibility[1][0] = true;
   }
};

/** \purpose
 * Checking if test will pass if no detecitons will be added
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased_NoDetections)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */

   /** \action
    * Call Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased
    */
   Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer, f_success);

   /** \result
    * Checking if no cluster will be dealiased
    */
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
   CHECK_FALSE(f_success);
}

/** \purpose
 * Checking if test will pass for nominal pass case
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased_NominalPass)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 11.1, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 10.15);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 11.05, 0.2F);

   /** \action
    * Call Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased
    */
   Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer, f_success);

   /** \result
    * Ckecking if clusters will be dealiased
    */
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
   CHECK_TRUE(f_success);
}

/** \purpose
 * Checking if test will pass when no range rates will be compatible
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased_NoRRCompatibility)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, rdot_interval_width_1, 1, 10.1);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 31.1, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, rdot_interval_width_1, 1, 10.15);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 31.05, 0.2F);

   /** \action
    * Call Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased
    */
   Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer, f_success);

   /** \result
    * Checking if no clusters will be dealiased
    */
   CHECK_FALSE(f_success);
}

/** \purpose
 * Checking if test will pass if no detecitons will be added
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased_NoDetections)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */

   /** \action
    * Call Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased
    */
   Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_newer, interval_newer, f_success);

   /** \result
    * Checking is not cluster will be dealiased
    */
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
   CHECK_FALSE(f_success);
}

/** \purpose
 * Checking if test will pass for nominal pass case
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased_NominalPass)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   cluster_older.f_dealiased = true;
   cluster_older.rep_rdotcomp = 11.0;

   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.15);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 11.05, 0.1F);

   /** \action
    * Call Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased
    */
   Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_newer, interval_newer, f_success);

   /** \result
    * Checking if cluster will be dealiased
    */
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
   CHECK_TRUE(f_success);
}

/** \purpose
 * Checking if test will pass when there is no compatible range rates
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased_NoRRCompatibility)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   cluster_older.f_dealiased = true;
   cluster_older.rep_rdotcomp = 20.0;

   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 31.1, 0.1F);

   /** \action
    * Call Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased
    */
   Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(raw_detection_list, det_props, detections_hist, sensors, tracker_info, globals, calib, cluster_older, cluster_newer,
      rngrate_interval_width_newer, interval_newer, f_success);

   /** \result
    * Checking if cluster will not be dealiased
    */
   CHECK_FALSE(f_success);
}

/** \purpose
 * Checking if test will pass for nominal pass case
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased_NominalPass)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   cluster_older.f_dealiased = true;
   cluster_older.rep_rdotcomp = 11.0;

   cluster_newer.f_dealiased = true;
   cluster_newer.rep_rdotcomp = 11.0;

   /** \action
    * Call Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased
    */
   Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased(calib, cluster_older, cluster_newer,
      f_success);

   /** \result
    * Checking if cluster will be dealiased
    */
   CHECK_TRUE(f_success);
}

/** \purpose
 * Checking if rdot widths are filling correctlly
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Find_All_Rdot_Widths_NominalPass)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 10.15);
   Add_Det(det_props, raw_detection_list, cluster_older, 2, 20.15);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 11.05, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 31.05, 0.2F);

   /** \action
    * Call Find_All_Rdot_Widths
    */
   Find_All_Rdot_Widths(cluster_older, raw_detection_list, sensors, detections_hist, ndets_older, rdot_width);

   /** \result
    * Checking if rdot widths are filling correctlly
    */
   CHECK_EQUAL(4, ndets_older);
   DOUBLES_EQUAL(rdot_interval_width_1, rdot_width[0], tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rdot_width[1], tolerance);
   DOUBLES_EQUAL(rdot_interval_width_1, rdot_width[2], tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rdot_width[3], tolerance);
}

/** \purpose
 * Checking if range rate calculating correctly
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Calc_Rep_Rdot_Comp_NominalPass)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 10.0);
   Add_Det(det_props, raw_detection_list, cluster_older, 2, 20.0);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 11.0, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 11.0, 0.2F);

   rdot_interval_ids[0] = 1;
   rdot_interval_ids[1] = 2;
   rdot_interval_ids[2] = 1;
   rdot_interval_ids[3] = 2;

   unique_rdot_interval_index = 0;
   unique_rdot_interval_ids[unique_rdot_interval_index] = 1;

   /** \action
    * Call Calc_Rep_Rdot_Comp
    */
   rep_rdot_comp = Calc_Rep_Rdot_Comp(cluster_older, det_props, detections_hist, unique_rdot_interval_ids, unique_rdot_interval_index, rdot_interval_ids);

   /** \result
    * Checking if rdot widths are filling correctlly
    */
   DOUBLES_EQUAL(10.5, rep_rdot_comp, tolerance);
}

/** \purpose
 * Checking if range rate calculating correctly
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Calc_Rep_Rdot_Comp_OnlyDets)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 10.0);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 11.0);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.0);

   rdot_interval_ids[0] = 1;
   rdot_interval_ids[1] = 1;
   rdot_interval_ids[2] = 1;

   unique_rdot_interval_index = 0;
   unique_rdot_interval_ids[unique_rdot_interval_index] = 1;

   /** \action
    * Call Calc_Rep_Rdot_Comp
    */
   rep_rdot_comp = Calc_Rep_Rdot_Comp(cluster_older, det_props, detections_hist, unique_rdot_interval_ids, unique_rdot_interval_index, rdot_interval_ids);

   /** \result
    * Checking if rdot widths are filling correctlly
    */
   DOUBLES_EQUAL(11.0, rep_rdot_comp, tolerance);
}

/** \purpose
 * Checking if range rate calculating correctly
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Calc_Rep_Rdot_Comp_OnlyHistDets)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 11.0, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 12.0, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 13.0, 0.3F);

   rdot_interval_ids[0] = 1;
   rdot_interval_ids[1] = 1;
   rdot_interval_ids[2] = 1;

   unique_rdot_interval_index = 0;
   unique_rdot_interval_ids[unique_rdot_interval_index] = 1;

   /** \action
    * Call Calc_Rep_Rdot_Comp
    */
   rep_rdot_comp = Calc_Rep_Rdot_Comp(cluster_older, det_props, detections_hist, unique_rdot_interval_ids, unique_rdot_interval_index, rdot_interval_ids);

   /** \result
    * Checking if rdot widths are filling correctlly
    */
   DOUBLES_EQUAL(12.0, rep_rdot_comp, tolerance);
}

/** \purpose
 * Checking if range rate calculating correctly
 */
TEST(f360_try_to_dealiase_rdots_in_two_clusters_support_functions, Calc_Rep_Rdot_Comp_NoDets)
{
   /** \precond
    * Setting up required variables like detections if needed.
    */

   /** \action
    * Call Calc_Rep_Rdot_Comp
    */
   rep_rdot_comp = Calc_Rep_Rdot_Comp(cluster_older, det_props, detections_hist, unique_rdot_interval_ids, unique_rdot_interval_index, rdot_interval_ids);

   /** \result
    * Checking if rdot widths are filling correctlly
    */
   DOUBLES_EQUAL(0.0, rep_rdot_comp, tolerance);
}
/** @}*/
