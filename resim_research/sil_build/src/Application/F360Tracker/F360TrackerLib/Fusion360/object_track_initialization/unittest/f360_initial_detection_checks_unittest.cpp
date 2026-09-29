/** \file
 * This file contains unit tests for content of f360_initial_detection_checks.cpp file
 */

#include "f360_initial_detection_checks.h"
#include "f360_cluster_grouping_data_generator.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_initial_detection_checks
 *  @{
 */

/** \brief
   Tests related to Initial Detection Checks. Checking if discriminating based on
   azimuth confidence and potential angle jumps works in terms of initialization.
 */
TEST_GROUP(f360_initial_detection_checks)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Hist_T det_hist{};
   rspp_variant_A::RSPP_Detection_List_T raw_detections{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Cluster_T cluster{};

   /** \setup
    * Initialize needed structures
    * set number of current dets for cluster to 2
    * set number of historical dets for cluster to 3
    * assign det ids for cluster: 0, 1, 2 (idx) to historical and 4,5 (id) to current
    */
   TEST_SETUP()
   {
      cluster.ndets = 2;
      cluster.num_old_dets=3;

      cluster.detids[0]=4;
      cluster.detids[1]=5;

      cluster.old_det_idx[0]=0;
      cluster.old_det_idx[1]=1;
      cluster.old_det_idx[2]=2;
   }

};

/** \purpose
 * Check if cluster containing one angle jump detection in current and one in historical
 * detections is valid to continue initialization (not regarding ambiguity and azimuth confidence).
 *
 * \req
 * NA
 */
TEST(f360_initial_detection_checks, One_potential_angle_jump_in_current_and_hist)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up 3 historical and 2 current detections
    * set up one historical detection with f_potential_angle_jump and confid_azimuth low in historical and current dets
    */

   //hist dets
   det_hist.det_data[0].f_potential_angle_jump=true;
   raw_detections.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_hist.det_data[1].f_potential_angle_jump=false;
   raw_detections.detections[1].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   det_hist.det_data[2].f_potential_angle_jump=false;
   raw_detections.detections[2].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   //current dets
   det_props[3].f_potential_angle_jump = true;
   raw_detections.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_props[4].f_potential_angle_jump = false;
   raw_detections.detections[4].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
    * call Initial_Detection_Checks()
    */
   bool result = Initial_Detection_Checks(det_hist, raw_detections, det_props, cluster);

   /** \result
    * cluster valid for initialization
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if cluster with one historical and none current detections marked as angle jump is not valid for initialization.
 *
 * \req
 * NA
 */

TEST(f360_initial_detection_checks, None_current_detections_marked_as_angle_jump)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up 3 historical and 2 current detections
    * set up one historical detection with f_potential_angle_jump and confid_azimuth low
    */

   //hist dets
   det_hist.det_data[0].f_potential_angle_jump=true;
   raw_detections.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_hist.det_data[1].f_potential_angle_jump=false;
   raw_detections.detections[1].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   det_hist.det_data[2].f_potential_angle_jump=false;
   raw_detections.detections[2].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   //current dets
   det_props[3].f_potential_angle_jump = false;
   det_props[3].f_angle_amb = true;
   raw_detections.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_props[4].f_potential_angle_jump = false;
   det_props[4].f_angle_amb = true;
   raw_detections.detections[4].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
    * call Initial_Detection_Checks()
    */
   bool result= Initial_Detection_Checks(det_hist, raw_detections, det_props, cluster);

   /** \result
    * cluster not valid for initialization
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if cluster with all current and hist detections marked as angle jump is not valid for initialization.
 *
 * \req
 * NA
 */

TEST(f360_initial_detection_checks, All_dets_with_angle_jump)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up 3 historical and 2 current detections
    * detection properties set as below
    */

   //hist dets
   det_hist.det_data[0].f_potential_angle_jump=true;
   raw_detections.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_hist.det_data[1].f_potential_angle_jump=true;
   raw_detections.detections[1].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   det_hist.det_data[2].f_potential_angle_jump=true;
   raw_detections.detections[2].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   //current dets
   det_props[3].f_potential_angle_jump = true;
   det_props[3].f_angle_amb = false;
   raw_detections.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_props[4].f_potential_angle_jump = true;
   det_props[4].f_angle_amb = false;
   raw_detections.detections[4].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
    * call Initial_Detection_Checks()
    */
   bool result= Initial_Detection_Checks(det_hist, raw_detections, det_props, cluster);

   /** \result
    * cluster not valid for initialization
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if the cluster with all detections with low azimuth confidence is not valid for initialization.
 *
 * \req
 * NA
 */

TEST(f360_initial_detection_checks, All_dets_with_low_az_conf)
{
   /** \precond
    * Preconditions from setup, additionally:
    * set up 3 historical and 2 current detections
    * detection properties set as below
    */

   //hist dets
   det_hist.det_data[0].f_potential_angle_jump=true;
   det_hist.det_data[0].az_conf = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_hist.det_data[1].f_potential_angle_jump=true;
   det_hist.det_data[1].az_conf = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_hist.det_data[2].f_potential_angle_jump=false;
   det_hist.det_data[2].az_conf = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   //current dets
   det_props[3].f_potential_angle_jump = true;
   det_props[3].f_angle_amb = false;
   raw_detections.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   det_props[4].f_potential_angle_jump = false;
   det_props[4].f_angle_amb = false;
   raw_detections.detections[4].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;

   /** \action
    * call Initial_Detection_Checks()
    */
   bool result = Initial_Detection_Checks(det_hist, raw_detections, det_props, cluster);

   /** \result
    * cluster not valid for initialization
    */
   CHECK_FALSE(result);
}

/** @}*/
