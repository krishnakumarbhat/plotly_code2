/** \file
 * This file contains unit tests for content of ocg_assign_detections_to_underdrivability_zones_helpers.cpp file
 */
#include <gtest/gtest.h>
#include <cmath>
#include "ocg_assign_detections_to_underdrivability_zones_helpers.h"

#include "ocg_calibrations.h"
#include "ocg_initialize_underdrivability.h"
#include "cmn_math_constants.h"
#include "cmn_math_func.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  compensate_ground_detection
 *  @{
 */

/** \brief
 * Test group of Compensate_Ground_Detection function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */

class compensate_ground_detection : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   float det_height = {};
   float det_elev{};
   bool is_ground_detection{};
   float det_rng{};

   /** \setup
    * Initialize tracker calibrations.
    * Set radar mounting height at 0.
    * Set detection range at 40 [m].
    * Set is_ground_detection as false.
    * Set det elevation at 0
    */
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
      det_rng = 40.0F;
      is_ground_detection = false;
      det_elev = 0.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when detection height is above threshold, detection is not compensated.
 * \req
 * NA.
 */
TEST_F(compensate_ground_detection, compensate_ground_detection__height_above_limit)
{
   /** \precond
    * Set up detecton height to be above limit
    */
   det_height = calib.underdrive_hypothesis_height_can_not_pass_lower + 0.5F;
   float expected_height = det_height;

   /** \action
    * Call tested function
    */
   Compensate_Ground_Detection(calib, det_height, det_elev, is_ground_detection, det_rng);

   /** \result
    * check whether detection was not marked as ground detection
    * Check whether detection height was not inverted
    */
   EXPECT_FALSE(is_ground_detection);
   EXPECT_FLOAT_EQ(expected_height, det_height);
}

/** \purpose
 * Purpose of this test is to verify whether when detection height is below threshold, detection is compensated.
 * \req
 * NA.
 */
TEST_F(compensate_ground_detection, compensate_ground_detection__height_below_limit)
{
   /** \precond
    * Set up detecton height to be below limit
    */
   det_height = calib.underdrive_hypothesis_height_can_not_pass_lower - 0.5F;
   float expected_height = -det_height;

   /** \action
    * Call tested function
    */
   Compensate_Ground_Detection(calib, det_height, det_elev, is_ground_detection, det_rng);

   /** \result
    * check whether detection was marked as ground detection
    * Check whether detection height was inverted
    */
   EXPECT_TRUE(is_ground_detection);
   EXPECT_FLOAT_EQ(expected_height, det_height);
}

/** \purpose
 * Purpose of this test is to verify whether division by zero is handled properly
 * \req
 * NA.
 */
TEST_F(compensate_ground_detection, compensate_ground_detection__div_by_zero)
{
   /** \precond
    * Set up detecton height to be below limit
    * Set detection range to 0.0F
    * Set detection elevation to value different than 0
    */
   det_height = calib.underdrive_hypothesis_height_can_not_pass_lower - 0.5F;
   det_rng = 0.0F;
   det_elev = 1.0F;

   /** \action
    * Call tested function
    */
   Compensate_Ground_Detection(calib, det_height, det_elev, is_ground_detection, det_rng);

   /** \result
    * Check whether detection elevation is equal to 0.0F
    */
   EXPECT_FLOAT_EQ(0.0F, det_elev);
}
/** @}*/

/** \defgroup  calc_in_which_zone_det_is_located
 *  @{
 */

/** \brief
 * Test group of Calc_In_Which_Zone_Det_Is_Located function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */
class calc_in_which_zone_det_is_located : public ::testing::Test
{
protected:
   OCG_Underdrivability_Internal_T underdrivability{};
   RSPP_Host_T host{};
   float det_vcs_long_posn{};

   /** \setup
    * Initialize underdrivability.
    */
   void SetUp() override
   {
      Initialize_Underdrivability(host, underdrivability);
   }
};

/** \purpose
 * Purpose of this test is to verify whether detection is properly assigned to underdrivability zone
 * \req
 * NA.
 */
TEST_F(calc_in_which_zone_det_is_located, calc_in_which_zone_det_is_located__properly_calculated_zone)
{
   /** \precond
    * Set up expected zone index
    * Set up detection position as GRID_MIN_X_DIST + N*CELL_LENGTH + 0.1 * CELL_LENGTH
    * where N is equal to expected zone index.
    */
   const uint32_t expected_zone_idx = 24U;

   det_vcs_long_posn = GRID_MIN_X_DIST + static_cast<float>(expected_zone_idx) * CELL_LENGTH + 0.1F * CELL_LENGTH;

   /** \action
    * Call tested function.
    */
   uint32_t zone_idx = Calc_In_Which_Zone_Det_Is_Located(underdrivability, det_vcs_long_posn);

   /** \result
    * Check whether returned zone index is equal to expected index.
    */
   EXPECT_EQ(expected_zone_idx, zone_idx);
}

/** \purpose
 * Purpose of this test is to verify whether detections with longitudinal position greater than furthest zone range
 * are assigned to the furthest zone.
 * \req
 * NA.
 */
TEST_F(calc_in_which_zone_det_is_located, calc_in_which_zone_det_is_located__very_far_detections_assigned_to_furthest_zone)
{
   /** \precond
    * Set up expected zone index
    * Set up detection position 2 * CELL_LENGTH * NUM_CELLS_X
    * where N is equal to expected zone index.
    */
   const uint32_t expected_zone_idx = NUM_CELLS_X - 1U;

   det_vcs_long_posn = (2.0F * static_cast<float>(NUM_CELLS_X) * CELL_LENGTH);

   /** \action
    * Call tested function.
    */
   uint32_t zone_idx = Calc_In_Which_Zone_Det_Is_Located(underdrivability, det_vcs_long_posn);

   /** \result
    * Check whether returned zone index is equal to expected index.
    */
   EXPECT_EQ(expected_zone_idx, zone_idx);
}
/** @}*/

/** \defgroup  Calc_Det_Elevation_Params
 *  @{
 */

/** \brief
 * Test group of Calc_Det_Elevation_Params function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */
class calc_det_elevation_params : public ::testing::Test
{
protected:
   RSPP_Detection_T det{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   float det_vcs_rng{};
   float det_vcs_height{};
   float det_vcs_elev{};

   /** \setup
    * Set up selected sensor needed calibrations
    */
   void SetUp() override
   {
      // mounting_position is in vcs with z negative about ground
      sensors[0].constant.mounting_position.vcs_position.height = -0.5F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether detection elevation is correctly calculated,
 * \req
 * NA.
 */
TEST_F(calc_det_elevation_params, calc_det_elevation_params__elevation_calculated_correctly)
{
   /** \precond
    * Set detection VCS position
    * Set detection range and elevation in SCS
    */

   const float scs_az = -0.1F;
   const float scs_elev = 0.1F;
   const float scs_rng = 20.0F;

   det.processed.vcs_position_x = scs_rng * cosf(scs_elev) * cosf(scs_az);
   det.processed.vcs_position_y = scs_rng * cosf(scs_elev) * sinf(scs_az);
   det.processed.vcs_position_z = scs_rng * sinf(scs_elev) + sensors[0].constant.mounting_position.vcs_position.height;

   det.raw.range = scs_rng;
   det.raw.azimuth = scs_az;
   det.processed.vcs_el = scs_elev;
   det.raw.sensor_id = 1;

   const float expected_vcs_height = -det.processed.vcs_position_z;
   const float expected_vcs_rng = sqrtf(
       (expected_vcs_height * expected_vcs_height) +
       (det.processed.vcs_position_x * det.processed.vcs_position_x) +
       (det.processed.vcs_position_y * det.processed.vcs_position_y));

   const float expected_vcs_elev = asinf((expected_vcs_height / expected_vcs_rng));

   /** \action
    * Call tested function.
    */
   Calc_Det_Elevation_Params(det, det_vcs_rng, det_vcs_height, det_vcs_elev);

   /** \result
    * Check whether returned zone index is equal to expected index.
    */
   EXPECT_FLOAT_EQ(expected_vcs_height, det_vcs_height);
   EXPECT_FLOAT_EQ(expected_vcs_rng, det_vcs_rng);
   EXPECT_FLOAT_EQ(expected_vcs_elev, det_vcs_elev);
}

/** \purpose
 * Purpose of this test is to verify whether division by zero protection is handled correctly
 * \req
 * NA.
 */
TEST_F(calc_det_elevation_params, calc_det_elevation_params__division_by_zero_handled)
{
   /** \precond
    * Set detection VCS position
    * Set detection range and elevation in SCS
    */

   const float scs_az = -0.1F;
   const float scs_elev = 0.1F;
   const float scs_rng = ocg::cmn::OCG_MIN_DENOMINATOR * 0.5F;

   det.processed.vcs_position_x = scs_rng * cosf(scs_elev) * cosf(scs_az);
   det.processed.vcs_position_y = scs_rng * cosf(scs_elev) * sinf(scs_az);

   det.raw.range = scs_rng;
   det.raw.azimuth = scs_az;
   det.processed.vcs_el = scs_elev;
   det.raw.sensor_id = 1;

   sensors[0].constant.mounting_position.vcs_position.height = 0.0F;
   /** \action
    * Call tested function.
    */
   Calc_Det_Elevation_Params(det, det_vcs_rng, det_vcs_height, det_vcs_elev);

   /** \result
    * Check whether returned detection elevation is equal to zero.
    */
   EXPECT_FLOAT_EQ(0.0F, det_vcs_elev);
}
/** @}*/

/** \defgroup  Is_Detection_Valid
 *  @{
 */

/** \brief
 * Test group of Is_Detection_Valid function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */
class is_detection_valid : public ::testing::Test
{
protected:
   RSPP_Detection_T det{};
   RSPP_Host_T vehicle_data{};
   OCG_Calibrations_T calib{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   void SetUp() override
   {
      det.raw.sensor_id = 1;
      Initialize_OCG_Calibrations(calib);
      Set_All_Params_Valid(calib, sensors, det);
   }

   void Set_All_Params_Valid(
       const OCG_Calibrations_T &calibrations,
       F360_Radar_Sensor_T (&radar_sensors)[MAX_NUMBER_OF_SENSORS],
       RSPP_Detection_T &detection)
   {
      detection.processed.motion_status = RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      detection.processed.vcs_position_x = GRID_MIN_X_DIST + (GRID_MAX_X_DIST - GRID_MIN_X_DIST) / 2.0F;
      detection.processed.vcs_position_y = 0.0F;
      detection.processed.range_rate_compensated = 0.0F;

      if (calibrations.underdrive_use_front_side_sensors)
      {
         radar_sensors[det.raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_RIGHT_FORWARD;
      }

      if (calibrations.underdrive_use_front_center_sensors)
      {
         radar_sensors[det.raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_CENTER_FORWARD;
      }
         

      detection.raw.f_super_res = false;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when all parameters are correct, function returns true.
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__all_params_valid_returns_true)
{
   /** \precond
    * Set all params to make detection valid.
    */

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned true.
    */
   EXPECT_TRUE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detections does not come from wanted sensor
 * function returns false
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__not_valid_sensor)
{
   /** \precond
    * Set all params to make detection valid.
    * Set sensor to invalid.
    */
   sensors[det.raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_UNKNOWN;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);
   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection is moving
 * function returns false
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__det_is_moving)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection motion status to moving
    */
   det.processed.motion_status = RSPP_DETECTION_MOTION_STATUS_MOVING;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when all params are valid
 * and detection motion status is stationary function returns true
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__det_is_stationary)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection motion status to stationary
    */
   det.processed.motion_status = RSPP_DETECTION_MOTION_STATUS_STATIONARY;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned true.
    */
   EXPECT_TRUE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when all params are valid
 * and detection motion statis is ambigious, function returns true
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__det_is_ambigious)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection motion status to ambigious
    */
   det.processed.motion_status = RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned true.
    */
   EXPECT_TRUE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection longitudinal positon
 * is smaller than overhead_zone_0_dist function returns false
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__longitudinal_position_too_low)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection longitudinal position to be smaller than closest zone.
    */
   det.processed.vcs_position_x = GRID_MIN_X_DIST - 10.0F;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection longitudinal positon
 * is greater than GRID_MAX_X_DIST function returns false
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__longitudinal_position_too_high)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection longitudinal position to be grater than maximum zone dist.
    */
   det.processed.vcs_position_x = GRID_MAX_X_DIST + 10.0F;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection distance from
 * curved host path is above limit, it is marked as not valid.
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__lat_offset_from_host_curv_too_high)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection lateral position above limit.
    */
   det.processed.vcs_position_y = (CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR) + 1.0F;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection lateral positon
 * is greater than maximum lateral position of detection, it is marked as not valid.
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__det_lat_pos_too_high)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection lateral position to be above limit
    * Set host curvature to make detection dist from curved host path below limit
    */
   det.processed.vcs_position_y = calib.underdrive_max_lat_posn + 1.0F;
   const float det_x = det.processed.vcs_position_x;
   const float det_y = det.processed.vcs_position_y;
   vehicle_data.curvature_rear = (2.0F * det_y) / (det_y * det_y + det_x * det_x);

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection is super resolution
 * but it's azimuth confidence is too low, function returns false
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__super_res_det_too_low_confid)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection f_super_res flag as true
    * Set detection azmiuth confidence above limit.
    */
   det.raw.f_super_res = true;
   det.raw.confid_azimuth = calib.underdrive_az_conf_th;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when is super resolution
 * and it's azimuth confidence is high enough, it is marked as valid
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__super_res_det_proper_az_confid)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection f_super_res flag as true
    * Set detection azmiuth confidence below limit.
    */
   det.raw.f_super_res = true;
   det.raw.confid_azimuth = calib.underdrive_az_conf_th - 1;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned true.
    */
   EXPECT_TRUE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether when detection is not
 * super resolution it is marked as valid.
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__det_not_super_res)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection f_super_res flag as false.
    */
   det.raw.f_super_res = false;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned true.
    */
   EXPECT_TRUE(f_result);
}

/** \purpose
 * Purpose of this test is to verify whether detection is marked as not valid
 * when it has too high range rate compensated.
 * \req
 * NA.
 */
TEST_F(is_detection_valid, is_detection_valid__too_high_range_rate_compensated)
{
   /** \precond
    * Set all params to make detection valid.
    * Set detection range rate to be above limit.
    */
   det.processed.range_rate_compensated = calib.underdrive_max_comp_range_rate + 10.0F;

   const float closest_dist_to_host_path = Calc_Lateral_Distance_To_Curved_Host_Path(vehicle_data.curvature_rear,
      calib.underdrive_small_curvature_th,
      det.processed.vcs_position_x,
      det.processed.vcs_position_y);

   const float max_lat_offset_from_host_curv = cmn::Linear_Equation_With_Saturation(det.processed.vcs_position_x,
      NUM_CELLS_X_CLOSE * CELL_LENGTH,
      (NUM_CELLS_X_CLOSE + NUM_CELLS_X_MID) * CELL_LENGTH,
      CELL_WIDTH / 2.0F,
      CELL_WIDTH * CELL_WIDTH_EXTENSION_FACTOR / 2.0F);

   /** \action
    * Call tested function.
    */
   bool f_result = Is_Detection_Valid(det, calib, sensors, closest_dist_to_host_path, max_lat_offset_from_host_curv);

   /** \result
    * Check whether function returned false.
    */
   EXPECT_FALSE(f_result);
}
/** @}*/

/** \defgroup  update_zone_innovation
 *  @{
 */

/** \brief
 * Test group of Update_Zone_Innovation function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */
class update_zone_innovation : public ::testing::Test
{
protected:
   OCG_Zones_Innovation_T zone_innovation{};
   OCG_Calibrations_T calib{};
   float det_vcs_rng{};
   float det_vcs_elev{};
   Raw_Detection_T det{};
   bool is_ground_detection{};

   /** \setup
    * Set up selected sensor needed calibrations
    */
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
      zone_innovation = {};
      det_vcs_rng = 20.0F;
      is_ground_detection = false;
      det_vcs_elev = 0.0F;
      det = {};
   }
};

/** \purpose
 * Purpose of this test is to verify whether detections are properly counted.
 * \req
 * NA.
 */
TEST_F(update_zone_innovation, update_zone_innovation__detections_properly_counted)
{
   /** \precond
    * Set up number of detections to add.
    */
   const uint32_t number_of_detections = 5U;
   const float lateral_weight = 1;
   /** \action
    * Call tested function.
    */
   for (uint32_t i = 0U; i < number_of_detections; i++)
   {
      Update_Zone_Innovation(
          zone_innovation,
          calib,
          det_vcs_rng,
          det_vcs_elev,
          det,
          is_ground_detection,
          lateral_weight);
   }

   /** \result
    * Check whether counted number of detections is equal to number_of_detections
    */
   const float expected_number = static_cast<float>(number_of_detections);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.height_can_pass[0]);
}

/** \purpose
 * Purpose of this test is to verify whether ground detections are not assigned to RCS innovation
 * \req
 * NA.
 */
TEST_F(update_zone_innovation, update_zone_innovation__ground_detections_not_counted_to_RCS_probabilities)
{
   /** \precond
    * Set up number of detections to add.
    * Set is_ground_detection as true.
    */
   const uint32_t number_of_detections = 5U;
   is_ground_detection = true;
   const float lateral_weight = 1;

   /** \action
    * Call tested function.
    */
   for (uint32_t i = 0U; i < number_of_detections; i++)
   {
      Update_Zone_Innovation(
          zone_innovation,
          calib,
          det_vcs_rng,
          det_vcs_elev,
          det,
          is_ground_detection,
          lateral_weight);
   }

   /** \result
    * Check whether counted number of detections is equal to 0.0
    */
   const float expected_number = 0.0F;
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_pass[0]);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_is_likely_to_pass[0]);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_not_pass[0]);
}

/** \purpose
 * Purpose of this test is to verify whether detections are assigned to rcs probabilities
 * only when normalized elevation is greater than zero and detection is not a ground detection
 * \req
 * NA.
 */
TEST_F(update_zone_innovation, update_zone_innovation__RCS_can_pass_probabilities_counted_only_when_normalized_elevation_greater_than_zero)
{
   /** \precond
    * Set is_ground_detection as false.
    * Set vcs elevation to 0.3
    */
   is_ground_detection = false;
   det_vcs_elev = 0.3F;
   const float lateral_weight = 1;

   /** \action
    * Call tested function.
    */
   Update_Zone_Innovation(
       zone_innovation,
       calib,
       det_vcs_rng,
       det_vcs_elev,
       det,
       is_ground_detection,
       lateral_weight);

   /** \result
    * Check whether counted number of detections is equal to 1.0
    */
   const float expected_number = 1.0F;
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_pass[0]);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_is_likely_to_pass[0]);
}

/** \purpose
 * Purpose of this test is to verify whether when normalized elevation is greater than zero but
 * detection is ground detection, detection is not taken into account in rcs probabilities.
 * \req
 * NA.
 */
TEST_F(update_zone_innovation, update_zone_innovation__RCS_normalized_elev_greater_than_zero_det_is_ground_detection)
{
   /** \precond
    * Set is_ground_detection as false.
    * Set det vcs elevation to 0.3
    */
   is_ground_detection = true;
   det_vcs_elev = 0.3F;
   const float lateral_weight = 1;

   /** \action
    * Call tested function.
    */
   Update_Zone_Innovation(
       zone_innovation,
       calib,
       det_vcs_rng,
       det_vcs_elev,
       det,
       is_ground_detection,
       lateral_weight);

   /** \result
    * Check whether counted number of detections in RCS innovation is equal to 0.0
    */
   const float expected_number = 0.0F;
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_pass[0]);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_is_likely_to_pass[0]);
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_not_pass[0]);
}

/** \purpose
 * Purpose of this test is to verify whether detection is not assigned to rcs_can_not_pass probability
 * innovation when both normalized elevations are smaller than 0.
 * \req
 * NA.
 */
TEST_F(update_zone_innovation, update_zone_innovation__RCS_can_not_pass_both_normalized_elev_smaller_than_zero)
{
   /** \precond
    * Set is_ground_detection as false.
    * Set det_vcs_elev to -0.2
    */
   is_ground_detection = false;
   det_vcs_elev = -0.2F;
   const float lateral_weight = 1;

   /** \action
    * Call tested function.
    */
   Update_Zone_Innovation(
       zone_innovation,
       calib,
       det_vcs_rng,
       det_vcs_elev,
       det,
       is_ground_detection,
       lateral_weight);

   /** \result
    * Check whether counted number of detections in can_not_pass innovation is equal to 0
    */
   const float expected_number = 0.0F;
   EXPECT_FLOAT_EQ(expected_number, zone_innovation.RCS_slope_can_not_pass[0]);
}
/** @}*/

/** \defgroup  calc_lateral_distance_to_curved_host_path
 *  @{
 */

/** \brief
 * Test group of Calc_Lateral_Distance_To_Curved_Host_Path function placed in
 * ocg_assing_detections_to_underdrivability_zones_helpers.cpp file
 */
class calc_lateral_distance_to_curved_host_path : public ::testing::Test
{
protected:
   float curvature_rear{};
   float small_curvature_th{};
   float det_vcs_position_x{};
   float det_vcs_position_y{};

   void SetUp() override
   {
      det_vcs_position_y = -5.0F;
      det_vcs_position_x = 20.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when host curvature is below limit
 * lateral distance is calculated as detection lateral position.
 * \req
 * NA.
 */
TEST_F(calc_lateral_distance_to_curved_host_path, calc_lateral_distance_to_curved_host_path__curvature_below_limit)
{
   /** \precond
    * Set curvature rear as 0
    * Set curvature threshold above 0
    */
   curvature_rear = 0.0F;
   small_curvature_th = 0.1F;

   /** \action
    * Call tested function.
    */
   float lateral_diff = Calc_Lateral_Distance_To_Curved_Host_Path(curvature_rear, small_curvature_th, det_vcs_position_x, det_vcs_position_y);

   /** \result
    * Check whether lateral diff is equal to absolute value of detection lateral position.
    */
   EXPECT_FLOAT_EQ(std::fabs(det_vcs_position_y), lateral_diff);
}

/** \purpose
 * Purpose of this test is to verify whether when host curvature is above limit
 * lateral distance is calculated in reference to host curvature.
 * \req
 * NA.
 */
TEST_F(calc_lateral_distance_to_curved_host_path, calc_lateral_distance_to_curved_host_path__curvature_above_limit)
{
   /** \precond
    * Set curvature threshold as 0
    * Set curvature rear above 0
    * Set expected lateral diff
    */
   small_curvature_th = 0.0F;
   curvature_rear = 0.1F;
   const float expected_lateral_diff = 15.0F;

   /** \action
    * Call tested function.
    */
   float lateral_diff = Calc_Lateral_Distance_To_Curved_Host_Path(curvature_rear, small_curvature_th, det_vcs_position_x, det_vcs_position_y);

   /** \result
    * Check whether lateral diff is equal to absolute value of detection lateral position.
    */
   EXPECT_DOUBLE_EQ(static_cast<double>(expected_lateral_diff), static_cast<double>(lateral_diff));
}

/** @}*/
