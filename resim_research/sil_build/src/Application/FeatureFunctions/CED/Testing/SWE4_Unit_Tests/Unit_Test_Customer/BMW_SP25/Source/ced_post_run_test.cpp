/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 CED post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41649}
 */

#include "ced_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_post_run.c"
#include "ced_pre_run.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}


/**
 * Check that the mapping of CED object class to the SFE object class is done correctly for fast two-wheel.
 * \uts{CSCSA-41755} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Class_To_Sfe__2wheel_resolved_to_motorcycle)
{
   /** \arrange set input such that object class is fast two-wheel. */
   SFE_object_class_T sfe_obj_class;
   float32_T speed   = 1.1f * CED_K_OBJ_CLASS_MAP_VEL_MAX_THRES * CED_KPH2MPS;
   tracker_obj_class = PA_OBJ_CLASS_2WHEEL;

   /** \action execute object class mapping to SFE object class */
   sfe_obj_class = Ced_Map_Object_Class_To_Sfe(tracker_obj_class, speed);

   /** \assert expect object class of SFE to be motorcycle */
   EXPECT_EQ(sfe_obj_class, SFE_OBJECT_CLASS_MOTORCYCLE);
}


/**
 * Check that the mapping of CED object class to the SFE object class is done correctly for slow two-wheel.
 * \uts{CSCSA-41756} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Class_To_Sfe__2wheel_resolved_to_bicycle)
{
   /** \arrange set input such that object class is slow two-wheel. */
   SFE_object_class_T sfe_obj_class;
   float32_T speed   = 0.9f * CED_K_OBJ_CLASS_MAP_VEL_MAX_THRES * CED_KPH2MPS;
   tracker_obj_class = PA_OBJ_CLASS_2WHEEL;

   /** \action execute object class mapping to SFE object class */
   sfe_obj_class = Ced_Map_Object_Class_To_Sfe(tracker_obj_class, speed);

   /** \assert expect object class of SFE to be bicycle */
   EXPECT_EQ(sfe_obj_class, SFE_OBJECT_CLASS_BICYCLE);
}

/**
 * Check that the mapping of CED object class to the SFE object class is done correctly for a pedestrian.
 * \uts{CSCSA-41757} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Class_To_Sfe__pedestrian_resolved_correctly)
{
   /** \arrange set input such that object class is pedestrian. */
   SFE_object_class_T sfe_obj_class;
   float32_T speed   = 4.0f;
   tracker_obj_class = PA_OBJ_CLASS_PEDESTRIAN;

   /** \action execute object class mapping to SFE object class */
   sfe_obj_class = Ced_Map_Object_Class_To_Sfe(tracker_obj_class, speed);

   /** \assert expect object class of SFE to be pedestrian */
   EXPECT_EQ(sfe_obj_class, SFE_OBJECT_CLASS_PEDESTRIAN);
}

/**
 * Check that the mapping of CED object class to the SFE object class is done correctly for a truck.
 * \uts{CSCSA-41758} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Class_To_Sfe__truck_resolved_correctly)
{
   /** \arrange set input such that object class is truck. */
   SFE_object_class_T sfe_obj_class;
   float32_T speed   = 4.0f;
   tracker_obj_class = PA_OBJ_CLASS_TRUCK;

   /** \action execute object class mapping to SFE object class */
   sfe_obj_class = Ced_Map_Object_Class_To_Sfe(tracker_obj_class, speed);

   /** \assert expect object class of SFE to be truck */
   EXPECT_EQ(sfe_obj_class, SFE_OBJECT_CLASS_TRUCK);
}

/**
 * Check that the mapping of CED object class to the SFE object class is done correctly for a car.
 * \uts{CSCSA-41759} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Class_To_Sfe__car_resolved_correctly)
{
   /** \arrange set input such that object class is car. */
   SFE_object_class_T sfe_obj_class;
   float32_T speed   = 4.0f;
   tracker_obj_class = PA_OBJ_CLASS_CAR;

   /** \action execute object class mapping to SFE object class */
   sfe_obj_class = Ced_Map_Object_Class_To_Sfe(tracker_obj_class, speed);

   /** \assert expect object class of SFE to be car */
   EXPECT_EQ(sfe_obj_class, SFE_OBJECT_CLASS_CAR);
}


/**
 * Check that initialization routine maps correctly to default.
 * \uts{CSCSA-41760} \sdd{SF-3472} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run_Init__default_for_sfe_output)
{
   /** \arrange set ced output to something different than default. */
   ced_output.SFE_CED_Status                 = 1u;
   ced_output.SFE_CED_alert_right            = 1u;
   ced_output.SFE_CED_dir_right              = 1u;
   ced_output.SFE_CED_ttc_right              = FBK_ONE_F;
   ced_output.SFE_CED_ttp_right              = FBK_ONE_F;
   ced_output.SFE_CED_id_right               = 1u;
   ced_output.SFE_CED_unique_id_right        = 1u;
   ced_output.SFE_CED_obj_speed_right        = FBK_ONE_F;
   ced_output.SFE_CED_obj_type_right         = 2u;
   ced_output.SFE_CED_obj_heading_right      = FBK_ONE_F;
   ced_output.SFE_CED_obj_lateral_pos_right  = FBK_ONE_F;
   ced_output.SFE_CED_obj_long_pos_right     = FBK_ONE_F;
   ced_output.SFE_CED_alert_left             = 1u;
   ced_output.SFE_CED_dir_left               = 1u;
   ced_output.SFE_CED_ttc_left               = FBK_ONE_F;
   ced_output.SFE_CED_ttp_left               = FBK_ONE_F;
   ced_output.SFE_CED_id_left                = 1u;
   ced_output.SFE_CED_unique_id_left         = 1u;
   ced_output.SFE_CED_obj_speed_left         = FBK_ONE_F;
   ced_output.SFE_CED_obj_type_left          = 2u;
   ced_output.SFE_CED_obj_heading_left       = FBK_ONE_F;
   ced_output.SFE_CED_obj_lateral_pos_left   = FBK_ONE_F;
   ced_output.SFE_CED_obj_long_pos_left      = FBK_ONE_F;
   ced_output.SFE_CED_rear_status            = 1u;
   ced_output.SFE_CED_rear_alert_right       = (uint8_t) 1u;
   ced_output.SFE_CED_rear_alert_left        = (uint8_t) 1u;
   ced_output.SFE_CED_rear_id_right          = 1u;
   ced_output.SFE_CED_rear_id_left           = 1u;
   ced_output.SFE_CED_rear_path_match_right  = 1u;
   ced_output.SFE_CED_rear_path_match_left   = 1u;
   ced_output.SFE_CED_rear_ttc_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttc_left          = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttp_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttp_left          = FBK_ONE_F;
   ced_output.SFE_CED_rear_lat_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_lat_left          = FBK_ONE_F;
   ced_output.SFE_CED_front_status           = 1u;
   ced_output.SFE_CED_front_alert_right      = 1u;
   ced_output.SFE_CED_front_alert_left       = 1u;
   ced_output.SFE_CED_front_id_right         = 1u;
   ced_output.SFE_CED_front_id_left          = 1u;
   ced_output.SFE_CED_front_path_match_right = 1u;
   ced_output.SFE_CED_front_path_match_left  = 1u;
   ced_output.SFE_CED_front_ttc_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_ttc_left         = FBK_ONE_F;
   ced_output.SFE_CED_front_ttp_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_ttp_left         = FBK_ONE_F;
   ced_output.SFE_CED_front_lat_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_lat_left         = FBK_ONE_F;

   /** \action execute initialization of sfe output */
   Ced_Reset_Output(&ced_output);

   /** \assert expect output to be set to default */
   EXPECT_EQ(ced_output.SFE_CED_Status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_dir_right, (uint8_t) UNDEF_DIRECTION);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttp_right, CED_INVALID_TIME);
   EXPECT_EQ(ced_output.SFE_CED_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_speed_right, FBK_ZERO_F);
   EXPECT_EQ(ced_output.SFE_CED_obj_type_right, (uint8_t) PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_lateral_pos_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_long_pos_right, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_dir_left, (uint8_t) UNDEF_DIRECTION);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttp_left, CED_INVALID_TIME);
   EXPECT_EQ(ced_output.SFE_CED_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_speed_left, FBK_ZERO_F);
   EXPECT_EQ(ced_output.SFE_CED_obj_type_left, (uint8_t) PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_lateral_pos_left, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_long_pos_left, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_rear_status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttp_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttp_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_lat_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_lat_left, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_front_status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_front_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttp_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttp_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_lat_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_lat_left, CED_INVALID_DISTANCE);
}


/**
 * Check that reset routine maps correctly to default.
 * \uts{CSCSA-41761} \sdd{SF-3411} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Reset_Output__default_for_sfe_output)
{
   /** \arrange set ced output to something different than default. */
   ced_output.SFE_CED_Status                = 1u;
   ced_output.SFE_CED_alert_right           = 1u;
   ced_output.SFE_CED_dir_right             = 1u;
   ced_output.SFE_CED_ttc_right             = FBK_ONE_F;
   ced_output.SFE_CED_ttp_right             = FBK_ONE_F;
   ced_output.SFE_CED_id_right              = 1u;
   ced_output.SFE_CED_unique_id_right       = 1u;
   ced_output.SFE_CED_obj_speed_right       = FBK_ONE_F;
   ced_output.SFE_CED_obj_type_right        = 2u;
   ced_output.SFE_CED_obj_heading_right     = FBK_ONE_F;
   ced_output.SFE_CED_obj_lateral_pos_right = FBK_ONE_F;
   ced_output.SFE_CED_obj_long_pos_right    = FBK_ONE_F;
   ced_output.SFE_CED_alert_left            = 1u;
   ced_output.SFE_CED_dir_left              = 1u;
   ced_output.SFE_CED_ttc_left              = FBK_ONE_F;
   ced_output.SFE_CED_ttp_left              = FBK_ONE_F;
   ced_output.SFE_CED_id_left               = 1u;
   ced_output.SFE_CED_unique_id_left        = 1u;
   ced_output.SFE_CED_obj_speed_left        = FBK_ONE_F;
   ced_output.SFE_CED_obj_type_left         = 2u;
   ced_output.SFE_CED_obj_heading_left      = FBK_ONE_F;
   ced_output.SFE_CED_obj_lateral_pos_left  = FBK_ONE_F;
   ced_output.SFE_CED_obj_long_pos_left     = FBK_ONE_F;

   ced_output.SFE_CED_rear_status            = 1u;
   ced_output.SFE_CED_rear_alert_right       = (uint8_t) 1u;
   ced_output.SFE_CED_rear_alert_left        = (uint8_t) 1u;
   ced_output.SFE_CED_rear_id_right          = 1u;
   ced_output.SFE_CED_rear_id_left           = 1u;
   ced_output.SFE_CED_rear_path_match_right  = 1u;
   ced_output.SFE_CED_rear_path_match_left   = 1u;
   ced_output.SFE_CED_rear_ttc_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttc_left          = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttp_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_ttp_left          = FBK_ONE_F;
   ced_output.SFE_CED_rear_lat_right         = FBK_ONE_F;
   ced_output.SFE_CED_rear_lat_left          = FBK_ONE_F;
   ced_output.SFE_CED_front_status           = 1u;
   ced_output.SFE_CED_front_alert_right      = 1u;
   ced_output.SFE_CED_front_alert_left       = 1u;
   ced_output.SFE_CED_front_id_right         = 1u;
   ced_output.SFE_CED_front_id_left          = 1u;
   ced_output.SFE_CED_front_path_match_right = 1u;
   ced_output.SFE_CED_front_path_match_left  = 1u;
   ced_output.SFE_CED_front_ttc_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_ttc_left         = FBK_ONE_F;
   ced_output.SFE_CED_front_ttp_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_ttp_left         = FBK_ONE_F;
   ced_output.SFE_CED_front_lat_right        = FBK_ONE_F;
   ced_output.SFE_CED_front_lat_left         = FBK_ONE_F;

   /** \action execute reset of sfe output */
   Ced_Reset_Output(&ced_output);

   /** \assert expect output to be set to default */
   EXPECT_EQ(ced_output.SFE_CED_Status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_dir_right, (uint8_t) UNDEF_DIRECTION);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttp_right, CED_INVALID_TIME);
   EXPECT_EQ(ced_output.SFE_CED_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_speed_right, FBK_ZERO_F);
   EXPECT_EQ(ced_output.SFE_CED_obj_type_right, (uint8_t) PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_lateral_pos_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_long_pos_right, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_dir_left, (uint8_t) UNDEF_DIRECTION);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_ttp_left, CED_INVALID_TIME);
   EXPECT_EQ(ced_output.SFE_CED_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_speed_left, FBK_ZERO_F);
   EXPECT_EQ(ced_output.SFE_CED_obj_type_left, (uint8_t) PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_lateral_pos_left, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_obj_long_pos_left, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_rear_status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttp_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_ttp_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_lat_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_rear_lat_left, CED_INVALID_DISTANCE);
   EXPECT_EQ(ced_output.SFE_CED_front_status, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, (uint8_t) CED_NO_ALERT);
   EXPECT_EQ(ced_output.SFE_CED_front_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttp_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_ttp_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_lat_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.SFE_CED_front_lat_left, CED_INVALID_DISTANCE);
}


/**
 * Check updating of sfe pcan signals is done correctly. Here an alert from rear right is triggered.
 * \uts{CSCSA-41762} \sdd{SF-3412} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_right_from_rear_triggered)
{
   /** \arrange set input such that PCAN signals related to rear right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_right                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_unique_id_right                                    = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_output.SFE_CED_ttp_right                                          = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, ced_output.SFE_CED_alert_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_right, ced_output.SFE_CED_id_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttc_right, ced_output.SFE_CED_ttc_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttp_right, ced_output.SFE_CED_ttp_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_lat_right, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_right, 1u);
}

/**
 * Check if path match index is false
 * right side, rear direction
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__match_index_incorrect_right_rear)
{
   /** \arrange set input such that PCAN signals related to rear right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_right                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, ced_output.SFE_CED_alert_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_right, ced_output.SFE_CED_id_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttc_right, ced_output.SFE_CED_ttc_right);
   EXPECT_EQ(ced_output.SFE_CED_rear_lat_right, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_right, 0u);
}


/**
 * Check updating of sfe pcan signals is done correctly. Here no alert at all is triggered, meaning that PCAN signals shall remain
 * on CED_NO_ALERT \uts{CSCSA-70478} \sdd{SF-3412} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_right_from_rear_triggered_no_alert)
{
   /** \arrange set input such that PCAN signals related to rear right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_NO_ALERT;
   ced_output.SFE_CED_dir_right                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;
   ced_input.f_ced_rear_mode                                             = FBK_TRUE;
   ced_input.f_ced_front_mode                                            = FBK_TRUE;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_status, ced_input.f_ced_rear_mode);
   EXPECT_EQ(ced_output.SFE_CED_front_status, ced_input.f_ced_front_mode);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here an alert from front right is triggered.
 * \uts{CSCSA-41763} \sdd{SF-3412} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_right_from_front_triggered)
{
   /** \arrange set input such that PCAN signals related to front right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_right                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_unique_id_right                                    = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_output.SFE_CED_ttp_right                                          = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for front right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, ced_output.SFE_CED_alert_right);
   EXPECT_EQ(ced_output.SFE_CED_front_id_right, ced_output.SFE_CED_id_right);
   EXPECT_EQ(ced_output.SFE_CED_front_id_right, ced_output.SFE_CED_unique_id_right);
   EXPECT_EQ(ced_output.SFE_CED_front_ttc_right, ced_output.SFE_CED_ttc_right);
   EXPECT_EQ(ced_output.SFE_CED_front_ttp_right, ced_output.SFE_CED_ttp_right);
   EXPECT_EQ(ced_output.SFE_CED_front_lat_right, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_right, 1u);
}

/**
 * Check if path match index is false
 * right side, front direction
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__match_index_incorrect_right_front)
{
   /** \arrange set input such that PCAN signals related to front right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_right                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;


   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for front right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, ced_output.SFE_CED_alert_right);
   EXPECT_EQ(ced_output.SFE_CED_front_id_right, ced_output.SFE_CED_id_right);
   EXPECT_EQ(ced_output.SFE_CED_front_ttc_right, ced_output.SFE_CED_ttc_right);
   EXPECT_EQ(ced_output.SFE_CED_front_lat_right, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_right, 0u);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here no alert at all is triggered, meaning that PCAN signals shall remain
 * on CED_NO_ALERT \uts{CSCSA-70479} \sdd{SF-3412} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_right_from_front_triggered_no_alert)
{
   /** \arrange set input such that PCAN signals related to rear right are updated. */
   ced_output.SFE_CED_alert_right                                        = CED_NO_ALERT;
   ced_output.SFE_CED_dir_right                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_right                                           = 1u;
   ced_output.SFE_CED_ttc_right                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;
   ced_input.f_ced_rear_mode                                             = FBK_TRUE;
   ced_input.f_ced_front_mode                                            = FBK_TRUE;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_status, ced_input.f_ced_rear_mode);
   EXPECT_EQ(ced_output.SFE_CED_front_status, ced_input.f_ced_front_mode);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here an alert from rear left is triggered.
 * \uts{CSCSA-41764} \sdd{SF-3412} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_left_from_rear_triggered)
{
   /** \arrange set input such that PCAN signals related to rear left are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_left                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_unique_id_left                                    = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_output.SFE_CED_ttp_left                                          = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear left to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, ced_output.SFE_CED_alert_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_left, ced_output.SFE_CED_id_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_left, ced_output.SFE_CED_unique_id_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttc_left, ced_output.SFE_CED_ttc_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttp_left, ced_output.SFE_CED_ttp_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_lat_left, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_left, 1u);
}

/**
 * Check if path match index is false
 * left side, rear direction
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__match_index_incorrect_left_rear)
{
   /** \arrange set input such that PCAN signals related to rear left are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_left                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear left to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, ced_output.SFE_CED_alert_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_id_left, ced_output.SFE_CED_id_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_ttc_left, ced_output.SFE_CED_ttc_left);
   EXPECT_EQ(ced_output.SFE_CED_rear_lat_left, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_rear_path_match_left, 0u);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here no alert at all is triggered, meaning that PCAN signals shall remain
 * on CED_NO_ALERT \uts{CSCSA-70480} \sdd{SF-3412} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_left_from_rear_triggered_no_alert)
{
   /** \arrange set input such that PCAN signals related to rear right are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_NO_ALERT;
   ced_output.SFE_CED_dir_left                                          = REAR_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;
   ced_input.f_ced_rear_mode                                            = FBK_TRUE;
   ced_input.f_ced_front_mode                                           = FBK_TRUE;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_status, ced_input.f_ced_rear_mode);
   EXPECT_EQ(ced_output.SFE_CED_front_status, ced_input.f_ced_front_mode);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here an alert from front left is triggered.
 * \uts{CSCSA-41765} \sdd{SF-3412} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_left_from_front_triggered)
{
   /** \arrange set input such that PCAN signals related to front left are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_left                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_unique_id_left                                    = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_output.SFE_CED_ttp_left                                          = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for front left to be correct */
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, ced_output.SFE_CED_alert_left);
   EXPECT_EQ(ced_output.SFE_CED_front_id_left, ced_output.SFE_CED_id_left);
   EXPECT_EQ(ced_output.SFE_CED_front_id_left, ced_output.SFE_CED_unique_id_left);
   EXPECT_EQ(ced_output.SFE_CED_front_ttc_left, ced_output.SFE_CED_ttc_left);
   EXPECT_EQ(ced_output.SFE_CED_front_ttp_left, ced_output.SFE_CED_ttp_left);
   EXPECT_EQ(ced_output.SFE_CED_front_lat_left, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_left, 1u);
}

/**
 * Check if path match index is false
 * left side, front direction
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__match_index_incorrect_left_front)
{
   /** \arrange set input such that PCAN signals related to front left are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_output.SFE_CED_dir_left                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for front left to be correct */
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, ced_output.SFE_CED_alert_left);
   EXPECT_EQ(ced_output.SFE_CED_front_id_left, ced_output.SFE_CED_id_left);
   EXPECT_EQ(ced_output.SFE_CED_front_ttc_left, ced_output.SFE_CED_ttc_left);
   EXPECT_EQ(ced_output.SFE_CED_front_lat_left, ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_front_path_match_left, 0u);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here no alert at all is triggered, meaning that PCAN signals shall remain
 * on CED_NO_ALERT \uts{CSCSA-70481} \sdd{SF-3412} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__alert_left_from_front_triggered_no_alert)
{
   /** \arrange set input such that PCAN signals related to front left are updated. */
   ced_output.SFE_CED_alert_left                                        = CED_NO_ALERT;
   ced_output.SFE_CED_dir_left                                          = FRONT_DIRECTION;
   ced_output.SFE_CED_id_left                                           = 1u;
   ced_output.SFE_CED_ttc_left                                          = 1.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT]  = 4u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 1.0f;
   ced_input.f_ced_rear_mode                                            = FBK_TRUE;
   ced_input.f_ced_front_mode                                           = FBK_TRUE;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect mapping for rear right to be correct */
   EXPECT_EQ(ced_output.SFE_CED_rear_status, ced_input.f_ced_rear_mode);
   EXPECT_EQ(ced_output.SFE_CED_front_status, ced_input.f_ced_front_mode);
}

/**
 * Check updating of sfe pcan signals is done correctly. Here no alert at all is triggered, meaning that PCAN signals shall remain
 * on CED_NO_ALERT. \uts{CSCSA-41766} \sdd{SF-3412} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Pcan_Signals__no_alert_triggered)
{
   /** \arrange set input such that PCAN signals remain on default. */
   ced_output.SFE_CED_alert_left        = CED_NO_ALERT;
   ced_output.SFE_CED_alert_right       = CED_NO_ALERT;
   ced_output.SFE_CED_front_alert_left  = 0u;
   ced_output.SFE_CED_rear_alert_left   = 0u;
   ced_output.SFE_CED_front_alert_right = 0u;
   ced_output.SFE_CED_rear_alert_right  = 0u;

   /** \action execute mapping routine for PCAN signals */
   Ced_Update_Pcan_Signals(&ced_output, &ced_instance.core_output, &ced_input);

   /** \assert expect no pcan signal set to alert */
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, 0u);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, 0u);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, 0u);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, 0u);
}


/**
 * Check CED output setting function for signals which are not PCAN signals. Here alerts are set on both side by core
 * \uts{CSCSA-41767} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Output__alert_on_both_sides)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]        = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]         = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_1;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.SFE_CED_dir_right, REAR_DIRECTION);
   EXPECT_EQ(ced_output.SFE_CED_ttc_right, ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_id_right, ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_right, ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_alert_right, ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_right, object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed);

   EXPECT_EQ(ced_output.SFE_CED_dir_left, FRONT_DIRECTION);
   EXPECT_EQ(ced_output.SFE_CED_ttc_left, ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_id_left, ced_instance.core_output.ced_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_left, ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_alert_left, ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_left, object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed);
}

/**
 * Check CED output setting function for signals which are not PCAN signals. Here alerts are set on both side by core
 * \uts{CSCSA-70482} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Output__alert_on_undefined)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_UNDEFINED;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]        = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_UNDEFINED;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]         = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_1;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.SFE_CED_dir_right, UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.SFE_CED_ttc_right, ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_id_right, ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_right, ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_alert_right, ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_right, object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed);

   EXPECT_EQ(ced_output.SFE_CED_dir_left, UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.SFE_CED_ttc_left, ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_id_left, ced_instance.core_output.ced_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_unique_id_left, ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_alert_left, ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_left, object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed);
}


/**
 * Check CED output setting function for signals which are not PCAN signals. Here no alert is set by core. Thus default values are
 * expected. \uts{CSCSA-41768} \sdd{SF-3410} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Output__no_alert)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT] = 255u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]   = -1.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]  = 255u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]    = -1.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_NO_ALERT;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect default values to be set for tracker signals */
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_right, 0.0f);
   EXPECT_EQ(ced_output.SFE_CED_obj_speed_left, 0.0f);
}

/**
 * Check customer CED status when CED disbaled.
 * \uts{CSCSA-112890} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Update_Output__sfe_ced_status)
{
   /** \arrange set CED as disabled */
   ced_input.f_ced_enable = FBK_FALSE;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.SFE_CED_Status, FBK_ZERO_UINT);
}

/**
 * Test superior function. Here no alert has been set by core and thus no alert shall is expected at feature output.
 * \uts{CSCSA-41769} \sdd{SF-3397} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__no_alert)
{
   /** \arrange set ced core output to default. */
   for (uint8_t side_index = 0u; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      ced_instance.core_output.ced_id[side_index]                       = FBK_ZERO_UINT;
      ced_instance.core_output.ced_index[side_index]                    = FBK_ZERO_UINT;
      ced_instance.core_output.ced_alert[side_index]                    = CED_NO_ALERT;
      ced_instance.core_output.ced_ttc[side_index]                      = CED_INVALID_TIME;
      ced_instance.core_output.ced_object_predicted_lat_pos[side_index] = FBK_ZERO_F;
      ced_instance.core_output.ced_object_direction[side_index]         = FBK_SIDE_UNDEFINED;
      ced_instance.core_output.ced_object_path_match_index[side_index]  = PT_DEFAULT_MATCH_INDEX;
   }

   /** \action Call main CED post run function. */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert expect no alert on sfe output or pcan signals */
   EXPECT_EQ(ced_output.SFE_CED_alert_right, 0u);
   EXPECT_EQ(ced_output.SFE_CED_alert_left, 0u);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_left, 0u);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_left, 0u);
   EXPECT_EQ(ced_output.SFE_CED_front_alert_right, 0u);
   EXPECT_EQ(ced_output.SFE_CED_rear_alert_right, 0u);
}

/**
 * Test superior function. Here no alert has been set by core and thus no alert shall is expected at feature output.
 * \uts{CSCSA-70483} \sdd{SF-3397} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__no_alert_active)
{
   /** \arrange set ced core output to default. */
   *current_state = CED_STATE_ACTIVE;
   for (uint8_t side_index = 0u; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      ced_instance.core_output.ced_id[side_index]                       = FBK_ZERO_UINT;
      ced_instance.core_output.ced_index[side_index]                    = FBK_ZERO_UINT;
      ced_instance.core_output.ced_alert[side_index]                    = CED_NO_ALERT;
      ced_instance.core_output.ced_ttc[side_index]                      = CED_INVALID_TIME;
      ced_instance.core_output.ced_object_predicted_lat_pos[side_index] = FBK_ZERO_F;
      ced_instance.core_output.ced_object_direction[side_index]         = FBK_SIDE_UNDEFINED;
      ced_instance.core_output.ced_object_path_match_index[side_index]  = PT_DEFAULT_MATCH_INDEX;
   }

   /** \action Call main CED post run function. */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert expect no alert on sfe output or pcan signals */
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_front_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_front_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_rear_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_rear_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_mirror_led_right, CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_mirror_led_left, CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_front_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_front_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_rear_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_rear_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_stop_automatic_opening_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_stop_automatic_opening_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_front_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_front_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_rear_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_rear_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}

/**
 * Check that mapping of travel direction in ced post run is done correctly for front direction.
 * \uts{CSCSA-41770} \sdd{SF-3453} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Travel_Direction_To_Sfe__works_properly_for_front_direction)
{
   /** \arrange Set core travel direction to front direction. */
   uint8_t ced_travel_direction = FBK_SIDE_FRONT;
   Ced_Target_Travel_Direction_T sfe_travel_direction;

   /** \action Execute travel direction mapping to SFE specific travel directions. */
   sfe_travel_direction = Ced_Map_Travel_Direction_To_Sfe(ced_travel_direction);

   /** \assert Expect travel direction of SFE to be also front direction */
   EXPECT_EQ(sfe_travel_direction, FRONT_DIRECTION);
}

/**
 * Check that mapping of travel direction in ced post run is done correctly for rear direction.
 * \uts{CSCSA-41771} \sdd{SF-3453} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Travel_Direction_To_Sfe__works_properly_for_rear_direction)
{
   /** \arrange Set core travel direction to rear direction. */
   uint8_t ced_travel_direction = FBK_SIDE_REAR;
   Ced_Target_Travel_Direction_T sfe_travel_direction;

   /** \action Execute travel direction mapping to SFE specific travel directions. */
   sfe_travel_direction = Ced_Map_Travel_Direction_To_Sfe(ced_travel_direction);

   /** \assert Expect travel direction of SFE to be also rear direction */
   EXPECT_EQ(sfe_travel_direction, REAR_DIRECTION);
}

/**
 * Check that mapping of travel direction in ced post run is done correctly for undefined direction.
 * \uts{CSCSA-41772} \sdd{SF-3453} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Travel_Direction_To_Sfe__works_properly_for_undefined_direction)
{
   /** \arrange Set core travel direction to undefined direction. */
   uint8_t ced_travel_direction = FBK_SIDE_UNDEFINED;
   Ced_Target_Travel_Direction_T sfe_travel_direction;

   /** \action Execute travel direction mapping to SFE specific travel directions. */
   sfe_travel_direction = Ced_Map_Travel_Direction_To_Sfe(ced_travel_direction);

   /** \assert Expect travel direction of SFE to be also undefined direction */
   EXPECT_EQ(sfe_travel_direction, UNDEF_DIRECTION);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_active_all_types works.
 * \uts{CSCSA-41773} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_active_light_warning_on_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_1.
 * \uts{CSCSA-70484} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface light warning on flashing_1. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_2.
 * \uts{CSCSA-70485} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface light warning on flashing_2. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_3.
 * \uts{CSCSA-70486} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_active_light_warning_on_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface light warning on flashing_3. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_not_active_all_types works.
 * \uts{CSCSA-41774} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_not_active_light_warning_on_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_not_active_light_warning_on_flashing_1.
 * \uts{CSCSA-70487} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_not_active_light_warning_on_no_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_not_active_light_warning_on_flashing_2.
 * \uts{CSCSA-70488} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_not_active_light_warning_on_no_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__information_not_active_light_warning_on_flashing_3.
 * \uts{CSCSA-70489} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__information_not_active_light_warning_on_no_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_information_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_active_all_types works.
 * \uts{CSCSA-41775} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_active_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_active_no_flashing_1 works.
 * \uts{CSCSA-70490} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_active_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_active_no_flashing_2 works.
 * \uts{CSCSA-70491} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_active_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_active_no_flashing_3 works.
 * \uts{CSCSA-70492} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_active_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_not_active_all_types works.
 * \uts{CSCSA-41776} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_not_active_warning_on_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_NO_ALERT;

   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_not_active_warning_on_no_flashing_1 works.
 * \uts{CSCSA-70493} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_not_active_warning_on_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_NO_ALERT;

   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__warning_not_active_warning_on_no_flashing_2 works.
 * \uts{CSCSA-70494} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_not_active_warning_on_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_NO_ALERT;

   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   /** \action call Ced_Update_Output */
   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_active_all_types works.
 * \uts{CSCSA-41777} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__warning_not_active_warning_on_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]             = CED_NO_ALERT;

   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_FALSE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_active_no_flashing works.
 * \uts{CSCSA-70495} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_active_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for no flashing. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_active_flashing_1 works.
 * \uts{CSCSA-70496} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_active_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for warning on flashing 1. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_active_flashing_2 works.
 * \uts{CSCSA-70497} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_active_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning on flashing 2. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_active_flashing_3 works.
 * \uts{CSCSA-70498} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_active_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning on flashing 3. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT],
             CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_not_active_no_flashing works.
 * \uts{CSCSA-41778} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_not_active_no_flashing)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning not active for no
    * flashing. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_1 works.
 * \uts{CSCSA-70499} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_1)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning not active for
    * type 1. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_2 works.
 * \uts{CSCSA-70500} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_2)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning not active for
    * type 2. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_3 works.
 * \uts{CSCSA-70501} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Mirror_Led__acute_warning_not_active_flashing_3)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for acute warning not active for
    * type 3. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                            = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]                 = FBK_SIDE_REAR;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                              = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                               = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                            = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                             = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]                  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                               = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                                = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                             = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                        = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed         = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed          = 20.0f;

   ced_input.ced_coding_parameters.c_sfe_mirror_light_acute_warning_type = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_3;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT], CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that Ced_Verify_Optical_Element__information_active_Right = Front_ and_Left = rear warning.
 * \uts{CSCSA-41779} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__information_active_right_front_and_left_rear)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for infromation active with Right =
    * Front warning and Left = rear warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_REAR;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_FRONT;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], FRONT_DIRECTION);
}

/**
 * Check that Ced_Verify_Optical_Element__information_active_Right = Rear warning and Left = Front warning.
 * \uts{CSCSA-70502} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__information_active_right_rear_and_left_front)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for infromation active with Right =
    * Rear warning and Left = Front warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], REAR_DIRECTION);
}

/**
 * Check that Ced_Verify_Optical_Element__information_not_active_Right = Front_ and_Rear = left warning.
 * \uts{CSCSA-41780} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__information_not_active_right_front_and_left_rear)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for infromation active with Right =
    * Front warning and Rear = left warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]     = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]        = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]     = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]      = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]        = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]         = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]      = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value = SETTING_SAFE_EXIT_VALUE_ACTIVATED;

   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_REAR;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_FRONT;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], UNDEF_DIRECTION);
}

/**
 * Check that Ced_Verify_Optical_Element__information_not_active_Right = Rear warning and Left = Front warning.
 * \uts{CSCSA-70503} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__information_not_active_right_rear_and_left_front)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for infromation active with Right =
    * Rear warning and Left = Front warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], UNDEF_DIRECTION);
}

/**
 * Check that Ced_Verify_Optical_Element__warning_active__Right = Front_ and_Rear = left warning works.
 * \uts{CSCSA-41781} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__warning_active_right_front_and_left_rear)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for Right = Front warning and Rear =
    * left warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_REAR;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_FRONT;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], FRONT_DIRECTION);
}

/**
 * Check that Ced_Verify_Optical_Element__warning_active__Right = rear_ and_Left = front warning works.
 * \uts{CSCSA-70504} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Optical_Element__warning_active_right_rear_and_left_front)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for Right = Rear warning and Left =
    * Front warning. */
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]  = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = FBK_SIDE_REAR;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT], REAR_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT], FRONT_DIRECTION);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT], REAR_DIRECTION);
}


/**
 * Check that Ced_Verify_Interior_Light__information_active_LV1_right_warning_left_no_warning works.
 * \uts{CSCSA-41782} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__information_active_LV1_right_warning_left_no_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for LV1_right_warning_left_no_warning
    */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT],
             CED_DOOR_WARNING_LEVEL_1);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV1_right_no_warning_left_warning works.
 * \uts{CSCSA-70505} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__information_active_LV1_right_no_warning_left_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for LV1_right_no_warning_left_warning
    */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT], CED_NO_ALERT);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV2_right_warning_left_no_warning works.
 * \uts{CSCSA-70506} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__information_active_LV2_right_warning_left_no_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for LV2_right_warning_left_no_warning
    */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_NO_ALERT;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT],
             CED_DOOR_WARNING_LEVEL_2);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV2_right_no_warning_left_warning works.
 * \uts{CSCSA-70507} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__information_active_LV2_right_no_warning_left_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for LV2_right_no_warning_left_warning
    */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_2;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT], CED_NO_ALERT);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV1_right_warning_left_no_warning works.
 * \uts{CSCSA-41783} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__acute_warning_active_LV1_right_warning_left_no_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for
    * LV1_right_warning_left_no_warning. */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]           = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                      = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                        = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                         = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]            = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                       = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                         = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                          = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                  = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed   = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed    = 20.0f;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information   = CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                      = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                       = CED_NO_ALERT;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_acute_warning = CED_DOOR_WARNING_LEVEL_1;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT],
             CED_DOOR_WARNING_LEVEL_1);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV1_right_no_warning_left_warning works.
 * \uts{CSCSA-70508} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__acute_warning_active_LV1_right_no_warning_left_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for
    * LV1_right_no_warning_left_warning. */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_NO_WARNING;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                      = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                       = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_acute_warning = CED_DOOR_WARNING_LEVEL_1;
   /** \action Call Ced_Update_Output */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT],
             CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT], CED_NO_ALERT);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV2_right_warning_left_no_warning works.
 * \uts{CSCSA-70509} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__acute_warning_active_LV2_right_warning_left_no_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for
    * LV2_right_warning_left_no_warning. */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_NO_WARNING;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                      = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                       = CED_NO_ALERT;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_acute_warning = CED_DOOR_WARNING_LEVEL_2;

   /** \action Call Ced_Update_Output */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT],
             CED_DOOR_WARNING_LEVEL_2);
}

/**
 * Check that Ced_Verify_Interior_Light__information_active_LV2_right_no_warning_left_warning works.
 * \uts{CSCSA-70510} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__acute_warning_active_LV2_right_no_warning_left_warning)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface for
    * LV2_right_no_warning_left_warning. */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                    = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                     = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                       = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                        = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed  = 20.0f;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information = CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                     = CED_ALERT_ACTIVE_LEVEL_2;

   /** \action Call Ced_Update_Output */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT],
             CED_DOOR_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT], CED_NO_ALERT);
}

/**
 * Check that Ced_Verify_Interior_Light__no_alert_all_types works.
 * \uts{CSCSA-41784} \sdd{SF-3410} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Verify_Interior_Light__no_alert_all_types)
{
   /** \arrange set ced core output and tracker signals consumed at customer output interface. */
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]           = FBK_SIDE_FRONT;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                      = 0u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                        = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                         = 1u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]            = FBK_SIDE_REAR;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                       = 1u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                         = 2.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                          = 2u;
   ced_input.bmw_boardnet_signals.setting_safe_exit.value                  = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]].speed   = 15.0f;
   object_data[ced_instance.core_output.ced_index[FBK_SIDE_LEFT]].speed    = 20.0f;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_information   = CED_DOOR_WARNING_LEVEL_1;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_warning       = CED_DOOR_WARNING_LEVEL_1;
   ced_input.ced_coding_parameters.c_sfe_interior_light_type_acute_warning = CED_DOOR_WARNING_LEVEL_1;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_NO_ALERT;

   /** \action execute output setter for ced */
   Ced_Update_Output(&ced_output, &ced_input, &ced_instance.core_input, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT], CED_NO_ALERT);
}

/**
 * Check that Ced_Reset_Bus_Signals_Output works.
 * \uts{CSCSA-70511} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Outputs_If_Not_Active_State__Initialising_bus_signals_required_for_State_Machine)
{
   /** \arrange set ced output bus signals to something different than default. */
   ced_output_bus_signals.ced_output_warning_optical_front_right        = ACTIVE_APPROACH_FRONT;
   ced_output_bus_signals.ced_output_warning_optical_front_left         = ACTIVE_APPROACH_FRONT;
   ced_output_bus_signals.ced_output_warning_optical_rear_right         = ACTIVE_APPROACH_REAR;
   ced_output_bus_signals.ced_output_warning_optical_rear_left          = ACTIVE_APPROACH_REAR;
   ced_output_bus_signals.ced_output_warning_mirror_led_right           = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;
   ced_output_bus_signals.ced_output_warning_mirror_led_left            = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;
   ced_output_bus_signals.ced_output_warning_ambient_lights_front_right = AMBIENT_LIGHTS_WARNING_LEVEL_1;
   ced_output_bus_signals.ced_output_warning_ambient_lights_front_left  = AMBIENT_LIGHTS_WARNING_LEVEL_1;
   ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right  = AMBIENT_LIGHTS_WARNING_LEVEL_1;
   ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left   = AMBIENT_LIGHTS_WARNING_LEVEL_1;
   ced_output_bus_signals.ced_output_warning_acoustic_front_right       = ACOUSTIC_WARNING;
   ced_output_bus_signals.ced_output_warning_acoustic_front_left        = ACOUSTIC_WARNING;
   ced_output_bus_signals.ced_output_warning_acoustic_rear_right        = ACOUSTIC_WARNING;
   ced_output_bus_signals.ced_output_warning_acoustic_rear_left         = ACOUSTIC_WARNING;
   ced_output_bus_signals.ced_output_door_stop_automatic_opening_right  = FBK_TRUE;
   ced_output_bus_signals.ced_output_door_stop_automatic_opening_left   = FBK_TRUE;
   ced_output_bus_signals.ced_output_door_lock_electronic_front_right   = FBK_TRUE;
   ced_output_bus_signals.ced_output_door_lock_electronic_front_left    = FBK_TRUE;
   ced_output_bus_signals.ced_output_door_lock_electronic_rear_right    = FBK_TRUE;
   ced_output_bus_signals.ced_output_door_lock_electronic_rear_left     = FBK_TRUE;
   ced_output_bus_signals.ced_output_display_door                       = DISPLAY_DOOR_WARNING;

   /** \action execute reset of sfe output */
   Ced_Set_Outputs_If_Not_Active_State(&ced_output_bus_signals);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_front_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_front_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_rear_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_optical_rear_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_mirror_led_right, CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_mirror_led_left, CED_MIRROR_LIGHT_WARNING_OFF);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_front_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_front_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_rear_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_warning_acoustic_rear_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_stop_automatic_opening_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_stop_automatic_opening_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_front_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_front_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_rear_right, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_door_lock_electronic_rear_left, FBK_FALSE);
   EXPECT_EQ(ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}


/**
 * Check that Ced_Set_Outputs_In_Active_State works for Information Front Left.
 * \uts{CSCSA-70512} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Outputs_In_Active_State__Front_left)
{
   /** \arrange ced core output and ced out to get front Left Infromation. */
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT]  = FRONT_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT]   = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT] = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT] = CED_DOOR_WARNING_LEVEL_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]  = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT] = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;


   /** \action execute to get Information */
   Ced_Set_Outputs_In_Active_State(&ced_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_left, ACTIVE_APPROACH_FRONT);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_mirror_led_left,
             ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_acoustic_front_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_stop_automatic_opening_left, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_lock_electronic_front_left, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}

/**
 * Check that Ced_Set_Outputs_In_Active_State works for Information Rear Left.
 * \uts{CSCSA-70513} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Outputs_In_Active_State__Rear_left)
{
   /** \arrange ced core output and ced out to get rear Left Infromation. */
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT]   = REAR_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT] = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT]  = CED_DOOR_WARNING_LEVEL_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT]   = CED_DOOR_WARNING_LEVEL_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]  = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT] = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute to get Information */
   Ced_Set_Outputs_In_Active_State(&ced_output);

   /** \assert expect that mapping is correctly set */

   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_left, ACTIVE_APPROACH_REAR);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, CED_DOOR_WARNING_LEVEL_1);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_mirror_led_left,
             ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_acoustic_rear_left, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_stop_automatic_opening_left, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_lock_electronic_rear_left, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}

/**
 * Check that Ced_Set_Outputs_In_Active_State works for Information Front Right.
 * \uts{CSCSA-70514} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Outputs_In_Active_State__Front_right)
{
   /** \arrange ced core output and ced out to get front right Infromation. */
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT] = FRONT_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT] = CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT] =
      CED_DOOR_WARNING_LEVEL_NO_WARNING;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]  = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT] = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute to get Information */
   Ced_Set_Outputs_In_Active_State(&ced_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_right, ACTIVE_APPROACH_FRONT);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_mirror_led_right,
             ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_acoustic_front_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_stop_automatic_opening_right, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_lock_electronic_front_right, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}

/**
 * Check that Ced_Set_Outputs_In_Active_State works for Information Rear Right.
 * \uts{CSCSA-70515} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Outputs_In_Active_State__Rear_right)
{
   /** \arrange ced core output and ced out to get rear right Infromation. */
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_LEFT]  = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_LEFT]   = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_FRONT_RIGHT] = UNDEF_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_optical[CED_DOOR_POSITION_REAR_RIGHT]  = REAR_DIRECTION;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_LEFT]  = CED_DOOR_WARNING_LEVEL_2;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_LEFT]   = CED_DOOR_WARNING_LEVEL_2;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_FRONT_RIGHT] = CED_DOOR_WARNING_LEVEL_2;
   ced_output.ced_output_warning_indicators.ced_output_warning_ambient[CED_DOOR_POSITION_REAR_RIGHT]  = CED_DOOR_WARNING_LEVEL_2;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_LEFT]  = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;
   ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT] = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;

   /** \action execute to get Information */
   Ced_Set_Outputs_In_Active_State(&ced_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_left, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_front_right, NOT_ACTIVE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_optical_rear_right, ACTIVE_APPROACH_REAR);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_left, AMBIENT_LIGHTS_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_front_right, AMBIENT_LIGHTS_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_left, AMBIENT_LIGHTS_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_ambient_lights_rear_right, AMBIENT_LIGHTS_WARNING_LEVEL_2);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_mirror_led_right,
             ced_output.ced_output_warning_indicators.ced_output_warning_mirror[CED_SIDE_RIGHT]);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_warning_acoustic_rear_right, ACOUSTIC_NO_WARNING);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_stop_automatic_opening_right, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_door_lock_electronic_rear_right, FBK_FALSE);
   EXPECT_EQ(ced_output.ced_output_bus_signals.ced_output_display_door, DISPLAY_DOOR_NO_WARNING);
}

/**
 * Check that Ced_Set_Alert_State works for TTC Right.
 * \uts{CSCSA-70516} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Alert_State__ttc_right)
{
   /** \arrange ced core output and ced out to check the alert. */
   ced_output.SFE_CED_ttc_right                       = -2.0;
   ced_output.SFE_CED_ttc_left                        = 2.0;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_NO_ALERT;

   /** \action execute to get Information */
   Ced_Set_Alert_State(&ced_output, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.SFE_CED_alert_left, ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]);
}

/**
 * Check that Ced_Set_Alert_State works for TTC Left.
 * \uts{CSCSA-70517} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Alert_State__ttc_left)
{
   /** \arrange ced core output and ced out to check the alert. */
   ced_output.SFE_CED_ttc_right                       = 2.0;
   ced_output.SFE_CED_ttc_left                        = -2.0;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_NO_ALERT;

   /** \action execute to get Information */
   Ced_Set_Alert_State(&ced_output, &ced_instance.core_output);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(ced_output.SFE_CED_alert_right, ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]);
}
