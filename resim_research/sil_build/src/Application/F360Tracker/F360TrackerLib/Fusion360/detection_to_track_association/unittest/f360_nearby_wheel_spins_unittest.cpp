/** \file
 * This file contains unit tests for content of f360_near_by_wheel_spins.cpp file
 */

#include "f360_nearby_wheel_spins.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include <cstring>
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_near_by_wheel_spins
 *  @{
 */

/** \brief
 * This group assumes only one wheel spin det
 */
TEST_GROUP(f360_near_by_wheel_spins__one_wheel_spin)
{
   F360_Calibrations_T calibrations;

   F360_Detection_Props_T det_properties[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   int num_of_dets;
   float32_t low_floating_value = 0.00001F;
   
   /** \setup
    * Init calibrations and add one wheel_spin detection
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      num_of_dets = 1;

      det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      det_properties[0].vcs_position.x = -3.0F;
      det_properties[0].vcs_position.y = 2.0F;
   }
};

/** \purpose  
 * Check if detection above wheel spin which is close enough is marked as wheel spin
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__one_det_to_mark_above)
{
   /** \precond
    * Add det above wheel-spin and within limits and sort them
    */  
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */	
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection is not marked as a wheel spin");
}


/** \purpose
 * Check if detection above a PAIR-type wheel spin cluster is not marked as nearby wheel spin when it has a close detection with similar range rate
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_above_not_marked_if_pair_cluster_and_has_close_det_with_similar_rr)
{
   /** \precond
    * Set seed detection as PAIR type; add det above wheel-spin, within limits, set f_has_close_det_with_similar_rr to true and sort them
    */
   det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_properties[1].f_has_close_det_with_similar_rr = true;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN because cluster is PAIR type and detection has a close detection with similar range rate
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection should not be marked when cluster is PAIR type and f_has_close_det_with_similar_rr is true");
}

/** \purpose
 * Check if detection above an OBJECT-type wheel spin cluster is marked as nearby wheel spin even when it has a close detection with similar range rate
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_above_marked_if_object_cluster_despite_close_rr)
{
   /** \precond
    * Keep seed detection as OBJECT type (from setup); add det above wheel-spin, within limits, set f_has_close_det_with_similar_rr to true and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_properties[1].f_has_close_det_with_similar_rr = true;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN because OBJECT-type cluster bypasses close_rr check
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection should be marked when cluster is OBJECT type despite f_has_close_det_with_similar_rr being true");
}

/** \purpose
 * Check if detection above wheel spin which is close enough is not marked as wheel spin due to being wheel spin already
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_wheelspin_already)
{
   /** \precond
    * Add det above wheel-spin and within limits and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, "Detection wheel spin type is changed as a wheel spin");
}

/** \purpose
 * Check if detection above wheel spin whether is not marked as wheel spin due to being to far away longitudinaly
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_too_far_away_longitudinaly)
{
   /** \precond
    * Add det above wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection above wheel spin whether is not marked as wheel spin due to being to far away laterally (positive)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_too_far_away_lateraly_positive)
{
   /** \precond
    * Add det above wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th + low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection above wheel spin which is not marked as wheel spin due to being to far away laterally (negative)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_too_far_away_lateraly_negative)
{
   /** \precond
    * Add det above wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y - calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if upper-lat fails but lower-lat passes so a detection below the cluster stays unmarked
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__lat_upper_out_lower_ok_not_marked)
{
    /** \precond
     * Place det below the wheel spin within long range, but beyond lower lat threshold; sort
     */
    det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + low_floating_value; // keep long distance within threshold
    det_properties[1].vcs_position.y = det_properties[0].vcs_position.y - calibrations.k_nbws_lat_marking_th - low_floating_value;
    det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
    num_of_dets++;

    raw_detect_list.number_of_valid_detections = num_of_dets;
    Sort_Detections_Vcs_Long(raw_detect_list);

    /** \action
     * Call Detect_Nearby_Wheel_Spins().
     */
    Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

    /** \result
     * Detection should remain INVALID because the first lat check fails even though the second passes
     */
    CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection should not be marked as a wheel spin");
}

/** \purpose
 * Check if upper-lat passes but lower-lat fails so a detection above the cluster stays unmarked
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__lat_upper_ok_lower_out_not_marked)
{
    /** \precond
     * Place det above the wheel spin within long range, but beyond upper lat threshold; sort
     */
    det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + low_floating_value; // keep long distance within threshold
    det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th + low_floating_value;
    det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
    num_of_dets++;

    raw_detect_list.number_of_valid_detections = num_of_dets;
    Sort_Detections_Vcs_Long(raw_detect_list);

    /** \action
     * Call Detect_Nearby_Wheel_Spins().
     */
    Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

    /** \result
     * Detection should remain INVALID because the second lat check fails even though the first passes
     */
    CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection should not be marked as a wheel spin");
}

/** \purpose
 * Check if a pre-set wheel-spin type blocks marking even when all the other "checks" pass
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__type_already_set_not_marked)
{
    /** \precond
     * Place det within both long and lat thresholds but pre-mark as WHEELSPIN type
     */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_asc_th + low_floating_value; 
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   for (int det_idx = 0; det_idx < num_of_dets; det_idx++)
   {
      raw_detect_list.detections[det_idx].processed.vcs_position_x = det_properties[det_idx].vcs_position.x;
      raw_detect_list.detections[det_idx].processed.vcs_position_y = det_properties[det_idx].vcs_position.y;
   }
    Sort_Detections_Vcs_Long(raw_detect_list);

    /** \action
     * Call Detect_Nearby_Wheel_Spins().
     */
    Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

    /** \result
     * Detection should remain as its original type because type check blocks marking
     */
    CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, "Detection should not be re-marked when type is already set");
}

/** \purpose
 * Check if detection below wheel spin, which is close enough, is marked as wheel spin
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__one_det_to_mark_below)
{
   /** \precond
    * Add det below wheel-spin, within limits, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x - calibrations.k_nbws_long_marking_th + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection is not marked as a wheel spin");
}

/** \purpose
 * Check if detection below wheel spin, which is close enough, is not marked as wheel spin due to being wheel spin already
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_wheel_spin_already)
{
   /** \precond
   * Add det below wheel-spin, mark it as wheel spin, and sort them
   */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x - calibrations.k_nbws_long_marking_th + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, "Detection wheel spin type is changed as a wheel spin");
}

/** \purpose
 * Check if detection below wheel spin is not marked as wheel spin due to being too far away longitudinaly
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_longitudinaly_too_far)
{
   /** \precond
    * Add det below wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x - calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection below wheel spin is not marked as wheel spin due to being to far away laterally (positive)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_laterally_positive_too_far)
{
   /** \precond
    * Add det below wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x - calibrations.k_nbws_long_marking_th + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th + low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection below wheel spin is not marked as wheel spin due to being to far away laterally (negative)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_laterally_negative_too_far)
{
   /** \precond
    * Add det below wheel-spin, beyond limit, and sort them
    */
   det_properties[1].vcs_position.x = det_properties[0].vcs_position.x - calibrations.k_nbws_long_marking_th + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y - calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection is not marked as wheel spin due to being out of area of interest (positive long)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_area_positive_long)
{
   /** \precond
    * Add det out of are of interest and sort them
    */
   det_properties[1].vcs_position.x = calibrations.k_nbws_max_long_pos + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection is not marked as wheel spin due to being out of area of interest (negative long)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_area_negative_long)
{
   /** \precond
    * Add det out of are of interest and sort them
    */
   det_properties[1].vcs_position.x = calibrations.k_nbws_min_long_pos - low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection is not marked as wheel spin due to being out of area of interest (positive lat)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_area_positive_lat)
{
   /** \precond
    * Add det out of are of interest and sort them
    */
   det_properties[1].vcs_position.x = det_properties[1].vcs_position.y;
   det_properties[1].vcs_position.y = calibrations.k_nbws_max_lat_pos + low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detection below wheel spin is not marked as wheel spin due to being out of area of interest (negative lat)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_area_negative_lat)
{
   /** \precond
    * Add det out of are of interest and sort them
    */
   det_properties[1].vcs_position.x = det_properties[1].vcs_position.y;
   det_properties[1].vcs_position.y = calibrations.k_nbws_min_lat_pos - low_floating_value;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}

/** \purpose
 * Check if detections are not marked as wheel spin due to being out of area of interest
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__dets_not_marked_if_out_of_area)
{
   /** \precond
    * Add 2 dets out of are of interest and sort them
    */
   det_properties[1].vcs_position.x = calibrations.k_nbws_max_long_pos + low_floating_value;
   det_properties[1].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   det_properties[2].vcs_position.x = calibrations.k_nbws_max_long_pos + 2 * low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should not be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[1].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection is marked as wheel spin as a wheel spin");
}


/** \purpose
 * Check if nothing happend if there is no dets to check
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__one_wheel_spin, Detect_Nearby_Wheel_Spins__no_dets_to_check)
{
   /** \precond
    * Sort det 
    */
   F360_Detection_Props_T ref_det_properties[MAX_NUMBER_OF_DETECTIONS];
   Sort_Detections_Vcs_Long(raw_detect_list);
   memcpy(&ref_det_properties, &det_properties, sizeof(det_properties));

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * This part of code should be reached
    */
   CHECK_TRUE(0 == memcmp(&ref_det_properties, &det_properties, sizeof(det_properties)));
}
/** @}*/




/** \defgroup  f360_near_by_wheel_spins__wheel_spin_cluster
 *  @{
 */

 /** \brief
  * Group used for checking if clustering is done properly.
  */
TEST_GROUP(f360_near_by_wheel_spins__wheel_spin_cluster)
{
   F360_Calibrations_T calibrations;

   F360_Detection_Props_T det_properties[MAX_NUMBER_OF_DETECTIONS];
   int num_of_dets;
   float32_t low_floating_value = 0.00001F;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};

   /** \setup
    * Init calibrations and add two wheel_spin detections
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      num_of_dets = 2;

      det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      det_properties[0].vcs_position.x = -3.0F;
      det_properties[0].vcs_position.y = 2.0F;

      det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
      det_properties[1].vcs_position.x = det_properties[0].vcs_position.x + calibrations.k_nbws_long_asc_th - low_floating_value;
      det_properties[1].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_asc_th - low_floating_value;;
   }
};

/** \purpose
 * Check if detection, which is within wheel spin cluster, is marked as wheel spin
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_marked_within_cluster)
{
   /** \precond
    * Add det within limits wheel spin cluster (long sequence 0 - 2 - 1)
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection is not marked as a wheel spin");
}

/** \purpose
 * Check if all detections between cluster boundaries are marked via the internal traversal loop
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__internal_detections_all_marked)
{
   /** \precond
    * Add two detections inside cluster bounds (long sequence 0 - 2 - 3 - 1)
    */
   det_properties[2].vcs_position.x = det_properties[0].vcs_position.x + (calibrations.k_nbws_long_asc_th * 0.25F);
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   det_properties[3].vcs_position.x = det_properties[1].vcs_position.x - (calibrations.k_nbws_long_asc_th * 0.25F);
   det_properties[3].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   for (int32_t det_idx = 0; det_idx < num_of_dets; det_idx++)
   {
      raw_detect_list.detections[det_idx].processed.vcs_position_x = det_properties[det_idx].vcs_position.x;
      raw_detect_list.detections[det_idx].processed.vcs_position_y = det_properties[det_idx].vcs_position.y;
   }

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Both internal detections should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "First internal detection is not marked as a wheel spin");
   CHECK_TRUE_TEXT(det_properties[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Second internal detection is not marked as a wheel spin");
}

/** \purpose
 * Ensure detections below a cluster are marked until the longitudinal break condition is met
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__below_cluster_stops_at_long_break)
{
   /** \precond
    * Add two detections below the cluster: one within long threshold, one beyond (order: far, near, cluster det0, det1)
    */
   const float32_t dx_close = calibrations.k_nbws_long_marking_th * 0.5F;
   const float32_t dx_far = calibrations.k_nbws_long_marking_th * 1.5F;

   det_properties[2].vcs_position.x = det_properties[0].vcs_position.x - dx_close;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + 0.15F; // So it will be changed to NEARBY
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   det_properties[3].vcs_position.x = det_properties[0].vcs_position.x - dx_far;
   det_properties[3].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   for (int32_t det_idx = 0; det_idx < num_of_dets; det_idx++)
   {
      raw_detect_list.detections[det_idx].processed.vcs_position_x = det_properties[det_idx].vcs_position.x;
      raw_detect_list.detections[det_idx].processed.vcs_position_y = det_properties[det_idx].vcs_position.y;
   }

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * The nearer detection is marked NEARBY; traversal stops before marking the farther one
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Near-below detection is not marked as a wheel spin");
   CHECK_TRUE_TEXT(det_properties[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Traversal did not stop at long break condition");
}

/** \purpose
 * Check if detection, which is within wheel spin cluster, is not marked as wheel spin due to beeing too far laterally (positive)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_cluster_laterally_positive)
{
   /** \precond
    * Add det within limits wheel spin cluster (long sequence 0 - 2 - 1)
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th + low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type != F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection is marked as a wheel spin");
}

/** \purpose
 * Check if detection, which is within wheel spin cluster, is not marked as wheel spin due to beeing too far laterally (negative)
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_not_marked_if_out_of_cluster_laterally_negative)
{
   /** \precond
    * Add det within limits wheel spin cluster (long sequence 0 - 2 - 1)
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y - calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type != F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection is marked as a wheel spin");
}

/** \purpose
 * Check if detection, which is within wheel spin cluster, is not marked as wheel spin due to already wheel spin
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_not_marked_if_already_wheel_spin)
{
   /** \precond
    * Add det within limits wheel spin cluster (long sequence 0 - 2 - 1)
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS, "Detection's wheel spin type is changed");
}

/** \purpose
 * Check if detection detection is not marked due to reached maxiumum number of clusters
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_not_marked_cluster_number_overflow)
{
   /** \precond
    * Add det within limits wheel spin cluster
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x + 2.0F * calibrations.k_nbws_long_asc_th;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;

   det_properties[3].vcs_position.x = det_properties[2].vcs_position.x + low_floating_value;
   det_properties[3].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   calibrations.k_nbws_max_num_clusters = 1;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID, "Detection should be not marked as wheel spin");
}

/** \purpose
 * Check if detection detection is marked (counter test for cluster number overflowin
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__det_marked_cluster_number_not_overflow)
{
   /** \precond
    * Add det within limits wheel spin cluster
    */
   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x + 2.0F * calibrations.k_nbws_long_asc_th;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   num_of_dets++;
   
   det_properties[3].vcs_position.x = det_properties[2].vcs_position.x + low_floating_value;
   det_properties[3].vcs_position.y = det_properties[0].vcs_position.y;
   det_properties[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_of_dets++;

   calibrations.k_nbws_max_num_clusters = 2;
   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_TRUE_TEXT(det_properties[3].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection should be marked as wheel spin");
}

/** \purpose
 * Verify that cluster starting with PAIR-type detection gets promoted to OBJECT when second detection is OBJECT type, allowing marking despite close_rr flag
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__cluster_pair_then_object_promotes_to_object)
{
   /** \precond
    * Set first cluster det as PAIR, second as OBJECT; add nearby det with f_has_close_det_with_similar_rr true and sort them
    */
   det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;

   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_properties[2].f_has_close_det_with_similar_rr = true;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY because OBJECT type in cluster promotes ws_type, bypassing close_rr guard
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection should be marked as NEARBY when cluster is promoted to OBJECT type");
}

/** \purpose
 * Verify that cluster starting with OBJECT-type detection retains OBJECT when second detection is PAIR type, allowing marking despite close_rr flag
 * \req
 * NA.
 */
TEST(f360_near_by_wheel_spins__wheel_spin_cluster, Detect_Nearby_Wheel_Spins__cluster_object_then_pair_stays_object)
{
   /** \precond
    * Set first cluster det as OBJECT, second as PAIR; add nearby det with f_has_close_det_with_similar_rr true and sort them
    */
   det_properties[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_properties[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;

   det_properties[2].vcs_position.x = det_properties[1].vcs_position.x + calibrations.k_nbws_long_marking_th - low_floating_value;
   det_properties[2].vcs_position.y = det_properties[0].vcs_position.y + calibrations.k_nbws_lat_marking_th - low_floating_value;
   det_properties[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_properties[2].f_has_close_det_with_similar_rr = true;
   num_of_dets++;

   raw_detect_list.number_of_valid_detections = num_of_dets;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibrations, det_properties);

   /** \result
    * Detection should be marked as NEARBY because OBJECT type dominates and is retained from the first detection
    */
   CHECK_TRUE_TEXT(det_properties[2].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_NEARBY, "Detection should be marked as NEARBY when cluster retains OBJECT type");
}

/** @}*/
