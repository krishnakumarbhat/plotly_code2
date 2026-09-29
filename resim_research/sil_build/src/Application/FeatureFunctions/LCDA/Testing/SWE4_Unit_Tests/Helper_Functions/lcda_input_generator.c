/**
 * @file c_testing_main.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file for LCDA test input generator.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "lcda_input_generator.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_persistent_t.h"
#include "lcda_process_bsw.h"
#include "lcda_process_cvw.h"
#include "lcda_types.h"
#include "pa_data.h"
#include "pa_shared_types.h"

#define make_editable(type, value) (*((type *) (&(value))))

void Lcda_Create_Valid_Bsw_Cvw_Track(Fbk_Object_Data_T *p_tracker_object, const uint8_t id)
{
   p_tracker_object->index                 = 0;
   p_tracker_object->status                = PA_OBJ_STATUS_MATURE;
   p_tracker_object->age                   = 6;
   p_tracker_object->vcs_pos.x             = 40.0f;
   p_tracker_object->vcs_heading           = 0.15f;
   p_tracker_object->curvi_heading         = 0.15f;
   p_tracker_object->vcs_vel.x             = 50.0f;
   p_tracker_object->curvi_vel.x           = 50.0f;
   p_tracker_object->curvi_vel_rel.x       = 5.0f;
   p_tracker_object->width                 = 2.0f;
   p_tracker_object->length                = 4.5f;
   p_tracker_object->id                    = id;
   p_tracker_object->unique_id             = (uint32_t) id;
   p_tracker_object->existence_probability = 1.0f;
}

void Lcda_Create_Bsw_Track(Fbk_Object_Data_T *p_tracker_object, const uint8_t id, float32_T lon_pos, float32_T lat_pos)
{
   Lcda_Create_Valid_Bsw_Cvw_Track(p_tracker_object, id);

   // set the longitudinal and lateral positions
   p_tracker_object->vcs_pos.y = lat_pos;
   p_tracker_object->vcs_pos.x = lon_pos;

   p_tracker_object->curvi_pos.y = lat_pos;
   p_tracker_object->curvi_pos.x = lon_pos;
}

void Lcda_Create_Zone(Vector_2d_T point0, float32_T length, float32_T width, Fbk_Field_Of_Interest_T *p_zone)
{
   // If the origin lies to the right of the ego, then the right side zone is created. Otherwise, the left side zone is created
   float32_T signed_width;

   if (point0.y > 0)
   {
      // For right side zone: y values in VCS of points 3, 4, 5 is point0.y - width
      signed_width = -1.0f * width;
   }
   else
   {
      // For left side zone : y values in VCS of points 3, 4, 5 is point0.y + width,
      signed_width = width;
   }

   p_zone->size = LCDA_NUMBER_OF_ZONE_POINTS;

   p_zone->points[0].x = point0.x;
   p_zone->points[0].y = point0.y;

   p_zone->points[1].x = point0.x - (length * 0.5f);
   p_zone->points[1].y = point0.y;

   p_zone->points[2].x = point0.x - length;
   p_zone->points[2].y = point0.y;

   p_zone->points[3].x = point0.x - length;
   p_zone->points[3].y = point0.y + signed_width;

   p_zone->points[4].x = point0.x - (length * 0.5f);
   p_zone->points[4].y = point0.y + signed_width;

   p_zone->points[5].x = point0.x;
   p_zone->points[5].y = point0.y + signed_width;
}

void Lcda_Create_Cvw_Alert(uint8_t side, uint8_t obj_id, Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   int8_t sign                        = 1;
   Lcda_Core_Input_T core_input       = {0};
   Lcda_Core_Output_T core_output     = {0};
   Lcda_Core_Calibration_T cals       = {0};
   Pa_Data_T data                     = {0};
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;

   Lcda_Persistent_T lcda_persistent                            = {0};
   Lcda_Bsw_Persistent_T bsw_persistent                         = {0};
   Fbk_Object_Data_T tracker_object                             = {0};
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {3.0f, 3.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   if (side == FBK_SIDE_LEFT)
   {
      sign = -1;
   }

   // Reset all the persistence that will be remaining from other runs of the LCDA core
   Lcda_Reset_Bsw_Core(&core_output.bsw_core_output, &bsw_persistent, &lcda_persistent, &core_input);
   Lcda_Reset_Cvw_Core(&core_output.cvw_core_output, &lcda_persistent, p_cvw_persistent);

   make_editable(float32_T, cals.k_lcda_min_exist_prop)                 = 0.7f;
   cals.k_lcda_min_track_age                                            = 5;
   cals.k_bsw_max_heading_abs                                           = 0.785f;
   cals.k_bsw_min_obj_long_vel                                          = 5.0f;
   cals.k_cvw_max_curvi_heading_abs                                     = 0.262f;
   cals.k_cvw_min_obj_curvi_long_vel                                    = 1.0f;
   make_editable(float32_T, cals.k_cvw_candidate_ttc)                   = 10.0f;
   cals.k_cvw_ttc                                                       = 5.0f;
   cals.k_cvw_ttc_hys                                                   = 0.5f;
   make_editable(boolean_T, cals.k_bsw_uses_cvw_alert_state_enabled)    = 1;
   make_editable(boolean_T, cals.k_lcda_allow_track_status_new)         = 1;
   cals.k_lcda_zone_intersect_critical_point_lateral_ratio              = 0.0f;
   cals.k_bsw_min_mature_cycles                                         = 1;
   cals.k_cvw_min_mature_cycles                                         = 0;
   make_editable(boolean_T, cals.k_lcda_f_enable_obj_in_ego_lane_check) = 0;
   cals.k_cvw_max_object_curvi_relative_speed                           = 1000.0;
   cals.k_cvw_min_object_curvi_relative_speed[0]                        = 0.0f;
   cals.k_cvw_min_object_curvi_relative_speed[1]                        = 0.0f;
   cals.k_cvw_min_object_curvi_relative_speed[2]                        = 0.0f;

   // Set up the data of the core input
   core_input.enabled_flags.f_lcda_enabled        = 1;
   core_input.p_pa_data                           = &data;
   core_input.lane_width                          = 3.0f;
   core_input.lane_center_offset                  = 0.0f;
   core_input.warn_settings.cvw_rel_vel_range.min = cals.k_cvw_min_object_curvi_relative_speed[0];
   core_input.warn_settings.cvw_rel_vel_range.max = cals.k_cvw_max_object_curvi_relative_speed;

   /* Set basic vehicle data */
   p_vehicle_data->host_length = 4.5f;
   p_vehicle_data->host_width  = 2.0f;

   make_editable(uint8_t, cals.k_cvw_zone_calculation_mode) = CVW_ZONE_CALC_FIXED_INPUT;
   cals.k_bsw_min_mature_cycles                             = 1;

   // The initial zone is always specified for the right side. The create zone then mirrors the zone for the left side
   core_input.initial_cvw_zone.size     = LCDA_NUMBER_OF_ZONE_POINTS;
   core_input.initial_cvw_zone_hys.size = LCDA_NUMBER_OF_ZONE_POINTS;

   core_input.initial_cvw_zone.points[0].x = -6.0f;
   core_input.initial_cvw_zone.points[0].y = 8.0f;

   core_input.initial_cvw_zone.points[1].x = -30.0f;
   core_input.initial_cvw_zone.points[1].y = 8.0f;

   core_input.initial_cvw_zone.points[2].x = -80.0f;
   core_input.initial_cvw_zone.points[2].y = 8.0f;

   core_input.initial_cvw_zone.points[3].x = -80.0f;
   core_input.initial_cvw_zone.points[3].y = 4.0f;

   core_input.initial_cvw_zone.points[4].x = -30.0f;
   core_input.initial_cvw_zone.points[4].y = 4.0f;

   core_input.initial_cvw_zone.points[5].x = -6.0f;
   core_input.initial_cvw_zone.points[5].y = 4.0f;

   core_input.warn_settings.cvw_ttc_threshold = cals.k_cvw_ttc;

   core_input.initial_cvw_zone_hys.points[0].x = -4.0f;
   core_input.initial_cvw_zone_hys.points[0].y = 9.0f;

   core_input.initial_cvw_zone_hys.points[1].x = -35.0f;
   core_input.initial_cvw_zone_hys.points[1].y = 9.0f;

   core_input.initial_cvw_zone_hys.points[2].x = -90.0f;
   core_input.initial_cvw_zone_hys.points[2].y = 9.0f;

   core_input.initial_cvw_zone_hys.points[3].x = -90.0f;
   core_input.initial_cvw_zone_hys.points[3].y = 3.0f;

   core_input.initial_cvw_zone_hys.points[4].x = -35.0f;
   core_input.initial_cvw_zone_hys.points[4].y = 3.0f;

   core_input.initial_cvw_zone_hys.points[5].x = -4.0f;
   core_input.initial_cvw_zone_hys.points[5].y = 3.0f;

   Lcda_Create_Bsw_Track(&tracker_object, obj_id, -20.0f, sign * 6.0f);

   /* Alert will be created on the object and the persistence will be updated */
   Lcda_Process_Cvw_Object(&core_output.cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object,
                           &core_input, &cals, &lcda_persistent, p_cvw_persistent);

   Lcda_Postprocess_Cvw(&core_output.cvw_core_output, f_use_small_lc_intention_zone, &cals, &lcda_persistent, p_cvw_persistent);
}
