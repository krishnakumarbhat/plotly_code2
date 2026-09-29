#ifndef DETECTIONS_FLOAT_T_H
#define DETECTIONS_FLOAT_T_H
#include "reuse.h"

typedef struct Detections_Float_Tag
{
   uint32_t valid_level;
   float32_T amplitude;
   float32_T range;
   float32_T range_rate_raw;
   float32_T azimuth_raw;
   float32_T elevation_raw;
   float32_T detection_snr;
   float32_T sd_azimuth;
   float32_T sd_range;
   float32_T sd_range_rate;
   uint8_t host_veh_clutter : 1;
   uint8_t azimuth_confidence : 2;
   uint8_t super_res_target : 1;
   uint8_t Ntd_target : 1;
   uint8_t bi_static_target : 1;
   uint8_t unused_1 : 2;
   uint16_t unused_2;

} Detections_Float_T;



#ifdef __cplusplus
extern "C"
{
#endif
   extern Detections_Float_T *Get_Tracker_Input_DSP_Dets_Float_DA(void);
#ifdef __cplusplus
}
#endif
#endif