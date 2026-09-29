
#include "olp_calibration.h"

Olp_Calibration_T Olp_Cals;

void Olp_Update_Cal_Default_Value(void)
{
   /* Generic Default Olp Calibrations values*/
   Olp_Cals.k_vp_snail_min_host_speed    = 0.1f;
   Olp_Cals.k_vp_snail_min_elapsed_time_s    = 0.01f;
   Olp_Cals.k_vp_snail_max_elapsed_time_s    = 0.1f;
   Olp_Cals.k_vp_snail_point_dist_separation    = 5.0f;
   Olp_Cals.k_vp_snail_point_heading_separation    = 0.314159265358979f;
   Olp_Cals.k_vp_host_curvature_min_speed_lowpass    = 0.1f;
   Olp_Cals.k_vp_host_curvature_distance_constant_fast    = 0.5f;
   Olp_Cals.k_vp_host_curvature_distance_constant_slow    = 3.0f;
   Olp_Cals.k_vp_host_curvature_max_delta    = 0.0001f;
   Olp_Cals.k_ad_min_ego_speed_for_convert_to_curvi    = 0.2f;
   Olp_Cals.k_vp_snail_trail_max_dist2    = 400.0f;
   Olp_Cals.k_vp_snail_trail_max_dist2_oldest_snail_point    = 9.0f;
   Olp_Cals.k_vp_min_speed_snail_trail_heading_rate    = 3.0f;
   Olp_Cals.k_vp_host_curvature_limit_delta    = 0u;
   Olp_Cals.k_vp_enable_snail_trail    = 1u;
}

#ifdef __cplusplus
   extern "C" Olp_Calibration_T* Olp_Get_Cal_Ptr(void)
#else
   unsigned char Olp_Calibration_T* Olp_Get_Cal_Ptr(void)
#endif
{
   return(&Olp_Cals);
}

