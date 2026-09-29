
#include "scw_input_generator.h"

void Scw_Create_Valid_Tracker_Object(Scw_Object_T *p_scw_object, const uint8_t index)
{
   p_scw_object->tracker_data.id                    = index;
   p_scw_object->tracker_data.index                 = index;
   p_scw_object->tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_scw_object->tracker_data.vcs_heading           = 0.0f;
   p_scw_object->tracker_data.heading_rate          = 0.0f;
   p_scw_object->tracker_data.vcs_vel_rel.x         = 5.0f;
   p_scw_object->tracker_data.width                 = 2.0f;
   p_scw_object->tracker_data.length                = 4.5f;
   p_scw_object->tracker_data.vcs_pos.x             = -10.0f;
   p_scw_object->tracker_data.vcs_pos.y             = -2.0f;
   p_scw_object->tracker_data.speed                 = 10.0f;
   p_scw_object->tracker_data.existence_probability = 1.0f;
   p_scw_object->extended_data.index                = index;
   p_scw_object->extended_data.lateral_distance     = 1.0f;
   p_scw_object->extended_data.lateral_ttc          = 10.0f;
   p_scw_object->nearest_corner_y  = p_scw_object->tracker_data.vcs_pos.y + 0.5f * p_scw_object->tracker_data.width;
   p_scw_object->rearmost_corner_x = p_scw_object->tracker_data.vcs_pos.x - 0.5f * p_scw_object->tracker_data.length;
}


void Scw_Set_In_Range_Persistents(Scw_Persistent_T *p_scw_persistent, const uint8_t index)
{
   p_scw_persistent->dyn_obj_data[index].f_in_heading_range                = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[index].f_in_relative_long_velocity_range = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[index].f_in_speed_range                  = FBK_TRUE;
   p_scw_persistent->dyn_obj_data[index].f_in_yawrate_range                = FBK_TRUE;
}
