/**
 * @file cta_object_validator_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_object_validator.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41962}
 */

#include "cta_object_validator_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_object_validator.c"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "pt_output_t.h"
}


/**
 * Arranges a valid object and tests the object validator. The object is expected to be valid.
 * \uts{CSCSA-41963} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_valid)
{
   /** \arrange creates a valid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be valid */
   EXPECT_TRUE(result);
}

/**
 * Arranges a valid object and modify its reflection flag. The object is expected to be invalid.
 * \uts{CSCSA-41964} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_reflection_and_invalid)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;
   object.tracker_data.f_reflection = FBK_TRUE;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its stage as well as stage age so that it is above its ignore cycles. The object is expected
 * to be invalid. \uts{CSCSA-41965} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_above_ignore_cycles_and_invalid)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side          = FBK_SIDE_LEFT;
   cals.k_cta_cycles_coasted_to_ignore       = 5;
   object.tracker_data.status                = PA_OBJ_STATUS_COASTED;
   object.tracker_data.f_is_in_rl_sensor_fov = FBK_TRUE;
   object.tracker_data.stage_age             = cals.k_cta_cycles_coasted_to_ignore + (uint8_t) 1;


   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its age so that it is below the validity threshold. The object is expected to be invalid.
 * \uts{CSCSA-41966} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_age_is_too_low)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;
   object.tracker_data.age          = cals.k_cta_min_object_age_check_valid - 1;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its speed so that it is outside of the allowed range. The object is expected to be invalid.
 * \uts{CSCSA-41967} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_speed_outside_of_range)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   object.tracker_data.speed = cals.k_cta_min_speed - EPSILON;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its speed so that it is outside of the allowed max range. The object is expected to be
 * invalid. \uts{CSCSA-306960} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_speed_outside_of_max_range)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   object.tracker_data.speed = cals.k_cta_max_speed + 999.9f;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}


/**
 * Arranges a valid object and modify its heading so that it is outside of the allowed range. The object is expected to be invalid.
 * \uts{CSCSA-41968} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_heading_outside_of_range)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   object.attributes->CTA_heading = cals.k_cta_heading_range[0u] - EPSILON;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its lateral velocity so that it is outside of the allowed range. The object is expected to be
 * invalid. \uts{CSCSA-41969} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_lateral_velocity_outside_of_range)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   cals.k_cta_min_lateral_approach_speed  = cals.k_cta_min_lateral_approach_speed + 2.0f * EPSILON;
   object.attributes->approach_side       = FBK_SIDE_LEFT;
   object.attributes->relative_velocity.y = cals.k_cta_min_lateral_approach_speed - EPSILON;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its existence probability so that it less than the allowed threshold. The object is expected
 * to be invalid. \uts{CSCSA-41970} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_existence_probability_is_below_thres)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   object.tracker_data.existence_probability = 0.9f * cals.k_cta_min_rel_existence_probability;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify the suppression counter so that it is less than the minimum needed threshold. The object is
 * expected to be invalid. \uts{CSCSA-41971} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_suppression_ctr_is_less_than_minimum_needed_ctr)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side   = FBK_SIDE_LEFT;
   pt_match_info.path_direction       = PATH_DIRECTION_LAT_LEFT;
   object.attributes->p_pt_match_info = &pt_match_info;

   cals.k_cta_object_supress_counter                   = 3u;
   cals.k_cta_f_use_object_supress_counter             = FBK_TRUE;
   object.persistent->obj_validity_suppression_counter = cals.k_cta_object_supress_counter - 2u;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify the obstruction probability is higher than the maximum allowed obstruction probability. The
 * object is expected to be invalid. \uts{CSCSA-41972} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_obstruction_probability_is_higher_than_allowed_thres)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   cals.k_cta_f_check_obstruction_probability_signal = FBK_TRUE;
   object.tracker_data.obstruction_prob              = cals.k_cta_max_obstruction_probability + EPSILON;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and let the check against potential ghost fails. The object is expected to be invalid.
 * \uts{CSCSA-41973} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_invalid_since_check_against_potential_ghost_fails)
{
   /** \arrange creates an invalid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   cals.k_cta_f_use_object_min_object_age_in_cycles = FBK_FALSE;
   cals.k_cta_f_use_ghost_detector                  = FBK_TRUE;
   object.tracker_data.vcs_heading                  = -0.5f * PI;
   pt_match_info.path_heading = object.tracker_data.vcs_heading + cals.k_cta_ghost_condition_max_heading_diff_path_tracker + EPSILON;
   object.attributes->p_pt_match_info = &pt_match_info;
   object.tracker_data.age            = cals.k_cta_ghost_validation_min_age - 1;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be invalid */
   EXPECT_FALSE(result);
}

/**
 * Arranges a valid object and modify its reflection flag. However this flag shall not be checked within this test The object is
 * expected to be valid. \uts{CSCSA-41974} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_reflection_but_valid)
{
   /** \arrange creates an valid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side     = FBK_SIDE_LEFT;
   object.tracker_data.f_reflection     = FBK_TRUE;
   cals.k_cta_f_check_reflection_signal = FBK_FALSE;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be valid */
   EXPECT_TRUE(result);
}

/**
 * Arranges a valid object and modify its stage_age and status to coasted. The object is expected to be valid.
 * \uts{CSCSA-41975} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__object_is_coasted_and_below_ignore_cycles_and_valid)
{
   /** \arrange creates an valid object */
   boolean_T result;
   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side    = FBK_SIDE_LEFT;
   cals.k_cta_cycles_coasted_to_ignore = 5;
   object.tracker_data.status          = PA_OBJ_STATUS_COASTED;
   object.tracker_data.stage_age       = cals.k_cta_cycles_coasted_to_ignore - (uint8_t) 1;

   /** \action executes function to test */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Expect the object to be valid */
   EXPECT_TRUE(result);
}

/**
 * Check that an object is classified as invalid for CTA if VRU conditions are not met.
 * \uts{CSCSA-145890} \sdd{SF-3872} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Valid__returns_FALSE_for_invalid_vru_object)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid for CTA. */
   boolean_T result = FBK_FALSE;
   float32_T len    = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width  = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   Cta_Create_Valid_Object(&object, &cals);
   object.attributes->approach_side = FBK_SIDE_LEFT;
   object.tracker_data.obj_class    = PA_OBJ_CLASS_PEDESTRIAN;
   object.tracker_data.length       = len - EPSILON;
   object.tracker_data.width        = width - EPSILON;
   object.tracker_data.speed        = cals.k_cta_pedestrian_min_speed - EPSILON;

   /** \action Call Cta_Is_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as invalid. */
   EXPECT_FALSE(result);
}

/**
 * Tests the speed range functionality of object validation Expect true since speed is in speed boundary
 * \uts{CSCSA-41976} \sdd{SF-3867} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Speed_In_Allowed_Range__in_range)
{
   /** \arrange speed range and speed value to check */
   boolean_T result;
   Float_Range_T speed_range = Create_Float_Range(0.0f, 20.0f);
   object.tracker_data.speed = 10.0f;

   /** \action executes function to test */
   result = Cta_Is_Object_Speed_In_Allowed_Range(&object, &speed_range);

   /** \assert true since speed is in specified speed boundary */
   EXPECT_TRUE(result);
}

#ifndef NDEBUG
/**
 * Tests the speed range functionality of object validation Expect assertion since object points to NULL
 * \uts{CSCSA-41977} \sdd{SF-3867} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Speed_In_Allowed_Range___object_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange object and speed range */
         Float_Range_T speed_range;
         /** \action executes function to test */
         Cta_Is_Object_Speed_In_Allowed_Range(NULL, &speed_range);
         /** \assert assertion since object is pointing to NULL */
      },
      ".*p_object.*");
}

/**
 * Tests the speed range functionality of object validation. Expect assertion since speed range points to NULL.
 * \uts{CSCSA-41978} \sdd{SF-3867} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Speed_In_Allowed_Range___float_range_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange object and speed range */
         /** \action executes function to test */
         Cta_Is_Object_Speed_In_Allowed_Range(&object, NULL);
         /** \assert assertion since speed range is pointing to NULL */
      },
      ".*p_cta_speed_range.*");
}
#endif

/**
 * Tests the heading range and heading variance rate functionality of object validation. Expect true since CTA_heading and
 * heading_variance are in their respective boundaries. \uts{CSCSA-41979} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__heading_in_range_heading_variance_in_range)
{
   /** \arrange heading range and heading_variance_rate as well as CTA heading and heading variance values to check */
   boolean_T result;
   Float_Range_T heading_range =
      Create_Float_Range(cals.k_cta_heading_range[0u], 0.5f * (cals.k_cta_heading_range[0u] + cals.k_cta_heading_range[1u]));
   Float_Range_T heading_variance_rate  = Create_Float_Range(0.0f, 0.5f * (0.0f + cals.k_cta_max_heading_variance));
   object.attributes->CTA_heading       = cals.k_cta_heading_range[0u];
   object.tracker_data.heading_variance = 0.5f;

   /** \action executes function to test */
   result = Cta_Is_Object_Heading_Inside_Allowed_Range(&object, &heading_range, &heading_variance_rate);

   /** \assert true since CTA_heading and heading_variance are in their specified boundaries */
   EXPECT_TRUE(result);
}

/**
 * Tests the heading range and heading variance rate functionality of object validation. Expect false since CTA_heading is not in
 * range, but heading_variance is. \uts{CSCSA-41980} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__heading_not_in_range_heading_variance_in_range)
{
   /** \arrange heading range and heading_variance_rate as well as CTA heading and heading variance values to check */
   boolean_T result;
   Float_Range_T heading_range =
      Create_Float_Range(cals.k_cta_heading_range[0u], 0.5f * (cals.k_cta_heading_range[0u] + cals.k_cta_heading_range[1u]));
   Float_Range_T heading_variance_rate  = Create_Float_Range(0.0f, 0.5f * (0.0f + cals.k_cta_max_heading_variance));
   object.attributes->CTA_heading       = cals.k_cta_heading_range[1u];
   object.tracker_data.heading_variance = 0.5f;

   /** \action executes function to test */
   result = Cta_Is_Object_Heading_Inside_Allowed_Range(&object, &heading_range, &heading_variance_rate);

   /** \assert false since CTA_heading is not in specified heading boundary */
   EXPECT_FALSE(result);
}

/**
 * Tests the heading range and heading variance rate functionality of object validation. Expect false since CTA_heading is in
 * range, but heading_variance is not. \uts{CSCSA-41981} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__heading_in_range_heading_variance_not_in_range)
{
   /** \arrange heading range and heading_variance_rate as well as CTA heading and heading variance values to check */
   boolean_T result;
   Float_Range_T heading_range =
      Create_Float_Range(cals.k_cta_heading_range[0u], 0.5f * (cals.k_cta_heading_range[0u] + cals.k_cta_heading_range[1u]));
   Float_Range_T heading_variance_rate  = Create_Float_Range(0.0f, 0.5f * (0.0f + cals.k_cta_max_heading_variance));
   object.attributes->CTA_heading       = cals.k_cta_heading_range[0u];
   object.tracker_data.heading_variance = cals.k_cta_max_heading_variance;

   /** \action executes function to test */
   result = Cta_Is_Object_Heading_Inside_Allowed_Range(&object, &heading_range, &heading_variance_rate);

   /** \assert false since heading_variance is in specified variance boundary */
   EXPECT_FALSE(result);
}

#ifndef NDEBUG
/**
 * Tests the heading range and heading variance rate functionality of object validation Expect assertion since object points to
 * NULL. \uts{CSCSA-41982} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__object_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange object and heading variance range */
         Float_Range_T heading_range =
            Create_Float_Range(cals.k_cta_heading_range[0u], 0.5f * (cals.k_cta_heading_range[0u] + cals.k_cta_heading_range[1u]));
         Float_Range_T heading_variance_rate  = Create_Float_Range(0.0f, 0.5f * (0.0f + cals.k_cta_max_heading_variance));
         object.attributes->CTA_heading       = cals.k_cta_heading_range[0u];
         object.tracker_data.heading_variance = cals.k_cta_max_heading_variance;
         /** \action executes function to test */
         Cta_Is_Object_Heading_Inside_Allowed_Range(NULL, &heading_range, &heading_variance_rate);
         /** \assert assertion since object is pointing to NULL */
      },
      ".*p_object.*");
}

/**
 * Tests the heading range and heading variance rate functionality of object validation Expect assertion since heading range points
 * to NULL. \uts{CSCSA-41983} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__heading_range_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange object and heading variance range */
         Float_Range_T heading_variance_rate  = Create_Float_Range(0.0f, 0.5f * (0.0f + cals.k_cta_max_heading_variance));
         object.tracker_data.heading_variance = cals.k_cta_max_heading_variance;
         /** \action executes function to test */
         Cta_Is_Object_Heading_Inside_Allowed_Range(&object, NULL, &heading_variance_rate);
         /** \assert assertion since heading range is pointing to NULL */
      },
      ".*p_cta_heading_range.*");
}

/**
 * Tests the heading range and heading variance rate functionality of object validation Expect assertion since heading variance
 * range points to NULL. \uts{CSCSA-41984} \sdd{SF-3866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Heading_Inside_Allowed_Range__heading_variance_range_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange object and heading variance range */
         Float_Range_T heading_range =
            Create_Float_Range(cals.k_cta_heading_range[0u], 0.5f * (cals.k_cta_heading_range[0u] + cals.k_cta_heading_range[1u]));
         object.attributes->CTA_heading = cals.k_cta_heading_range[0u];
         /** \action executes function to test */
         Cta_Is_Object_Heading_Inside_Allowed_Range(&object, &heading_range, NULL);
         /** \assert assertion since heading variance range is pointing to NULL */
      },
      ".*p_cta_heading_variance_range.*");
}

#endif // indef NDEBUG
/**
 * Tests the relative lateral object velocity functionality of object validation. Expect true since the relative lateral velocity
 * is above the calibratable threshold. \uts{CSCSA-41985} \sdd{SF-3859} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Lat_Obj_Vel_Above_Threshold__relative_velocity_above_min_value)
{
   /** \arrange relative lateral object velocity to check */
   boolean_T result;
   object.attributes->relative_velocity.y = -(cals.k_cta_min_lateral_approach_speed + EPSILON);

   /** \action executes function to test */
   result = Cta_Is_Lat_Obj_Vel_Above_Threshold(&object, cals.k_cta_min_lateral_approach_speed);

   /** \assert true since the relative velocity is above the calibratable threshold */
   EXPECT_TRUE(result);
}

/**
 * Tests the suppression counter above threshold functionality of object validation. Expect true since the flag use object suppress
 * counter is set to true, while the counter itself is above the calibratable threshold. \uts{CSCSA-41986} \sdd{SF-3854}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_Suppr_Counter_Exceed_Threshold__flag_use_suppression_counter_true)
{
   /** \arrange flag suppression counter and suppression counter value to check */
   boolean_T result;
   cals.k_cta_f_use_object_supress_counter             = FBK_TRUE;
   object.persistent->obj_validity_suppression_counter = cals.k_cta_object_supress_counter + (uint8_t) 1;

   /** \action executes function to test */
   result = Cta_Does_Suppr_Counter_Exceed_Threshold(&object, &cals);

   /** \assert true since the flag to use the suppression counter is true while the suppression counter itself is above the
    * calibratable threshold */
   EXPECT_TRUE(result);
}

/**
 * Tests the obstruction probability below threshold functionality of object validation. Expect true since the flag check
 * obstruction probability is set to true, while the obstruction probability itself is below the calibratable threshold.
 * \uts{CSCSA-41987} \sdd{SF-3868} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Obstruction_Probability_Below_Threshold__flag_check_obstruction_probability_true)
{
   /** \arrange flag check obstruction probability and obstruction probability value to check */
   boolean_T result;
   cals.k_cta_f_check_obstruction_probability_signal = FBK_TRUE;
   object.tracker_data.obstruction_prob              = cals.k_cta_max_obstruction_probability - EPSILON;

   /** \action executes function to test */
   result = Cta_Is_Obstruction_Probability_Below_Threshold(&object, &cals);

   /** \assert true since the flag to check the obstruction probability is true while the obstruction probability itself is below
    * the calibratable threshold */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect true since the sub-function to check if the object is a
 * potential ghost is false and the sub-function to check if the potential ghost should be considered evaluates as true, but the
 * ghost detector is not used. \uts{CSCSA-41988} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_false_object_is_ghost_false_ghost_considered_true)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = 0.0f;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost + 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_FALSE;
   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert true since Cta_Is_Object_A_Potential_Ghost evaluates false and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * true and ghost detector flag is set to false */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect true since the sub-function to check if the object is a
 * potential ghost is true and the sub-function to check if the potential ghost should be considered evaluates as true, but the
 * ghost detector is not used. \uts{CSCSA-41989} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_false_object_is_ghost_true_ghost_considered_true)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_FALSE;

   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert true since Cta_Is_Object_A_Potential_Ghost evaluates true and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * true and ghost detector flag is set to false */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect true since the sub-function to check if the object is a
 * potential ghost is true and the sub-function to check if the potential ghost should be considered evaluates as false, but the
 * ghost detector is not used. \uts{CSCSA-41990} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_false_object_is_ghost_true_ghost_considered_false)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_FALSE;

   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert true since Cta_Is_Object_A_Potential_Ghost evaluates true and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * false and ghost detector flag is set to false */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect true since the sub-function to check if the object is a
 * potential ghost is false and the sub-function to check if the potential ghost should be considered evaluates as true, but the
 * ghost detector is used. \uts{CSCSA-41991} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_true_object_is_ghost_false_ghost_considered_true)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = 0.0f;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost + 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_TRUE;

   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert true since Cta_Is_Object_A_Potential_Ghost evaluates false and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * true and ghost detector flag is set to true */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect true since the sub-function to check if the object is a
 * potential ghost is true and the sub-function to check if the potential ghost should be considered evaluates as true, but the
 * ghost detector is used. \uts{CSCSA-41992} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_true_object_is_ghost_true_ghost_considered_true)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_TRUE;

   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert true since Cta_Is_Object_A_Potential_Ghost evaluates true and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * true and ghost detector flag is set to true */
   EXPECT_TRUE(result);
}

/**
 * Tests check against a potential ghost has passed functionality. Expect false since the sub-function to check if the object is a
 * potential ghost is true and the sub-function to check if the potential ghost should be considered evaluates as false, but the
 * ghost detector is used. \uts{CSCSA-41993} \sdd{SF-3857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test,
       Cta_Is_Check_Against_Potential_Ghost_Passed__flag_use_ghost_detector_true_object_is_ghost_true_ghost_considered_false)
{
   /** \arrange path tracking output, tracker output, and ghost detector flag to check */
   boolean_T result;
   /* Set-up for Cta_Is_Object_A_Potential_Ghost */
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;
   object.attributes->p_pt_match_info                              = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info                       = &(pt_output.nearest_path_output[0]);
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;
   /* Ghost detector flag */
   cals.k_cta_f_use_ghost_detector = FBK_TRUE;

   /** \action executes function to test */
   result = Cta_Is_Check_Against_Potential_Ghost_Passed(&object, &cals);

   /** \assert false since Cta_Is_Object_A_Potential_Ghost evaluates true and Cta_Should_Potential_Ghost_Be_Considered evaluates to
    * false and ghost detector flag is set to true */
   EXPECT_FALSE(result);
}

/**
 * Tests if objects are potential ghost functionality used within check for a potential ghost passed and therefore of object
 * validation. Expect true since sub-function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to true and
 * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to true. \uts{CSCSA-41994} \sdd{SF-3863} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_A_Potential_Ghost__heading_diff_true_cross_nearest_path_true)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);

   /** \action executes function to test */
   result = Cta_Is_Object_A_Potential_Ghost(&object, &cals);

   /** \assert true since the sub function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to true and
    * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to true */
   EXPECT_TRUE(result);
}

/**
 * Tests if objects are potential ghost functionality used within check for a potential ghost passed and therefore of object
 * validation. Expect true since sub-function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to true and
 * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to false. \uts{CSCSA-41995} \sdd{SF-3863} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_A_Potential_Ghost__heading_diff_true_cross_nearest_path_false)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost + 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);

   /** \action executes function to test */
   result = Cta_Is_Object_A_Potential_Ghost(&object, &cals);

   /** \assert true since the sub function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to true and
    * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to false */
   EXPECT_TRUE(result);
}

/**
 * Tests if objects are potential ghost functionality used within check for a potential ghost passed and therefore of object
 * validation. Expect true since sub-function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to false and
 * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to true. \uts{CSCSA-41996} \sdd{SF-3863} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_A_Potential_Ghost__heading_diff_false_cross_nearest_path_true)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);

   /** \action executes function to test */
   result = Cta_Is_Object_A_Potential_Ghost(&object, &cals);

   /** \assert true since the sub function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to false and
    * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to true */
   EXPECT_TRUE(result);
}

/**
 * Tests if objects are potential ghost functionality used within check for a potential ghost passed and therefore of object
 * validation. Expect false since sub-function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to false and
 * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to false. \uts{CSCSA-41997} \sdd{SF-3863} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_A_Potential_Ghost__heading_diff_false_cross_nearest_path_false)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   Pt_Output_T pt_output;
   /* arrange output for Cta_Is_Matchs_Head_Diff_Gt_Limits */
   pt_output.path_obj_pair_output[0].track_match  = 1;
   object.tracker_data.vcs_heading                = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;
   pt_output.path_obj_pair_output[0].path_heading = EPSILON;
   /* arrange output for Cta_Does_New_Obj_Cross_Nearest_Path */
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost + 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   object.tracker_data.age                                         = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);

   /** \action executes function to test */
   result = Cta_Is_Object_A_Potential_Ghost(&object, &cals);

   /** \assert false since the sub function Cta_Is_Matchs_Head_Diff_Gt_Limits evaluates to false and
    * Cta_Does_New_Obj_Cross_Nearest_Path evaluates to false */
   EXPECT_FALSE(result);
}

/**
 * Tests heading difference of a path-object pair is exceeding a given limit functionality used within check for a potential ghost
 * and therefore of object validation. Expect true since the track match is positive and the heading difference is above threshold.
 * \uts{CSCSA-41998} \sdd{SF-3860} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Matchs_Head_Diff_Gt_Limits__pt_track_match_positive_heading_difference_above_threshold)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   /* arrange path tracking output */
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match  = 1;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   object.attributes->p_pt_match_info             = &(pt_output.path_obj_pair_output[0]);
   /* arrange tracker output */
   object.tracker_data.vcs_heading = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;

   /** \action executes function to test */
   result = Cta_Is_Matchs_Head_Diff_Gt_Limits(&object, &cals);

   /** \assert true since the path tracking track match is positive and heading difference is above threshold */
   EXPECT_TRUE(result);
}

/**
 * Tests heading difference of a path-object pair is exceeding a given limit functionality used within check for a potential ghost
 * and therefore of object validation. Expect false since the path tracking output pointer is NULL. \uts{CSCSA-41999} \sdd{SF-3860}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Matchs_Head_Diff_Gt_Limits__pt_pointer_NULL)
{
   /** \arrange path tracking pointer to check */
   boolean_T result;
   object.attributes->p_pt_match_info = NULL;

   /** \action executes function to test */
   result = Cta_Is_Matchs_Head_Diff_Gt_Limits(&object, &cals);

   /** \assert false since the path tracking pointer is NULL */
   EXPECT_FALSE(result);
}

/**
 * Tests heading difference of a path-object pair is exceeding a given limit functionality used within check for a potential ghost
 * and therefore of object validation. Expect false since the track match is negative. \uts{CSCSA-42000} \sdd{SF-3860}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Matchs_Head_Diff_Gt_Limits__pt_track_match_negative)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   /* arrange path tracking output */
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match  = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   object.attributes->p_pt_match_info             = &(pt_output.path_obj_pair_output[0]);
   /* arrange tracker output */
   object.tracker_data.vcs_heading = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;

   /** \action executes function to test */
   result = Cta_Is_Matchs_Head_Diff_Gt_Limits(&object, &cals);

   /** \assert false since the path tracking track match is negative */
   EXPECT_FALSE(result);
}

/**
 * Tests heading difference of a path-object pair is exceeding a given limit functionality used within check for a potential ghost
 * and therefore of object validation. Expect false since the difference between vcs heading and path heading is positive and lt
 * the calibratable value. \uts{CSCSA-42001} \sdd{SF-3860} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Matchs_Head_Diff_Gt_Limits__vcs_heading_minus_path_heading_positive_lt_threshold)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   /* arrange path tracking output */
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match  = (int8_t) 1;
   pt_output.path_obj_pair_output[0].path_heading = EPSILON;
   object.attributes->p_pt_match_info             = &(pt_output.path_obj_pair_output[0]);
   /* arrange tracker output */
   object.tracker_data.vcs_heading = cals.k_cta_ghost_condition_max_heading_diff_path_tracker;

   /** \action executes function to test */
   result = Cta_Is_Matchs_Head_Diff_Gt_Limits(&object, &cals);

   /** \assert false since the heading difference is positive and lt threshold */
   EXPECT_FALSE(result);
}

/**
 * Tests heading difference of a path-object pair is exceeding a given limit functionality used within check for a potential ghost
 * and therefore of object validation. Expect false since the difference between vcs heading and path heading is negative and lt
 * the calibratable value. \uts{CSCSA-42002} \sdd{SF-3860} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Matchs_Head_Diff_Gt_Limits__vcs_heading_minus_path_heading_negative_lt_threshold)
{
   /** \arrange path tracking output and track match to check */
   boolean_T result;
   /* arrange path tracking output */
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match  = (int8_t) 1;
   pt_output.path_obj_pair_output[0].path_heading = -EPSILON;
   object.attributes->p_pt_match_info             = &(pt_output.path_obj_pair_output[0]);
   /* arrange tracker output */
   object.tracker_data.vcs_heading = -cals.k_cta_ghost_condition_max_heading_diff_path_tracker;

   /** \action executes function to test */
   result = Cta_Is_Matchs_Head_Diff_Gt_Limits(&object, &cals);

   /** \assert false since the heading difference is negative and lt threshold */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. Here a ghost object is created near a valid path. This tests is expected to be true.
 * \uts{CSCSA-42003} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__ghost_object_crosses_nearest_path)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match                   = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to be a ghost. */
   EXPECT_TRUE(result);
}

/**
 * Tests part of ghost detector functionality. Path tracking is disabled and thus this check shall return false.
 * \uts{CSCSA-42004} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__path_tracking_output_is_null)
{
   /** \arrange Path tracking information unavailable */
   boolean_T result;
   object.attributes->p_pt_nearest_path_info = NULL;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert expect test to return false */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. The sub-function Cta_Is_New_Obj_Matched_To_Path returns true, thus false is
 * expected. \uts{CSCSA-42005} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__object_is_matched_to_path_long_enough)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match                   = 0;
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. It is expected that this check returns false in case that nearest path information
 * is missing. \uts{CSCSA-42006} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__new_object_but_missing_path_information)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   /* Set-up Cta_Is_New_Obj_Matched_To_Path to return false */
   pt_output.path_obj_pair_output[0].track_match     = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].track_match_age = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   /* Arrange other path information */
   pt_output.nearest_path_output[0].track_idx_nearest_path         = PT_DEFAULT_MATCH_INDEX;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. It is expected that this check returns false in case that the age is not sufficient.
 * \uts{CSCSA-42007} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__new_object_with_sufficient_path_info_tracker_age_unsufficient)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   /* Set-up Cta_Is_New_Obj_Matched_To_Path to return false */
   pt_output.path_obj_pair_output[0].track_match     = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].track_match_age = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   /* Arrange other path information */
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths + 1u;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. New object follows its nearest path. It is expected that the object is not a ghost.
 * \uts{CSCSA-42008} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__new_object_follows_its_nearest_path_positive_heading)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match                   = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = 0.9f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. New object follows its nearest path. It is expected that the object is not a ghost.
 * \uts{CSCSA-42009} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__new_object_follows_its_nearest_path_negative_heading)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match                   = PT_DEFAULT_MATCH_INDEX;
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = -0.9f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 0.9f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. New has an threshold exceeding heading variance but is too far away from its nearest
 * path. \uts{CSCSA-42010} \sdd{SF-3853} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Does_New_Obj_Cross_Nearest_Path__new_object_is_too_far_away_from_its_nearest_path)
{
   /** \arrange 0 - Arrange */
   boolean_T result;
   Pt_Output_T pt_output;
   pt_output.path_obj_pair_output[0].track_match                   = 1;
   pt_output.path_obj_pair_output[0].track_match_age               = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   pt_output.nearest_path_output[0].track_idx_nearest_path         = 1;
   pt_output.nearest_path_output[0].segment_heading_diff           = 1.1f * cals.k_cta_max_seg_heading_diff_no_ghost;
   pt_output.nearest_path_output[0].range_vcs_proj_to_path_segment = 1.1f * cals.k_cta_range_to_path_segment_ghost_qualif;

   object.attributes->p_pt_match_info        = &(pt_output.path_obj_pair_output[0]);
   object.attributes->p_pt_nearest_path_info = &(pt_output.nearest_path_output[0]);
   object.tracker_data.age                   = cals.k_cta_min_qual_age_obj_crossing_paths - 1u;

   /** \action executes function to test */
   result = Cta_Does_New_Obj_Cross_Nearest_Path(&object, &cals);

   /** \assert Expect object to not be a ghost. */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. A new object is matched to a path with a sufficient confidence. The match exists a
 * specific amout of successive cycles and thus FBK_TRUE is expected \uts{CSCSA-42011} \sdd{SF-3861}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_New_Obj_Matched_To_Path__pt_track_match_age_gt_threshold_new_obj_is_matched_long_enough)
{
   /** \arrange Path tracking match information */
   boolean_T result;

   pt_match_info.track_match          = 0;
   pt_match_info.track_match_age      = cals.k_cta_cycles_valid_match_of_pot_ghost;
   object.attributes->p_pt_match_info = &(pt_match_info);

   /** \action executes function to test */
   result = Cta_Is_New_Obj_Matched_To_Path(&object, &cals);

   /** \assert expect test to return true */
   EXPECT_TRUE(result);
}

/**
 * Tests part of ghost detector functionality. A new object is not matched to a path. The match exists a specific amout of
 * successive cycles and thus FBK_FALSE is expected. \uts{CSCSA-42012} \sdd{SF-3861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_New_Obj_Matched_To_Path__pt_track_match_age_gt_threshold_new_obj_is_matched_not_long_enough)
{
   /** \arrange Path tracking match information */
   boolean_T result;
   pt_match_info.track_match          = PT_DEFAULT_MATCH_INDEX;
   pt_match_info.track_match_age      = cals.k_cta_cycles_valid_match_of_pot_ghost;
   object.attributes->p_pt_match_info = &(pt_match_info);

   /** \action executes function to test */
   result = Cta_Is_New_Obj_Matched_To_Path(&object, &cals);

   /** \assert expect test to return false */
   EXPECT_FALSE(result);
}

/**
 * Tests part of ghost detector functionality. A new object is matched to a path with a sufficient confidence. The match does not
 * exist a specific amout of successive cycles and thus FBK_FALSE is expected. \uts{CSCSA-42013} \sdd{SF-3861}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_New_Obj_Matched_To_Path__pt_track_match_age_lt_threshold_new_obj_is_matched_long_enough)
{
   /** \arrange Path tracking match information */
   boolean_T result;

   pt_match_info.track_match          = 0;
   pt_match_info.track_match_age      = cals.k_cta_cycles_valid_match_of_pot_ghost - 1u;
   object.attributes->p_pt_match_info = &(pt_match_info);

   /** \action executes function to test */
   result = Cta_Is_New_Obj_Matched_To_Path(&object, &cals);

   /** \assert expect test to return false */
   EXPECT_FALSE(result);
}

/**
 * Tests object to be considered as a ghost functionality. The object is expected to be considered as a ghost.
 * \uts{CSCSA-42014} \sdd{SF-3869} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Should_Potential_Ghost_Be_Considered__status_true_age_true_stage_age_true)
{
   /** \arrange tracker output to check */
   boolean_T result;
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;

   /** \action executes function to test */
   result = Cta_Should_Potential_Ghost_Be_Considered(&object, &cals);

   /** \assert true since status, age, and stage age evaluate as true within function */
   EXPECT_TRUE(result);
}

/**
 * Tests object to be considered as a ghost functionality. The object is not expected to be considered as a ghost.
 * \uts{CSCSA-42015} \sdd{SF-3869} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Should_Potential_Ghost_Be_Considered__status_false_age_true_stage_age_true)
{
   /** \arrange tracker output to check */
   boolean_T result;
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;

   /** \action executes function to test */
   result = Cta_Should_Potential_Ghost_Be_Considered(&object, &cals);

   /** \assert false since status evaluates as false and age as well as stage age evaluate as true within function */
   EXPECT_FALSE(result);
}

/**
 * Tests object to be considered as a ghost functionality. The object is not expected to be considered as a ghost.
 * \uts{CSCSA-42016} \sdd{SF-3869} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Should_Potential_Ghost_Be_Considered__status_true_age_false_stage_age_true)
{
   /** \arrange tracker output to check */
   boolean_T result;
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age - 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature + 1u;

   /** \action executes function to test */
   result = Cta_Should_Potential_Ghost_Be_Considered(&object, &cals);

   /** \assert false since status evaluates as true and age as false while stage age evaluates as true within function */
   EXPECT_FALSE(result);
}

/**
 * Tests object to be considered as a ghost functionality. The object is not expected to be considered as a ghost.
 * \uts{CSCSA-42017} \sdd{SF-3869} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Should_Potential_Ghost_Be_Considered__status_true_age_true_stage_age_false)
{
   /** \arrange tracker output to check */
   boolean_T result;
   /* Set-up for Cta_Should_Potential_Ghost_Be_Considered */
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.age       = cals.k_cta_ghost_validation_min_age + 1u;
   object.tracker_data.stage_age = cals.k_cta_ghost_validation_min_mature - 1u;

   /** \action executes function to test */
   result = Cta_Should_Potential_Ghost_Be_Considered(&object, &cals);

   /** \assert false since status and age evaluate as true while stage age evaluates as false within function */
   EXPECT_FALSE(result);
}

/**
 * Tests hysteresis functionality in object validation Since previously no criticality level is reached, the hysteresis shall not
 * be applied. \uts{CSCSA-42018} \sdd{SF-3855} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Fill_Object_Valid_Crit_With_Hyst__dont_apply_hys)
{
   /** \arrange thresholdsfor criticality level */
   CTA_Obj_Valid_Crit_T obj_valid_criteria;
   uint8_t mode = CTA_MODE_REAR;

   cals.k_cta_rel_warning_hysteresis              = 1.1f;
   object.attributes->approach_side               = FBK_SIDE_LEFT;
   object.persistent->prev_cycle_crit_level[mode] = CTA_CRIT_LEVEL_NONE;

   /** \action executes function to test */
   Cta_Fill_Object_Valid_Crit_With_Hyst(&object, &cals, &obj_valid_criteria);

   /** \assert criticality level thresholds filled with default values */
   EXPECT_FLOAT_EQ(obj_valid_criteria.cta_heading_range.min, cals.k_cta_heading_range[0u]);
   EXPECT_FLOAT_EQ(obj_valid_criteria.cta_heading_range.max, cals.k_cta_heading_range[1u]);
}

/**
 * Tests hysteresis functionality in object validation Since previously criticality level was reached, the hysteresis shall be
 * applied. \uts{CSCSA-42019} \sdd{SF-3855} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Fill_Object_Valid_Crit_With_Hyst__apply_hys)
{
   /** \arrange thresholdsfor criticality level */
   CTA_Obj_Valid_Crit_T obj_valid_criteria;
   uint8_t mode = CTA_MODE_REAR;

   cals.k_cta_rel_warning_hysteresis              = 1.5f;
   object.attributes->approach_side               = FBK_SIDE_LEFT;
   object.persistent->prev_cycle_crit_level[mode] = CTA_CRIT_LEVEL_1;
   /** \action executes function to test */
   Cta_Fill_Object_Valid_Crit_With_Hyst(&object, &cals, &obj_valid_criteria);

   /** \assert criticality level thresholds with hysteresis applied */
   EXPECT_NE(obj_valid_criteria.cta_heading_range.min, cals.k_cta_heading_range[0u]);
   EXPECT_NE(obj_valid_criteria.cta_heading_range.max, cals.k_cta_heading_range[1u]);
}

#ifndef NDEBUG
/**
 * Tests the hysteresis functionality of object validator An assertion shall be expected with an object critera set to NULL
 * \uts{CSCSA-42020} \sdd{SF-3855} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Fill_Object_Valid_Crit_With_Hyst___obj_valid_criteria_NULL)
{
   ASSERT_DEATH(
      {
         /** \arrange criteria set to NULL */
         /** \action executes function to test */
         Cta_Fill_Object_Valid_Crit_With_Hyst(&object, &cals, NULL);
         /** \assert expect assert since validity criteria points to NULL */
      },
      ".*p_obj_valid_criteria.*");
}
#endif

/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter shall be set to maximum
 * value, since path tracking is deactivated. \uts{CSCSA-42021} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_tracking_deactivated)
{
   /** \arrange lateral left path and cta approach from left side */
   object.persistent->obj_validity_suppression_counter = 0u;

   pt_match_info.path_direction       = PATH_DIRECTION_LAT_LEFT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   object.attributes->approach_side   = FBK_SIDE_LEFT;
   cals.k_cta_f_apply_path_tracking   = FBK_FALSE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall be increased to maximum value */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}

/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter be set to its maximum, since
 * no path tracking is disabled. \uts{CSCSA-42022} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__increase_counter_since_no_match_exists)
{
   /** \arrange NULL pointer to path tracking match */
   object.persistent->obj_validity_suppression_counter = 0u;
   object.attributes->p_pt_match_info                  = NULL;
   cals.k_cta_f_apply_path_tracking                    = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall be increased by 1 */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter shall be increased since path
 * direction (lateral left) and cta approach side (left) does not match. \uts{CSCSA-42023} \sdd{SF-3714}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_left_cta_approach_left)
{
   /** \arrange lateral left path and cta approach from left side */
   object.persistent->obj_validity_suppression_counter = 0u;

   pt_match_info.path_direction       = PATH_DIRECTION_LAT_LEFT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   object.attributes->approach_side   = FBK_SIDE_LEFT;
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall be increased by 1 */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, 1);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter shall be increased since path
 * direction (lateral right) and cta approach side (right) does not match. \uts{CSCSA-42024} \sdd{SF-3714}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_right_cta_approach_right)
{
   /** \arrange lateral right path and cta approach from right side */
   object.persistent->obj_validity_suppression_counter = 0u;

   object.attributes->approach_side   = FBK_SIDE_RIGHT;
   pt_match_info.path_direction       = PATH_DIRECTION_LAT_RIGHT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall be increased by 1 */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, 1);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter shall be set to its maximum
 * because the directions are matching. \uts{CSCSA-42025} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_left_cta_approach_right)
{
   /** \arrange lateral left path and cta approach from right side */
   object.persistent->obj_validity_suppression_counter = 0u;

   object.attributes->approach_side   = FBK_SIDE_RIGHT;
   pt_match_info.path_direction       = PATH_DIRECTION_LAT_LEFT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall remain */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. The counter shall be set to its maximum,
 * since the directions are opposites. \uts{CSCSA-42026} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_right_cta_approach_left)
{
   /** \arrange lateral right path and cta approach from left side */
   object.persistent->obj_validity_suppression_counter = 0u;

   object.attributes->approach_side   = FBK_SIDE_LEFT;
   pt_match_info.path_direction       = PATH_DIRECTION_LAT_RIGHT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter shall remain */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}

/**
 * Tests update functionality of object suppression counter based on path tracking input. Counter shall be set to a maximum, since
 * the path direction is not defined. \uts{CSCSA-42027} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_none_cta_approach_left)
{
   /** \arrange path with undefined direction and approach side left */
   object.persistent->obj_validity_suppression_counter = 0u;

   object.attributes->approach_side   = FBK_SIDE_LEFT;
   pt_match_info.path_direction       = PATH_DIRECTION_NONE;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter increase by 1 */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. path direction is lateral left and lat
 * velocity negative. \uts{CSCSA-42028} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_left_cta_approach_none_lat_vel_negative)
{
   /** \arrange path with lateral left direction and undefined approach side */
   object.persistent->obj_validity_suppression_counter = 0u;
   object.attributes->approach_side                    = FBK_SIDE_UNDEFINED;
   object.tracker_data.vcs_vel.y                       = -5.0f;

   pt_match_info.path_direction       = PATH_DIRECTION_LAT_LEFT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter increase to max */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. path direction is lateral left and lat
 * velocity negative. \uts{CSCSA-42029} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_lateral_right_cta_approach_none_lat_vel_positive)
{
   /** \arrange path with lateral left direction and object with an undefined approach side. */
   object.persistent->obj_validity_suppression_counter = 0u;
   object.attributes->approach_side                    = FBK_SIDE_UNDEFINED;

   pt_match_info.path_direction       = PATH_DIRECTION_LAT_RIGHT;
   object.attributes->p_pt_match_info = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking   = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Counter increase to max */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. Here a path match change has occured to a
 * path whose direction is not matching to the object moving direction. Thus a counter increment to one is expected.
 * \uts{CSCSA-42030} \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_match_has_changed)
{
   /** \arrange path with lateral right direction and a non matching direction of */
   object.persistent->obj_validity_suppression_counter = 0u;
   object.attributes->approach_side                    = FBK_SIDE_RIGHT;

   pt_match_info.path_direction         = PATH_DIRECTION_LAT_RIGHT;
   pt_match_info.track_match            = 2u;
   pt_match_info.track_match_last_cycle = 5u;
   object.attributes->p_pt_match_info   = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking     = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Expect counter increment by one */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, 1);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. Here a path match change has occured to a
 * path whose direction is matching to the object moving direction. Thus a counter shall be set to a maximum. \uts{CSCSA-42031}
 * \sdd{SF-3714} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Update_Validity_Suppression_Ctr__path_match_has_changed_to_matching_direction_path)
{
   /** \arrange path with lateral right direction a path match change to a path which is matching . */
   object.persistent->obj_validity_suppression_counter = 0u;
   object.attributes->approach_side                    = FBK_SIDE_LEFT;
   pt_match_info.path_direction                        = PATH_DIRECTION_LAT_RIGHT;
   pt_match_info.track_match                           = 2u;
   pt_match_info.track_match_last_cycle                = 5u;
   object.attributes->p_pt_match_info                  = &(pt_match_info);
   cals.k_cta_f_apply_path_tracking                    = FBK_TRUE;

   /** \action executes function to test */
   Cta_Update_Validity_Suppression_Ctr(&object, &cals);

   /** \assert Expect maximum value to be set. */
   EXPECT_EQ(object.persistent->obj_validity_suppression_counter, CTA_COUNTER_MAX);
}


/**
 * Checks whether an object is below the specified amount of ignore cycles. Here the object is mature and thus true is expected.
 * \uts{CSCSA-42032} \sdd{SF-3858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Coasted_Obj_Below_Ignore_Cycles__mature_obj_is_valid)
{
   /** \arrange set up a mature object. */
   boolean_T res;
   object.tracker_data.status = PA_OBJ_STATUS_MATURE;

   /** \action executes function to test */
   res = Cta_Is_Coasted_Obj_Below_Ignore_Cycles(&object, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an coasted implausible object is below the specified amount of ignore cycles. Here the object is mature and thus
 * true is expected. \uts{CSCSA-188376} \sdd{SF-3858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Coasted_Obj_Below_Ignore_Cycles__Coasted_Status_Implausible_valid)
{
   /** \arrange set up a mature object. */
   boolean_T res;
   object.tracker_data.status = PA_OBJ_STATUS_COASTED;

   /** \action executes function to test */
   res = Cta_Is_Coasted_Obj_Below_Ignore_Cycles(&object, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is below the specified amount of ignore cycles. Here the object is coasted but its age is still in a
 * valid range. \uts{CSCSA-42033} \sdd{SF-3858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Coasted_Obj_Below_Ignore_Cycles__coasted_obj_is_valid)
{
   /** \arrange set up a valid coasted object. */
   boolean_T res;
   cals.k_cta_cycles_coasted_to_ignore       = 3u;
   object.tracker_data.status                = PA_OBJ_STATUS_COASTED;
   object.tracker_data.f_is_in_rl_sensor_fov = FBK_TRUE;
   object.tracker_data.stage_age             = cals.k_cta_cycles_coasted_to_ignore;

   /** \action executes function to test */
   res = Cta_Is_Coasted_Obj_Below_Ignore_Cycles(&object, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is below the specified amount of ignore cycles. Here the object is coasted and its age is invalid.
 * \uts{CSCSA-42034} \sdd{SF-3858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Coasted_Obj_Below_Ignore_Cycles__coasted_obj_is_invalid)
{
   /** \arrange set up a invalid coasted object. */
   boolean_T res;
   cals.k_cta_cycles_coasted_to_ignore       = 3u;
   object.tracker_data.status                = PA_OBJ_STATUS_COASTED;
   object.tracker_data.f_is_in_rl_sensor_fov = FBK_TRUE;
   object.tracker_data.stage_age             = cals.k_cta_cycles_coasted_to_ignore + 1u;

   /** \action executes function to test */
   res = Cta_Is_Coasted_Obj_Below_Ignore_Cycles(&object, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether an the existence probability is valid. Here the the probability is in a valid range and thus true is expected.
 * \uts{CSCSA-42035} \sdd{SF-3862} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Obj_Exist_Prob_Above_Threshold__probability_valid)
{
   /** \arrange set up a valid existence probability. */
   boolean_T res;
   float32_T min_existence_probability_obj   = 0.6f;
   object.tracker_data.existence_probability = min_existence_probability_obj * 1.1f;

   /** \action executes function to test */
   res = Cta_Is_Obj_Exist_Prob_Above_Threshold(&object, min_existence_probability_obj);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is classified as reflection by the tracker. Here it is classified as a reflection and thus true is
 * returned. \uts{CSCSA-42036} \sdd{SF-3864} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_A_Reflection__obj_is_reflection)
{
   /** \arrange set up a reflection object. */
   boolean_T res;
   object.tracker_data.f_reflection     = FBK_TRUE;
   cals.k_cta_f_check_reflection_signal = FBK_TRUE;

   /** \action executes function to test */
   res = Cta_Is_Object_A_Reflection(&object, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an objects age is in a valid range.Here the object age is in a valid area and thus true is returned.
 * \uts{CSCSA-42037} \sdd{SF-3865} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_Object_Age_Above_Threshold__obj_age_is_above_threshold)
{
   /** \arrange set up an old enough object. */
   boolean_T res;
   cals.k_cta_min_object_age_check_valid            = 3u;
   cals.k_cta_f_use_object_min_object_age_in_cycles = FBK_TRUE;
   object.tracker_data.age                          = cals.k_cta_min_object_age_check_valid + 1u;

   /** \action executes function to test */
   res = Cta_Is_Object_Age_Above_Threshold(&object, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether the hysteresis enum is chosen correctly. Here a criticality level was reached earlier and thus an active
 * hysteresis is expected \uts{CSCSA-42038} \sdd{SF-3856} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Get_Hysteresis_Level__hysteresis_rear)
{
   /** \arrange set up a previously critical object. */
   Hysteresis_Level_T hysteresis_level;
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR]  = CTA_CRIT_LEVEL_1;
   object.persistent->prev_cycle_crit_level[CTA_MODE_FRONT] = CTA_CRIT_LEVEL_1;

   /** \action executes function to test */
   hysteresis_level = Cta_Get_Hysteresis_Level(&object);

   /** \assert Expect hysteresis to be active. */
   EXPECT_EQ(hysteresis_level, HYST_LEVEL_ACTIVE_PREV_WARN);
}

/**
 * Checks whether the hysteresis enum is chosen correctly. Here a criticality level was reached earlier and thus an active
 * hysteresis is expected \uts{CSCSA-188377} \sdd{SF-3856} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Get_Hysteresis_Level__hysteresis_front)
{
   /** \arrange set up a previously critical object. */
   Hysteresis_Level_T hysteresis_level;
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR]  = CTA_CRIT_LEVEL_NONE;
   object.persistent->prev_cycle_crit_level[CTA_MODE_FRONT] = CTA_CRIT_LEVEL_1;

   /** \action executes function to test */
   hysteresis_level = Cta_Get_Hysteresis_Level(&object);

   /** \assert Expect hysteresis to be active. */
   EXPECT_EQ(hysteresis_level, HYST_LEVEL_ACTIVE_PREV_WARN);
}

/**
 * Check that an car object is classified as valid for CTA.
 * \uts{CSCSA-145891} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_car)
{
   /** \arrange Set up calibrations and tracker output such that object is valid. */
   boolean_T result;

   object.tracker_data.obj_class = PA_OBJ_CLASS_CAR;
   object.tracker_data.length    = 4.5f;
   object.tracker_data.width     = 2.0f;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that an slow moving pedestrian object is classified valid for CTA.
 * \uts{CSCSA-145892} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_slow_pedestrian_but_size_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object.tracker_data.length    = len + EPSILON;
   object.tracker_data.width     = width + EPSILON;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed - EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an slow moving unknown class object is classified valid for CTA.
 * \uts{CSCSA-145893} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_is_slow_moving_but_size_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_UNKNOWN;
   object.tracker_data.length    = len + EPSILON;
   object.tracker_data.width     = width + EPSILON;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed - EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that a pedestrian object is classified valid for CTA when its size is below threshold
 * \uts{CSCSA-145894} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_pedestrian_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object.tracker_data.length    = len - EPSILON;
   object.tracker_data.width     = width - EPSILON;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that a unknown object is classified valid for CTA when its size is below threshold
 * \uts{CSCSA-145895} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_UNKNOWN;
   object.tracker_data.length    = len - EPSILON;
   object.tracker_data.width     = width - EPSILON;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an pedestrian object is classified as valid for CTA.
 * \uts{CSCSA-145896} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_pedestrian_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;

   object.tracker_data.obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object.tracker_data.length    = 1.0f;
   object.tracker_data.width     = 1.0f;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an unknwon object is classified as valid for CTA.
 * \uts{CSCSA-145897} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;

   object.tracker_data.obj_class = PA_OBJ_CLASS_UNKNOWN;
   object.tracker_data.length    = 1.0f;
   object.tracker_data.width     = 1.0f;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that a pedestrian object is classified as invalid for CTA.
 * \uts{CSCSA-145898} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_FALSE_if_pedestrian_object_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_pedestrian_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_pedestrian_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object.tracker_data.length    = len - EPSILON;
   object.tracker_data.width     = width - EPSILON;
   object.tracker_data.speed     = cals.k_cta_pedestrian_min_speed - EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check that a slow moving 2wheel object is classified as valid for CTA.
 * \uts{CSCSA-145899} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_slow_2wheel_but_size_overthreshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_2wheel_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_2wheel_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_2WHEEL;
   object.tracker_data.length    = len + EPSILON;
   object.tracker_data.width     = width + EPSILON;
   object.tracker_data.speed     = cals.k_cta_2wheel_min_speed - EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that a small 2whell object is classified as valid for CTA.
 * \uts{CSCSA-145900} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_2wheel_object_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_2wheel_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_2wheel_min_size);

   object.tracker_data.obj_class = PA_OBJ_CLASS_2WHEEL;
   object.tracker_data.length    = len - EPSILON;
   object.tracker_data.width     = width - EPSILON;
   object.tracker_data.speed     = cals.k_cta_2wheel_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an 2wheel object is classified as valid for CTA.
 * \uts{CSCSA-145901} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_TRUE_if_2wheel_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is valid. */
   boolean_T result;

   object.tracker_data.obj_class = PA_OBJ_CLASS_2WHEEL;
   object.tracker_data.length    = 3.0f;
   object.tracker_data.width     = 1.2f;
   object.tracker_data.speed     = cals.k_cta_2wheel_min_speed + EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an 2wheel object is classified as invalid for CTA.
 * \uts{CSCSA-145902} \sdd{CSCSA-145874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Object_Validator_Test, Cta_Is_VRU_Object_Valid__returns_FALSE_if_2wheel_object_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   boolean_T result;
   float32_T len   = Fast_Sqrt(cals.k_cta_2wheel_min_size);
   float32_T width = Fast_Sqrt(cals.k_cta_2wheel_min_size);


   object.tracker_data.obj_class = PA_OBJ_CLASS_2WHEEL;
   object.tracker_data.length    = len - EPSILON;
   object.tracker_data.width     = width - EPSILON;
   object.tracker_data.speed     = cals.k_cta_2wheel_min_speed - EPSILON;

   /** \action Call Cta_Is_VRU_Object_Valid to evaluate if object is valid for CTA. */
   result = Cta_Is_VRU_Object_Valid(&object, &cals);

   /** \assert Verify that object is classified as invalid. */
   EXPECT_FALSE(result);
}
