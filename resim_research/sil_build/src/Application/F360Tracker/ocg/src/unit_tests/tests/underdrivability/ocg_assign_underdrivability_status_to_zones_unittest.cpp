/** \file
 * This file contains unit tests for content of ocg_assign_underdrivability_status_to_zones.cpp file
 */

#include "ocg_assign_underdrivability_status_to_zones.h"
#include "ocg_calibrations.h"
#include <gtest/gtest.h>

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_assign_underdrivability_status_to_zones
 *  @{
 */

/** \brief
 * Test group of function: Assign_Underdrivability_Status_To_Zones().
 * Tests verify whether zones are properly distinguished into criticality levels
 * basing on their longitudinal validity region and have their probabilities tested.
 */
class f360_assign_underdrivability_status_to_zones : public ::testing::Test
{
protected:
   OCG_Underdrivability_Internal_T underdrivability{};
   RSPP_Host_T vehicle_data{};
   OCG_Calibrations_T calib{};

   /** \setup
    * Initialize calibrations.
    */
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether all zones are tested.
 */
TEST_F(f360_assign_underdrivability_status_to_zones, Assign_Underdrivability_Status_To_Zones__All_Zones_Are_Tested)
{
   /** \precond
    * Set up parameters of all zones to have high probabilities indicating an overhead object.
    * Set up parameters of all zones to have high number of measurements.
    * Set up underdrivability_status of all zones to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   for (int i = 0; i < NUM_CELLS_X; i++)
   {
      underdrivability.zones[i].p_height_can_pass = 1.0F;
      underdrivability.zones[i].p_RCS_slope_can_pass = 1.0F;
      underdrivability.zones[i].p_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_height_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_RCS_slope_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_can_not_pass = 0.0F;
      underdrivability.zones[i].state_height_is_likely_to_pass[0] = 20.0F;
      underdrivability.zones[i].state_RCS_slope_is_likely_to_pass[0] = 20.0F;
      underdrivability.zones[i].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   }

   // Set up vehicle speed to make all zones be in area of interest.
   vehicle_data.speed = ((NUM_CELLS_X + 1) * CELL_LENGTH + GRID_MIN_X_DIST) / calib.underdrive_time_zone_crit_low;

   /** \action
    * Call tested function.
    */
   Assign_Underdrivability_Status_To_Zones(underdrivability, vehicle_data, calib);

   /** \result
    * Check whether zones have assigned status indicating an overhead object.
    */

   for (int i = 0; i < NUM_CELLS_X; i++)
   {
      bool f_success = (underdrivability.zones[i].cell_classification.underdrivability_status == UNDERDRIVABLE_STATUS_CAN_PASS_UNDER) ||
                       (underdrivability.zones[i].cell_classification.underdrivability_status == UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER);
      EXPECT_TRUE(f_success);
   }
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether zones that are outside field of view
 * are marked as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
 */
TEST_F(f360_assign_underdrivability_status_to_zones, Assign_Underdrivability_Status_To_Zones__Not_Relevant_Zones_Marked_As_Not_To_Consider)
{
   /** \precond
    * Set up parameters of all zones to have high probabilities indicating an overhead object.
    * Set up parameters of all zones to have high number of measurements.
    * Set up underdrivability_status of all zones to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   for (int i = 0; i < NUM_CELLS_X; i++)
   {
      underdrivability.zones[i].p_height_can_pass = 1.0F;
      underdrivability.zones[i].p_RCS_slope_can_pass = 1.0F;
      underdrivability.zones[i].p_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_height_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_RCS_slope_is_likely_to_pass = 1.0F;
      underdrivability.zones[i].p_can_not_pass = 0.0F;
      underdrivability.zones[i].state_height_is_likely_to_pass[0] = 20.0F;
      underdrivability.zones[i].state_RCS_slope_is_likely_to_pass[0] = 20.0F;
      underdrivability.zones[i].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   }

   // Set up vehicle speed to make all zones be in area of interest expect the last one
   vehicle_data.speed = ((NUM_CELLS_X - 2) * CELL_LENGTH + GRID_MIN_X_DIST) / calib.underdrive_time_zone_crit_low;

   /** \action
    * Call tested function.
    */
   Assign_Underdrivability_Status_To_Zones(underdrivability, vehicle_data, calib);

   /** \result
    * Check whether last zone has assigned underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
    */
   EXPECT_EQ(underdrivability.zones[NUM_CELLS_X - 1].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
}
/** @}*/
