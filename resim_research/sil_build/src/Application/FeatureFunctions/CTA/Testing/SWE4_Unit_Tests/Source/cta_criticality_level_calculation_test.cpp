/**
 * @file cta_criticality_level_calculation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_criticality_level_calculation.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41877}
 */

#include "cta_criticality_level_calculation_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_criticality_level_calculation.c"
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the front right of the host
 * parallel to the butterfly zone for RCTA. It is expected that the object is not included in the reduced field of view, thus
 * returning of true is expected. \uts{CSCSA-41878} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_front_not_in_reduced_fov_yet_in_rcta_case)
{
   /** \arrange objects attributes and reduced fiel of view border */
   boolean_T res;
   Cta_Mode_T mode = CTA_MODE_REAR;

   cals.k_cta_sensor_fov_border[mode] = 0.785398f; /*approximately 45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point = Create_2d_Vector_Coordinates(5.0f, 5.0f);
   object.attributes->approach_side                            = FBK_SIDE_RIGHT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is not incldued in the reduced field of view */
   EXPECT_TRUE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the front right of the host onto
 * the border of the reduced field of view for RCTA. It is expected that the object is not included in the reduced field of view,
 * thus returning of true is expected. \uts{CSCSA-41879} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_front_close_to_zone_but_not_in_zone_yet)
{
   /** \arrange object directly located on the reduced field of view border */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode = CTA_MODE_REAR;

   cals.k_cta_sensor_fov_border[mode] = 0.785398f; /*approximately 45 deg*/

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(EPSILON, 0.5f * p_vehicle_data->host_width + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is not incldued in the reduced field of view */
   EXPECT_TRUE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the front right of the host a
 * little below of the reduced field of view for RCTA. It is expected that the object is included in the reduced field of view,
 * thus returning of false is expected. \uts{CSCSA-41880} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_front_passed_the_line_by_epsilon_longitudinal)
{
   /** \arrange object moving slightly below the defined border of reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode = CTA_MODE_REAR;

   cals.k_cta_sensor_fov_border[mode] = 0.785398f; /*approximately 45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-EPSILON, 0.5f * p_vehicle_data->host_width + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert object is within the reduced field of view */
   EXPECT_FALSE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the front left of the host above
 * the reduced field of view for RCTA. It is expected that the object is not included in the reduced field of view, thus returning
 * of true is expected. \uts{CSCSA-41881} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_front_close_to_zone_approach_from_left_not_in_zone_yet)
{
   /** \arrange object from front left outside the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode = CTA_MODE_REAR;

   cals.k_cta_sensor_fov_border[mode] = -0.785398f; /*approximately -45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(EPSILON, -(0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is not includued in the reduced field of view */
   EXPECT_TRUE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the front left of the host
 * reduced field of view for RCTA. It is expected that the object is included in the reduced field of view, thus returning of false
 * is expected. \uts{CSCSA-41882} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_front_close_to_zone_approach_from_left_entered_zone)
{
   /** \arrange object from front left inside the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode                    = CTA_MODE_REAR;
   cals.k_cta_sensor_fov_border[mode] = -0.785398f; /*approximately -45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-EPSILON, -(0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is included in the reduced field of view */
   EXPECT_FALSE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the rear left of the host reduced
 * field of view for FCTA. It is expected that the object is not included in the reduced field of view, thus returning of true is
 * expected. \uts{CSCSA-41883} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_rear_close_to_zone_approach_from_left_outside_fov_fcta)
{
   /** \arrange object approaching from the rear left of host outside of the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode                    = CTA_MODE_FRONT;
   cals.k_cta_sensor_fov_border[mode] = -0.785398f; /*approximately -45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length - EPSILON, -(0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is not incldued in the reduced field of view */
   EXPECT_TRUE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the rear left of the host reduced
 * field of view for FCTA. It is expected that the object is included in the reduced field of view, thus returning of false is
 * expected. \uts{CSCSA-41884} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_rear_approach_from_left_in_fov_fcta)
{
   /** \arrange object approaching from the rear left of host inside the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode                    = CTA_MODE_FRONT;
   cals.k_cta_sensor_fov_border[mode] = -0.785398f; /*approximately -45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length + EPSILON, -(0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_LEFT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is included in the reduced field of view */
   EXPECT_FALSE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the rear right of the host
 * reduced field of view for FCTA. It is expected that the object is included in the reduced field of view, thus returning of false
 * is expected. \uts{CSCSA-41885} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_rear_approach_from_right_in_fov_fcta)
{
   /** \arrange object approaching from the rear right of host inside the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode = CTA_MODE_FRONT;

   cals.k_cta_sensor_fov_border[mode] = 0.785398f; /*approximately 45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length + EPSILON, (0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_RIGHT;


   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is included in the reduced field of view */
   EXPECT_FALSE(res);
}

/**
 * Checks whether object is outside the sensor field of view. Here the object is approaching from the rear right of the host
 * reduced field of view for FCTA. It is expected that the object is outside reduced field of view, thus returning of true is
 * expected. \uts{CSCSA-41886} \sdd{SF-3784} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Obj_Outside_Of_Sensor_Fov__obj_coming_from_rear_approach_from_right_not_in_fov_fcta)
{
   /** \arrange object approaching from the rear right of host inside the reduced field of view */
   boolean_T res;
   float32_T lateral_ref_comp;
   Cta_Mode_T mode                    = CTA_MODE_FRONT;
   cals.k_cta_sensor_fov_border[mode] = 0.785398f; /*approximately 45 deg*/
   p_vehicle_data->host_length        = 5.0f;
   p_vehicle_data->host_width         = 2.5f;

   lateral_ref_comp = p_vehicle_data->host_length * Fast_Tan(cals.k_cta_sensor_fov_border[mode]);

   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length - EPSILON, (0.5f * p_vehicle_data->host_width) + lateral_ref_comp);
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   /** \action executes function to test */
   res = Cta_Is_Obj_Outside_Of_Sensor_Fov(object.attributes, &cals, p_vehicle_data, mode);

   /** \assert Expect that the object is not incldued in the reduced field of view */
   EXPECT_TRUE(res);
}

/**
 * Tests Setting of criticality level threshold qualification data. Here setting of additional qualification cycles is tested. A
 * merge of a previously warned object occured. It is expected, that the object with a new id does not need to qualify again.
 * \uts{CSCSA-41887} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__merge_of_warned_object_occured)
{
   /** \arrange setup previously warned object */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_2;
   Cta_Mode_T mode     = CTA_MODE_REAR;

   /*Target properties*/
   object.attributes->approach_side                                                     = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info                                                   = NULL;
   object.tracker_data.f_merge_occured                                                  = FBK_TRUE;
   object.tracker_data.id_merged_obj                                                    = 1u;
   cta_persistent.previous_most_critical_obj_id[mode][object.attributes->approach_side] = object.tracker_data.id_merged_obj;
   cta_persistent.previous_crit_level[mode][object.attributes->approach_side]           = CTA_CRIT_LEVEL_2;
   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;


   /** \action executes function to test */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert expect that the qualification cycles are set to 0 and that the object is not supressed */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], CTA_COUNTER_MAX);
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
   EXPECT_EQ(crit_level_cals.mature_cycles, FBK_ZERO_UINT);
}


/**
 * Tests setting of counter logic for criticality level. Here a merge has occured but the criticality is less after the merge so
 * that the supression counter is not set to its maximum in that case. \uts{CSCSA-41888} \sdd{SF-3786}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__merge_of_warned_object_occured_but_criticality_is_less)
{
   /** \arrange merge scenario in which the criticality is reduced */
   Cta_Persistent_T cta_persistent{};
   Cta_Mode_T mode     = CTA_MODE_REAR;
   uint8_t level_index = CTA_CRIT_LEVEL_2;

   level_index = CTA_CRIT_LEVEL_2;

   /*Target properties*/
   object.attributes->approach_side                                                     = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info                                                   = NULL;
   object.tracker_data.f_merge_occured                                                  = FBK_TRUE;
   object.tracker_data.id_merged_obj                                                    = 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index]                 = 1;
   cta_persistent.previous_most_critical_obj_id[mode][object.attributes->approach_side] = object.tracker_data.id_merged_obj;
   cta_persistent.previous_crit_level[mode][object.attributes->approach_side]           = CTA_CRIT_LEVEL_1;
   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert suppression counter for the level shall be the same as before */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 1u);
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
   EXPECT_EQ(crit_level_cals.mature_cycles, FBK_ZERO_UINT);
}


/**
 * Tests setting of counter logic for criticality level. Here a merge has occured of objects occured. However it was not the most
 * critical object which was merged. Thus it is expected that the suppression counter as well as other thresholds are not
 * deactivated for this object. \uts{CSCSA-41889} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__not_the_most_critical_object_has_been_merged)
{
   /** \arrange merge scenario in which not the most critical object has been merged */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_2;
   Cta_Mode_T mode     = CTA_MODE_REAR;

   /*Target properties*/
   object.attributes->approach_side                                                     = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info                                                   = NULL;
   object.tracker_data.f_merge_occured                                                  = FBK_TRUE;
   object.tracker_data.id_merged_obj                                                    = 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index]                 = 1;
   cta_persistent.previous_most_critical_obj_id[mode][object.attributes->approach_side] = object.tracker_data.id_merged_obj + 1u;
   cta_persistent.previous_crit_level[mode][object.attributes->approach_side]           = CTA_CRIT_LEVEL_1;
   object.persistent->prev_cycle_crit_level[mode]                                       = CTA_CRIT_LEVEL_1;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   /*Calibrations such that each customer calibration is passing*/
   cals.k_cta_min_mature_cycles_level_qualifiction = 100u;
   object.attributes->ttc                          = 0.9f * cals.k_cta_min_ttc_additional_mature_qualification;

   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 1u);
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests setting of counter logic for criticality level. Here no path tracking information is available for an object which has not
 * qualified for a criticality level. \uts{CSCSA-41890} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Set_Additional_Thres_Data__apply_additional_cycles_since_no_level_reached_and_no_pt_information)
{
   /** \arrange scenario in which no path information is available */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;
   /*Target properties*/

   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = NULL;
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length                                           = 4.0f;
   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_min_age_obj_outside_sensor_fov          = 0;
   cals.k_cta_sensor_fov_border[mode]                 = 0.0f;
   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles,
             cals.k_cta_min_mature_cycles_level_qualifiction + cals.k_cta_additional_qualification_mature_cycles);
}


/**
 * Tests setting of counter logic for criticality level. Here no path tracking information is available for an object which has not
 * qualified for a criticality level. TTC condition shall not be fulfilled here. \uts{CSCSA-41891} \sdd{SF-3786}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Set_Additional_Thres_Data__apply_min_age_since_no_level_reached_and_no_pt_information_ttc_condition_not_fulfilled)
{
   /** \arrange scenario in which no path information is available */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = NULL;
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_sensor_fov_border[mode]                 = 0.0f;
   cals.k_cta_addit_mature_cycles_outside_sensor_fov  = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov          = 0;
   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = object.attributes->ttc + EPSILON;

   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests setting of counter logic for criticality level. Here path tracking information is available but the path is qualified as
 * longitudinal path. \uts{CSCSA-41892} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Set_Additional_Thres_Data__apply_additional_cycles_for_since_path_direction_is_longitudinal)
{
   /** \arrange scenario so that additional cycles for longitudinal paths are applied */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LONG_BACKWARD;

   level_index                        = CTA_CRIT_LEVEL_NONE;
   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov  = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov          = 0;
   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;

   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles,
             cals.k_cta_min_mature_cycles_level_qualifiction + cals.k_cta_additional_qualification_mature_cycles);
}


/**
 * Tests setting of counter logic for criticality level. Here path direction is set left
 * \uts{CSCSA-85511} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__direction_left)
{
   /** \arrange scenario so that additional cycles for longitudinal paths are applied */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LAT_LEFT;

   level_index                        = CTA_CRIT_LEVEL_NONE;
   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov         = 0;

   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
}


/**
 * Tests setting of counter logic for criticality level. Here path direction is set right
 * \uts{CSCSA-85512} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__direction_right)
{
   /** \arrange scenario so that additional cycles for longitudinal paths are applied */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LAT_RIGHT;

   level_index                        = CTA_CRIT_LEVEL_NONE;
   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov         = 0;

   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values */
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
}

/**
 * Tests setting of counter logic for criticality level. Here path tracking information is available but the path is qualified as
 * longitudinal path. TTC condition shall not be fulfilled \uts{CSCSA-41893} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Set_Additional_Thres_Data__apply_additional_cycles_for_since_path_direction_is_longitudinal_ttc_condition_not_fulfilled)
{
   /** \arrange scenario with longitudinal path so that minimum age is changed. Additionally ttc condition is not fulfilled */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_FRONT;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LONG_BACKWARD;

   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov         = 0;

   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = object.attributes->ttc + EPSILON;

   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert expect no additional level qualification */
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests setting of counter logic for criticality level. Here path tracking information is available but the path is qualified as
 * lateral path. \uts{CSCSA-41894} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__set_min_age_to_zero_since_lateral_left_path_is_available)
{
   /** \arrange scenario for object being matched to a lateral path */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_REAR;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LAT_LEFT;

   level_index                        = CTA_CRIT_LEVEL_NONE;
   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov         = 0;

   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values and minimum age with 0 */
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests setting of counter logic for criticality level. Here path tracking information is available but the path is qualified as
 * lateral path. \uts{CSCSA-41895} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__set_min_age_to_zero_since_lateral_right_path_is_available)
{
   /** \arrange scenario for object being matched to a lateral path */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_NONE;
   Cta_Mode_T mode     = CTA_MODE_REAR;
   Pt_Output_T pt_output{};
   pt_output.path_obj_pair_output[0].path_direction = PATH_DIRECTION_LAT_RIGHT;

   cals.k_cta_sensor_fov_border[mode] = 0.0f;

   /*Target properties*/
   object.attributes->approach_side    = FBK_SIDE_RIGHT;
   object.attributes->ttc              = 1.2f;
   object.attributes->p_pt_match_info  = &pt_output.path_obj_pair_output[0];
   object.tracker_data.f_merge_occured = FBK_FALSE;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;

   cta_comparison_data.max_level[mode][object.attributes->approach_side] = CTA_CRIT_LEVEL_NONE;

   /*Deactivate sensor FOV functionality*/
   cals.k_cta_addit_mature_cycles_outside_sensor_fov = 0;
   cals.k_cta_min_age_obj_outside_sensor_fov         = 0;

   cals.k_cta_min_object_age_thres                    = 2;
   cals.k_cta_additional_qualification_mature_cycles  = 2;
   cals.k_cta_min_ttc_additional_mature_qualification = 1.0f;


   /** \action test target function */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert counter threshold data shall be initialized with default values and minimum age with 0 */
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}

/**
 * Tests Setting of criticality level threshold qualification data. When no merge is occuring, the default setting of qualification
 * counters is expected. again. \uts{CSCSA-41896} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__previously_warned_object_vanished)
{
   /** \arrange object which vanishes in the current cycle but was warned in the previous one */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_2;
   Cta_Mode_T mode     = CTA_MODE_REAR;

   crit_level_cals.minimum_age = 2;
   /*Target properties*/
   object.attributes->approach_side                                                     = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info                                                   = NULL;
   object.tracker_data.f_merge_occured                                                  = FBK_FALSE;
   object.tracker_data.id_merged_obj                                                    = PA_INVALID_OBJ_ID;
   cta_persistent.previous_most_critical_obj_id[mode][object.attributes->approach_side] = 1;
   cta_persistent.previous_crit_level[mode][object.attributes->approach_side]           = CTA_CRIT_LEVEL_2;
   object.persistent->prev_cycle_crit_level[mode]                                       = CTA_CRIT_LEVEL_1;
   cta_comparison_data.max_level[mode][object.attributes->approach_side]                = CTA_CRIT_LEVEL_1;

   /*Host properties*/
   p_vehicle_data->host_length = 4.0f;


   /** \action executes function to test */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert default configuration of qualification counters is expected */
   EXPECT_EQ(crit_level_cals.minimum_age, FBK_ZERO_UINT);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests update functionality of object suppression counter based on path tracking input. path direction is lateral left and lat
 * velocity negative. \uts{CSCSA-41897} \sdd{SF-3786} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Additional_Thres_Data__set_min_age_and_additional_maturity_cycles)
{
   /** \arrange path with lateral left direction and negative velocity */
   Cta_Persistent_T cta_persistent{};
   uint8_t level_index = CTA_CRIT_LEVEL_2;
   Cta_Mode_T mode     = CTA_MODE_REAR;

   cals.k_cta_sensor_fov_border[mode] = 0.0f; /*deactivated
                                                                                                                        here*/
   crit_level_cals.minimum_age = 2;

   /*disable outside sensor fov check*/
   object.tracker_data.vcs_pos                                   = Create_2d_Vector_Coordinates(-10.0f, 10.0f);
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.x = object.tracker_data.vcs_pos.x;
   object.attributes->approach_side                              = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info                            = NULL;
   object.tracker_data.f_merge_occured                           = FBK_FALSE;
   object.tracker_data.id_merged_obj                             = PA_INVALID_OBJ_ID;
   cta_persistent.previous_most_critical_obj_id[mode][object.attributes->approach_side] = 1;
   cta_persistent.previous_crit_level[mode][object.attributes->approach_side]           = CTA_CRIT_LEVEL_2;

   p_vehicle_data->host_length = 4.0f;


   /** \action executes function to test */
   Cta_Set_Additional_Thres_Data(&crit_level_cals, &cta_comparison_data, &cals, &object, &cta_persistent, p_vehicle_data,
                                 level_index, mode);

   /** \assert Counter increase by 1 */
   EXPECT_EQ(crit_level_cals.minimum_age, cals.k_cta_min_object_age_thres);
   EXPECT_EQ(crit_level_cals.mature_cycles, cals.k_cta_min_mature_cycles_level_qualifiction);
}


/**
 * Tests whether an object status is sufficient. It is expected that the object is sufficient
 * \uts{CSCSA-41898} \sdd{SF-3785} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Track_Status_Sufficient__track_status_is_sufficient_with_mature_cycles_3)
{
   /** \arrange object whose status is sufficient */
   boolean_T ret;
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.stage_age = 3u;
   crit_level_cals.mature_cycles = object.tracker_data.stage_age;

   /** \action executes function to test */
   ret = Cta_Is_Track_Status_Sufficient(&(object.tracker_data), &crit_level_cals);

   /** \assert Expect object being sufficient */
   EXPECT_TRUE(ret);
}

/**
 * Tests whether an object status is sufficient. It is expected that the object is sufficient, since the threshold is chosen to 0.
 * \uts{CSCSA-41899} \sdd{SF-3785} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Track_Status_Sufficient__track_status_is_sufficient_with_mature_cycles_0)
{
   /** \arrange thresholds which are causing the object to be defaulty sufficient */
   boolean_T ret;
   crit_level_cals.mature_cycles = 0;

   /** \action executes function to test */
   ret = Cta_Is_Track_Status_Sufficient(&(object.tracker_data), &crit_level_cals);

   /** \assert Expect object being sufficient */
   EXPECT_TRUE(ret);
}


/**
 * Tests whether an object status is sufficient. It is expected that the object is not sufficient, since its status is coasted.
 * \uts{CSCSA-41900} \sdd{SF-3785} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Is_Track_Status_Sufficient__track_status_is_not_sufficient_since_status_is_coasted)
{
   /** \arrange object whose status is not sufficient */
   boolean_T ret;
   object.tracker_data.status    = PA_OBJ_STATUS_COASTED;
   object.tracker_data.stage_age = 3u;
   crit_level_cals.mature_cycles = object.tracker_data.stage_age;

   /** \action executes function to test */
   ret = Cta_Is_Track_Status_Sufficient(&(object.tracker_data), &crit_level_cals);

   /** \assert Expect that object is not sufficient */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether an object status is sufficient. It is expected that the object is not sufficient, since the maturity cycle count
 * is less than the minimum needed threshold. \uts{CSCSA-41901} \sdd{SF-3785} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Is_Track_Status_Sufficient__track_status_is_not_sufficient_since_maturity_count_is_too_low)
{
   /** \arrange object whose status is not sufficient */
   boolean_T ret;
   object.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.stage_age = 2u;
   crit_level_cals.mature_cycles = object.tracker_data.stage_age + 1u;

   /** \action executes function to test */
   ret = Cta_Is_Track_Status_Sufficient(&(object.tracker_data), &crit_level_cals);

   /** \assert Expect that object is not sufficient */
   EXPECT_FALSE(ret);
}


/**
 * Tests application of level threshold hystereses. Host velocity extension shall be deactivated and just the hysteresis gain shall
 * be applied \uts{CSCSA-41902} \sdd{SF-3782} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Apply_Level_Thres_Hyst__trackers_relative_velocity_shall_be_used_both_flags_false)
{
   /** \arrange Setup of level calibrations */
   uint8_t level_index = CTA_CRIT_LEVEL_1 - 1u; /**<criticality level 1 is mapped to index 0*/
   Cta_Mode_T mode     = CTA_MODE_REAR;
   float32_T isect_max = -2.0f;
   float32_T isect_min = -6.0f;
   float32_T diff      = Fbk_Abs_F(isect_max - isect_min);
   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode, isect_max, isect_min, 1.0f);

   /** \action executes function to test */
   Cta_Apply_Level_Thres_Hyst(&crit_level_cals, &cals, mode, level_index);
   /** \assert hysteresis gain shall not affect intersection zones */
   EXPECT_FLOAT_EQ(crit_level_cals.max_long_point_criticality_level[mode][level_index],
                   isect_max + Fbk_Half(cals.k_cta_rel_warning_hysteresis * diff));
   EXPECT_FLOAT_EQ(crit_level_cals.min_long_point_criticality_level[mode][level_index],
                   isect_min - Fbk_Half(cals.k_cta_rel_warning_hysteresis * diff));
}


/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since it is outside of the
 * zone. \uts{CSCSA-41903} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__target_is_not_in_zone)
{
   /** \arrange Setup conditions for qualification of criticality level */
   boolean_T result;
   Cta_Mode_T mode = CTA_MODE_REAR;

   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   /*conditions for qualification of criticality level*/
   boolean_T target_in_zone = FBK_FALSE;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since it ttc condition is not
 * fulfilled. \uts{CSCSA-41904} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__ttc_condition_not_fulfilled)
{
   /** \arrange Setup conditions for qualification of criticality level with failing ttc condition */
   boolean_T result;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   boolean_T target_in_zone = FBK_TRUE;
   object.attributes->ttc   = 1.0f;
   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode, 0.0f, 0.0f, object.attributes->ttc - EPSILON);

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since its intersection point
 * is below the minimum boundary of intersection zone. \uts{CSCSA-41905} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__min_boundary_of_intersection_zone_condition_not_fulfilled)
{
   /** \arrange Setup conditions for qualification of criticality level with failing min_boundary_of_intersection_zone condition */
   boolean_T result;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON, -EPSILON,
                                        object.attributes->ttc + EPSILON);


   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}

/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since its intersection point
 * is above the maximum boundary of intersection zone. \uts{CSCSA-41906} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__max_boundary_of_intersection_zone_condition_not_fulfilled)
{
   /** \arrange Setup conditions for qualification of criticality level with failing max_boundary_of_intersection_zone condition */
   boolean_T result;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - 2.0f * EPSILON,
                                        object.attributes->ttc + EPSILON);

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since eclipse value condition
 * is not fulfilled. \uts{CSCSA-41907} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__eclipse_val_condition_not_fulfilled)
{
   /** \arrange Setup conditions for qualification of criticality level with failing eclipse value condition */
   boolean_T result;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.tracker_data.eclipse_value                                  = 0.5f;


   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);
   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value - EPSILON;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall not be critical since its age condition is not
 * fulfilled. \uts{CSCSA-41908} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__age_condition_not_fulfilled)
{
   /** \arrange Setup conditions for qualification of criticality level with failing age condition */
   boolean_T result;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 2;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.tracker_data.eclipse_value                                  = 0.5f;
   object.tracker_data.age                                            = 3;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);

   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value + EPSILON;
   crit_level_cals.minimum_age       = object.tracker_data.age + (uint32_t) 1;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 0);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall be critical since each condition is fulfilled.
 * However the criticality is suppressed, so that the object needs to qualify for criticality over successive amount of cycles.
 * \uts{CSCSA-41909} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__object_critical_but_not_passing_suppression_condition)
{
   /** \arrange Setup conditions for critical object which gets supressed */
   boolean_T result;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   cals.k_cta_cycle_count_suppress_true_warning                         = 3;
   object.persistent->crit_level_suppression_counter[mode][level_index] = cals.k_cta_cycle_count_suppress_true_warning - 1u;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.tracker_data.eclipse_value                                  = 0.5f;
   object.tracker_data.age                                            = 3;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);

   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value + EPSILON;
   crit_level_cals.minimum_age       = object.tracker_data.age - (uint32_t) 1;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 3);
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall be critical since each condition is fulfilled.
 * Object is suppressed due to reaching maximum number of critical cycles. \uts{CSCSA-41910} \sdd{SF-3783}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__object_critical_but_suppressed_due_to_max_counter)
{
   /** \arrange Setup conditions for critical object */
   boolean_T result;

   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 3;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.persistent->n_alert_cycles[mode]                            = UINT8_MAX;
   object.tracker_data.eclipse_value                                  = 0.5f;
   object.tracker_data.age                                            = 3;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);

   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value + EPSILON;
   crit_level_cals.minimum_age       = object.tracker_data.age - (uint32_t) 1;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall not be qualified for criticality level */
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall be critical since each condition is fulfilled.
 * Also the supression cycles are covered in this testcase. \uts{CSCSA-41911} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_Single_Level__object_critical_and_passing_suppression_condition)
{
   /** \arrange Setup conditions for critical object */
   boolean_T result;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 4;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.tracker_data.eclipse_value                                  = 0.5f;
   object.tracker_data.age                                            = 4;

   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);

   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value + EPSILON;
   crit_level_cals.minimum_age       = object.tracker_data.age - (uint32_t) 1;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 5);
   EXPECT_TRUE(result);
}


/**
 * Tests functionality of object being classified in criticality level. Object shall be critical since each condition is fulfilled.
 * Also the suppression cycles are covered in this testcase. \uts{CSCSA-41912} \sdd{SF-3783} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test,
       Cta_Check_Single_Level__object_critical_and_passing_suppression_condition_but_track_status_not_sufficient)
{
   /** \arrange Setup conditions for critical object */
   boolean_T result;
   uint8_t level_index                                                  = CTA_CRIT_LEVEL_1 - 1u;
   Cta_Mode_T mode                                                      = CTA_MODE_REAR;
   object.persistent->crit_level_suppression_counter[mode][level_index] = 3;

   boolean_T target_in_zone                                           = FBK_TRUE;
   object.attributes->ttc                                             = 1.0f;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -11.0f;
   object.tracker_data.eclipse_value                                  = 0.5f;
   object.tracker_data.age                                            = 3;
   object.tracker_data.stage_age                                      = object.tracker_data.age;


   Cta_Init_Level_Calibration_One_Level(&crit_level_cals, level_index, mode,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON,
                                        object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON,
                                        object.attributes->ttc + EPSILON);
   crit_level_cals.max_eclipse_value = object.tracker_data.eclipse_value + EPSILON;
   crit_level_cals.minimum_age       = object.tracker_data.age - (uint32_t) 1;
   crit_level_cals.mature_cycles     = object.tracker_data.stage_age + (uint32_t) 1;

   /** \action executes function to test */
   result = Cta_Check_Single_Level(&object, &crit_level_cals, &cals, target_in_zone, mode, level_index);

   /** \assert object shall qualified for criticality level */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][level_index], 4);
   EXPECT_FALSE(result);
}

/**
 * Check that the addition of suppression cycles is determined correctly. Here false is expected because the approach side does not
 * match reference point position. \uts{CSCSA-41913} \sdd{SF-3788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Shall_Addit_Pos_Suppr_Be_Appl__general_test)
{
   /** \arrange Set data to pass the conditions */
   float32_T result;
   attributes.ref_point_candidate[FBK_SIDE_LEFT].point.x = 1.0f;
   attributes.approach_side                              = FBK_SIDE_LEFT;
   Vector_2d_T point1                                    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T point2                                    = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   Line_Hesse_T line                                     = Line_Hesse_Create_Using_Two_Points(&point1, &point2);

   /** \action Execute function to test */
   result = Cta_Shall_Addit_Pos_Suppr_Be_Appl(&attributes, p_vehicle_data, line, CTA_MODE_FRONT);

   /** \assert Check if false is returned */
   EXPECT_FALSE(result);
}

/**
 * Check that the provided object is given the highest criticality level.
 * \uts{CSCSA-41914} \sdd{SF-3787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Set_Object_Highest_Crit__general_test)
{
   /** \arrange Set data of the objects */
   float32_T ttc         = 3.0f;
   float32_T cta_heading = 2.0f;
   uint8_t index         = 3u;

   Cta_Object_Data_T high_object{};
   Cta_Object_Persistent_T high_persistent{};
   Cta_Object_Attributes_T high_attributes{};

   high_object.attributes = &high_attributes;
   high_object.persistent = &high_persistent;

   attributes.ttc                    = ttc;
   obj_persistent.cta_object_heading = cta_heading;
   object.tracker_data.index         = index;

   /** \action Execute function to test */
   Cta_Set_Object_Highest_Crit(&high_object, &object);

   /** \assert Check that the properties of one object is transferred to the other */
   EXPECT_EQ(index, high_object.tracker_data.index);
   EXPECT_FLOAT_EQ(cta_heading, high_object.persistent->cta_object_heading);
   EXPECT_FLOAT_EQ(ttc, high_object.attributes->ttc);
}

/**
 * Check that the addition of suppression cycles is determined correctly.
 * \uts{CSCSA-41915} \sdd{SF-3791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_All_Level__general_test)
{
   /** \arrange set calibration and object properties to increment the level */
   Cta_Persistent_T cta_persistent{};
   Cta_Mode_T mode                                            = CTA_MODE_REAR;
   attributes.ttc                                             = 3.0f;
   crit_level_cals.ttc_criticality_level[mode][0]             = attributes.ttc + EPSILON;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -2.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON;
   crit_level_cals.max_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON;
   obj_tracker_output.eclipse_value                          = 1.0f;
   crit_level_cals.max_eclipse_value                         = obj_tracker_output.eclipse_value + EPSILON;
   obj_tracker_output.age                                    = 3u;
   crit_level_cals.minimum_age                               = obj_tracker_output.age - 1u;
   cals.k_cta_cycle_count_suppress_true_warning              = 0;
   obj_tracker_output.stage_age                              = 3u;
   crit_level_cals.mature_cycles                             = obj_tracker_output.stage_age - 1u;
   obj_tracker_output.status                                 = PA_OBJ_STATUS_MATURE;
   object.tracker_data                                       = obj_tracker_output;

   cals.k_cta_max_object_eclipse_for_level_qualification = crit_level_cals.max_eclipse_value;
   cals.k_cta_min_mature_cycles_level_qualifiction       = (uint8_t) crit_level_cals.mature_cycles;
   /** \action executes function to test */
   Cta_Check_All_Level(&crit_level_cals, &cta_comparison_data, &cta_persistent, &object, &cals, FBK_TRUE, p_vehicle_data, mode);

   /** \assert check that an object alert is set */
   EXPECT_NE(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Check that the addition of suppression cycles is determined correctly. Here, current obj criticallity level is same as
 * comparision data, but ttc is lower \uts{CSCSA-85513} \sdd{SF-3791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_All_Level__same_criticality_lower_tcc)
{
   /** \arrange set calibration and object properties to increment the level */
   Cta_Persistent_T cta_persistent{};
   Cta_Object_Data_T high_object{};
   Cta_Object_Attributes_T high_attributes{};
   Cta_Mode_T mode                                            = CTA_MODE_REAR;
   attributes.ttc                                             = 3.0f;
   crit_level_cals.ttc_criticality_level[mode][0]             = attributes.ttc + EPSILON;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -2.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON;
   crit_level_cals.max_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON;
   obj_tracker_output.eclipse_value                          = 1.0f;
   crit_level_cals.max_eclipse_value                         = obj_tracker_output.eclipse_value + EPSILON;
   obj_tracker_output.age                                    = 3u;
   crit_level_cals.minimum_age                               = obj_tracker_output.age - 1u;
   cals.k_cta_cycle_count_suppress_true_warning              = 0;
   obj_tracker_output.stage_age                              = 3u;
   crit_level_cals.mature_cycles                             = obj_tracker_output.stage_age - 1u;
   obj_tracker_output.status                                 = PA_OBJ_STATUS_MATURE;
   object.tracker_data                                       = obj_tracker_output;
   cals.k_cta_max_object_eclipse_for_level_qualification     = crit_level_cals.max_eclipse_value;
   cals.k_cta_min_mature_cycles_level_qualifiction           = (uint8_t) crit_level_cals.mature_cycles;
   cta_comparison_data.max_level[mode][object.attributes->approach_side]                = CTA_CRIT_LEVEL_1;
   obj_persistent.prev_cycle_crit_level[mode]                                           = 2u;
   high_attributes.ttc                                                                  = 5.0f;
   high_object.attributes                                                               = &high_attributes;
   cta_comparison_data.object_with_highest_crit[mode][object.attributes->approach_side] = high_object;

   /** \action executes function to test */
   Cta_Check_All_Level(&crit_level_cals, &cta_comparison_data, &cta_persistent, &object, &cals, FBK_TRUE, p_vehicle_data, mode);

   /** \assert check that an object alert is set */
   EXPECT_NE(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Check that the addition of suppression cycles is determined correctly. Here, current obj criticallity level is same as
 * comparision data, but ttc is higher \uts{CSCSA-85514} \sdd{SF-3791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_All_Level__same_criticality_higher_tcc)
{
   /** \arrange set calibration and object properties to increment the level */
   Cta_Persistent_T cta_persistent{};
   Cta_Object_Data_T high_object{};
   Cta_Object_Attributes_T high_attributes{};
   Cta_Mode_T mode                                            = CTA_MODE_REAR;
   attributes.ttc                                             = 3.0f;
   crit_level_cals.ttc_criticality_level[mode][0]             = attributes.ttc + EPSILON;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -2.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON;
   crit_level_cals.max_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON;
   obj_tracker_output.eclipse_value                          = 1.0f;
   crit_level_cals.max_eclipse_value                         = obj_tracker_output.eclipse_value + EPSILON;
   obj_tracker_output.age                                    = 3u;
   crit_level_cals.minimum_age                               = obj_tracker_output.age - 1u;
   cals.k_cta_cycle_count_suppress_true_warning              = 0;
   obj_tracker_output.stage_age                              = 3u;
   crit_level_cals.mature_cycles                             = obj_tracker_output.stage_age - 1u;
   obj_tracker_output.status                                 = PA_OBJ_STATUS_MATURE;
   object.tracker_data                                       = obj_tracker_output;
   cals.k_cta_max_object_eclipse_for_level_qualification     = crit_level_cals.max_eclipse_value;
   cals.k_cta_min_mature_cycles_level_qualifiction           = (uint8_t) crit_level_cals.mature_cycles;

   cta_comparison_data.max_level[mode][object.attributes->approach_side]                = CTA_CRIT_LEVEL_1;
   obj_persistent.prev_cycle_crit_level[mode]                                           = 2u;
   high_attributes.ttc                                                                  = 2.0f;
   high_object.attributes                                                               = &high_attributes;
   cta_comparison_data.object_with_highest_crit[mode][object.attributes->approach_side] = high_object;

   /** \action executes function to test */
   Cta_Check_All_Level(&crit_level_cals, &cta_comparison_data, &cta_persistent, &object, &cals, FBK_TRUE, p_vehicle_data, mode);

   /** \assert check that an object alert is set */
   EXPECT_NE(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Current obj criticallity level is lower, so it's not updated
 * \uts{CSCSA-188373} \sdd{SF-3791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Criticality_Level_Calculation_Test, Cta_Check_All_Level__lower_criticality)
{
   /** \arrange set calibration and object properties to increment the level */
   Cta_Persistent_T cta_persistent{};
   Cta_Object_Data_T high_object{};
   Cta_Object_Attributes_T high_attributes{};
   Cta_Mode_T mode                                            = CTA_MODE_REAR;
   attributes.ttc                                             = 3.0f;
   crit_level_cals.ttc_criticality_level[mode][0]             = attributes.ttc + EPSILON;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] = -2.0f;
   crit_level_cals.min_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] - EPSILON;
   crit_level_cals.max_long_point_criticality_level[mode][0] = attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode] + EPSILON;
   obj_tracker_output.eclipse_value                          = 1.0f;
   crit_level_cals.max_eclipse_value                         = obj_tracker_output.eclipse_value + EPSILON;
   obj_tracker_output.age                                    = 3u;
   crit_level_cals.minimum_age                               = obj_tracker_output.age - 1u;
   cals.k_cta_cycle_count_suppress_true_warning              = 0;
   obj_tracker_output.stage_age                              = 3u;
   obj_tracker_output.id                                     = 1u;
   crit_level_cals.mature_cycles                             = obj_tracker_output.stage_age - 1u;
   obj_tracker_output.status                                 = PA_OBJ_STATUS_MATURE;
   object.tracker_data                                       = obj_tracker_output;
   cals.k_cta_max_object_eclipse_for_level_qualification     = crit_level_cals.max_eclipse_value;
   cals.k_cta_min_mature_cycles_level_qualifiction           = (uint8_t) crit_level_cals.mature_cycles;

   cta_comparison_data.max_level[mode][object.attributes->approach_side]                = CTA_CRIT_LEVEL_2;
   obj_persistent.prev_cycle_crit_level[mode]                                           = 2u;
   high_attributes.ttc                                                                  = 2.0f;
   high_object.attributes                                                               = &high_attributes;
   high_object.tracker_data.id                                                          = 6u;
   cta_comparison_data.object_with_highest_crit[mode][object.attributes->approach_side] = high_object;

   /** \action executes function to test */
   Cta_Check_All_Level(&crit_level_cals, &cta_comparison_data, &cta_persistent, &object, &cals, FBK_TRUE, p_vehicle_data, mode);

   /** \assert check that an object alert is set */
   EXPECT_EQ(cta_comparison_data.max_level[mode][object.attributes->approach_side], CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_comparison_data.object_with_highest_crit[mode][object.attributes->approach_side].tracker_data.id, 6);
}