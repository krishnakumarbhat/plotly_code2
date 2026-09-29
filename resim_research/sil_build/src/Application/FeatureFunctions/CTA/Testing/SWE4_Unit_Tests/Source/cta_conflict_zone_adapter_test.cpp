/**
 * @file cta_conflict_zone_adapter_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_conflict_zone_adapter.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41842}
 */

#include "cta_conflict_zone_adapter_test.hpp"
#include <algorithm>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <math.h>

extern "C"
{
#include "cta_conflict_zone_adapter.c"
#include "fbk_macros.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
}

/**
 * Tests the longitudinal conflict zone extension. Here no extension is enabled. It is exepected, that the intersection zone
 * remains unchanged. \uts{CSCSA-41843} \sdd{SF-3740} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__no_extension_enabled)
{
   /** \arrange setup initial conflict zone */
   float32_T init_val = 1.0f;
   float32_T expected_array[CTA_NUM_CRIT_LEVEL];
   std::fill_n(expected_array, CTA_NUM_CRIT_LEVEL, init_val);

   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;
   mode                                                      = CTA_MODE_FRONT;
   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, expected_array, expected_array, mode);

   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert Expect initial zone to be unchanged. */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][i], init_val);
      EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[mode][i], init_val);
   }
}


/**
 * Tests the longitudinal conflict zone extension for front CTA. Here the heading extension of the conflict zone is enabled. It is
 * expected that zone is extended dependend on the targets heading. \uts{CSCSA-41844} \sdd{SF-3740}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__obj_heading_ext_for_FCTA_enabled)
{
   /** \arrange setup initial conflict zone and object with heading pointing to first quadrant */
   float32_T heading_angle = 0.523599f; /*30 deg in rad*/
   min_cals_val            = 1.0f;
   max_cals_val            = 3.0f;
   width                   = max_cals_val - min_cals_val;
   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);

   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;

   mode                                  = CTA_MODE_FRONT;
   confl_zone_ext_params.target_head_fac = 1.0f / Fast_Sin(heading_angle);


   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);

   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert Expect initial conflict zone to be extended to the front. */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[mode][i],
                      min_cals_val + width * confl_zone_ext_params.target_head_fac);
   }
}

/**
 * Tests the longitudinal conflict zone extension for rear CTA. Here the heading extension of the conflict zone is enabled. It is
 * expected that zone is extended dependend on the targets heading. \uts{CSCSA-41845} \sdd{SF-3740}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__obj_heading_ext_for_RCTA_enabled)
{
   /** \arrange setup initial conflict zone and object with heading pointing to fourth quadrant */
   float32_T heading_angle = 2.61799f;
   min_cals_val            = -3.0f;
   max_cals_val            = -1.0f;
   width                   = max_cals_val - min_cals_val;
   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);

   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_FALSE;

   confl_zone_ext_params.target_head_fac = 1.0f / Fast_Sin(heading_angle);

   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);


   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert Expect initial conflict zone to be extended to the rear */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][i],
                      max_cals_val - width * confl_zone_ext_params.target_head_fac);
   }
}

/**
 * Tests the longitudinal conflict zone extension for front CTA. Here the steering angle of the conflict zone is enabled. It is
 * expected that zone length is reduced to the front. \uts{CSCSA-41846} \sdd{SF-3740} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__steering_angle_zone_reduction_for_FCTA_enabled)
{
   /** \arrange setup initial conflict zone */
   min_cals_val = 1.0f;
   max_cals_val = 3.0f;
   width        = max_cals_val - min_cals_val;

   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);

   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_FALSE;

   mode = CTA_MODE_FRONT;

   confl_zone_ext_params.host_steer_fac[mode] = 0.9f;

   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);

   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert expect that the zone to the front is reduced by the host steering factor */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[mode][i],
                      min_cals_val + width * confl_zone_ext_params.host_steer_fac[mode]);
   }
}

/**
 * Tests the longitudinal conflict zone extension for rear CTA. Here the steering angle of the conflict zone is enabled. It is
 * expected that zone length is reduced to the rear. \uts{CSCSA-41847} \sdd{SF-3740} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__steering_angle_zone_reduction_for_RCTA_enabled)
{
   /** \arrange setup initial conflict zone */
   min_cals_val = -3.0f;
   max_cals_val = -1.0f;
   width        = max_cals_val - min_cals_val;
   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);


   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_FALSE;

   mode = CTA_MODE_REAR;

   confl_zone_ext_params.host_steer_fac[mode] = 0.9f;

   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);

   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert expect that the zone to the rear is reduced by the host steering factor */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][i],
                      max_cals_val - width * confl_zone_ext_params.host_steer_fac[mode]);
   }
}


/**
 * Tests the zone extension factor calculation based on lookuptables. Here a value shall be interpolated in the mid of two grid
 * point values of the lookuptable and the mean of the two grid values is expected. \uts{CSCSA-41848} \sdd{SF-3744}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test,
       Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor__return_interpolation_of_steering_angle_factor)
{
   /** \arrange setup value to interpolate and expected result value */
   float32_T steering_angle = (core_cals.k_cta_fcta_steer_angle_table[2] + core_cals.k_cta_fcta_steer_angle_table[3]) / 2.0f;
   float32_T result, expected_res;

   expected_res = (core_cals.k_cta_fcta_steer_factor_table[2] + core_cals.k_cta_fcta_steer_factor_table[3]) / 2.0f;
   /** \action executes function to test */
   result = Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor(steering_angle, core_cals.k_cta_fcta_steer_angle_table,
                                                                core_cals.k_cta_fcta_steer_factor_table,
                                                                CTA_K_CTA_RCTA_STEER_ANGLE_TABLE_ARRAY_SIZE_DIM0);

   /** \assert expect that the routine returns the mean of the two grid values of the lookuptable */
   EXPECT_FLOAT_EQ(expected_res, result);
}

/**
 * Tests the routine for application of steering and object heading extension values for rcta. An extension by a super position of
 * both extension is expected. \uts{CSCSA-41849} \sdd{SF-3740} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Ext_To_Level_Logic_Calib__apply_steer_and_obj_head)
{
   /** \arrange steering angle and objects heading for zone extension */
   float32_T heading_angle = 2.61799f;
   float32_T expected_res_steer, expected_res_sum;
   float32_T width_intermediate;

   min_cals_val = -3.0f;
   max_cals_val = -1.0f;
   width        = max_cals_val - min_cals_val;

   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = 0.72f;
   confl_zone_ext_params.target_head_fac      = 1.0f / (Fast_Sin(heading_angle));

   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);
   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);

   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_TRUE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_FALSE;


   /*Calc result*/
   expected_res_steer = max_cals_val - width * confl_zone_ext_params.host_steer_fac[mode];
   width_intermediate = (max_cals_val - expected_res_steer);
   expected_res_sum   = max_cals_val - width_intermediate * confl_zone_ext_params.target_head_fac;

   /** \action executes function to test */
   Cta_Apply_Ext_To_Level_Logic_Calib(&crit_level_cals, &confl_zone_ext_params, &core_cals, mode);

   /** \assert expect rear part of zone to be extended */
   for (uint8_t i = 0; i < (uint8_t) CTA_NUM_CRIT_LEVEL; i++)
   {
      EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][i], expected_res_sum);
   }
}

/**
 * Tests the routine for application of a given extension to the conflict zone. Here it is expected, that the rear border of the
 * zone is modifed in case of rear cta. \uts{CSCSA-41850} \sdd{SF-3741} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Factor_For_Level_Extension__RCTA_enabled_single_level_extension)
{
   /** \arrange factor for extension calculation */
   float32_T factor = 0.72f;
   uint8_t level    = 0;

   min_cals_val = -3.0f;
   max_cals_val = -1.0f;
   width        = max_cals_val - min_cals_val;
   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);
   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);
   mode = CTA_MODE_REAR;

   /** \action executes function to test */
   Cta_Apply_Factor_For_Level_Extension(&crit_level_cals, factor, level, mode);

   /** \assert expect rear part of zone to be extended */
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][level], max_cals_val - width * factor);
}

/**
 * Tests the routine for application of a given extension to the conflict zone. Here it is expected, that the front border of the
 * zone is modifed in case of front cta. \uts{CSCSA-41851} \sdd{SF-3741} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Apply_Factor_For_Level_Extension__FCTA_enabled_single_level_extension)
{
   /** \arrange factor for extension calculation */
   float32_T factor = 0.72f;
   uint8_t level    = 0;
   mode             = CTA_MODE_FRONT;
   min_cals_val     = 1.0f;
   max_cals_val     = 3.0f;
   width            = max_cals_val - min_cals_val;
   std::fill_n(init_level_cals_min_array, CTA_NUM_CRIT_LEVEL, min_cals_val);
   std::fill_n(init_level_cals_max_array, CTA_NUM_CRIT_LEVEL, max_cals_val);
   Cta_Init_Conflict_Zone_Borders(&crit_level_cals, init_level_cals_min_array, init_level_cals_max_array, mode);

   /** \action executes function to test */
   Cta_Apply_Factor_For_Level_Extension(&crit_level_cals, factor, level, mode);

   /** \assert expect front part of zone to be extended */
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[mode][level], min_cals_val + width * factor);
}

/**
 * Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. Adaption to
 * object heading is set active. Call Cta_Adapt_Long_Crit_Level_Ranges and verify that level logic calibrations are adapted and
 * heading factor is non-zero. \uts{CSCSA-41852} \sdd{SF-3747} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Adapt_Long_Crit_Level_Ranges__intersect_line_by_object_heading_active)
{
   /** \arrange Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. */
   Angle_T angle{};
   Cta_Inters_Zone_Ext_Param_T inters_zone{};

   mode                                                   = CTA_MODE_REAR;
   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading = FBK_TRUE;

   core_cals.k_cta_min_park_angle                            = PI / 2.0f;
   angle.cos                                                 = cos(angle.angle);
   angle.sin                                                 = sin(angle.angle);
   crit_level_cals.max_long_point_criticality_level[mode][0] = 1.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = 0.0f;
   inters_zone.host_vel_fac                                  = 1.0f;
   inters_zone.host_steer_fac[mode]                          = 1.0f;

   /** \action Call Cta_Adapt_Long_Crit_Level_Ranges. */
   Cta_Adapt_Long_Crit_Level_Ranges(&crit_level_cals, &inters_zone, &object, &core_cals, mode);

   /** \assert Verify that level logic calibrations are adapted. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 1.0f);
   EXPECT_NE(crit_level_cals.max_long_point_criticality_level[mode][0], 0);
}

/**
 * Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. Adaption to
 * object heading is set inactive. Call Cta_Adapt_Long_Crit_Level_Ranges and verify that level logic calibrations are adapted and
 * heading factor is zero. \uts{CSCSA-41853} \sdd{SF-3747} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Adapt_Long_Crit_Level_Ranges__intersect_line_by_object_heading_inactive)
{
   /** \arrange Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_min_park_angle                            = PI / 2.0f;
   crit_level_cals.max_long_point_criticality_level[mode][0] = 1.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = 0.0f;
   inters_zone.host_vel_fac                                  = 1.0f;
   inters_zone.host_steer_fac[mode]                          = 1.0f;

   /** \action Call Cta_Adapt_Long_Crit_Level_Ranges. */
   Cta_Adapt_Long_Crit_Level_Ranges(&crit_level_cals, &inters_zone, &object, &core_cals, mode);

   /** \assert Verify that level logic calibrations are adapted. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 0.0f);
   EXPECT_NE(crit_level_cals.max_long_point_criticality_level[mode][0], 0);
}

/**
 * Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. Adaption to host
 * speed is set active. Adaptation is applied only for rear zone. Call Cta_Adapt_Long_Crit_Level_Ranges and verify that level logic
 * calibrations are adapted and host speed factor is correct. \uts{CSCSA-188948} \sdd{SF-3747} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Adapt_Long_Crit_Level_Ranges__host_vel_treshold_rear_mode)
{
   /** \arrange Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_TRUE;


   crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][0] = 1.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][0] = 0.0f;
   crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][1] = 2.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][1] = 1.0f;

   crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][0] = 0.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][0] = -1.0f;
   crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][1] = -1.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][1] = -2.0f;


   mode                     = CTA_MODE_REAR;
   inters_zone.host_vel_fac = 1.2f;

   /** \action Call Cta_Adapt_Long_Crit_Level_Ranges. */
   Cta_Adapt_Long_Crit_Level_Ranges(&crit_level_cals, &inters_zone, &object, &core_cals, mode);

   /** \assert Verify that level logic calibrations are adapted. */
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][0], 0.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][0], -1.2f);
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][1], -1.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][1], -2.2f);

   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][0], 1.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][0], 0.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][1], 2.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][1], 1.0f);
}

/**
 * Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. Adaption to host
 * speed is set active. Adaptation is applied only for front zone. Call Cta_Adapt_Long_Crit_Level_Ranges and verify that level
 * logic calibrations are adapted and host speed factor is correct. \uts{CSCSA-188949} \sdd{SF-3747}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Adapt_Long_Crit_Level_Ranges__host_vel_treshold_front_mode)
{
   /** \arrange Set up parameters such that Cta_Adapt_Long_Crit_Level_Ranges adapts the longitudinal criticallity level ranges. */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_f_adapt_intersect_lines_by_obj_heading    = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed     = FBK_TRUE;


   crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][0] = 1.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][0] = 0.0f;
   crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][1] = 2.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][1] = 1.0f;

   crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][0] = 0.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][0] = -1.0f;
   crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][1] = -1.0f;
   crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][1] = -2.0f;


   mode                     = CTA_MODE_FRONT;
   inters_zone.host_vel_fac = 1.2f;

   /** \action Call Cta_Adapt_Long_Crit_Level_Ranges. */
   Cta_Adapt_Long_Crit_Level_Ranges(&crit_level_cals, &inters_zone, &object, &core_cals, mode);

   /** \assert Verify that level logic calibrations are adapted. */
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][0], 0.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][0], -1.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][1], -1.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][1], -2.0f);

   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][0], 1.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][0], 0.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][1], 2.0f);
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][1], 1.0f);
}


/**
 * Calculate object heading adaption for various cases. This case: object heading positive, smaller than limit. Compare computed
 * value with manually calculated value. \uts{CSCSA-41854} \sdd{SF-3742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Obj_Head_Adapt__positive_angle_smaller_than_limit)
{
   /** \arrange Set up heading adaption parameters */
   float32_T obj_heading = 45.0f * PI / 180.0f;
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_min_park_angle = PI / 2.0f;

   /** \action Call Cta_Calc_Fac_Obj_Head_Adapt. */
   Cta_Calc_Fac_Obj_Head_Adapt(obj_heading, &core_cals, &inters_zone);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 1.0f);
}

/**
 * Calculate object heading adaption for various cases. This case: object heading negative, smaller than limit. Compare computed
 * value with manually calculated value. \uts{CSCSA-41855} \sdd{SF-3742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Obj_Head_Adapt__negative_angle_smaller_than_limit)
{
   /** \arrange Set up heading adaption parameters */
   float32_T obj_heading = -45.0f * PI / 180.0f;
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_min_park_angle = PI / 2.0f;

   /** \action Call Cta_Calc_Fac_Obj_Head_Adapt. */
   Cta_Calc_Fac_Obj_Head_Adapt(obj_heading, &core_cals, &inters_zone);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 1.0f);
}

/**
 * Calculate object heading adaption for various cases. This case: object heading positive, greater than limit. Compare computed
 * value with manually calculated value. \uts{CSCSA-41856} \sdd{SF-3742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Obj_Head_Adapt__positive_angle_greater_than_limit)
{
   /** \arrange Set up heading adaption parameters */
   float32_T obj_heading = PI / 2.0f;
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_min_park_angle = -PI / 2.0f;

   /** \action Call Cta_Calc_Fac_Obj_Head_Adapt. */
   Cta_Calc_Fac_Obj_Head_Adapt(obj_heading, &core_cals, &inters_zone);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 1.0f);
}

/**
 * Calculate object heading adaption for various cases. This case: object heading negative, greater than limit. Compare computed
 * value with manually calculated value. \uts{CSCSA-41857} \sdd{SF-3742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Obj_Head_Adapt__negative_angle_greater_than_limit)
{
   /** \arrange Set up heading adaption parameters */
   float32_T obj_heading = PI / 2.0f;
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   core_cals.k_cta_min_park_angle = -PI / 2.0f;

   /** \action Call Cta_Calc_Fac_Obj_Head_Adapt. */
   Cta_Calc_Fac_Obj_Head_Adapt(obj_heading, &core_cals, &inters_zone);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_FLOAT_EQ(inters_zone.target_head_fac, 1.0f);
}

#ifndef NDEBUG
/**
 * Calculate object heading adaption for various cases. This case: object heading equal the limit. Expect death.
 * \uts{CSCSA-306959} \sdd{SF-3742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Obj_Head_Adapt__angle_equal_heading)
{
   EXPECT_DEATH(
      {
         /** \arrange Set up heading adaption parameters */
         float32_T obj_heading = 4.0f;
         Cta_Inters_Zone_Ext_Param_T inters_zone{};
         core_cals.k_cta_min_park_angle = 4.0f;

         /** \action Call Cta_Calc_Fac_Obj_Head_Adapt. */
         Cta_Calc_Fac_Obj_Head_Adapt(obj_heading, &core_cals, &inters_zone);

         /** \assert assertion since heading_factor_inverse is below 0. */
      },
      ".heading_factor_inverse > EPSILON*");
}
#endif

/**
 * Use host depending extension factor lookup for various cases. This case: adapt steering angle active, adapt conflict zone active
 * Expect application of lookup table with a factor unequal zero. \uts{CSCSA-41858} \sdd{SF-3748} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Host_Dep_Ext_Fac__adapt_steering_angle_active_adapt_conflict_zone_active)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                                                      = CTA_MODE_FRONT;
   p_vehicle_data->steering_angle                            = 1.0f;
   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_TRUE;
   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Host_Dep_Ext_Fac(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_GE(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use host depending extension factor lookup for various cases. This case: adapt steering angle active, adapt conflict zone active
 * Expect application of lookup table with a factor equal zero. \uts{CSCSA-41859} \sdd{SF-3748} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Host_Dep_Ext_Fac__adapt_steering_angle_inactive_adapt_conflict_zone_inactive)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                           = CTA_MODE_FRONT;
   p_vehicle_data->steering_angle = 1.0f;

   core_cals.k_cta_f_adapt_intersect_lines_by_steering_angle = FBK_FALSE;
   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Host_Dep_Ext_Fac(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_EQ(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use host depending extension factor lookup for various cases. This case: adapt host speed active, adapt conflict zone active for
 * host speed active, expect factor not equal zero. \uts{CSCSA-188950} \sdd{SF-3748} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Host_Dep_Ext_Fac__adapt_host_speed_factor)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   p_vehicle_data->host_speed = 2.0f;

   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed = FBK_TRUE;
   core_cals.k_cta_rcta_host_speed_factor                = 0.2f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Host_Dep_Ext_Fac(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_EQ(inters_zone.host_vel_fac, 1.4f);
}

/**
 * Use steering angle adaption lookup for various cases. This case: front radar, steering angle positive Expect application of
 * lookup table with a factor unequal zero. \uts{CSCSA-41860} \sdd{SF-3743} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Steer_Angle_Adapt__front_radar_steering_angle_positive)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                           = CTA_MODE_FRONT;
   p_vehicle_data->steering_angle = 1.0f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Fac_Steer_Angle_Adapt(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_NE(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use steering angle adaption lookup for various cases. This case: front radar, steering angle negative Expect application of
 * lookup table with a factor unequal zero. \uts{CSCSA-41861} \sdd{SF-3743} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Steer_Angle_Adapt__front_radar_steering_angle_negative)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                           = CTA_MODE_FRONT;
   p_vehicle_data->steering_angle = -1.0f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Fac_Steer_Angle_Adapt(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_NE(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use steering angle adaption lookup for various cases. This case: rear radar, steering angle positive Expect application of
 * lookup table with a factor unequal zero. \uts{CSCSA-41862} \sdd{SF-3743} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Steer_Angle_Adapt__rear_radar_steering_angle_positive)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                           = CTA_MODE_REAR;
   p_vehicle_data->steering_angle = 1.0f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Fac_Steer_Angle_Adapt(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_NE(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use steering angle adaption lookup for various cases. This case: rear radar, steering angle negative Expect application of
 * lookup table with a factor unequal zero. \uts{CSCSA-41863} \sdd{SF-3743} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Steer_Angle_Adapt__rear_radar_steering_angle_negative)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                           = CTA_MODE_REAR;
   p_vehicle_data->steering_angle = -1.0f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Fac_Steer_Angle_Adapt(&inters_zone, &core_cals, p_vehicle_data);

   /** \assert Compare computed value with manually calculated value. */
   EXPECT_NE(inters_zone.host_steer_fac[mode], 0.0f);
}

/**
 * Use host speed based adaption for intersection point treshold. This case: rear radar, host speed negative, Expect host_steer_fac
 * to be greater than zero. \uts{CSCSA-188951} \sdd{CSCSA-187874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Conflict_Zone_Adapter_Test, Cta_Calc_Fac_Host_Speed_Adapt__negative_host_speed)
{
   /** \arrange Set up heading adaption parameters */
   Cta_Inters_Zone_Ext_Param_T inters_zone{};
   mode                                                  = CTA_MODE_REAR;
   p_vehicle_data->host_speed                            = -1.0f;
   core_cals.k_cta_f_adapt_intersect_lines_by_host_speed = FBK_TRUE;
   core_cals.k_cta_rcta_host_speed_factor                = 2.0f;

   /** \action Call Cta_Calc_Host_Dep_Ext_Fac. */
   Cta_Calc_Fac_Host_Speed_Adapt(&inters_zone, &core_cals, p_vehicle_data);


   /** \assert Compare computed value with manually calculated value. */
   EXPECT_NE(inters_zone.host_steer_fac[mode], 2.0f);
}
