/**
 * @file lcda_process_slc_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_process_slc.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42802}
 */

#include "lcda_process_slc_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_process_slc.c"
#include "ml_math.h"
}

static void Lcda_Create_Valid_Slc_Object(Slc_Object_T *p_slc_object,
                                         Fbk_Object_Data_T *p_tracker_object,
                                         uint8_t obj_idx,
                                         float32_T lon_ttc,
                                         float32_T lat_ttc,
                                         float32_T lane_change_prob)
{

   p_tracker_object->index = obj_idx;
   p_slc_object->lon_ttc   = lon_ttc;
   p_slc_object->lat_ttc   = lat_ttc;

   p_slc_object->f_obj_besides_ego = FBK_FALSE;
   p_slc_object->f_obj_overlap     = FBK_FALSE;
   p_slc_object->f_obj_lane_change = FBK_FALSE;
   p_slc_object->f_obj_misses_ego  = FBK_FALSE;

   p_slc_object->lane_change_prob = lane_change_prob;
}

/**
 * Set up SLC data such that a SLC warning zone can be created. Check that the SLC zone vertices have the expected coordinates.
 * \uts{CSCSA-42803} \sdd{SF-6742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Create_Slc_Object_Zone__zone_zero_lane_center_offset)
{

   /** \arrange Set up SLC data and target object. */
   slc_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 0;

   /** \action Call Lcda_Create_Slc_Object_Zone to create SLC zone. */
   Lcda_Create_Slc_Object_Zone(&slc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC zone vertices have correct coordinates. */
   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[0]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[1]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[2]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[3]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[4]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].x, lcda_cals.k_slc_zone_x[0]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].x, lcda_cals.k_slc_zone_x[1]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].x, lcda_cals.k_slc_zone_x[2]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].x, lcda_cals.k_slc_zone_x[3]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].x, lcda_cals.k_slc_zone_x[4]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].x, lcda_cals.k_slc_zone_x[5]);
}

/**
 * Set up SLC data such that a SLC warning zone can be created. Add a lane center offset which affects the SLC zone. Check that the
 * SLC zone vertices have the expected coordinates. \uts{CSCSA-42804} \sdd{SF-6742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Create_Slc_Object_Zone__zone_with_lane_center_offset)
{
   /** \arrange Set up SLC data and target object. */
   slc_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0;

   // Set a non-zero lane center offset
   lcda_core_input.lane_center_offset = 1.0f;

   /** \action Call Lcda_Create_Slc_Object_Zone to create SLC zone. */
   Lcda_Create_Slc_Object_Zone(&slc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC zone vertices have correct coordinates. */
   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[0]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[1]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[2]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[3]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[4]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[5]) + lcda_core_input.lane_center_offset));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].x, lcda_cals.k_slc_zone_x[0]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].x, lcda_cals.k_slc_zone_x[1]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].x, lcda_cals.k_slc_zone_x[2]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].x, lcda_cals.k_slc_zone_x[3]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].x, lcda_cals.k_slc_zone_x[4]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].x, lcda_cals.k_slc_zone_x[5]);
}

/**
 * Set up SLC data such that a SLC warning zone can be created. Check that the SLC zone vertices have hysteresis applied, if a SLC
 * object qualifies. \uts{CSCSA-42805} \sdd{SF-6742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Create_Slc_Object_Zone__zone_zero_lane_center_offset2)
{

   /** \arrange Set up SLC data and target object. Set SLC mature counter such that an object is qualifying. */
   slc_object.ego_side                       = FBK_SIDE_LEFT;
   tracker_object.index                      = 0;
   uint8_t mature_count                      = lcda_cals.k_slc_min_mature_cycles + 1u;
   lcda_cals.k_zone_hys_obj_width_correction = 0.0f;

   /** \action Call Lcda_Create_Slc_Object_Zone to create SLC zone. */
   Lcda_Create_Slc_Object_Zone(&slc_object, mature_count, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC zone vertices have correct coordinates (altered by hysteresis). */
   // zone y-co ord should be a multiple of the lane width
   EXPECT_LT(slc_object.zone.points[0].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[0]));
   EXPECT_LT(slc_object.zone.points[1].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[1]));
   EXPECT_LT(slc_object.zone.points[2].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[2]));
   EXPECT_GT(slc_object.zone.points[3].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[3]));
   EXPECT_GT(slc_object.zone.points[4].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[4]));
   EXPECT_GT(slc_object.zone.points[5].y, (lcda_core_input.lane_width * -lcda_cals.k_slc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].x, lcda_cals.k_slc_zone_x[0]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].x, lcda_cals.k_slc_zone_x[1]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].x, lcda_cals.k_slc_zone_x[2]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].x, lcda_cals.k_slc_zone_x[3]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].x, lcda_cals.k_slc_zone_x[4]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].x, lcda_cals.k_slc_zone_x[5]);
}

/**
 * Set up SLC data such that a SLC warning zone can be created. Check that the SLC zone vertices have the expected coordinates.
 * \uts{CSCSA-42855} \sdd{SF-6742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Create_Slc_Object_Zone__lane_lower_than_calibration)
{

   /** \arrange Set up SLC data and target object. */
   slc_object.ego_side                       = FBK_SIDE_LEFT;
   tracker_object.index                      = 0;
   lcda_core_input.lane_width                = lcda_cals.k_lcda_min_lane_width - EPSILON;
   float32_T cal_lane_width                  = lcda_cals.k_lcda_min_lane_width;
   lcda_cals.k_zone_hys_obj_width_correction = -EPSILON;

   /** \action Call Lcda_Create_Slc_Object_Zone to create SLC zone. */
   Lcda_Create_Slc_Object_Zone(&slc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC zone vertices have correct coordinates. */
   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[0]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[1]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[2]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[3]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[4]));
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].y, (cal_lane_width * -lcda_cals.k_slc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].x, lcda_cals.k_slc_zone_x[0]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].x, lcda_cals.k_slc_zone_x[1]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].x, lcda_cals.k_slc_zone_x[2]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].x, lcda_cals.k_slc_zone_x[3]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].x, lcda_cals.k_slc_zone_x[4]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].x, lcda_cals.k_slc_zone_x[5]);
}

/**
 * Create SLC warn relevant target object with status invalid. Compute if the target object is SLC relevant and check that this is
 * true. \uts{CSCSA-42806} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_FALSE_track_status_INVALID)
{

   /** \arrange Set up target object data with status invalid. */
   boolean_T result;

   tracker_object.index  = 5u;
   tracker_object.status = PA_OBJ_STATUS_INVALID;


   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC target object with too large heading. Compute if the target object is SLC relevant and check that this is false.
 * \uts{CSCSA-42807} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_FALSE_obj_heading_too_large)
{

   /** \arrange Set up target object data with large heading. */
   boolean_T result;

   tracker_object.index         = 5u;
   tracker_object.status        = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading = 0.9f;
   tracker_object.curvi_vel.x   = 20.0f;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC target object with too small velocity. Compute if the target object is SLC relevant and check that this is false.
 * \uts{CSCSA-42808} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_FALSE_obj_velocity_too_low)
{

   /** \arrange Set up target object data with small velocity. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = 0.9f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 1.0f;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC target object with too high negative velocity (too small absolute value). Compute if the target object is SLC
 * relevant and check that this is false. \uts{CSCSA-186409} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_FALSE_negative_obj_velocity_too_high)
{

   /** \arrange Set up target object data with small velocity. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = -0.9f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 1.0f;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC target object with velocity higher than the threshold. Compute if the target object is SLC relevant and check that
 * this is true. \uts{CSCSA-186410} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_TRUE_obj_velocity_high_enough)
{

   /** \arrange Set up target object data with high velocity. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = 1.1f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 1.0f;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create SLC target object with velocity lower than the negative threshold. Compute if the target object is SLC relevant and check
 * that this is true. \uts{CSCSA-186411} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_TRUE_negative_obj_velocity_low_enough)
{

   /** \arrange Set up target object data with low negative velocity. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = -1.1f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 1.0f;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create SLC target object with eclipse higher than the threshold. Compute if the target object is SLC relevant and check that
 * this is true. \uts{CSCSA-186412} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__high_eclipse)
{

   /** \arrange Set up target object data with eclipse higher than the threshold. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = 1.1f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 1.0f;
   tracker_object.eclipse_value         = 1.1f * lcda_cals.k_slc_max_obj_eclipse;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC target object with existence probability lower than the threshold. Compute if the target object is SLC relevant and
 * check that this is true. \uts{CSCSA-186413} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__low_existence_probability)
{

   /** \arrange Set up target object data with existence probability lower than the threshold. */
   boolean_T result;

   tracker_object.index                 = 5u;
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.curvi_heading         = 0.9f * lcda_cals.k_slc_max_curvi_heading_abs;
   tracker_object.curvi_vel.x           = 1.1f * lcda_cals.k_slc_min_obj_curvi_long_vel_abs;
   tracker_object.existence_probability = 0.0f;
   tracker_object.eclipse_value         = 0.9f * lcda_cals.k_slc_max_obj_eclipse;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}


/**
 * Create SLC warn relevant target object with status mature. Compute if the target object is SLC relevant and check that this is
 * true. \uts{CSCSA-42809} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_TRUE_track_status_MATURE)
{

   /** \arrange Set up target object data with status mature. */
   boolean_T result;
   uint8_t valid_slc_obj_idx = 10u;
   Lcda_Create_Valid_Slc_Track(&tracker_object, valid_slc_obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);

   tracker_object.status = PA_OBJ_STATUS_MATURE;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create SLC warn relevant target object with status coasted. Compute if the target object is SLC relevant and check that this is
 * true. \uts{CSCSA-42810} \sdd{SF-6733} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Object_Relevant_For_Slc__is_TRUE_track_status_COASTED)
{
   /** \arrange Set up target object data with status coasted. */
   boolean_T result;
   uint8_t valid_slc_obj_idx = 10u;
   Lcda_Create_Valid_Slc_Track(&tracker_object, valid_slc_obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);

   tracker_object.status = PA_OBJ_STATUS_COASTED;

   /** \action Call Lcda_Is_Object_Relevant_For_Slc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Slc(&lcda_core_input, &tracker_object, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Set up lateral TTC which is greater than the critical threshold. Compute the lane change probability and check that it is set to
 * zero. \uts{CSCSA-42811} \sdd{SF-6731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Get_Lane_Change_Prob__zero_lat_ttc_greater_than_critical_threshold)
{

   /** \arrange Set up lateral TTC which is greater than the critical threshold. */
   float32_T result;
   float32_T lateral_ttc = 10.0f;

   /** \action Call Lcda_Get_Lane_Change_Prob to compute the lane change probability. */
   result = Lcda_Get_Lane_Change_Prob(lateral_ttc, &lcda_cals);

   /** \assert Check probability is zero. */
   EXPECT_FLOAT_EQ(result, LCDA_SLC_PROBABILITY_NONE);
}

/**
 * Set up lateral TTC which is negative (target object moving away from host). Compute the lane change probability and check that
 * it is set to zero. \uts{CSCSA-42812} \sdd{SF-6731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Get_Lane_Change_Prob__zero_lat_ttc_negative)
{

   /** \arrange Set up lateral TTC which is negative. */
   float32_T result;
   float32_T lateral_ttc = -10.0f;

   /** \action Call Lcda_Get_Lane_Change_Prob to compute the lane change probability. */
   result = Lcda_Get_Lane_Change_Prob(lateral_ttc, &lcda_cals);

   /** \assert Check probability is zero. */
   EXPECT_FLOAT_EQ(result, LCDA_SLC_PROBABILITY_NONE);
}

/**
 * Set up positiv lateral TTC. Compute the lane change probability and check that it is set to the correct value (taken from
 * internal look up table). \uts{CSCSA-42813} \sdd{SF-6731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Get_Lane_Change_Prob__interpolates_prob_from_lat_ttc)
{

   /** \arrange Set up positiv lateral TTC. */
   float32_T result;
   float32_T lateral_ttc = 2.5f;

   /** \action Call Lcda_Get_Lane_Change_Prob to compute the lane change probability. */
   result = Lcda_Get_Lane_Change_Prob(lateral_ttc, &lcda_cals);

   /** \assert Check probability equals expected value. */
   EXPECT_FLOAT_EQ(result, 0.75f);
}

/**
 * Create SLC warn relevant target object behind the ego on the right side which is fully qualified. Process the SLC warning state
 * and check that an alert level is raised. \uts{CSCSA-42814} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__alerts_on_critical_obj_behind_ego_on_RIGHT)
{
   /** \arrange Set up target object behind ego. */
   uint8_t critical_obj_idx = 5;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, 5.0f, -40.0f, -3.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_RIGHT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_RIGHT], tracker_object.id);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_RIGHT], tracker_object.unique_id);
   EXPECT_NE(slc_core_output.slc_lon_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_RIGHT]);
}

/**
 * Create SLC warn relevant target object beside the ego on the right side which is fully qualified. Process the SLC warning state
 * and check that an alert level is raised. \uts{CSCSA-42815} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__alerts_on_critical_obj_beside_ego_on_RIGHT)
{
   /** \arrange Set up target object beside ego. */
   uint8_t critical_obj_idx = 5;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, 5.0f, -p_vehicle_data->host_length - EPSILON, -3.0f, 2.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_RIGHT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_RIGHT], tracker_object.id);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_RIGHT], tracker_object.unique_id);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lon_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_NE(slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_RIGHT]);
}

/**
 * Create SLC warn relevant target object behind the ego on the left side which is fully qualified. Process the SLC warning state
 * and check that an alert level is raised. \uts{CSCSA-42816} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__alerts_on_critical_obj_behind_ego_on_LEFT)
{
   /** \arrange Set up target object behind ego. */
   uint8_t critical_obj_idx = 3;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -5.0f, -40.0f, 3.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], tracker_object.id);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], tracker_object.unique_id);
   EXPECT_NE(slc_core_output.slc_lon_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create SLC warn relevant target object beside the ego on the left side which is fully qualified. Process the SLC warning state
 * and check that an alert level is raised. \uts{CSCSA-42817} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__alerts_on_critical_obj_beside_ego_on_LEFT)
{
   /** \arrange Set up target object beside ego. */
   uint8_t critical_obj_idx = 3;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -5.0f, -p_vehicle_data->host_length - EPSILON, 3.0f, 2.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], tracker_object.id);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], tracker_object.unique_id);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lon_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_NE(slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create two SLC warn relevant target objects which are fully qualified. Process the SLC warning state and check that an alert
 * level is raised for the target object with higher lane change probability. \uts{CSCSA-42818} \sdd{SF-6745}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__alerts_on_obj_with_higher_lane_change_prob_LEFT)
{

   /** \arrange Set up target objects with one having a higher lane change probability. */
   uint8_t critical_obj_idx      = 2; // lower lateral ttc hence higher lane change prob (most critical)
   uint8_t less_critical_obj_idx = 5;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -5.0f, -40.0f, 4.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state for both target objects. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);
   Lcda_Create_Valid_Slc_Track(&tracker_object, less_critical_obj_idx, -5.0f, -30.0f, 3.0f, 15.0f);
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object with the higher lane change probability is warned and that an alert state level is
    * raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], critical_obj_idx + 1);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], critical_obj_idx + 1);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create SLC warn relevant target object which is moving parallel with the ego (along the longitudinal axis). Process the SLC
 * warning state and check that an alert level is not raised. \uts{CSCSA-42819} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__no_alert_when_valid_obj_is_moving_straight_with_zero_lat_vel)
{

   /** \arrange Set up target object. */
   uint8_t critical_obj_idx = 2;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -5.0f, -40.0f, 0.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create two SLC warn relevant target objects which are fully qualified. Process the SLC warning state and check that an alert
 * level is raised for the target object with lower TTC. \uts{CSCSA-42820} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__no_alert_on_obj_when_long_ttc_higher_than_critical_ttc)
{

   /** \arrange Set up target objects with habing a smaller TTC. */
   uint8_t critical_obj_idx      = 2;
   uint8_t less_critical_obj_idx = 5; // lower lateral ttc i.e. higher lane change prob as critical obj, however has long ttc more
                                      // than critical threshold

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -5.0f, -40.0f, 3.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state for both target objects. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);
   Lcda_Create_Valid_Slc_Track(&tracker_object, less_critical_obj_idx, -5.0f, -30.0f, 4.0f, 2.0f);
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object with lower TTC is warned and that an alert state level is raised. */
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], critical_obj_idx);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], critical_obj_idx + 1);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], critical_obj_idx + 1);
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create scenario where object has a high positive longitudinal velocity relative to the host vehicle. It will miss the host and
 * is thus uncritical. \uts{CSCSA-42821} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__obj_high_pos_long_vel_misses_ego)
{
   /** \arrange Set up target object with high long velocity */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is uncritical for SLC due to missing the host. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_RIGHT]);
}

/**
 * Create scenario where object has a high negative longitudinal velocity relative to the host vehicle. It will miss the host and
 * is thus uncritical. \uts{CSCSA-42822} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__obj_high_neg_long_vel_misses_ego)
{
   /** \arrange Set up target object with high long velocity */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, -12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the target object is uncritical for SLC due to missing the host. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_RIGHT]);
}


/**
 * Create SLC relevant target object behind the ego on the left side which is not in the zone. Process the SLC warning state and
 * check that an alert level is off and mature count in zone value set to 0. \uts{CSCSA-99046} \sdd{SF-6745}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__no_alert_object_not_in_zone)
{
   /** \arrange Set up target object behind ego. */
   uint8_t critical_obj_idx                                   = 3;
   slc_persistent.mature_count_in_slc_zone[tracker_object.id] = 10;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -8.0f, -40.0f, 3.0f, 12.0f);

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the object does not trigger the alert and its mature count is set to 0. */
   EXPECT_NE(slc_core_output.slc_index[FBK_SIDE_LEFT], critical_obj_idx);
   EXPECT_NE(slc_core_output.slc_id[FBK_SIDE_LEFT], tracker_object.id);
   EXPECT_NE(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], tracker_object.unique_id);
   EXPECT_EQ(slc_persistent.mature_count_in_slc_zone[tracker_object.id], FBK_ZERO_UINT);
}

/**
 * Create SLC irrelevant target object. Process the SLC warning state and check that an alert level is off and mature count in zone
 * value set to 0. \uts{CSCSA-186414} \sdd{SF-6745} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Object__no_alert_irrelevant_object)
{
   /** \arrange Set up irrelevant target. */
   uint8_t critical_obj_idx = 3;

   Lcda_Create_Valid_Slc_Track(&tracker_object, critical_obj_idx, -8.0f, -40.0f, 3.0f, 12.0f);
   tracker_object.status                                      = PA_OBJ_STATUS_INVALID;
   slc_persistent.mature_count_in_slc_zone[tracker_object.id] = 10;

   /** \action Call Lcda_Process_Slc_Object to compute the SLC alert state. */
   Lcda_Process_Slc_Object(&slc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &slc_persistent);

   /** \assert Check that the object does not trigger the alert and its mature count is set to 0. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(slc_persistent.mature_count_in_slc_zone[critical_obj_idx], FBK_ZERO_UINT);
}


/*
 * Check that for an active alert in the current cycle the SLC post-processing resets the holding counter and fills the persistent
 * data according to this alert. \uts{CSCSA-42823} \sdd{SF-6743} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Postprocess_Slc__resets_holding_counter_and_fills_persistent_data_for_active_alert)
{
   /** \arrange Set up core output with an alert and persistent data with holding counter unequal zero. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]             = FBK_TRUE;
   slc_core_output.slc_index[FBK_SIDE_LEFT]             = 3u;
   slc_core_output.slc_id[FBK_SIDE_LEFT]                = 15u;
   slc_core_output.slc_unique_id[FBK_SIDE_LEFT]         = 16u;
   slc_persistent.slc_hold_counter[FBK_SIDE_LEFT]       = 4u;
   slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT] = lcda_cals.k_slc_alert_qualifying_counter;

   /** \action Call function Lcda_Postprocess_Slc for post-processing of current SLC cycle. */
   Lcda_Postprocess_Slc(&slc_core_output, &lcda_cals, &slc_persistent);

   /** \assert Verify that holding counter is reseted and previous alert is set in persistent data. */
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT], slc_core_output.slc_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_LEFT], slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_LEFT], slc_core_output.slc_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.slc_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Set SLC alert on left side with sufficient qualification cycles. Check that the alert is written to the core output.
 * \uts{CSCSA-42824} \sdd{SF-6923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Output__alert_permitted)
{
   /** \arrange Set SLC alert and persistent data. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]               = FBK_TRUE;
   slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT]   = lcda_cals.k_slc_alert_qualifying_counter;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = PA_INVALID_OBJ_INDEX;

   /** \action Call Lcda_Process_Slc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Slc_Output(&slc_core_output, &slc_persistent, &lcda_cals);

   /** \assert Check that the SLC alert is raised. */
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
}

/**
 * Set SLC alert on left side with insufficient qualification cycles. Check that the alert is not written to the core output.
 * \uts{CSCSA-42825} \sdd{SF-6923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Output__alert_suppressed)
{
   /** \arrange Set SLC alert and persistent data. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]               = FBK_TRUE;
   slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT]   = lcda_cals.k_slc_alert_qualifying_counter - 1u;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = PA_INVALID_OBJ_INDEX;

   /** \action Call Lcda_Process_Slc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Slc_Output(&slc_core_output, &slc_persistent, &lcda_cals);

   /** \assert Check that the SLC alert is not raised. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT], lcda_cals.k_slc_alert_qualifying_counter);
}

/**
 * Set no SLC alert on left side with insufficient qualification cycles. Check that the qualification counter within persistent
 * data is reset. \uts{CSCSA-42826} \sdd{SF-6923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Output__qualifying_counter_reset)
{
   /** \arrange Set SLC alert and persistent data. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]               = FBK_FALSE;
   slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT]   = 3u;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = PA_INVALID_OBJ_INDEX;

   /** \action Call Lcda_Process_Slc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Slc_Output(&slc_core_output, &slc_persistent, &lcda_cals);

   /** \assert Check that the SLC qualification counter is reset. */
   EXPECT_EQ(slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Check that SLC alert is held if no active alert is present and holding counter is below threshold.
 * \uts{CSCSA-42827} \sdd{SF-6923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Output__holds_alert_if_holding_counter_below_threshold)
{
   /** \arrange Set up core output and persistent data, such that alert holding is expected. */
   lcda_cals.k_slc_alert_holding_cycles = 4u;

   slc_core_output.slc_alert[FBK_SIDE_LEFT]                   = FBK_FALSE;
   slc_persistent.slc_hold_counter[FBK_SIDE_LEFT]             = lcda_cals.k_slc_alert_holding_cycles - 1u;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT]     = 4u;
   slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_LEFT]        = 4u;
   slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_LEFT] = 5u;

   /** \action Call Lcda_Process_Slc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Slc_Output(&slc_core_output, &slc_persistent, &lcda_cals);

   /** \assert Check that SLC alert is held and holding counter is increased. */
   EXPECT_TRUE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.slc_hold_counter[FBK_SIDE_LEFT], lcda_cals.k_slc_alert_holding_cycles);
}

/**
 * Check that SLC alert is not held if no active alert is present and holding counter is above threshold.
 * \uts{CSCSA-42828} \sdd{SF-6923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Process_Slc_Output__does_not_hold_alert_if_holding_counter_above_threshold)
{
   /** \arrange Set up core output and persistent data, such that no alert holding is expected. */
   lcda_cals.k_slc_alert_holding_cycles = 4u;

   slc_core_output.slc_alert[FBK_SIDE_LEFT]               = FBK_FALSE;
   slc_core_output.slc_index[FBK_SIDE_LEFT]               = PA_INVALID_OBJ_INDEX;
   slc_core_output.slc_id[FBK_SIDE_LEFT]                  = PA_INVALID_OBJ_ID;
   slc_core_output.slc_unique_id[FBK_SIDE_LEFT]           = PA_INVALID_OBJ_ID;
   slc_persistent.slc_hold_counter[FBK_SIDE_LEFT]         = lcda_cals.k_slc_alert_holding_cycles;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = 4u;
   slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_LEFT]    = 4u;

   /** \action Call Lcda_Process_Slc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Slc_Output(&slc_core_output, &slc_persistent, &lcda_cals);

   /** \assert Check that SLC alert is not held and holding counter is reset. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_persistent.slc_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Create SLC warn relevant target object with a TTC above the specified threshold. Compute if the objects TTC is below the
 * threshold and check that this is false. \uts{CSCSA-42829} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_FALSE_ttc_is_default_large)
{

   /** \arrange Set up target object data such that its TTC is above the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0, LCDA_DEFAULT_LARGE_TTC, LCDA_DEFAULT_LARGE_TTC, 0.0f);

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC warn relevant target object with a TTC below the specified threshold. Compute if the objects TTC is below the
 * threshold and check that this is true. \uts{CSCSA-42830} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_TRUE_ttc_is_small)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0, EPSILON, EPSILON, 0.0f);

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is below the threshold. */
   EXPECT_TRUE(result);
}

/**
 * Create SLC warn relevant target object behind the ego with a lateral TTC above the specified threshold. Compute if the objects
 * TTC is below the threshold and check that this is false. \uts{CSCSA-42831} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_FALSE_ttc_long_threshold_lat_large_obj_behind_ego)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   uint8_t obj_idx = 0;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, lcda_cals.k_slc_critical_lon_ttc, LCDA_DEFAULT_LARGE_TTC, 0.0f);

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC warn relevant target object behind the ego with a longitudinal TTC above the specified threshold. Compute if the
 * objects TTC is below the threshold and check that this is false. \uts{CSCSA-42832} \sdd{SF-6735}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_FALSE_ttc_long_large_lat_threshold_obj_behind_ego)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   uint8_t obj_idx = 0;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, LCDA_DEFAULT_LARGE_TTC, lcda_cals.k_slc_critical_lat_ttc, 0.0f);

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC warn relevant target object besides the ego with a lateral TTC above the specified threshold. Compute if the objects
 * TTC is below the threshold and check that this is false. \uts{CSCSA-42833} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_FALSE_ttc_long_threshold_lat_large_obj_besides_ego)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   uint8_t obj_idx = 0;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, lcda_cals.k_slc_critical_lon_ttc, LCDA_DEFAULT_LARGE_TTC, 0.0f);

   slc_object.f_obj_besides_ego = FBK_TRUE;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create SLC warn relevant target object besides the ego with a longitudinal TTC above the specified threshold. Compute if the
 * objects TTC is below the threshold and check that this is false. \uts{CSCSA-42834} \sdd{SF-6735}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_TRUE_ttc_long_large_lat_threshold_obj_besides_ego)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   uint8_t obj_idx = 0;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, LCDA_DEFAULT_LARGE_TTC, lcda_cals.k_slc_critical_lat_ttc, 0.0f);

   slc_object.f_obj_overlap = FBK_TRUE;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_TRUE(result);
}


/**
 * Create SLC warn relevant target object beside the ego with a lateral TTC above the default threshold but below the hysteresis
 * threshold. Set an active alert for the SLC object. Compute if the objects TTC is below the hysteresis threshold and check that
 * this is true. \uts{CSCSA-42835} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__is_TRUE_ttc_lat_threshold_hysteresis)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold if and only if the hysteresis is
    * active. Set an active alert for the object in the previous cycle. */
   uint8_t obj_idx = 5;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, LCDA_DEFAULT_LARGE_TTC,
                                lcda_cals.k_slc_critical_lat_ttc + lcda_cals.k_slc_critical_lat_ttc_hys, 0.0f);

   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = obj_idx;
   slc_object.f_obj_overlap                               = FBK_TRUE;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is below the (hysteresis) threshold. */
   EXPECT_TRUE(result);
}


/**
 * Create SLC warn relevant target object beside the ego with a lateral TTC above the default threshold but below the hysteresis
 * threshold. Set an active alert for the SLC object. Compute if the objects TTC is below the hysteresis threshold and check that
 * this is true. \uts{CSCSA-99047} \sdd{SF-6735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Ttc_Below_Thresholds__true_false)
{

   /** \arrange Set up target object data such that its TTC is below the specified threshold if and only if the hysteresis is
    * active. Set an active alert for the object in the previous cycle. */
   uint8_t obj_idx = 5;
   boolean_T result;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, obj_idx, LCDA_DEFAULT_LARGE_TTC,
                                lcda_cals.k_slc_critical_lat_ttc + lcda_cals.k_slc_critical_lat_ttc_hys, 0.0f);

   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = FBK_ZERO_UINT;
   slc_object.f_obj_overlap                               = FBK_TRUE;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Thresholds(&slc_object, &lcda_cals, &lcda_core_input.warn_settings, &slc_persistent);

   /** \assert Check that the TTC is below the (hysteresis) threshold. */
   EXPECT_FALSE(result);
}


/**
 * Check that the side persistent data is filled properly from the SLC core output.
 * \uts{CSCSA-42836} \sdd{SF-6730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Fill_Side_Persistent_Slc_Data__fills_persistent_data_from_core_output_correctly)
{
   /** \arrange Set up core output with non-default values. */
   slc_core_output.slc_index[FBK_SIDE_LEFT]      = 23u;
   slc_core_output.slc_index[FBK_SIDE_RIGHT]     = 2u;
   slc_core_output.slc_id[FBK_SIDE_LEFT]         = 26u;
   slc_core_output.slc_id[FBK_SIDE_RIGHT]        = 5u;
   slc_core_output.slc_unique_id[FBK_SIDE_LEFT]  = 27u;
   slc_core_output.slc_unique_id[FBK_SIDE_RIGHT] = 6u;

   /** \action Call function Lcda_Fill_Side_Persistent_Slc_Data to fill persistent side data from core output */
   Lcda_Fill_Side_Persistent_Slc_Data(&slc_persistent, &slc_core_output);

   /** \assert Verify that values from persistent side data and core output are the same. */
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT], slc_core_output.slc_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_RIGHT], slc_core_output.slc_index[FBK_SIDE_RIGHT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_LEFT], slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT], slc_core_output.slc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_LEFT], slc_core_output.slc_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_RIGHT], slc_core_output.slc_unique_id[FBK_SIDE_RIGHT]);
}

/**
 * Set last warned target object index and create a tracker object with the same index. Compute if the tracker objects index was
 * warned last cycle. Check that this is the case. \uts{CSCSA-42837} \sdd{SF-6737} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle__is_TRUE_was_critical_last_cycle)
{

   /** \arrange Set up persistent data. */
   uint8_t index = 4u;
   boolean_T result;

   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_LEFT] = index;
   tracker_object.index                                   = index;

   /** \action Call Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle to obtain if object was warned last cycle. */
   result = Lcda_Was_Most_Critical_Slc_Obj_Last_Cycle(&slc_object, &slc_persistent);

   /** \assert Check if target object was most critical last cycle. */
   EXPECT_TRUE(result);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is behind the guard rail. Check
 * if target object is flagged as behind guardrail. \uts{CSCSA-42838} \sdd{SF-6734} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_true_when_object_is_behind_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_LEFT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = -5.0f;
   tracker_object.width     = 1.9f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as behind guardrail. */
   EXPECT_TRUE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is overlapping the guard rail.
 * Check if target object is flagged as not behind guardrail. \uts{CSCSA-42839} \sdd{SF-6734} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_false_when_object_is_on_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_LEFT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = -4.0f;
   tracker_object.width     = 2.0f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is between host and the guard
 * rail (not overlapping with any). Check if target object is flagged as not behind guardrail. \uts{CSCSA-42840} \sdd{SF-6734}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_false_when_object_is_ahead_of_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_LEFT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 1.9f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is behind the guard rail. Check
 * if target object is flagged as behind guardrail. \uts{CSCSA-42841} \sdd{SF-6734} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_true_when_object_is_behind_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_RIGHT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = 5.0f;
   tracker_object.width     = 1.9f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as behind guardrail. */
   EXPECT_TRUE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is overlapping the guard rail.
 * Check if target object is flagged as not behind guardrail. \uts{CSCSA-42842} \sdd{SF-6734} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_false_when_object_is_on_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_RIGHT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = 4.0f;
   tracker_object.width     = 2.0f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is between host and the guard
 * rail (not overlapping with any). Check if target object is flagged as not behind guardrail. \uts{CSCSA-42843} \sdd{SF-6734}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Is_Slc_Object_Behind_Guardrail__is_false_when_object_is_ahead_of_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t side    = FBK_SIDE_RIGHT;
   uint8_t obj_idx = 0;

   tracker_object.vcs_pos.y = 3.0f;
   tracker_object.width     = 1.9f;
   slc_object.ego_side      = side;
   tracker_object.index     = obj_idx;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Slc_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Slc_Object_Behind_Guardrail(&slc_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Set up SLC persistent data such that it does not equal its default values. Reset the SLC core output and check that the SLC
 * persistent data and output is reset for SLC. \uts{CSCSA-42844} \sdd{SF-6746} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Reset_Slc_Core__resets_slc_info_correctly)
{
   /** \arrange Set up SLC persistent data. */
   slc_core_output.f_slc_is_enabled                            = FBK_TRUE;
   slc_core_output.slc_alert[FBK_SIDE_LEFT]                    = FBK_TRUE;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_RIGHT]     = 4u;
   slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT]        = 7u;
   slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_RIGHT] = 7u;

   /** \action Call Lcda_Reset_Slc_Core to reset SLC core. */
   Lcda_Reset_Slc_Core(&slc_core_output, &slc_persistent);

   /** \assert Check if SLC output and persistent data was reset. */
   EXPECT_FALSE(slc_core_output.f_slc_is_enabled);
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}

/**
 * Fill SLC core output with values unequal zero. Clear the core output and check that all values equal the default values.
 * \uts{CSCSA-42845} \sdd{SF-6728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Clear_Slc_Core_Output_On_Side__clears_slc_output_as_expected)
{
   /** \arrange Fill SLC core output data with non-default values for one side. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]            = FBK_TRUE;
   slc_core_output.slc_index[FBK_SIDE_LEFT]            = 5u;
   slc_core_output.slc_id[FBK_SIDE_LEFT]               = 6u;
   slc_core_output.slc_unique_id[FBK_SIDE_LEFT]        = 6u;
   slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT]          = 3.0f;
   slc_core_output.slc_lon_ttc[FBK_SIDE_LEFT]          = 3.0f;
   slc_core_output.slc_lane_change_prob[FBK_SIDE_LEFT] = 0.5f;

   /** \action Call Lcda_Clear_Slc_Core_Output_On_Side to clear SLC core output data for tested side. */
   Lcda_Clear_Slc_Core_Output_On_Side(&slc_core_output, FBK_SIDE_LEFT);

   /** \assert Check that SLC core output data for tested side only contains default values. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lon_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lane_change_prob[FBK_SIDE_LEFT], 0.0f);
}

/**
 * Check that the SLC pre-processing resets the SLC core output.
 * \uts{CSCSA-42846} \sdd{SF-6744} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Preprocess_Slc__resets_slc_core_output)
{
   /** \arrange Fill SLC core output data of both sides with non-default values. */
   slc_core_output.slc_alert[FBK_SIDE_LEFT]             = FBK_TRUE;
   slc_core_output.slc_index[FBK_SIDE_LEFT]             = 5u;
   slc_core_output.slc_id[FBK_SIDE_RIGHT]               = 6u;
   slc_core_output.slc_unique_id[FBK_SIDE_RIGHT]        = 6u;
   slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT]          = 3.0f;
   slc_core_output.slc_lon_ttc[FBK_SIDE_RIGHT]          = 3.0f;
   slc_core_output.slc_lane_change_prob[FBK_SIDE_RIGHT] = 0.5f;

   /** \action Call Lcda_Preprocess_Slc to pre-process SLC. */
   Lcda_Preprocess_Slc(&slc_core_output, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC core output data only contains default values for both sides. */
   EXPECT_FALSE(slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(slc_core_output.slc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_core_output.slc_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_core_output.slc_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lon_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(slc_core_output.slc_lane_change_prob[FBK_SIDE_RIGHT], 0.0f);
}

/**
 * Fill SLC persistent data with values unequal zero. Clear the persistent data and check that all values equal zero.
 * \uts{CSCSA-42847} \sdd{SF-6729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Clear_Slc_Persistent__clears_slc_persistent_as_expected)
{
   /** \arrange Fill persistent data with non-zero values. */
   slc_persistent.mature_count_in_slc_zone[6u]             = 4u;
   slc_persistent.mature_count_in_slc_zone[12u]            = 6u;
   slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT]    = 3u;
   slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_RIGHT] = 3u;
   slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT]    = 6u;
   slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT]    = 7u;
   slc_persistent.slc_hold_counter[FBK_SIDE_RIGHT]         = 3u;

   /** \action Call Lcda_Clear_Slc_Persistent to clear SLC data. */
   Lcda_Clear_Slc_Persistent(&slc_persistent);

   /** \assert Check that SLC persistent data are reseted to default values. */

   EXPECT_EQ(slc_persistent.mature_count_in_slc_zone[6u], FBK_ZERO_UINT);
   EXPECT_EQ(slc_persistent.mature_count_in_slc_zone[12u], FBK_ZERO_UINT);
   EXPECT_EQ(slc_persistent.slc_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(slc_persistent.prev_slc_alert_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_persistent.prev_slc_alert_unique_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(slc_persistent.slc_hold_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Create a target object and initialize its SLC object data with default values and tracker data. Check that tracker data are
 * filled and default status values set. \uts{CSCSA-42848} \sdd{SF-6732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Init_Slc_Object_Data__inits_slc_obj_data_as_expected)
{
   /** \arrange Create SLC target object. */
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 4u, 2.0f, 4.0f, 0.5f);

   /** \action Call Lcda_Init_Slc_Object_Data to initialize target objects data. */
   Lcda_Init_Slc_Object_Data(&slc_object, &tracker_object);

   /** \assert Check that data is set correctly. */
   EXPECT_FLOAT_EQ(slc_object.lon_ttc, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(slc_object.lane_change_prob, FBK_ZERO_F);
   EXPECT_EQ(slc_object.p_tracker_data, &tracker_object);
}

/**
 * Create SLC warn relevant target with a smaller longitudinal TTC than the target object warned in the last cycle. Update the most
 * critical SLC object and check that the newly created target object replaces the last warned target. \uts{CSCSA-42849}
 * \sdd{SF-6736} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Most_Critical_Slc_Object__critical_obj_is_replaced_by_more_critical_obj)
{
   /** \arrange Set up persistent data and target object with more critical TTC. */
   uint8_t side = FBK_SIDE_LEFT;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 4u, 1.0f, 5.0f, 0.9f);

   slc_object.ego_side                        = side;
   slc_core_output.slc_index[side]            = 2u;
   slc_core_output.slc_id[side]               = 3u;
   slc_core_output.slc_unique_id[side]        = 4u;
   slc_core_output.slc_lon_ttc[side]          = 1.5f;
   slc_core_output.slc_lane_change_prob[side] = 0.8f;
   tracker_object.curvi_pos.x                 = -5.f;
   tracker_object.curvi_pos.y                 = -2.0f;
   tracker_object.curvi_heading               = 0.0f;
   tracker_object.length                      = 5.0f;
   tracker_object.width                       = 2.0f;
   tracker_object.curvi_vel_rel.x             = 10.0f;

   /** \action Call Lcda_Set_Most_Critical_Slc_Object to update most critical SLC object. */
   Lcda_Set_Most_Critical_Slc_Object(&slc_core_output, &slc_object);

   /** \assert Check that the target object replaces the alert data stored in the persistent data, and ttp is calulacted as
    * expected. */
   EXPECT_EQ(tracker_object.index, slc_core_output.slc_index[side]);
   EXPECT_EQ(tracker_object.id, slc_core_output.slc_id[side]);
   EXPECT_EQ(tracker_object.unique_id, slc_core_output.slc_unique_id[side]);
   EXPECT_EQ(slc_object.lon_ttc, slc_core_output.slc_lon_ttc[side]);
   EXPECT_EQ(slc_object.lane_change_prob, slc_core_output.slc_lane_change_prob[side]);
   EXPECT_EQ(slc_core_output.slc_ttp[side], 0.75f);
}

/**
 * Create SLC warn relevant target with a larger longitudinal TTC than the target object warned in the last cycle. Update the most
 * critical SLC object and check that the newly created target object does not replace the last warned target. \uts{CSCSA-42850}
 * \sdd{SF-6736} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Most_Critical_Slc_Object__critical_obj_is_not_replaced_by_less_critical_obj)
{
   /** \arrange Set up persistent data and target object with less critical TTC. */
   uint8_t side                   = FBK_SIDE_LEFT;
   uint8_t index                  = 2u;
   uint8_t id                     = 3u;
   uint32_t unique_id             = 4u;
   float32_T lon_ttc              = 1.5f;
   float32_T slc_lane_change_prob = 0.8f;
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 4u, 2.0f, 3.0f, 0.7f);

   slc_object.ego_side                        = side;
   slc_core_output.slc_index[side]            = index;
   slc_core_output.slc_id[side]               = id;
   slc_core_output.slc_unique_id[side]        = unique_id;
   slc_core_output.slc_lon_ttc[side]          = lon_ttc;
   slc_core_output.slc_lane_change_prob[side] = slc_lane_change_prob;

   /** \action Call Lcda_Set_Most_Critical_Slc_Object to update most critical SLC object. */
   Lcda_Set_Most_Critical_Slc_Object(&slc_core_output, &slc_object);

   /** \assert Check that the target object does not replace the alert data stored in the persistent data. */
   EXPECT_EQ(index, slc_core_output.slc_index[side]);
   EXPECT_EQ(id, slc_core_output.slc_id[side]);
   EXPECT_EQ(unique_id, slc_core_output.slc_unique_id[side]);
   EXPECT_EQ(lon_ttc, slc_core_output.slc_lon_ttc[side]);
   EXPECT_EQ(slc_lane_change_prob, slc_core_output.slc_lane_change_prob[side]);
}

/**
 * Test that the ego overlap offset for objects beside the ego is calculated properly.
 * \uts{CSCSA-42851} \sdd{SF-6928} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Get_Ego_Overlap_Offset__offset_is_calculated_properly)
{
   /** \arrange Set up ego speed such that it is between to values from the lookup-table. */
   float32_T ego_speed = 0.5f * (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);
   float32_T ego_overlap_offset;

   /** \action Call Lcda_Get_Ego_Overlap_Offset to calculate overlap offset for the given input speed. */
   ego_overlap_offset = Lcda_Get_Ego_Overlap_Offset(ego_speed, &lcda_cals);

   /** \assert Check that calculated ego overlap offset is located between the same values from look-up table. */
   EXPECT_GE(ego_overlap_offset, lcda_cals.k_slc_lookup_ego_overlap_offset[0]);
   EXPECT_LE(ego_overlap_offset, lcda_cals.k_slc_lookup_ego_overlap_offset[1]);
}

/**
 * Create an object such that an overlap is given.
 * \uts{CSCSA-42852} \sdd{SF-6955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Object_Position_Flags__f_obj_overlap_true)
{
   /** \arrange Set up object with overlap */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, -12.0f);
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0u, 0.5, 0.5, 0.5);
   p_vehicle_data->host_speed = 0.5f * (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);

   /** \action Call Lcda_Set_Object_Position_Flags to compute the SLC object flags. */
   Lcda_Set_Object_Position_Flags(&slc_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that f_obj_overlap is true. */
   EXPECT_TRUE(slc_object.f_obj_overlap);
}

/**
 * Create an object such that the object has overlap but is also besides the ego.
 * \uts{CSCSA-42853} \sdd{SF-6955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Object_Position_Flags__f_obj_besides_ego_true)
{
   /** \arrange Set up object with overlap that is also besides the ego */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, -12.0f);
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0u, 0.5, 0.5, 0.5);
   p_vehicle_data->host_speed = 0.5f * (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);

   /** \action Call Lcda_Set_Object_Position_Flags to compute the SLC object flags. */
   Lcda_Set_Object_Position_Flags(&slc_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that f_obj_besides_ego is true. */
   EXPECT_TRUE(slc_object.f_obj_besides_ego);
}

/**
 * Create an object such that the object has overlap but is also besides the ego, but will miss the ego in the future.
 * \uts{CSCSA-42854} \sdd{SF-6955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Object_Position_Flags__f_obj_misses_ego_true)
{
   /** \arrange Set up object with overlap */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, -12.0f);
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0u, 0.5, 0.5, 0.5);
   p_vehicle_data->host_speed = 0.5f * (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);

   /** \action Call Lcda_Set_Object_Position_Flags to compute the SLC object flags. */
   Lcda_Set_Object_Position_Flags(&slc_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that the target object is uncritical for SLC due to missing the host. */
   EXPECT_TRUE(slc_object.f_obj_misses_ego);
}

/**
 * Create an object such that an overlap is given.
 * \uts{CSCSA-42856} \sdd{SF-6955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Object_Position_Flags__object_away_ego)
{
   /** \arrange Set up object with overlap that is also besides the ego */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, -2.5f, 3.0f, -12.0f);
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0u, 0.5, 0.5, 0.5);
   p_vehicle_data->host_speed = (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);

   lcda_cals.k_slc_lookup_ego_overlap_offset[0] = 1.0f;
   lcda_cals.k_slc_lookup_ego_overlap_offset[1] = 1.0f;
   lcda_cals.k_slc_lookup_ego_overlap_offset[2] = 1.0f;
   tracker_object.vcs_pos.x = -(Fbk_Half(tracker_object.length) + p_vehicle_data->host_length + 1.0f - EPSILON);


   /** \action Call Lcda_Set_Object_Position_Flags to compute the SLC object flags. */
   Lcda_Set_Object_Position_Flags(&slc_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that f_obj_besides_ego is false. */
   EXPECT_FALSE(slc_object.f_obj_besides_ego);
}


/**
 * Create an object such that there is no overlap.
 * \uts{CSCSA-211416} \sdd{SF-6955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Set_Object_Position_Flags__f_obj_overlap_false)
{
   /** \arrange Set up object with overlap */
   Lcda_Create_Valid_Slc_Track(&tracker_object, 1u, -5.0f, 10.0f, 3.0f, -12.0f);
   Lcda_Create_Valid_Slc_Object(&slc_object, &tracker_object, 0u, 0.5, 0.5, 0.5);
   p_vehicle_data->host_speed = 0.5f * (lcda_cals.k_slc_lookup_ego_speed[0] + lcda_cals.k_slc_lookup_ego_speed[1]);

   /** \action Call Lcda_Set_Object_Position_Flags to compute the SLC object flags. */
   Lcda_Set_Object_Position_Flags(&slc_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that f_obj_overlap is true. */
   EXPECT_FALSE(slc_object.f_obj_overlap);
}


/**
 * Set up SLC data such that a SLC warning zone can be created. Add a lane center offset which affects the SLC zone. Check that the
 * SLC zone vertices have the expected coordinates. \uts{CSCSA-211417} \sdd{SF-6742} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Slc_Test, Lcda_Create_Slc_Object_Zone__zone_with_lane_center_offset2)
{
   /** \arrange Set up SLC data and target object. */
   slc_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0;
   tracker_object.width = -1.0f;

   // Set a non-zero lane center offset
   lcda_core_input.lane_center_offset = 1.0f;

   /** \action Call Lcda_Create_Slc_Object_Zone to create SLC zone. */
   Lcda_Create_Slc_Object_Zone(&slc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that SLC zone vertices have correct coordinates. */
   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[0]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[1]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[2]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[3]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[4]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_slc_zone_y[5]) + lcda_core_input.lane_center_offset));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(slc_object.zone.points[0].x, lcda_cals.k_slc_zone_x[0]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[1].x, lcda_cals.k_slc_zone_x[1]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[2].x, lcda_cals.k_slc_zone_x[2]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[3].x, lcda_cals.k_slc_zone_x[3]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[4].x, lcda_cals.k_slc_zone_x[4]);
   EXPECT_FLOAT_EQ(slc_object.zone.points[5].x, lcda_cals.k_slc_zone_x[5]);
}
