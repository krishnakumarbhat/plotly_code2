/** \file
 * This file contains unit tests for content of OCG_Underdrivability.cpp file
 */

#include "ocg_underdrivability.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"
#include "ocg_initialize_underdrivability.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  OCG_Underdrivability
 *  @{
 */

/** \brief
 * Test group of Underdrivability function. Tests verify whether
 * algorithm is properly executed, compensated for host movment and reset.
 */
class OCG_Underdrivability : public ::testing::Test
{
protected:
   RSPP_Detection_List_T detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   OCG_Underdrivability_Internal_T underdrivability{};
   RSPP_Host_T vehicle_data{};
   OCG_Calibrations_T calib{};

   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
      Initialize_Underdrivability(vehicle_data, underdrivability);
   }
};

/** \purpose
 * Purpose of this test is to verify whether algorithm is reset when host is reversing.
 * \req
 * NA.
 */
TEST_F(OCG_Underdrivability, Underdrivability__Host_Reversing)
{
   /** \precond
    * Initialize underdrivability struct with values different than zero.
    * Set host speed to be below 0.0
    */
   underdrivability.props.circular_buffer_idx = 10U;
   underdrivability.props.host_travel_distance = 10.0F;
   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      OCG_Single_Underdrivability_Zone_T &zone = underdrivability.zones[i];
      zone.p_can_not_pass = 1.0F;
      zone.cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   }
   vehicle_data.speed = -1.0F;

   /** \action
    * Call tested function.
    */
   Underdrivability(detection_list, sensors, vehicle_data, calib, underdrivability);

   /** \result
    * Check whether underdrivability struct was reset.
    */
   bool f_succes = true;
   f_succes &= (0U == underdrivability.props.circular_buffer_idx);
   f_succes &= (0.0F == underdrivability.props.host_travel_distance);
   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      for (uint32_t state_idx = 0; state_idx < UD_HEIGHT_STATE_SIZE; state_idx++)
      {
         f_succes &= (0.0F == underdrivability.zones[i].state_height_can_pass[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_height_is_likely_to_pass[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_height_can_not_pass_upper[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_height_can_not_pass_lower[state_idx]);
      }

      for (uint32_t state_idx = 0; state_idx < UD_RCS_STATE_SIZE; state_idx++)
      {
         f_succes &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_pass[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_RCS_slope_is_likely_to_pass[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_not_pass_upper[state_idx]);
         f_succes &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_not_pass_lower[state_idx]);
      }

      f_succes &= (0.0F == underdrivability.zones[i].p_height_can_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_height_is_likely_to_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_height_can_not_pass_upper);
      f_succes &= (0.0F == underdrivability.zones[i].p_height_can_not_pass_lower);
      f_succes &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_RCS_slope_is_likely_to_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_not_pass_upper);
      f_succes &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_not_pass_lower);
      f_succes &= (0.0F == underdrivability.zones[i].p_can_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_is_likely_to_pass);
      f_succes &= (0.0F == underdrivability.zones[i].p_can_not_pass);

      f_succes &= (UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER == underdrivability.zones[i].cell_classification.underdrivability_status);
   }
   EXPECT_TRUE(f_succes);
}

/** \purpose
 * Purpose of this test is to verify whether zones are shifted.
 * \req
 * NA.
 */
TEST_F(OCG_Underdrivability, Underdrivability__Zones_Are_Shifted)
{
   /** \precond
    * Set host delta position as 1.5 * zone_length
    * Save current circular buffer index.
    * Set status of selected zone to: UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
    */
   const uint32_t selected_zone_idx = 10U;
   vehicle_data.vcs_speed = vehicle_data.speed;
   underdrivability.props.host_travel_distance = 1.8F * CELL_LENGTH;
   const uint16_t current_circular_buffer_idx = underdrivability.props.circular_buffer_idx;
   underdrivability.zones[selected_zone_idx].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
   underdrivability.zones[selected_zone_idx - 1U].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

   /** \action
    * Call tested function.
    */
   Underdrivability(detection_list, sensors, vehicle_data, calib, underdrivability);

   /** \result
    * Check whether circular buffer idx was incremented.
    * Check whether underdrivability status was shifted.
    */
   EXPECT_EQ(current_circular_buffer_idx + 1U, underdrivability.props.circular_buffer_idx);
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, underdrivability.zones[selected_zone_idx - 1U].cell_classification.underdrivability_status);
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, underdrivability.zones[selected_zone_idx - 2U].cell_classification.underdrivability_status);
}

/** \purpose
 * Purpose of this test is to verify whether algorithm is executed.
 * \req
 * NA.
 */
TEST_F(OCG_Underdrivability, Underdrivability__Algorithm_Is_Executed)
{
   /** \precond
    * Set up one detection parameters.
    * Set host speed above calibration threshold.
    * Set sensor type to make it wanted one.
    */
   detection_list.number_of_valid_detections = 1U;
   detection_list.detections[0].raw.sensor_id = 1;
   detection_list.detections[0].raw.f_super_res = false;
   detection_list.detections[0].raw.confid_azimuth = 0;

   detection_list.detections[0].processed.range_rate_compensated = 0.0F;
   detection_list.detections[0].processed.motion_status = RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   detection_list.detections[0].processed.vcs_position_x = GRID_MIN_X_DIST + CELL_LENGTH / 2.0F;
   detection_list.detections[0].processed.vcs_position_y = 0.0F;

   vehicle_data.speed = calib.underdrive_slow_moving_host + 1.0F; 
   
   if (calib.underdrive_use_front_side_sensors)
   {
      sensors[detection_list.detections[0].raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_RIGHT_FORWARD;
   }

   if (calib.underdrive_use_front_center_sensors)
   {
      sensors[detection_list.detections[0].raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_CENTER_FORWARD;
   }

   /** \action
    * Call tested function.
    */
   Underdrivability(detection_list, sensors, vehicle_data, calib, underdrivability);

   /** \result
    * Check whether state first zone has changed.
    */
   EXPECT_FLOAT_EQ(1.0F, underdrivability.zones[0].state_height_can_pass[0]);
}
/** @}*/
