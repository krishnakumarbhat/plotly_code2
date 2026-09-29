/** \file
 * This file contains unit tests for content of f360_identify_and_flag_internal_reflections.cpp file
 */

#include "f360_identify_and_flag_internal_reflections.h"
#include <CppUTest/TestHarness.h>

#include "f360_math.h"
#include <limits>

using namespace f360_variant_A;

/** \defgroup  f360_identify_and_flag_internal_reflections__low_level_functions
 *  @{
 */

/** \brief
 * This test group verifies the functionality of functions inside f360_identify_and_flag_internal_reflections.cpp
 * that are ran at least once per sensor per tracker iteration.
 */
TEST_GROUP(f360_identify_and_flag_internal_reflections__low_level_functions)
{
   /** \setup
    * Initialize variables used in function calls.
    * In TEST_SETUP(), the following is done;
    * - Reset all entries in all buffer slots to 0.
    * - Reset used entries in det1 and det2 to 0
    * Setup helper function Check_If_Det_And_Buffer_Slot_Are_Identical() that compares a detection to a buffer slot.
    * If they are identical, returns true. Otherwise, false.
    */

   F360_Internal_Reflections_Calib_T int_ref_calib = {};
   Internal_Reflection_Buffer_Slot int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   rspp_variant_A::RSPP_Detection_T det1 = {};
   rspp_variant_A::RSPP_Detection_T det2 = {};
   const rspp_variant_A::RSPP_Detection_T empty_det = {};

   const float FLT_EPS = std::numeric_limits<float>::epsilon();

   TEST_SETUP()
   {

      for (int i_det = 0; i_det < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_det++)
      {
         int_ref_buffer[i_det].age = 0U;
         int_ref_buffer[i_det].occurrence_count = 0U;
         int_ref_buffer[i_det].range = 0.0F;
         int_ref_buffer[i_det].azimuth = 0.0F;
         int_ref_buffer[i_det].rcs = 0.0F;
         int_ref_buffer[i_det].f_updated_this_cycle = false;
         int_ref_buffer[i_det].f_classified_as_internal_reflection = false;
      }

      det1.raw.range = 0.0F;
      det1.raw.range_rate = 0.0F;
      det1.raw.azimuth = 0.0F;
      det1.raw.rcs = 0.0F;

      det2.raw.range = 0.0F;
      det2.raw.range_rate = 0.0F;
      det2.raw.azimuth = 0.0F;
      det2.raw.rcs = 0.0F;

   }

   bool Check_If_Det_And_Buffer_Slot_Are_Identical(
         const rspp_variant_A::RSPP_Detection_T & det,
         const Internal_Reflection_Buffer_Slot & buffer_slot)
   {
      return ((std::abs(det.raw.range - buffer_slot.range) < FLT_EPS) &&
              (std::abs(det.raw.azimuth - buffer_slot.azimuth) < FLT_EPS) &&
              (std::abs(det.raw.rcs - buffer_slot.rcs) < FLT_EPS));
   }
};


/** \purpose
 *  Verify detections are added to the first unused buffer slot when adding
 *  new detections to the buffer when it is completely empty.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_To_Unused_Reflection_Buffer_Slot__add_detection_to_empty_buffer)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Setup det1's range, azimuth, and rcs to arbitrary values.
    */
   det1.raw.range = 1.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 3.0F;

   /** \action
    * Call Add_Det_To_Unused_Reflection_Buffer_Slot().
    */
   Add_Det_To_Unused_Reflection_Buffer_Slot(det1, int_ref_buffer);

   /** \result
    * - Verify the detection was added to the expected buffer slot
    * - Verify remaining buffer slots were unaffected
    */
   CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(det1, int_ref_buffer[0]));
   for (int i = 1; i < INTERNAL_REFLECTIONS_BUFFER_SIZE; i++)
   {
      CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(empty_det, int_ref_buffer[i]));
   }
}

/** \purpose
 *  Verify detections are added to the first unused buffer slot when adding
 *  new detections to the buffer when some slots are already used.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_To_Unused_Reflection_Buffer_Slot__add_detection_to_partially_full_buffer)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Setup det1's range, azimuth, and rcs to arbitrary values that are not all 0.
    * - Set buffer slot 1's occurence_count to 1 to indicate it is used.
    */

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].occurrence_count = 1U;

   /** \action
    * Add det2 to buffer by calling Add_Det_To_Unused_Reflection_Buffer_Slot().
    */
   Add_Det_To_Unused_Reflection_Buffer_Slot(det1, int_ref_buffer);

   /** \result
    * - Verify the detection was added to the expected buffer slot
    * - Verify the other buffer slots were unaffected.
    */
   CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(empty_det, int_ref_buffer[0]));
   CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(det1, int_ref_buffer[1]));
   for (int i = 2; i < INTERNAL_REFLECTIONS_BUFFER_SIZE; i++)
   {
      CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(empty_det, int_ref_buffer[i]));
   }
}

/** \purpose
 *  Verify new detections are not added to the buffer when it is completely full.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_To_Unused_Reflection_Buffer_Slot__add_detection_to_full_buffer)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Setup det1's range, azimuth, and rcs to arbitrary values.
    * - Set the occurence_count of all buffer slots to >0 to indicate they are used.
    */
   det1.raw.range = 1.0F;
   det1.raw.azimuth = 1.0F;
   det1.raw.rcs = 1.0F;

   for (int i = 0; i < INTERNAL_REFLECTIONS_BUFFER_SIZE; i++)
   {
      int_ref_buffer[i].occurrence_count = 1U;
   }

   /** \action
    * Call Add_Det_To_Unused_Reflection_Buffer_Slot().
    */
   Add_Det_To_Unused_Reflection_Buffer_Slot(det1, int_ref_buffer);

   /** \result
    * - Verify that the detection was not added and that all buffer slots were unaffected
    */
   for (int i = 0; i < INTERNAL_REFLECTIONS_BUFFER_SIZE; i++)
   {
      CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(empty_det, int_ref_buffer[i]));
   }
}


/** \purpose
 *  Verify all buffer slot entries are updated as expected when adding a new detection.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_To_Unused_Reflection_Buffer_Slot__verify_buffer_slot_properties_are_updated_as_expected)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Setup det1's range, azimuth, and rcs to arbitrary values.
    */

   det1.raw.range = 1.0F;
   det1.raw.azimuth = 1.0F;
   det1.raw.rcs = 1.0F;

   /** \action
    * Call Add_Det_To_Unused_Reflection_Buffer_Slot().
    */
   Add_Det_To_Unused_Reflection_Buffer_Slot(det1, int_ref_buffer);

   /** \result
    * Verify all entries of the buffer slot the detection was expected to be added to are updated as expected.
    */
   CHECK_TRUE(Check_If_Det_And_Buffer_Slot_Are_Identical(det1, int_ref_buffer[0]));
   CHECK_TRUE(int_ref_buffer[0].age == 1U);
   CHECK_TRUE(int_ref_buffer[0].occurrence_count == 1U);
}


/** \purpose
 *  Verify a buffer slot is not classified as internal reflection when the slot does not have the sufficient age to be evaluated.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Classify_Internal_Detection_Buffer_Slot__verify_slot_is_not_flagged_when_age_is_low)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Set relevant int_ref_calib parameters to reasonable values
    * - Set buffer slot 1's age to be smaller than sensor.internal_reflections_age_threshold.
    * - Set buffer slot 1's occurrence_count such that occurrence_rate = occurence_count / age is larger than int_ref_calib.occurrence_threshold.
    */

   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   int_ref_buffer[0].age = 199U;
   int_ref_buffer[0].occurrence_count = 199U;

   /** \action
    * Call function Classify_Internal_Detection_Buffer_Slot().
    */
   bool result = Classify_Internal_Detection_Buffer_Slot(int_ref_buffer[0], int_ref_calib);

   /** \result
    * Verify the buffer slot is not classified as internal reflection.
    */
   CHECK_FALSE_TEXT(result, "The buffer slot was flagged as internal reflection when it was expected not to.");
}

/** \purpose
 *  Verify a buffer slot is not classified as internal reflection when the slot has the sufficient age to be evaluated but not the required occurrence rate.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Classify_Internal_Detection_Buffer_Slot__verify_slot_is_not_flagged_when_occurrence_rate_is_low)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Set relevant int_ref_calib parameters to reasonable values
    * - Set buffer slot 1's age to be equal to int_ref_calib.age_threshold.
    * - Set buffer slot 1's occurrence_count such that the occurrence_rate = occurence_count / age is equal to int_ref_calib.occurrence_threshold.
    */

   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   int_ref_buffer[0].age = 200U;
   int_ref_buffer[0].occurrence_count = 19U;

   /** \action
    * Call function Classify_Internal_Detection_Buffer_Slot().
    */
   bool result = Classify_Internal_Detection_Buffer_Slot(int_ref_buffer[0], int_ref_calib);

   /** \result
    * Verify the buffer slot is not classified as internal reflection.
    */
   CHECK_FALSE_TEXT(result, "The buffer slot was flagged as internal reflection when it was expected not to.");
}


/** \purpose
 *  Verify a buffer slot is classified as internal reflection when the slot has the sufficient age to be evaluated and the required occurrence rate.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Classify_Internal_Detection_Buffer_Slot__verify_slot_is_flagged_when_criteria_is_met)
{
   /** \precond
    * - Reset all entries in all buffer slots to 0.
    * - Set relevant int_ref_calib parameters to reasonable values
    * - Set buffer slot 1's age to be equal to int_ref_calib.age_threshold.
    * - Set buffer slot 1's occurrence_count such that the occurrence_rate = occurence_count / age is larger than int_ref_calib.occurrence_threshold.
    */

   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   int_ref_buffer[0].age = 200U;
   int_ref_buffer[0].occurrence_count = 21U;

   /** \action
    * Call function Classify_Internal_Detection_Buffer_Slot().
    */
   bool result = Classify_Internal_Detection_Buffer_Slot(int_ref_buffer[0], int_ref_calib);

   /** \result
    * Verify the buffer slot is classified as internal reflection.
    */
   CHECK_TRUE_TEXT(result, "The buffer slot was not flagged as internal reflection when it was expected to.");
}


/** \purpose
 *  Verify that when adding a detection to a buffer slot, the buffer slot gets updated to contain
 *  the average of all detections that have been added to the slot.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_to_Existing_Internal_Reflection_Buffer_Slot__verify_slot_detection_info_is_updated_as_expected)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set buffer slot 1's occurrence count to 1 such that only 1 detection is stored in the buffer
    * Set buffer slot 1's age to 1 to something < int_ref_calib.age_threshold
    * Set buffer slot 1's detection to arbitrary values
    * Set det1's range, range-rate, and rcs arbitrarily
    */

   int_ref_calib.age_threshold = 200U;

   int_ref_buffer[0].occurrence_count = 1U;
   int_ref_buffer[0].age = 1;

   int_ref_buffer[0].range = 1.0F;
   int_ref_buffer[0].azimuth = 1.0F;
   int_ref_buffer[0].rcs = 1.0F;
   int_ref_buffer[0].f_classified_as_internal_reflection = true;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   /** \action
    * Call function Add_Det_to_Existing_Internal_Reflection_Buffer_Slot().
    */
   Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(int_ref_calib, det1, int_ref_buffer[0]);

   /** \result
    * - Verify the buffer slot's range, azimuth, and rcs is updated as expected
    * - Verify the slot is not classified as internal reflection as it is not old enough to be classified.
    */
   DOUBLES_EQUAL_TEXT(1.5F, int_ref_buffer[0].range, FLT_EPS, "The buffer slot detection info was not computed as the average between the stored buffer info and the input detection.");
   DOUBLES_EQUAL_TEXT(1.5F, int_ref_buffer[0].azimuth, FLT_EPS,"The buffer slot detection info was not computed as the average between the stored buffer info and the input detection.");
   DOUBLES_EQUAL_TEXT(1.5F, int_ref_buffer[0].rcs, FLT_EPS, "The buffer slot detection info was not computed as the average between the stored buffer info and the input detection.");
   CHECK_FALSE(int_ref_buffer[0].f_classified_as_internal_reflection);
}

/** \purpose
 * Verify a buffer slot's occurrence count gets incremented during its first update of 1 tracker iteration.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_to_Existing_Internal_Reflection_Buffer_Slot__verify_slot_occurence_count_is_incremented)
{
   /** \precond
    * Set int_ref_calib parameters to some values (irrelevant to test but needed as input parameter)
    * Set det1 with some arbitrary values (irrelevant to test but needed as input parameter)
    * Set buffer slot 1's occurrence_count to arbitrary number that is less than UINT16_MAX
    * Set buffer slot 1's f_updated_this_cycle to false
    */

   int_ref_buffer[0].occurrence_count = 6U;
   int_ref_buffer[0].f_updated_this_cycle = false;

   /** \action
    * Call function Add_Det_to_Existing_Internal_Reflection_Buffer_Slot().
    */
   Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(int_ref_calib, det1, int_ref_buffer[0]);

   /** \result
    * Verify the buffer slot's occurence_count is incremented.
    * Verify the buffer slot is flagged to indicate it has been updated in this cycle.
    */

   CHECK_TRUE(int_ref_buffer[0].occurrence_count == 7U);
   CHECK_TRUE(int_ref_buffer[0].f_updated_this_cycle);
}

/** \purpose
 *   Verify a buffer slot's occurrence count does not get incremented after its first update of 1 tracker iteration.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_to_Existing_Internal_Reflection_Buffer_Slot__verify_slots_occ_count_is_not_incremented__already_updated)
{
   /** \precond
    * Set int_ref_calib parameters to some values (irrelevant to test but needed as input parameter)
    * Set det1 with some arbitrary values (irrelevant to test but needed as input parameter)
    * Set buffer slot 1's occurrence_count to arbitrary number that is less than UINT16_MAX
    * Set buffer slot 1's f_updated_this_cycle to true to indicate it has already been updated this cycle
    */

   int_ref_buffer[0].occurrence_count = 6U;
   int_ref_buffer[0].f_updated_this_cycle = true;

   /** \action
    * Call function Add_Det_to_Existing_Internal_Reflection_Buffer_Slot().
    */
   Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(int_ref_calib, det1, int_ref_buffer[0]);

   /** \result
    * Verify the buffer slot's occurence_count is not incremented.
    * Verify the buffer slot keeps its flag to indicate it has been updated in this cycle.
    */

   CHECK_TRUE(int_ref_buffer[0].occurrence_count == 6U);
   CHECK_TRUE(int_ref_buffer[0].f_updated_this_cycle);
}

/** \purpose
 * Verify a buffer slot's occurrence count does not get incremented during its first update of 1 tracker
 * iteration when it has reached saturation.
 * \req
 * NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_to_Existing_Internal_Reflection_Buffer_Slot__verify_slots_occ_count_is_not_incremented__satured)
{
   /** \precond
    * Set int_ref_calib parameters to some values (irrelevant to test but needed as input parameter)
    * Set det1 with some arbitrary values (irrelevant to test but needed as input parameter)
    * Set buffer.occurrence_count to UINT32_MAX
    * Set buffer.f_updated_this_cycle to false
    */

   int_ref_buffer[0].occurrence_count = 4294967295U;
   int_ref_buffer[0].f_updated_this_cycle = false;

   /** \action
    * Call function Add_Det_to_Existing_Internal_Reflection_Buffer_Slot().
    */
   Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(int_ref_calib, det1, int_ref_buffer[0]);

   /** \result
    *  Verify the buffer slot occurence_count is not incremented.
    *  Verify the buffer slot keeps its flag to indicate it has been updated in this cycle.
    */
   CHECK_TRUE(int_ref_buffer[0].occurrence_count == 4294967295U);
   CHECK_TRUE(int_ref_buffer[0].f_updated_this_cycle);
}


/** \purpose
 * Verify the buffer slot gets re-classified if it should be flagged as an internal reflection when a detection is added to it.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Add_Det_to_Existing_Internal_Reflection_Buffer_Slot__verify_slot_is_classified_as_internal_reflection)
{
   /** \precond
    * Set det1 with some arbitrary values (irrelevant to test but needed as input parameter)
    * Set relevant int_ref_calib parameters to some values
    * Set the buffer slot age >= int_ref_calib.age_threshold
    * Set the buffer slot occurence_count such that occurence_rate = occurence_count/age > int_ref_calib.occurrence_threshold
    * Set the buffer slot flag f_classified_as_internal_reflection to false
    */
   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   int_ref_buffer[0].age = 200U;
   int_ref_buffer[0].occurrence_count = 200U;

   int_ref_buffer[0].f_classified_as_internal_reflection = false;

   /** \action
    * Call function Add_Det_to_Existing_Internal_Reflection_Buffer_Slot().
    */
   Add_Det_to_Existing_Internal_Reflection_Buffer_Slot(int_ref_calib, det1, int_ref_buffer[0]);

   /** \result
    * Verify the buffer slot is classified as internal reflection.
    */
   CHECK_TRUE(int_ref_buffer[0].f_classified_as_internal_reflection);
}


/** \purpose
 * Verify that a detection that is identical to an existing valid buffer slot, is flagged as a match.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Does_Det_Match_Buffer_Slot__perfect_match)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to some arbitrary values
    * Set the buffer slot detection information to be identical with det1.
    * Set the buffer slot's age to be larger than 0 to indicate it's a valid slot.
    */

   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].range = det1.raw.range;
   int_ref_buffer[0].azimuth = det1.raw.azimuth;
   int_ref_buffer[0].rcs = det1.raw.rcs;

   int_ref_buffer[0].age = 1U;

   /** \action
    * Call function Does_Det_Match_Buffer_Slot().
    */
   bool result = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[0], int_ref_calib);

   /** \result
    * Verify the buffer slot is flagged as a match.
    */
   CHECK_TRUE(result);
}


/** \purpose
 * Verify a detection is not flagged as a match against a buffer slot as the slot is unused.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Does_Det_Match_Buffer_Slot__unused_slot)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to some arbitrary values
    * Set the buffer slot to be identical with det1.
    * Set the buffer slot's age to be 0 to indicate it's not a valid slot.
    */

   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].age = 0U;

   int_ref_buffer[0].range = det1.raw.range;
   int_ref_buffer[0].azimuth = det1.raw.azimuth;
   int_ref_buffer[0].rcs = det1.raw.rcs;

   /** \action
    * Call function Does_Det_Match_Buffer_Slot().
    */
   bool result = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[0], int_ref_calib);

   /** \result
    * Verify the buffer slot is not flagged as a match.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify a detection is matched as expected against a buffer slot when the only thing that differs is its range.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Does_Det_Match_Buffer_Slot__range_difference)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to some arbitrary values
    * Set buffer slot 0, 1, 2, and 3 to be identical with det1 except for their range information.
    * Set buffer slot 0's range to det1.range + 1.001F * calibration range tolerance.
    * Set buffer slot 1's range to det1.range - 1.001F * calibration range tolerance.
    * Set buffer slot 2's range to det1.range + 0.999F * calibration range tolerance.
    * Set buffer slot 3's range to det1.range - 0.999F * calibration range tolerance.
    * Set the relevant buffer slots age to be >0 to indicate they're valid entries.
    */

   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].range = det1.raw.range + 1.001F * int_ref_calib.range_tolerance;
   int_ref_buffer[0].azimuth = det1.raw.azimuth;
   int_ref_buffer[0].rcs = det1.raw.rcs;
   int_ref_buffer[1].range = det1.raw.range - 1.001F * int_ref_calib.range_tolerance;
   int_ref_buffer[1].azimuth = det1.raw.azimuth;
   int_ref_buffer[1].rcs = det1.raw.rcs;

   int_ref_buffer[2].range = det1.raw.range + 0.999F * int_ref_calib.range_tolerance;
   int_ref_buffer[2].azimuth = det1.raw.azimuth;
   int_ref_buffer[2].rcs = det1.raw.rcs;
   int_ref_buffer[3].range = det1.raw.range - 0.999F * int_ref_calib.range_tolerance;
   int_ref_buffer[3].azimuth = det1.raw.azimuth;
   int_ref_buffer[3].rcs = det1.raw.rcs;

   int_ref_buffer[0].age = 1U;
   int_ref_buffer[1].age = 1U;
   int_ref_buffer[2].age = 1U;
   int_ref_buffer[3].age = 1U;

   /** \action
    * Call function Does_Det_Match_Buffer_Slot().
    */
   bool result_1 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[0], int_ref_calib);
   bool result_2 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[1], int_ref_calib);
   bool result_3 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[2], int_ref_calib);
   bool result_4 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[3], int_ref_calib);

   /** \result
    * Verify the buffer slots are flagged as a match as expected.
    */
   CHECK_FALSE(result_1);
   CHECK_FALSE(result_2);
   CHECK_TRUE(result_3);
   CHECK_TRUE(result_4);
}


/** \purpose
 * Verify a detection is matched as expected against a buffer slot when the only thing that differs is its azimuth.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Does_Det_Match_Buffer_Slot__azimuth_difference)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to some arbitrary values
    * Set buffer slot 0, 1, 2, and 3 to be identical with det1 except for their azimuth information.
    * Set buffer slot 0's range to det1.azimuth + 1.001F * calibration azimuth tolerance.
    * Set buffer slot 1's range to det1.azimuth - 1.001F * calibration azimuth tolerance.
    * Set buffer slot 2's range to det1.azimuth + 0.999F * calibration azimuth tolerance.
    * Set buffer slot 3's range to det1.azimuth - 0.999F * calibration azimuth tolerance.
    * Set the relevant buffer slots age to be >0 to indicate they're valid entries.
    */
   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].range = det1.raw.range;
   int_ref_buffer[0].azimuth = det1.raw.azimuth + 1.001F * int_ref_calib.azimuth_tolerance;
   int_ref_buffer[0].rcs = det1.raw.rcs;
   int_ref_buffer[1].range = det1.raw.range;
   int_ref_buffer[1].azimuth = det1.raw.azimuth - 1.001F * int_ref_calib.azimuth_tolerance;
   int_ref_buffer[1].rcs = det1.raw.rcs;

   int_ref_buffer[2].range = det1.raw.range;
   int_ref_buffer[2].azimuth = det1.raw.azimuth + 0.999F * int_ref_calib.azimuth_tolerance;
   int_ref_buffer[2].rcs = det1.raw.rcs;
   int_ref_buffer[3].range = det1.raw.range;
   int_ref_buffer[3].azimuth = det1.raw.azimuth - 0.999F * int_ref_calib.azimuth_tolerance;
   int_ref_buffer[3].rcs = det1.raw.rcs;

   int_ref_buffer[0].age = 1U;
   int_ref_buffer[1].age = 1U;
   int_ref_buffer[2].age = 1U;
   int_ref_buffer[3].age = 1U;

   /** \action
    * Call function Does_Det_Match_Buffer_Slot().
    */
   bool result_1 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[0], int_ref_calib);
   bool result_2 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[1], int_ref_calib);
   bool result_3 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[2], int_ref_calib);
   bool result_4 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[3], int_ref_calib);

   /** \result
    * Verify the buffer slots are flagged as a match as expected.
    */
   CHECK_FALSE(result_1);
   CHECK_FALSE(result_2);
   CHECK_TRUE(result_3);
   CHECK_TRUE(result_4);
}


/** \purpose
 * Verify a detection is matched as expected against a buffer slot when the only thing that differs is its rcs.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Does_Det_Match_Buffer_Slot__rcs_difference)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to some arbitrary values
    * Set buffer slot 0, 1, 2, and 3 to be identical with det1 except for their azimuth information.
    * Set buffer slot 0's range to det1.rcs + 1.001F * calibration rcs tolerance.
    * Set buffer slot 1's range to det1.rcs - 1.001F * calibration rcs tolerance.
    * Set buffer slot 2's range to det1.rcs + 0.999F * calibration rcs tolerance.
    * Set buffer slot 3's range to det1.rcs - 0.999F * calibration rcs tolerance.
    * Set the relevant buffer slots age to be >0 to indicate they're valid entries.
    */
   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].range = det1.raw.range;
   int_ref_buffer[0].azimuth = det1.raw.azimuth;
   int_ref_buffer[0].rcs = det1.raw.rcs + 1.001F * int_ref_calib.rcs_tolerance;
   int_ref_buffer[1].range = det1.raw.range;
   int_ref_buffer[1].azimuth = det1.raw.azimuth;
   int_ref_buffer[1].rcs = det1.raw.rcs - 1.001F * int_ref_calib.rcs_tolerance;

   int_ref_buffer[2].range = det1.raw.range;
   int_ref_buffer[2].azimuth = det1.raw.azimuth;
   int_ref_buffer[2].rcs = det1.raw.rcs + 0.999F * int_ref_calib.rcs_tolerance;
   int_ref_buffer[3].range = det1.raw.range;
   int_ref_buffer[3].azimuth = det1.raw.azimuth;
   int_ref_buffer[3].rcs = det1.raw.rcs - 0.999F * int_ref_calib.rcs_tolerance;

   int_ref_buffer[0].age = 1U;
   int_ref_buffer[1].age = 1U;
   int_ref_buffer[2].age = 1U;
   int_ref_buffer[3].age = 1U;

   /** \action
    * Call function Does_Det_Match_Buffer_Slot().
    */
   bool result_1 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[0], int_ref_calib);
   bool result_2 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[1], int_ref_calib);
   bool result_3 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[2], int_ref_calib);
   bool result_4 = Does_Det_Match_Buffer_Slot(det1, int_ref_buffer[3], int_ref_calib);

   /** \result
    * Verify the buffer slots are flagged as a match as expected.
    */
   CHECK_FALSE(result_1);
   CHECK_FALSE(result_2);
   CHECK_TRUE(result_3);
   CHECK_TRUE(result_4);
}


/** \purpose
 * Verify buffer slots are correctly matched against a detection on a arbitrary buffer slot
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Find_Buffer_Slot_Matching_Detection__match_at_first_buf_idx)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values > 0
    * Reset all entries in all buffer slots to 0.
    * Set det1 to some arbitrary values
    * Set int_ref_buffer[0] to be a valid slot that's identical to det1.
    */

   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[0].age = 1U;
   int_ref_buffer[0].range = det1.raw.range;
   int_ref_buffer[0].azimuth = det1.raw.azimuth;
   int_ref_buffer[0].rcs = det1.raw.rcs;

   /** \action
    * Call function Find_Buffer_Slot_Matching_Detection().
    */
   int result = Find_Buffer_Slot_Matching_Detection(int_ref_buffer, det1, int_ref_calib);

   /** \result
    * Verify that buffer index 0 was returned.
    */
   CHECK_EQUAL_TEXT(0, result, "An incorrect buffer index was returned from function.");
}

/** \purpose
 * Verify all buffer slots are checked for a potential match against a detection.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Find_Buffer_Slot_Matching_Detection__match_at_last_buf_idx)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values > 0
    * Reset all entries in all buffer slots to 0.
    * Set det1 to some arbitrary values
    * Set int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE-1] to be a valid slot that's identical to det1.
    */
   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE-1].age = 1U;
   int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE-1].range = det1.raw.range;
   int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE-1].azimuth = det1.raw.azimuth;
   int_ref_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE-1].rcs = det1.raw.rcs;

   /** \action
    * Call function Find_Buffer_Slot_Matching_Detection().
    */
   int result = Find_Buffer_Slot_Matching_Detection(int_ref_buffer, det1, int_ref_calib);

   /** \result
    * Verify that the last buffer index was returned.
    */
   CHECK_EQUAL_TEXT(INTERNAL_REFLECTIONS_BUFFER_SIZE-1, result, "An incorrect buffer index was returned from function.");
}

/** \purpose
 * Verify all buffer slots are checked for a match against the input detection, when no match is found, buffer index -1 is returned.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Find_Buffer_Slot_Matching_Detection__no_match)
{
   /** \precond
    * Set relevant int_ref_calib to some values > 0
    * Reset all entries in all buffer slots to 0.
    * Set det1 to some arbitrary values > 0
    */
   int_ref_calib.rcs_tolerance = 0.1F;
   int_ref_calib.azimuth_tolerance = 0.1F;
   int_ref_calib.range_tolerance = 0.1F;

   det1.raw.range = 2.0F;
   det1.raw.azimuth = 2.0F;
   det1.raw.rcs = 2.0F;

   /** \action
    * Call function Find_Buffer_Slot_Matching_Detection().
    */
   int result = Find_Buffer_Slot_Matching_Detection(int_ref_buffer, det1, int_ref_calib);

   /** \result
    * Verify that -1 is returned to indicate there was no match.
    */
   CHECK_EQUAL_TEXT(-1, result, "An incorrect buffer index was returned from function.");
}


/** \purpose
 * Verify that a detection is flagged as relevant for the buffer according to calibration parameters
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Is_Detection_Relevant_For_Internal_Reflection_Buffer__relevant)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 and det2 to have range, rcs, and range-rate on the inside limit of what's considered to be relevant for the buffer.
    */
   int_ref_calib.max_abs_range_rate = 0.1F;
   int_ref_calib.range_max = 5.0F;
   int_ref_calib.rcs_max = -15.0F;

   det1.raw.range_rate = int_ref_calib.max_abs_range_rate;
   det1.raw.range = int_ref_calib.range_max;
   det1.raw.rcs = int_ref_calib.rcs_max;

   det2.raw.range_rate = -int_ref_calib.max_abs_range_rate;
   det2.raw.range = int_ref_calib.range_max;
   det2.raw.rcs = int_ref_calib.rcs_max;

   /** \action
    * Call function Is_Detection_Relevant_For_Internal_Reflection_Buffer().
    */
   bool result_1 = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det1, int_ref_calib);
   bool result_2 = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det2, int_ref_calib);

   /** \result
    * Verify that the detections are both flagged as relevant.
    */
   CHECK_TRUE(result_1);
   CHECK_TRUE(result_2);
}


/** \purpose
 * Verify that a detection is not flagged as relevant for the buffer due to the detections' range-rate being too high.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Is_Detection_Relevant_For_Internal_Reflection_Buffer__not_relevant__rr_too_high)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 and det2 to have range, rcs the inside limit of what's considered to be relevant for the buffer.
    * Set det1 and det2 to have range-rate on the outside limit of what's considered to be relevant for the buffer.
    */
   int_ref_calib.max_abs_range_rate = 0.1F;
   int_ref_calib.range_max = 5.0F;
   int_ref_calib.rcs_max = -15.0F;

   det1.raw.range_rate = 1.1 * int_ref_calib.max_abs_range_rate;
   det1.raw.range = int_ref_calib.range_max;
   det1.raw.rcs = int_ref_calib.rcs_max;

   det2.raw.range_rate = - 1.1 * int_ref_calib.max_abs_range_rate;
   det2.raw.range = int_ref_calib.range_max;
   det2.raw.rcs = int_ref_calib.rcs_max;

   /** \action
    * Call function Is_Detection_Relevant_For_Internal_Reflection_Buffer().
    */
   bool result_1 = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det1, int_ref_calib);
   bool result_2 = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det2, int_ref_calib);

   /** \result
    * Verify that none of the detections are flagged as relevant.
    */
   CHECK_FALSE(result_1);
   CHECK_FALSE(result_2);
}


/** \purpose
 *  Verify that a detection is not flagged as relevant for the buffer due to the detections' range being too far away.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Is_Detection_Relevant_For_Internal_Reflection_Buffer__not_relevant__range_too_long)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to have range-rate, rcs the inside limit of what's considered to be relevant for the buffer.
    * Set det1 to have range on the outside limit of what's considered to be relevant for the buffer.
    */
   int_ref_calib.max_abs_range_rate = 0.1F;
   int_ref_calib.range_max = 5.0F;
   int_ref_calib.rcs_max = -15.0F;

   det1.raw.range_rate = int_ref_calib.max_abs_range_rate;
   det1.raw.range = 1.1 * int_ref_calib.range_max;
   det1.raw.rcs = int_ref_calib.rcs_max;

   /** \action
    * Call function Is_Detection_Relevant_For_Internal_Reflection_Buffer().
    */
   bool result = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det1, int_ref_calib);

   /** \result
    * Verify that the detection is not flagged as relevant.
    */
   CHECK_FALSE(result);
}


/** \purpose
 *  Verify that a detection is not flagged as relevant for the buffer due to the detections' rcs being too high.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Is_Detection_Relevant_For_Internal_Reflection_Buffer__not_relevant__rcs_too_high)
{
   /** \precond
    * Set relevant int_ref_calib parameters to some values
    * Set det1 to have range-rate, range the inside limit of what's considered to be relevant for the buffer.
    * Set det1 to have rcs on the outside limit of what's considered to be relevant for the buffer.
    */
   int_ref_calib.max_abs_range_rate = 0.1F;
   int_ref_calib.range_max = 5.0F;
   int_ref_calib.rcs_max = -15.0F;

   det1.raw.range_rate = int_ref_calib.max_abs_range_rate;
   det1.raw.range = int_ref_calib.range_max;
   det1.raw.rcs = int_ref_calib.rcs_max + 1.0F;

   /** \action
    * Call function Is_Detection_Relevant_For_Internal_Reflection_Buffer().
    */
   bool result = Is_Detection_Relevant_For_Internal_Reflection_Buffer(det1, int_ref_calib);

   /** \result
    * Verify that the detection is not flagged as relevant.
    */
   CHECK_FALSE(result);
}



/** \purpose
 * Verify that every buffer slot are looped over during a call to Update_Single_Buffer_Age_And_Reset_Implausible_Slots().
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Update_Single_Buffer_Age_And_Reset_Implausible_Slots__verify_all_entries_are_updated)
{
   /** \precond
    * Reset all entries in all buffer slots to 0.
    * Set relevant int_ref_calib parameters to some values
    * Set all buffer slots f_updated_this_cycle to true to indicate they were updated in the previous cycle
    * Set the age of all buffer slots to > 0 but < int_ref_calib.age_threshold to indicate they're valid slots
    */
   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   for (int i_buf = 0; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++)
   {
      int_ref_buffer[i_buf].f_updated_this_cycle = true;
      int_ref_buffer[i_buf].age = 1U;
   }

   /** \action
    * Call function Update_Single_Buffer_Age_And_Reset_Implausible_Slots().
    */
   Update_Single_Buffer_Age_And_Reset_Implausible_Slots(int_ref_calib, int_ref_buffer);

   /** \result
    * Verify the f_updated_this_cycle was reset to false for all buffer slots.
    */
   for (int i_buf = 0; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++)
   {
      CHECK_FALSE_TEXT(int_ref_buffer[i_buf].f_updated_this_cycle, "A buffer slot was not reset.");
   }
}


/** \purpose
 * Verify the age of buffer slots are incremented as expected.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Update_Single_Buffer_Age_And_Reset_Implausible_Slots__verify_age_incrementation)
{
   /** \precond
    * Reset all entries in all buffer slots to 0.
    * Set relevant int_ref_calib parameters to some values
    * Set the age of two arbitrary slots to some number smaller than UINT16_MAX
    */
   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_threshold = 0.1F;

   int_ref_buffer[0].age = 0U;

   int_ref_buffer[1].age = 1U;

   int_ref_buffer[7].age = 5U;

   /** \action
    * Call function Update_Single_Buffer_Age_And_Reset_Implausible_Slots().
    */
   Update_Single_Buffer_Age_And_Reset_Implausible_Slots(int_ref_calib, int_ref_buffer);

   /** \result
    * Verify that a slot with an age of 0 does not get incremented by 1
    * Verify the slots with an age smaller than UINT16_MAX was incremented by 1
    * Verify the slot with an age equal to UINT16_MAX was not incremented.
    */
   CHECK_EQUAL_TEXT(0U, int_ref_buffer[0].age, "A buffer slot's age was incremented while it was expected not to.");
   CHECK_EQUAL_TEXT(2U, int_ref_buffer[1].age, "A buffer slot's age was not incremented as expected.");
   CHECK_EQUAL_TEXT(6U, int_ref_buffer[7].age, "A buffer slot's age was not incremented as expected.");
}

/** \purpose
 * Verify that the age and occurence_count of a buffer slot is correctly handled in case of saturation of the age of the buffer slot.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Update_Single_Buffer_Age_And_Reset_Implausible_Slots__verify_age_saturation)
{
   /** \precond
    * Reset all entries in all buffer slots to 0.
    * Set relevant sensor calibration parameters to some values
    * Set the age of one arbitrary slot to UINT32_MAX.
    * Set the corresponding occurrence rate to a value that will assure that the slot is not reset
    */
   int_ref_calib.age_threshold = 200U;
   int_ref_calib.occurrence_lowerlimit = 0.1F;

   int_ref_buffer[13].age = 4294967295U;
   int_ref_buffer[13].occurrence_count = 4294960000U; // To not trigger a reset of buffer slot

   /** \action
    * Call function Update_Single_Buffer_Age_And_Reset_Implausible_Slots().
    */
   Update_Single_Buffer_Age_And_Reset_Implausible_Slots(int_ref_calib, int_ref_buffer);

   /** \result
    * Verify that the age and occurence_count of the buffer slot are both divided by 2.
    */
   CHECK_EQUAL_TEXT(2147483647, int_ref_buffer[13].age, "A buffer slot's age was not divided by 2");
   CHECK_EQUAL_TEXT(2147480000, int_ref_buffer[13].occurrence_count, "A buffer slot's occurence_count was not divided by 2.");
}


/** \purpose
 * Verify buffer slots are reset as expected depending on their age, and occurence_count in relation to set calibration parameters.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__low_level_functions, Update_Single_Buffer_Age_And_Reset_Implausible_Slots__verify_buffer_slot_is_reset_as_expected)
{
   /** \precond
    * Reset all entries in all buffer slots to 0.
    * Set relevant int_ref_calib parameters to some values
    * Setup 1 buffer slot (slot A) that should be reset as its age is large enough but has an occurrence count lower than needed to be kept
    * Setup 1 buffer slot (slot B) that should not be reset as its age is large enough but has an occurrence count lower than needed to be kept
    * Setup 1 buffer slot (slot C) that should not be reset as its age is not enough to evaluate its occurrence rate
    */
   int_ref_calib.age_threshold = 100U;
   int_ref_calib.occurrence_lowerlimit = 0.2F;

   // Should be reset as its age is large enough but has an occurrence count lower than needed to be kept
   int_ref_buffer[0].age = 99U;
   int_ref_buffer[0].occurrence_count = 19U;
   int_ref_buffer[0].f_classified_as_internal_reflection = true;

   // Should not be reset as its age is large enough but has an occurrence count lower than needed to be kept
   int_ref_buffer[1].age = 99U;
   int_ref_buffer[1].occurrence_count = 20U;
   int_ref_buffer[1].f_classified_as_internal_reflection = true;

   // Should not be reset as its age is not enough to evaluate its occurrence rate
   int_ref_buffer[2].age = 98U;
   int_ref_buffer[2].occurrence_count = 1U;
   int_ref_buffer[2].f_classified_as_internal_reflection = true;


   /** \action
    * Call function Update_Single_Buffer_Age_And_Reset_Implausible_Slots().
    */
   Update_Single_Buffer_Age_And_Reset_Implausible_Slots(int_ref_calib, int_ref_buffer);

   /** \result
    * Verify that buffer slot A was reset as expected.
    * Verify that buffer slots B & C were updated as expected
    */
   CHECK_EQUAL_TEXT(0U, int_ref_buffer[0].age, "A buffer slot's age was not reset expected.");
   CHECK_EQUAL_TEXT(0U, int_ref_buffer[0].occurrence_count, "A buffer slot's occurrence_count was not reset as expected.");
   CHECK_FALSE_TEXT(int_ref_buffer[0].f_classified_as_internal_reflection, "A buffer slot's f_classified_as_internal_reflection was not reset as expected.");

   CHECK_EQUAL_TEXT(100U, int_ref_buffer[1].age, "A buffer slot's age was not incremented as expected.");
   CHECK_EQUAL_TEXT(20U, int_ref_buffer[1].occurrence_count, "A buffer slot's occurrence_count was not kept as expected.");
   CHECK_TRUE_TEXT(int_ref_buffer[1].f_classified_as_internal_reflection, "A buffer slot's f_classified_as_internal_reflection was reset while not expected.");

   CHECK_EQUAL_TEXT(99U, int_ref_buffer[2].age, "A buffer slot's age was not incremented expected.");
   CHECK_EQUAL_TEXT(1U, int_ref_buffer[2].occurrence_count, "A buffer slot's occurrence_count was not kept as expected.");
   CHECK_TRUE_TEXT(int_ref_buffer[2].f_classified_as_internal_reflection, "A buffer slot's f_classified_as_internal_reflection was reset while not expected.");
}

/** @}*/



/** \defgroup  f360_identify_and_flag_internal_reflections__high_level_functions
 *  @{
 */

/** \brief
 * This test group verifies the functionality of functions inside f360_identify_and_flag_internal_reflections.cpp
 * that are ran only once for all sensors in a tracker iteration.
 */
TEST_GROUP(f360_identify_and_flag_internal_reflections__high_level_functions)
{

   /** \setup
    * Initialize and reset variables used in function call.
    * In TEST_SETUP(), the following is done;
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Set the OTG speed of all sensors to be slightly higher than internal_reflections_min_host_vel
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Reset all entries in raw_det_list to 0
    * - Reset f_ok_to_use to true for for all dets in dets_props
    * - Reset f_func_enabled_this_iteration to false
    */

   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_det_list = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T dets_props[MAX_NUMBER_OF_DETECTIONS] = {};
   bool f_func_enabled_this_iteration;

   const float FLT_EPS = std::numeric_limits<float>::epsilon();

   TEST_SETUP()
   {

      for (int i_sens = 0; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++)
      {
         sensors[i_sens].variable.is_valid = true;

         sensors[i_sens].constant.internal_reflections.f_enable = true;

         sensors[i_sens].constant.internal_reflections.min_host_vel = 0.5F;
         sensors[i_sens].constant.internal_reflections.age_threshold = 200U;
         sensors[i_sens].constant.internal_reflections.occurrence_lowerlimit = 0.09F;
         sensors[i_sens].constant.internal_reflections.occurrence_threshold = 0.1F;

         sensors[i_sens].constant.internal_reflections.rcs_tolerance = 0.1F;
         sensors[i_sens].constant.internal_reflections.azimuth_tolerance = 0.1F;
         sensors[i_sens].constant.internal_reflections.range_tolerance = 0.1F;

         sensors[i_sens].constant.internal_reflections.max_abs_range_rate = 0.1F;
         sensors[i_sens].constant.internal_reflections.rcs_max = -15.0F;
         sensors[i_sens].constant.internal_reflections.range_max = 5.0F;

         sensors[i_sens].variable.vcs_velocity.longitudinal = 1.1F * sensors[i_sens].constant.internal_reflections.min_host_vel;

         for (int i_det = 0; i_det < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_det++)
         {
            sensor_props[i_sens].internal_reflections_buffer[i_det] = Internal_Reflection_Buffer_Slot{};
         }
      }

      raw_det_list = {};

      for (int i_det = 0; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
      {
         dets_props[i_det].f_ok_to_use = true;
      }

      f_func_enabled_this_iteration = false;
   }

};


/** \purpose
 * Verify that all sensors are looped over during call to Is_Bike_Carrier_Attached()
 * and that each corresponding buffer's age is incremented as expected.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Is_Bike_Carrier_Attached__verify_all_sensors_buffers_are_updated)
{
   /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Set the age of every buffer slot for every sensor to 1
   */

   for (int i_sens = 0; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++){
      for (int i_buf = 0; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++){
         sensor_props[i_sens].internal_reflections_buffer[i_buf].age = 1U;
      }
   }

   /** \action
    * Call function Is_Bike_Carrier_Attached().
    */
   Is_Bike_Carrier_Attached(sensors, sensor_props);

   /** \result
    * Verify that the age of every buffer slot of every sensor is incremented by 1
    */

   for (int i_sens = 0; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++)
   {
     for (int i_buf = 0; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++){
        CHECK_EQUAL_TEXT(2U, sensor_props[i_sens].internal_reflections_buffer[i_buf].age, "Not all sensor's buffers were time updated as expected.");
     }
  }
}


/** \purpose
 *  Verify a sensor's buffer is not updated when the sensor is moving too slow.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Is_Bike_Carrier_Attached__verify_a_buffer_is_not_updated_if_host_too_slow)
{
   /** \precond
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Set the vcs_velocity.longitudinal to be lower than internal_reflections.min_host_vel for a sensor
    * - Set the age of a buffer slot to arbitrary number smaller than UIN16_MAX for the sensor
    */
   sensors[0].variable.vcs_velocity.longitudinal = sensors[0].constant.internal_reflections.min_host_vel;
   sensor_props[0].internal_reflections_buffer[0].age = 1U;

   /** \action
    * Call function Is_Bike_Carrier_Attached().
    */
   Is_Bike_Carrier_Attached(sensors, sensor_props);

   /** \result
    * Verify the sensor's buffer's age was not incremented
    */
   CHECK_EQUAL_TEXT(1U,  sensor_props[0].internal_reflections_buffer[0].age, "A buffer slot was time updated when it was expected not to.");
}


/** \purpose
 *  Verify sensor's buffer slots are not updated if the function is disabled for the sensor or the sensor is invalid.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Is_Bike_Carrier_Attached__verify_a_buffer_is_not_updated_if_func_is_not_enabled)
{
   /** \precond
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Set a buffer slot's age for 4 different sensors to arbitrary number smaller than UINT16_MAX
    * - Setup 4 sensors such that
    * -- 1 has sensors.variable.is_valid and sensors.internal_reflections.f_enable set to false
    * -- 1 has sensors.variable.is_valid and sensors.internal_reflections.f_enable set to true
    * -- 1 has sensors.variable.is_valid set to true and sensors.internal_reflections.f_enable set to false
    * -- 1 has sensors.variable.is_valid set to false and sensors.internal_reflections.f_enable set to true
    */

   sensor_props[0].internal_reflections_buffer[0].age = 1U;
   sensor_props[1].internal_reflections_buffer[0].age = 1U;
   sensor_props[2].internal_reflections_buffer[0].age = 1U;
   sensor_props[3].internal_reflections_buffer[0].age = 1U;

   sensors[0].constant.internal_reflections.f_enable = false;
   sensors[0].variable.is_valid = false;

   sensors[1].constant.internal_reflections.f_enable = true;
   sensors[1].variable.is_valid = true;

   sensors[2].constant.internal_reflections.f_enable = false;
   sensors[2].variable.is_valid = true;

   sensors[3].constant.internal_reflections.f_enable = true;
   sensors[3].variable.is_valid = false;

   /** \action
    * Call function Is_Bike_Carrier_Attached().
    */
   Is_Bike_Carrier_Attached(sensors, sensor_props);

   /** \result
    * Verify the buffer slot's age is incremented as expected.
    */
   CHECK_EQUAL_TEXT(1U, sensor_props[0].internal_reflections_buffer[0].age, "A buffer slot was updated when it was expected not to.");
   CHECK_EQUAL_TEXT(2U, sensor_props[1].internal_reflections_buffer[0].age, "A buffer slot was not updated when it was expected to.");
   CHECK_EQUAL_TEXT(1U, sensor_props[2].internal_reflections_buffer[0].age, "A buffer slot was updated when it was expected not to.");
   CHECK_EQUAL_TEXT(1U, sensor_props[3].internal_reflections_buffer[0].age, "A buffer slot was updated when it was expected not to.");

}

/** \purpose
 *  Verify f_func_enabled_this_iteration is set to true if at least 1 sensor has the internal_reflections.f_enable flag set to true.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Is_Bike_Carrier_Attached__verify_f_func_enabled_this_iteration_is_set_to_true)
{
   /** \precond
    * - Set sensors.internal_reflections.f_enable flag to true for at least 1 sensor.
    * - Reset f_func_enabled_this_iteration to false.
    */

   /** \action
    * Call function Is_Bike_Carrier_Attached().
    */
   bool f_func_enabled_this_iteration = Is_Bike_Carrier_Attached(sensors, sensor_props);

   /** \result
    * Verify f_func_enabled_this_iteration is set to true.
    */
   CHECK_TRUE_TEXT(f_func_enabled_this_iteration, "f_func_enabled_this_iteration was kept as false when it was expected to be set to true.");
}

/** \purpose
 *  Verify f_func_enabled_this_iteration is kept as false if all sensors has the internal_reflections.f_enable flag set to false.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Is_Bike_Carrier_Attached__verify_f_func_enabled_this_iteration_is_kept_as_false)
{
   /** \precond
    * - Set sensors.internal_reflections.f_enable flag to false for all sensors.
    * - Reset f_func_enabled_this_iteration to false.
    */

   for (F360_Radar_Sensor_T & sensor_cal : sensors)
   {
      sensor_cal.constant.internal_reflections.f_enable = false;
   }

   /** \action
    * Call function Is_Bike_Carrier_Attached().
    */
   bool f_func_enabled_this_iteration = Is_Bike_Carrier_Attached(sensors, sensor_props);

   /** \result
    * Verify f_func_enabled_this_iteration is kept as false.
    */
   CHECK_FALSE_TEXT(f_func_enabled_this_iteration, "f_func_enabled_this_iteration was set to true when it was expected to be kept as false.");
}


/** \purpose
 *  Verify that Is_Bike_Carrier_Attached() are called during call to Identify_And_Flag_Internal_Reflections().
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_buffers_are_time_updated)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Pick some arbitrary buffer slots and set their age > 0 and < internal_reflections.age_threshold to indicate they're valid slots
    */

   sensor_props[0].internal_reflections_buffer[0].age = 1U;
   sensor_props[3].internal_reflections_buffer[2].age = 2U;

   /** \action
    * Call Is_Bike_Carrier_Attached().
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   
   const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);
   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify these slots' age were incremented by 1.
    */
   CHECK_EQUAL_TEXT(2U, sensor_props[0].internal_reflections_buffer[0].age, "A buffer slot's age were incremented as expected.");
   CHECK_EQUAL_TEXT(3U, sensor_props[3].internal_reflections_buffer[2].age, "A buffer slot's age were incremented as expected.");
}


/** \purpose
 *  Verify that all valid detections are looped over as expected during function call.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_all_valid_dets_are_checked)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup 4 "unique" detections such that each detection will get entered into its own buffer slot
   * - Set number_of_valid_detections to 3 such the 4th detection in raw_det_list should not be considered
   */

   raw_det_list.number_of_valid_detections = 3;

   raw_det_list.detections[0].raw.range = 0.15F; // increment by at least internal_reflections.range_tolerance
   raw_det_list.detections[1].raw.range = 0.3F;
   raw_det_list.detections[2].raw.range = 0.45F;
   raw_det_list.detections[3].raw.range = 0.6F;

   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[2].raw.rcs = -15.0F;
   raw_det_list.detections[3].raw.rcs = -15.0F;

   raw_det_list.detections[0].raw.sensor_id = 1;
   raw_det_list.detections[1].raw.sensor_id = 1;
   raw_det_list.detections[2].raw.sensor_id = 1;
   raw_det_list.detections[3].raw.sensor_id = 1;


   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that only 3 detections were added to the buffer
    */

   DOUBLES_EQUAL_TEXT(0.15F,sensor_props[0].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the buffer as expected.");
   DOUBLES_EQUAL_TEXT(0.3F, sensor_props[0].internal_reflections_buffer[1].range, FLT_EPS, "A detection was not added to the buffer as expected.");
   DOUBLES_EQUAL_TEXT(0.45F,sensor_props[0].internal_reflections_buffer[2].range, FLT_EPS, "A detection was not added to the buffer as expected.");
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[0].internal_reflections_buffer[3].range, FLT_EPS, "A detection was added to the buffer when it was not expected.");

}


/** \purpose
 *  Verify that detections are split into different buffers determined by their sensor id.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_dets_are_split_per_sensor)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup 4 "unique" detections such that each detection will get entered into its own buffer slot
   */

   raw_det_list.number_of_valid_detections = 4;

   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;

   raw_det_list.detections[1].raw.range = 0.3F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 2;

   raw_det_list.detections[2].raw.range = 0.45F;
   raw_det_list.detections[2].raw.rcs = -15.0F;
   raw_det_list.detections[2].raw.sensor_id = 3;

   raw_det_list.detections[3].raw.range = 0.6F;
   raw_det_list.detections[3].raw.rcs = -15.0F;
   raw_det_list.detections[3].raw.sensor_id = 4;


   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that the detections were added to the expected buffer and buffer slot.
    */
   DOUBLES_EQUAL_TEXT(0.15F,sensor_props[0].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot.");
   DOUBLES_EQUAL_TEXT(0.3F, sensor_props[1].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot.");
   DOUBLES_EQUAL_TEXT(0.45F,sensor_props[2].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot.");
   DOUBLES_EQUAL_TEXT(0.6F, sensor_props[3].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot.");
}


/** \purpose
 *  Verify that the function only considers detections from sensors that have internal_reflections.f_enable set to true and the detections are considered relevant
 *  as per Is_Detection_Relevant_For_Internal_Reflection_Buffer().
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_only_relevant_dets_from_relevant_sensors_are_considered)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup 2 "relevant" dets as per Is_Detection_Relevant_For_Internal_Reflection_Buffer().
   * - Setup 2 "irrelevant" dets
   * - All 4 detections shall come from different sensors.
   * - Set sensors[x].internal_reflections.f_enable such that there are:
   * -- A detection that originates from a sensor that have internal_reflections.f_enable = true and is relevant to buffer.
   * -- A detection that originates from a sensor that have internal_reflections.f_enable = false and is relevant to buffer.
   * -- A detection that originates from a sensor that have internal_reflections.f_enable = true and is not relevant to buffer.
   * -- A detection that originates from a sensor that have internal_reflections.f_enable = false and is not relevant to buffer.
   *
   */

   raw_det_list.number_of_valid_detections = 4;

   // Irrelevant
   raw_det_list.detections[0].raw.range = 10.0F;
   raw_det_list.detections[1].raw.range = 11.0F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.rcs = -15.0F;

   // Relevant
   raw_det_list.detections[2].raw.range = 0.15F;
   raw_det_list.detections[3].raw.range = 0.3F;
   raw_det_list.detections[2].raw.rcs = -15.0F;
   raw_det_list.detections[3].raw.rcs = -15.0F;

   raw_det_list.detections[0].raw.sensor_id = 1;
   raw_det_list.detections[1].raw.sensor_id = 2;
   raw_det_list.detections[2].raw.sensor_id = 3;
   raw_det_list.detections[3].raw.sensor_id = 4;

   sensors[0].constant.internal_reflections.f_enable = false;
   sensors[1].constant.internal_reflections.f_enable = true;
   sensors[2].constant.internal_reflections.f_enable = false;
   sensors[3].constant.internal_reflections.f_enable = true;


   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that only the relevant detections were added to the expected buffer and buffer slot.
    */
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[0].internal_reflections_buffer[0].range, FLT_EPS, "A detection was added to a buffer slot when it was expected not to.");
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[1].internal_reflections_buffer[0].range, FLT_EPS, "A detection was added to a buffer slot when it was expected not to.");
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[2].internal_reflections_buffer[0].range, FLT_EPS, "A detection was added to a buffer slot when it was expected not to.");
   DOUBLES_EQUAL_TEXT(0.3F, sensor_props[3].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot.");

}


/** \purpose
 *  Verify that detections are not added to the buffers when the corresponding sensor is moving too slow.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_only_sensors_moving_fast_enough_are_considered)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Set 1 sensor to move slower than internal_reflections.min_host_vel
   * - Set 1 sensor to move at than internal_reflections_min.host_vel
   * - Setup 2 relevant dets from sensor moving too slow.
   * - Setup 2 relevant dets from sensor moving fast enough.
   */

   sensors[0].variable.vcs_velocity.longitudinal = 0.9F * sensors[0].constant.internal_reflections.min_host_vel;
   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;
   raw_det_list.detections[1].raw.range = 0.3F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 1;

   sensors[1].variable.vcs_velocity.longitudinal = 1.1F * sensors[1].constant.internal_reflections.min_host_vel;
   raw_det_list.detections[2].raw.range = 0.15F;
   raw_det_list.detections[2].raw.rcs = -15.0F;
   raw_det_list.detections[2].raw.sensor_id = 2;
   raw_det_list.detections[3].raw.range = 0.3F;
   raw_det_list.detections[3].raw.rcs = -15.0F;
   raw_det_list.detections[3].raw.sensor_id = 2;

   raw_det_list.number_of_valid_detections = 4;

   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that only the detections from the fast moving sensor were added to the expected buffer and buffer slot.
    */
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[0].internal_reflections_buffer[0].range, FLT_EPS, "A detection was added to a sensor's buffer when it was expected not to b/c the sensor is moving too slow.");
   DOUBLES_EQUAL_TEXT(0.0F, sensor_props[0].internal_reflections_buffer[1].range, FLT_EPS, "A detection was added to a sensor's buffer when it was expected not to b/c the sensor is moving too slow.");
   DOUBLES_EQUAL_TEXT(0.15F,sensor_props[1].internal_reflections_buffer[0].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot when it was expected to.");
   DOUBLES_EQUAL_TEXT(0.3F, sensor_props[1].internal_reflections_buffer[1].range, FLT_EPS, "A detection was not added to the expected buffer and/or buffer slot when it was expected to.");
}


/** \purpose
 *  Verify that new detections gets matched against existing ones stored in the sensor buffer and added to it if its similar enough.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_new_dets_are_added_to_buffers)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup a detection from two different sensor that are relevant for buffers
   * - Pre-fill corresponding buffers with some number of instances of this detection
   */

   raw_det_list.number_of_valid_detections = 2;

   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;

   raw_det_list.detections[1].raw.range = 0.3F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 2;

   sensor_props[0].internal_reflections_buffer[0].age = 10U;
   sensor_props[0].internal_reflections_buffer[0].occurrence_count = 2U;
   sensor_props[0].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[0].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[0].internal_reflections_buffer[0].rcs = -15.0F;

   sensor_props[1].internal_reflections_buffer[5].age = 10U;
   sensor_props[1].internal_reflections_buffer[5].occurrence_count = 9U;
   sensor_props[1].internal_reflections_buffer[5].range = 0.3F;
   sensor_props[1].internal_reflections_buffer[5].azimuth = 0.0F;
   sensor_props[1].internal_reflections_buffer[5].rcs = -15.0F;

   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that the occurrence count of the buffers were incremented
    */
   CHECK_EQUAL_TEXT(3U,  sensor_props[0].internal_reflections_buffer[0].occurrence_count, "A detection was not added to a sensor's buffer as expected.");
   CHECK_EQUAL_TEXT(10U, sensor_props[1].internal_reflections_buffer[5].occurrence_count, "A detection was not added to a sensor's buffer as expected.");
}


/** \purpose
 *  Verify that new detections gets matched against existing ones stored in the sensor buffer but not added to it if the sensor is moving too slow.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_new_dets_are_matched_against_buffers_but_not_added)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Set 1 sensor to move slower than internal_reflections.min_host_vel
   * - Set 1 sensor to move at than internal_reflections.min_host_vel
   * - Setup a detection from these two sensors that are relevant for buffers
   * - Pre-fill corresponding buffers with some number of instances of this detection
   */

   raw_det_list.number_of_valid_detections = 2;

   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;

   sensors[0].variable.vcs_velocity.longitudinal = 0.9F * sensors[0].constant.internal_reflections.min_host_vel;

   sensor_props[0].internal_reflections_buffer[0].age = 10U;
   sensor_props[0].internal_reflections_buffer[0].occurrence_count = 2U;
   sensor_props[0].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[0].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[0].internal_reflections_buffer[0].rcs = -15.0F;

   raw_det_list.detections[1].raw.range = 0.15F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 2;

   sensors[1].variable.vcs_velocity.longitudinal = 1.1F * sensors[1].constant.internal_reflections.min_host_vel;

   sensor_props[1].internal_reflections_buffer[0].age = 10U;
   sensor_props[1].internal_reflections_buffer[0].occurrence_count = 2U;
   sensor_props[1].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[1].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[1].internal_reflections_buffer[0].rcs = -15.0F;

   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * - Verify that the occurrence count of the buffer slot of the sensor that is not moving fast enough was not incremented.
    * - Verify that the occurrence count of the buffer slot of the sensor that is moving fast enough was incremented.
    */
   CHECK_EQUAL_TEXT(2U,  sensor_props[0].internal_reflections_buffer[0].occurrence_count, "A detection was used to update a sensor's buffer when it was expected not to b/c the sensor is moving too slow.");
   CHECK_EQUAL_TEXT(3U,  sensor_props[1].internal_reflections_buffer[0].occurrence_count, "A detection was not used to update a sensor's buffer when it was expected to.");
}


/** \purpose
 *  Verify that if a buffer has met the criteria for flagging detections as internal reflections,
 *  that new, similar enough detections, gets added to the buffer and flagged as not ok to use.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_dets_are_flagged_as_nok_to_use)
{
  /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup 2 different detections that is relevant for the buffers.
   * - Pre-fill a corresponding buffer slot with 1 of these detections.
   * - Pre-fill a corresponding buffer slot with an age => internal_reflections.age_threshold
   * - Pre-fill a corresponding buffer slot with an occurence_count such that the occurence_rate > internal_reflections.occurrence_threshold
   */

   raw_det_list.number_of_valid_detections = 2;

   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;

   raw_det_list.detections[1].raw.range = 0.3F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 1;

   sensor_props[0].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[0].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[0].internal_reflections_buffer[0].rcs = -15.0F;
   sensor_props[0].internal_reflections_buffer[0].age = 199U;
   sensor_props[0].internal_reflections_buffer[0].occurrence_count = 199U;

   /** \action
    * Call Is_Bike_Carrier_Attached()
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);
   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * - Verify that the detection stored sufficiently long in the buffer gets flagged as not ok to use
    * - Verify that the detection not previously stored in the buffer gets flagged as ok to use
    * - Verify both detections were added to expected buffer slots
    */
   CHECK_FALSE(dets_props[0].f_ok_to_use);
   CHECK_EQUAL(200U, sensor_props[0].internal_reflections_buffer[0].occurrence_count);
   CHECK_EQUAL(200U, sensor_props[0].internal_reflections_buffer[0].age);

   CHECK_TRUE(dets_props[1].f_ok_to_use);
   CHECK_EQUAL(1U, sensor_props[0].internal_reflections_buffer[1].occurrence_count);
   CHECK_EQUAL(1U, sensor_props[0].internal_reflections_buffer[1].age);
}


/** \purpose
 *  Verify that if a buffer has met the criteria for flagging detections as internal reflections, that new, similar enough detections, gets flagged
 *  as not ok to use but not added to the buffer as the sensor is moving too slow.
 * \req
 *  NA.
 */
TEST(f360_identify_and_flag_internal_reflections__high_level_functions, Identify_And_Flag_Internal_Reflections__verify_dets_are_flagged_but_not_added_if_slow_moving_sensor)
{
 /** \precond
   * - Set all relevant sensors for all sensors to some reasonable value
   * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
   * - Reset all entries in all buffer slots for all sensors to 0.
   * - Reset all entries in raw_det_list to 0
   * - Reset f_ok_to_use to true for for all dets in dets_props
   * - Setup 2 identical detections from 2 different sensors that are relevant for the buffers.
   * - Pre-fill corresponding buffer slots with these detections.
   * - Pre-fill corresponding buffer slots with an age => internal_reflections_age_threshold
   * - Pre-fill corresponding buffer slots with an occurence_count such that the occurence_rate > internal_reflections.occurrence_threshold
   * - Set the velocity of 1 sensor to be slower than internal_reflections.min_host_vel
   * - Set the velocity of 1 sensor to be faster than internal_reflections.min_host_vel
   */

   raw_det_list.number_of_valid_detections = 2;

   raw_det_list.detections[0].raw.range = 0.15F;
   raw_det_list.detections[0].raw.rcs = -15.0F;
   raw_det_list.detections[0].raw.sensor_id = 1;

   sensor_props[0].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[0].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[0].internal_reflections_buffer[0].rcs = -15.0F;
   sensor_props[0].internal_reflections_buffer[0].age = 200U;
   sensor_props[0].internal_reflections_buffer[0].occurrence_count = 200U;
   sensor_props[0].internal_reflections_buffer[0].f_classified_as_internal_reflection = true;

   sensors[0].variable.vcs_velocity.longitudinal = 0.9F * sensors[0].constant.internal_reflections.min_host_vel;


   raw_det_list.detections[1].raw.range = 0.15F;
   raw_det_list.detections[1].raw.rcs = -15.0F;
   raw_det_list.detections[1].raw.sensor_id = 2;

   sensor_props[1].internal_reflections_buffer[0].range = 0.15F;
   sensor_props[1].internal_reflections_buffer[0].azimuth = 0.0F;
   sensor_props[1].internal_reflections_buffer[0].rcs = -15.0F;
   sensor_props[1].internal_reflections_buffer[0].age = 200U;
   sensor_props[1].internal_reflections_buffer[0].occurrence_count = 200U;
   sensor_props[1].internal_reflections_buffer[0].f_classified_as_internal_reflection = true;

   sensors[1].variable.vcs_velocity.longitudinal = 1.1F * sensors[1].constant.internal_reflections.min_host_vel;

   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * - Verify that both detections gets flagged as not ok to use.
    * - Verify that the detection from the faster moving sensor was used to update the buffer
    * - Verify that the detection from the slower moving sensor was not used to update the buffer
    */
   CHECK_FALSE(dets_props[0].f_ok_to_use);
   CHECK_EQUAL(200U, sensor_props[0].internal_reflections_buffer[0].occurrence_count);

   CHECK_FALSE(dets_props[1].f_ok_to_use);
   CHECK_EQUAL(201U, sensor_props[1].internal_reflections_buffer[0].occurrence_count);
}

/** @}*/
