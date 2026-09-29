/**
 * @file lcda_process_elc_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_process_elc.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42764}
 */

#include "lcda_process_elc_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_process_elc.c"
#include "lcda_types.h"
#include "pa_const_macros.h"
}


static void Lcda_Create_Valid_Elc_Object(Elc_Object_T *p_elc_object,
                                         Fbk_Object_Data_T *p_tracker_object,
                                         uint8_t obj_id,
                                         float32_T lon_ttc,
                                         float32_T obj_decel_to_reach_host_speed)
{


   p_tracker_object->index                     = obj_id - 1u;
   p_tracker_object->id                        = obj_id;
   p_elc_object->lon_ttc                       = lon_ttc;
   p_elc_object->obj_decel_to_reach_host_speed = obj_decel_to_reach_host_speed;
}

/**
 * Set up ELC persistent data such that it does not equal its default values. Reset the ELC core output and check that the ELC
 * persistent data and output is reset for ELC. \uts{CSCSA-42765} \sdd{SF-6721} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Reset_Elc_Core__resets_elc_info_correctly)
{
   /** \arrange Set up ELC persistent data. */
   elc_core_output.f_elc_is_enabled                       = FBK_TRUE;
   elc_core_output.elc_alert[FBK_SIDE_LEFT]               = FBK_TRUE;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT] = 4u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT]   = 5u;

   /** \action Call Lcda_Reset_Elc_Core to reset ELC core. */
   Lcda_Reset_Elc_Core(&elc_core_output, &elc_persistent);

   /** \assert Check if ELC output and persistent data was reset. */
   EXPECT_FALSE(elc_core_output.f_elc_is_enabled);
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}

/**
 * Create ELC warn relevant target object on the right side which is fully qualified. Process the ELC warning state and check that
 * an alert level is raised. \uts{CSCSA-42766} \sdd{SF-6720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__alerts_on_critical_obj_on_RIGHT)
{
   /** \arrange Set up target object. */
   uint8_t critical_obj_id = 5u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, 5.0f, -30.0f, -3.0f, 15.0f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id] = lcda_cals.k_elc_min_mature_cycles;

   lcda_cals.k_elc_critical_longitudinal_ttc = 3.0f;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_RIGHT], tracker_object.index);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_RIGHT], critical_obj_id);
   EXPECT_TRUE(elc_core_output.elc_alert[FBK_SIDE_RIGHT]);
}

/**
 * Create ELC warn relevant target object on the left side which is fully qualified. Process the ELC warning state and check that
 * an alert level is raised. \uts{CSCSA-42767} \sdd{SF-6720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__alerts_on_critical_obj_on_LEFT)
{
   /** \arrange Set up target object. */
   uint8_t critical_obj_id = 3u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, -5.0f, -30.0f, 3.0f, 15.0f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id] = lcda_cals.k_elc_min_mature_cycles + 1u;

   lcda_cals.k_elc_critical_longitudinal_ttc = 3.0f;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the correct target object is warned and that an alert state level is raised. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], tracker_object.index);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], critical_obj_id);
   EXPECT_TRUE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create two ELC warn relevant target objects on the left side which are fully qualified. Process the ELC warning state and check
 * that an alert level is raised for the longitudinal closer target object. \uts{CSCSA-42768} \sdd{SF-6720}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__no_alert_on_obj_when_long_ttc_higher_than_critical_ttc)
{
   /** \arrange Set up target objects with one being closer (longitudinal) to the host. */
   uint8_t critical_obj_id      = 2u;
   uint8_t less_critical_obj_id = 5u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, -5.0f, -40.0f, 3.0f, 12.0f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id]      = lcda_cals.k_elc_min_mature_cycles;
   elc_persistent.mature_count_in_elc_zone[less_critical_obj_id] = lcda_cals.k_elc_min_mature_cycles;

   lcda_cals.k_elc_critical_longitudinal_ttc       = 12.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold = 4.0f;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state for both target objects. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);
   Lcda_Create_Valid_Elc_Track(&tracker_object, less_critical_obj_id, -5.0f, -30.0f, 4.0f, 2.0f);
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the closer target object is warned and that an alert state level is raised. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], critical_obj_id - 1u);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], critical_obj_id);
   EXPECT_TRUE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
}

/**
 * Create a valid ELC target object outside of the ELC zone with a mature ELC zone counter greater than zero. Process the ELC
 * warning state and check that the mature ELC zone counter is reset. \uts{CSCSA-42769} \sdd{SF-6720}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__counter_reset_for_obj_outside_zone)
{
   /** \arrange Set up target object and mature counter. */
   uint8_t obj_id = 5u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_id, 0.0f, 0.0f, 0.0f, 0.0f);
   elc_persistent.mature_count_in_elc_zone[obj_id] = lcda_cals.k_elc_min_mature_cycles;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the mature ELC zone counter is reset. */
   EXPECT_EQ(elc_persistent.mature_count_in_elc_zone[obj_id], 0u);
}

/**
 * Fill ELC persistent data with values unequal zero. Clear the persistent data and check that all values equal zero.
 * \uts{CSCSA-42770} \sdd{SF-6705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Clear_Elc_Persistent__clears_elc_persistent_as_expected)
{
   /** \arrange Fill persistent data with non-zero values. */
   uint8_t iobj;
   uint8_t side;
   boolean_T result = FBK_TRUE;

   elc_persistent.mature_count_in_elc_zone[FBK_SIDE_LEFT]  = 4u;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT]  = 3u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]     = 7u;
   elc_persistent.mature_count_in_elc_zone[FBK_SIDE_RIGHT] = 3u;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_RIGHT] = 4u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT]    = 8u;

   /** \action Call Lcda_Clear_Elc_Persistent to clear ELC data. */
   Lcda_Clear_Elc_Persistent(&elc_persistent);

   /** \assert Check that ELC persistent data are all zero. */
   for (side = 0; side < FBK_NUMBER_OF_SIDES; side++)
   {
      for (iobj = 0; iobj < PA_OBJ_NUMBER_OF_OBJECTS; iobj++)
      {
         if (elc_persistent.mature_count_in_elc_zone[iobj] != 0)
         {
            result = FBK_FALSE;
            break;
         }
      }
   }

   EXPECT_TRUE(result);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}

/**
 * Create a target object and initialize its ELC object data with default values and tracker data. Check that tracker data are
 * filled and default status values set. \uts{CSCSA-42771} \sdd{SF-6707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Init_Elc_Object_Data__inits_elc_obj_data_as_expected)
{
   /** \arrange Create ELC target object. */
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 4u, 2.0f, 4.0f);

   /** \action Call Lcda_Init_Elc_Object_Data to initialize target objects data. */
   Lcda_Init_Elc_Object_Data(&elc_object, &tracker_object);

   /** \assert Check that data is set correctly. */
   EXPECT_FLOAT_EQ(elc_object.lon_ttc, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(elc_object.obj_decel_to_reach_host_speed, 0.0f);
   EXPECT_EQ(elc_object.p_tracker_data, &tracker_object);
}

/**
 * Fill ELC core output with values unequal zero. Clear the core output and check that all values equal the default values.
 * \uts{CSCSA-42772} \sdd{SF-6704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Clear_Elc_Core_Output_On_Side__clears_elc_output_as_expected)
{
   /** \arrange Fill ELC core output data with non-default values for one side. */
   elc_core_output.elc_alert[FBK_SIDE_LEFT]                     = FBK_TRUE;
   elc_core_output.elc_index[FBK_SIDE_LEFT]                     = 5u;
   elc_core_output.elc_id[FBK_SIDE_LEFT]                        = 6u;
   elc_core_output.elc_ttc[FBK_SIDE_LEFT]                       = 3.0f;
   elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_LEFT] = 2.0f;

   /** \action Call Lcda_Clear_Elc_Core_Output_On_Side to clear ELC core output data for tested side. */
   Lcda_Clear_Elc_Core_Output_On_Side(&elc_core_output, FBK_SIDE_LEFT);

   /** \assert Check that ELC core output data for tested side only contains default values. */
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(elc_core_output.elc_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_LEFT], 0.0f);
}

/**
 * Check that the ELC pre-processing resets the ELC core output.
 * \uts{CSCSA-42773} \sdd{SF-6719} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Preprocess_Elc__resets_elc_core_output)
{
   /** \arrange Fill ELC core output data of both sides with non-default values. */
   elc_core_output.elc_alert[FBK_SIDE_LEFT]                      = FBK_TRUE;
   elc_core_output.elc_index[FBK_SIDE_LEFT]                      = 5u;
   elc_core_output.elc_id[FBK_SIDE_RIGHT]                        = 6u;
   elc_core_output.elc_ttc[FBK_SIDE_RIGHT]                       = 3.0f;
   elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_RIGHT] = 2.0f;

   /** \action Call Lcda_Preprocess_Elc to pre-process ELC. */
   Lcda_Preprocess_Elc(&elc_core_output, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC core output data only contains default values for both sides. */
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(elc_core_output.elc_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_RIGHT], 0.0f);
}

/**
 * Set up ELC data such that a ELC warning zone can be created. Check that the ELC zone vertices have the expected coordinates.
 * \uts{CSCSA-42774} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__creates_zone_zero_lane_center_offset)
{

   /** \arrange Set up ELC data and target object. */
   elc_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 2u;

   lcda_cals.k_lcda_min_lane_width           = 2.0f;
   lcda_cals.k_zone_hys_obj_width_correction = 0;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */
   Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_EQ(elc_object.zone.size, LCDA_NUMBER_OF_ZONE_POINTS);

   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[0]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[1]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[2]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[3]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[4]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].x, lcda_cals.k_elc_zone_x[0]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].x, lcda_cals.k_elc_zone_x[1]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].x, lcda_cals.k_elc_zone_x[2]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].x, lcda_cals.k_elc_zone_x[3]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].x, lcda_cals.k_elc_zone_x[4]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].x, lcda_cals.k_elc_zone_x[5]);
}

/**
 * Set up ELC data such that a ELC warning zone can be created. Add a lane center offset which affects the ELC zone. Check that the
 * ELC zone vertices have the expected coordinates. \uts{CSCSA-42775} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__creates_zone_adding_lane_center_offset)
{
   /** \arrange Set up ELC data and target object. */
   elc_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 2u;

   // Set a non-zero lane center offset
   lcda_core_input.lane_center_offset = 1.0f;
   lcda_cals.k_lcda_min_lane_width    = 2.0f;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */
   Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_EQ(elc_object.zone.size, LCDA_NUMBER_OF_ZONE_POINTS);

   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[0]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[1]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[2]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[3]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[4]) + lcda_core_input.lane_center_offset));
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].y,
                   ((lcda_core_input.lane_width * lcda_cals.k_elc_zone_y[5]) + lcda_core_input.lane_center_offset));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].x, lcda_cals.k_elc_zone_x[0]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].x, lcda_cals.k_elc_zone_x[1]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].x, lcda_cals.k_elc_zone_x[2]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].x, lcda_cals.k_elc_zone_x[3]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].x, lcda_cals.k_elc_zone_x[4]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].x, lcda_cals.k_elc_zone_x[5]);
}

/**
 * Set up target object indices for both sides in ELC core output. Write core ELC output data to persistent ELC data. Check that
 * the persistent data contains the correct target object indices. \uts{CSCSA-42776} \sdd{SF-6706} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Fill_Side_Persistent_Elc_Data__sets_alert_from_last_cycle_correctly)
{
   /** \arrange Set up target object values for both sides in ELC core output. */
   elc_core_output.elc_index[FBK_SIDE_LEFT]  = 5u;
   elc_core_output.elc_index[FBK_SIDE_RIGHT] = 6u;
   elc_core_output.elc_id[FBK_SIDE_LEFT]     = 8u;
   elc_core_output.elc_id[FBK_SIDE_RIGHT]    = 9u;

   /** \action Call Lcda_Fill_Side_Persistent_Elc_Data to write core output data to persistent data. */
   Lcda_Fill_Side_Persistent_Elc_Data(&elc_persistent, &elc_core_output);

   /** \assert Check that core output and persistent values are equal. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_RIGHT], elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_RIGHT]);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_RIGHT], elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT]);
}

/**
 * Check that for an active alert in the current cycle the ELC post-processing resets the holding counter and fills the persistent
 * data according to this alert. \uts{CSCSA-42777} \sdd{SF-6718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Postprocess_Elc__resets_holding_counter_and_fills_persistent_data_for_active_alert)
{
   /** \arrange Set up core output with an alert and persistent data with holding counter unequal zero. */
   elc_core_output.elc_index[FBK_SIDE_LEFT]       = 12u;
   elc_core_output.elc_id[FBK_SIDE_LEFT]          = 3u;
   elc_core_output.elc_alert[FBK_SIDE_LEFT]       = FBK_TRUE;
   elc_persistent.elc_hold_counter[FBK_SIDE_LEFT] = 4u;

   /** \action Call function Lcda_Postprocess_Elc for post-processing of current ELC cycle. */
   Lcda_Postprocess_Elc(&elc_core_output, &lcda_cals, &elc_persistent);

   /** \assert Verify that holding counter is reseted and previous alert is set in persistent data. */
   EXPECT_TRUE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT], elc_core_output.elc_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT], elc_core_output.elc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_persistent.elc_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Create ELC warn relevant target with a smaller longitudinal TTC than the target object warned in the last cycle. Update the most
 * critical ELC object and check that the newly created target object replaces the last warned target. \uts{CSCSA-42778}
 * \sdd{SF-6711} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Set_Most_Critical_Elc_Object__critical_obj_is_replaced_by_more_critical_obj)
{
   /** \arrange Set up persistent data and target object with more critical TTC. */
   uint8_t side = FBK_SIDE_LEFT;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 4u, 1.0f, 5.0f);

   elc_object.ego_side                                 = side;
   elc_core_output.elc_index[side]                     = 2u;
   elc_core_output.elc_id[side]                        = 3u;
   elc_core_output.elc_ttc[side]                       = 1.5f;
   elc_core_output.elc_decel_to_reach_host_speed[side] = 4.0f;

   /** \action Call Lcda_Set_Most_Critical_Elc_Object to update most critical ELC object. */
   Lcda_Set_Most_Critical_Elc_Object(&elc_core_output, &elc_object);

   /** \assert Check that the target object replaces the alert data stored in the persistent data. */
   EXPECT_EQ(tracker_object.index, elc_core_output.elc_index[side]);
   EXPECT_EQ(tracker_object.id, elc_core_output.elc_id[side]);
   EXPECT_EQ(elc_object.lon_ttc, elc_core_output.elc_ttc[side]);
   EXPECT_EQ(elc_object.obj_decel_to_reach_host_speed, elc_core_output.elc_decel_to_reach_host_speed[side]);
}

/**
 * Create ELC warn relevant target with a larger longitudinal TTC than the target object warned in the last cycle. Update the most
 * critical ELC object and check that the newly created target object does not replace the last warned target. \uts{CSCSA-42779}
 * \sdd{SF-6711} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Set_Most_Critical_Elc_Object__critical_obj_is_not_replaced_by_less_critical_obj)
{
   /** \arrange Set up persistent data and target object with less critical TTC. */
   uint8_t side                            = FBK_SIDE_LEFT;
   uint8_t index                           = 2u;
   uint8_t id                              = 3u;
   float32_T lon_ttc                       = 1.5f;
   float32_T obj_decel_to_reach_host_speed = 4.0f;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 4u, 2.0f, 3.0f);

   elc_object.ego_side                                 = side;
   elc_core_output.elc_index[side]                     = index;
   elc_core_output.elc_id[side]                        = id;
   elc_core_output.elc_ttc[side]                       = lon_ttc;
   elc_core_output.elc_decel_to_reach_host_speed[side] = obj_decel_to_reach_host_speed;

   /** \action Call Lcda_Set_Most_Critical_Elc_Object to update most critical ELC object. */
   Lcda_Set_Most_Critical_Elc_Object(&elc_core_output, &elc_object);

   /** \assert Check that the target object does not replace the alert data stored in the persistent data. */
   EXPECT_EQ(index, elc_core_output.elc_index[side]);
   EXPECT_EQ(id, elc_core_output.elc_id[side]);
   EXPECT_EQ(lon_ttc, elc_core_output.elc_ttc[side]);
   EXPECT_EQ(obj_decel_to_reach_host_speed, elc_core_output.elc_decel_to_reach_host_speed[side]);
}

/**
 * Create ELC warn relevant target object on the left side and set persistent data such that it corresponds to the last warned
 * target object. Update if the target object from last cycle was the most critical and check that this is true. \uts{CSCSA-42780}
 * \sdd{SF-6712} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle__is_TRUE_obj_critial_on_FBK_SIDE_LEFT_in_last_cycle)
{
   /** \arrange Set up persistent data and target object. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 10.0f);

   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]  = 3u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT] = 2u;

   /** \action Call Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle to obtain if target object was most critital in last cycle. */
   result = Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(&elc_object, &elc_persistent);

   /** \assert Check that the target object was most critical in last cycle. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object on the right side and set persistent data such that it corresponds to the last warned
 * target object. Update if the target object from last cycle was the most critical and check that this is true. \uts{CSCSA-42781}
 * \sdd{SF-6712} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle__is_TRUE_obj_critial_FBK_SIDE_RIGHT_in_last_cycle)
{
   /** \arrange Set up persistent data and target object. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 10.0f);

   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]  = 2u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT] = 3u;

   /** \action Call Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle to obtain if target object was most critital in last cycle. */
   result = Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(&elc_object, &elc_persistent);

   /** \assert Check that the target object was most critical in last cycle. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object on the right side and set persistent data such that it does not corresponds to the last
 * warned target object. Update if the target object from last cycle was the most critical and check that this is false.
 * \uts{CSCSA-42782} \sdd{SF-6712} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle__is_FALSE_obj_not_critial_in_last_cycle)
{
   /** \arrange Set up persistent data and target object. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 10.0f);

   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]  = 2u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_RIGHT] = 2u;

   /** \action Call Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle to obtain if target object was most critital in last cycle. */
   result = Lcda_Was_Most_Critical_Elc_Obj_Last_Cycle(&elc_object, &elc_persistent);

   /** \assert Check that the target object was not most critical in last cycle. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with a TTC below the specified threshold. Compute if the objects TTC is below the
 * threshold and check that this is true. \uts{CSCSA-42783} \sdd{SF-6710} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Ttc_Below_Threshold__is_TRUE_ttc_is_below_critical_value)
{
   /** \arrange Set up target object data such that its TTC is below the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, 1.0f, 10.0f);

   lcda_cals.k_elc_critical_longitudinal_ttc_hys = 4.0f;
   lcda_cals.k_elc_critical_longitudinal_ttc     = 2.0f;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Threshold(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the TTC is below the threshold. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with a TTC above the specified threshold. Compute if the objects TTC is below the
 * threshold and check that this is false. \uts{CSCSA-42784} \sdd{SF-6710} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Ttc_Below_Threshold__is_FALSE_ttc_is_above_critical_value)
{
   /** \arrange Set up target object data such that its TTC is above the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, 5.0f, 10.0f);

   lcda_cals.k_elc_critical_longitudinal_ttc_hys = 4.0f;
   lcda_cals.k_elc_critical_longitudinal_ttc     = 2.0f;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Threshold(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with a TTC above the specified threshold but below the hysteresis threshold.
 * Additionally, the target object was warned last cycle. Compute if the objects TTC is below the threshold and check that this is
 * true. \uts{CSCSA-42785} \sdd{SF-6710} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Ttc_Below_Threshold__is_TRUE_ttc_is_below_critical_hys_value_and_obj_critial_in_last_cycle)
{
   /** \arrange Set up target object data such that its TTC is above the specified threshold but below the hysteresis threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, 3.0f, 10.0f);

   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT] = 3;
   lcda_cals.k_elc_critical_longitudinal_ttc_hys       = 4.0f;
   lcda_cals.k_elc_critical_longitudinal_ttc           = 2.0f;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Threshold(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the TTC is below the threshold. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with a TTC above both the specified threshold and the hysteresis threshold. Additionally,
 * the target object was warned last cycle. Compute if the objects TTC is below the threshold and check that this is false.
 * \uts{CSCSA-42786} \sdd{SF-6710} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Ttc_Below_Threshold__is_FALSE_ttc_is_above_critical_value_and_obj_not_critial_in_last_cycle)
{
   /** \arrange Set up target object data such that its TTC is above both the specified threshold and the hysteresis threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, 3.0f, 10.0f);

   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT]  = 2;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_RIGHT] = 2;
   lcda_cals.k_elc_critical_longitudinal_ttc_hys           = 4.0f;
   lcda_cals.k_elc_critical_longitudinal_ttc               = 2.0f;

   /** \action Call Lcda_Is_Ttc_Below_Threshold to check if TTC is below the threshold. */
   result = Lcda_Is_Ttc_Below_Threshold(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the TTC is not below the threshold. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with a deceleration above the specified threshold. Compute if the objects deceleration is
 * above the threshold and check that it is evaluated critical. \uts{CSCSA-42787} \sdd{SF-6708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Deceleration_Critical__is_TRUE_deceleration_is_above_threshold)
{
   /** \arrange Set up target object data such that its develeration is above the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 5.0f);

   lcda_cals.k_elc_obj_safe_deceleration_threshold_hys = 2.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold     = 4.0f;

   /** \action Call Lcda_Is_Deceleration_Critical to check if deceleration is critical. */
   result = Lcda_Is_Deceleration_Critical(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the deceleration is critical. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with a deceleration below the specified threshold. Compute if the objects deceleration is
 * critical and check that it is evaluated critical. \uts{CSCSA-42788} \sdd{SF-6708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Deceleration_Critical__is_FALSE_deceleration_is_below_threshold)
{
   /** \arrange Set up target object data such that its deceleration is below the specified threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 1.0f);

   lcda_cals.k_elc_obj_safe_deceleration_threshold_hys = 2.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold     = 4.0f;

   /** \action Call Lcda_Is_Deceleration_Critical to check if deceleration is critical. */
   result = Lcda_Is_Deceleration_Critical(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the deceleration is critical. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with a deceleration below the specified threshold but above the hysteresis threshold.
 * Additionally, the target object was warned last cycle. Compute if the objects deceleration is critical and check that this is
 * true. \uts{CSCSA-42789} \sdd{SF-6708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Deceleration_Critical__is_TRUE_deceleration_above_threshold_hys_and_obj_critial_in_last_cycle)
{
   /** \arrange Set up target object data such that its deceleration is below the specified threshold but above the hysteresis
    * threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 3.0f);

   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT] = 3u;
   lcda_cals.k_elc_obj_safe_deceleration_threshold_hys = 2.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold     = 4.0f;

   /** \action Call Lcda_Is_Deceleration_Critical to check if deceleration is critical. */
   result = Lcda_Is_Deceleration_Critical(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the deceleration is critical. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with a deceleration below both the specified threshold and the hysteresis threshold.
 * Additionally, the target object was warned last cycle. Compute if the objects deceleration is critical and check that this is
 * false. \uts{CSCSA-42790} \sdd{SF-6708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Deceleration_Critical__is_FALSE_deceleration_below_threshold_and_obj_not_critial_in_last_cycle)
{
   /** \arrange Set up target object data such that its deceleration is below both the specified threshold and the hysteresis
    * threshold. */
   boolean_T result;
   Lcda_Create_Valid_Elc_Object(&elc_object, &tracker_object, 3u, LCDA_DEFAULT_LARGE_TTC, 3.0f);

   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT]  = 2u;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_RIGHT] = 2u;
   lcda_cals.k_elc_obj_safe_deceleration_threshold_hys     = 2.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold         = 4.0f;

   /** \action Call Lcda_Is_Deceleration_Critical to check if deceleration is critical. */
   result = Lcda_Is_Deceleration_Critical(&elc_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the deceleration is not critical. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with status mature. Compute if the target object is ELC relevant and check that this is
 * true. \uts{CSCSA-42791} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_TRUE_track_status_MATURE)
{
   /** \arrange Set up target object data with status mature. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status = PA_OBJ_STATUS_MATURE;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with status coasted. Compute if the target object is ELC relevant and check that this is
 * true. \uts{CSCSA-42792} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_TRUE_track_status_COASTED)
{
   /** \arrange Set up target object data with status coasted. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status = PA_OBJ_STATUS_COASTED;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create ELC warn relevant target object with status new. Compute if the target object is ELC relevant and check that this is
 * true. \uts{CSCSA-42793} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_track_status_NEW)
{
   /** \arrange Set up target object data with status new. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status = PA_OBJ_STATUS_NEW;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn relevant target object with status invalid. Compute if the target object is ELC relevant and check that this is
 * true. \uts{CSCSA-42794} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_track_status_INVALID)
{
   /** \arrange Set up target object data with status invalid. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status = PA_OBJ_STATUS_INVALID;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC target object with too large heading. Compute if the target object is ELC relevant and check that this is false.
 * \uts{CSCSA-42795} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_obj_heading_too_large)
{
   /** \arrange Set up target object data with large heading. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.curvi_heading = Fbk_Abs_F(lcda_cals.k_elc_max_curvi_heading_abs * 2.0f);

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC target object with too small velocity. Compute if the target object is ELC relevant and check that this is false.
 * \uts{CSCSA-42796} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_obj_velocity_too_low)
{
   /** \arrange Set up target object data with small velocity. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.curvi_vel.x = Fbk_Abs_F(lcda_cals.k_elc_min_obj_curvi_long_vel_abs / 2.0f);

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Check that ELC alert is held if no active alert is present and holding counter is below threshold.
 * \uts{CSCSA-42797} \sdd{SF-6924} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Output__holds_alert_if_holding_counter_below_threshold)
{
   /** \arrange Set up core output and persistent data, such that alert holding is expected. */
   lcda_cals.k_elc_alert_holding_cycles = 4u;

   elc_core_output.elc_alert[FBK_SIDE_LEFT]               = FBK_FALSE;
   elc_persistent.elc_hold_counter[FBK_SIDE_LEFT]         = lcda_cals.k_elc_alert_holding_cycles - 1u;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT] = 4u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]    = 4u;

   /** \action Call Lcda_Process_Elc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Elc_Output(&elc_core_output, &elc_persistent, &lcda_cals);

   /** \assert Check that ELC alert is held and holding counter is increased. */
   EXPECT_TRUE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_persistent.elc_hold_counter[FBK_SIDE_LEFT], lcda_cals.k_elc_alert_holding_cycles);
}

/**
 * Check that ELC alert is not held if no active alert is present and holding counter is above threshold.
 * \uts{CSCSA-42798} \sdd{SF-6924} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Output__does_not_hold_alert_if_holding_counter_above_threshold)
{
   /** \arrange Set up core output and persistent data, such that no alert holding is expected. */
   lcda_cals.k_elc_alert_holding_cycles = 4u;

   elc_core_output.elc_alert[FBK_SIDE_LEFT]               = FBK_FALSE;
   elc_core_output.elc_index[FBK_SIDE_LEFT]               = PA_INVALID_OBJ_INDEX;
   elc_core_output.elc_id[FBK_SIDE_LEFT]                  = PA_INVALID_OBJ_ID;
   elc_persistent.elc_hold_counter[FBK_SIDE_LEFT]         = lcda_cals.k_elc_alert_holding_cycles;
   elc_persistent.prev_elc_alert_obj_index[FBK_SIDE_LEFT] = 4u;
   elc_persistent.prev_elc_alert_obj_id[FBK_SIDE_LEFT]    = 4u;

   /** \action Call Lcda_Process_Elc_Output to determine if alert shall be written to core output. */
   Lcda_Process_Elc_Output(&elc_core_output, &elc_persistent, &lcda_cals);

   /** \assert Check that ELC alert is not held and holding counter is reset. */
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(elc_persistent.elc_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Set up ELC data such that a ELC warning zone can be created. Check that the ELC zone vertices have the expected coordinates.
 * \uts{CSCSA-42799} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__lane_width_lower_than_calibration)
{

   /** \arrange Set up ELC data and target object. */
   elc_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 2u;

   lcda_cals.k_lcda_min_lane_width           = lcda_core_input.lane_width + EPSILON;
   lcda_cals.k_zone_hys_obj_width_correction = 0;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */
   Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_EQ(elc_object.zone.size, LCDA_NUMBER_OF_ZONE_POINTS);

   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[0]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[1]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[2]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[3]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[4]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].y, (lcda_cals.k_lcda_min_lane_width * -lcda_cals.k_elc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].x, lcda_cals.k_elc_zone_x[0]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].x, lcda_cals.k_elc_zone_x[1]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].x, lcda_cals.k_elc_zone_x[2]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].x, lcda_cals.k_elc_zone_x[3]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].x, lcda_cals.k_elc_zone_x[4]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].x, lcda_cals.k_elc_zone_x[5]);
}

/**
 * Set up ELC data such that a ELC warning zone can be created. Check that the ELC zone vertices have the expected coordinates.
 * \uts{CSCSA-42800} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__lane_width_greater_than_calibration)
{

   /** \arrange Set up ELC data and target object. */
   elc_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 2u;

   lcda_cals.k_lcda_min_lane_width           = lcda_core_input.lane_width - EPSILON;
   lcda_cals.k_zone_hys_obj_width_correction = 0;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */
   Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_EQ(elc_object.zone.size, LCDA_NUMBER_OF_ZONE_POINTS);

   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[0]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[1]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[2]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[3]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[4]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].x, lcda_cals.k_elc_zone_x[0]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].x, lcda_cals.k_elc_zone_x[1]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].x, lcda_cals.k_elc_zone_x[2]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].x, lcda_cals.k_elc_zone_x[3]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].x, lcda_cals.k_elc_zone_x[4]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].x, lcda_cals.k_elc_zone_x[5]);
}

/**
 * Set up ELC data such that assertion is thrown due to wrong calibration.
 * \uts{CSCSA-42801} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__assert_wrong_calibration)
{

   /** \arrange Set up ELC data and target object. */
   lcda_cals.k_zone_hys_obj_width_correction = -1.0;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_DEBUG_DEATH({ Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals); }, "");
}


/**
 * Create ELC warn target object with status coasted and existance probability 0. Compute if the target object is ELC relevant and
 * check that this is false. \uts{CSCSA-211410} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_status_COASTED_exist_prob_below_thresh)
{
   /** \arrange Set up target object data with status coasted. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.existence_probability = 0.0f;

   lcda_cals.k_lcda_min_exist_prop = 0.7f;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Create ELC warn target object with status mature and existance probability 0. Compute if the target object is ELC relevant and
 * check that this is false. \uts{CSCSA-211411} \sdd{SF-6709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Is_Object_Relevant_For_Elc__is_FALSE_status_MATURE_exist_prob_below_thresh)
{
   /** \arrange Set up target object data with status coasted. */
   boolean_T result;
   uint8_t obj_idx = 5u;
   Lcda_Create_Valid_Elc_Track(&tracker_object, obj_idx, -5.0f, -40.0f, 3.0f, -12.0f);
   tracker_object.status                = PA_OBJ_STATUS_COASTED;
   tracker_object.existence_probability = 0.0f;
   lcda_cals.k_lcda_min_exist_prop      = 0.7f;

   /** \action Call Lcda_Is_Object_Relevant_For_Elc to check if target object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Elc(&lcda_core_input, &tracker_object, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object is not relevant. */
   EXPECT_FALSE(result);
}


/**
 * Set up ELC data such that a ELC warning zone can be created. Check that the ELC zone vertices have the expected coordinates.
 * \uts{CSCSA-211412} \sdd{SF-6717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Create_Elc_Object_Zone__creates_zone_hysteresis_offset)
{

   /** \arrange Set up ELC data and target object. */
   elc_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 2u;
   tracker_object.width = -1.0f;

   lcda_cals.k_lcda_min_lane_width           = 2.0f;
   lcda_cals.k_zone_hys_obj_width_correction = 1.0f;

   /** \action Call Lcda_Create_Elc_Object_Zone to create ELC zone. */
   Lcda_Create_Elc_Object_Zone(&elc_object, 0u, &lcda_core_input, &lcda_cals);

   /** \assert Check that ELC zone vertices have correct coordinates. */
   EXPECT_EQ(elc_object.zone.size, LCDA_NUMBER_OF_ZONE_POINTS);

   // zone y-co ord should be a multiple of the lane width
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[0]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[1]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[2]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[3]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[4]));
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].y, (lcda_core_input.lane_width * -lcda_cals.k_elc_zone_y[5]));

   // zone x coord should be equal to the calibrations
   EXPECT_FLOAT_EQ(elc_object.zone.points[0].x, lcda_cals.k_elc_zone_x[0]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[1].x, lcda_cals.k_elc_zone_x[1]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[2].x, lcda_cals.k_elc_zone_x[2]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[3].x, lcda_cals.k_elc_zone_x[3]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[4].x, lcda_cals.k_elc_zone_x[4]);
   EXPECT_FLOAT_EQ(elc_object.zone.points[5].x, lcda_cals.k_elc_zone_x[5]);
}


/**
 * Create ELC target object on the left side which has negative ttc and thus expect no alert and default values of output id.
 * \uts{CSCSA-211413} \sdd{SF-6720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__no_alert_on_obj_when_ttc_negative)
{
   /** \arrange Set up target object with negative ttc. */
   uint8_t critical_obj_id = 2u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, -5.0f, -5.0f, 3.0f, 100.0f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id] = lcda_cals.k_elc_min_mature_cycles;

   lcda_cals.k_elc_critical_longitudinal_ttc       = 12.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold = 4.0f;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state for target. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object does not trigger an alert. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], 255u);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], 0u);
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
}


/**
 * Create ELC target object on the left side with too high ttc. Process the ELC warning state and check that there is no alert.
 * \uts{CSCSA-211414} \sdd{SF-6720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__no_alert_when_ttc_too_high)
{
   /** \arrange Set up target object with too high ttc. */
   uint8_t critical_obj_id = 2u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, -5.0f, -10.0f, 3.0f, 0.01f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id] = lcda_cals.k_elc_min_mature_cycles;

   lcda_cals.k_elc_critical_longitudinal_ttc       = 12.0f;
   lcda_cals.k_elc_obj_safe_deceleration_threshold = 4.0f;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state for target. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the target object does not trigger an alert. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], 255u);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], 0u);
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
}


/**
 * Create ELC target object on the left side which is not qualified due to mature count in zone. Process the ELC warning state and
 * check that an alert level is not raised. \uts{CSCSA-211415} \sdd{SF-6720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Elc_Test, Lcda_Process_Elc_Object__no_alert_mature_count_too_low)
{
   /** \arrange Set up target object. */
   uint8_t critical_obj_id = 3u;

   Lcda_Create_Valid_Elc_Track(&tracker_object, critical_obj_id, -5.0f, -30.0f, 3.0f, 15.0f);
   elc_persistent.mature_count_in_elc_zone[critical_obj_id] = lcda_cals.k_elc_min_mature_cycles + 1u;

   lcda_cals.k_elc_critical_longitudinal_ttc = 3.0f;
   lcda_cals.k_elc_min_mature_cycles         = 8u;

   lcda_cals.k_elc_zone_x[0] = -5.0f;
   lcda_cals.k_elc_zone_x[1] = -40.0f;
   lcda_cals.k_elc_zone_x[2] = -90.0f;
   lcda_cals.k_elc_zone_x[3] = -90.0f;
   lcda_cals.k_elc_zone_x[4] = -40.0f;
   lcda_cals.k_elc_zone_x[5] = -5.0f;

   lcda_cals.k_elc_zone_y[0] = 3.0f;
   lcda_cals.k_elc_zone_y[1] = 3.0f;
   lcda_cals.k_elc_zone_y[2] = 3.0f;
   lcda_cals.k_elc_zone_y[3] = 1.0f;
   lcda_cals.k_elc_zone_y[4] = 1.0f;
   lcda_cals.k_elc_zone_y[5] = 1.0f;

   lcda_cals.k_elc_zone_y_hys[0] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[1] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[2] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[3] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[4] = 0.1f;
   lcda_cals.k_elc_zone_y_hys[5] = 0.1f;

   /** \action Call Lcda_Process_Elc_Object to compute the ELC alert state. */
   Lcda_Process_Elc_Object(&elc_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &elc_persistent);

   /** \assert Check that the alert is not raised for processed target. */
   EXPECT_EQ(elc_core_output.elc_index[FBK_SIDE_LEFT], 255u);
   EXPECT_EQ(elc_core_output.elc_id[FBK_SIDE_LEFT], 0u);
   EXPECT_FALSE(elc_core_output.elc_alert[FBK_SIDE_LEFT]);
}