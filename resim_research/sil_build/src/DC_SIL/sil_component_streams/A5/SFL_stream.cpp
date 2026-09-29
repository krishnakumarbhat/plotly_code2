#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "sfl_stream.h"
#include "cta_output_t.h"
#include "ta_output_t.h"
#include "scw_output_t.h"
#include "recw_output_t.h"
#include "lcda_output_t.h"
#include "ced_output_t.h"
#include "pt_output_t.h"

DC_SFL_Stream_T *GetSFLDataPtr();
DC_SFL_Stream_T gen7_sfl_object;

void CopyFFOutToSFLStream(Cta_Output_T *cta_output, Ta_Output_T *ta_output, Scw_Output_T *scw_output, Recw_Output_T *recw_output, Lcda_Output_T *lcda_output, Ced_Output_T *ced_output, Pt_Output_T *pt_output) {
   DC_SFL_Stream_T *sfl_obj = GetSFLDataPtr();
   memset(sfl_obj, 0, sizeof(DC_SFL_Stream_T));

   sfl_obj->recw_output.recw_alert_level              = recw_output->recw_alert_level;
   sfl_obj->recw_output.recw_crash_probability        = recw_output->recw_crash_probability;
   sfl_obj->recw_output.recw_id                       = recw_output->recw_id;
   sfl_obj->recw_output.recw_status                   = 1;
   sfl_obj->recw_output.recw_ttc_s                    = recw_output->recw_ttc_s;
   sfl_obj->recw_output.recw_unique_id                = recw_output->recw_unique_id;
   sfl_obj->recw_output.ttc_threshold_alert_level_1_s = recw_output->ttc_threshold_alert_level_1_s;
   sfl_obj->recw_output.ttc_threshold_alert_level_2_s = recw_output->ttc_threshold_alert_level_2_s;

   for (int i = 0; i < DC_FBK_NUMBER_OF_SIDES; i++) {

      sfl_obj->scw_output.f_scw_dyn_enabled                   = scw_output->f_scw_dyn_enabled;
      sfl_obj->scw_output.f_scw_enabled                       = scw_output->f_scw_enabled;
      sfl_obj->scw_output.f_scw_guardrail_enabled             = scw_output->f_scw_guardrail_enabled;
      sfl_obj->scw_output.scw_object[i].acceleration_mps2.x   = scw_output->scw_object[i].acceleration_mps2.x;
      sfl_obj->scw_output.scw_object[i].acceleration_mps2.y   = scw_output->scw_object[i].acceleration_mps2.y;
      sfl_obj->scw_output.scw_object[i].age                   = scw_output->scw_object[i].age;
      sfl_obj->scw_output.scw_object[i].alert_level           = scw_output->scw_object[i].alert_level;
      sfl_obj->scw_output.scw_object[i].existence_probability = scw_output->scw_object[i].existence_probability;
      sfl_obj->scw_output.scw_object[i].heading_rad           = scw_output->scw_object[i].heading_rad;
      sfl_obj->scw_output.scw_object[i].id                    = scw_output->scw_object[i].id;
      sfl_obj->scw_output.scw_object[i].lateral_distance_m    = scw_output->scw_object[i].lateral_distance_m;
      sfl_obj->scw_output.scw_object[i].lateral_ttc_s         = scw_output->scw_object[i].lateral_ttc_s;
      sfl_obj->scw_output.scw_object[i].length_m              = scw_output->scw_object[i].length_m;
      sfl_obj->scw_output.scw_object[i].position_m.x          = scw_output->scw_object[i].position_m.x;
      sfl_obj->scw_output.scw_object[i].position_m.y          = scw_output->scw_object[i].position_m.y;
      sfl_obj->scw_output.scw_object[i].ttle_s                = scw_output->scw_object[i].ttle_s;
      sfl_obj->scw_output.scw_object[i].ttp_s                 = scw_output->scw_object[i].ttp_s;
      sfl_obj->scw_output.scw_object[i].type                  = scw_output->scw_object[i].type;
      sfl_obj->scw_output.scw_object[i].unique_id             = scw_output->scw_object[i].unique_id;
      sfl_obj->scw_output.scw_object[i].velocity_mps.x        = scw_output->scw_object[i].velocity_mps.x;
      sfl_obj->scw_output.scw_object[i].velocity_mps.y        = scw_output->scw_object[i].velocity_mps.y;
      sfl_obj->scw_output.scw_object[i].width_m               = scw_output->scw_object[i].width_m;
      sfl_obj->scw_output.scw_object[i].yawrate_radps         = scw_output->scw_object[i].yawrate_radps;

      sfl_obj->lcda_output.bsw_alert[i]                   = lcda_output->bsw_alert[i];
      sfl_obj->lcda_output.bsw_id[i]                      = lcda_output->bsw_id[i];
      sfl_obj->lcda_output.bsw_unique_id[i]               = lcda_output->bsw_unique_id[i];
      sfl_obj->lcda_output.cvw_alert[i]                   = lcda_output->cvw_alert[i];
      sfl_obj->lcda_output.cvw_id[i]                      = lcda_output->cvw_id[i];
      sfl_obj->lcda_output.cvw_ttc_s[i]                   = lcda_output->cvw_ttc_s[i];
      sfl_obj->lcda_output.cvw_unique_id[i]               = lcda_output->cvw_unique_id[i];
      sfl_obj->lcda_output.f_bsw_enabled                  = lcda_output->f_bsw_enabled;
      sfl_obj->lcda_output.f_cvw_enabled                  = lcda_output->f_cvw_enabled;
      sfl_obj->lcda_output.f_slc_enabled                  = lcda_output->f_slc_enabled;
      sfl_obj->lcda_output.lcda_status                    = lcda_output->lcda_status;
      sfl_obj->lcda_output.slc_alert[i]                   = lcda_output->slc_alert[i];
      sfl_obj->lcda_output.slc_id[i]                      = lcda_output->slc_id[i];
      sfl_obj->lcda_output.slc_lane_change_probability[i] = lcda_output->slc_lane_change_probability[i];
      sfl_obj->lcda_output.slc_ttc_s[i]                   = lcda_output->slc_ttc_s[i];
      sfl_obj->lcda_output.slc_unique_id[i]               = lcda_output->slc_unique_id[i];

      sfl_obj->ced_output.ced_alert[i]                      = ced_output->ced_alert[i];
      sfl_obj->ced_output.ced_object[i].direction           = ced_output->ced_object[i].direction;
      sfl_obj->ced_output.ced_object[i].heading_rad         = ced_output->ced_object[i].heading_rad;
      sfl_obj->ced_output.ced_object[i].id                  = ced_output->ced_object[i].id;
      sfl_obj->ced_output.ced_object[i].lat_pos_m           = ced_output->ced_object[i].lat_pos_m;
      sfl_obj->ced_output.ced_object[i].length_m            = ced_output->ced_object[i].length_m;
      sfl_obj->ced_output.ced_object[i].long_pos_m          = ced_output->ced_object[i].long_pos_m;
      sfl_obj->ced_output.ced_object[i].predicted_lat_pos_m = ced_output->ced_object[i].predicted_lat_pos_m;
      sfl_obj->ced_output.ced_object[i].speed_mps           = ced_output->ced_object[i].speed_mps;
      sfl_obj->ced_output.ced_object[i].ttc_s               = ced_output->ced_object[i].ttc_s;
      sfl_obj->ced_output.ced_object[i].ttp_s               = ced_output->ced_object[i].ttp_s;
      sfl_obj->ced_output.ced_object[i].type                = ced_output->ced_object[i].type;
      sfl_obj->ced_output.ced_object[i].unique_id           = ced_output->ced_object[i].unique_id;
      sfl_obj->ced_output.ced_object[i].width_m             = ced_output->ced_object[i].width_m;
      sfl_obj->ced_output.f_ced_enable                      = ced_output->f_ced_enable;

      for (int j = 0; j < DC_CTA_NUM_MODES; j++) {

         sfl_obj->cta_output.f_cta_enabled                                              = cta_output->f_cta_enabled;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].alert_level            = cta_output->most_critical_object_by_sides[j][i].alert_level;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].brake_deceleration     = cta_output->most_critical_object_by_sides[j][i].brake_deceleration;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].f_brake_qualifier      = cta_output->most_critical_object_by_sides[j][i].f_brake_qualifier;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].f_standstill_qualifier = cta_output->most_critical_object_by_sides[j][i].f_standstill_qualifier;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].heading_rad            = cta_output->most_critical_object_by_sides[j][i].heading_rad;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].id                     = cta_output->most_critical_object_by_sides[j][i].id;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].intersection_point_x_m = cta_output->most_critical_object_by_sides[j][i].intersection_point_x_m;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].objPoseX_m             = cta_output->most_critical_object_by_sides[j][i].objPoseX_m;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].objPoseY_m             = cta_output->most_critical_object_by_sides[j][i].objPoseY_m;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].objVelocityX_mps       = cta_output->most_critical_object_by_sides[j][i].objVelocityX_mps;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].objVelocityY_mps       = cta_output->most_critical_object_by_sides[j][i].objVelocityY_mps;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].ttc_s                  = cta_output->most_critical_object_by_sides[j][i].ttc_s;
         sfl_obj->cta_output.most_critical_object_by_sides[j][i].unique_id              = cta_output->most_critical_object_by_sides[j][i].unique_id;
      }
      sfl_obj->ta_output.f_ta_enable                               = ta_output->f_ta_enable;
      sfl_obj->ta_output.ta_alert_level[i]                         = ta_output->ta_alert_level[i];
      sfl_obj->ta_output.ta_algorithm_state                        = ta_output->ta_algorithm_state;
      sfl_obj->ta_output.ta_f_vehicle_state_relevant               = ta_output->ta_f_vehicle_state_relevant;
      sfl_obj->ta_output.ta_most_critical_side                     = ta_output->ta_most_critical_side;
      sfl_obj->ta_output.ta_n_critical_objects                     = ta_output->ta_n_critical_objects;
      sfl_obj->ta_output.ta_n_relevant_objects                     = ta_output->ta_n_relevant_objects;
      sfl_obj->ta_output.ta_n_valid_objects                        = ta_output->ta_n_valid_objects;
      sfl_obj->ta_output.ta_object[i].ta_decel_estimate_mps2       = ta_output->ta_object[i].ta_decel_estimate_mps2;
      sfl_obj->ta_output.ta_object[i].ta_distance_m                = ta_output->ta_object[i].ta_distance_m;
      sfl_obj->ta_output.ta_object[i].ta_f_obj_in_danger_zone      = ta_output->ta_object[i].ta_f_obj_in_danger_zone;
      sfl_obj->ta_output.ta_object[i].ta_f_obj_in_info_zone        = ta_output->ta_object[i].ta_f_obj_in_info_zone;
      sfl_obj->ta_output.ta_object[i].ta_f_obj_in_wing_zone        = ta_output->ta_object[i].ta_f_obj_in_wing_zone;
      sfl_obj->ta_output.ta_object[i].ta_id                        = ta_output->ta_object[i].ta_id;
      sfl_obj->ta_output.ta_object[i].ta_index                     = ta_output->ta_object[i].ta_index;
      sfl_obj->ta_output.ta_object[i].ta_ttb_s                     = ta_output->ta_object[i].ta_ttb_s;
      sfl_obj->ta_output.ta_object[i].ta_ttc_s                     = ta_output->ta_object[i].ta_ttc_s;
      sfl_obj->ta_output.ta_object[i].ta_ttp_s                     = ta_output->ta_object[i].ta_ttp_s;
      sfl_obj->ta_output.ta_object[i].ta_waypoint_at_collision_m.x = ta_output->ta_object[i].ta_waypoint_at_collision_m.x;
      sfl_obj->ta_output.ta_object[i].ta_waypoint_at_collision_m.y = ta_output->ta_object[i].ta_waypoint_at_collision_m.y;
   }
   for (int k = 0; k < DC_PA_OBJ_NUMBER_OF_OBJECTS; k++) {
      sfl_obj->pt_output.f_pt_operational                                      = pt_output->f_pt_operational;
      sfl_obj->pt_output.nearest_path_output[k].range_vcs_proj_to_path_segment = pt_output->nearest_path_output[k].range_vcs_proj_to_path_segment;
      sfl_obj->pt_output.nearest_path_output[k].segment_heading_diff           = pt_output->nearest_path_output[k].segment_heading_diff;
      sfl_obj->pt_output.nearest_path_output[k].track_idx_nearest_path         = pt_output->nearest_path_output[k].track_idx_nearest_path;
      sfl_obj->pt_output.path_obj_pair_output[k].length_of_trajectory          = pt_output->path_obj_pair_output[k].length_of_trajectory;
      sfl_obj->pt_output.path_obj_pair_output[k].path_direction                = pt_output->path_obj_pair_output[k].path_direction;
      sfl_obj->pt_output.path_obj_pair_output[k].path_heading                  = pt_output->path_obj_pair_output[k].path_heading;
      sfl_obj->pt_output.path_obj_pair_output[k].path_state                    = pt_output->path_obj_pair_output[k].path_state;
      sfl_obj->pt_output.path_obj_pair_output[k].range_at_host_edge            = pt_output->path_obj_pair_output[k].range_at_host_edge;
      sfl_obj->pt_output.path_obj_pair_output[k].range_at_zero                 = pt_output->path_obj_pair_output[k].range_at_zero;
      sfl_obj->pt_output.path_obj_pair_output[k].range_to_current_path_part    = pt_output->path_obj_pair_output[k].range_to_current_path_part;
      sfl_obj->pt_output.path_obj_pair_output[k].track_match                   = pt_output->path_obj_pair_output[k].track_match;
      sfl_obj->pt_output.path_obj_pair_output[k].track_match_age               = pt_output->path_obj_pair_output[k].track_match_age;
      sfl_obj->pt_output.path_obj_pair_output[k].track_match_last_cycle        = pt_output->path_obj_pair_output[k].track_match_last_cycle;
   }
}

DC_SFL_Stream_T *GetSFLDataPtr() {
   return &gen7_sfl_object;
}