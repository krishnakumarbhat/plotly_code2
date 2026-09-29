/** \file
 * This file contains unit tests for content of ocg_assign_detections_to_underdrivability_zones.cpp file
 */

#include "ocg_assign_detections_to_underdrivability_zones.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_assign_detections_to_underdrivability_zones
 *  @{
 */

/** \brief
 * Test group of function: Assign_Detections_To_Underdrivability_Zones().
 * Tests verify whether valid detections are properly assigned to zones
 */
class f360_assign_detections_to_underdrivability_zones : public ::testing::Test
{
protected:
   OCG_Underdrivability_Internal_T underdrivability{};
   OCG_Zones_Innovation_T zones_innovation[NUM_CELLS_X]{};
   RSPP_Host_T vehicle_data{};
   RSPP_Detection_List_T detection_list{};
   OCG_Calibrations_T calib{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
    * Initialize tracker calibrations
    */
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }

   void Set_Detection_Parameters_Valid(
       RSPP_Detection_T &det,
       const uint32_t zone_idx)
   {
      det.raw.sensor_id = 1;
      det.raw.f_super_res = false;

      det.processed.motion_status = RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      det.processed.vcs_position_x = (GRID_MIN_X_DIST + static_cast<float>(zone_idx) * CELL_LENGTH + 0.01F);
      det.processed.vcs_position_y = 0.0F;
      det.processed.range_rate_compensated = 0.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when detection is valid, it is assigned to underdrivabiliy zone.
 * \req
 * NA.
 */
TEST_F(f360_assign_detections_to_underdrivability_zones, Assign_Detections_To_Underdrivability_Zones__Detection_Is_Valid)
{
   /** \precond
    * Set selected detection parameters to make it valid.
    */
   const uint32_t zone_idx = 10U;
   detection_list.number_of_valid_detections = 1U;
   vehicle_data.curvature_rear = 0.0F;

   Set_Detection_Parameters_Valid(detection_list.detections[0], zone_idx);
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
   Assign_Detections_To_Underdrivability_Zones(
       underdrivability,
       zones_innovation,
       detection_list,
       calib,
       sensors);

   /** \result
    * Check whether detection was assigned to any zone.
    */
   EXPECT_FLOAT_EQ(1.0F, zones_innovation[zone_idx].height_can_pass[0]);
}

/** \purpose
 * Purpose of this test is to verify whether when detection is not valid, it is not assigned to any zone.
 * \req
 * NA.
 */
TEST_F(f360_assign_detections_to_underdrivability_zones, Assign_Detections_To_Underdrivability_Zones__Detection_Is_Not_Valid)
{
   /** \precond
    * Set selected detection parameters to make it not valid.
    */
   const uint32_t zone_idx = 0U;
   detection_list.number_of_valid_detections = 1U;
   RSPP_Detection_T *det = &detection_list.detections[zone_idx];

   detection_list.detections[zone_idx].raw.sensor_id = 1;

   if (calib.underdrive_use_front_side_sensors)
   {
      sensors[det->raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_RIGHT_FORWARD;
   }

   if (calib.underdrive_use_front_center_sensors)
   {
      sensors[det->raw.sensor_id - 1].constant.mounting_location = RSPP_MOUNTING_LOCATION_CENTER_FORWARD;
   }

   det->processed.motion_status = RSPP_DETECTION_MOTION_STATUS_MOVING;
   det->processed.vcs_position_x = (GRID_MIN_X_DIST + GRID_MAX_X_DIST) / 2.0F;
   det->processed.vcs_position_y = 0.0F;

   vehicle_data.curvature_rear = 0.0F;

   det->raw.f_super_res = false;

   det->processed.range_rate_compensated = 0.0F;

   /** \action
    * Call tested function.
    */
   Assign_Detections_To_Underdrivability_Zones(
       underdrivability,
       zones_innovation,
       detection_list,
       calib,
       sensors);

   /** \result
    * Check whether detection was not assigned to any zone.
    */
   bool f_success = true;
   for (uint32_t i = 0U; i < NUM_CELLS_X; i++)
   {
      if (zones_innovation[i].height_can_pass[0] > 0.0F)
      {
         f_success = false;
         break;
      }
   }

   EXPECT_TRUE(f_success);
}

/** \purpose
 * Purpose of this test is to verify whether when given multiple detections, 2 of them are valid and one is not - two are properly
 * assigned to zone.
 * \req
 * NA.
 */
TEST_F(f360_assign_detections_to_underdrivability_zones, Assign_Detections_To_Underdrivability_Zones__Multiple_Detections)
{
   /** \precond
    * Set parameters of 3 selected detections to make it valid.
    * Set motion status of third detection to make it invalid.
    */
   const uint32_t first_zone_idx = 10U;
   const uint32_t second_zone_idx = 12U;
   const uint32_t third_zone_idx = 14U;
   detection_list.number_of_valid_detections = 3U;
   vehicle_data.curvature_rear = 0.0F;

   Set_Detection_Parameters_Valid(detection_list.detections[0], first_zone_idx);
   Set_Detection_Parameters_Valid(detection_list.detections[1], second_zone_idx);
   Set_Detection_Parameters_Valid(detection_list.detections[2], third_zone_idx);

   detection_list.detections[2].processed.motion_status = RSPP_DETECTION_MOTION_STATUS_MOVING;

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
   Assign_Detections_To_Underdrivability_Zones(
       underdrivability,
       zones_innovation,
       detection_list,
       calib,
       sensors);

   /** \result
    * Check whether detections were assigned to first two zones.
    * Check whether detection was not assigned to last zone.
    */
   EXPECT_FLOAT_EQ(1.0F, zones_innovation[first_zone_idx].height_can_pass[0]);
   EXPECT_FLOAT_EQ(1.0F, zones_innovation[second_zone_idx].height_can_pass[0]);
   EXPECT_FLOAT_EQ(0.0F, zones_innovation[third_zone_idx].height_can_pass[0]);
}
/** @}*/
