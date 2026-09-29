#ifndef SNAIL_COMPUTATION_CPP
#define SNAIL_COMPUTATION_CPP

#include "olp_snail_trail_computation.h"
#include "olp_ml_math.h"

namespace olp
{

   void snail_trail_calculation::snail_trail_time_update(Snail_Trail_T &p_snail_trail, const Vehicle_Info_T &vehicle_ouput, const Olp_Calibration_T &p_cals)
   {
      float yaw_per_cycle = vehicle_ouput.comp_yaw_rate_filtered * OLP_CYCLE_TIME;
      if (vehicle_ouput.filt_veh_speed_over_ground < p_cals.k_vp_snail_min_host_speed)
      {
         /* clear snail trail when stopped/moving very slow/in reverse */
         Init_snail_trail(p_snail_trail);
      }
      else
      {
         /* update host world position */
         Olp_Vector_2d_T world_time_update;

         world_time_update = Get_World_Time_Update(vehicle_ouput, yaw_per_cycle);
         world_time_update = olp_ml_wrapper::Olp_Vector_2d_Alg_Rotate(p_snail_trail.snail_host_heading, world_time_update);
         p_snail_trail.snail_host_position = olp_ml_wrapper::Olp_Vector_2d_Alg_Add(p_snail_trail.snail_host_position, world_time_update);
         p_snail_trail.snail_diff_heading += yaw_per_cycle;
         p_snail_trail.snail_host_heading = olp_ml_wrapper::Olp_Create_Angle(p_snail_trail.snail_host_heading.angle + yaw_per_cycle);
         p_snail_trail.snail_diff_dist += OLP_CYCLE_TIME * vehicle_ouput.filt_veh_speed_over_ground;
         p_snail_trail.snail_host_dist += OLP_CYCLE_TIME * vehicle_ouput.filt_veh_speed_over_ground;
         /* check if a new snail trail point should be stored */
         if ((p_snail_trail.snail_diff_dist >= p_cals.k_vp_snail_point_dist_separation) || (OLP_FAST_ABS(p_snail_trail.snail_diff_heading) >= p_cals.k_vp_snail_point_heading_separation) || /*PRQA S 3415*/ /*functions do not have side effects*/
               ((p_snail_trail.snail_index == 0) && OLP_IS_FALSE(p_snail_trail.f_snail_full_buffer)))
         {
            short int isnail;
            short int isnail_last;
            Olp_Vector_2d_T point2point;
            /* store new snail point */
            isnail = p_snail_trail.snail_index;
            isnail_last = isnail - 1;
            if (isnail_last < 0)
            {
               isnail_last = NUMBER_OF_SNAIL_POINTS - 1;
            }
            p_snail_trail.snail_trail_states[isnail].point = p_snail_trail.snail_host_position;

            point2point = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(p_snail_trail.snail_trail_states[isnail_last].point, p_snail_trail.snail_trail_states[isnail].point);
            p_snail_trail.snail_trail_states[isnail].distance_traveled = p_snail_trail.snail_host_dist;
            p_snail_trail.snail_trail_states[isnail].dist_between_points = olp_ml_wrapper::Olp_Vector_2d_Alg_Abs(point2point);
            p_snail_trail.snail_trail_states[isnail].heading = p_snail_trail.snail_host_heading.angle;

            /* reset distance */
            p_snail_trail.snail_diff_dist = 0;
            p_snail_trail.snail_diff_heading = 0;

            /* increment index */
            p_snail_trail.snail_index++;
            if (p_snail_trail.snail_index >= NUMBER_OF_SNAIL_POINTS)
            {
               p_snail_trail.snail_index = 0;
               p_snail_trail.f_snail_full_buffer = TRUE;
            }

            if (IS_TRUE(p_snail_trail.f_snail_full_buffer))
            {
               p_snail_trail.oldest_snail_trail_index = p_snail_trail.snail_index; /* snail_index is index where the next snail point will be stored, currently the old value is still valid*/
            }
            else
            {
               p_snail_trail.oldest_snail_trail_index = 0; /*buffer isn't full oldest value is at index zero*/
            }
         }
      }
   }

   void snail_trail_calculation::Init_snail_trail(Snail_Trail_T &p_snail_trail)
   {
      int i;

      p_snail_trail.f_snail_full_buffer      = FALSE;
      p_snail_trail.snail_index              = 0;
      p_snail_trail.oldest_snail_trail_index = 0;
      p_snail_trail.snail_diff_dist          = 0.0f;
      p_snail_trail.snail_diff_heading       = 0.0f;
      p_snail_trail.snail_host_position      = olp_ml_wrapper::Olp_Create_2d_Vector_Origin();
      p_snail_trail.snail_host_dist          = 0.0f;
      p_snail_trail.snail_host_heading       = olp_ml_wrapper::Olp_Create_Angle(0.0f);

      for (i = 0; i < NUMBER_OF_SNAIL_POINTS; i++)
      {
         p_snail_trail.snail_trail_states[i].point               = olp_ml_wrapper::Olp_Create_2d_Vector_Origin();
         p_snail_trail.snail_trail_states[i].distance_traveled   = 0.0f;
         p_snail_trail.snail_trail_states[i].dist_between_points = 0.0f;
         p_snail_trail.snail_trail_states[i].heading             = 0.0f;
      }
   }

   Olp_Vector_2d_T snail_trail_calculation::Get_World_Time_Update(const Vehicle_Info_T &vehicle_ouput, const float &yaw_per_cycle)
   {
      Olp_Vector_2d_T local_time_update;
      Olp_Angle_T chord_angle; /* [rad] chord vector angle in vcs coordinates */
      float chord_length;      /* [m] length of the chord */

      /* checking for yaw_per_cycle to ensure the sine is giving big enough result.
   * The division by yawrate then must be OK since yawrate always is much bigger than yaw_per_cycle */
      if (OLP_FAST_ABS(yaw_per_cycle) > OLP_EPSILON)
      {
         chord_length = 2.0f * (vehicle_ouput.filt_veh_speed_over_ground / vehicle_ouput.comp_yaw_rate_filtered) * sinf(0.5f * yaw_per_cycle);
      }
      else
      {
         chord_length = vehicle_ouput.filt_veh_speed_over_ground * OLP_CYCLE_TIME;
      }

      /* The angle of the chord in vcs coordinates is the angle between the host velocity and chord plus rear axle sideslip. */
      chord_angle = olp_ml_wrapper::Olp_Create_Angle((0.5f * yaw_per_cycle) + vehicle_ouput.sideslip_rear_axle);

      local_time_update.x = chord_length * chord_angle.cos;
      local_time_update.y = chord_length * chord_angle.sin;

      return local_time_update;
   }
}
#endif
