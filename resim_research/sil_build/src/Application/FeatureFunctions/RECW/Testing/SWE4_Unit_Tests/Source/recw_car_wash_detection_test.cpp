/**
 * @file recw_car_wash_detection_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is car wash detection test source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44252}
 */

#include "recw_car_wash_detection_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_car_wash_detection.c"
#include "recw_car_wash_detection.h"
#include "recw_core_calibration.h"
#include "recw_types.h"
}

/**
 * Disable the car wash detection and set car wash flag to true for an object. Verify that the car wash detection returns false for
 * that object. \uts{CSCSA-44253} \sdd{SF-7868} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__returns_FALSE_for_RECW_CAR_WASH_OFF_mode)
{
   /** \arrange Set up cals and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_OFF;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns false for that object. */
   EXPECT_FALSE(result);
}

/**
 * Call the car wash detection with undefined activation cal. Verify that the car wash detection returns false.
 * \uts{CSCSA-44254} \sdd{SF-7868} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__returns_FALSE_for_unknown_RECW_CAR_WASH_mode)
{
   /** \arrange Set up cals and object car wash flag, for default. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = 255u;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns false for that object. */
   EXPECT_FALSE(result);
}

/**
 * Enable the car wash detection in target state mode and set car wash flag to false for an object. Verify that the car wash
 * detection returns false for that object in a setting where the flag is not changed in the current run. \uts{CSCSA-44255}
 * \sdd{SF-7868} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__works_properly_for_RECW_CAR_WASH_TARGET_STATE_mode_false_case)
{
   /** \arrange Set up cals, tracker data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index = obj_idx;
   /* Set object data such that car wash flag is not changed in run of Recw_Eval_Car_Wash_Conditions */
   recw_object.tracker_data.vcs_vel_rel.x    = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_object.tracker_data.age              = 0u;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_TARGET_STATE;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns false for that object. */
   EXPECT_FALSE(result);
}

/**
 * Enable the car wash detection in target state mode and set car wash flag to true for an object. Verify that the car wash
 * detection returns true for that object in a setting where the flag is not changed in the current run. \uts{CSCSA-44256}
 * \sdd{SF-7868} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__works_properly_for_RECW_CAR_WASH_TARGET_STATE_mode_true_case)
{
   /** \arrange Set up cals, tracker data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index = obj_idx;
   /* Set object data such that car wash flag is not changed in run of Recw_Eval_Car_Wash_Conditions */
   recw_object.tracker_data.vcs_vel_rel.x    = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_object.tracker_data.age              = 0u;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_TARGET_STATE;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns true for that object. */
   EXPECT_TRUE(result);
}

/**
 * Enable the car wash detection in neutral gear mode and set car wash flag to true for an object. Verify that the car wash
 * detection returns false for that object in a setting where the vehicle is not in neutral gear. \uts{CSCSA-44257} \sdd{SF-7868}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__works_properly_for_RECW_CAR_WASH_NEUTRAL_GEAR_mode_false_case)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_NEUTRAL_GEAR;
   p_vehicle_data->prndl                     = PA_VEH_PRNDL_STATE_DRIVE;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns false for that object. */
   EXPECT_FALSE(result);
}

/**
 * Enable the car wash detection in neutral gear mode and set car wash flag to true for an object. Verify that the car wash
 * detection returns false for that object in a setting where the vehicle has too high speed. \uts{CSCSA-112358} \sdd{SF-7868}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__RECW_CAR_WASH_NEUTRAL_GEAR_mode_false_case_speed_too_high)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Car_Wash_Scenario_Flags_T car_wash_scenario_flags;
   Recw_Set_Car_Wash_Flag_True(&car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_NEUTRAL_GEAR;
   p_vehicle_data->prndl                     = PA_VEH_PRNDL_STATE_NEUTRAL;
   recw_cals.k_recw_max_speed_ego_car_wash   = 3.0;
   p_vehicle_data->host_speed                = recw_cals.k_recw_max_speed_ego_car_wash + EPSILON;

   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &car_wash_scenario_flags);

   /** \assert Verify that the car wash detection returns false for that object. */
   EXPECT_FALSE(result);
}

/**
 * Enable the car wash detection in neutral gear mode and set car wash flag to false for an object. Verify that the car wash
 * detection returns true for that object in a setting where all conditions are met. \uts{CSCSA-44258} \sdd{SF-7868}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__works_properly_for_RECW_CAR_WASH_NEUTRAL_GEAR_mode_true_case)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_NEUTRAL_GEAR;
   p_vehicle_data->prndl                     = PA_VEH_PRNDL_STATE_NEUTRAL;
   recw_cals.k_recw_max_speed_ego_car_wash   = 3.0;
   p_vehicle_data->host_speed                = recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;


   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that the car wash detection returns true for that object. */
   EXPECT_TRUE(result);
}

/**
 * Enable the car wash detection in neutral gear mode and set car wash flag to false for an object. Verify that the car wash
 * detection returns true for that object in a setting where all conditions are met. \uts{CSCSA-112359} \sdd{SF-7868}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Obj_Car_Wash_Ghost__RECW_CAR_WASH_NEUTRAL_GEAR_mode_true_case_and_negative_speed)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;
   boolean_T result;

   Recw_Set_Car_Wash_Flag_False(&recw_pers.car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index            = obj_idx;
   recw_cals.k_recw_en_active_car_wash_logic = RECW_CAR_WASH_NEUTRAL_GEAR;
   p_vehicle_data->prndl                     = PA_VEH_PRNDL_STATE_NEUTRAL;
   recw_cals.k_recw_max_speed_ego_car_wash   = 3.0;
   p_vehicle_data->host_speed                = -recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;


   /** \action Call car wash detection. */
   result = Recw_Is_Obj_Car_Wash_Ghost(p_vehicle_data, &recw_cals, &recw_object, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that the car wash detection returns true for that object. */
   EXPECT_TRUE(result);
}

/**
 * Set car wash flag to true for an object and then call evaluation function with a relative velocity of the object below the
 * threshold. Verify that the evaluation function sets the car wash flag to false for that object. \uts{CSCSA-44259} \sdd{SF-7857}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_false_if_rel_vel_below_threshold)
{
   /** \arrange Set up cals, tracker data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index                    = obj_idx;
   recw_cals.k_recw_max_rel_lon_vel_release_car_wash = 3.0;
   recw_object.tracker_data.vcs_vel_rel.x            = recw_cals.k_recw_max_rel_lon_vel_release_car_wash / 2.0f;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that evaluation function set car wash flag for object to false. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age but other not-matching
 * conditions for the detection of a car wash ghost. Verify that the evaluation function does not change car wash flag of the
 * object. \uts{CSCSA-44260} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_false_if_age_matches_and_conditions_are_not_met)
{
   /** \arrange Set up cals, tracker data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* This is the condition that is not fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that evaluation function does not change car wash flag. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age and longitudianl distance
 * out of range for the detection of a car wash ghost. Verify that the evaluation function does not change car wash flag of the
 * object. \uts{CSCSA-112360} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_false_if_age_matches_and_lon_dist_is_out_of_range)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_False(&recw_pers.car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* Make sure all other conditions are fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;
   p_vehicle_data->host_speed                 = recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 1.0f;
   recw_object.tracker_data.vcs_pos.y         = recw_cals.k_recw_max_lat_distance_car_wash / 2.0f;
   recw_cals.k_recw_max_lon_distance_car_wash = 6.0f;
   recw_object.tracker_data.length            = 1.0f;
   p_vehicle_data->host_length                = 4.0f;
   /* This is the condition that is not fulfilled */
   recw_object.tracker_data.vcs_pos.x = recw_cals.k_recw_max_lon_distance_car_wash;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that evaluation function does not change car wash flag. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&recw_pers.car_wash_scenario_flags, obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age and lateral distance out of
 * range for the detection of a car wash ghost. Verify that the evaluation function does not change car wash flag of the object.
 * \uts{CSCSA-112361} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_false_if_age_matches_and_lat_dist_is_out_of_range)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_False(&recw_pers.car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* Make sure all other conditions are fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;
   p_vehicle_data->host_speed                 = recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 1.0f;
   recw_cals.k_recw_max_lon_distance_car_wash = 6.0f;
   recw_object.tracker_data.length            = 1.0f;
   p_vehicle_data->host_length                = 4.0f;
   recw_object.tracker_data.vcs_pos.x         = 0.5f;
   /* This is the condition that is not fulfilled */
   recw_object.tracker_data.vcs_pos.y = recw_cals.k_recw_max_lat_distance_car_wash + EPSILON;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that evaluation function does not change car wash flag. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&recw_pers.car_wash_scenario_flags, obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age and host speed too high for
 * the detection of a car wash ghost. Verify that the evaluation function does not change car wash flag of the object.
 * \uts{CSCSA-112362} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_false_if_age_matches_and_ego_speed_too_high)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_False(&recw_pers.car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* Make sure all other conditions are fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 1.0f;
   recw_object.tracker_data.vcs_pos.y         = recw_cals.k_recw_max_lat_distance_car_wash / 2.0f;
   recw_cals.k_recw_max_lon_distance_car_wash = 6.0f;
   recw_object.tracker_data.length            = 1.0f;
   p_vehicle_data->host_length                = 4.0f;
   recw_object.tracker_data.vcs_pos.x         = 0.5f;
   /* This is the condition that is not fulfilled */
   p_vehicle_data->host_speed = recw_cals.k_recw_max_speed_ego_car_wash + EPSILON;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that evaluation function does not change car wash flag. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&recw_pers.car_wash_scenario_flags, obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age and matching conditions for
 * the detection of a car wash ghost. Verify that the evaluation function sets the car wash flag to true for that object.
 * \uts{CSCSA-44261} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Eval_Car_Wash_Conditions__sets_flag_to_true_if_age_matches_and_conditions_are_met)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* Make sure all conditions are fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;
   p_vehicle_data->host_speed                 = recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 1.0f;
   recw_object.tracker_data.vcs_pos.y         = recw_cals.k_recw_max_lat_distance_car_wash / 2.0f;
   recw_cals.k_recw_max_lon_distance_car_wash = 6.0f;
   recw_object.tracker_data.length            = 1.0f;
   p_vehicle_data->host_length                = 4.0f;
   recw_object.tracker_data.vcs_pos.x         = 0.5f;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &(recw_pers.car_wash_scenario_flags));

   /** \assert Verify that evaluation function set car wash flag for object to true. */
   EXPECT_TRUE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx));
}

/**
 * Set car wash flag to false for an object and then call evaluation function with matching object age and matching conditions for
 * the detection of a car wash ghost. Verify that the evaluation function sets the car wash flag to true for that object.
 * \uts{CSCSA-112363} \sdd{SF-7857} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test,
       Recw_Eval_Car_Wash_Conditions__sets_flag_to_true_if_age_matches_and_conditions_are_met_with_negative_obj_pos)
{
   /** \arrange Set up cals, tracker data, vehicle data and object car wash flag. */
   uint8_t obj_idx = 4;

   Recw_Set_Car_Wash_Flag_False(&recw_pers.car_wash_scenario_flags, obj_idx);
   recw_object.tracker_data.index         = obj_idx;
   recw_object.tracker_data.vcs_vel_rel.x = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 2u;
   recw_object.tracker_data.age           = recw_cals.k_recw_min_object_age + 1u;
   /* Make sure all conditions are fulfilled */
   recw_cals.k_recw_min_rel_lon_vel_car_wash  = recw_cals.k_recw_max_rel_lon_vel_release_car_wash + 1u;
   recw_cals.k_recw_max_speed_ego_car_wash    = 2.0f;
   p_vehicle_data->host_speed                 = recw_cals.k_recw_max_speed_ego_car_wash / 2.0f;
   recw_cals.k_recw_max_lat_distance_car_wash = 1.0f;
   recw_object.tracker_data.vcs_pos.y         = -recw_cals.k_recw_max_lat_distance_car_wash / 2.0f;
   recw_cals.k_recw_max_lon_distance_car_wash = 6.0f;
   recw_object.tracker_data.length            = 1.0f;
   p_vehicle_data->host_length                = 4.0f;
   recw_object.tracker_data.vcs_pos.x         = -5.0f;

   /** \action Call evaluation function. */
   Recw_Eval_Car_Wash_Conditions(p_vehicle_data, &recw_cals, &recw_object, &recw_pers.car_wash_scenario_flags);

   /** \assert Verify that evaluation function set car wash flag for object to true. */
   EXPECT_TRUE(Recw_Is_Car_Wash_Scenario(&recw_pers.car_wash_scenario_flags, obj_idx));
}

/**
 * Set car wash flag to true for three objects. Verify that the Recw_Is_Car_Wash_Scenario function returns true for those objects.
 * \uts{CSCSA-44262} \sdd{SF-7858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Car_Wash_Scenario__true_Condition)
{
   /** \arrange Set car wash flags to true for three objects. */
   uint8_t obj_idx1 = 1;
   uint8_t obj_idx2 = 5;
   uint8_t obj_idx3 = 8;

   boolean_T result1;
   boolean_T result2;
   boolean_T result3;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \action Call Recw_Is_Car_Wash_Scenario function. */
   result1 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   result2 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   result3 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \assert Verify that true is returned for all three objects. */
   EXPECT_TRUE(result1);
   EXPECT_TRUE(result2);
   EXPECT_TRUE(result3);
}

/**
 * Set car wash flag to false for three objects. Verify that the Recw_Is_Car_Wash_Scenario function returns false for those
 * objects. \uts{CSCSA-44263} \sdd{SF-7858} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Is_Car_Wash_Scenario__false_Condition)
{
   /** \arrange Set car wash flags to false for three objects. */
   uint8_t obj_idx1 = 2;
   uint8_t obj_idx2 = 6;
   uint8_t obj_idx3 = 9;

   boolean_T result1;
   boolean_T result2;
   boolean_T result3;

   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \action Call Recw_Is_Car_Wash_Scenario function. */
   result1 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   result2 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   result3 = Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \assert Verify that false is returned for all three objects. */
   EXPECT_FALSE(result1);
   EXPECT_FALSE(result2);
   EXPECT_FALSE(result3);
}

/**
 * Set car wash flag to true for three objects. Verify that the initialization function for the car wash flags resets those flags
 * to false. \uts{CSCSA-44264} \sdd{SF-7867} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Init_Car_Wash_Flags__works_properly)
{
   /** \arrange Set car wash flags to true for three objects. */
   uint8_t obj_idx1 = 2;
   uint8_t obj_idx2 = 6;
   uint8_t obj_idx3 = 9;

   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \action Call initialization function. */
   Recw_Init_Car_Wash_Flags(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags);

   /** \assert Verfiy that flags are reset to false. */
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx1));
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx2));
   EXPECT_FALSE(Recw_Is_Car_Wash_Scenario(&(recw_pers.car_wash_scenario_flags), obj_idx3));
}

/**
 * Test that setting of the car wash flag to true works properly. Verify that bits are set accordingly.
 * \uts{CSCSA-44265} \sdd{SF-7860} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Set_Car_Wash_Flag_True__works_properly)
{
   /** \arrange Set up three object indices with different bit positions. */
   uint8_t obj_idx1 = 1;
   uint8_t obj_idx2 = 10;
   uint8_t obj_idx3 = 19;

   /** \action Call function Recw_Set_Car_Wash_Flag_True to set flags to true for those objects. */
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   Recw_Set_Car_Wash_Flag_True(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \assert Verfiy that bits are set accordingly. */
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx1 >> 3u], 2u);
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx2 >> 3u], 4u);
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx3 >> 3u], 8u);
}

/**
 * Test that setting of the car wash flag to false works properly. Verify that bits are set accordingly.
 * \uts{CSCSA-44266} \sdd{SF-7859} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Car_Wash_Detection_Test, Recw_Set_Car_Wash_Flag_False__works_properly)
{
   /** \arrange Set up three object indices with different bit positions. */
   uint8_t obj_idx1 = 1;
   uint8_t obj_idx2 = 10;
   uint8_t obj_idx3 = 19;

   /** \action Call function Recw_Set_Car_Wash_Flag_False to set flags to false for those objects. */
   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx1);
   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx2);
   Recw_Set_Car_Wash_Flag_False(&(recw_pers.car_wash_scenario_flags), obj_idx3);

   /** \assert Verfiy that bits are set accordingly. */
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx1 >> 3u], 0u);
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx2 >> 3u], 0u);
   EXPECT_EQ(recw_pers.car_wash_scenario_flags.possible_car_wash_scenario_flags[obj_idx3 >> 3u], 0u);
}
