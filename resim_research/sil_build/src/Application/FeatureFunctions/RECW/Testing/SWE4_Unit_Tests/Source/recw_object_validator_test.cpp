/**
 * @file recw_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44292}
 */

#include "recw_object_validator_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
extern "C"
{
#include "fbk_macros.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_vector_2d.h"
#include "recw_object_validator.c"
}


/**
 * Test that an object that fulfills required conditions is relevant. Verify that the object is classified as relevant.
 * \uts{CSCSA-44294} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_TRUE_if_all_conditions_are_met)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object that fulfills required conditions is relevant. Verify that the object is classified as relevant.
 * \uts{CSCSA-44295} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_TRUE_if_lanecheck_disabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.attributes.f_obj_is_within_lane = FBK_FALSE;
   recw_cals.k_recw_f_apply_lane_filter     = FBK_FALSE;


   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object that fulfills required conditions is relevant. Verify that the object is classified as relevant.
 * \uts{CSCSA-112370} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_TRUE_if_lanecheck_enabled_and_obj_witin_lane)
{
   /** \arrange Set up an object that is in the ego lane, set within-lane counter to threshold and enable lane filter. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Fill_Object_Valid_Crit(&recw_valid_criteria, &recw_pers, &recw_obj, &recw_cals);

   recw_cals.k_recw_f_apply_lane_filter                                       = 1u;
   recw_cals.k_recw_lane_filter_width                                         = 3.0f;
   recw_cals.k_recw_lane_width_slope                                          = 0.0f;
   recw_cals.k_recw_lane_filter_num_consecutive_cycles                        = 3u;
   recw_obj.tracker_data.index                                                = obj_idx;
   recw_obj.tracker_data.curvi_pos.x                                          = -1.0f;
   recw_obj.tracker_data.curvi_pos.y                                          = recw_cals.k_recw_lane_filter_width / 3.0f;
   recw_pers.object_data[recw_obj.tracker_data.id].object_within_lane_counter = recw_cals.k_recw_lane_filter_num_consecutive_cycles;
   recw_valid_criteria.lane_filter_width                                      = recw_cals.k_recw_lane_filter_width;
   p_vehicle_data->host_speed                                                 = 4.0f;

   recw_cals.k_recw_f_apply_lane_filter = FBK_TRUE;


   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object coasted over threshold cycles is not relevant. Verify that the object is classified as not relevant.
 * \uts{CSCSA-112371} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_coasted_cycles_to_ignore_obj_reached)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for coasted cycles. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_obj.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.stage_age = recw_cals.k_recw_max_allowed_consecutive_coasted_cycles + 1u;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object classified as traffic light ghost is not relevant. Verify that the object is classified as not relevant.
 * \uts{CSCSA-112372} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_is_traffic_light_ghost)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for traffic light ghost flag. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 1u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets - 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets - 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length + 0.1f;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with invalid heading is not relevant. Verify that the object is classified as not relevant.
 * \uts{CSCSA-112373} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_heading_not_relevant)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for heading above threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   Recw_Fill_Object_Valid_Crit(&recw_valid_criteria, &recw_pers, &recw_obj, &recw_cals);

   recw_valid_criteria.max_approach_angle = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.tracker_data.vcs_heading      = 1.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a width above the calibrated threshold is not relevant. Verify that the object is classified as not
 * relevant. \uts{CSCSA-44296} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_width_above_threshold)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for width above threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.width = 1.1f * recw_cals.k_recw_max_object_width_warn_on;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a velocity below the calibrated threshold is not relevant. Verify that the object is classified as not
 * relevant. \uts{CSCSA-44297} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_speed_below_stationary_threshold)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for velocity below threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.speed = recw_cals.k_recw_min_speed_not_stationary - EPSILON;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a relative velocity above the calibrated upper threshold is not relevant. Verify that the object is
 * classified as not relevant. \uts{CSCSA-44298} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_rel_vel_above_upper_threshold)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for relative velocity above threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.vcs_vel_rel.x     = 1.5f * recw_cals.k_recw_max_rel_velocity[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.attributes.effective_rel_vel.x = recw_obj.tracker_data.vcs_vel_rel.x;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a relative velocity below the calibrated lower threshold is not relevant. Verify that the object is
 * classified as not relevant. \uts{CSCSA-44299} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_rel_vel_below_lower_threshold)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for relative velocity below threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1] - 1.0f;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}


/**
 * Test that an object with an existence probability below the calibrated threshold is not relevant. Verify that the object is
 * classified as not relevant. \uts{CSCSA-44300} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_exist_prob_is_too_low)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for existence probability threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.existence_probability = recw_cals.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_1] - EPSILON;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with age below the calibrated threshold is not relevant. Verify that the object is classified as not
 * relevant. \uts{CSCSA-112374} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_age_is_too_low)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for object age threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_pers.object_data[recw_obj.tracker_data.id].age = recw_cals.k_recw_min_object_age - FBK_ONE_UINT;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with an eclipse value over the calibrated threshold is not relevant. Verify that the object is classified as
 * not relevant. \uts{CSCSA-112375} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_eclipse_value_is_too_high)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for eclipse value threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.eclipse_value = recw_cals.k_recw_max_eclipse_value_for_valid_object + EPSILON;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a heading accuracy over the calibrated threshold is not relevant. Verify that the object is classified
 * as not relevant. \uts{CSCSA-112376} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_accuracy_heading_is_too_high)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for accuracy heading threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);
   recw_obj.tracker_data.accuracy_heading = recw_cals.k_recw_heading_accuracy_threshold + EPSILON;

   /** \action Call function that evaluates if object is relevant. */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that object is flagged as carwash ghost. Expect FALSE on function output
 * \uts{CSCSA-204753} \sdd{SF-7925} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_Relevant__returns_FALSE_if_obj_is_car_wash_ghost)
{
   /** \arrange Setup calibration values, create object which possibly is car wash ghost */
   boolean_T result;
   uint8_t obj_idx = 5u;

   recw_cals.k_recw_en_active_car_wash_logic  = RECW_CAR_WASH_TARGET_STATE;
   recw_cals.k_recw_max_lon_distance_car_wash = 5.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 5.0f;
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = 0.0f;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_obj.attributes.f_object_is_car_wash_ghost = FBK_TRUE;
   recw_obj.tracker_data.vcs_pos.x                = 0.5f;
   recw_obj.tracker_data.vcs_pos.y                = 1.0f;
   recw_obj.tracker_data.age                      = recw_cals.k_recw_min_object_age + 1u;

   /** \action Call Recw_Is_Object_Relevant func */
   result = Recw_Is_Object_Relevant(&recw_obj, &recw_pers, p_vehicle_data, &recw_cals, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that object will not be considered as relevant */
   EXPECT_FALSE(result);
}

/**
 * Test that an object in the ego lane is correctly classified if the calibrated minimal number of cycles is reached. Verify that
 * true is returned for ego lane check. \uts{CSCSA-44301} \sdd{SF-7921} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_In_Ego_Lane__returns_TRUE_if_obj_is_in_ego_lane)
{
   /** \arrange Set up an object that is in the ego lane, set within-lane counter to threshold and enable lane filter. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   boolean_T result;
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 1u;

   recw_cals.k_recw_f_apply_lane_filter                     = 1u;
   recw_cals.k_recw_lane_filter_width                       = 3.0f;
   recw_cals.k_recw_lane_width_slope                        = 0.0f;
   recw_cals.k_recw_lane_filter_num_consecutive_cycles      = 3u;
   recw_obj.tracker_data.index                              = obj_idx;
   recw_obj.tracker_data.id                                 = obj_id;
   recw_obj.tracker_data.curvi_pos.x                        = -1.0f;
   recw_obj.tracker_data.curvi_pos.y                        = recw_cals.k_recw_lane_filter_width / 3.0f;
   recw_pers.object_data[obj_id].object_within_lane_counter = recw_cals.k_recw_lane_filter_num_consecutive_cycles;
   recw_valid_criteria.lane_filter_width                    = recw_cals.k_recw_lane_filter_width;
   p_vehicle_data->host_speed                               = 4.0f;

   /** \action Call function that evaluates if object is in ego lane. */
   result = Recw_Is_Object_In_Ego_Lane(&recw_pers, &recw_valid_criteria, &recw_obj, p_vehicle_data, &recw_cals);

   /** \assert Verify that true is returned for ego lane check and within-lane counter is incremented. */
   EXPECT_TRUE(result);
   EXPECT_EQ(recw_pers.object_data[obj_id].object_within_lane_counter, recw_cals.k_recw_lane_filter_num_consecutive_cycles + 1);
}

/**
 * Test that VCS coordinates are used for the assesment if object in the ego lane if the ego vehicle is standing.
 * \uts{CSCSA-44302} \sdd{SF-7921} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_In_Ego_Lane__uses_vcs_coordinates_if_ego_is_standing)
{
   /** \arrange Set up an object that is in the ego lane regarding VCS coordinates, but outside ego lane regarding curvi
    * coordinates. Set within-lane counter to threshold, enable lane filter and set ego vehicle speed to zero. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   boolean_T result;
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 9u;

   recw_cals.k_recw_f_apply_lane_filter                     = 1u;
   recw_cals.k_recw_lane_filter_width                       = 3.0f;
   recw_cals.k_recw_lane_width_slope                        = 0.0f;
   recw_cals.k_recw_lane_filter_num_consecutive_cycles      = 3u;
   recw_obj.tracker_data.index                              = obj_idx;
   recw_obj.tracker_data.id                                 = obj_id;
   recw_obj.tracker_data.vcs_pos.x                          = -1.0f;
   recw_obj.tracker_data.vcs_pos.y                          = recw_cals.k_recw_lane_filter_width / 3.0f;
   recw_obj.tracker_data.curvi_pos.x                        = -1.0f;
   recw_obj.tracker_data.curvi_pos.y                        = recw_cals.k_recw_lane_filter_width * 3.0f;
   recw_pers.object_data[obj_id].object_within_lane_counter = recw_cals.k_recw_lane_filter_num_consecutive_cycles;
   recw_valid_criteria.lane_filter_width                    = recw_cals.k_recw_lane_filter_width;
   p_vehicle_data->host_speed                               = 0.0f;

   /** \action Call function that evaluates if object is in ego lane. */
   result = Recw_Is_Object_In_Ego_Lane(&recw_pers, &recw_valid_criteria, &recw_obj, p_vehicle_data, &recw_cals);

   /** \assert Verify that true is returned for ego lane check and within-lane counter is incremented. */
   EXPECT_TRUE(result);
   EXPECT_EQ(recw_pers.object_data[obj_id].object_within_lane_counter, recw_cals.k_recw_lane_filter_num_consecutive_cycles + 1);
}

/**
 * Test that an object in the ego lane is correctly classified if the calibrated minimal number of cycles is not reached. Verify
 * that false is returned for ego lane check. \uts{CSCSA-44303} \sdd{SF-7921} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_In_Ego_Lane__returns_FALSE_if_num_consecutive_cycles_too_low)
{
   /** \arrange Set up an object that is in the ego lane, set within-lane counter below threshold and enable lane filter. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   boolean_T result;
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 4u;

   recw_cals.k_recw_f_apply_lane_filter                     = 1u;
   recw_cals.k_recw_lane_filter_width                       = 3.0f;
   recw_cals.k_recw_lane_width_slope                        = 0.0f;
   recw_cals.k_recw_lane_filter_num_consecutive_cycles      = 3u;
   recw_obj.tracker_data.index                              = obj_idx;
   recw_obj.tracker_data.id                                 = obj_id;
   recw_obj.tracker_data.curvi_pos.x                        = -1.0f;
   recw_obj.tracker_data.curvi_pos.y                        = recw_cals.k_recw_lane_filter_width / 3.0f;
   recw_pers.object_data[obj_id].object_within_lane_counter = recw_cals.k_recw_lane_filter_num_consecutive_cycles - 1u;
   recw_valid_criteria.lane_filter_width                    = recw_cals.k_recw_lane_filter_width;
   p_vehicle_data->host_speed                               = 4.0f;

   /** \action Call function that evaluates if object is in ego lane. */
   result = Recw_Is_Object_In_Ego_Lane(&recw_pers, &recw_valid_criteria, &recw_obj, p_vehicle_data, &recw_cals);

   /** \assert Verify that false is returned for ego lane check and within-lane counter is incremented. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.object_data[obj_id].object_within_lane_counter, recw_cals.k_recw_lane_filter_num_consecutive_cycles);
}

/**
 * Test that an object outside of the ego lane is correctly classified. Verify that false is returned for ego lane check.
 * \uts{CSCSA-44304} \sdd{SF-7921} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Object_In_Ego_Lane__returns_FALSE_if_obj_outside_ego_lane)
{
   /** \arrange Set up an object that is outside of the ego lane, set within-lane counter to threshold and enable lane filter. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   boolean_T result;
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 12u;

   recw_cals.k_recw_f_apply_lane_filter                     = 1u;
   recw_cals.k_recw_lane_filter_width                       = 3.0f;
   recw_cals.k_recw_lane_width_slope                        = 0.0f;
   recw_cals.k_recw_lane_filter_num_consecutive_cycles      = 3u;
   recw_obj.tracker_data.index                              = obj_idx;
   recw_obj.tracker_data.id                                 = obj_id;
   recw_obj.tracker_data.curvi_pos.x                        = -1.0f;
   recw_obj.tracker_data.curvi_pos.y                        = recw_cals.k_recw_lane_filter_width;
   recw_pers.object_data[obj_id].object_within_lane_counter = recw_cals.k_recw_lane_filter_num_consecutive_cycles;
   recw_valid_criteria.lane_filter_width                    = recw_cals.k_recw_lane_filter_width;
   p_vehicle_data->host_speed                               = 4.0f;

   /** \action Call function that evaluates if object is in ego lane. */
   result = Recw_Is_Object_In_Ego_Lane(&recw_pers, &recw_valid_criteria, &recw_obj, p_vehicle_data, &recw_cals);

   /** \assert Verify that false is returned for ego lane check and within-lane counter is reset to zero. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.object_data[obj_id].object_within_lane_counter, FBK_ZERO_INT);
}

/**
 * Test that the objects longitudinal velocity is in valid range Longitudinal and lateral relative velocity shall be in permissible
 * range. Thus true is expected. \uts{CSCSA-44305} \sdd{SF-7919} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Long_Rel_Vel_In_Valid_Range__true_since_all_conditions_fulfilled)
{
   /** \arrange Set up an object whose velocity components are in valid range */
   boolean_T result;
   Recw_Obj_Valid_Crit_T recw_valid_criteria;
   recw_valid_criteria.long_rel_vel_range = Create_Float_Range(0.0f, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
   recw_obj.tracker_data.vcs_vel_rel.x    = 0.5f * recw_cals.k_recw_max_allowed_rel_vel_long_diff;
   recw_obj.tracker_data.vcs_vel_rel.y    = 0.5f * recw_cals.k_recw_max_allowed_rel_vel_lat_diff;
   recw_obj.attributes.effective_rel_vel  = Create_2d_Vector_Origin();

   /** \action Call function that evaluates whether velocity components are in valid range. */
   result = Recw_Is_Long_Rel_Vel_In_Valid_Range(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that true is returned for velocity range check */
   EXPECT_TRUE(result);
}


/**
 * Test that the objects longitudinal velocity is in valid range Longitudinal velocity shall be outside of the validity range. Thus
 * false is expected. \uts{CSCSA-44306} \sdd{SF-7919} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Long_Rel_Vel_In_Valid_Range__false_long_rel_vel_outside_of_range)
{
   /** \arrange Set up an object whose velocity components are outside valid range */
   boolean_T result;
   Recw_Obj_Valid_Crit_T recw_valid_criteria;
   recw_valid_criteria.long_rel_vel_range = Create_Float_Range(0.0f, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
   recw_obj.tracker_data.vcs_vel_rel.x    = 1.1f * recw_cals.k_recw_max_allowed_rel_vel_long_diff;
   /** \action Call function that evaluates whether velocity components are in valid range. */
   result = Recw_Is_Long_Rel_Vel_In_Valid_Range(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that false is returned for velocity range check */
   EXPECT_FALSE(result);
}

/**
 * Test that the objects longitudinal velocity is in valid range Longitudinal velocity difference exceeds the permissible
 * threshold. Thus false is expected. \uts{CSCSA-44307} \sdd{SF-7919} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Long_Rel_Vel_In_Valid_Range__long_rel_vel_difference_outside_of_range)
{
   /** \arrange Set up an object whose velocity components are outside valid range */
   boolean_T result;
   Recw_Obj_Valid_Crit_T recw_valid_criteria;
   recw_valid_criteria.long_rel_vel_range  = Create_Float_Range(0.0f, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
   recw_obj.tracker_data.vcs_vel_rel.x     = recw_valid_criteria.long_rel_vel_range.min;
   recw_obj.attributes.effective_rel_vel.x = recw_valid_criteria.long_rel_vel_range.min;

   recw_obj.tracker_data.vcs_vel_rel.y     = 0.0f;
   recw_obj.attributes.effective_rel_vel.y = recw_obj.tracker_data.vcs_vel_rel.y + recw_cals.k_recw_max_allowed_rel_vel_lat_diff;


   /** \action Call function that evaluates whether velocity components are in valid range. */
   result = Recw_Is_Long_Rel_Vel_In_Valid_Range(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that false is returned for velocity range check */
   EXPECT_FALSE(result);
}


/**
 * Test that the objects longitudinal velocity is in valid range Longitudinal velocity difference exceeds the permissible
 * threshold. Thus false is expected. \uts{CSCSA-44308} \sdd{SF-7919} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Long_Rel_Vel_In_Valid_Range__lat_rel_vel_difference_outside_of_range)
{
   /** \arrange Set up an object whose velocity components are outside valid range */
   boolean_T result;
   Recw_Obj_Valid_Crit_T recw_valid_criteria;
   recw_valid_criteria.long_rel_vel_range = Create_Float_Range(0.0f, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
   recw_obj.tracker_data.vcs_vel_rel.x    = 0.9f * recw_cals.k_recw_max_allowed_rel_vel_long_diff;
   recw_obj.attributes.effective_rel_vel.x =
      recw_obj.tracker_data.vcs_vel_rel.x + 1.1f * recw_cals.k_recw_max_allowed_rel_vel_long_diff;

   /** \action Call function that evaluates whether velocity components are in valid range. */
   result = Recw_Is_Long_Rel_Vel_In_Valid_Range(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that false is returned for velocity range check */
   EXPECT_FALSE(result);
}

/**
 * Test that object that fulfills all conditions is classified as traffic light ghost. Verify that object is classified as traffic
 * light ghost. \uts{CSCSA-44309} \sdd{SF-7922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Traffic_Light_Ghost__returns_TRUE_if_all_conditions_are_met)
{
   /** \arrange Set up object that fulfills conditions to be classified as traffic light ghost. */
   boolean_T result;

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 1u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets - 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets - 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length + 0.1f;

   /** \action Call function to evaluate if object is a traffic light ghost. */
   result = Recw_Is_Traffic_Light_Ghost(p_vehicle_data, &recw_obj, &recw_cals);

   /** \assert Verify that object is classified as traffic light ghost. */
   EXPECT_TRUE(result);
}

/**
 * Test that objects are not classified as traffic light ghost if ghost detection is disabled. Verify that an object that fulfills
 * all conditions is not classified as traffic light ghost. \uts{CSCSA-44310} \sdd{SF-7922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Traffic_Light_Ghost__returns_FALSE_if_ghost_detection_is_disabled)
{
   /** \arrange Set up object that fulfills conditions to be classified as traffic light ghost and disable ghost detection. */
   boolean_T result;

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 0u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets - 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets - 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length + 0.1f;

   /** \action Call function to evaluate if object is a traffic light ghost. */
   result = Recw_Is_Traffic_Light_Ghost(p_vehicle_data, &recw_obj, &recw_cals);

   /** \assert Verify that object is not classified as traffic light ghost. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object that is too old is not detected as an traffic light ghost. Verify that object is not classified as traffic
 * light ghost. \uts{CSCSA-44311} \sdd{SF-7922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Traffic_Light_Ghost__returns_FALSE_if_obj_age_too_large)
{
   /** \arrange Set up object that fulfills conditions to be classified as traffic light ghost, except for age too large. */
   boolean_T result;

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 1u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets + 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets - 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length + 0.1f;

   /** \action Call function to evaluate if object is a traffic light ghost. */
   result = Recw_Is_Traffic_Light_Ghost(p_vehicle_data, &recw_obj, &recw_cals);

   /** \assert Verify that object is not classified as traffic light ghost. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object that is too fast is not detected as an traffic light ghost. Verify that object is not classified as traffic
 * light ghost. \uts{CSCSA-44312} \sdd{SF-7922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Traffic_Light_Ghost__returns_FALSE_if_obj_velocity_is_too_large)
{
   /** \arrange Set up object that fulfills conditions to be classified as traffic light ghost, except for velocity too large. */
   boolean_T result;

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 1u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets - 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets + 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length + 0.1f;

   /** \action Call function to evaluate if object is a traffic light ghost. */
   result = Recw_Is_Traffic_Light_Ghost(p_vehicle_data, &recw_obj, &recw_cals);

   /** \assert Verify that object is not classified as traffic light ghost. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object that is located to far from the ego is not detected as an traffic light ghost. Verify that object is not
 * classified as traffic light ghost. \uts{CSCSA-44313} \sdd{SF-7922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Traffic_Light_Ghost__returns_FALSE_if_obj_position_does_not_match_criteria)
{
   /** \arrange Set up object that fulfills conditions to be classified as traffic light ghost, except for object position. */
   boolean_T result;

   recw_cals.k_recw_f_enable_traffic_light_ghost_detection = 1u;
   recw_cals.k_recw_min_age_for_close_slow_targets         = 4u;
   recw_cals.k_recw_min_abs_speed_for_young_close_targets  = 2.0f;
   recw_cals.k_recw_min_dist_for_young_slow_targets        = 1.0f;
   recw_obj.tracker_data.age                               = recw_cals.k_recw_min_age_for_close_slow_targets - 1u;
   recw_obj.tracker_data.vcs_vel.x                         = recw_cals.k_recw_min_abs_speed_for_young_close_targets - 0.1f;
   recw_obj.tracker_data.vcs_pos.x = -recw_cals.k_recw_min_dist_for_young_slow_targets - p_vehicle_data->host_length - 0.1f;

   /** \action Call function to evaluate if object is a traffic light ghost. */
   result = Recw_Is_Traffic_Light_Ghost(p_vehicle_data, &recw_obj, &recw_cals);

   /** \assert Verify that object is not classified as traffic light ghost. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object that is behind of the ego is classified as behind the ego.
 * \uts{CSCSA-44314} \sdd{SF-7918} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Behind_Ego__returns_TRUE_for_object_behind_of_ego)
{
   /** \arrange Set up object that is located behind of ego. */
   boolean_T result;

   recw_obj.tracker_data.vcs_pos.x = -10.0f;
   recw_obj.tracker_data.length    = 5.0f;

   /** \action Call function to evaluate if object is behind of ego. */
   result = Recw_Is_Obj_Behind_Ego(&recw_obj);

   /** \assert Verify that object is classified as behind of ego. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object that is in front of the ego is not classified as behind the ego.
 * \uts{CSCSA-44315} \sdd{SF-7918} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Behind_Ego__returns_FALSE_for_object_in_front_of_ego)
{
   /** \arrange Set up object that is located in front of ego. */
   boolean_T result;

   recw_obj.tracker_data.vcs_pos.x = 10.0f;
   recw_obj.tracker_data.length    = 5.0f;

   /** \action Call function to evaluate if object is behind of ego. */
   result = Recw_Is_Obj_Behind_Ego(&recw_obj);

   /** \assert Verify that object is not classified as behind of ego. */
   EXPECT_FALSE(result);
}

/**
 * Test that the object validation criteria are filled properly if no hysteresis is present.
 * \uts{CSCSA-44316} \sdd{SF-7917} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Fill_Object_Valid_Crit__fills_correct_values_without_hysteresis)
{
   /** \arrange Set up persistent data and object data such that hysteresis should not be used. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};

   recw_obj.tracker_data.id     = 4u;
   recw_pers.recw_id_prev_cycle = 6u;

   /** \action Call function Recw_Fill_Object_Valid_Crit to fill object validation criteria. */
   Recw_Fill_Object_Valid_Crit(&recw_valid_criteria, &recw_pers, &recw_obj, &recw_cals);

   /** \assert Verify that object validation criteria are filled with default values without hysteresis. */
   EXPECT_FLOAT_EQ(recw_valid_criteria.long_rel_vel_range.min, recw_cals.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_valid_criteria.long_rel_vel_range.max, recw_cals.k_recw_max_rel_velocity[RECW_ALERT_ACTIVE_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_valid_criteria.max_approach_angle, recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_valid_criteria.min_existence_probability, recw_cals.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_valid_criteria.lane_filter_width, recw_cals.k_recw_lane_filter_width);
}

/**
 * Test that the object validation criteria are filled properly if hysteresis is present.
 * \uts{CSCSA-44317} \sdd{SF-7917} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Fill_Object_Valid_Crit__fills_correct_values_with_hysteresis)
{
   /** \arrange Set up persistent data and object data such that hysteresis should be used. */
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};

   recw_obj.tracker_data.id     = 4u;
   recw_pers.recw_id_prev_cycle = 4u;

   /** \action Call function Recw_Fill_Object_Valid_Crit to fill object validation criteria. */
   Recw_Fill_Object_Valid_Crit(&recw_valid_criteria, &recw_pers, &recw_obj, &recw_cals);

   /** \assert Verify that object validation criteria are filled with default values with hysteresis. */
   EXPECT_FLOAT_EQ(recw_valid_criteria.long_rel_vel_range.min, recw_cals.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1]
                                                                  - recw_cals.k_recw_min_rel_velocity_hys[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_valid_criteria.long_rel_vel_range.max,
                   recw_cals.k_recw_max_rel_velocity[RECW_ALERT_ACTIVE_LEVEL_1] + recw_cals.k_recw_max_rel_velocity_hys);
   EXPECT_FLOAT_EQ(recw_valid_criteria.max_approach_angle,
                   recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1] + recw_cals.k_recw_max_heading_hys);
   EXPECT_FLOAT_EQ(recw_valid_criteria.min_existence_probability,
                   recw_cals.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_1] - recw_cals.k_recw_min_existence_prob_hys);
   EXPECT_FLOAT_EQ(recw_valid_criteria.lane_filter_width, recw_cals.k_recw_lane_filter_width + recw_cals.k_recw_lane_filter_width_hys);
}

/**
 * Test that an object with a heading above the calibrated threshold is not relevant if heading filter is disabled. Verify that the
 * object heading is classified as not relevant. \uts{CSCSA-44318} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Heading_Relevant__returns_FALSE_if_obj_heading_is_above_threshold_and_filter_is_disabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for heading above threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.tracker_data.vcs_heading      = 1.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a filtered heading below the calibrated threshold is relevant if heading filter is disabled. Verify
 * that the object heading is classified as relevant. \uts{CSCSA-112377} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Heading_Relevant__returns_TRUE_if_obj_heading_is_below_threshold_and_filter_is_disabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.tracker_data.vcs_heading      = 0.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object with a filtered heading below the calibrated threshold is relevant if heading filter is disabled. Verify
 * that the object heading is classified as relevant. \uts{CSCSA-112378} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test,
       Recw_Is_Obj_Heading_Relevant__returns_TRUE_if_obj_heading_is_negative_below_threshold_and_filter_is_disabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.tracker_data.vcs_heading      = -0.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object with a filtered heading above the calibrated threshold is not relevant if heading filter is enabled. Verify
 * that the object heading is classified as not relevant. \uts{CSCSA-44319} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Heading_Relevant__returns_FALSE_if_obj_heading_is_above_threshold_and_filter_is_enabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant except for filtered heading above threshold. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};

   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle   = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.attributes.filtered_heading     = 1.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_cals.k_recw_f_enable_heading_filter = FBK_TRUE;

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that an object with a filtered heading below the calibrated threshold is relevant if heading filter is enabled. Verify that
 * the object heading is classified as relevant. \uts{CSCSA-112379} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Heading_Relevant__returns_TRUE_if_obj_heading_is_below_threshold_and_filter_is_enabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle   = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.attributes.filtered_heading     = 0.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_cals.k_recw_f_enable_heading_filter = FBK_TRUE;

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object with a filtered heading below the calibrated threshold is relevant if heading filter is enabled. Verify that
 * the object heading is classified as relevant. \uts{CSCSA-112380} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test,
       Recw_Is_Obj_Heading_Relevant__returns_TRUE_if_obj_heading_is_negative_below_threshold_and_filter_is_enabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle   = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.attributes.filtered_heading     = -0.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_cals.k_recw_f_enable_heading_filter = FBK_TRUE;

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Test that an object with a heading diff over threshold is not relevant if heading filter is enabled. Verify that the object
 * heading is classified as not relevant. \uts{CSCSA-112381} \sdd{SF-7920} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Is_Obj_Heading_Relevant__returns_FALSE_if_obj_heading_diff_over_allowed_and_filter_is_enabled)
{
   /** \arrange Set up an object that fulfills all conditions to be relevant. */
   boolean_T result;
   uint8_t obj_idx = 4u;
   Recw_Obj_Valid_Crit_T recw_valid_criteria{};
   Recw_Create_Valid_Object(&recw_obj, &recw_cals, obj_idx);

   recw_valid_criteria.max_approach_angle = recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.attributes.filtered_heading   = -0.5f * recw_cals.k_recw_max_heading[RECW_ALERT_ACTIVE_LEVEL_1];
   recw_obj.tracker_data.vcs_heading = recw_obj.attributes.filtered_heading + recw_cals.k_recw_max_allowed_heading_diff + EPSILON;
   recw_cals.k_recw_f_enable_heading_filter = FBK_TRUE;

   /** \action Call function that evaluates if heading is relevant. */
   result = Recw_Is_Obj_Heading_Relevant(&recw_valid_criteria, &recw_obj, &recw_cals);

   /** \assert Verify that heading is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Test that a coasted object with a stage age above the threshold is classified as not relevant.
 * \uts{CSCSA-44320} \sdd{SF-7916} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached__returns_TRUE_if_coasted_cycles_reached_threshold)
{
   /** \arrange Set up an object with status coasted and stage age above threshold. */
   boolean_T result;

   recw_obj.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.stage_age = recw_cals.k_recw_max_allowed_consecutive_coasted_cycles + 1u;

   /** \action Call function that evaluates coasted cycles. */
   result = Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached(&recw_obj, &recw_cals);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(result);
}

/**
 * Test that a coasted object with a stage age below the threshold is classified as relevant.
 * \uts{CSCSA-112382} \sdd{SF-7916} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Object_Validator_Test, Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached__returns_FALSE_if_coasted_cycles_not_reached_threshold)
{
   /** \arrange Set up an object with status coasted and stage age below threshold. */
   boolean_T result;

   recw_obj.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.stage_age = recw_cals.k_recw_max_allowed_consecutive_coasted_cycles - 1u;

   /** \action Call function that evaluates coasted cycles. */
   result = Recw_Are_Coasted_Cycles_To_Ignore_Obj_Reached(&recw_obj, &recw_cals);

   /** \assert Verify that false is returned. */
   EXPECT_FALSE(result);
}
