/**
 * @file scw_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW unit tests
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44460}
 */

#include "scw_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "scw.c"
#include "scw_input_generator.h"
#include "scw_types.h"
}

#define SCW_MAX_LATERAL_TTC 10.0f

/*
 * Tests whether the algorithm runs when the feature is activated.
 * \uts{CSCSA-44461} \sdd{SF-8050} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Core_Run__run_algorithm)
{
   /** \arrange Set up valid inputs. */
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;

   /** \action Call the core run function */
   Scw_Core_Run(p_scw_core_output, p_scw_core_input, p_scw_persistent, p_scw_calibration);

   /** \assert Verify that the core algorithm runs by checking that the alert level is reset. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests whether the feature is reset when the feature is deactivated.
 * \uts{CSCSA-44462} \sdd{SF-8050} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Core_Run__reset_scw_if_not_enabled)
{
   /** \arrange Disable SCW. */
   p_scw_core_input->f_scw_enable                = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;

   /** \action Call the core run function */
   Scw_Core_Run(p_scw_core_output, p_scw_core_input, p_scw_persistent, p_scw_calibration);

   /** \assert Verify that the alert level is reset. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/**
 * Tests whether the feature is reset when the host speed is below the threshold.
 * \uts{CSCSA-44513} \sdd{SF-8050} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Core_Run__reset_scw_if_host_speed_below_threshold)
{
   /** \arrange Set up host speed below threshold. */
   p_scw_calibration->k_scw_min_host_speed       = 3.0f;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed - EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;

   /** \action Call the core run function */
   Scw_Core_Run(p_scw_core_output, p_scw_core_input, p_scw_persistent, p_scw_calibration);

   /** \assert Verify that the alert level is reset. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests the SCW feature main algorithm for a critical object.
 * \uts{CSCSA-44463} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_object)
{
   /** \arrange Set up a critical SCW situation. */
   uint8_t obj_index                             = 1u;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]  = obj_index;
   object_data->length                           = 4.0f;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.y              = -2.0f;
   object_data[obj_index].age                    = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[obj_index].vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   object_data[obj_index].heading_rate  = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;
   object_data[obj_index].vcs_vel_rel.x = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].vcs_vel_rel.y = 0.0f;
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold = FBK_ZERO_INT;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_1);
}

/*
 * Tests the SCW feature main algorithm for a critical object, too low lateral ttc.
 * \uts{CSCSA-121122} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_object_level_2_lat_ttc)
{
   /** \arrange Set up a critical SCW situation. */
   uint8_t obj_index                             = 1u;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                    = 1.8f;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].length                 = 4.0f;
   object_data[obj_index].width                  = 1.8f;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.y     = -(0.5f * object_data[obj_index].width + p_scw_calibration->k_scw_min_dynamic_lat_distance
                                        + 0.5f * p_vehicle_data->host_width + EPSILON);
   object_data[obj_index].age           = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[obj_index].vcs_heading   = FBK_ZERO_F;
   object_data[obj_index].heading_rate  = FBK_ZERO_F;
   object_data[obj_index].vcs_vel_rel.x = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].vcs_vel_rel.y = 1.0f;
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold = FBK_ZERO_INT;

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_2);
}

/*
 * Tests the SCW feature main algorithm for a critical object, low lateral ttc, trailer attached and disabled.
 * \uts{CSCSA-204182} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_object_low_lat_ttc_trailer_attached_disabled)
{
   /** \arrange Set up a critical SCW situation. */
   uint8_t obj_index = 1u;

   p_scw_calibration->k_scw_f_enable_trailer_ttc_extension = FBK_FALSE;
   p_scw_calibration->k_scw_trailer_lat_ttc_extension      = 0.5f;
   p_scw_core_input->trailer.f_present                     = FBK_TRUE;
   p_scw_core_input->trailer.length                        = 4.0f;
   p_scw_core_input->trailer.width                         = 2.0f;

   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                    = 1.8f;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].length                 = 4.0f;
   object_data[obj_index].width                  = 1.8f;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.y     = -(0.5f * object_data[obj_index].width + p_scw_calibration->k_scw_min_dynamic_lat_distance
                                        + 0.5f * p_scw_core_input->trailer.width + EPSILON);
   object_data[obj_index].age           = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[obj_index].vcs_heading   = FBK_ZERO_F;
   object_data[obj_index].heading_rate  = FBK_ZERO_F;
   object_data[obj_index].vcs_vel_rel.x = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].vcs_vel_rel.y =
      (p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON)
         / (p_scw_calibration->k_scw_min_dynamic_lat_ttc + p_scw_calibration->k_scw_trailer_lat_ttc_extension)
      + EPSILON;
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold = FBK_ZERO_INT;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_1);
}

/*
 * Tests the SCW feature main algorithm for a critical object, low lateral ttc, zero width trailer attached and enabled.
 * \uts{CSCSA-204183} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_object_low_lat_ttc_trailer_attached_enabled_width_0)
{
   /** \arrange Set up a critical SCW situation. */
   uint8_t obj_index = 1u;

   p_scw_calibration->k_scw_f_enable_trailer_ttc_extension = FBK_TRUE;
   p_scw_calibration->k_scw_trailer_lat_ttc_extension      = 0.5f;
   p_scw_core_input->trailer.f_present                     = FBK_TRUE;
   p_scw_core_input->trailer.length                        = 4.0f;
   p_scw_core_input->trailer.width                         = 0.0f;

   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                    = 1.8f;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].length                 = 4.0f;
   object_data[obj_index].width                  = 1.8f;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.y     = -(0.5f * object_data[obj_index].width + p_scw_calibration->k_scw_min_dynamic_lat_distance
                                        + 0.5f * p_vehicle_data->host_width + EPSILON);
   object_data[obj_index].age           = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[obj_index].vcs_heading   = FBK_ZERO_F;
   object_data[obj_index].heading_rate  = FBK_ZERO_F;
   object_data[obj_index].vcs_vel_rel.x = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].vcs_vel_rel.y =
      (p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON)
         / (p_scw_calibration->k_scw_min_dynamic_lat_ttc + p_scw_calibration->k_scw_trailer_lat_ttc_extension)
      + EPSILON;
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold = FBK_ZERO_INT;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_1);
}

/*
 * Tests the SCW feature main algorithm for a critical object, low lateral ttc, zero length trailer attached and enabled.
 * \uts{CSCSA-204184} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_object_low_lat_ttc_trailer_attached_enabled_length_0)
{
   /** \arrange Set up a critical SCW situation. */
   uint8_t obj_index = 1u;

   p_scw_calibration->k_scw_f_enable_trailer_ttc_extension = FBK_TRUE;
   p_scw_calibration->k_scw_trailer_lat_ttc_extension      = 0.5f;
   p_scw_core_input->trailer.f_present                     = FBK_TRUE;
   p_scw_core_input->trailer.length                        = 0.0f;
   p_scw_core_input->trailer.width                         = 2.0f;

   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                    = 1.8f;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].length                 = 4.0f;
   object_data[obj_index].width                  = 1.8f;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.y     = -(0.5f * object_data[obj_index].width + p_scw_calibration->k_scw_min_dynamic_lat_distance
                                        + 0.5f * p_scw_core_input->trailer.width + EPSILON);
   object_data[obj_index].age           = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[obj_index].vcs_heading   = FBK_ZERO_F;
   object_data[obj_index].heading_rate  = FBK_ZERO_F;
   object_data[obj_index].vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] + EPSILON;
   object_data[obj_index].vcs_vel_rel.x = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].vcs_vel_rel.y =
      (p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON)
         / (p_scw_calibration->k_scw_min_dynamic_lat_ttc + p_scw_calibration->k_scw_trailer_lat_ttc_extension)
      + EPSILON;
   object_data[obj_index].speed = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + EPSILON;
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold = FBK_ZERO_INT;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_1);
}

/*
 * Tests the SCW feature main algorithm for a critical guardrail.
 * \uts{CSCSA-44464} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_guardrail)
{
   /** \arrange Set up a critical SCW situation. */
   p_vehicle_data->host_speed                                       = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                                       = 2.0f;
   p_vehicle_data->host_length                                      = 1.0f;
   p_scw_core_input->f_scw_enable                                   = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic                           = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail                         = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]                    = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated                         = FBK_FALSE;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]                   = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -2.0f;

   p_scw_calibration->k_scw_guardrail_cycles_in_zone_threshold = FBK_ZERO_INT;

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_1);
}

/*
 * Tests the SCW feature main algorithm for a critical guardrail, too low lateral ttc.
 * \uts{CSCSA-121124} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_guardrail_level_2_lat_ttc)
{
   /** \arrange Set up a critical SCW situation. */
   p_vehicle_data->host_speed                                       = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                                       = 2.0f;
   p_vehicle_data->host_length                                      = 1.0f;
   p_scw_core_input->f_scw_enable                                   = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic                           = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail                         = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]                    = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated                         = FBK_FALSE;
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT]             = FBK_ONE_UINT;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position =
      -(0.5f * p_vehicle_data->host_width + p_scw_calibration->k_scw_min_guardrail_lat_distance + EPSILON);
   pa_data.time_diff_to_last_cycle = 0.05f;
   p_scw_persistent->core_grail_lat_position[FBK_SIDE_LEFT][0] =
      p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position - 2.0f;
   p_scw_persistent->core_grail_lat_position[FBK_SIDE_LEFT][1] =
      p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position - 1.0f;

   p_scw_calibration->k_scw_guardrail_cycles_in_zone_threshold = FBK_ZERO_INT;

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that the alert level is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_ALERT_LEVEL_2);
}

/*
 * Tests the SCW feature main algorithm for a critical guardrail.
 * \uts{CSCSA-101390} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_critical_guardrail_and_guardrail_disabled)
{
   /** \arrange Set up a critical SCW situation. */
   p_vehicle_data->host_speed                                       = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                                       = 2.0f;
   p_vehicle_data->host_length                                      = 1.0f;
   p_scw_core_input->f_scw_enable                                   = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic                           = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail                         = FBK_FALSE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]                    = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated                         = FBK_FALSE;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -2.0f;

   p_scw_calibration->k_scw_guardrail_cycles_in_zone_threshold = FBK_ZERO_INT;

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests the SCW feature main algorithm for non critical guardrail.
 * \uts{CSCSA-101391} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_non_critical_guardrail_and_guardrail_enabled)
{
   /** \arrange Set up a critical SCW situation. */
   p_vehicle_data->host_speed                                       = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                                       = 2.0f;
   p_vehicle_data->host_length                                      = 1.0f;
   p_scw_core_input->f_scw_enable                                   = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic                           = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail                         = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]                    = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated                         = FBK_FALSE;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -10.0f;

   p_scw_calibration->k_scw_guardrail_cycles_in_zone_threshold = FBK_ZERO_INT;

   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests the SCW feature main algorithm for no critical objects.
 * \uts{CSCSA-101392} \sdd{SF-8045} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Algorithm__run_no_critical_objects)
{
   /** \arrange */
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_vehicle_data->host_width                    = 2.0f;
   p_vehicle_data->host_length                   = 1.0f;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail      = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   /* to cover more branches */
   p_scw_calibration->k_scw_f_enable_trailer_ttc_extension = FBK_TRUE;
   p_scw_core_input->trailer.f_present                     = FBK_TRUE;
   p_scw_core_input->trailer.length                        = 3.0f;
   p_scw_core_input->trailer.width                         = 2.0f;


   /** \action Call the main algorithm function */
   Scw_Algorithm(p_scw_core_output, p_scw_persistent, p_vehicle_data, p_scw_core_input, p_scw_calibration);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests the logic on how to find the most critical object index. Check for invalid current object index.
 * \uts{CSCSA-121125} \sdd{CSCSA-121169} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Most_Critical_Dyn_Obj__test_invalid_current_object)
{
   /** \arrange Set up SCW object indices. */
   uint8_t most_critical_obj_idx = PA_INVALID_OBJ_INDEX;
   uint8_t new_obj_idx           = 0;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, new_obj_idx);
   Scw_Critical_Object_T scw_critical_object = {
      1, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};

   /** \action Call the function to get the most critical obj index */
   Scw_Get_Most_Critical_Dyn_Obj(&scw_critical_object, &scw_object);
   most_critical_obj_idx = scw_critical_object.index;

   /** \assert Verify that the correct index is returned. */
   EXPECT_EQ(most_critical_obj_idx, new_obj_idx);
}

/*
 * Tests the logic on how to find the most critical object index. Check for valid current and new object where current object is
 * laterally closer. \uts{CSCSA-121126} \sdd{CSCSA-121169} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Most_Critical_Dyn_Obj__test_both_obj_valid_current_obj_closer)
{
   /** \arrange Set up SCW object indices. */
   uint8_t most_critical_obj_idx             = PA_INVALID_OBJ_INDEX;
   uint8_t current_obj_idx                   = 0;
   uint8_t new_obj_idx                       = 1;
   float32_T current_obj_lat_distance        = p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON;
   Scw_Critical_Object_T scw_critical_object = {current_obj_idx, FBK_ZERO_F, current_obj_lat_distance,
                                                FBK_ZERO_F,      FBK_ZERO_F, SCW_MAX_LATERAL_TTC,
                                                10.0f,           10.0f}; /* any SCW_ALERT_LEVEL_1 lat. distance and
                                                                   TTC */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, new_obj_idx);
   scw_object.extended_data.lateral_distance = current_obj_lat_distance + 0.1f;

   /** \action Call the function to get the most critical obj index */
   Scw_Get_Most_Critical_Dyn_Obj(&scw_critical_object, &scw_object);
   most_critical_obj_idx = scw_critical_object.index;

   /** \assert Verify that the correct index is returned. */
   EXPECT_EQ(most_critical_obj_idx, current_obj_idx);
}

/*
 * Tests the logic on how to find the most critical object index. Check for valid current and new object where new object is
 * laterally closer. \uts{CSCSA-121127} \sdd{CSCSA-121169} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Most_Critical_Dyn_Obj__test_both_obj_valid_new_obj_closer)
{
   /** \arrange Set up SCW object indices. */
   uint8_t most_critical_obj_idx             = PA_INVALID_OBJ_INDEX;
   uint8_t current_obj_idx                   = 0;
   uint8_t new_obj_idx                       = 1;
   float32_T current_obj_lat_distance        = p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON + 0.1f;
   Scw_Critical_Object_T scw_critical_object = {current_obj_idx, FBK_ZERO_F, current_obj_lat_distance,
                                                FBK_ZERO_F,      FBK_ZERO_F, SCW_MAX_LATERAL_TTC,
                                                10.0f,           10.0f}; /* any SCW_ALERT_LEVEL_1 lat. distance and
                                                                   TTC */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, new_obj_idx);
   scw_object.extended_data.lateral_distance = current_obj_lat_distance - 0.1f;

   /** \action Call the function to get the most critical obj index */
   Scw_Get_Most_Critical_Dyn_Obj(&scw_critical_object, &scw_object);
   most_critical_obj_idx = scw_critical_object.index;

   /** \assert Verify that the correct index is returned. */
   EXPECT_EQ(most_critical_obj_idx, new_obj_idx);
}

/*
 * Tests the logic on how to find the most critical object index. Check for valid current and new object where new object is
 * laterally as close as the current one, but is longitudinally closer. \uts{CSCSA-121128} \sdd{CSCSA-121169}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Most_Critical_Dyn_Obj__test_both_obj_valid_new_obj_closer_longitudinally)
{
   /** \arrange Set up SCW object indices. */
   uint8_t most_critical_obj_idx      = PA_INVALID_OBJ_INDEX;
   uint8_t current_obj_idx            = 0;
   uint8_t new_obj_idx                = 1;
   float32_T current_obj_lat_distance = p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON;
   /* any SCW_ALERT_LEVEL_1 lat. distance and TTC */
   Scw_Critical_Object_T scw_critical_object = {
      current_obj_idx, -2.0f, current_obj_lat_distance, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, new_obj_idx);
   scw_object.extended_data.lateral_distance = current_obj_lat_distance;
   scw_object.tracker_data.vcs_pos.x         = scw_critical_object.position_x + 0.1f;

   /** \action Call the function to get the most critical obj index */
   Scw_Get_Most_Critical_Dyn_Obj(&scw_critical_object, &scw_object);
   most_critical_obj_idx = scw_critical_object.index;

   /** \assert Verify that the correct index is returned. */
   EXPECT_EQ(most_critical_obj_idx, new_obj_idx);
}

/*
 * Tests the logic on how to find the most critical object index. Check for valid current and new object where new object is
 * laterally as close as the current one, but the current object is longitudinally closer. \uts{CSCSA-121129} \sdd{CSCSA-121169}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Most_Critical_Dyn_Obj__test_both_obj_valid_current_obj_closer_longitudinally)
{
   /** \arrange Set up SCW object indices. */
   uint8_t most_critical_obj_idx      = PA_INVALID_OBJ_INDEX;
   uint8_t current_obj_idx            = 0;
   uint8_t new_obj_idx                = 1;
   float32_T current_obj_lat_distance = p_scw_calibration->k_scw_min_dynamic_lat_distance + EPSILON;
   /* any SCW_ALERT_LEVEL_1 lat. distance and TTC */
   Scw_Critical_Object_T scw_critical_object = {
      current_obj_idx, -2.0f, current_obj_lat_distance, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, new_obj_idx);
   scw_object.extended_data.lateral_distance = current_obj_lat_distance;
   scw_object.tracker_data.vcs_pos.x         = scw_critical_object.position_x - 0.1f;

   /** \action Call the function to get the most critical obj index */
   Scw_Get_Most_Critical_Dyn_Obj(&scw_critical_object, &scw_object);
   most_critical_obj_idx = scw_critical_object.index;

   /** \assert Verify that the correct index is returned. */
   EXPECT_EQ(most_critical_obj_idx, current_obj_idx);
}

/*
 * Tests the logic that resets the critical object counter for object not in zone.
 * \uts{CSCSA-44468} \sdd{SF-8030} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Dynamic_Objs__reset_count_in_zone_if_not_in_zone)
{
   /** \arrange Set up peristent counter. */
   uint8_t obj_index                             = 1u;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                    = p_scw_calibration->k_scw_min_candidate_age + FBK_ONE_UINT;
   object_data[obj_index].vcs_pos.y              = -10.0f;
   object_data[obj_index].vcs_vel_rel.x          = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] + EPSILON;
   object_data[obj_index].speed                  = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + EPSILON;
   object_data[obj_index].existence_probability  = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   Scw_Critical_Object_T scw_critical_object     = {
          obj_index, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};

   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count = 3;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Dynamic_Objs(&scw_critical_object, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone,
                                 p_scw_calibration);

   /** \assert Verify that the counter is reset. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, FBK_ZERO_INT);
}

/*
 * Tests the logic that resets the critical object counter for object that is not valid.
 * \uts{CSCSA-101394} \sdd{SF-8030} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Dynamic_Objs__reset_count_in_zone_count_if_obj_not_valid)
{
   /** \arrange Set up peristent counter. */
   uint8_t obj_index                             = 1u;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                    = FBK_ZERO_UINT;
   object_data[obj_index].vcs_pos.y              = -2.0f;
   object_data[obj_index].vcs_vel_rel.x          = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] - EPSILON;
   object_data[obj_index].speed                  = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] - EPSILON;
   object_data[obj_index].existence_probability  = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   Scw_Critical_Object_T scw_critical_object     = {
          obj_index, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};

   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count = 3;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Dynamic_Objs(&scw_critical_object, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone,
                                 p_scw_calibration);

   /** \assert Verify that the counter is reset. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, FBK_ZERO_INT);
}

/*
 * Tests the logic that resets the critical object counter for object that is not relevant (rel. long. velocity too low).
 * \uts{CSCSA-101395} \sdd{SF-8030} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Dynamic_Objs__reset_count_in_zone_count_if_obj_not_relevant)
{
   /** \arrange Set up peristent counter. */
   uint8_t obj_index                             = 1u;
   p_vehicle_data->host_speed                    = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic        = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_NO_ALERT;
   p_scw_persistent->prev_feature_activated      = FBK_FALSE;
   object_data[obj_index].id                     = obj_index;
   object_data[obj_index].status                 = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                    = p_scw_calibration->k_scw_min_candidate_age + FBK_ONE_UINT;
   object_data[obj_index].vcs_pos.y              = -2.0f;
   object_data[obj_index].vcs_vel_rel.x          = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] - EPSILON;
   object_data[obj_index].speed                  = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + EPSILON;
   object_data[obj_index].existence_probability  = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   Scw_Critical_Object_T scw_critical_object     = {
          obj_index, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};

   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count = 3;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Dynamic_Objs(&scw_critical_object, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone,
                                 p_scw_calibration);

   /** \assert Verify that the counter is reset. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, FBK_ZERO_INT);
}

/*
 * Tests the logic that doesnt increase the critical object counter for object that is not mature.
 * \uts{CSCSA-101396} \sdd{SF-8030} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Dynamic_Objs__not_increase_count_in_zone_count_if_obj_not_mature)
{
   /** \arrange Set up peristent counter. */
   uint8_t expected_value                 = p_scw_calibration->k_scw_candidate_mature_cycles_in_zone_threshold + FBK_ONE_UINT;
   uint8_t obj_index                      = 1u;
   p_vehicle_data->host_speed             = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable         = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]  = SCW_NO_ALERT;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT] = SCW_OBJECT_TYPE_DYNAMIC;
   object_data[obj_index].id                      = obj_index;
   object_data[obj_index].status                  = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].age                     = p_scw_calibration->k_scw_min_candidate_age + FBK_ONE_UINT;
   object_data[obj_index].vcs_pos.y               = -2.0f;
   object_data[obj_index].vcs_vel_rel.x           = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   Scw_Critical_Object_T scw_critical_object    = {
         obj_index, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};
   p_scw_persistent->prev_feature_activated     = FBK_FALSE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT] = object_data[obj_index].id;

   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count = expected_value;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Dynamic_Objs(&scw_critical_object, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone,
                                 p_scw_calibration);

   /** \assert Verify that the counter is not increased. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, expected_value);
}

/*
 * Tests the logic that the critical object is not determined if the object is not mature in zone.
 * \uts{CSCSA-204185} \sdd{SF-8030} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Dynamic_Objs__obj_not_mature_in_zone)
{
   /** \arrange Set up peristent counter. */
   uint8_t obj_index                              = 1u;
   p_vehicle_data->host_speed                     = p_scw_calibration->k_scw_min_host_speed + EPSILON;
   p_scw_core_input->f_scw_enable                 = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic         = FBK_TRUE;
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]  = SCW_NO_ALERT;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT] = SCW_OBJECT_TYPE_DYNAMIC;
   object_data[obj_index].id                      = obj_index;
   object_data[obj_index].status                  = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                     = p_scw_calibration->k_scw_min_candidate_age + FBK_ONE_UINT;
   object_data[obj_index].vcs_pos.y               = -2.0f;
   object_data[obj_index].vcs_vel_rel.x           = 0.5f
                                          * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                             + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   object_data[obj_index].speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   object_data[obj_index].existence_probability = p_scw_calibration->k_scw_min_candidate_existence_probability + EPSILON;
   Scw_Critical_Object_T scw_critical_object    = {
         obj_index, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};
   p_scw_persistent->prev_feature_activated     = FBK_FALSE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT] = object_data[obj_index].id;

   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count = FBK_ZERO_UINT;
   Scw_Set_In_Range_Persistents(p_scw_persistent, obj_index);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Dynamic_Objs(&scw_critical_object, p_scw_persistent, p_scw_core_input, &scw_zone, &scw_hysteresis_zone,
                                 p_scw_calibration);

   /** \assert Verify that the counter is not increased. */
   EXPECT_EQ(scw_critical_object.lateral_distance, SCW_MAX_LATERAL_DISTANCE);
   EXPECT_EQ(scw_critical_object.lateral_ttc, SCW_MAX_LATERAL_TTC);
   EXPECT_EQ(scw_critical_object.ttle, 10.0f);
   EXPECT_EQ(scw_critical_object.ttp, 10.0f);
}

/*
 * Tests the logic that resets the critical object counter for object not in zone.
 * \uts{CSCSA-44470} \sdd{SF-8039} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Guardrail_Critical__reset_count_in_zone_grail_if_not_in_zone)
{
   /** \arrange Set up peristent counter. */
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT] = p_scw_calibration->k_scw_guardrail_cycles_in_zone_threshold + FBK_ONE_UINT;
   p_vehicle_data->host_width                           = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -10.0f;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Is_Guardrail_Critical(p_scw_persistent, p_scw_core_input, p_vehicle_data, &scw_zone, &scw_hysteresis_zone, FBK_SIDE_LEFT,
                             p_scw_calibration);

   /** \assert Verify that the counter is reset. */
   EXPECT_EQ(p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT], FBK_ZERO_INT);
}

/*
 * Checks that the count_in_zone_grail counter is saturated and does not overflow.
 * \uts{CSCSA-44512} \sdd{SF-8039} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Guardrail_Critical__saturate_count_in_zone_grail_counter)
{
   /** \arrange Set up peristent counter. */
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT]             = 255;
   p_vehicle_data->host_width                                       = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -2.0f;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   Scw_Is_Guardrail_Critical(p_scw_persistent, p_scw_core_input, p_vehicle_data, &scw_zone, &scw_hysteresis_zone, FBK_SIDE_LEFT,
                             p_scw_calibration);

   /** \assert Verify that the counter is saturated and does not overflow. */
   EXPECT_EQ(p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT], UINT8_MAX);
}

/*
 * Checks that the guardrail is not critical if count_in_zone_grail counter is below threshold.
 * \uts{CSCSA-101397} \sdd{SF-8039} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Guardrail_Critical__count_in_zone_grail_counter_low)
{
   /** \arrange Set up peristent counter. */
   boolean_T is_critical;
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT]             = FBK_ZERO_UINT;
   p_vehicle_data->host_width                                       = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -2.0f;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call the function to get check guardrail criticality. */
   is_critical = Scw_Is_Guardrail_Critical(p_scw_persistent, p_scw_core_input, p_vehicle_data, &scw_zone, &scw_hysteresis_zone,
                                           FBK_SIDE_LEFT, p_scw_calibration);

   /** \assert Verify that guardrail is not critical. */
   EXPECT_FALSE(is_critical);
}

/*
 * Tests the logic that sets the critical guardrail data.
 * \uts{CSCSA-135502} \sdd{CSCSA-125683} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Critical_Guardrails__guardrail_in_zone)
{
   /** \arrange Set up peristent counter. */
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT]             = 255;
   p_vehicle_data->host_width                                       = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail + EPSILON;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = -p_vehicle_data->host_width;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);
   Scw_Critical_Object_T scw_critical_guardrail = {
      FBK_ZERO_UINT, FBK_ZERO_F, SCW_MAX_LATERAL_DISTANCE, FBK_ZERO_F, FBK_ZERO_F, SCW_MAX_LATERAL_TTC, 10.0f, 10.0f};
   p_scw_persistent->core_grail_lat_position[FBK_SIDE_LEFT][0] =
      p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position - 2.0f;
   p_scw_persistent->core_grail_lat_position[FBK_SIDE_LEFT][1] =
      p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position - 1.0f;
   pa_data.time_diff_to_last_cycle = 1.0f;

   /** \action Call the function to get check guardrail criticality. */
   Scw_Get_Critical_Guardrails(&scw_critical_guardrail, p_scw_persistent, p_scw_core_input, p_vehicle_data, &scw_zone,
                               &scw_hysteresis_zone, p_scw_calibration);

   /** \assert Verify that the counter is reset. */
   EXPECT_FLOAT_EQ(scw_critical_guardrail.lateral_distance, 1.0f);
   EXPECT_FLOAT_EQ(scw_critical_guardrail.lateral_velocity, 1.0f);
   EXPECT_FLOAT_EQ(scw_critical_guardrail.lateral_ttc, 1.0f);
}

/**
 * Test that the initial SCW zone is filled directly from the dedicated calibration values of the zone points, if it is not
 * adjusted by the ego size. \uts{CSCSA-44471} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_not_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   uint8_t idx;
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size = 0;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);
   /** \assert Verify that zone is equal to calibration defined initial zone. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(scw_zone.points[idx].x, p_scw_calibration->k_scw_initial_zone_x[idx]);
      EXPECT_FLOAT_EQ(scw_zone.points[idx].y, p_scw_calibration->k_scw_initial_zone_y[idx]);
   }
   EXPECT_EQ(scw_zone.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that the initial SCW zone is filled correctly, if it is adjusted by the ego size.
 * \uts{CSCSA-44472} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   uint8_t idx;
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size = 1;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is equal to calibration defined initial zone adjusted by ego size. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(scw_zone.points[idx].y, p_scw_calibration->k_scw_initial_zone_y[idx] + 0.5f * p_vehicle_data->host_width);
   }
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_FRONT_LEFT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_FRONT_LEFT]);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_FRONT_RIGHT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_FRONT_RIGHT]);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT] - p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT] - p_vehicle_data->host_length);
   EXPECT_EQ(scw_zone.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that the SCW zone is extended properly if the Trailer is attached; not adjusted by the ego size.
 * \uts{CSCSA-101398} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_trailer_attached_and_not_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   float32_T expected_extension;
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_TRUE;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = 5.0f;
   p_scw_core_input->trailer.width                          = 2.0f;
   p_scw_core_input->trailer.angle                          = 0.0f;
   expected_extension = p_scw_core_input->trailer.length + p_scw_calibration->k_scw_trailer_zone_ext_safety_margin;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is extended by trailer length + calibratable margin. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT] - expected_extension);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT] - expected_extension);
}

/**
 * Test that the SCW zone is extended to max length if very long Trailer is attached; not adjusted by the ego size.
 * \uts{CSCSA-255251} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_max_length_if_long_trailer_attached_and_ego_size_independent)
{
   /** \arrange Set up calibration values and vehicle data. */
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_TRUE;
   p_scw_calibration->k_scw_max_zone_length                 = 10.0f;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = 25.0f;
   p_scw_core_input->trailer.width                          = 2.0f;
   p_scw_core_input->trailer.angle                          = 0.0f;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is extended by max zone length. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT] - p_scw_calibration->k_scw_max_zone_length);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x,
                   p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT] - p_scw_calibration->k_scw_max_zone_length);
}

/**
 * Test that the SCW zone is extended to max width if very wide Trailer is attached; not adjusted by the ego size.
 * \uts{CSCSA-255252} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_max_width_if_wide_trailer_attached_and_ego_size_independent)
{
   /** \arrange Set up calibration values and vehicle data. */
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_TRUE;
   p_scw_calibration->k_scw_max_zone_width                  = 5.0f;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = 5.0f;
   p_scw_core_input->trailer.width                          = 20.0f;
   p_scw_core_input->trailer.angle                          = 0.0f;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is extended by max zone width. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].y,
                   p_scw_calibration->k_scw_initial_zone_y[SCW_ZONE_REAR_LEFT] + p_scw_calibration->k_scw_max_zone_width);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_FRONT_RIGHT].y,
                   p_scw_calibration->k_scw_initial_zone_y[SCW_ZONE_FRONT_LEFT] + p_scw_calibration->k_scw_max_zone_width);
}

/**
 * Test that the SCW zone is not extended if the Trailer is attached but extension is disabled; not adjusted by the ego size.
 * \uts{CSCSA-101399} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_trailer_attached_extension_disabled_not_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_FALSE;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = 5.0f;
   p_scw_core_input->trailer.width                          = 2.0f;
   p_scw_core_input->trailer.angle                          = 0.0f;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is not extended. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT]);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT]);
}

/**
 * Test that the SCW zone is not extended if 0-length Trailer attached and extension is enabled; not adjusted by the ego size.
 * \uts{CSCSA-101400} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_0_length_trailer_attached_extension_enabled_not_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_TRUE;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = FBK_ZERO_F;
   p_scw_core_input->trailer.width                          = 2.0f;
   p_scw_core_input->trailer.angle                          = FBK_ZERO_F;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is not extended. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT]);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT]);
}

/**
 * Test that the SCW zone is not extended if 0-width Trailer attached and extension is enabled; not adjusted by the ego size.
 * \uts{CSCSA-101401} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_initial_zone_correctly_if_0_width_trailer_attached_extension_enabled_not_adjusted_by_ego_size)
{
   /** \arrange Set up calibration values and vehicle data. */
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size      = 0;
   p_scw_calibration->k_scw_f_enable_trailer_zone_extension = FBK_TRUE;
   p_scw_core_input->trailer.f_present                      = FBK_TRUE;
   p_scw_core_input->trailer.length                         = 5.0;
   p_scw_core_input->trailer.width                          = FBK_ZERO_F;
   p_scw_core_input->trailer.angle                          = FBK_ZERO_F;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that zone is not extended. */
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_RIGHT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_RIGHT]);
   EXPECT_FLOAT_EQ(scw_zone.points[SCW_ZONE_REAR_LEFT].x, p_scw_calibration->k_scw_initial_zone_x[SCW_ZONE_REAR_LEFT]);
}

/**
 * Test that the hysteresis SCW zone is filled correctly, if offset is defined in a valid way.
 * \uts{CSCSA-44473} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_hysteresis_zone_correctly_if_offset_is_valid)
{
   /** \arrange Set up calibration values and vehicle data. */
   uint8_t idx;
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size               = 0;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_FRONT_LEFT]   = 1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_FRONT_RIGHT]  = 1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_MIDDLE_LEFT]  = -1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_MIDDLE_RIGHT] = -1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_REAR_LEFT]    = -1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_REAR_RIGHT]   = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_FRONT_LEFT]   = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_FRONT_RIGHT]  = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_MIDDLE_LEFT]  = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_MIDDLE_RIGHT] = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_REAR_LEFT]    = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_REAR_RIGHT]   = 1.0f;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that hysteresis zone is equal to calibration defined initial zone plus offset. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(scw_hysteresis_zone.points[idx].x,
                      p_scw_calibration->k_scw_initial_zone_x[idx] + p_scw_calibration->k_scw_hys_zone_x_offset[idx]);
      EXPECT_FLOAT_EQ(scw_hysteresis_zone.points[idx].y,
                      p_scw_calibration->k_scw_initial_zone_y[idx] + p_scw_calibration->k_scw_hys_zone_y_offset[idx]);
   }
   EXPECT_EQ(scw_zone.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that the hysteresis SCW zone is filled correctly, if offset is invalid. In this case the hysteresis zone should be equal to
 * the initial zone. \uts{CSCSA-44474} \sdd{SF-8028} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Create_Zones__fills_hysteresis_zone_correctly_if_offset_is_invalid)
{
   /** \arrange Set up calibration values and vehicle data. */
   uint8_t idx;
   p_scw_calibration->k_scw_f_adjust_zones_to_ego_size               = 0;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_FRONT_LEFT]   = -1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_FRONT_RIGHT]  = -1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_MIDDLE_LEFT]  = 1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_MIDDLE_RIGHT] = 1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_REAR_LEFT]    = 1.0f;
   p_scw_calibration->k_scw_hys_zone_x_offset[SCW_ZONE_REAR_RIGHT]   = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_FRONT_LEFT]   = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_FRONT_RIGHT]  = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_MIDDLE_LEFT]  = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_MIDDLE_RIGHT] = -1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_REAR_LEFT]    = 1.0f;
   p_scw_calibration->k_scw_hys_zone_y_offset[SCW_ZONE_REAR_RIGHT]   = -1.0f;

   /** \action Call function that creates the zones. */
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \assert Verify that hysteresis zone is equal to calibration defined initial zone. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(scw_hysteresis_zone.points[idx].x, p_scw_calibration->k_scw_initial_zone_x[idx]);
      EXPECT_FLOAT_EQ(scw_hysteresis_zone.points[idx].y, p_scw_calibration->k_scw_initial_zone_y[idx]);
   }
   EXPECT_EQ(scw_zone.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that correct zone is used for object that was not relevant before and is on right side of ego.
 * \uts{CSCSA-44475} \sdd{SF-8027} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Entity_Specific_Zone__no_hysteresis_no_flip)
{
   /** \arrange Set up SCW object that was not relevant before and is on right side of ego. */
   uint8_t scw_approach_side       = FBK_SIDE_RIGHT;
   uint8_t count_of_entity_in_zone = 0;
   uint8_t idx;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call function Scw_Calculate_Entity_Specific_Zone to determine zone to use. */
   Scw_Calculate_Entity_Specific_Zone(&(res_foi), &scw_zone, &scw_hysteresis_zone, scw_approach_side, count_of_entity_in_zone);

   /** \assert Verify that standard zone is used. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[idx].x, scw_zone.points[idx].x);
      EXPECT_FLOAT_EQ(res_foi.points[idx].y, scw_zone.points[idx].y);
   }
   EXPECT_EQ(res_foi.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that correct zone is used for object that was not relevant before and is on left side of ego.
 * \uts{CSCSA-44476} \sdd{SF-8027} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Entity_Specific_Zone__no_hysteresis_zone_is_flipped_to_left)
{
   /** \arrange Set up SCW object that was not relevant before and is on left side of ego. */
   uint8_t scw_approach_side       = FBK_SIDE_LEFT;
   uint8_t count_of_entity_in_zone = 0;
   uint8_t idx;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call function Scw_Calculate_Entity_Specific_Zone to determine zone to use. */
   Scw_Calculate_Entity_Specific_Zone(&(res_foi), &scw_zone, &scw_hysteresis_zone, scw_approach_side, count_of_entity_in_zone);

   /** \assert Verify that flipped standard zone is used. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[idx].x, scw_zone.points[idx].x);
      EXPECT_FLOAT_EQ(res_foi.points[idx].y, -scw_zone.points[idx].y);
   }
   EXPECT_EQ(res_foi.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that correct zone is used for object that was relevant before and is on right side of ego.
 * \uts{CSCSA-44477} \sdd{SF-8027} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Entity_Specific_Zone__hysteresis_zone_is_used)
{
   /** \arrange Set up SCW object that was relevant before and is on right side of ego. */
   uint8_t scw_approach_side       = FBK_SIDE_RIGHT;
   uint8_t count_of_entity_in_zone = 1;
   uint8_t idx;
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call function Scw_Calculate_Entity_Specific_Zone to determine zone to use. */
   Scw_Calculate_Entity_Specific_Zone(&(res_foi), &scw_zone, &scw_hysteresis_zone, scw_approach_side, count_of_entity_in_zone);

   /** \assert Verify that hysteresis zone is used. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[idx].x, scw_hysteresis_zone.points[idx].x);
      EXPECT_FLOAT_EQ(res_foi.points[idx].y, scw_hysteresis_zone.points[idx].y);
   }
   EXPECT_EQ(res_foi.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that correct zone is used for object that was relevant before and is on left side of ego.
 * \uts{CSCSA-44478} \sdd{SF-8027} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Entity_Specific_Zone__hysteresis_zone_is_used_and_flipping_applied)
{
   /** \arrange Set up SCW object that was relevant before and is on left side of ego. */
   uint8_t scw_approach_side       = FBK_SIDE_LEFT;
   uint8_t count_of_entity_in_zone = 1;
   uint8_t idx;

   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   /** \action Call function Scw_Calculate_Entity_Specific_Zone to determine zone to use. */
   Scw_Calculate_Entity_Specific_Zone(&(res_foi), &scw_zone, &scw_hysteresis_zone, scw_approach_side, count_of_entity_in_zone);

   /** \assert Verify that flipped hysteresis zone is used. */
   for (idx = 0; idx < SCW_NUMBER_OF_ZONE_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[idx].x, scw_hysteresis_zone.points[idx].x);
      EXPECT_FLOAT_EQ(res_foi.points[idx].y, -scw_hysteresis_zone.points[idx].y);
   }
   EXPECT_EQ(res_foi.size, SCW_NUMBER_OF_ZONE_POINTS);
}

/**
 * Test that an object that is completely inside of the SCW zone is correctly classified.
 * \uts{CSCSA-44479} \sdd{SF-8034} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Zone__obj_is_completely_within_the_zone)
{
   /** \arrange Set up SCW object that is completely within zone. */
   boolean_T f_obj_in_zone;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.width       = 2.0f;
   Vector_2d_T obj_center_pos          = Vector_2d_Alg_Middle(&(scw_zone.points[0]), &(scw_zone.points[2]));
   scw_object.tracker_data.vcs_pos.x   = obj_center_pos.x;
   scw_object.tracker_data.vcs_pos.y   = obj_center_pos.y;
   scw_object.tracker_data.vcs_heading = 0.0f;

   /** \action Call Scw_Is_Dyn_Object_In_Zone to determine if object is in zone. */
   f_obj_in_zone = Scw_Is_Dyn_Object_In_Zone(&scw_object, &scw_zone, p_scw_core_input->p_pa_data->vehicle_data.host_length);

   /** \assert Verify that object is classified as in zone. */
   EXPECT_TRUE(f_obj_in_zone);
}

/**
 * Test that an object that is partially inside of the SCW zone is correctly classified.
 * \uts{CSCSA-44480} \sdd{SF-8034} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Zone__objects_left_side_placed_close_near_zone_border)
{
   /** \arrange Set up SCW object that is in zone close to zone border. */
   boolean_T f_obj_in_zone;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.width       = 2.0f;
   Vector_2d_T obj_center_pos          = Vector_2d_Alg_Middle(&(scw_zone.points[1]), &(scw_zone.points[0]));
   scw_object.tracker_data.vcs_pos.x   = obj_center_pos.x;
   scw_object.tracker_data.vcs_pos.y   = obj_center_pos.y + 0.5f * scw_object.tracker_data.width - EPSILON;
   scw_object.tracker_data.vcs_heading = 0.0f;

   /** \action Call Scw_Is_Dyn_Object_In_Zone to determine if object is in zone. */
   f_obj_in_zone = Scw_Is_Dyn_Object_In_Zone(&scw_object, &scw_zone, p_scw_core_input->p_pa_data->vehicle_data.host_length);

   /** \assert Verify that object is classified as in zone. */
   EXPECT_TRUE(f_obj_in_zone);
}


/**
 * Test that an object that is outside of the SCW zone is correctly classified.
 * \uts{CSCSA-44481} \sdd{SF-8034} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Zone__object_not_in_zone)
{
   /** \arrange Set up SCW object that is not in zone. */
   boolean_T f_obj_in_zone;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   Scw_Create_Zones(&scw_zone, &scw_hysteresis_zone, p_scw_core_input, p_scw_calibration, p_vehicle_data);

   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.width       = 2.0f;
   Vector_2d_T obj_center_pos          = Vector_2d_Alg_Middle(&(scw_zone.points[1]), &(scw_zone.points[0]));
   scw_object.tracker_data.vcs_pos.x   = obj_center_pos.x;
   scw_object.tracker_data.vcs_pos.y   = obj_center_pos.y + scw_object.tracker_data.width;
   scw_object.tracker_data.vcs_heading = 0.0f;

   /** \action Call Scw_Is_Dyn_Object_In_Zone to determine if object is in zone. */
   f_obj_in_zone = Scw_Is_Dyn_Object_In_Zone(&scw_object, &scw_zone, p_scw_core_input->p_pa_data->vehicle_data.host_length);

   /** \assert Verify that object is classified as not in zone. */
   EXPECT_FALSE(f_obj_in_zone);
}


/**
 * Test that an object with relative velocity below the allowed range is correctly classified, if previously invalid.
 * \uts{CSCSA-135505} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_low_rel_long_velocity_previously_invalid)
{
   /** \arrange Set up SCW object with relative velocity below allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] - EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as outside of the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with relative velocity below the allowed range is correctly classified, if previously valid.
 * \uts{CSCSA-135506} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_low_rel_long_velocity_previously_valid)
{
   /** \arrange Set up SCW object with relative velocity below allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_TRUE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x =
      p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] - p_scw_calibration->k_scw_candidate_velocity_hys - EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as outside of the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with relative velocity above the allowed range is correctly classified, if previously invalid.
 * \uts{CSCSA-135507} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_high_rel_long_velocity_previously_invalid)
{
   /** \arrange Set up SCW object with relative velocity above the allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX] + EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as outside of the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with relative velocity above the allowed range is correctly classified, if previously valid.
 * \uts{CSCSA-135508} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_high_rel_long_velocity_previously_valid)
{
   /** \arrange Set up SCW object with relative velocity above the allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_TRUE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x =
      p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX] + p_scw_calibration->k_scw_candidate_velocity_hys + EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as outside of the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object on the left with relative lateral velocity in allowed range is correctly classified.
 * \uts{CSCSA-121138} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__valid_rel_lat_velocity_previously_valid_L)
{
   /** \arrange Set up SCW object with relative lateral velocity in allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_pos.y = -3.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as in the allowed range. */
   EXPECT_TRUE(f_velocity_in_range);
}


/**
 * Test that an object on the right with relative lateral velocity in allowed range is correctly classified.
 * \uts{CSCSA-121139} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__valid_rel_lat_velocity_previously_valid_R)
{
   /** \arrange Set up SCW object with relative lateral velocity in allowed range. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_pos.y = 3.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as in the allowed range. */
   EXPECT_TRUE(f_velocity_in_range);
}


/**
 * Test that an object with speed in allowed range is correctly classified.
 * \uts{CSCSA-121144} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__valid_low_speed_previously_valid)
{
   /** \arrange Set up SCW object with speed below lower threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed         = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_TRUE(f_velocity_in_range);
}


/**
 * Test that an object with speed in allowed range is correctly classified.
 * \uts{CSCSA-121145} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__valid_high_speed_previously_valid)
{
   /** \arrange Set up SCW object with speed above higher threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed         = p_scw_calibration->k_scw_candidate_velocity[SCW_MAX] - EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_TRUE(f_velocity_in_range);
}


/**
 * Test that an object with speed below the allowed range is correctly classified.
 * \uts{CSCSA-121146} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_low_speed_previously_invalid)
{
   /** \arrange Set up SCW object with velocity below lower threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed         = p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] - EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with speed above the allowed range is correctly classified.
 * \uts{CSCSA-121147} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_high_speed_previously_invalid)
{
   /** \arrange Set up SCW object with velocity above higher threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_FALSE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed         = p_scw_calibration->k_scw_candidate_velocity[SCW_MAX] + EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with speed below the allowed range is correctly classified.
 * \uts{CSCSA-121148} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_low_speed_previously_valid)
{
   /** \arrange Set up SCW object with velocity below lower threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_calibration->k_scw_candidate_velocity_hys = 0.5f * p_scw_calibration->k_scw_candidate_velocity[SCW_MIN];

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] - p_scw_calibration->k_scw_candidate_velocity_hys - EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with speed above the allowed range is correctly classified.
 * \uts{CSCSA-121149} \sdd{SF-8037} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Velocity_Correct__invalid_high_speed_previously_valid)
{
   /** \arrange Set up SCW object with velocity above higher threshold. */
   boolean_T f_velocity_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_speed_range                  = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[idx].f_in_relative_long_velocity_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed         = p_scw_calibration->k_scw_candidate_velocity[SCW_MAX] + EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Velocity_Correct to determine if object velocity is in allowed range. */
   f_velocity_in_range = Scw_Is_Dyn_Object_Velocity_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that velocity is classified as not in the allowed range. */
   EXPECT_FALSE(f_velocity_in_range);
}


/**
 * Test that an object with heading in allowed range is correctly classified.
 * \uts{CSCSA-121150} \sdd{CSCSA-121175} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Heading_Correct__heading_in_range)
{
   /** \arrange Set up SCW object with heading in allowed range. */
   boolean_T f_heading_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_heading_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Heading_Correct to determine if object heading is in allowed range. */
   f_heading_in_range = Scw_Is_Dyn_Object_Heading_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that heading is classified as in the allowed range. */
   EXPECT_TRUE(f_heading_in_range);
}


/**
 * Test that an object with heading outside of allowed range is correctly classified.
 * \uts{CSCSA-121151} \sdd{CSCSA-121175} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Heading_Correct__heading_outside_of_range)
{
   /** \arrange Set up SCW object with heading outside of allowed range. */
   boolean_T f_heading_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_heading_range = FBK_FALSE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_heading = p_scw_calibration->k_scw_candidate_heading[SCW_MIN] - EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Heading_Correct to determine if object heading is in allowed range. */
   f_heading_in_range = Scw_Is_Dyn_Object_Heading_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that heading is classified as outside of the allowed range. */
   EXPECT_FALSE(f_heading_in_range);
}


/**
 * Test that an object with heading in allowed range is correctly classified if it was in range previously.
 * \uts{CSCSA-121152} \sdd{CSCSA-121175} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Heading_Correct__heading_in_range_previously_in_range)
{
   /** \arrange Set up SCW object with heading in allowed range. */
   boolean_T f_heading_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_heading_range = FBK_TRUE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);

   /** \action Call function Scw_Is_Dyn_Object_Heading_Correct to determine if object heading is in allowed range. */
   f_heading_in_range = Scw_Is_Dyn_Object_Heading_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that heading is classified as in the allowed range. */
   EXPECT_TRUE(f_heading_in_range);
}


/**
 * Test that an object with heading outside of allowed range is correctly classified if it was in range previously.
 * \uts{CSCSA-121153} \sdd{CSCSA-121175} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Heading_Correct__heading_outside_of_range_previously_in_range)
{
   /** \arrange Set up SCW object with heading outside of allowed range. */
   boolean_T f_heading_in_range;
   uint8_t idx = 3;

   p_scw_persistent->dyn_obj_data[idx].f_in_heading_range = FBK_TRUE;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_heading =
      p_scw_calibration->k_scw_candidate_heading[SCW_MIN] - p_scw_calibration->k_scw_candidate_heading_hys - EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Heading_Correct to determine if object heading is in allowed range. */
   f_heading_in_range = Scw_Is_Dyn_Object_Heading_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that heading is classified as outside of the allowed range. */
   EXPECT_FALSE(f_heading_in_range);
}


/**
 * Test that an object with yawrate in allowed range is correctly classified.
 * \uts{CSCSA-267750} \sdd{CSCSA-267748} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Yawrate_Correct__yawrate_in_range__previously_in_range__negative_yawrate)
{
   /** \arrange Set up SCW object with yawrate in allowed range. */
   boolean_T f_yawrate_in_range;
   uint8_t idx = 3;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.heading_rate = -0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /** \action Call function Scw_Is_Dyn_Object_Yawrate_Correct to determine if object yawrate is in allowed range. */
   f_yawrate_in_range = Scw_Is_Dyn_Object_Yawrate_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that yawrate is classified as in the allowed range. */
   EXPECT_TRUE(f_yawrate_in_range);
}


/**
 * Test that an object with yawrate out of allowed range is not relevant.
 * \uts{CSCSA-267751} \sdd{CSCSA-267748} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Yawrate_Correct__yawrate_out_of_range__previously_in_range__positive_yawrate)
{
   /** \arrange Set up SCW object with yawrate out of allowed range. */
   boolean_T f_yawrate_in_range;
   uint8_t idx = 3;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.heading_rate = p_scw_calibration->k_scw_candidate_yawrate + EPSILON;

   /** \action Call function Scw_Is_Dyn_Object_Yawrate_Correct to determine if object yawrate is out of allowed range. */
   f_yawrate_in_range = Scw_Is_Dyn_Object_Yawrate_Correct(&scw_object, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that yawrate is classified as out of the allowed range. */
   EXPECT_FALSE(f_yawrate_in_range);
}


/**
 * Test that an object that is on the left guardrail is classified as in conflict with the environment.
 * \uts{CSCSA-44488} \sdd{SF-8033} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Environment_Conflict__is_TRUE_when_object_is_on_left_guardrail)
{
   /** \arrange Set up a SCW object that is on the left guardrail. */
   boolean_T f_is_in_conflict;
   uint8_t side = FBK_SIDE_LEFT;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;

   p_scw_core_input->guardrail_data[side].radar.lateral_position = -4.0;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 1.0f;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y = -3.0f;
   scw_object.tracker_data.width     = 2.1f;

   /** \action Call function Scw_Is_Dyn_Object_In_Environment_Conflict to determine if object is in conflict with environment. */
   f_is_in_conflict = Scw_Is_Dyn_Object_In_Environment_Conflict(&scw_object, p_scw_core_input, side, p_scw_calibration);

   /** \assert Verify that object is classified as in conflict with environment. */
   EXPECT_TRUE(f_is_in_conflict);
}

/**
 * Test that an object that is ahead of the left guardrail is classified as not in conflict with the environment.
 * \uts{CSCSA-44489} \sdd{SF-8033} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Environment_Conflict__is_FALSE_when_object_is_ahead_of_left_guardrail)
{
   /** \arrange Set up a SCW object that is ahead of the left guardrail. */
   boolean_T f_is_in_conflict;
   uint8_t side = FBK_SIDE_LEFT;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;

   p_scw_core_input->guardrail_data[side].radar.lateral_position = -4.0;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 1.0f;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y = -3.0f;
   scw_object.tracker_data.width     = 1.9f;

   /** \action Call function Scw_Is_Dyn_Object_In_Environment_Conflict to determine if object is in conflict with environment. */
   f_is_in_conflict = Scw_Is_Dyn_Object_In_Environment_Conflict(&scw_object, p_scw_core_input, side, p_scw_calibration);

   /** \assert Verify that object is classified as not in conflict with environment. */
   EXPECT_FALSE(f_is_in_conflict);
}

/**
 * Test that an object that is on the right guardrail is classified as in conflict with the environment.
 * \uts{CSCSA-44490} \sdd{SF-8033} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Environment_Conflict__is_TRUE_when_object_is_on_right_guardrail)
{
   /** \arrange Set up a SCW object that is on the right guardrail. */
   boolean_T f_is_in_conflict;
   uint8_t side = FBK_SIDE_RIGHT;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;

   p_scw_core_input->guardrail_data[side].radar.lateral_position = 4.0;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 1.0f;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y = 3.0f;
   scw_object.tracker_data.width     = 2.1f;

   /** \action Call function Scw_Is_Dyn_Object_In_Environment_Conflict to determine if object is in conflict with environment. */
   f_is_in_conflict = Scw_Is_Dyn_Object_In_Environment_Conflict(&scw_object, p_scw_core_input, side, p_scw_calibration);

   /** \assert Verify that object is classified as in conflict with environment. */
   EXPECT_TRUE(f_is_in_conflict);
}

/**
 * Test that an object that is ahead of the right guardrail is classified as not in conflict with the environment.
 * \uts{CSCSA-44491} \sdd{SF-8033} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_In_Environment_Conflict__is_FALSE_when_object_is_ahead_of_right_guardrail)
{
   /** \arrange Set up a SCW object that is ahead of the right guardrail. */
   boolean_T f_is_in_conflict;
   uint8_t side = FBK_SIDE_RIGHT;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;

   p_scw_core_input->guardrail_data[side].radar.lateral_position = 4.0;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 1.0f;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y = 3.0f;
   scw_object.tracker_data.width     = 1.9f;

   /** \action Call function Scw_Is_Dyn_Object_In_Environment_Conflict to determine if object is in conflict with environment. */
   f_is_in_conflict = Scw_Is_Dyn_Object_In_Environment_Conflict(&scw_object, p_scw_core_input, side, p_scw_calibration);

   /** \assert Verify that object is classified as not in conflict with environment. */
   EXPECT_FALSE(f_is_in_conflict);
}

/**
 * Call function to evaluate if an object is relevant with an relevant object. Verify that the object is actually classified as
 * relevant. \uts{CSCSA-44492} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__is_TRUE_if_all_conditions_are_fulfilled)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_LEFT;
   uint8_t idx  = 3u;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.5f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[side].radar.lateral_position =
      scw_object.tracker_data.vcs_pos.y - (0.5f * scw_object.tracker_data.width) - EPSILON;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is classified as relevant for SCW. */
   EXPECT_TRUE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is relevant with an object where heading is below lower threshold. Verify that the object
 * is not classified as relevant. \uts{CSCSA-44493} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__is_FALSE_if_heading_is_below_lower_threshold)
{
   /** \arrange Set up tracker data and object such that object is relevant for SCW, except for heading being below lower
    * threshold. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_RIGHT;
   uint8_t idx  = 3u;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.0f;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);
   p_scw_calibration->k_scw_candidate_heading[SCW_MIN] = EPSILON;

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y     = 3.0f;
   scw_object.tracker_data.vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] + EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading  = p_scw_calibration->k_scw_candidate_heading[SCW_MIN] - EPSILON;
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is not classified as relevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is relevant with an object where heading is above upper threshold. Verify that the object
 * is not classified as relevant. \uts{CSCSA-44494} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__is_FALSE_if_heading_is_above_upper_threshold)
{
   /** \arrange Set up tracker data and object such that object is relevant for SCW, except for heading being above upper
    * threshold. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_LEFT;
   uint8_t idx  = 3u;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.0f;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] + EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading  = -1.0f * (p_scw_calibration->k_scw_candidate_heading[SCW_MAX] + EPSILON);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is not classified as relevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is irrelevant with incorrect yawrate.
 * \uts{CSCSA-267752} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__is_false_if_yawrate_is_above_the_threshold)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW, except yawrate. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_RIGHT;
   uint8_t idx  = 3u;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.0f;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y     = 3.0f;
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = p_scw_calibration->k_scw_candidate_yawrate + EPSILON;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is not classified as relevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is irrelevant with incorrect relative velocity.
 * \uts{CSCSA-101404} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__is_false_if_rel_velocity_is_incorrect)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW, except relative velocity. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_RIGHT;
   uint8_t idx  = 3u;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.0f;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, 0);
   scw_object.tracker_data.vcs_pos.y     = 3.0f;
   scw_object.tracker_data.vcs_vel_rel.x = p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN] - EPSILON;
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is not classified as relevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is irrelevant with incorrect existence probability.
 * \uts{CSCSA-204186} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__incorrect_existence_probability)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW, except existence probability. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_LEFT;
   uint8_t idx  = 3u;

   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.confidence = 0.0f;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   scw_object.tracker_data.existence_probability = 0.5f * p_scw_calibration->k_scw_min_candidate_existence_probability;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is not classified as relevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is irrelevant when it's overlapping the guardrail.
 * \uts{CSCSA-204187} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__environment_conflict_left)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_LEFT;
   uint8_t idx  = 3u;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /* add the guardrail overlapped by the object */
   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail       = 0.5f;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = scw_object.tracker_data.vcs_pos.y;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is classified as irrelevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is irrelevant when it's overlapping the guardrail.
 * \uts{CSCSA-204188} \sdd{SF-8035} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Relevant__environment_conflict_right)
{
   /** \arrange Set up tracker data and SCW object such that object is relevant for SCW. */
   boolean_T f_obj_relevant;
   uint8_t side = FBK_SIDE_RIGHT;
   uint8_t idx  = 3u;

   Scw_Set_In_Range_Persistents(p_scw_persistent, idx);

   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, idx);
   scw_object.tracker_data.vcs_pos.y     = 3.0f;
   scw_object.tracker_data.vcs_vel_rel.x = 0.5f
                                           * (p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MIN]
                                              + p_scw_calibration->k_scw_candidate_relative_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_vel_rel.y = 0.0f;
   scw_object.tracker_data.speed =
      0.5f * (p_scw_calibration->k_scw_candidate_velocity[SCW_MIN] + p_scw_calibration->k_scw_candidate_velocity[SCW_MAX]);
   scw_object.tracker_data.vcs_heading =
      0.5f * (p_scw_calibration->k_scw_candidate_heading[SCW_MIN] + p_scw_calibration->k_scw_candidate_heading[SCW_MAX]);
   scw_object.tracker_data.heading_rate = 0.5f * p_scw_calibration->k_scw_candidate_yawrate;

   /* add the guardrail overlapped by the object */
   p_scw_calibration->k_scw_min_exist_prob_radar_guardrail       = 0.5f;
   p_scw_core_input->guardrail_data[side].radar.confidence       = 0.99f;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = scw_object.tracker_data.vcs_pos.y;
   p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_VALID;

   /** \action Call function to evaluate if object is relevant. */
   f_obj_relevant = Scw_Is_Dyn_Object_Relevant(&scw_object, p_scw_core_input, p_scw_calibration, p_scw_persistent, side);

   /** \assert Verify that the object is classified as irrelevant for SCW. */
   EXPECT_FALSE(f_obj_relevant);
}

/**
 * Call function to evaluate if an object is valid with a valid mature object. Verify that the object is actually classified as
 * valid. \uts{CSCSA-44495} \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_TRUE_if_all_conditions_are_fulfilled_with_status_mature)
{
   /** \arrange Set up tracker data such that object is valid for SCW with status mature. */
   boolean_T f_obj_valid;
   uint8_t idx = 3u;

   object_data[idx].status = PA_OBJ_STATUS_MATURE;
   object_data[idx].age    = p_scw_calibration->k_scw_min_candidate_age + 1u;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is classified as valid for SCW. */
   EXPECT_TRUE(f_obj_valid);
}

/**
 * Call function to evaluate if an object is valid with a valid coasted object with previous result on left side. Verify that the
 * object is actually classified as valid. \uts{CSCSA-44496} \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_TRUE_if_all_conditions_are_fulfilled_with_status_coasted_and_prev_alert_left)
{
   /** \arrange Set up tracker data such that object is valid for SCW with status coasted because of a previous result on left
    * side. */
   boolean_T f_obj_valid;
   uint8_t idx = 3u;

   object_data[idx].id                             = idx;
   object_data[idx].status                         = PA_OBJ_STATUS_COASTED;
   object_data[idx].age                            = p_scw_calibration->k_scw_min_candidate_age + 1u;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]    = object_data[idx].id;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_persistent->prev_obj_id[FBK_SIDE_RIGHT]   = FBK_ZERO_INT;
   p_scw_persistent->prev_obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_NONE;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is classified as valid for SCW. */
   EXPECT_TRUE(f_obj_valid);
}

/**
 * Call function to evaluate if an object is valid with a valid coasted object with previous result on right side. Verify that the
 * object is actually classified as valid. \uts{CSCSA-44497} \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_TRUE_if_all_conditions_are_fulfilled_with_status_coasted_and_prev_alert_right)
{
   /** \arrange Set up tracker data such that object is valid for SCW with status coasted because of a previous result on right
    * side. */
   boolean_T f_obj_valid;
   uint8_t idx = 3u;

   object_data[idx].id                             = idx;
   object_data[idx].status                         = PA_OBJ_STATUS_COASTED;
   object_data[idx].age                            = p_scw_calibration->k_scw_min_candidate_age + 1u;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]    = FBK_ZERO_INT;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_NONE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_RIGHT]   = object_data[idx].id;
   p_scw_persistent->prev_obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_DYNAMIC;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is classified as valid for SCW. */
   EXPECT_TRUE(f_obj_valid);
}

/**
 * Call function to evaluate if an object is valid with an object that is coasted and has not triggered an alert before, which is
 * identified by non-matching id to previous alert. Verify that the object is not classified as valid. \uts{CSCSA-44498}
 * \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_FALSE_if_obj_is_coasted_without_alert_in_mature_status_because_ids_not_matching)
{
   /** \arrange Set up tracker data such that object is valid for SCW, except for status coasted without alert in mature status,
    * which is identified by non-matching id to previous alert. */
   boolean_T f_obj_valid;
   uint8_t idx = 3u;

   object_data[idx].status                         = PA_OBJ_STATUS_COASTED;
   object_data[idx].age                            = p_scw_calibration->k_scw_min_candidate_age + 1u;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]    = FBK_ZERO_INT;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_NONE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_RIGHT]   = 1u;
   p_scw_persistent->prev_obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_GUARDRAIL;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is not classified as valid for SCW. */
   EXPECT_FALSE(f_obj_valid);
}

/**
 * Call function to evaluate if an object is valid with an object that is coasted and has not triggered an alert before, which is
 * identified by non-matching object type to previous alert. Verify that the object is not classified as valid. \uts{CSCSA-44499}
 * \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_FALSE_if_obj_is_coasted_without_alert_in_mature_status_because_obj_type_not_matching)
{
   /** \arrange Set up tracker data such that object is valid for SCW, except for status coasted without alert in mature status,
    * which is identified by non-matching object type to previous alert. */
   boolean_T f_obj_valid;
   uint8_t idx = 1u;

   object_data[idx].status                         = PA_OBJ_STATUS_COASTED;
   object_data[idx].age                            = p_scw_calibration->k_scw_min_candidate_age + 1u;
   object_data[idx].id                             = 1u;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]    = 1u;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_persistent->prev_obj_id[FBK_SIDE_RIGHT]   = 1u;
   p_scw_persistent->prev_obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_GUARDRAIL;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is not classified as valid for SCW. */
   EXPECT_FALSE(f_obj_valid);
}

/**
 * Call function to evaluate if an object is valid with an object that is too young. Verify that the object is not classified as
 * valid. \uts{CSCSA-44500} \sdd{SF-8036} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Dyn_Object_Valid__is_FALSE_if_obj_is_too_young)
{
   /** \arrange Set up tracker data such that object is valid for SCW, except for being too young. */
   boolean_T f_obj_valid;
   uint8_t idx                                = 3u;
   p_scw_calibration->k_scw_min_candidate_age = 4u;

   object_data[idx].status = PA_OBJ_STATUS_MATURE;
   object_data[idx].age    = p_scw_calibration->k_scw_min_candidate_age - 1u;

   /** \action Call function to evaluate if object is valid. */
   f_obj_valid = Scw_Is_Dyn_Object_Valid(p_scw_core_input, idx, p_scw_calibration, p_scw_persistent);

   /** \assert Verify that the object is not classified as valid for SCW. */
   EXPECT_FALSE(f_obj_valid);
}

/**
 * Test that the reset of core output for a specified side works properly.
 * \uts{CSCSA-44501} \sdd{SF-8042} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Core_Output_For_Side__works_properly)
{
   /** \arrange Set up core output data for left side with non-default values. */
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]          = SCW_ALERT_LEVEL_1;
   p_scw_core_output->obj_id[FBK_SIDE_LEFT]               = 4u;
   p_scw_core_output->obj_index[FBK_SIDE_LEFT]            = 4u;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]             = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_lateral_distance[FBK_SIDE_LEFT] = 2.0f;
   p_scw_core_output->obj_lateral_ttc[FBK_SIDE_LEFT]      = 1.0f;

   /** \action Call function to reset core output for left side. */
   Scw_Reset_Core_Output_For_Side(p_scw_core_output, p_scw_calibration, FBK_SIDE_LEFT);

   /** \assert Verify that core output data for left side is reseted to default values. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
   EXPECT_EQ(p_scw_core_output->obj_id[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_unique_id[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(p_scw_core_output->obj_type[FBK_SIDE_LEFT], SCW_OBJECT_TYPE_NONE);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_distance[FBK_SIDE_LEFT], p_scw_calibration->k_scw_lateral_distance_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_ttc[FBK_SIDE_LEFT], p_scw_calibration->k_scw_lateral_ttc_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttp[FBK_SIDE_LEFT], p_scw_calibration->k_scw_ttp_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttle[FBK_SIDE_LEFT], p_scw_calibration->k_scw_ttle_default);
}

/**
 * Test that the reset of core output works properly.
 * \uts{CSCSA-44502} \sdd{SF-8041} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Core_Output__works_properly)
{
   /** \arrange Set up core output data with non-default values. */
   p_scw_core_output->alert_level[FBK_SIDE_LEFT]           = SCW_ALERT_LEVEL_1;
   p_scw_core_output->obj_id[FBK_SIDE_LEFT]                = 5u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_LEFT]         = 5u;
   p_scw_core_output->obj_index[FBK_SIDE_LEFT]             = 4u;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]              = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_lateral_distance[FBK_SIDE_LEFT]  = 2.0f;
   p_scw_core_output->obj_lateral_ttc[FBK_SIDE_LEFT]       = 1.0f;
   p_scw_core_output->alert_level[FBK_SIDE_RIGHT]          = SCW_ALERT_LEVEL_1;
   p_scw_core_output->obj_id[FBK_SIDE_RIGHT]               = 22u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT]        = 22u;
   p_scw_core_output->obj_index[FBK_SIDE_RIGHT]            = 21u;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]             = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_lateral_distance[FBK_SIDE_RIGHT] = 2.0f;
   p_scw_core_output->obj_lateral_ttc[FBK_SIDE_RIGHT]      = 1.0f;

   /** \action Call function to reset core output. */
   Scw_Reset_Core_Output(p_scw_core_output, p_scw_calibration);

   /** \assert Verify that core output data is reseted to default values. */
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
   EXPECT_EQ(p_scw_core_output->obj_id[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_unique_id[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(p_scw_core_output->obj_type[FBK_SIDE_LEFT], SCW_OBJECT_TYPE_NONE);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_distance[FBK_SIDE_LEFT], p_scw_calibration->k_scw_lateral_distance_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_ttc[FBK_SIDE_LEFT], p_scw_calibration->k_scw_lateral_ttc_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttp[FBK_SIDE_LEFT], p_scw_calibration->k_scw_ttp_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttle[FBK_SIDE_LEFT], p_scw_calibration->k_scw_ttle_default);
   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_RIGHT], SCW_NO_ALERT);
   EXPECT_EQ(p_scw_core_output->obj_id[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(p_scw_core_output->obj_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(p_scw_core_output->obj_type[FBK_SIDE_RIGHT], SCW_OBJECT_TYPE_NONE);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_distance[FBK_SIDE_RIGHT], p_scw_calibration->k_scw_lateral_distance_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_lateral_ttc[FBK_SIDE_RIGHT], p_scw_calibration->k_scw_lateral_ttc_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttp[FBK_SIDE_RIGHT], p_scw_calibration->k_scw_ttp_default);
   EXPECT_FLOAT_EQ(p_scw_core_output->obj_ttle[FBK_SIDE_RIGHT], p_scw_calibration->k_scw_ttle_default);
}

/**
 * Test that the reset of object scw_persistent data works properly.
 * \uts{CSCSA-44503} \sdd{SF-8043} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Object_Persistent_Data__works_properly)
{
   /** \arrange Set up object scw_persistent data for with non-default values. */
   uint8_t obj_index                                               = 6u;
   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count  = 4u;
   p_scw_persistent->dyn_obj_data[obj_index].f_relevant_last_cycle = FBK_TRUE;

   /** \action Call function to reset object scw_persistent data. */
   Scw_Reset_Object_Persistent_Data(p_scw_persistent, obj_index);

   /** \assert Verify that object scw_persistent data is reseted to default values. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, FBK_ZERO_UINT);
   EXPECT_FALSE(p_scw_persistent->dyn_obj_data[obj_index].f_relevant_last_cycle);
}

/**
 * Test that the reset of "prev_was" persistent components works correctly.
 * \uts{CSCSA-305555} \sdd{CSCSA-305551} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Prev_Was_for_Side__not_reset_if_incorrect_side)
{
   /** \arrange Set up persistent data for with non-default values. */
   p_scw_persistent->prev_was_below_min_lat_ttc[FBK_SIDE_LEFT]      = true;
   p_scw_persistent->prev_was_below_max_lat_ttc[FBK_SIDE_LEFT]      = true;
   p_scw_persistent->prev_was_below_min_lat_distance[FBK_SIDE_LEFT] = true;
   p_scw_persistent->prev_was_below_max_lat_distance[FBK_SIDE_LEFT] = true;

   /** \action Call function to reset scw_persistent data. */
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, FBK_NUMBER_OF_SIDES + 1u);

   /** \assert Verify that "prev_was" persistent components are not reset. */
   EXPECT_TRUE(p_scw_persistent->prev_was_below_min_lat_ttc[FBK_SIDE_LEFT]);
   EXPECT_TRUE(p_scw_persistent->prev_was_below_max_lat_ttc[FBK_SIDE_LEFT]);
   EXPECT_TRUE(p_scw_persistent->prev_was_below_min_lat_distance[FBK_SIDE_LEFT]);
   EXPECT_TRUE(p_scw_persistent->prev_was_below_max_lat_distance[FBK_SIDE_LEFT]);
}

/**
 * Test that the reset of scw_persistent data works properly.
 * \uts{CSCSA-44504} \sdd{SF-8044} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Persistent_Data__works_properly)
{
   /** \arrange Set up persistent data for with non-default values. */
   uint8_t obj_index                                               = 6u;
   p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count  = 4u;
   p_scw_persistent->dyn_obj_data[obj_index].f_relevant_last_cycle = FBK_TRUE;
   p_scw_persistent->prev_feature_activated                        = FBK_TRUE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]                    = 4u;
   p_scw_persistent->prev_obj_unique_id[FBK_SIDE_LEFT]             = 4u;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]                  = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT]            = 4u;

   /** \action Call function to reset scw_persistent data. */
   Scw_Reset_Persistent_Data(p_scw_persistent);

   /** \assert Verify that persistent data is reseted to default values. */
   EXPECT_EQ(p_scw_persistent->dyn_obj_data[obj_index].mature_in_zone_count, FBK_ZERO_INT);
   EXPECT_FALSE(p_scw_persistent->dyn_obj_data[obj_index].f_relevant_last_cycle);
   EXPECT_FALSE(p_scw_persistent->prev_feature_activated);
   EXPECT_EQ(p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(p_scw_persistent->prev_obj_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT], SCW_OBJECT_TYPE_NONE);
   EXPECT_EQ(p_scw_persistent->count_in_zone_grail[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Test that the feature is active if the host speed is above the threshold.
 * \uts{CSCSA-44505} \sdd{SF-8038} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Feature_Activated__returns_true_if_host_speed_above_threshold)
{
   /** \arrange Set up host speed at threshold for feature activation. */
   boolean_T f_feature_active               = FBK_FALSE;
   p_scw_persistent->prev_feature_activated = FBK_FALSE;
   p_vehicle_data->host_speed               = p_scw_calibration->k_scw_min_host_speed;

   /** \action Call function to evalute if SCW should be active. */
   f_feature_active = Scw_Is_Feature_Activated(p_scw_persistent, p_vehicle_data, p_scw_calibration);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_feature_active);
}

/**
 * Test that the feature is active if the host speed is above the threshold with active hysteresis.
 * \uts{CSCSA-44506} \sdd{SF-8038} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Feature_Activated__returns_true_if_host_speed_above_threshold_with_hysteresis)
{
   /** \arrange Set up host speed at threshold plus hysteresis for feature activation. */
   boolean_T f_feature_active               = FBK_FALSE;
   p_scw_persistent->prev_feature_activated = FBK_TRUE;
   p_vehicle_data->host_speed = p_scw_calibration->k_scw_min_host_speed + p_scw_calibration->k_scw_min_host_speed_hys;

   /** \action Call function to evalute if SCW should be active. */
   f_feature_active = Scw_Is_Feature_Activated(p_scw_persistent, p_vehicle_data, p_scw_calibration);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_feature_active);
}

/**
 * Test that the feature is not active if the host speed is below threshold.
 * \uts{CSCSA-44507} \sdd{SF-8038} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Feature_Activated__returns_false_if_host_speed_below_threshold)
{
   /** \arrange Set up host speed below threshold. */
   boolean_T f_feature_active               = FBK_FALSE;
   p_scw_persistent->prev_feature_activated = FBK_FALSE;
   p_scw_calibration->k_scw_min_host_speed  = 3.0f;
   p_vehicle_data->host_speed               = p_scw_calibration->k_scw_min_host_speed - EPSILON;

   /** \action Call function to evalute if SCW should be active. */
   f_feature_active = Scw_Is_Feature_Activated(p_scw_persistent, p_vehicle_data, p_scw_calibration);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(f_feature_active);
}

/**
 * Test that the initialization of the core input works properly.
 * \uts{CSCSA-44508} \sdd{SF-8032} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Init_Core_Input__works_properly)
{
   /** \arrange Set up core input with non-default values. */
   p_scw_core_input->f_scw_enable           = FBK_TRUE;
   p_scw_core_input->f_scw_enable_dynamic   = FBK_TRUE;
   p_scw_core_input->f_scw_enable_guardrail = FBK_TRUE;

   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position  = 3.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence        = 5.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type              = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.confidence       = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.type             = SCW_GUARDRAIL_VALID;

   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position  = 2.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence        = 4.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.type              = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position = 1.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence       = 3.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.type             = SCW_GUARDRAIL_VALID;

   /** \action Call function to initialize core input. */
   Scw_Init_Core_Input(p_scw_core_input);

   /** \assert Verify that core input is set to default values. */
   EXPECT_FALSE(p_scw_core_input->f_scw_enable);
   EXPECT_FALSE(p_scw_core_input->f_scw_enable_dynamic);
   EXPECT_FALSE(p_scw_core_input->f_scw_enable_guardrail);

   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, 0.0f);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence, 0.0f);
   EXPECT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type, SCW_GUARDRAIL_INVALID);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position, 0.0f);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.confidence, 0.0f);
   EXPECT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].radar.type, SCW_GUARDRAIL_INVALID);

   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, 0.0f);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.confidence, 0.0f);
   EXPECT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].camera.type, SCW_GUARDRAIL_INVALID);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, 0.0f);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.confidence, 0.0f);
   EXPECT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_RIGHT].camera.type, SCW_GUARDRAIL_INVALID);
}

/**
 * Test that the reset of SCW works properly.
 * \uts{CSCSA-44510} \sdd{SF-8051} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset__works_properly)
{
   /** \arrange Set up core input, core output and scw_persistent data with some non-default values. */
   p_scw_core_input->f_scw_enable                                         = FBK_TRUE;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = 3.0f;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence       = 5.0f;

   p_scw_core_output->alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;
   p_scw_core_output->obj_id[FBK_SIDE_LEFT]      = 5u;

   p_scw_persistent->prev_feature_activated            = FBK_TRUE;
   p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT]        = 4u;
   p_scw_persistent->prev_obj_unique_id[FBK_SIDE_LEFT] = 4u;
   p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT]      = SCW_OBJECT_TYPE_DYNAMIC;

   /** \action Call function to reset SCW. */
   Scw_Reset(p_scw_core_input, p_scw_core_output, p_scw_persistent, p_scw_calibration);

   /** \assert Verify that core input, core output and scw_persistent data is set to default values. */
   EXPECT_FALSE(p_scw_core_input->f_scw_enable);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, 0.0f);
   EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence, 0.0f);

   EXPECT_EQ(p_scw_core_output->alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
   EXPECT_EQ(p_scw_core_output->obj_id[FBK_SIDE_LEFT], FBK_ZERO_UINT);

   EXPECT_FALSE(p_scw_persistent->prev_feature_activated);
   EXPECT_EQ(p_scw_persistent->prev_obj_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(p_scw_persistent->prev_obj_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(p_scw_persistent->prev_obj_type[FBK_SIDE_LEFT], SCW_OBJECT_TYPE_NONE);
}

/**
 * Test that a valid guardrail is correctly classified by corresponding function.
 * \uts{CSCSA-44511} \sdd{SF-8040} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Valid_Guardrail__returns_true_for_valid_guardrail)
{
   /** \arrange Set up valid guardrail on left side. */
   boolean_T f_guardrail_valid;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail;

   /** \action Call function Scw_Is_Valid_Guardrail to evaluate if guardrail is valid. */
   f_guardrail_valid = Scw_Is_Valid_Guardrail(p_scw_core_input, FBK_SIDE_LEFT, p_scw_calibration);

   /** \assert Verify that guardrail is classified as valid. */
   EXPECT_TRUE(f_guardrail_valid);
}

/**
 * Test that invalid guardrail is correctly classified by corresponding function.
 * \uts{CSCSA-101406} \sdd{SF-8040} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Valid_Guardrail__returns_false_for_invalid_guardrail_high_confidence)
{
   /** \arrange Set up valid guardrail on left side. */
   boolean_T f_guardrail_valid;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_INVALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail;

   /** \action Call function Scw_Is_Valid_Guardrail to evaluate if guardrail is valid. */
   f_guardrail_valid = Scw_Is_Valid_Guardrail(p_scw_core_input, FBK_SIDE_LEFT, p_scw_calibration);

   /** \assert Verify that guardrail is classified as valid. */
   EXPECT_FALSE(f_guardrail_valid);
}

/**
 * Test that invalid guardrail is correctly classified by corresponding function.
 * \uts{CSCSA-101407} \sdd{SF-8040} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Is_Valid_Guardrail__returns_false_for_valid_guardrail_low_confidence)
{
   /** \arrange Set up valid guardrail on left side. */
   boolean_T f_guardrail_valid;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.type       = SCW_GUARDRAIL_VALID;
   p_scw_core_input->guardrail_data[FBK_SIDE_LEFT].radar.confidence = p_scw_calibration->k_scw_min_exist_prob_radar_guardrail - EPSILON;

   /** \action Call function Scw_Is_Valid_Guardrail to evaluate if guardrail is valid. */
   f_guardrail_valid = Scw_Is_Valid_Guardrail(p_scw_core_input, FBK_SIDE_LEFT, p_scw_calibration);

   /** \assert Verify that guardrail is classified as valid. */
   EXPECT_FALSE(f_guardrail_valid);
}

/**
 * Tests whether the lateral zone hysteresis shall be updated.
 * \uts{CSCSA-44514} \sdd{SF-8186} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Shall_Lat_Zone_Hyst_Be_Applied__lateral_off_shall_be_added_for_right_zone)
{
   /** \arrange Set up cals such that an hysteresis application on the right side is demanded. */
   boolean_T res;
   uint8_t zone_point_index                                     = SCW_ZONE_FRONT_RIGHT;
   p_scw_calibration->k_scw_hys_zone_y_offset[zone_point_index] = 0.25f;

   /** \action Call function to test */
   res = Scw_Shall_Lat_Zone_Hyst_Be_Applied(p_scw_calibration, zone_point_index);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(res);
}

/**
 * Tests whether the lateral zone hysteresis shall be updated. Here the calibration is set to negative values, thus no update is
 * expected \uts{CSCSA-44515} \sdd{SF-8186} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Shall_Lat_Zone_Hyst_Be_Applied__no_lateral_hysteresis_shall_be_added)
{
   /** \arrange Set up cals such that no hysteresis application is demanded. */
   boolean_T res;
   uint8_t zone_point_index                                     = SCW_ZONE_FRONT_RIGHT;
   p_scw_calibration->k_scw_hys_zone_y_offset[zone_point_index] = -0.25f;

   /** \action Call function to test. */
   res = Scw_Shall_Lat_Zone_Hyst_Be_Applied(p_scw_calibration, zone_point_index);

   /** \assert Verify that false is returned. */
   EXPECT_FALSE(res);
}


/**
 * Tests whether the longitudinal zone hysteresis shall be updated.
 * \uts{CSCSA-44516} \sdd{SF-8185} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Shall_Long_Zone_Hyst_Be_Applied__lateral_off_shall_be_added_for_right_zone)
{
   /** \arrange Set up cals such that an hysteresis application on the right side is demanded. */
   boolean_T res;
   uint8_t zone_point_index                                     = SCW_ZONE_FRONT_RIGHT;
   p_scw_calibration->k_scw_hys_zone_x_offset[zone_point_index] = 0.25f;

   /** \action Call function to test */
   res = Scw_Shall_Long_Zone_Hyst_Be_Applied(p_scw_calibration, zone_point_index);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Tests whether the longitudinal zone hysteresis shall be updated. Here the calibration is set such that no update is expected
 * \uts{CSCSA-44517} \sdd{SF-8185} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Shall_Long_Zone_Hyst_Be_Applied__no_longitudinal_offset_for_you)
{
   /** \arrange Set up cals such that no hysteresis application is demanded. */
   boolean_T res;
   uint8_t zone_point_index                                     = SCW_ZONE_FRONT_RIGHT;
   p_scw_calibration->k_scw_hys_zone_x_offset[zone_point_index] = -0.25f;

   /** \action Call function to test. */
   res = Scw_Shall_Long_Zone_Hyst_Be_Applied(p_scw_calibration, zone_point_index);

   /** \assert Verify that false is returned. */
   EXPECT_FALSE(res);
}


/**
 * Test that the reset of critical object data works properly.
 * \uts{CSCSA-121154} \sdd{CSCSA-121172} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Reset_Critical_Object_Data__works_properly)
{
   /** \arrange Set up critical object data with non-default values. */
   Scw_Critical_Object_T scw_critical_object;
   scw_critical_object.index            = FBK_ZERO_UINT;
   scw_critical_object.position_x       = 0.5f * SCW_MIN_POSITION_X;
   scw_critical_object.lateral_distance = 0.5f * SCW_MAX_LATERAL_DISTANCE;
   scw_critical_object.lateral_ttc      = 0.5f * SCW_MAX_LATERAL_TTC;

   /** \action Call function to reset object persistent data. */
   Scw_Reset_Critical_Object_Data(&scw_critical_object, p_scw_calibration);

   /** \assert Verify that critical object data is reseted to default values. */
   EXPECT_EQ(scw_critical_object.index, PA_INVALID_OBJ_INDEX);
   EXPECT_FLOAT_EQ(scw_critical_object.position_x, SCW_MIN_POSITION_X);
   EXPECT_FLOAT_EQ(scw_critical_object.lateral_distance, p_scw_calibration->k_scw_lateral_distance_default);
   EXPECT_FLOAT_EQ(scw_critical_object.lateral_ttc, p_scw_calibration->k_scw_lateral_ttc_default);
   EXPECT_FLOAT_EQ(scw_critical_object.ttp, p_scw_calibration->k_scw_ttp_default);
   EXPECT_FLOAT_EQ(scw_critical_object.ttle, p_scw_calibration->k_scw_ttle_default);
}


/**
 * Test that the preparation of object data works properly.
 * \uts{CSCSA-121155} \sdd{CSCSA-121174} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Prepare_Object_Information__works_properly)
{
   /** \arrange Set up the object data with other then preparation values. */
   Scw_Object_T scw_object;
   scw_object.extended_data.index           = FBK_ZERO_UINT;
   uint8_t new_index                        = FBK_ONE_UINT;
   float32_T new_position_x                 = SCW_MIN_POSITION_X;
   scw_object.tracker_data.vcs_pos.x        = FBK_ZERO_F;
   pa_data.object_data[new_index].vcs_pos.x = new_position_x;

   /** \action Call function to prepare object data. */
   Scw_Prepare_Object_Information(&scw_object, p_scw_core_input, p_scw_calibration, new_index);

   /** \assert Verify that the object data is set properly */
   EXPECT_EQ(scw_object.extended_data.index, new_index);
   EXPECT_FLOAT_EQ(scw_object.extended_data.position_x, new_position_x);
}


/**
 * Test that dynamic object lateral distance is calculated properly on the left side.
 * \uts{CSCSA-121156} \sdd{CSCSA-121167} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Dyn_Object_Lateral_Distance__works_properly_left_side)
{
   /** \arrange Set up the object and host data. */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, FBK_ZERO_UINT);
   p_vehicle_data->host_width = 1.8f;
   float32_T expected_lateral_distance =
      -scw_object.tracker_data.vcs_pos.y - 0.5f * scw_object.tracker_data.width - 0.5f * p_vehicle_data->host_width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Dyn_Object_Lateral_Distance(&scw_object, p_scw_core_input);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that dynamic object lateral distance is calculated properly on the right side.
 * \uts{CSCSA-121157} \sdd{CSCSA-121167} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Dyn_Object_Lateral_Distance__works_properly_right_side)
{
   /** \arrange Set up the object and host data. */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, FBK_ZERO_UINT);
   scw_object.tracker_data.vcs_pos.y = 2.0f;
   scw_object.nearest_corner_y       = 1.0f;
   p_vehicle_data->host_width        = 1.8f;
   float32_T expected_lateral_distance =
      scw_object.tracker_data.vcs_pos.y - 0.5f * scw_object.tracker_data.width - 0.5f * p_vehicle_data->host_width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Dyn_Object_Lateral_Distance(&scw_object, p_scw_core_input);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that dynamic object lateral distance is calculated properly on the right side, with connected wide trailer.
 * \uts{CSCSA-255253} \sdd{CSCSA-121167} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Dyn_Object_Lateral_Distance__wide_trailer_connected)
{
   /** \arrange Set up the object and host data. */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, FBK_ZERO_UINT);
   scw_object.tracker_data.vcs_pos.y   = 2.0f;
   scw_object.nearest_corner_y         = 1.0f;
   p_vehicle_data->host_width          = 1.8f;
   p_scw_core_input->trailer.f_present = FBK_TRUE;
   p_scw_core_input->trailer.length    = 5.0f;
   p_scw_core_input->trailer.width     = 2.0f;
   float32_T expected_lateral_distance =
      scw_object.tracker_data.vcs_pos.y - 0.5f * scw_object.tracker_data.width - 0.5f * p_scw_core_input->trailer.width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Dyn_Object_Lateral_Distance(&scw_object, p_scw_core_input);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that dynamic object lateral distance is calculated properly on the right side, with connected narrow trailer.
 * \uts{CSCSA-255254} \sdd{CSCSA-121167} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Dyn_Object_Lateral_Distance__narrow_trailer_connected)
{
   /** \arrange Set up the object and host data. */
   Scw_Object_T scw_object;
   Scw_Create_Valid_Tracker_Object(&scw_object, FBK_ZERO_UINT);
   scw_object.tracker_data.vcs_pos.y   = 2.0f;
   scw_object.nearest_corner_y         = 1.0f;
   p_vehicle_data->host_width          = 1.8f;
   p_scw_core_input->trailer.f_present = FBK_TRUE;
   p_scw_core_input->trailer.length    = 5.0f;
   p_scw_core_input->trailer.width     = 1.5f;
   float32_T expected_lateral_distance =
      scw_object.tracker_data.vcs_pos.y - 0.5f * scw_object.tracker_data.width - 0.5f * p_vehicle_data->host_width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Dyn_Object_Lateral_Distance(&scw_object, p_scw_core_input);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that the guardrail lateral distance is calculated properly on the left side.
 * \uts{CSCSA-121158} \sdd{CSCSA-121173} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Guardrail_Lateral_Distance__works_properly_left_side)
{
   /** \arrange Set up the guardrail and host data. */
   uint8_t side                                                  = FBK_SIDE_LEFT;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   p_vehicle_data->host_width                                    = 1.8f;
   float32_T expected_lateral_distance =
      -p_scw_core_input->guardrail_data[side].radar.lateral_position - 0.5f * pa_data.vehicle_data.host_width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Guardrail_Lateral_Distance(p_scw_core_input, side);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that the guardrail lateral distance is calculated properly on the right side.
 * \uts{CSCSA-121159} \sdd{CSCSA-121173} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Guardrail_Lateral_Distance__works_properly_right_side)
{
   /** \arrange Set up the guardrail and host data. */
   uint8_t side                                                  = FBK_SIDE_RIGHT;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = 2.0f;
   p_vehicle_data->host_width                                    = 1.8f;
   float32_T expected_lateral_distance =
      p_scw_core_input->guardrail_data[side].radar.lateral_position - 0.5f * pa_data.vehicle_data.host_width;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Guardrail_Lateral_Distance(p_scw_core_input, side);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that the guardrail lateral distance is calculated properly on the right side when the guardrail is overlapping the host.
 * \uts{CSCSA-204189} \sdd{CSCSA-121173} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Guardrail_Lateral_Distance__right_side_overlapping_host)
{
   /** \arrange Set up the guardrail and host data. */
   uint8_t side                                                  = FBK_SIDE_RIGHT;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = 0.5f;
   p_vehicle_data->host_width                                    = 1.8f;
   float32_T expected_lateral_distance                           = FBK_ZERO_F;
   float32_T lateral_distance;

   /** \action Call function to calculate dynamic object lateral distance. */
   lateral_distance = Scw_Get_Guardrail_Lateral_Distance(p_scw_core_input, side);

   /** \assert Verify that the object data is set properly */
   EXPECT_FLOAT_EQ(lateral_distance, expected_lateral_distance);
}


/**
 * Test that the lateral ttc is calculated properly on the left side.
 * \uts{CSCSA-121160} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__left_side)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = 3.0f;
   float32_T relative_lateral_velocity = 1.50f;
   float32_T expected_ttc              = lateral_distance / relative_lateral_velocity;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_LEFT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral ttc is calculated properly on the right side.
 * \uts{CSCSA-121161} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__right_side)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = 3.0f;
   float32_T relative_lateral_velocity = -1.50f;
   float32_T expected_ttc              = lateral_distance / -relative_lateral_velocity;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_RIGHT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral ttc is calculated properly on the left side, negative lateral distance.
 * \uts{CSCSA-121162} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__left_side_negative_lat_distance)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = -3.0f;
   float32_T relative_lateral_velocity = 1.50f;
   float32_T expected_ttc              = -FBK_ONE_F;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_LEFT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral ttc is calculated properly on the right side, negative lateral distance.
 * \uts{CSCSA-121163} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__right_side_negative_lat_distance)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = -3.0f;
   float32_T relative_lateral_velocity = -1.50f;
   float32_T expected_ttc              = -FBK_ONE_F;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_RIGHT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral ttc is calculated properly on the left side, object moves away.
 * \uts{CSCSA-121164} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__left_side_negative_ttc)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = 3.0f;
   float32_T relative_lateral_velocity = -1.50f;
   float32_T expected_ttc              = -FBK_ONE_F;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_LEFT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral ttc is calculated properly on the right side, object moves away.
 * \uts{CSCSA-121165} \sdd{CSCSA-121170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Lateral_TTC__right_side_negative_ttc)
{
   /** \arrange Set up the input data. */
   float32_T lateral_distance          = 3.0f;
   float32_T relative_lateral_velocity = 1.50f;
   float32_T expected_ttc              = -FBK_ONE_F;
   float32_T ttc;

   /** \action Call function to calculate lateral ttc. */
   ttc = Scw_Get_TTx(lateral_distance, relative_lateral_velocity, LAT_TTC_RIGHT, SCW_MAX_LATERAL_TTC, -FBK_ONE_F);

   /** \assert Verify that the ttc is calculated properly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that the lateral velocity of the Host to the guardrail is calculated correctly, left side.
 * \uts{CSCSA-135509} \sdd{CSCSA-125685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Guardrail_Position_Derivative__velocity_left)
{
   /** \arrange Set up the input data. */
   uint8_t side                                                  = FBK_SIDE_LEFT;
   uint8_t order                                                 = SCW_VELOCITY;
   p_scw_persistent->core_grail_lat_position[side][1]            = -3.0f;
   p_scw_persistent->count_in_zone_grail[side]                   = 255u;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   pa_data.time_diff_to_last_cycle                               = 1.0f;
   float32_T expected_velocity =
      (p_scw_core_input->guardrail_data[side].radar.lateral_position - p_scw_persistent->core_grail_lat_position[side][1])
      / p_scw_core_input->p_pa_data->time_diff_to_last_cycle;
   float32_T velocity;

   /** \action Call function to calculate lateral velocity. */
   velocity = Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, order, side);

   /** \assert Verify that the lateral velocity is calculated correctly */
   EXPECT_FLOAT_EQ(velocity, expected_velocity);
}


/**
 * Test that the lateral velocity of the Host to the guardrail is calculated correctly, right side.
 * \uts{CSCSA-204190} \sdd{CSCSA-125685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Guardrail_Position_Derivative__velocity_right)
{
   /** \arrange Set up the input data. */
   uint8_t side                                                  = FBK_SIDE_RIGHT;
   uint8_t order                                                 = SCW_VELOCITY;
   p_scw_persistent->core_grail_lat_position[side][1]            = -3.0f;
   p_scw_persistent->count_in_zone_grail[side]                   = 255u;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   pa_data.time_diff_to_last_cycle                               = 1.0f;
   float32_T expected_velocity =
      (p_scw_core_input->guardrail_data[side].radar.lateral_position - p_scw_persistent->core_grail_lat_position[side][1])
      / p_scw_core_input->p_pa_data->time_diff_to_last_cycle;
   float32_T velocity;

   /** \action Call function to calculate lateral velocity. */
   velocity = Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, order, side);

   /** \assert Verify that the lateral velocity is calculated correctly */
   EXPECT_FLOAT_EQ(velocity, expected_velocity);
}


/**
 * Test that the lateral velocity of the Host to the guardrail is calculated correctly, invalid side.
 * \uts{CSCSA-204191} \sdd{CSCSA-125685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Guardrail_Position_Derivative__velocity_invalid_side)
{
   /** \arrange Set up the input data. */
   uint8_t side                                                  = FBK_SIDE_RIGHT + FBK_ONE_UINT; /* invalid side */
   uint8_t order                                                 = SCW_VELOCITY;
   p_scw_persistent->core_grail_lat_position[side][1]            = -3.0f;
   p_scw_persistent->count_in_zone_grail[side]                   = 255u;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   pa_data.time_diff_to_last_cycle                               = 1.0f;
   float32_T expected_velocity                                   = FBK_ZERO_F;
   float32_T velocity;

   /** \action Call function to calculate lateral velocity. */
   velocity = Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, order, side);

   /** \assert Verify that the lateral velocity is calculated correctly */
   EXPECT_FLOAT_EQ(velocity, expected_velocity);
}


/**
 * Test that the lateral acceleration of the Host to the guardrail is calculated correctly.
 * \uts{CSCSA-135510} \sdd{CSCSA-125685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Guardrail_Position_Derivative__acceleration_left)
{
   /** \arrange Set up the input data. */
   uint8_t side                                                  = FBK_SIDE_LEFT;
   uint8_t order                                                 = SCW_ACCELERATION;
   p_scw_persistent->core_grail_lat_position[side][0]            = -4.0f;
   p_scw_persistent->core_grail_lat_position[side][1]            = -3.0f;
   p_scw_persistent->count_in_zone_grail[side]                   = 255u;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   pa_data.time_diff_to_last_cycle                               = 1.0f;
   float32_T expected_acceleration =
      (p_scw_core_input->guardrail_data[side].radar.lateral_position - (2.0f * p_scw_persistent->core_grail_lat_position[side][1])
       + p_scw_persistent->core_grail_lat_position[side][0])
      / (pa_data.time_diff_to_last_cycle * pa_data.time_diff_to_last_cycle);
   float32_T acceleration;

   /** \action Call function to calculate lateral acceleration. */
   acceleration = Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, order, side);

   /** \assert Verify that the lateral acceleration is calculated correctly */
   EXPECT_FLOAT_EQ(acceleration, expected_acceleration);
}


/**
 * Test that the default output value is returned for derivative order higher than 2.
 * \uts{CSCSA-204192} \sdd{CSCSA-125685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Guardrail_Position_Derivative__default)
{
   /** \arrange Set up the input data. */
   uint8_t side                                                  = FBK_SIDE_LEFT;
   uint8_t order                                                 = 3u;
   p_scw_persistent->core_grail_lat_position[side][1]            = -3.0f;
   p_scw_persistent->count_in_zone_grail[side]                   = 255u;
   p_scw_core_input->guardrail_data[side].radar.lateral_position = -2.0f;
   pa_data.time_diff_to_last_cycle                               = 1.0f;
   float32_T expected_output                                     = FBK_ZERO_F;
   float32_T output;

   /** \action Call function to calculate lateral velocity. */
   output = Scw_Calculate_Guardrail_Position_Derivative(p_scw_persistent, p_scw_core_input, order, side);

   /** \assert Verify that the output is calculated correctly */
   EXPECT_FLOAT_EQ(output, expected_output);
}


/**
 * Test that the criticality level is determined properly.
 * \uts{CSCSA-121166} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__returns_level_2)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance - EPSILON;
   float32_T lateral_ttc          = min_lateral_ttc - EPSILON;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_2;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined properly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 2 if the object's lateral distance is critical and ttc equals zero.
 * \uts{CSCSA-210361} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__returns_level_2_zero_ttc)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance - EPSILON;
   float32_T lateral_ttc          = 0.0f;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_2;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral distance is critical, ttc equals zero and
 * min_lateral_ttc is negative (rather unexpected) \uts{CSCSA-210362} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__returns_level_1_zero_ttc_negative_min_lateral_ttc)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = -1.0f;
   float32_T lateral_distance     = min_lateral_distance - EPSILON;
   float32_T lateral_ttc          = 0.0f;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral distance is critical and ttc is negative.
 * \uts{CSCSA-210363} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__returns_level_1_negative_ttc)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance - EPSILON;
   float32_T lateral_ttc          = -1.0f;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral distance is critical but ttc not
 * \uts{CSCSA-204193} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__level_1_low_distance)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance - EPSILON;
   float32_T lateral_ttc          = min_lateral_ttc + EPSILON;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's ttc is critical but lateral distance not
 * \uts{CSCSA-204194} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__level_1_low_ttc)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance + EPSILON;
   float32_T lateral_ttc          = min_lateral_ttc - EPSILON;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral distance and ttc are not critical
 * \uts{CSCSA-204195} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__level_1)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance + EPSILON;
   float32_T lateral_ttc          = min_lateral_ttc + EPSILON;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined properly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral distance is not critical and the ttc is negative
 * \uts{CSCSA-204196} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__level_1_negative_ttc)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = min_lateral_distance + EPSILON;
   float32_T lateral_ttc          = -1.0f;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined properly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that the function returns criticality level 1 if the object's lateral ttc is not critical and the lateral distance is
 * negative \uts{CSCSA-305556} \sdd{CSCSA-121171} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Get_Criticality_Level__level_1_negative_distance)
{
   /** \arrange Set up the input data. */
   float32_T min_lateral_distance = 0.3f;
   float32_T min_lateral_ttc      = 0.7f;
   float32_T lateral_distance     = -1.0f;
   float32_T lateral_ttc          = 1.0f;
   float32_T lateral_distance_hys = 0.2f;
   float32_T lateral_ttc_hys      = 0.2f;
   Scw_Alert_Level_T criticality_level;
   Scw_Alert_Level_T expected_level = SCW_ALERT_LEVEL_1;
   uint8_t side                     = FBK_SIDE_LEFT;
   Scw_Reset_Prev_Was_for_Side(p_scw_persistent, side);

   /** \action Call function to determine criticality level. */
   criticality_level = Scw_Get_Criticality_Level(p_scw_persistent, lateral_distance, lateral_ttc, min_lateral_distance,
                                                 min_lateral_distance, min_lateral_ttc, min_lateral_ttc, lateral_distance_hys,
                                                 lateral_ttc_hys, side);

   /** \assert Verify that the criticality level is determined correctly */
   EXPECT_EQ(criticality_level, expected_level);
}


/**
 * Test that object farthest longitudinal corner is calculated correctly, distance in threshold range.
 * \uts{CSCSA-333887} \sdd{CSCSA-333866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Farthest_Object_Corner_Long__long_pos_below_thresh)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T expected_x;

   scw_object.tracker_data.vcs_heading = -PI / 4.0f;
   scw_object.tracker_data.width       = 1.0f;
   scw_object.tracker_data.length      = 1.0f;
   scw_object.tracker_data.vcs_pos.x   = -1.0f;
   scw_object.tracker_data.vcs_pos.y   = 1.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   /* simplified calculations with assumption of heading = -45deg */
   expected_x = scw_object.tracker_data.vcs_pos.x - scw_object.tracker_data.length * sqrtf(2.0f) / 2.0f;

   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   /** \action Call function to calculate object farthest longitudinal corner. */
   Scw_Calculate_Farthest_Object_Corner_Long(&object_corners, &scw_object);

   /** \assert Verify that object farthest longitudinal corner is calculated correctly. */
   EXPECT_FLOAT_EQ(scw_object.rearmost_corner_x, expected_x);
}


/**
 * Test that object farthest longitudinal corner is calculated correctly, distance above maximum value.
 * \uts{CSCSA-333888} \sdd{CSCSA-333866} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Farthest_Object_Corner_Long__long_pos_above_thresh)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T expected_x;

   scw_object.tracker_data.vcs_heading = -PI / 4.0f;
   scw_object.tracker_data.width       = 1.0f;
   scw_object.tracker_data.length      = 1.0f;
   scw_object.tracker_data.vcs_pos.x   = 1.1f * SCW_BIG_VALUE;
   scw_object.tracker_data.vcs_pos.y   = 1.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   /* simplified calculations with assumption of heading = -45deg */
   expected_x = SCW_BIG_VALUE;

   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   /** \action Call function to calculate object farthest longitudinal corner. */
   Scw_Calculate_Farthest_Object_Corner_Long(&object_corners, &scw_object);

   /** \assert Verify that object farthest longitudinal corner is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.rearmost_corner_x, expected_x);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case for only front corner in left zone.
 * \uts{CSCSA-333889} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__only_front_corner_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = 0.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.vcs_pos.x   = -5.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_FRONT_RIGHT_CORNER].y);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case for only rear corner in left zone.
 * \uts{CSCSA-333890} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__only_rear_corner_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = 0.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.vcs_pos.x   = 1.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_REAR_RIGHT_CORNER].y);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when whole object in left zone and rear corner is
 * nearest. \uts{CSCSA-333891} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__whole_object_heading_negative_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = -PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 2.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_REAR_RIGHT_CORNER].y);
}

/**
 * Test that object nearest lateral corner/point is calculated correctly, case when whole object in left zone and front corner is
 * nearest. \uts{CSCSA-333892} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__whole_object_heading_positive_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 2.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_FRONT_RIGHT_CORNER].y);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when long object in left zone and front point is
 * nearest. \uts{CSCSA-333893} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__long_object_heading_positive_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T nearest_point_front_bumper;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 10.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   nearest_point_front_bumper = Scw_Nearest_Point_Lateral_Interpolation(
      object_corners.points[FBK_REAR_RIGHT_CORNER].x, Fbk_Abs_F(object_corners.points[FBK_REAR_RIGHT_CORNER].y),
      object_corners.points[FBK_FRONT_RIGHT_CORNER].x, Fbk_Abs_F(object_corners.points[FBK_FRONT_RIGHT_CORNER].y), FBK_ZERO_F);

   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, -nearest_point_front_bumper);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when long object in left zone and rear point is
 * nearest. \uts{CSCSA-333894} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__long_object_heading_negative_in_left_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T nearest_point_rear_bumper;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = -PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 10.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = -3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = -5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = -3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = -1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = -1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = -1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = -3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = -5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = -5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   nearest_point_rear_bumper = Scw_Nearest_Point_Lateral_Interpolation(object_corners.points[FBK_REAR_RIGHT_CORNER].x,
                                                                       Fbk_Abs_F(object_corners.points[FBK_REAR_RIGHT_CORNER].y),
                                                                       object_corners.points[FBK_FRONT_RIGHT_CORNER].x,
                                                                       Fbk_Abs_F(object_corners.points[FBK_FRONT_RIGHT_CORNER].y),
                                                                       -p_vehicle_data->host_length);

   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, -nearest_point_rear_bumper);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when long object in right zone and front point is
 * nearest. \uts{CSCSA-333895} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__long_object_heading_negative_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T nearest_point_front_bumper;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = -PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 10.0f;
   scw_object.tracker_data.vcs_pos.x   = 2.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = 2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = 5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = 5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = 5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = 2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   nearest_point_front_bumper = Scw_Nearest_Point_Lateral_Interpolation(
      object_corners.points[FBK_REAR_LEFT_CORNER].x, Fbk_Abs_F(object_corners.points[FBK_REAR_LEFT_CORNER].y),
      object_corners.points[FBK_FRONT_LEFT_CORNER].x, Fbk_Abs_F(object_corners.points[FBK_FRONT_LEFT_CORNER].y), FBK_ZERO_F);

   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, nearest_point_front_bumper);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when long object in right zone and rear point is
 * nearest. \uts{CSCSA-333896} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__long_object_heading_positive_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;
   float32_T nearest_point_rear_bumper;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 10.0f;
   scw_object.tracker_data.vcs_pos.x   = 2.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = 2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = 5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = 5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = 5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = 2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));

   nearest_point_rear_bumper = Scw_Nearest_Point_Lateral_Interpolation(object_corners.points[FBK_REAR_LEFT_CORNER].x,
                                                                       Fbk_Abs_F(object_corners.points[FBK_REAR_LEFT_CORNER].y),
                                                                       object_corners.points[FBK_FRONT_LEFT_CORNER].x,
                                                                       Fbk_Abs_F(object_corners.points[FBK_FRONT_LEFT_CORNER].y),
                                                                       -p_vehicle_data->host_length);

   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, nearest_point_rear_bumper);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case when whole object in right zone and rear corner is
 * nearest. \uts{CSCSA-333897} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__whole_object_heading_positive_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 2.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_REAR_LEFT_CORNER].y);
}

/**
 * Test that object nearest lateral corner/point is calculated correctly, case when whole object in right zone and front corner is
 * nearest. \uts{CSCSA-333898} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__whole_object_heading_negative_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = -PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 2.0f;
   scw_object.tracker_data.vcs_pos.x   = -2.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_FRONT_LEFT_CORNER].y);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case for only rear corner in right zone.
 * \uts{CSCSA-333899} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__only_rear_corner_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.vcs_pos.x   = 1.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_REAR_LEFT_CORNER].y);
}


/**
 * Test that object nearest lateral corner/point is calculated correctly, case for only front corner in right zone.
 * \uts{CSCSA-333900} \sdd{CSCSA-333861} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Calculate_Nearest_Object_Corner_Lat__only_front_corner_in_right_zone)
{
   /** \arrange Set up the object data. */
   Fbk_Object_Corners_T object_corners;
   Scw_Object_T scw_object;


   p_vehicle_data->host_length         = 2.0f;
   scw_object.tracker_data.vcs_heading = PI / 6.0f;
   scw_object.tracker_data.width       = 2.0f;
   scw_object.tracker_data.length      = 4.0f;
   scw_object.tracker_data.vcs_pos.x   = -5.0f;
   scw_object.tracker_data.vcs_pos.y   = 3.0f;
   scw_object.rearmost_corner_x        = SCW_BIG_VALUE;
   scw_zone.points[0].x                = 1.0f;
   scw_zone.points[0].y                = 5.0f;
   scw_zone.points[1].x                = 1.0f;
   scw_zone.points[1].y                = 3.0f;
   scw_zone.points[2].x                = 1.0f;
   scw_zone.points[2].y                = 1.0f;
   scw_zone.points[3].x                = -2.0f;
   scw_zone.points[3].y                = 1.0f;
   scw_zone.points[4].x                = -5.0f;
   scw_zone.points[4].y                = 1.0f;
   scw_zone.points[5].x                = -5.0f;
   scw_zone.points[5].y                = 3.0f;
   scw_zone.points[6].x                = -5.0f;
   scw_zone.points[6].y                = 5.0f;
   scw_zone.points[7].x                = -2.0f;
   scw_zone.points[7].y                = 5.0f;
   scw_zone.size                       = 8u;


   Fbk_Calculate_Target_Corners(&object_corners, &(scw_object.tracker_data.vcs_pos), &(scw_object.tracker_data.vcs_heading),
                                &(scw_object.tracker_data.length), &(scw_object.tracker_data.width));


   /** \action Call function to calculate object nearest lateral corner/point. */
   Scw_Calculate_Nearest_Object_Corner_Lat(&object_corners, &scw_zone, &scw_object, p_vehicle_data->host_length);

   /** \assert Verify that object nearest lateral corner/point is calculated correctly */
   EXPECT_FLOAT_EQ(scw_object.nearest_corner_y, object_corners.points[FBK_FRONT_LEFT_CORNER].y);
}


/**
 * Test that interpolated lateral coordinate is calculated correctly, case for both point creating the line having same X values.
 * \uts{CSCSA-333901} \sdd{CSCSA-333823} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Nearest_Point_Lateral_Interpolation__x_equal)
{
   /** \arrange Set up input data. */

   float32_T x1 = 1.0f;
   float32_T y1 = 1.0f;
   float32_T x2 = 1.0f;
   float32_T y2 = 1.0f;
   float32_T x  = 1.0f;
   float32_T y;
   float32_T y_exp = (y1 + y2) / 2.0f;

   /** \action Call function to calculate object corners and most advanced points. */
   y = Scw_Nearest_Point_Lateral_Interpolation(x1, y1, x2, y2, x);

   /** \assert Verify that interpolated coordinate is calculated correctly */
   EXPECT_FLOAT_EQ(y, y_exp);
}

/**
 * Test that interpolated lateral coordinate is calculated correctly, case for X1 bigger than X2.
 * \uts{CSCSA-333902} \sdd{CSCSA-333823} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Nearest_Point_Lateral_Interpolation__x_sub_negative)
{
   /** \arrange Set up input data. */

   float32_T x1 = 1.0f;
   float32_T y1 = 1.0f;
   float32_T x2 = 2.0f;
   float32_T y2 = 1.0f;
   float32_T x  = 1.0f;
   float32_T y;
   float32_T y_exp = y1 + ((x - x1) * ((y2 - y1) / (x2 - x1)));


   /** \action Call function to calculate object corners and most advanced points. */
   y = Scw_Nearest_Point_Lateral_Interpolation(x1, y1, x2, y2, x);

   /** \assert Verify that interpolated coordinate is calculated correctlyy */
   EXPECT_FLOAT_EQ(y, y_exp);
}


/**
 * Test that interpolated lateral coordinate is calculated correctly, case for X2 bigger than X1.
 * \uts{CSCSA-333903} \sdd{CSCSA-333823} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Test, Scw_Nearest_Point_Lateral_Interpolation__x_sub_positive)
{
   /** \arrange Set up input data. */

   float32_T x1 = 2.0f;
   float32_T y1 = 1.0f;
   float32_T x2 = 1.0f;
   float32_T y2 = 1.0f;
   float32_T x  = 1.0f;
   float32_T y;
   float32_T y_exp = y1 + ((x - x1) * ((y2 - y1) / (x2 - x1)));


   /** \action Call function to interpolate coordinate. */
   y = Scw_Nearest_Point_Lateral_Interpolation(x1, y1, x2, y2, x);

   /** \assert Verify that interpolated coordinate is calculated correctly */
   EXPECT_FLOAT_EQ(y, y_exp);
}