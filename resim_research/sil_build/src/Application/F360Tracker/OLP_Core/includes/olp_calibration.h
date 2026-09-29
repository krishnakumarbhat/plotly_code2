#ifndef OLP_CALIBRATION_HPP
#define OLP_CALIBRATION_HPP

/* Generic Olp Calibrations */
typedef struct Olp_Calibration_Tag
{
   float	k_vp_snail_min_host_speed;/*minimum host speed to enable the snail trail*/
   float	k_vp_snail_min_elapsed_time_s;/*minimum elapsed time to enable the snail trail*/
   float	k_vp_snail_max_elapsed_time_s;/*maximum elapsed time to enable the snail trail*/
   float	k_vp_snail_point_dist_separation;/*distance the host needs to travel before adding a new snail point<INCREASE>less snail trail points, larger snail trail distance</INCREASE><DECREASE>more snail trail points, shorter snail trail distance</DECREASE>*/
   float	k_vp_snail_point_heading_separation;/*heading the host needs to yaw before adding a new snail point<INCREASE>less snail trail points in curves, larger snail trail distance</INCREASE><DECREASE>more snail trail points in curves, shorter snail trail distance</DECREASE>*/
   float	k_vp_host_curvature_min_speed_lowpass;/*minimum speed to update the low pass curvature filter*/
   float	k_vp_host_curvature_distance_constant_fast;/*distance constant for updating fast filtered curvature*/
   float	k_vp_host_curvature_distance_constant_slow;/*distance constant for updating slow filtered curvature*/
   float	k_vp_host_curvature_max_delta;/*maximum change in curvature in one time step*/
   float	k_ad_min_ego_speed_for_convert_to_curvi;/*minimum ego speed to compute curvi coordiantes. Below this speed curvi coordinates are same as vcs*/
   float	k_vp_snail_trail_max_dist2;/*maximum squared distance between query point and snail point to find a match*/
   float	k_vp_snail_trail_max_dist2_oldest_snail_point;/*maximum squared distance between query point and oldest snail point (if this point is chosen) to use snail trail for curvi*/
   float	k_vp_min_speed_snail_trail_heading_rate;
   unsigned char	k_vp_host_curvature_limit_delta;/*flag indicating the maximum curvature change in one time step should be limited*/
   unsigned char	k_vp_enable_snail_trail;/*flag to enable snail trail calculation*/
   unsigned char unused_k1; /* padding */
   unsigned char unused_k2; /* padding */
}Olp_Calibration_T;

#ifdef __cplusplus
extern "C" Olp_Calibration_T* Olp_Get_Cal_Ptr(void);
#else
Olp_Calibration_T* Olp_Get_Cal_Ptr(void);
#endif
#endif
