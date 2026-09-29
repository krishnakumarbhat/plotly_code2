/**
 * @file fbk_host_lane.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module records a host trail which is then later used
 * for a host lane path creation.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_host_trail.h"
#include "fbk_core_calibration_t.h"
#include "fbk_debug_interface.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include <assert.h>

/*===========================================================================*\
* Static Function Declaration
\*===========================================================================*/

/**
 * @brief Returns the shift of host vehicle in world coordinates.
 *
 * @return vector containing the shift of host between previous and current cycle.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4164}
 * @verification{}
 */
static Vector_2d_T Fbk_Get_Wcs_Shift(const Pa_Data_T *p_pa_data /**< Context data of PT */,
                                     const Angle_T *p_yaw_shift /**< yaw shift compared between cycles */);

/**
 * @brief Checks whether a new point shall be added to the host trail. This is the case when heading criteria or distance criteria
 * are fulfilled. True also returned in case that the ringbuffer is empty.
 *
 * @return True in case that a new point shall be added to the host trail.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4163}
 * @verification{}
 */
static boolean_T Fbk_Shall_Point_Be_Added_To_Trail(const Fbk_Host_Trail_T *p_host_trail /**< host trail structure of PT*/,
                                                   const Fbk_Core_Calibration_T *p_cals /**< fbk calibration values. */);

/**
 * @brief Adds a point to the trail structure. This structure is used as a FIFO ringbuffer for sake of efficiency. Also updates
 * other point properties like the distance between the new point and previous.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4170}
 * @verification{}
 */
static void Fbk_Add_Host_Trailer_Point(Fbk_Host_Trail_T *p_host_trail /**< host trail structure of PT*/);

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/

void Fbk_Update_Host_Trail(Fbk_Host_Trail_T *p_host_trail, const Pa_Data_T *p_pa_data, const Fbk_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_host_trail);
   assert(NULL != p_pa_data);
   assert(NULL != p_cals);

   if (p_pa_data->vehicle_data.host_speed >= p_cals->k_fbk_host_trail_max_recording_speed)
   {
      /* clear host trail when */
      Fbk_Init_Host_Trail(p_host_trail);
   }
   else
   {
      /* update host world position and persistent information*/
      Vector_2d_T world_time_update;
      float32_T distance_traveled;
      Angle_T yaw_shift;

      yaw_shift         = Create_Angle(p_pa_data->vehicle_data.yawrate * p_pa_data->time_diff_to_last_cycle);
      world_time_update = Fbk_Get_Wcs_Shift(p_pa_data, &yaw_shift);
      world_time_update = Vector_2d_Alg_Rotate(&p_host_trail->trail_host_heading, &world_time_update);

      p_host_trail->trail_host_position = Vector_2d_Alg_Add(&p_host_trail->trail_host_position, &world_time_update);

      /* Update criteria necessary for addition of a point to the host trail */
      p_host_trail->trail_diff_heading += yaw_shift.angle;
      p_host_trail->trail_host_heading = Create_Angle(p_host_trail->trail_host_heading.angle + yaw_shift.angle);

      /* increment distance */
      distance_traveled = p_pa_data->time_diff_to_last_cycle * p_pa_data->vehicle_data.host_speed;
      p_host_trail->trail_diff_dist += distance_traveled;
      p_host_trail->trail_host_dist += distance_traveled;

      if (Fbk_Shall_Point_Be_Added_To_Trail(p_host_trail, p_cals))
      {
         Fbk_Add_Host_Trailer_Point(p_host_trail);
         /*Reset flag when a point is added to the host trail*/
         p_host_trail->f_was_trail_point_added_this_cycle = FBK_TRUE;
      }
      else
      {
         p_host_trail->f_was_trail_point_added_this_cycle = FBK_FALSE;
      }
   }

   Binary_Fbk_Debug_Pass_Host_Trail(p_host_trail);
}

void Fbk_Init_Host_Trail(Fbk_Host_Trail_T *p_host_trail)
{
   uint8_t i;

   /* Assert */
   assert(NULL != p_host_trail);

   p_host_trail->f_trail_full_buffer                = FBK_FALSE;
   p_host_trail->f_was_trail_point_added_this_cycle = FBK_FALSE;
   p_host_trail->trail_index                        = FBK_ZERO_UINT;
   p_host_trail->oldest_trail_index                 = FBK_ZERO_UINT;
   p_host_trail->trail_diff_dist                    = FBK_ZERO_F;
   p_host_trail->trail_diff_heading                 = FBK_ZERO_F;
   p_host_trail->trail_host_position                = Create_2d_Vector_Origin();
   p_host_trail->trail_host_dist                    = FBK_ZERO_F;
   p_host_trail->trail_host_heading                 = Create_Angle(FBK_ZERO_F);

   for (i = FBK_ZERO_UINT; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      p_host_trail->segments[i].point               = Create_2d_Vector_Origin();
      p_host_trail->segments[i].distance_traveled   = FBK_ZERO_F;
      p_host_trail->segments[i].dist_between_points = FBK_ZERO_F;
      p_host_trail->segments[i].heading             = FBK_ZERO_F;
   }
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Host_Trail_Empty(const Fbk_Host_Trail_T *p_host_trail)
{
   /* Assert */
   assert(NULL != p_host_trail);

   return (boolean_T) (Fbk_Is_False(p_host_trail->f_trail_full_buffer) && (FBK_ZERO_UINT == p_host_trail->trail_index));
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Get_Point_Indices(Fbk_Point_Pair_T *p_index_pair, const Fbk_Host_Lane_Interval_T *p_interval, const uint8_t trail_point_ctr)
{
   /* Asserts */
   assert(NULL != p_index_pair);
   assert(NULL != p_interval);

   p_index_pair->passed = (uint8_t) ((uint8_t) (p_interval->interval_end + trail_point_ctr) - FBK_ONE_UINT);
   p_index_pair->next   = (uint8_t) (p_interval->interval_end + trail_point_ctr);

   /*Saturate indices due to ring buffer of trail */
   if (p_index_pair->passed >= FBK_NUM_HOST_TRAIL_POINTS)
   {
      p_index_pair->passed = (uint8_t) (p_index_pair->passed - FBK_NUM_HOST_TRAIL_POINTS);
   }
   if (p_index_pair->next >= FBK_NUM_HOST_TRAIL_POINTS)
   {
      p_index_pair->next = (uint8_t) (p_index_pair->next - FBK_NUM_HOST_TRAIL_POINTS);
   }
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Init_Trail_Vcs(Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS],
                        const Fbk_Host_Trail_T *p_host_trail,
                        const float32_T rear_axle_position)
{
   uint8_t idx;

   /* Assert */
   assert(NULL != p_host_trail);

   for (idx = FBK_ZERO_UINT; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      if (Fbk_Is_True(p_host_trail->f_trail_full_buffer)
          || ((idx >= p_host_trail->oldest_trail_index) && (idx < p_host_trail->trail_index)))
      {
         trail_vcs[idx].x = p_host_trail->segments[idx].point.x - p_host_trail->trail_host_position.x;
         trail_vcs[idx].y = p_host_trail->segments[idx].point.y - p_host_trail->trail_host_position.y;

         Fbk_Swap_Float(&(trail_vcs[idx].x), &(trail_vcs[idx].y));
         trail_vcs[idx] = Vector_2d_Alg_Rotate(&p_host_trail->trail_host_heading, &trail_vcs[idx]);
         Fbk_Swap_Float(&(trail_vcs[idx].x), &(trail_vcs[idx].y));
         trail_vcs[idx].x += rear_axle_position;
      }
      else
      {
         trail_vcs[idx].x = FBK_ZERO_F;
         trail_vcs[idx].y = FBK_ZERO_F;
      }
   }
}

/*===========================================================================*\
* Static Function Definition
\*===========================================================================*/

static boolean_T Fbk_Shall_Point_Be_Added_To_Trail(const Fbk_Host_Trail_T *p_host_trail, const Fbk_Core_Calibration_T *p_cals)
{
   boolean_T f_shall_point_be_added = FBK_FALSE;
   boolean_T f_diff_dist;
   boolean_T f_diff_head;

   /* Asserts */
   assert(NULL != p_host_trail);
   assert(NULL != p_cals);

   f_diff_dist = (boolean_T) (p_host_trail->trail_diff_dist >= p_cals->k_fbk_host_trail_dist_separation);
   f_diff_head = (boolean_T) (Fbk_Abs_F(p_host_trail->trail_diff_heading) >= p_cals->k_fbk_host_trail_heading_separation);

   if (f_diff_dist || f_diff_head || (Fbk_Is_Host_Trail_Empty(p_host_trail)))
   {
      /*Only add a point when the separation condition is fulfilled either by the distance or heading criteria or when the
       * buffer is completly empty.*/
      f_shall_point_be_added = FBK_TRUE;
   }

   return f_shall_point_be_added;
}

static Vector_2d_T Fbk_Get_Wcs_Shift(const Pa_Data_T *p_pa_data, const Angle_T *p_yaw_shift)
{
   Vector_2d_T local_time_update; /* [m] total shift between previous and current cycle*/
   Angle_T chord_angle;           /* [rad] chord vector angle in vcs coordinates */
   float32_T chord_length;        /* [m] length of the chord */
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_pa_data);
   assert(NULL != p_yaw_shift);

   p_vehicle_data = &p_pa_data->vehicle_data;

   /* checking for yaw_per_cycle to ensure the sine is giving big enough result.
    * The division by yawrate then must be OK since yawrate always is much bigger than yaw_per_cycle */
   if (Fbk_Abs_F(p_yaw_shift->angle) > EPSILON)
   {
      chord_length = 2.0f * (p_vehicle_data->host_speed / p_vehicle_data->yawrate) * Fast_Sin(Fbk_Half(p_yaw_shift->angle));
   }
   else
   {
      chord_length = p_vehicle_data->host_speed * p_pa_data->time_diff_to_last_cycle;
   }

   /* The angle of the chord in vcs coordinates is the angle between the host velocity and chord. The rear axle sideslip is
    * neglected for slow maneuvers which are typical for path tracking . */
   chord_angle = Create_Angle(Fbk_Half(p_yaw_shift->angle));

   local_time_update.x = chord_length * chord_angle.cos;
   local_time_update.y = chord_length * chord_angle.sin;

   return local_time_update;
}

static void Fbk_Add_Host_Trailer_Point(Fbk_Host_Trail_T *p_host_trail)
{
   uint8_t trail_index_previous;
   Vector_2d_T point2point;

   /* Assert */
   assert(NULL != p_host_trail);

   if (FBK_ZERO_UINT == p_host_trail->trail_index)
   {
      /* In case that buffer is full and trail index is 0, the previous index consists of the entry at maximum index.*/
      trail_index_previous = FBK_NUM_HOST_TRAIL_POINTS - FBK_ONE_UINT;
   }
   else
   {
      /*In case of non default values, the index is chosen as current index minus one.*/
      trail_index_previous = (uint8_t) (p_host_trail->trail_index - FBK_ONE_UINT);
   }

   /* Update properties of trail.*/
   p_host_trail->segments[p_host_trail->trail_index].point = p_host_trail->trail_host_position;
   point2point = Vector_2d_Alg_Diff(&p_host_trail->segments[trail_index_previous].point,
                                    &p_host_trail->segments[p_host_trail->trail_index].point);
   p_host_trail->segments[p_host_trail->trail_index].distance_traveled   = p_host_trail->trail_host_dist;
   p_host_trail->segments[p_host_trail->trail_index].dist_between_points = Vector_2d_Alg_Abs(&point2point);
   p_host_trail->segments[p_host_trail->trail_index].heading             = p_host_trail->trail_host_heading.angle;

   /* Reset criteria which are responsible for addition of points. */
   p_host_trail->trail_diff_dist    = FBK_ZERO_F;
   p_host_trail->trail_diff_heading = FBK_ZERO_F;

   /* Increase the index and set it back to zero in case that the buffer is full. */
   p_host_trail->trail_index++;
   if (p_host_trail->trail_index >= FBK_NUM_HOST_TRAIL_POINTS)
   {
      p_host_trail->trail_index         = FBK_ZERO_UINT;
      p_host_trail->f_trail_full_buffer = FBK_TRUE;
   }

   if (Fbk_Is_True(p_host_trail->f_trail_full_buffer))
   {
      /* trail_index is index where the next trail point will be stored, currently the old value is still valid */
      p_host_trail->oldest_trail_index = p_host_trail->trail_index;
   }
   else
   {
      /*buffer is not full and thus the oldest point is at zero.*/
      p_host_trail->oldest_trail_index = FBK_ZERO_UINT;
   }
}
