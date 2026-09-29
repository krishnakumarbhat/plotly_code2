#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "gt_input_stream.h"
#include "sil_ecu_input_ext.h"

DC_GT_Logging_Data_T *GetDspaceInputDataPtr();
DC_GT_Logging_Data_T gen7_dspace_input_object;

void CopyTracksToDspaceInputStream(DC_INPUT_DATA_T *sil_input_buffer) {
   DC_GT_Logging_Data_T *dspace_input_obj = GetDspaceInputDataPtr();
   memset(dspace_input_obj, 0, sizeof(DC_GT_Logging_Data_T));
   for (int i = 0; i < TRACKER_NUMBER_OF_OBJECTS; i++) {
      dspace_input_obj->object[i].vcs_long_posn    = sil_input_buffer->object_rl[i].vcs_long_posn;
      dspace_input_obj->object[i].vcs_lat_posn     = sil_input_buffer->object_rl[i].vcs_lat_posn;
      dspace_input_obj->object[i].vcs_long_vel     = sil_input_buffer->object_rl[i].vcs_long_vel;
      dspace_input_obj->object[i].vcs_lat_vel      = sil_input_buffer->object_rl[i].vcs_lat_vel;
      dspace_input_obj->object[i].vcs_long_accel   = sil_input_buffer->object_rl[i].vcs_long_accel;
      dspace_input_obj->object[i].vcs_lat_accel    = sil_input_buffer->object_rl[i].vcs_lat_accel;
      dspace_input_obj->object[i].vcs_long_vel_rel = sil_input_buffer->object_rl[i].vcs_long_vel_rel;
      dspace_input_obj->object[i].vcs_lat_vel_rel  = sil_input_buffer->object_rl[i].vcs_lat_vel_rel;
      dspace_input_obj->object[i].vcs_heading      = sil_input_buffer->object_rl[i].heading;
      dspace_input_obj->object[i].iso_long_posn    = sil_input_buffer->object_rl[i].vcs_long_posn;
      dspace_input_obj->object[i].iso_lat_posn     = -sil_input_buffer->object_rl[i].vcs_lat_posn;
      dspace_input_obj->object[i].iso_long_vel     = sil_input_buffer->object_rl[i].vcs_long_vel;
      dspace_input_obj->object[i].iso_lat_vel      = -sil_input_buffer->object_rl[i].vcs_lat_vel;
      dspace_input_obj->object[i].iso_long_accel   = sil_input_buffer->object_rl[i].vcs_long_accel;
      dspace_input_obj->object[i].iso_lat_accel    = -sil_input_buffer->object_rl[i].vcs_lat_accel;
      dspace_input_obj->object[i].iso_long_vel_rel = sil_input_buffer->object_rl[i].vcs_long_vel_rel;
      dspace_input_obj->object[i].iso_lat_vel_rel  = -sil_input_buffer->object_rl[i].vcs_lat_vel_rel;
      dspace_input_obj->object[i].speed            = sil_input_buffer->object_rl[i].speed;
      dspace_input_obj->object[i].iso_heading      = -sil_input_buffer->object_rl[i].heading;
      dspace_input_obj->object[i].length           = sil_input_buffer->object_rl[i].length;
      dspace_input_obj->object[i].width            = sil_input_buffer->object_rl[i].width;
   }
}

DC_GT_Logging_Data_T *GetDspaceInputDataPtr() {
   return &gen7_dspace_input_object;
}