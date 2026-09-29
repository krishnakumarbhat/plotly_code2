/**
 * @file cta_factory_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_factory.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41916}
 */

#include "cta_factory_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_factory.c"
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
}


#ifndef NDEBUG
/**
 * tests the death functionallity. The object is expected to be valid.
 * \uts{CSCSA-41917} \sdd{SF-3820} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Fill_Current_Object__assertion_invalid_obj_index)
{
   EXPECT_DEATH(
      {
         /** \arrange choose invalid obj index */
         uint8_t obj_index = PA_OBJ_NUMBER_OF_OBJECTS;

         /** \action executes function to test */
         Cta_Fill_Current_Object(&object, &cta_instance, p_vehicle_data, obj_index);
         /** \assert death with invalid object */
      },
      ".*PA_OBJ_NUMBER_OF_OBJECTS.*");
}
#endif

/**
 * tests whether correct values are set for cta object. The cta object is expected to be have valid memory adresses for its
 * attributes. \uts{CSCSA-41918} \sdd{SF-3820} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Fill_Current_Object__fill_object_with_tracker_info_and_update_it)
{

   /** \arrange choose valid obj index */
   uint8_t obj_index              = 1u;
   uint8_t obj_id                 = 0u;
   data.object_data[obj_index].id = obj_id;

   /** \action executes function to test */
   Cta_Fill_Current_Object(&object, &cta_instance, p_vehicle_data, obj_index);

   /** \assert Expect valid memory adresses */
   EXPECT_EQ(&(cta_instance.obj_attributes_array[obj_id]), object.attributes);
   EXPECT_EQ(&(cta_instance.obj_persistent_array[obj_id]), object.persistent);
   EXPECT_EQ(object_data->id, object.tracker_data.id);
}


/**
 * Tests reset of attribute array. All attributes shall be reset to their default value.
 * \uts{CSCSA-41919} \sdd{SF-3822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Reset_Obj_Attribute_Array__reset_attributes_to_their_default)
{
   uint8_t index;
   /** \arrange arrange a default value for each entry */
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      cta_instance.obj_attributes_array[index].approach_side = FBK_SIDE_LEFT;
   }

   /** \action executes function to test */
   Cta_Reset_Obj_Attribute_Array(cta_instance.obj_attributes_array);

   /** \assert Expect default values */
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      EXPECT_EQ(cta_instance.obj_attributes_array[index].approach_side, FBK_SIDE_UNDEFINED);
   }
}

#ifndef NDEBUG
/**
 * Tests death functionality of attribute array entry reset. All attributes shall be reset to their default value.
 * \uts{CSCSA-41920} \sdd{SF-3812} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Reset_Target_Attributes__p_target_attributes_null)
{
   EXPECT_DEATH(
      {
         /** \arrange nothing needed to be arranged here */
         /** \action executes function to test */
         Cta_Reset_Target_Attributes(NULL);
         /** \assert death with invalid attribute entry */
      },
      ".*p_target_attributes.*");
}
#endif

/**
 * Tests reset of persistent array. All persistent values shall be reset to their default value.
 * \uts{CSCSA-41921} \sdd{SF-3823} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Reset_Obj_Persistent_Array__reset_persistent_to_their_default)
{
   uint8_t index;
   /** \arrange arrange a default value for each entry */
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      cta_instance.obj_persistent_array[index].f_prev_cta_alert_suppress            = FBK_TRUE;
      cta_instance.obj_persistent_array[index].prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;
   }

   /** \action executes function to test */
   Cta_Reset_Obj_Persistent_Array(cta_instance.obj_persistent_array);

   /** \assert Expect valid memory adresses */
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      EXPECT_FALSE(cta_instance.obj_persistent_array[index].f_prev_cta_alert_suppress);
      EXPECT_EQ(cta_instance.obj_persistent_array[index].prev_cycle_crit_level[CTA_MODE_REAR], CTA_CRIT_LEVEL_NONE);
   }
}

#ifndef NDEBUG
/**
 * Tests death functionality of persistent array entry reset. All persistent shall be reset to their default value.
 * \uts{CSCSA-41922} \sdd{SF-3811} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Reset_Object_Persistent__p_object_persistent_null)
{
   EXPECT_DEATH(
      {
         /** \arrange nothing needed to be arranged here */
         /** \action executes function to test */
         Cta_Reset_Object_Persistent(NULL);
         /** \assert death with invalid persistent entry */
      },
      ".*p_object_persistent.*");
}
#endif

/**
 * Tests reset of persistent array via id. All persistent values shall be reset to their default value for persistent entry
 * specified by id. \uts{CSCSA-41923} \sdd{SF-3824} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Reset_Single_Obj_Persistent__reset_persistent_entry_to_default)
{
   /** \arrange id and setup value of persistent array unequal to default */
   uint8_t id                                                                 = 0;
   cta_instance.obj_persistent_array[id].f_prev_cta_alert_suppress            = FBK_TRUE;
   cta_instance.obj_persistent_array[id].prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;

   /** \action executes function to test */
   Cta_Reset_Single_Obj_Persistent(&cta_instance.obj_persistent_array[id]);

   /** \assert Expect flag being false */
   EXPECT_FALSE(cta_instance.obj_persistent_array[id].f_prev_cta_alert_suppress);
   EXPECT_EQ(cta_instance.obj_persistent_array[id].prev_cycle_crit_level[CTA_MODE_REAR], CTA_CRIT_LEVEL_NONE);
}


/**
 * Tests update of cta object and arrange a path tracking setup here so that path information is used. Expect heading of path
 * tracking to be used. \uts{CSCSA-41924} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__take_path_heading)
{
   /** \arrange object moving to the left side and path with lateral left direction */
   uint8_t track_match_in = 1u;
   float32_T path_heading = -0.6f * PI;

   object.tracker_data.vcs_heading  = -0.5f * PI;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking = FBK_TRUE;
   pt_match_info.track_match           = track_match_in;
   pt_match_info.path_heading          = path_heading;
   pt_match_info.path_direction        = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0]   = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect path heading which is used here */
   EXPECT_EQ(object.attributes->CTA_heading, path_heading);
}


/**
 * Tests update of cta object and arrange a scenario where the path tracking information shall not be used. Expect heading of path
 * tracking to be used. \uts{CSCSA-41925} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__mitigate_path_tracking_info_usage_since_host_moves)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = -0.9f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_TRUE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = -p_cals->k_cta_min_host_speed_to_discard_pt_info;
   pt_match_info.track_match                      = track_match_in;
   pt_match_info.path_heading                     = path_heading;
   pt_match_info.path_direction                   = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0]              = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_FLOAT_EQ(object.attributes->CTA_heading, tracker_heading);
}

/**
 * Tests update of cta object scenario where the path tracking information shall not be used due to path tracking disabled
 * \uts{CSCSA-278520} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__object_match_to_PT_but_cal_PT_disabled)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = -0.9f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_FALSE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = -p_cals->k_cta_min_host_speed_to_discard_pt_info;
   pt_match_info.track_match                      = track_match_in;
   pt_match_info.path_heading                     = path_heading;
   pt_match_info.path_direction                   = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0]              = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect pt match information is NULL */
   EXPECT_EQ(object.attributes->p_pt_match_info, nullptr);
}


/**
 * Tests update of cta object and arrange a scenario where the path tracking information shall not be used. Expect heading of path
 * tracking to be used. \uts{CSCSA-41926} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__mitigate_path_tracking_info_usage_since_host_moves_positive_abs_case)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.index        = FBK_ZERO_UINT;
   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = 0.9f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_TRUE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = -p_cals->k_cta_min_host_speed_to_discard_pt_info;

   pt_match_info.track_match         = track_match_in;
   pt_match_info.path_heading        = path_heading;
   pt_match_info.path_direction      = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0] = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_FLOAT_EQ(object.attributes->CTA_heading, tracker_heading);
}

/**
 * Tests update of cta object and arrange a scenario where the path tracking information shall used when distance is enough
 * \uts{CSCSA-188374} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__use_path_tracking_info_due_to_enough_lateral_distance)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.index        = FBK_ZERO_UINT;
   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = 1.1f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_TRUE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = -p_cals->k_cta_min_host_speed_to_discard_pt_info;

   pt_match_info.track_match         = track_match_in;
   pt_match_info.path_heading        = path_heading;
   pt_match_info.path_direction      = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0] = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_FLOAT_EQ(object.attributes->CTA_heading, path_heading);
}


/**
 * Tests update of cta object and arrange a scenario where the path tracking information shall be used even when the host moves.
 * Expect heading of path tracking to be used. \uts{CSCSA-41927} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__flag_to_mitigate_on_but_host_is_standstill)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = 0.9f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_TRUE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = 0.9f * p_cals->k_cta_min_host_speed_to_discard_pt_info;
   pt_match_info.track_match                      = track_match_in;
   pt_match_info.path_heading                     = path_heading;
   pt_match_info.path_direction                   = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0]              = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_EQ(object.attributes->CTA_heading, path_heading);
}


/**
 * Tests update of cta object and arrange a scenario where the path tracking information shall be used even when the host moves.
 * Expect heading of path tracking to be used sincen the object is too far away. \uts{CSCSA-41928} \sdd{SF-3813}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__flag_to_mitigate_on_and_host_moving_but_object_not_near)
{
   /** \arrange object moving to the left side and path with lateral left direction. However host moves and pt shall be mitigated.
    */
   uint8_t track_match_in    = 1u;
   float32_T path_heading    = -0.6f * PI;
   float32_T tracker_heading = -0.5f * PI;

   object.tracker_data.vcs_heading  = tracker_heading;
   object.tracker_data.vcs_pos.y    = 1.1f * p_cals->k_cta_obj_dist_to_discard_pt_info;
   object.attributes->approach_side = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking            = FBK_TRUE;
   p_cals->k_cta_f_discard_pt_heading_when_moving = FBK_TRUE;
   p_vehicle_data->host_speed                     = 0.9f * p_cals->k_cta_min_host_speed_to_discard_pt_info;
   pt_match_info.track_match                      = track_match_in;
   pt_match_info.path_heading                     = path_heading;
   pt_match_info.path_direction                   = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0]              = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_EQ(object.attributes->CTA_heading, path_heading);
}


/**
 * Tests update of cta object. Path information is not available but path tracking is enabled. Expect heading of tracker to be
 * used. \uts{CSCSA-41929} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__path_heading_enabled_but_path_info_null)
{
   /** \arrange object moving to the left side */
   object.tracker_data.vcs_heading    = -0.5f * PI;
   object.attributes->approach_side   = FBK_SIDE_RIGHT;
   object.attributes->p_pt_match_info = NULL;

   p_cals->k_cta_f_apply_path_tracking = FBK_TRUE;
   cta_core_input.p_pt_output          = NULL;
   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_EQ(object.attributes->CTA_heading, object.tracker_data.vcs_heading);
}


/**
 * Tests update of cta object. Path information is available but path tracking match index is not valid. Expect heading of tracker
 * to be used. \uts{CSCSA-41930} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__path_heading_enabled_info_not_null_index_not_valid)
{
   /** \arrange object moving to the left side and path with lateral left direction */
   float32_T path_heading = -0.6f * PI;

   object.tracker_data.index             = 0u;
   object.tracker_data.vcs_heading       = -0.5f * PI;
   object.attributes->approach_side      = FBK_SIDE_RIGHT;
   object.persistent->prev_approach_side = object.attributes->approach_side;

   p_cals->k_cta_f_apply_path_tracking = FBK_TRUE;

   pt_match_info.track_match         = PT_DEFAULT_MATCH_INDEX;
   pt_match_info.path_heading        = path_heading;
   pt_match_info.path_direction      = (Pt_Path_Direction_T) 1u;
   pt_output.path_obj_pair_output[0] = pt_match_info;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect tracker heading which is used here */
   EXPECT_EQ(object.attributes->CTA_heading, object.tracker_data.vcs_heading);
}


/**
 * Tests update of cta object. Approach side changed an the object persistent data shall be reset.
 * \uts{CSCSA-41931} \sdd{SF-3813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Update_Object_Data__approach_side_changed)
{
   /** \arrange object moving to the left side and path with lateral left direction */
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;

   object.tracker_data.vcs_heading       = -0.5f * PI;
   object.persistent->prev_approach_side = FBK_SIDE_LEFT;
   object.attributes->approach_side      = FBK_SIDE_RIGHT;

   p_cals->k_cta_f_apply_path_tracking = FBK_FALSE;

   /** \action executes function to test */
   Cta_Update_Object_Data(&object, p_cals, &cta_core_input, p_vehicle_data);

   /** \assert Expect persistent data to be reset */
   EXPECT_EQ(object.persistent->prev_cycle_crit_level[CTA_MODE_REAR], CTA_CRIT_LEVEL_NONE);
}

/**
 * Tests calculation cta object's heading. Expected heading after multiplication by the coefficient and heading of the previous
 * cycle \uts{CSCSA-41932} \sdd{SF-4059} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Heading_Filter__heading_calculation)
{
   /** \arrange object with the highest critacility in previous cycle */
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;
   p_cals->k_cta_f_enable_heading_exp_moving_average       = FBK_TRUE;

   object.tracker_data.vcs_heading       = -0.5f * PI;
   object.persistent->cta_object_heading = -0.40f * PI;

   float32_T calculated_heading = object.tracker_data.vcs_heading;
   float32_T result             = (p_cals->k_cta_object_heading_exp_moving_average_alpha * object.tracker_data.vcs_heading)
                      + ((FBK_ONE_F - p_cals->k_cta_object_heading_exp_moving_average_alpha) * object.persistent->cta_object_heading);

   /** \action executes function to test */
   calculated_heading = Cta_Heading_Filter(&object, p_cals);

   /** \assert Expect persistent data to be reset */
   EXPECT_FLOAT_EQ(calculated_heading, result);
}

/**
 * Tests calculation cta object's heading. Criticality level set to zero. Expected heading taken from tracker
 * \uts{CSCSA-41933} \sdd{SF-4059} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Heading_Filter__crit_zero_heading_from_tracker)
{
   /** \arrange object with the highest critacility in previous cycle */
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_NONE;
   p_cals->k_cta_f_enable_heading_exp_moving_average       = FBK_TRUE;

   object.tracker_data.vcs_heading       = -0.5f * PI;
   object.persistent->cta_object_heading = -0.40f * PI;

   float32_T calculated_heading = 0.0f;

   /** \action executes function to test */
   calculated_heading = Cta_Heading_Filter(&object, p_cals);

   /** \assert Expect persistent data to be reset */
   EXPECT_FLOAT_EQ(calculated_heading, object.tracker_data.vcs_heading);
}

/**
 * Tests calculation cta object's heading. Exponential moving average function disabled. Expected heading taken from tracker
 * \uts{CSCSA-41934} \sdd{SF-4059} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Heading_Filter__flag_disabled_heading_from_tracker)
{
   /** \arrange object with the highest critacility in previous cycle */
   object.persistent->prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_1;
   p_cals->k_cta_f_enable_heading_exp_moving_average       = FBK_FALSE;

   object.tracker_data.vcs_heading       = -0.5f * PI;
   object.persistent->cta_object_heading = -0.40f * PI;

   float32_T calculated_heading = 0.0f;

   /** \action executes function to test */
   calculated_heading = Cta_Heading_Filter(&object, p_cals);

   /** \assert Expect persistent data to be reset */
   EXPECT_FLOAT_EQ(calculated_heading, object.tracker_data.vcs_heading);
}

/**
 * Tests calculation of side. Expect left approach side.
 * \uts{CSCSA-41935} \sdd{SF-3808} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Calculate_Side__return_left_approach_side)
{
   /** \arrange object with positive heading */
   object.tracker_data.vcs_heading = 0.5f * PI;

   /** \action executes function to test */
   Cta_Calculate_Side(&object);

   /** \assert Expect left approach side */
   EXPECT_EQ(object.attributes->approach_side, FBK_SIDE_LEFT);
}


/**
 * Tests detection of approach side changes. Expect an approach side change.
 * \uts{CSCSA-41936} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__Approach_Side_Has_Changed_from_left_to_right)
{
   /** \arrange object wwhich changes its approach direction from left to right */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_LEFT;
   object.attributes->approach_side      = FBK_SIDE_RIGHT;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect true */
   EXPECT_TRUE(result);
}


/**
 * Tests detection of approach side changes. Expect an approach side change.
 * \uts{CSCSA-41937} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__Approach_Side_Has_Changed_from_right_to_left)
{
   /** \arrange object wwhich changes its approach direction from right to left */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_RIGHT;
   object.attributes->approach_side      = FBK_SIDE_LEFT;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect true */
   EXPECT_TRUE(result);
}


/**
 * Tests detection of approach side changes. Expect no approach side change.
 * \uts{CSCSA-41938} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__Approach_Side_Has_Not_Changed_and_stays_left)
{
   /** \arrange object which keeps the direction left */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_LEFT;
   object.attributes->approach_side      = FBK_SIDE_LEFT;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests detection of approach side changes. Expect no approach side change.
 * \uts{CSCSA-41939} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__Approach_Side_Has_Not_Changed_and_stays_right)
{
   /** \arrange object which keeps the direction right */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_RIGHT;
   object.attributes->approach_side      = FBK_SIDE_RIGHT;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests detection of approach side changes. Expect no approach side change.
 * \uts{CSCSA-41940} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__Previous_approach_side_undefined)
{
   /** \arrange object which which moves left for first time */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_UNDEFINED;
   object.attributes->approach_side      = FBK_SIDE_LEFT;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests detection of approach side changes. Expect no approach side change.
 * \uts{CSCSA-41941} \sdd{SF-3810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Has_Approach_Side_Changed__approach_side_undefined)
{
   /** \arrange object which lost its approach side variable */
   boolean_T result;
   object.persistent->prev_approach_side = FBK_SIDE_LEFT;
   object.attributes->approach_side      = FBK_SIDE_UNDEFINED;
   /** \action executes function to test */
   result = Cta_Has_Approach_Side_Changed(&object);

   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests routine to initialize internal cta object.
 * \uts{CSCSA-41942} \sdd{SF-3821} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Init_Object__check_that_everything_is_default_afterwards)
{
   /** \arrange Initialize object in fixture class to something unequal to nullptr */

   /** \action executes function to test */
   Cta_Init_Object(&object);

   /** \assert Expect nullpointer everywhere. */
   EXPECT_EQ(nullptr, object.attributes);
   EXPECT_EQ(nullptr, object.persistent);
   EXPECT_EQ(PA_INVALID_OBJ_ID, object.tracker_data.id);
}


/**
 * Tests routine to initialize internal cta comparison data.
 * \uts{CSCSA-41943} \sdd{SF-4027} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Init_Comparison_Data__check_that_everything_is_default_afterwards)
{
   /** \arrange Initialize object with non defaults */
   uint8_t side_idx;
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      cta_comparison_data.max_level[CTA_MODE_REAR][side_idx]  = CTA_CRIT_LEVEL_1;
      cta_comparison_data.max_level[CTA_MODE_FRONT][side_idx] = CTA_CRIT_LEVEL_1;
   }
   /** \action executes function to test */
   Cta_Init_Comparison_Data(&cta_comparison_data, &cta_instance);

   /** \assert Expect defaults to be set. */
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(cta_comparison_data.max_level[CTA_MODE_REAR][side_idx], CTA_CRIT_LEVEL_NONE);
      EXPECT_EQ(cta_comparison_data.max_level[CTA_MODE_FRONT][side_idx], CTA_CRIT_LEVEL_NONE);
   }
}

/**
 * Tests routine to initialize internal criticality level data.
 * \uts{CSCSA-41944} \sdd{SF-4057} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Factory_Test, Cta_Init_Crit_Level_Cals__check_that_criticality_level_are_initialized_correctly)
{
   /** \arrange Initialize object with non defaults */
   uint8_t mode_idx;
   uint8_t level_idx;
   Cta_Crit_Level_Calibration_T cta_crit_level_cals{};

   for (mode_idx = FBK_ZERO_UINT; mode_idx < FBK_NUMBER_OF_SIDES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < FBK_NUMBER_OF_SIDES; level_idx++)
      {
         cta_core_input.ttc_criticality_level[mode_idx][level_idx] = 1.0f;

         p_cals->k_cta_max_long_point_criticality_level[mode_idx][level_idx] = -2.0f;
         p_cals->k_cta_min_long_point_criticality_level[mode_idx][level_idx] = -5.0f;
      }
   }
   p_vehicle_data->host_length = 5.0f;

   /** \action executes function to test */
   Cta_Init_Crit_Level_Cals(&cta_crit_level_cals, &cta_core_input, p_cals, p_vehicle_data);

   /** \assert Expect defaults to be set. */
   for (level_idx = FBK_ZERO_UINT; level_idx < FBK_NUMBER_OF_SIDES; level_idx++)
   {
      EXPECT_FLOAT_EQ(cta_crit_level_cals.max_long_point_criticality_level[CTA_MODE_REAR][level_idx], -3.0f);
      EXPECT_FLOAT_EQ(cta_crit_level_cals.min_long_point_criticality_level[CTA_MODE_REAR][level_idx], 0.0f);
      EXPECT_FLOAT_EQ(cta_crit_level_cals.max_long_point_criticality_level[CTA_MODE_FRONT][level_idx], -2.0f);
      EXPECT_FLOAT_EQ(cta_crit_level_cals.min_long_point_criticality_level[CTA_MODE_FRONT][level_idx], -5.0f);

      for (mode_idx = FBK_ZERO_UINT; mode_idx < FBK_NUMBER_OF_SIDES; mode_idx++)
      {
         EXPECT_FLOAT_EQ(cta_crit_level_cals.ttc_criticality_level[mode_idx][level_idx], 1.0f);
      }
   }
}
