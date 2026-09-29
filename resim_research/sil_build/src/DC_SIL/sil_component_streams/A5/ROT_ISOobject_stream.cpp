#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "rot_iso_object_stream.h"
#include "radar_ecu_CORE1.h"
#include "f360_log_types.h"
#include "f360_host_calib.h"
#include "dc_config.h"

DC_ROT_ISO_Object_Stream_T *GetROTISOObjectDataPtr();
DC_ROT_ISO_Object_Stream_T gen7_rot_iso_object;

void CopyTrackerOutToROTISOStream(ROT_Object_List_Info_T *iso_obj, f360_variant_A::F360_Host_Calib_T *host_calib, Run_Mode_T run_mode) {
   DC_ROT_ISO_Object_Stream_T *rot_iso_obj = GetROTISOObjectDataPtr();
   memset(rot_iso_obj, 0, sizeof(DC_ROT_ISO_Object_Stream_T));
   float center_offset[2][1] = {0};
   float obj_center[2][1]    = {0};
   float temp[2][1]          = {0};
   float iso_x_posn;
   for (int i = 0; i < DC_ROT_NUMBER_OF_REDUCED_OBJECT_TRACKS && i < f360_variant_A::NUMBER_OF_REDUCED_OBJECT_TRACKS; i++) {
      int ref_point        = iso_obj->rot_object_list[i].reference_point;
      float length         = iso_obj->rot_object_list[i].length;
      float width          = iso_obj->rot_object_list[i].width;
      float yaw            = iso_obj->rot_object_list[i].iso_orientation;
      float cos_yaw        = cosf(yaw);
      float sin_yaw        = sinf(yaw);
      float rotation[2][2] = {{cos_yaw, -sin_yaw}, {sin_yaw, cos_yaw}};
      if (run_mode == DETECTIONS_UDP) {
         iso_x_posn = iso_obj->rot_object_list[i].iso_x_posn;
      } else {
         iso_x_posn = iso_obj->rot_object_list[i].iso_x_posn - host_calib->dist_rear_axle_to_vcs_m;
      }
      float obj_posn[2][1] = {{iso_x_posn}, {iso_obj->rot_object_list[i].iso_y_posn}};

      switch (ref_point) {
      case 0:
         center_offset[0][0] = 0;
         center_offset[1][0] = 0;
         break;
      case 1:
         center_offset[0][0] = -length / 2;
         center_offset[1][0] = -width / 2;
         break;
      case 2:
         center_offset[0][0] = -length / 2;
         center_offset[1][0] = 0;
         break;
      case 3:
         center_offset[0][0] = -length / 2;
         center_offset[1][0] = width / 2;
         break;
      case 4:
         center_offset[0][0] = 0;
         center_offset[1][0] = width / 2;
         break;
      case 5:
         center_offset[0][0] = length / 2;
         center_offset[1][0] = width / 2;
         break;
      case 6:
         center_offset[0][0] = length / 2;
         center_offset[1][0] = 0;
         break;
      case 7:
         center_offset[0][0] = length / 2;
         center_offset[1][0] = -width / 2;
         break;
      case 8:
         center_offset[0][0] = 0;
         center_offset[1][0] = -width / 2;
         break;
      }

      for (int p = 0; p < 2; p++) {
         temp[p][0]       = rotation[p][0] * center_offset[0][0] + rotation[p][1] * center_offset[1][0];
         obj_center[p][0] = obj_posn[p][0] + temp[p][0];
      }

      rot_iso_obj->iso_objects.rot_object_list[i].id                    = iso_obj->rot_object_list[i].id;
      rot_iso_obj->iso_objects.rot_object_list[i].unique_id             = iso_obj->rot_object_list[i].unique_id;
      rot_iso_obj->iso_objects.rot_object_list[i].time_since_created    = iso_obj->rot_object_list[i].time_since_created;
      rot_iso_obj->iso_objects.rot_object_list[i].time_since_published  = iso_obj->rot_object_list[i].time_since_published;
      rot_iso_obj->iso_objects.rot_object_list[i].existence_probability = iso_obj->rot_object_list[i].existence_probability;
      // rot_iso_obj->iso_objects.rot_object_list[i].f_moveable = iso_obj->rot_object_list[i].
      // rot_iso_obj->iso_objects.rot_object_list[i].f_moving = iso_obj->rot_object_list[i].
      rot_iso_obj->iso_objects.rot_object_list[i].object_class        = iso_obj->rot_object_list[i].object_class;
      rot_iso_obj->iso_objects.rot_object_list[i].object_status       = iso_obj->rot_object_list[i].movement_status;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_orientation     = iso_obj->rot_object_list[i].iso_orientation;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_orientation_var = iso_obj->rot_object_list[i].iso_orientation_var;
      rot_iso_obj->iso_objects.rot_object_list[i].reference_point     = ref_point;
      rot_iso_obj->iso_objects.rot_object_list[i].sensor_src          = iso_obj->rot_object_list[i].sensor_src;
      rot_iso_obj->iso_objects.rot_object_list[i].length              = length;
      rot_iso_obj->iso_objects.rot_object_list[i].width               = width;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_posn          = obj_center[0][0];
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_posn          = obj_center[1][0];
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_posn_var      = iso_obj->rot_object_list[i].iso_x_posn_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_posn_var      = iso_obj->rot_object_list[i].iso_y_posn_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_xy_posn_cov     = iso_obj->rot_object_list[i].iso_xy_posn_cov;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_vel           = iso_obj->rot_object_list[i].iso_x_vel;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_vel           = iso_obj->rot_object_list[i].iso_y_vel;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_vel_var       = iso_obj->rot_object_list[i].iso_x_vel_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_vel_var       = iso_obj->rot_object_list[i].iso_y_vel_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_xy_vel_cov      = iso_obj->rot_object_list[i].iso_xy_vel_cov;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_acc           = iso_obj->rot_object_list[i].iso_x_acc;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_acc           = iso_obj->rot_object_list[i].iso_y_acc;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_x_acc_var       = iso_obj->rot_object_list[i].iso_x_acc_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_y_acc_var       = iso_obj->rot_object_list[i].iso_y_acc_var;
      rot_iso_obj->iso_objects.rot_object_list[i].iso_xy_acc_cov      = iso_obj->rot_object_list[i].iso_xy_acc_cov;
   }
}

DC_ROT_ISO_Object_Stream_T *GetROTISOObjectDataPtr() {
   return &gen7_rot_iso_object;
}