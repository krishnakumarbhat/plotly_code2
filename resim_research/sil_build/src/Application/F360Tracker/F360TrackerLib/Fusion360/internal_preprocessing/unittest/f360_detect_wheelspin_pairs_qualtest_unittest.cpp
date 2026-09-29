/** \file
 * Test file including combined qualification and unit tests for wheelspin detection.
*/

#include "f360_detect_wheelspin_pairs.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <vector>
#include <tuple>
#include "rspp_detection_list.h"

/** \defgroup  f360_detect_wheelspin_pairs
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Test group including all input to Detect_Wheel_Spin_Pairs and a helper file to
*  add detections used for testing. It also includes some constants located in Detect_Wheel_Spin_Pairs
*  that could be used to quickly adapt tests if the internal constants change.
**/
TEST_GROUP(f360_detect_wheelspin_pairs_qualtest_unittest)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets = 0;
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   // Internal parameters in function
   const uint8_t MAX_SAVED_WHEEL_SPIN_PAIRS = (20U);
   const uint32_t MAX_NR_DETS_TO_MARK = (10U);

   // Non-spuare value of the max distance allowed
   float32_t max_wheel_spin_dist;

   /** \setup
   * Nothing to setup in this test group
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      max_wheel_spin_dist = F360_Sqrtf(calibrations.k_max_wheel_spin_dist_sq);
      raw_detect_list.number_of_valid_detections = num_dets;
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

   void Run_Action()
   {
      Detect_Wheel_Spin_Pairs(calibrations, raw_detect_list, sensors, det_props, timing_info);
   }

   /*
    * Helper function to add detections that look sorted according to long pos. The detections
    * will be sorted in the order they are added. A pointer to the detection is returned so the
    * test can keep track of it.
    */
   F360_Detection_Props_T * Add_Detection_Sorted(
         const float32_t long_pos,
         const float32_t lat_pos,
         const float32_t azimuth,
         const float32_t range_rate)
   {
      det_props[num_dets].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[num_dets].vcs_position.x = long_pos;
      det_props[num_dets].vcs_position.y = lat_pos;
      raw_detect_list.detections[num_dets].processed.vcs_az = azimuth;
      raw_detect_list.detections[num_dets].raw.range_rate = range_rate;
      det_props[num_dets].range_rate_dealiased = range_rate;

      // Add sorted linkage to previous and next detection
      raw_detect_list.detections[num_dets].processed.next_sorted_idx = -1;
      if (num_dets > 0U)
      {
         raw_detect_list.detections[num_dets - 1U].processed.next_sorted_idx = num_dets;
         raw_detect_list.detections[num_dets].processed.prev_sorted_idx = num_dets - 1U;
      }
      else
      {
         raw_detect_list.detections[num_dets].processed.prev_sorted_idx = -1;
      }

      return &det_props[num_dets++];
   }

};

/**
*\purpose  Test that check that the algorithm evaluated the spread in range rate of a detection pair properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, doppler_spread_limits)
{
   /** \precond
    * Setup three pairs of detection separated in space. The pairs should have range rate spread under, on
    * and above the limit for marking wheelspin respectively.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t AZIMUTH = 0.0f;

   // Add a detection pair with low spread in doppler
   float32_t long_pos = 0.0F;
   float32_t lat_pos = 0.0F;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE + 0.99*calibrations.k_min_wheel_spin_doppler_spread)
         );

   // Add detection pair with border value spread in doppler
   long_pos = 0.5F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos = 0.5F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> border_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE + calibrations.k_min_wheel_spin_doppler_spread)
         );

   // Add detection pair with too low doppler spread for being wheelspin
   long_pos = 0.9F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos = 0.9F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, AZIMUTH, BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Check that the pair with range rate spread under the limit is not marked as wheelspin but the other
    * two pairs are.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose  Test that check that the algorithm evaluated the azimuth difference between pairs properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, azimuth_spread_limits)
{
   /** \precond
    * Setup three pairs of detection separated in space. The pairs should have azimuth difference under, on
    * and above the limit for marking wheelspin respectively.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t BASE_AZIMUTH = 0.0f;

   float32_t long_pos = 0.0F;
   float32_t lat_pos = 0.0F;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH + 0.9F*calibrations.k_max_azimuth_difference_for_wheelspin_pair, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos = 0.5F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos = 0.5F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> border_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH + calibrations.k_max_azimuth_difference_for_wheelspin_pair, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos = 0.9F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos = 0.9F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, lat_pos, BASE_AZIMUTH + 1.1F*calibrations.k_max_azimuth_difference_for_wheelspin_pair, HIGH_SPREAD_RANGE_RATE)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Check that the pairs with azimuth spread under the limit get marked as wheelspin but the pairs
    * with spread and on and above the limit does not.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose Test that check that the algorithm evaluated the spread in long position of a detection pair properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, long_spread_limits)
{
   /** \precond
    * Setup two pairs with high enough doppler spread to be wheelspin pair but have long pos spread
    * under and above the limit.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;

   float32_t long_pos_base = 0.0F;
   float32_t lat_pos_base = 0.0F;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos_base, lat_pos_base, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos_base + 0.9F*max_wheel_spin_dist, lat_pos_base, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos_base = 0.5F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos_base = 0.5F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos_base, lat_pos_base, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos_base + 1.1F*max_wheel_spin_dist, lat_pos_base, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that the pair with long spread below the limit get marked but the one with spread above don't.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose   Test that check that the algorithm evaluated the spread in lat position of a detection pair properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, lat_spread_limits)
{
   /** \precond
    * Setup two pairs with high enough doppler spread to be wheelspin pair but have lat pos spread
    * under and above the limit.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;

   float32_t long_pos_base = 0.0F;
   float32_t lat_pos_base = 0.0F;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos_base, lat_pos_base, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos_base, lat_pos_base + 0.9F*max_wheel_spin_dist, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos_base = 0.5F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;
   lat_pos_base = 0.5F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos_base, lat_pos_base, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos_base, lat_pos_base + 1.1F*max_wheel_spin_dist, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that the pair with lat spread below the limit get marked but the one with spread above don't.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose Test that check that the algorithm evaluated the absolute long position of detections properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, long_limits)
{
   /** \precond
    * Setup three pairs with high enough doppler spread to be wheelspin pair but have long pos of
    * the detections under, on and above the limit for when to consider wheelspin.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t LAT_POS = 0.0F;

   float32_t long_pos = 0.9F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos = calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> border_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   long_pos = 1.1F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that the long pos limit is inclusive so that the detections under and on the limit get marked as
    * wheelspin but those above don't.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose Test that check that the algorithm evaluated the absolute lat position of detections properly.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, lat_limits)
{
   /** \precond
    * Setup three pairs with high enough doppler spread to be wheelspin pair but have lat pos of
    * the detections under, on and above the limit for when to consider wheelspin.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t LONG_POS = 0.0F;

   float32_t lat_pos = 0.9F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> low_spread_pair = std::make_tuple(
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   lat_pos = calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> border_spread_pair = std::make_tuple(
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   lat_pos = 1.1F * calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> high_spread_pair = std::make_tuple(
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(LONG_POS, lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that the lat pos limit is inclusive so that the detections under and on the limit get marked as
    * wheelspin but those above don't.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(low_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(border_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(high_spread_pair)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(high_spread_pair)->wheel_spin_type);
}

/**
*\purpose Test that checks that only detection that have not already been marked as wheelspin is considered
*\purpose in the algorithm.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, remarking)
{
   /** \precond
    * Setup three detection pairs. The first two should have high enough doppler spread to be marked but
    * have one of the two detections in the pair already marked as wheelspin. The third pair have too low
    * doppler spread to be marked but both detections have already been marked.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t LOW_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 0.9*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t LAT_POS = 0.0F;

   // Add a detection pair with first detecion already marked
   float32_t long_pos = 0.0F;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> pair_with_base_marked = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );
   std::get<0>(pair_with_base_marked)->wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;

   // Add a detection pair with second detecion already marked
   long_pos = 0.5F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> pair_with_other_marked = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE)
         );
   std::get<1>(pair_with_other_marked)->wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;

   // Add a detection pair with both detecions already marked
   long_pos = 0.9F * calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   std::tuple<F360_Detection_Props_T*, F360_Detection_Props_T*> pair_with_both_marked = std::make_tuple(
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE),
         Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, LOW_SPREAD_RANGE_RATE)
         );
   std::get<0>(pair_with_both_marked)->wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   std::get<1>(pair_with_both_marked)->wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;

   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that no detection status is changed for any of the detections in the pairs
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(pair_with_base_marked)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<1>(pair_with_base_marked)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, std::get<0>(pair_with_other_marked)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(pair_with_other_marked)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<0>(pair_with_both_marked)->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, std::get<1>(pair_with_both_marked)->wheel_spin_type);
}

/**
*\purpose Test that verifies the behavior of the algorithm when we have multiple possible candidates for wheelspin pairs.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, multiple_candidates)
{
   /** \precond
    * Setup three detections that could all form a detection pair with each other. Also place detections between them
    * so that they could not be marked as wheelspin by beeing close to another formed pair.
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01F * calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t HIGHER_SPREAD_RANGE_RATE = HIGH_SPREAD_RANGE_RATE + 1.01F * calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t BASE_LONG_POS = 0.0F;
   const float32_t BASE_LAT_POS = 0.0F;

   // Add first detection in possible pair
   F360_Detection_Props_T * first_candidate = Add_Detection_Sorted(BASE_LONG_POS, BASE_LAT_POS, AZIMUTH, BASE_RANGE_RATE);

   // Add sorted detections in between that could not form any pairs
   for(int i = 0; i < 3; i++)
   {
      (void) Add_Detection_Sorted(BASE_LONG_POS - 0.99*max_wheel_spin_dist, BASE_LAT_POS, AZIMUTH, BASE_RANGE_RATE + 1.0F);
   }

   // Add second detection to a possible pair
   F360_Detection_Props_T * second_candidate = Add_Detection_Sorted(BASE_LONG_POS + 0.99*max_wheel_spin_dist, BASE_LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE);

   // Add sorted detections in between that could not form any pairs
   for(int i = 0; i < 3; i++)
   {
      (void) Add_Detection_Sorted(BASE_LONG_POS + 0.99*max_wheel_spin_dist, BASE_LAT_POS + 0.99*max_wheel_spin_dist, AZIMUTH, (HIGH_SPREAD_RANGE_RATE + HIGHER_SPREAD_RANGE_RATE)/2.0F);
   }

   // Add the third detection that could form a pair
   F360_Detection_Props_T * third_candidate = Add_Detection_Sorted(BASE_LONG_POS + 0.99*max_wheel_spin_dist, BASE_LAT_POS, AZIMUTH, HIGHER_SPREAD_RANGE_RATE);
   
   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that the detections in the pair with the highest doppler spread is marked as wheelspin while the
    * other is not marked at all.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, first_candidate->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, second_candidate->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, third_candidate->wheel_spin_type);
}
/**
*\purpose Test that verifies that detections will not be marked as wheel spin if they have detection with very similar range rate in proximity,
* even if there is another close detection with much higher range rat.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, similar_rr_close)
{
   /** \precond
    * Setup three detections - one wheelspin pair and one detection with similar range rate to one of them 
   **/
   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01F * calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t BASE_LONG_POS = 0.0F;
   const float32_t BASE_LAT_POS = 0.0F;
   
   F360_Detection_Props_T * first_paired_det = Add_Detection_Sorted(BASE_LONG_POS, BASE_LAT_POS, AZIMUTH, BASE_RANGE_RATE);
   F360_Detection_Props_T * second_paired_det = Add_Detection_Sorted(BASE_LONG_POS, BASE_LAT_POS + 0.99*max_wheel_spin_dist, AZIMUTH, HIGH_SPREAD_RANGE_RATE);
   F360_Detection_Props_T * det_in_proximity = Add_Detection_Sorted(BASE_LONG_POS, BASE_LAT_POS - 0.99*max_wheel_spin_dist, AZIMUTH, BASE_RANGE_RATE + 0.1F);
   
   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
    * Verify that both detections with similar range rate will not be marked as wheel_spin, but the one with different will.
    * Also verify that f_has_close_det_with_similar_rr is set for the two detections with similar range rate and not for the other.
   **/
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, first_paired_det->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det_in_proximity->wheel_spin_type);
   CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, second_paired_det->wheel_spin_type);
   CHECK_TRUE(first_paired_det->f_has_close_det_with_similar_rr);
   CHECK_TRUE(det_in_proximity->f_has_close_det_with_similar_rr);
   CHECK_FALSE(second_paired_det->f_has_close_det_with_similar_rr);
   
}


/**
*\purpose  Test the marking of detections close to detection pairs.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, proximity_detections)
{
   /** \precond
    * Setup two detections that could form a wheelspin pair but add other detections that could not form pairs between
    * them in the sorted detection props list. Some fill the requirements to be considered close to the wheelspin pair
    * some don't.
   **/
   std::vector<F360_Detection_Props_T*> wheelspin_dets;
   std::vector<F360_Detection_Props_T*> non_wheelspin_dets;

   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t LONG_POS = 0.5*calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair;

   // Detection values inside borders
   float32_t base_lat_pos = 0.5*calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair;
   float32_t base_proximity_lat_pos = base_lat_pos - 0.99*max_wheel_spin_dist;
   float32_t high_spread_lat_pos = base_lat_pos + 0.99*max_wheel_spin_dist;
   float32_t high_spread_proximity_lat_pos = high_spread_lat_pos + 0.99*max_wheel_spin_dist;

   /*
    * Add a base detection with eight detections close to it. The three detections with slightly different range rate added before and after should be
    * marked as wheelspin as well as the base detection since the algo look backwards and forwards 3 steps for each
    * wheelspin detection.
    */
   non_wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, base_proximity_lat_pos, AZIMUTH, BASE_RANGE_RATE - 2.5F));
   for(int i = 0; i < 3; i++)
   {
      wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, base_proximity_lat_pos, AZIMUTH, BASE_RANGE_RATE - 0.6F*(i+1)));
   }

   // Add the base detection that should be marked as wheelspin pair with the high spread detection.
   wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, base_lat_pos, AZIMUTH, BASE_RANGE_RATE));

   /*
    * Add a high range detection that will be marked as a wheelspin pair with the base detection. Also add three detections
    * sorted before it that are close to the high range one. One is a bit too far in lat position to be condidered a wheelspin
    * but the others should be marked wheelspin. Also add three close detectios sorted after. One should have too high long
    * pos so that the algo stop looking for any more close detection.
    */
   wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_proximity_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE + 0.6F));
   non_wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_lat_pos + 1.01*max_wheel_spin_dist, AZIMUTH, HIGH_SPREAD_RANGE_RATE));
   wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_proximity_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE + 1.2F));

   // Add the high range rate detection that form a wheelspin pair with the base detection
   wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE));

   wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_proximity_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE + 1.8F));
   non_wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS + 1.01*max_wheel_spin_dist, high_spread_proximity_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE));
   non_wheelspin_dets.push_back(Add_Detection_Sorted(LONG_POS, high_spread_proximity_lat_pos, AZIMUTH, HIGH_SPREAD_RANGE_RATE));

   raw_detect_list.number_of_valid_detections = num_dets;
   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   Run_Action();

   /** \result
   * Verify that all detections that are either in the wheelspin pair or considered close to a pair are marked as wheelspin
   * but that all others don't.
   **/
   for (F360_Detection_Props_T * det: wheelspin_dets)
   {
      CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, det->wheel_spin_type);
   }

   for (F360_Detection_Props_T * det: non_wheelspin_dets)
   {
      CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det->wheel_spin_type);
   }
}

/**
*\purpose  Test that no more than the maximum specified number of wheelspin pairs can be marked.
*\req    CPR-3858
*/
TEST(f360_detect_wheelspin_pairs_qualtest_unittest, max_wheelspin_pairs)
{
   /** \precond
    * Setup possible more possible wheelspin pairs than the internal structures of the algorithm allowes
    * and verify that no more than the maximum allowed wheelspin pairs gets marked in the end.
   **/
   std::vector<F360_Detection_Props_T*> wheelspin_dets;
   std::vector<F360_Detection_Props_T*> non_wheelspin_dets;

   const float32_t BASE_RANGE_RATE = 0.0F;
   const float32_t HIGH_SPREAD_RANGE_RATE = BASE_RANGE_RATE + 1.01*calibrations.k_min_wheel_spin_doppler_spread;
   const float32_t AZIMUTH = 0.0f;
   const float32_t LAT_POS = 0.0F;

   // Iteratively add more detection pairs with increasing long pos
   float32_t long_pos = 0.0F;
   uint8_t i;
   for(i = 0; i < MAX_NR_DETS_TO_MARK; i++)
   {
      long_pos += 0.9*calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair/MAX_SAVED_WHEEL_SPIN_PAIRS;
      wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE));
      wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE));
   }

   for(i = MAX_NR_DETS_TO_MARK; i < MAX_SAVED_WHEEL_SPIN_PAIRS; i++)
   {
      long_pos += 0.9*calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair /MAX_SAVED_WHEEL_SPIN_PAIRS;
      non_wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE));
      non_wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE));
   }

   long_pos += 0.9*calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair /MAX_SAVED_WHEEL_SPIN_PAIRS;
   non_wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, BASE_RANGE_RATE));
   non_wheelspin_dets.push_back(Add_Detection_Sorted(long_pos, LAT_POS, AZIMUTH, HIGH_SPREAD_RANGE_RATE));

   /** \action
    * Run the wheelspin pair detector as defined by the test group
   **/
   raw_detect_list.number_of_valid_detections = num_dets;
   Run_Action();

   /** \result
   * Verify that no more than the maximum allowed pairs get marked.
   **/
   for (F360_Detection_Props_T * det: wheelspin_dets)
   {
      CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, det->wheel_spin_type);
   }

   for (F360_Detection_Props_T * det: non_wheelspin_dets)
   {
      CHECK_EQUAL(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det->wheel_spin_type);
   }
}

/** @}*/
