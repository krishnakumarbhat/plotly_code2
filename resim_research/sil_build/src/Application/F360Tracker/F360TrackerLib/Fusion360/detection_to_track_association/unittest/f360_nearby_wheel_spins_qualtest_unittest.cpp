/** \file
   Qualification tests for CPR-3858 requirement. Tests check if detection which is located close enough to another
   detection already marked as wheel spin is marked correctly as wheel spin nearby type
*/


#include "f360_nearby_wheel_spins.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_object_track.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/** \defgroup 	f360_detect_wheelspin_nearby_qualtest
*  @{
*/

using namespace f360_variant_A;

/** \brief
   Group of tests checking if detections are correctly marked as nearby wheel spins
 */
TEST_GROUP(f360_detect_wheelspin_nearby_qualtest)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t num_dets = 0;
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T object_track = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};

   const float32_t low_floating_value = 0.00001F;

   /** \setup
   * Init calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
   }
};

/**
*\purpose  Checks if detections are correctly marked as NEARBY wheel spins
*\req      CPR-3858
*/
TEST(f360_detect_wheelspin_nearby_qualtest, f360_detect_wheelspin_nearby_qualtest__correctly_marked_as_nearby_wheel_spin)
{
   /** \precond
    * Add det above wheel-spin and within limits and sort them
    */
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_props[0].vcs_position.x = -3.0F;
   det_props[0].vcs_position.y = 2.0F;

   det_props[1].vcs_position.x = det_props[0].vcs_position.x + calibs.k_nbws_long_marking_th - low_floating_value;
   det_props[1].vcs_position.y = det_props[0].vcs_position.y + calibs.k_nbws_lat_marking_th - low_floating_value;
   det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_dets = 2;
   raw_detect_list.number_of_valid_detections = num_dets;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins().
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibs, det_props);

   /** \result
    * Detection should be marked as NEARBY WHEEL SPIN
    */
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_NEARBY, det_props[1].wheel_spin_type, "Detection is not marked as a NEARBY wheel spin");
}

/**
*\purpose  Checks if detections are correctly not marked as wheel spins
*\req      CPR-3858
*/
TEST(f360_detect_wheelspin_nearby_qualtest, f360_detect_wheelspin_nearby_qualtest__correctly_not_marked_as_wheel_spin)
{
   /** \precond
    * Add det which is out of the limits and sort them
    */
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_props[0].vcs_position.x = -3.0F;
   det_props[0].vcs_position.y = 2.0F;

   det_props[1].vcs_position.x = det_props[0].vcs_position.x + calibs.k_nbws_long_marking_th + low_floating_value;
   det_props[1].vcs_position.y = det_props[0].vcs_position.y + calibs.k_nbws_lat_marking_th - low_floating_value;
   det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   num_dets = 2;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Detect_Nearby_Wheel_Spins()
    */
   Detect_Nearby_Wheel_Spins(raw_detect_list, calibs, det_props);

   /** \result
    * Detection should not be marked as WHEEL SPIN
    */
   CHECK_EQUAL_TEXT(F360_DETECTION_WHEELSPIN_TYPE_INVALID, det_props[1].wheel_spin_type, "Detection is incorrectly marked as a wheel spin");
}

/** @}*/
