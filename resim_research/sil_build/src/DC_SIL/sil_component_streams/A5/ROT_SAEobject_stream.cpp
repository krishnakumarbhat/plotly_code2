#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "rot_sae_object_stream.h"
#include "radar_ecu_CORE1.h"
#include "f360_log_types.h"
#include "f360_host_calib.h"

DC_ROT_SAE_Object_Stream_T *GetROTSAEObjectDataPtr();
DC_ROT_SAE_Object_Stream_T gen7_rot_sae_object;

void CopyTrackerOutToROTSAEStream(f360_variant_A::F360_Object_Log_Output_T *obj) {
   DC_ROT_SAE_Object_Stream_T *rot_sae_obj = GetROTSAEObjectDataPtr();
   memset(rot_sae_obj, 0, sizeof(DC_ROT_SAE_Object_Stream_T));
   float center_offset[2][1] = {0};
   float obj_center[2][1]    = {0};
   float temp[2][1]          = {0};
   for (size_t i = 0; i < DC_ROT_NUMBER_OF_OBJECT_TRACKS; i++) {
      int ref_point        = obj->object[i].reference_point;
      float length         = obj->object[i].len1 + obj->object[i].len2;
      float width          = obj->object[i].wid1 + obj->object[i].wid2;
      float yaw            = obj->object[i].vcs_pointing;
      float cos_yaw        = cosf(yaw);
      float sin_yaw        = sinf(yaw);
      float rotation[2][2] = {{cos_yaw, -sin_yaw}, {sin_yaw, cos_yaw}};
      float obj_posn[2][1] = {{obj->object[i].vcs_xposn}, {obj->object[i].vcs_yposn}};
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

      rot_sae_obj->sae_objects.object[i].unique_id = obj->object[i].reducedID;
      rot_sae_obj->sae_objects.object[i].trkID     = obj->object[i].reducedID;
      rot_sae_obj->sae_objects.object[i].reducedID = obj->object[i].reducedID;
      // rot_sae_obj->sae_objects.object[i].ndets = obj->object[i].num_reduced_dets;
      rot_sae_obj->sae_objects.object[i].ndets                    = obj->object[i].ndets;
      rot_sae_obj->sae_objects.object[i].existence_probability    = obj->object[i].existence_probability;
      rot_sae_obj->sae_objects.object[i].status                   = obj->object[i].reducedStatus;
      rot_sae_obj->sae_objects.object[i].time_since_track_updated = obj->object[i].time_since_track_updated;
      rot_sae_obj->sae_objects.object[i].reference_point          = ref_point;
      rot_sae_obj->sae_objects.object[i].len1                     = obj->object[i].len1;
      rot_sae_obj->sae_objects.object[i].len2                     = obj->object[i].len2;
      rot_sae_obj->sae_objects.object[i].wid1                     = obj->object[i].wid1;
      rot_sae_obj->sae_objects.object[i].wid2                     = obj->object[i].wid2;
      rot_sae_obj->sae_objects.object[i].vcs_heading              = obj->object[i].vcs_heading;
      rot_sae_obj->sae_objects.object[i].vcs_pointing             = obj->object[i].vcs_pointing;
      rot_sae_obj->sae_objects.object[i].object_class             = obj->object[i].object_class;
      rot_sae_obj->sae_objects.object[i].f_moveable               = obj->object[i].f_moveable;
      rot_sae_obj->sae_objects.object[i].f_fast_moving            = obj->object[i].f_fast_moving;
      rot_sae_obj->sae_objects.object[i].vcs_xposn                = obj_center[0][0];
      rot_sae_obj->sae_objects.object[i].vcs_yposn                = obj_center[1][0];
      rot_sae_obj->sae_objects.object[i].vcs_xvel                 = obj->object[i].vcs_xvel;
      rot_sae_obj->sae_objects.object[i].vcs_yvel                 = obj->object[i].vcs_yvel;
      rot_sae_obj->sae_objects.object[i].vcs_xaccel               = obj->object[i].vcs_xaccel;
      rot_sae_obj->sae_objects.object[i].vcs_yaccel               = obj->object[i].vcs_yaccel;
   }
}

DC_ROT_SAE_Object_Stream_T *GetROTSAEObjectDataPtr() {
   return &gen7_rot_sae_object;
}