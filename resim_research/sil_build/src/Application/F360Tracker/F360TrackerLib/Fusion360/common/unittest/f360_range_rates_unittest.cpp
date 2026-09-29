/** \file
 * This file contains unit tests for content of f360_range_rates.cpp file
 */

#include "f360_range_rates.h"
#include "f360_math.h"
#include "f360_constants.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

 /** \defgroup  f360_range_rates
  *  @{
  */
  /** \brief
   * Test group for testing Calculate_Det_Range_Rate_Comp()
   */
TEST_GROUP(f360_range_rates__Calculate_Det_Range_Rate_Comp)
{
   const float32_t floating_threshold = 0.0001F;
};

/** \purpose
 * Calculate_Det_Range_Rate_Comp should return correct value of compensated range rate when sensor does not move
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_Det_Range_Rate_Comp, calculation_correctness_stationary_sensor)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(60.0F);
   float32_t range_rate_raw = 5.0F;
   float32_t sen_lat_vel_vcs = 0.0F;
   float32_t sen_long_vel_vcs = 0.0F;

   /** \action
    * Call Calculate_Det_Range_Rate_Comp()
    */
   float32_t result = Calculate_Det_Range_Rate_Comp(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), range_rate_raw, sen_lat_vel_vcs, sen_long_vel_vcs);

   /** \result
    * Not changed raw value
    */
   DOUBLES_EQUAL(range_rate_raw, result, floating_threshold);
}

/** \purpose
 * Calculate_Det_Range_Rate_Comp should return correct value of compensated range rate when sensor moves and azimuth and lat velocty are positive
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_Det_Range_Rate_Comp, calculation_correctness_moving_sensor_positive_azimuth)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(60.0F);
   float32_t range_rate_raw = 5.0F;
   float32_t sen_lat_vel_vcs = 1.0F;
   float32_t sen_long_vel_vcs = 3.0F;

   /** \action
    * Call Calculate_Det_Range_Rate_Comp()
    */
   float32_t result = Calculate_Det_Range_Rate_Comp(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), range_rate_raw, sen_lat_vel_vcs, sen_long_vel_vcs);

   DOUBLES_EQUAL(7.3660F, result, floating_threshold);
}

/** \purpose
 * Calculate_Det_Range_Rate_Comp should return correct value of compensated range rate when sensor moves and azimuth and lat velocty are negative
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_Det_Range_Rate_Comp, calculation_correctness_moving_sensor_negative_azimuth)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(-60.0F);
   float32_t range_rate_raw = 5.0F;
   float32_t sen_lat_vel_vcs = -1.0F;
   float32_t sen_long_vel_vcs = 3.0F;

   /** \action
    * Call Calculate_Det_Range_Rate_Comp()
    */
   float32_t result = Calculate_Det_Range_Rate_Comp(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), range_rate_raw, sen_lat_vel_vcs, sen_long_vel_vcs);

   DOUBLES_EQUAL(7.3660F, result, floating_threshold);
}

/** \purpose
 * Calculate_Det_Range_Rate_Comp should return correct value of compensated range rate for 0 azimuth when sensor moves
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_Det_Range_Rate_Comp, calculation_correctness_moving_sensor_zero_azimuth)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(0);
   float32_t range_rate_raw = 5.0F;
   float32_t sen_lat_vel_vcs = 1.0F;
   float32_t sen_long_vel_vcs = 3.0F;

   /** \action
    * Call Calculate_Det_Range_Rate_Comp()
    */
   float32_t result = Calculate_Det_Range_Rate_Comp(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), range_rate_raw, sen_lat_vel_vcs, sen_long_vel_vcs);

   DOUBLES_EQUAL(8.0F, result, floating_threshold);
}
/** @}*/


/** \defgroup  f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity
 *  @{
 */
 /** \brief
  * Test group for testing Calculate_OTG_Range_Rate_From_Velocity()
  */
TEST_GROUP(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity)
{
   const float32_t floating_threshold = 0.0001F;
};

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate when velocity is zero
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_zero_velocity)
{
   float32_t azimuth_vcs = F360_DEG2RAD(60.0F);
   float32_t long_vel_vcs = 0.0F;
   float32_t lat_vel_vcs = 0.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(azimuth_vcs), F360_Sinf(azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(0.0F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate when velocity is non-zero
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(60.0F);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(2.366025F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate for 0 azimuth when non-zero velocity
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity_zero_azimuth)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(0);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(3.0F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate for 90 azimuth when non-zero velocity
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity_azimuth_90)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(90);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(1.0F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate for -90 azimuth when non-zero velocity
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity_azimuth_neg_90)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(-90);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(-1.0F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate when azimuth is above 120 deg and velocity iz non-zero
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity_azimuth_above_120)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(121.0F);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(-0.6879F, result, floating_threshold);
}

/** \purpose
 * Calculate_OTG_Range_Rate_From_Velocity should return correct value of compensated range rate when azimuth is below -120 deg and velocity iz non-zero
 * \req
 *  NA.
 */
TEST(f360_range_rates__Calculate_OTG_Range_Rate_From_Velocity, calculation_correctness_non_zero_velocity_azimuth_below_neg_120)
{
   float32_t det_azimuth_vcs = F360_DEG2RAD(-121.0F);
   float32_t long_vel_vcs = 3.0F;
   float32_t lat_vel_vcs = 1.0F;

   /** \action
    * Call Calculate_OTG_Range_Rate_From_Velocity()
    */
   float32_t result = Calculate_OTG_Range_Rate_From_Velocity(F360_Cosf(det_azimuth_vcs), F360_Sinf(det_azimuth_vcs), long_vel_vcs, lat_vel_vcs);

   DOUBLES_EQUAL(-2.4023F, result, floating_threshold);
}
/** @}*/

/** \defgroup  Calculate_Projected_Range_Rate__Basics
 *  @{
 */

 /** \brief
  * Group for checking basic calculations of range rate
  */
TEST_GROUP(Calculate_Projected_Range_Rate__Basics)
{
   const Point view_point = { 1.0F, 2.0F };
   const Point item_pos = { 5.0F, 6.0F };
};

/** \purpose
 * Check calcualtion correctness when object approaches directly to the view point
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_approaches_directly)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -2.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -2.828427F
    */
   DOUBLES_EQUAL(-2.828427F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object approaches indirectly to the view point (velocity vector points above the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_approaches_indirectly_above)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -2.0F, -1.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -2.12132F
    */
   DOUBLES_EQUAL(-2.12132F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object approaches indirectly to the view point (velocity vector points below the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_approaches_indirectly_below)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -1.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -2.12132F
    */
   DOUBLES_EQUAL(-2.12132F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves away directly from the view point.
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_moves_away_directly)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { 2.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.828427F
    */
   DOUBLES_EQUAL(2.828427F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves away indirectly from the view point (velocity vector points above the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_moves_away_indirectly_above)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { 2.0F, 4.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 4.24264
    */
   DOUBLES_EQUAL(4.24264, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves away indirectly from the view point (velocity vector points below the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_moves_away_indirectly_below)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { 4.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 4.24264F
    */
   DOUBLES_EQUAL(4.24264F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object does not move
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, stopped_object)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { 0.0F, 0.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves perpendicularly to the view point (counterclockwise)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_moves_perpendicularly__counterclockwise)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -2.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves perpendicularly to the view point (clockwise)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Basics, object_moves_perpendicularly__clockwise)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { 2.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}
/** @}*/


/** \defgroup  Calculate_Projected_Range_Rate__Swapped_Positions
 *  @{
 */

 /** \brief
  * Group for checking basic calculations of range rate. It is duplication of tests from Calculate_Projected_Range_Rate__Basics
  * but with swapped positions of object and view point
  */
TEST_GROUP(Calculate_Projected_Range_Rate__Swapped_Positions)
{
   //Take setup from Calculate_Projected_Range_Rate__Basics group and swap object and view point position.
   const Point view_point = { 5.0F, 6.0F };
   const Point item_pos = { 1.0F, 2.0F };
};

/** \purpose
 * Check calcualtion correctness when object moves away directly from the view point.
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_moves_away_directly)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -2.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.828427F
    */
   DOUBLES_EQUAL(2.828427F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves away indirectly from the view point (velocity vector points above the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_moves_away_indirectly_above)
{
   /** \precond
    * Set object's velocity
    */
   const F360_VCS_Velocity_T item_velocity = { -2.0F, -1.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.12132F
    */
   DOUBLES_EQUAL(2.12132F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves away indirectly from the view point (velocity vector points below the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_moves_away_indirectly_below)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { -1.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.12132F
    */
   DOUBLES_EQUAL(2.12132F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object approaches directly to the view point
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_approaches_directly)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { 2.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.828427F
    */
   DOUBLES_EQUAL(-2.828427F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object approaches indirectly to the view point (velocity vector points above the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_approaches_indirectly_above)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { 2.0F, 4.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -4.24264F
    */
   DOUBLES_EQUAL(-4.24264F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object approaches indirectly to the view point (velocity vector points below the crossing line)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_approaches_indirectly_below)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { 4.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -4.24264F
    */
   DOUBLES_EQUAL(-4.24264F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object does not move
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, stopped_object)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { 0.0F, 0.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves perpendicularly to the view point (counterclockwise)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_moves_perpendicularly__counterclockwise)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { -2.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object moves perpendicularly to the view point (clockwise)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Swapped_Positions, object_moves_perpendicularly__clockwise)
{
   /** \precond
    * Set object's velocity
    */
   
   const F360_VCS_Velocity_T item_velocity = { 2.0F, -2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 0.0F
    */
   DOUBLES_EQUAL(0.0F, result, 0.0001F);
}
/** @}*/

/** \defgroup  Calculate_Projected_Range_Rate__Same_Positions
 *  @{
 */

 /** \brief
  * Group for checking corner cases. When at least one position parameter is the same
  */
TEST_GROUP(Calculate_Projected_Range_Rate__Same_Positions)
{
};

/** \purpose
 * Check calcualtion correctness when object and view point have the same position
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Same_Positions, same_position)
{
   /** \precond
    * Set object's velocity and position
    * Set veiw point's position
    */  
   const Point view_point = { 5.0F, 6.0F };
   const Point item_pos = { 5.0F, 6.0F };
   const F360_VCS_Velocity_T item_velocity = { 1.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to NaN
    */
   CHECK_TRUE(std::isnan(result));
}

/** \purpose
 * Check calcualtion correctness when object and view point have the same longitudinal position (objeect is above)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Same_Positions, same_longitudinal_position_object_is_above)
{
   /** \precond
    * Set object's velocity and position
    * Set veiw point's position
    */   
   const Point view_point = { 5.0F, 6.0F };;
   const Point item_pos = { 5.0F, 7.0F };
   const F360_VCS_Velocity_T item_velocity = { 1.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 2.0F
    */
   DOUBLES_EQUAL(2.0F, result, 0.0001F);
}

/** \purpose
 * Check calcualtion correctness when object and view point have the same longitudinal position (objeect is below)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Same_Positions, same_longitudinal_position_objeect_is_below)
{
   /** \precond
    * Set object's velocity and position
    * Set veiw point's position
    */
   const Point view_point = { 5.0F, 7.0F };
   const Point item_pos = { 5.0F, 6.0F };
   const F360_VCS_Velocity_T item_velocity = { 1.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -2.0F
    */
   DOUBLES_EQUAL(-2.0F, result, 0.0001F);
}

/** \purpose
* Check calcualtion correctness when object and view point have the same lateral position (object is on the left)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Same_Positions, same_lateral_position_object_is_on_the_left)
{
   /** \precond
    * Set object's velocity and position
    * Set veiw point's position
    */
   const Point view_point = { 5.0F, 6.0F };
   const Point item_pos = { 4.0F, 6.0F };
   const F360_VCS_Velocity_T item_velocity = { 1.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to -1.0F
    */
   DOUBLES_EQUAL(-1.0F, result, 0.0001F);
}

/** \purpose
* Check calcualtion correctness when object and view point have the same lateral position (object is on the right)
 * \req
 * NA
 */
TEST(Calculate_Projected_Range_Rate__Same_Positions, same_lateral_position_object_is_one_the_right)
{
   /** \precond
    * Set object's velocity and position
    * Set veiw point's position
    */
   const Point view_point = { 4.0F, 6.0F };
   const Point item_pos = { 5.0F, 6.0F };
   const F360_VCS_Velocity_T item_velocity = { 1.0F, 2.0F };

   /** \action
    * Call Calculate_Projected_Range_Rate()
    */
   const float result = Calculate_Projected_Range_Rate(view_point, item_pos, item_velocity);

   /** \result
    * Range rate equal to 1.0F
    */
   DOUBLES_EQUAL(1.0F, result, 0.0001F);
}

/** @}*/


/** \defgroup  Calc_Range_Rate_Difference_From_Expectations__functionality
 *  @{
 */
 /** \brief
  * Test group for testing Calc_Range_Rate_Difference_From_Expectations()
  */
TEST_GROUP(Calc_Range_Rate_Difference_From_Expectations__functionality)
{
   const float32_t floating_threshold = 0.0001F;

   F360_VCS_Velocity_T obj_velocity_vcs;

   /** \setup
   * Set object's velocity
   **/
   TEST_SETUP()
   {
      obj_velocity_vcs.lateral = 1.0F;
      obj_velocity_vcs.longitudinal = 3.0F;
   }
};

/** \purpose
 * Calc_Range_Rate_Difference_From_Expectations should correctly calculate difference if detection range rate compensated is 0
 * \req
 *  NA.
 */
TEST(Calc_Range_Rate_Difference_From_Expectations__functionality, calculation_correctness_0_range_rate)
{
   float32_t det_az_vcs = F360_DEG2RAD(60.0F);
   float32_t det_rr_comp = 0.0F;

   /** \action
    * Call Calc_Range_Rate_Difference_From_Expectations()
    */
   float32_t result = Calc_Range_Rate_Difference_From_Expectations(obj_velocity_vcs, F360_Cosf(det_az_vcs), F360_Sinf(det_az_vcs), det_rr_comp);

   DOUBLES_EQUAL(2.366025F, result, floating_threshold);
}

/** \purpose
   * Calc_Range_Rate_Difference_From_Expectations should correctly calculate difference if detection range rate compensated is positive
   * \req
   *  NA.
   */
TEST(Calc_Range_Rate_Difference_From_Expectations__functionality, calculation_correctness_positive_range_rate)
{
   float32_t det_az_vcs = F360_DEG2RAD(60.0F);
   float32_t det_rr_comp = 2.0F;

   /** \action
      * Call Calc_Range_Rate_Difference_From_Expectations()
      */
   float32_t result = Calc_Range_Rate_Difference_From_Expectations(obj_velocity_vcs, F360_Cosf(det_az_vcs), F360_Sinf(det_az_vcs), det_rr_comp);

   DOUBLES_EQUAL(0.366025F, result, floating_threshold);
}

/** \purpose
   * Calc_Range_Rate_Difference_From_Expectations should correctly calculate difference if detection range rate compensated is negative
   * \req
   *  NA.
   */
TEST(Calc_Range_Rate_Difference_From_Expectations__functionality, calculation_correctness_negative_range_rate)
{
   float32_t det_az_vcs = F360_DEG2RAD(60.0F);
   float32_t det_rr_comp = -2.0F;

   /** \action
      * Call Calc_Range_Rate_Difference_From_Expectations()
      */
   float32_t result = Calc_Range_Rate_Difference_From_Expectations(obj_velocity_vcs, F360_Cosf(det_az_vcs), F360_Sinf(det_az_vcs), det_rr_comp);

   DOUBLES_EQUAL(4.366025F, result, floating_threshold);
}
/** @}*/