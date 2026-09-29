/** \file
 * This file contains unit tests for content of f360_update_aeb_confidence_helpers.cpp file
 */

#include "f360_update_aeb_confidence_helpers.h"
#include <CppUTest/TestHarness.h>
#include "f360_calibrations.h"

using namespace f360_variant_A;

/** \defgroup  f360_update_aeb_confidence_helpers
 *  @{
 */

/** \brief
 * Tests for Compute_Iso_Relative_X_Vel() helper function.
 */
TEST_GROUP(f360_update_aeb_confidence_helpers)
{
   F360_Object_Track_T object = {};
   F360_Host_T host = {};
   const float32_t tolerance = 1e-3F;

   /** \setup
    * Initialize object and host to baseline values.
    * Object at (50, 0) heading straight ahead, zero curvature,
    * host driving straight at 20 m/s with no sideslip and no yaw rate.
    */
   TEST_SETUP()
   {
      object.vcs_position.Set_Position(50.0F, 0.0F);
      object.bbox.Set_Length(4.5F);
      object.bbox.Set_Width(1.8F);
      object.bbox.Set_Orientation(Angle(0.0F));
      object.reference_point = F360_REFERENCE_POINT_FRONT;
      object.vcs_velocity.longitudinal = -10.0F;
      object.vcs_velocity.lateral = 0.0F;
      object.heading_rate = 0.0F;
      object.vcs_heading.Value(0.0F);

      host.vcs_speed = 20.0F;
      host.vcs_sideslip = 0.0F;
      host.yaw_rate_rad = 0.0F;
   }
};

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel returns correct relative velocity
 * when both host and object are driving straight ahead with no yaw rate.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Straight_Ahead_No_Yaw_Rate)
{
   /** \precond
    * Object at x=50, y=0, moving at -10 m/s longitudinally (approaching).
    * Host at 20 m/s, no sideslip, no yaw rate. Zero heading rate on object.
    */

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_velocity_x = vcs_speed * cos(sideslip) = 20.0 * cos(0) = 20.0
    * result = -10.0 - 20.0 - 0 = -30.0
    */
   DOUBLES_EQUAL(-30.0F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel accounts for host sideslip
 * by reducing effective host longitudinal speed.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Host_Has_Sideslip)
{
   /** \precond
    * Host has nonzero sideslip angle of 0.1 rad.
    */
   host.vcs_sideslip = 0.1F;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_velocity_x = vcs_speed * cos(sideslip) = 20.0 * cos(0.1) ~= 19.900
    * result = -10.0 - 19.900 - 0 ~= -29.9
    */
   DOUBLES_EQUAL(-29.9F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel accounts for host yaw rate
 * when object is laterally offset to the right (positive y), confirming correct
 * sign of the yaw rate coupling term.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Host_Has_Yaw_Rate_And_Right_Lateral_Offset)
{
   /** \precond
    * Object offset to the right at y = 3.0 m.
    * Host has yaw rate of 0.05 rad/s.
    */
   object.vcs_position.Set_Position(50.0F, 3.0F);
   host.yaw_rate_rad = 0.05F;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_yaw_coupling_x = -yaw_rate_rad * y = -0.05 * 3.0 = -0.15
    * result = -10.0 - 20.0 - (-0.15) = -10.0 - 20.0 + 0.15 = -29.85
    */
   DOUBLES_EQUAL(-29.85F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel accounts for host yaw rate
 * when object is laterally offset to the left (negative y), confirming correct
 * sign of the yaw rate coupling term.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Host_Has_Yaw_Rate_And_Left_Lateral_Offset)
{
   /** \precond
    * Object offset to the left at y = -3.0 m.
    * Host has yaw rate of 0.05 rad/s.
    */
   object.vcs_position.Set_Position(50.0F, -3.0F);
   host.yaw_rate_rad = 0.05F;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_yaw_coupling_x = -yaw_rate_rad * y = -0.05 * (-3.0) = +0.15
    * result = -10.0 - 20.0 - 0.15 = -30.15
    */
   DOUBLES_EQUAL(-30.15F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel accounts for nonzero object heading rate
 * when computing velocity at the reference point for a CTCA object.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__CTCA_Object_Has_Heading_Rate)
{
   /** \precond
    * Object has CTCA filter type and nonzero heading rate of 0.1 rad/s.
    * Use FRONT_RIGHT reference point so there is a lateral offset from rear center,
    * which makes the heading_rate cross product contribute to the X velocity.
    */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object.heading_rate = 0.1F;
   object.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * vec_from_center_to_ref_tcs = (length/2, width/2) = (2.25, 0.9)
    * vec_from_rear_center_to_ref_tcs = (2.25 + 4.5/2, 0.9) = (4.5, 0.9)
    * heading_rate_cross_product_x = -heading_rate * vec_y = -0.1 * 0.9 = -0.09
    * longi_vcs_velocity_in_ref_pnt = -10.0 + (-0.09) = -10.09
    * result = -10.09 - 20.0 - 0 = -30.09
    */
   DOUBLES_EQUAL(-30.09F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel returns zero relative velocity
 * when object and host have matching speeds moving in the same direction.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Same_Speed_Same_Direction)
{
   /** \precond
    * Object longitudinal velocity equals host speed (both moving forward at 20 m/s).
    * Reference point is front center, object heading = 0.
    */
   object.vcs_velocity.longitudinal = 20.0F;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_velocity_x = vcs_speed * cos(sideslip) = 20.0 * cos(0) = 20.0
    * result = 20.0 - 20.0 - 0 = 0.0
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Verify that Compute_Iso_Relative_X_Vel handles a stationary host
 * and only returns the object longitudinal velocity at the reference point.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__Stationary_Host)
{
   /** \precond
    * Host is stationary (vcs_speed = 0).
    */
   host.vcs_speed = 0.0F;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * host_velocity_x = vcs_speed * cos(sideslip) = 0.0 * cos(0) = 0.0
    * result = -10.0 - 0.0 - 0 = -10.0
    */
   DOUBLES_EQUAL(-10.0F, result, tolerance);
}

/** \purpose
 * Verify that for a CCA object, nonzero heading rate does not affect the
 * computed relative velocity because the rear-center-to-reference-point
 * transformation is only applied to CTCA objects.
 * \req
 * NA
 */
TEST(f360_update_aeb_confidence_helpers, Compute_Iso_Relative_X_Vel__CCA_Object_Heading_Rate_Ignored)
{
   /** \precond
    * Object has CCA filter type, nonzero heading rate, and FRONT_RIGHT reference point.
    */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.heading_rate = 0.1F;
   object.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call Compute_Iso_Relative_X_Vel.
    */
   const float32_t result = Compute_Iso_Relative_X_Vel(object, host);

   /** \result
    * longi_vcs_velocity_in_ref_pnt = vcs_velocity.longitudinal = -10.0 (heading rate not applied for CCA)
    * result = -10.0 - 20.0 - 0 = -30.0
    */
   DOUBLES_EQUAL(-30.0F, result, tolerance);
}

/** @}*/
