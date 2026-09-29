#include "olp_distance_based_curvature.h"
#include "olp_ml_math.h"


namespace olp
{
#define ALLOWED_DELTA_FACTOR (0.015f)
#define MINIMUM_RAD_CURVATURE (10.0f)
#define SMOOTH_CURV_DEBOUNCE (40u)
#define SHARP_CURV_DEBOUNCE (5u)

   void distance_based_curvature::distance_based_curvature_computation(const Vehicle_Info_T& vehicle_output, Curvature_Info_T& curvature_input, const Olp_Calibration_T& pcals)
   {

      float curvature_measured = 0.0f;
      float Rad_of_curvature = 0.0f;
      static float Rad_of_curvature_prev = 0.0f;
      static unsigned char Rad_Curvtr_Smooth_Cntr = 0u;
      static unsigned char Rad_Curvtr_Sharp_Cntr = 0u;
      
      curvature_input.f_host_curvature_calculated = OLP_FALSE;
      /*% check speed threshold for low pass filter*/
      if (vehicle_output.filt_veh_speed_over_ground >= pcals.k_vp_host_curvature_min_speed_lowpass)
      {
         /* To avoid impact of noisy yaw rate on curvature calculations, yaw rate of upto +-1deg/sec is approximated to 0. */
         if (OLP_FAST_ABS(vehicle_output.comp_yaw_rate_filtered) > 0.0174533f)
         {
            /*% compute raw measured curvature*/
            curvature_measured = vehicle_output.comp_yaw_rate_filtered / vehicle_output.filt_veh_speed_over_ground;
         }
         else
         {
            curvature_measured = 0.0f;
         }

         /*% *** Slow Filtering ***
         * % compute slow time constant */
         const float dist = OLP_CYCLE_TIME * vehicle_output.filt_veh_speed_over_ground;
         float dist_ratio = -dist / pcals.k_vp_host_curvature_distance_constant_slow;
         float alpha = FAST_EXP(dist_ratio);

         /*% compute change in curvature */
         float curvature_delta = (1.F - alpha) * (curvature_measured - curvature_input.host_curvature_slow);

         /*% limit maximum delta*/
         if (OLP_TRUE == (signed int)pcals.k_vp_host_curvature_limit_delta)
         {
            float max_delta = OLP_MAX(-pcals.k_vp_host_curvature_max_delta, curvature_delta);
            curvature_delta = OLP_MIN(pcals.k_vp_host_curvature_max_delta, max_delta);
         }
         /*% apply delta*/
         curvature_input.host_curvature_slow = curvature_input.host_curvature_slow + curvature_delta;
         curvature_input.f_abs_curvature_LT_cal = OLP_FAST_ABS(curvature_input.host_curvature_slow) < OLP_CURV_TRHESHOLD ? 1U : 0U;

         /* Distance based curvature calculation is active after Smooth curv is detected(SMOOTH_CURV_DEBOUNCE).
            Stays active for hold time(SHARP_CURV_DEBOUNCE) after sharp curv is detected */
         if (OLP_FAST_ABS(curvature_input.host_curvature_slow) > OLP_TRACKER_THRESHOLD_IS_ZERO)
         {
            Rad_of_curvature = 1 / curvature_input.host_curvature_slow;

            if ((OLP_FAST_ABS(Rad_of_curvature) > MINIMUM_RAD_CURVATURE) &&
                  ((OLP_FAST_ABS(Rad_of_curvature)) > ((1.0f - ALLOWED_DELTA_FACTOR) * (OLP_FAST_ABS(Rad_of_curvature_prev)))) &&
                  ((OLP_FAST_ABS(Rad_of_curvature)) < ((1.0f + ALLOWED_DELTA_FACTOR) * (OLP_FAST_ABS(Rad_of_curvature_prev)))))
            {
               Rad_Curvtr_Smooth_Cntr += 1u;
               if (SMOOTH_CURV_DEBOUNCE <= Rad_Curvtr_Smooth_Cntr)
               {
                  Rad_Curvtr_Smooth_Cntr = SMOOTH_CURV_DEBOUNCE;
                  curvature_input.f_host_curvature_calculated = OLP_TRUE;
                  Rad_Curvtr_Sharp_Cntr = 0u;
               }
            }
            else
            {
               if (SMOOTH_CURV_DEBOUNCE <= Rad_Curvtr_Smooth_Cntr)
               {
                  Rad_Curvtr_Sharp_Cntr += 1u;
                  if (SHARP_CURV_DEBOUNCE <= Rad_Curvtr_Sharp_Cntr)
                  {
                     Rad_Curvtr_Sharp_Cntr = 0u;
                     Rad_Curvtr_Smooth_Cntr = 0u;
                  }
                  else
                  {
                     curvature_input.f_host_curvature_calculated = OLP_TRUE;
                  }
               }
            }
            Rad_of_curvature_prev = Rad_of_curvature;
         }
         else
         {
            Rad_of_curvature_prev = 0.0f;
            Rad_Curvtr_Smooth_Cntr = 0u;
            Rad_Curvtr_Sharp_Cntr = 0u;
         }
      }
      else
      {
         Rad_of_curvature_prev = 0.0f;
         Rad_Curvtr_Smooth_Cntr = 0u;
         Rad_Curvtr_Sharp_Cntr = 0u;
      }
   }

   Olp_Vector_2d_T distance_based_curvature::Transform_Point_With_Distance_Based_Curvature(const Vehicle_Info_T & vehicle_output, const Olp_Vector_2d_T & obj_center_pos, const Curvature_Info_T & curvature_input)
   {
      Olp_Vector_2d_T transformed_point;
      Olp_Vector_2d_T by_sideslip_rotated_point;
      Olp_Angle_T     vcs_side_slip;

      /*
      As host vehicle is moving on a circle/clothoid vcs coordinate system is rotated by side slip.
      Therefor the given position must be corrected by side slip angle
      */
      vcs_side_slip = olp_ml_wrapper::Olp_Create_Angle(vehicle_output.vcs_sideslip);
      by_sideslip_rotated_point = olp_ml_wrapper::Olp_Vector_2d_Alg_RotateNegative(vcs_side_slip, obj_center_pos);


      if (OLP_TRUE == curvature_input.f_abs_curvature_LT_cal)
      {
         /* small curvature use clothoid model */
         float delta_lat;
         delta_lat = 0.5f * curvature_input.host_curvature_slow * (by_sideslip_rotated_point.x * by_sideslip_rotated_point.x);
         transformed_point.x = by_sideslip_rotated_point.x;
         transformed_point.y = by_sideslip_rotated_point.y - delta_lat;
      }
      else
      {
         /* large curvature use circle model */
         float r;
         float sign_r;
         float rhat;
         float temp;
         float num;
         float atan_ratio;

         assert(OLP_FAST_ABS(curvature_input.host_curvature_slow) > OLP_TRACKER_THRESHOLD_IS_ZERO);
         r = 1.0f / curvature_input.host_curvature_slow;
         sign_r = (float)OLP_SIGN(r);
         rhat = r - by_sideslip_rotated_point.y;
         temp = OLP_FAST_SQRT((by_sideslip_rotated_point.x * by_sideslip_rotated_point.x) + (rhat * rhat));
         num = temp - (sign_r * rhat);

         if (OLP_FAST_ABS(by_sideslip_rotated_point.x) > OLP_TRACKER_THRESHOLD_IS_ZERO)
         {
            float ratio;
            ratio = num / by_sideslip_rotated_point.x;
            atan_ratio = OLP_FAST_ATAN(ratio);
         }
         else
         {
            atan_ratio = 0.0f;
         }
         transformed_point.x = 2.0f * OLP_FAST_ABS(r) * atan_ratio;
         transformed_point.y = r - (sign_r * temp);
      }
      return transformed_point;
   }
}
