#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "vse_stream.h"
#include "f360_host.h"
#include "f360_host_raw.h"

DC_Vse_Stream_T *GetVSEDataPtr();
DC_Vse_Stream_T gen7_vse_object;

void CopyHostDataToVSEStream(f360_variant_A::F360_Host_T *host_info, f360_variant_A::F360_Host_Raw_T *host_raw) {
   DC_Vse_Stream_T *vse_obj = GetVSEDataPtr();
   memset(vse_obj, 0, sizeof(DC_Vse_Stream_T));
   vse_obj->veh_params.gear_position            = host_raw->prndl;
   vse_obj->veh_params.lros_drive_dir           = host_raw->prndl;
   vse_obj->veh_params.lros_drive_reverse       = host_raw->reverse_gear;
   vse_obj->veh_params.veh_ign_sts              = 0;
   vse_obj->veh_params.veh_pitch                = 0;
   vse_obj->veh_params.veh_pitch_qf             = 0;
   vse_obj->veh_params.veh_roll                 = 0;
   vse_obj->veh_params.veh_roll_qf              = 0;
   vse_obj->veh_params.veh_spd_comp_factor      = static_cast<int8_t>(host_info->speed_correction_factor);
   vse_obj->veh_params.veh_speed                = host_info->speed;
   vse_obj->veh_params.veh_speed_qf             = host_info->speed_qf;
   vse_obj->veh_params.veh_steering_angle       = host_raw->steering_wheel_angle_rad;
   vse_obj->veh_params.veh_steering_sign_status = 0;
   vse_obj->veh_params.veh_yaw                  = host_raw->raw_yaw_rate_rad;
   vse_obj->veh_params.veh_yaw_qf               = (uint8_t)3;
   vse_obj->veh_params.yaw_rate_bias            = 0;

   double timestamp_s               = host_raw->timestamp_s;
   vse_obj->veh_data_ts.ts_secs     = static_cast<uint32_t>(timestamp_s);
   vse_obj->veh_data_ts.ts_nanosecs = static_cast<uint32_t>((timestamp_s - vse_obj->veh_data_ts.ts_secs) * 1E9);

   vse_obj->veh_data_ts.ts_status = 0;

   vse_obj->veh_fs_coeff_values.enable_fs_calc = 0;
   for (int i = 0; i < NO_OF_COEFF; i++) {
      vse_obj->veh_fs_coeff_values.fs_az_coeff[i] = 0.0;
      vse_obj->veh_fs_coeff_values.fs_el_coeff[i] = 0.0;
   }
   vse_obj->veh_fs_coeff_values.sensor_posn = 0;
}

DC_Vse_Stream_T *GetVSEDataPtr() {
   return &gen7_vse_object;
}