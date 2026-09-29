/**
 * @file cta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for nissan cta pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-71103}
 */

#include "cta_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_pre_run.c"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_context.h"
}

/**
 * Check that mapping in cta pre run is done correctly.
 * \uts{} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__check_mapping_between_input_and_core_input)
{
   /** \arrange set cta input to something */
   cta_input.f_cta_enable = FBK_ONE_UINT;

   /** \action execute pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect mapping is done correctly */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, FBK_ONE_UINT);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);
   /* Map TTC thresholds to the provided ones from calibration structure */
   for (uint8_t mode_idx = FBK_ZERO_UINT; mode_idx < CTA_NUM_MODES; mode_idx++)
   {
      for (uint8_t level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_criticality_level[mode_idx][level_idx]);
      }
   }
}

/**
 * Check that cta zone is set correctly
 * \uts{CSCSA-72247} \sdd{SF-70876} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Construct_Zone_general_test)
{
   /** \arrange set typical calibration data and calculate expected zone coordinates*/
   float32_T expected_x[4u];
   float32_T expected_y[4u];
   uint8_t idx;
   expected_x[0] = -p_vehicle_data->host_length - p_cta_cal->k_cta_max_long_point_criticality_level[0][0];
   expected_x[1] = -p_vehicle_data->host_length - p_cta_cal->k_cta_min_long_point_criticality_level[0][0];
   expected_x[2] = expected_x[1] - (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[1]));
   expected_x[3] = expected_x[0] + (p_cta_cal->k_cta_max_length_fov * Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0]));
   expected_y[0] = p_cta_cal->k_cta_butterfly_lat[0];
   expected_y[1] = p_cta_cal->k_cta_butterfly_lat[1];
   expected_y[2] = p_cta_cal->k_cta_max_length_fov;
   expected_y[3] = p_cta_cal->k_cta_max_length_fov;
   /** \action execute zone funciton */
   Cta_Construct_Zone(&(cta_instance.core_input.cta_zone), p_cta_cal, p_vehicle_data);
   /** \assert expect zone paramaters to be set correctly. */
   EXPECT_EQ(cta_instance.core_input.cta_zone.size, 4u);
   for (idx = 0; idx < p_cta_cal->k_cta_amount_butterfly_points_in_use; idx++)
   {
      EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[idx].x, expected_x[idx]);
      EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[idx].y, expected_y[idx]);
   }
}
