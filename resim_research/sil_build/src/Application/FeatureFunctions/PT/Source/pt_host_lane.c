/**
 * @file pt_host_lane.c
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

/* Pt internals */
#include "pt_host_lane.h"
#include "fbk_host_trail.h"
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "ml_angle_normalize.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_output_t.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Type definitions
\*===========================================================================*/

/**
 * @brief Structure which is giving reference to the host trail created by Feature Building Kit and a flag whether a path was
 * already created out of it to reduce time consumption each cycle.
 */

/*===========================================================================*\
* Static Function Declaration
\*===========================================================================*/

/**
 * @brief Gets the path direction of the host trail path based on the absolute shift of the host vehicle in wcs.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7623}
 * @verification{}
 */
static void Pt_Get_Trail_Based_Path_Dir(Pt_Path_T *p_path /**< path information*/,
                                        const Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS] /**< trail transformed to vcs*/,
                                        const Pt_Core_Calibration_T *p_cals /**< pt calibration data*/,
                                        const Fbk_Host_Trail_T *p_fbk_host_trail);

/**
 * @brief Transforms trail into path information at a given grid index.
 *
 * @return True if path point was added correctly at a given index.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7622}
 * @verification{}
 */
static boolean_T
Pt_Transform_Trail_To_Path_Points(Pt_Path_T *p_host_path /**< Path used for creation of host path*/,
                                  const Fbk_Host_Lane_Interval_T *p_interval /**< Interval to be used for path creation*/,
                                  const uint8_t grid_idx /**< grid index */,
                                  const Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS] /**< trail transformed to vcs*/,
                                  const float32_T grid_array[PT_NUM_GRID_POINTS] /**<grid array*/);

/**
 * @brief Searches for a suitable interval for path generation. This is needed such that a 90 degree turn for e.g. CTA is not
 * completly used for path creation but only the interesting part of the host trail.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7631}
 * @verification{}
 */
static boolean_T Pt_Search_Trail_Interval_For_Path_Gen(Fbk_Host_Lane_Interval_T *p_interval /**<interval to be in use*/,
                                                       const Pt_Core_Calibration_T *p_cals /**<*calibration parameters*/,
                                                       const Fbk_Host_Trail_T *p_fbk_host_trail);

/**
 * @brief Checks whether a path point for the given indices can be generated.
 *
 * @return True when the grid array entry is surrounded by the grid components.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7621}
 * @verification{}
 */
static boolean_T Pt_Can_Path_Point_Be_Generated(const float32_T *p_passed_grid_comp /**< first border*/,
                                                const float32_T *p_next_grid_comp /**< second border*/,
                                                const float32_T *p_grid_array_entry /**< grid entry to check*/);

/**
 * @brief Returns the index which is used for the host trail path.
 *
 * @return True when an index has been found.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7632}
 * @verification{}
 */
static boolean_T Pt_Get_Host_Trail_Path_Index(const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< all paths*/,
                                              uint8_t *p_path_index /**< index to be returned*/);

/**
 * @brief Checks whether a host trail path is existing.
 *
 * @return True when a path created out of the host trail is already existing.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7651}
 * @verification{}
 */
static boolean_T Pt_Is_Host_Lane_Path_Existent(const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< all paths*/);

/**
 * @brief Extrapolates non-grid borders of the path if possible.
 *
 * @return void.
 *
 * @SRS{SF-1567}
 * @SAE{}
 * @SDD{CSCSA-122370}
 * @verification{}
 */
static void Pt_Extrapolate_Non_Grid_Borders_Of_The_Path(Pt_Path_T *p_host_path /**< Path used for creation of host path*/,
                                                        const Pt_Input_T *p_pt_input /**< path tracking input*/);

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/
void Pt_Transform_Trail_To_Path(Pt_Persistent_T *p_pt_persistent,
                                const Pt_Core_Calibration_T *p_cals,
                                const Pt_Input_T *p_pt_input,
                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Fbk_Host_Trail_T *p_fbk_host_trail)
{
   uint8_t path_index = PT_DEFAULT_DISCR_BORDER;
   boolean_T f_trail_path_slot_found;
   boolean_T f_start_conditions_fulfilled;

   /* Assert */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_fbk_host_trail);

   /* Check if all requirements are fulfilled to run trail to path conversion. */
   f_start_conditions_fulfilled = (boolean_T) (Fbk_Is_True(p_cals->k_pt_enable_host_trail)
                                               && (Fbk_Is_True(p_fbk_host_trail->f_was_trail_point_added_this_cycle)
                                                   || Fbk_Is_False(Pt_Is_Host_Lane_Path_Existent(p_pt_persistent->paths)))
                                               && (p_vehicle_data->host_speed < p_cals->k_pt_trail_max_speed_trail_to_path_conv)
                                               && (p_fbk_host_trail->trail_host_dist > p_cals->k_pt_minimum_host_trail_length));

   f_trail_path_slot_found = Pt_Get_Host_Trail_Path_Index(p_pt_persistent->paths, &path_index);

   /* If not all requirements are fulfilled, return immediatelly. */
   if (Fbk_Is_True(f_start_conditions_fulfilled) && Fbk_Is_True(f_trail_path_slot_found))
   {
      boolean_T f_suitable_interval_found;
      Fbk_Host_Lane_Interval_T interval;

      /* At first extract boundaries of trail which shall be used for Path transformation.
      Take a look at the trail heading for this, such that parts for parking or turn maneuvers are not included in the host path. */
      f_suitable_interval_found = Pt_Search_Trail_Interval_For_Path_Gen(&interval, p_cals, p_fbk_host_trail);

      /* Only transform the host trail to path information in case that an useable interval is found. */
      if (Fbk_Is_True(f_suitable_interval_found))
      {
         Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS];
         Pt_Path_T *p_host_path                  = &(p_pt_persistent->paths[path_index]);
         boolean_T f_trail_points_mapped_to_path = FBK_FALSE;
         uint8_t grid_idx;

         /* Remember path properties before points mapping, which modifies these values. In case of failed path update, use memo
          * values to reset path state from before the search. */
         uint8_t memo_first_p = p_host_path->first_p;
         uint8_t memo_last_p  = p_host_path->last_p;

         Fbk_Init_Trail_Vcs(trail_vcs, p_fbk_host_trail, p_vehicle_data->rear_axle_position);

         Pt_Get_Trail_Based_Path_Dir(&p_pt_persistent->paths[path_index], trail_vcs, p_cals, p_fbk_host_trail);

         /* Initialize borders for search */
         p_host_path->last_p  = FBK_ZERO_UINT;
         p_host_path->first_p = PT_NUM_GRID_POINTS + PT_SINGLE_GRID_POINT_OFFSET;

         /* Map trail points to path points */
         for (grid_idx = FBK_ZERO_UINT; grid_idx < PT_NUM_GRID_POINTS; grid_idx++)
         {
            if (Fbk_Is_True(Pt_Transform_Trail_To_Path_Points(p_host_path, &interval, grid_idx, trail_vcs, p_pt_input->grid_pt_array))
                && (Fbk_Is_True(PT_SINGLE_GRID_POINT_OFFSET < p_host_path->last_p)))
            {
               f_trail_points_mapped_to_path = FBK_TRUE;
            }
         }

         /* Fill host trail path information only if any path points were added */
         if (Fbk_Is_True(f_trail_points_mapped_to_path))
         {
            Pt_Extrapolate_Non_Grid_Borders_Of_The_Path(p_host_path, p_pt_input);
            p_host_path->path_state    = PATH_STATUS_HOST_TRAIL;
            p_host_path->num_groupings = 5u;
         }
         else
         {
            /* Reset path borders if no path point was added */
            p_host_path->first_p = memo_first_p;
            p_host_path->last_p  = memo_last_p;
         }
      }
   }
}

/*===========================================================================*\
* Static Function Definition
\*===========================================================================*/

static boolean_T Pt_Transform_Trail_To_Path_Points(Pt_Path_T *p_host_path,
                                                   const Fbk_Host_Lane_Interval_T *p_interval,
                                                   const uint8_t grid_idx,
                                                   const Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS],
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t trail_point_ctr;
   boolean_T f_point_added = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_host_path);
   assert(NULL != p_interval);
   assert(NULL != trail_vcs);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_idx);

   for (trail_point_ctr = FBK_ONE_UINT; trail_point_ctr < p_interval->interval_range; trail_point_ctr++)
   {
      Fbk_Point_Pair_T index_pair;
      float32_T passed_grid_comp;
      float32_T passed_point_comp;
      float32_T next_grid_comp;
      float32_T next_point_comp;

      Fbk_Get_Point_Indices(&index_pair, p_interval, trail_point_ctr);

      if (Pt_Is_Path_Lateral(p_host_path))
      {
         passed_grid_comp  = trail_vcs[index_pair.passed].y;
         passed_point_comp = trail_vcs[index_pair.passed].x;
         next_grid_comp    = trail_vcs[index_pair.next].y;
         next_point_comp   = trail_vcs[index_pair.next].x;
      }
      else
      {
         passed_grid_comp  = trail_vcs[index_pair.passed].x;
         passed_point_comp = trail_vcs[index_pair.passed].y;
         next_grid_comp    = trail_vcs[index_pair.next].x;
         next_point_comp   = trail_vcs[index_pair.next].y;
      }

      if ((Fbk_Abs_F(passed_grid_comp - grid_array[grid_idx]) <= EPSILON))
      {
         p_host_path->path_points[grid_idx] = passed_point_comp;
         p_host_path->first_p               = Fbk_Min(p_host_path->first_p, grid_idx);
         p_host_path->last_p                = Fbk_Max(p_host_path->last_p, grid_idx);
         f_point_added                      = FBK_TRUE;
      }
      else if (Fbk_Abs_F(next_grid_comp - grid_array[grid_idx]) <= EPSILON)
      {
         p_host_path->path_points[grid_idx] = next_point_comp;
         p_host_path->first_p               = Fbk_Min(p_host_path->first_p, grid_idx);
         p_host_path->last_p                = Fbk_Max(p_host_path->last_p, grid_idx);
         f_point_added                      = FBK_TRUE;
      }
      else if (Pt_Can_Path_Point_Be_Generated(&passed_grid_comp, &next_grid_comp, &grid_array[grid_idx]))
      {
         p_host_path->path_points[grid_idx] = Get_Y_Value_From_Line_By_Coordinates(
            passed_grid_comp, passed_point_comp, next_grid_comp, next_point_comp, grid_array[grid_idx]);
         p_host_path->first_p = Fbk_Min(p_host_path->first_p, grid_idx);
         p_host_path->last_p  = Fbk_Max(p_host_path->last_p, grid_idx);
         f_point_added        = FBK_TRUE;
      }
      else
      {
         /*Do nothing */
      }

      if (Fbk_Is_True(f_point_added))
      {
         break;
      }
   }

   return f_point_added;
}

static void Pt_Extrapolate_Non_Grid_Borders_Of_The_Path(Pt_Path_T *p_host_path, const Pt_Input_T *p_pt_input)
{
   float32_T half_grid_point_width;

   /* Asserts */
   assert(NULL != p_host_path);
   assert(NULL != p_pt_input);
   assert(PT_SINGLE_GRID_POINT_OFFSET <= p_host_path->last_p);
   assert(PT_HIGHEST_GRID_POINT_INDEX >= p_host_path->last_p);
   assert(PT_LOWEST_GRID_POINT_INDEX <= p_host_path->first_p);
   assert(PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET >= p_host_path->first_p);

   half_grid_point_width = Fbk_Half(p_pt_input->grid_pt_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);

   /*Extrapolate non-grid borders of the path.*/
   if (Pt_Is_Path_Lateral(p_host_path))
   {
      p_host_path->first.x = Get_Y_Value_From_Line_By_Coordinates(
         p_pt_input->grid_pt_array[p_host_path->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_host_path->path_points[p_host_path->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_pt_input->grid_pt_array[p_host_path->first_p], p_host_path->path_points[p_host_path->first_p],
         p_pt_input->grid_pt_array[p_host_path->first_p] - half_grid_point_width);

      p_host_path->first.y = p_pt_input->grid_pt_array[p_host_path->first_p] - half_grid_point_width;

      p_host_path->last_mat.x = Get_Y_Value_From_Line_By_Coordinates(
         p_pt_input->grid_pt_array[p_host_path->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         p_host_path->path_points[p_host_path->last_p - PT_SINGLE_GRID_POINT_OFFSET], p_pt_input->grid_pt_array[p_host_path->last_p],
         p_host_path->path_points[p_host_path->last_p], p_pt_input->grid_pt_array[p_host_path->last_p] + half_grid_point_width);

      p_host_path->last_mat.y = p_pt_input->grid_pt_array[p_host_path->last_p] + half_grid_point_width;
   }
   else
   {
      p_host_path->first.y = Get_Y_Value_From_Line_By_Coordinates(
         p_pt_input->grid_pt_array[p_host_path->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_host_path->path_points[p_host_path->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_pt_input->grid_pt_array[p_host_path->first_p], p_host_path->path_points[p_host_path->first_p],
         p_pt_input->grid_pt_array[p_host_path->first_p] - half_grid_point_width);

      p_host_path->first.x = p_pt_input->grid_pt_array[p_host_path->first_p] - half_grid_point_width;

      p_host_path->last_mat.y = Get_Y_Value_From_Line_By_Coordinates(
         p_pt_input->grid_pt_array[p_host_path->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         p_host_path->path_points[p_host_path->last_p - PT_SINGLE_GRID_POINT_OFFSET], p_pt_input->grid_pt_array[p_host_path->last_p],
         p_host_path->path_points[p_host_path->last_p], p_pt_input->grid_pt_array[p_host_path->last_p] + half_grid_point_width);

      p_host_path->last_mat.x = p_pt_input->grid_pt_array[p_host_path->last_p] + half_grid_point_width;
   }
}

static boolean_T Pt_Can_Path_Point_Be_Generated(const float32_T *p_passed_grid_comp,
                                                const float32_T *p_next_grid_comp,
                                                const float32_T *p_grid_array_entry)
{
   boolean_T f_can_path_point_be_generated = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_passed_grid_comp);
   assert(NULL != p_next_grid_comp);
   assert(NULL != p_grid_array_entry);

   if (((*p_next_grid_comp < *p_grid_array_entry) && (*p_passed_grid_comp > *p_grid_array_entry))
       || ((*p_passed_grid_comp < *p_grid_array_entry) && (*p_next_grid_comp > *p_grid_array_entry)))
   {
      f_can_path_point_be_generated = FBK_TRUE;
   }

   return f_can_path_point_be_generated;
}

static void Pt_Get_Trail_Based_Path_Dir(Pt_Path_T *p_path,
                                        const Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS],
                                        const Pt_Core_Calibration_T *p_cals,
                                        const Fbk_Host_Trail_T *p_fbk_host_trail)
{
   /* Since the first two points of a path within path tracking are responsible for direction calculation, the first two points of
    * the trail are used here to determine the path direction.*/
   Vector_2d_T long_unit_vector;
   Vector_2d_T vector_of_interest;
   float32_T heading_temp;
   uint8_t next_point;

   /* Assert */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != trail_vcs);
   assert(NULL != p_fbk_host_trail);

   next_point = (uint8_t) (p_fbk_host_trail->oldest_trail_index + FBK_ONE_UINT);

   if (next_point >= FBK_NUM_HOST_TRAIL_POINTS)
   {
      /*Due to the ring buffer the oldest point could be at an higher index than the second oldest point.*/
      next_point = (uint8_t) (next_point - FBK_NUM_HOST_TRAIL_POINTS);
   }

   long_unit_vector   = Create_2d_Vector_X_Normal();
   vector_of_interest = Vector_2d_Alg_Diff(&trail_vcs[next_point], &trail_vcs[p_fbk_host_trail->oldest_trail_index]);
   heading_temp       = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &vector_of_interest));
   heading_temp       = Normalize_Angle(heading_temp, FBK_ZERO_F);

   /*Use limits similar to the path tracking internals*/
   if ((heading_temp >= p_cals->k_pt_lower_lim_obj_orient_lat) && (heading_temp <= p_cals->k_pt_upper_lim_obj_orient_lat))
   {
      p_path->direction = PATH_DIRECTION_LAT_RIGHT;
   }
   else
   {
      p_path->direction = PATH_DIRECTION_LONG_FORWARD;
   }
}

static boolean_T Pt_Is_Host_Lane_Path_Existent(const Pt_Path_T paths[PT_NUMBER_OF_PATHS])
{
   uint8_t idx;
   boolean_T f_path_is_existent = FBK_FALSE;

   /* Assert */
   assert(NULL != paths);

   for (idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      if (PATH_STATUS_HOST_TRAIL == paths[idx].path_state)
      {
         f_path_is_existent = FBK_TRUE;
         break;
      }
   }

   return f_path_is_existent;
}

static boolean_T Pt_Get_Host_Trail_Path_Index(const Pt_Path_T paths[PT_NUMBER_OF_PATHS], uint8_t *p_path_index)
{
   boolean_T f_empty_path_slot_found = FBK_FALSE;
   uint8_t idx;

   /* Assert */
   assert(NULL != paths);
   assert(NULL != p_path_index);

   for (idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      if (PATH_STATUS_HOST_TRAIL == paths[idx].path_state)
      {
         f_empty_path_slot_found = FBK_TRUE;
         *p_path_index           = idx;
         break;
      }
   }

   if (Fbk_Is_False(f_empty_path_slot_found))
   {
      f_empty_path_slot_found = Pt_Search_Empty_Path_Slot(p_path_index, paths);
   }

   return f_empty_path_slot_found;
}

static boolean_T Pt_Search_Trail_Interval_For_Path_Gen(Fbk_Host_Lane_Interval_T *p_interval,
                                                       const Pt_Core_Calibration_T *p_cals,
                                                       const Fbk_Host_Trail_T *p_fbk_host_trail)
{
   boolean_T f_interval_found = FBK_FALSE;
   uint8_t newest_idx;
   uint8_t outer_counter_limit;
   uint8_t inner_counter_limit;
   uint8_t outer_loop_counter;
   uint8_t diff_idx;

   /* Assert */
   assert(NULL != p_interval);
   assert(NULL != p_cals);
   assert(NULL != p_fbk_host_trail);

   /*Initialize the interval.*/
   p_interval->interval_start = PT_DEFAULT_DISCR_BORDER;
   p_interval->interval_end   = PT_DEFAULT_DISCR_BORDER;
   p_interval->interval_range = PT_DEFAULT_DISCR_BORDER;

   /* Get newest index (which is most likely nearest to the host vehicle) and the amount of iterations.*/
   if (Fbk_Is_True(p_fbk_host_trail->f_trail_full_buffer))
   {
      if (FBK_ZERO_UINT == p_fbk_host_trail->trail_index)
      {
         /*Prevent the underflow caused by uint8_t and ringbuffer structure of host trail.*/
         newest_idx = FBK_NUM_HOST_TRAIL_POINTS - FBK_ONE_UINT;
      }
      else
      {
         /*trail_idx is always set to the next index which shall be updated by host vehicle movement (thus the oldest point in case
          * that the ringbuffer is full). Newest index is thus the decrement of the oldest.*/
         newest_idx = (uint8_t) (p_fbk_host_trail->trail_index - FBK_ONE_UINT);
      }
      /*Counter limit is always set to amount of path points.*/
      outer_counter_limit = FBK_NUM_HOST_TRAIL_POINTS;
   }
   else
   {
      /*Set newest idx and limits for trails where the host trail ring buffer is not full yet.*/
      newest_idx          = (uint8_t) (p_fbk_host_trail->trail_index - FBK_ONE_UINT);
      outer_counter_limit = (uint8_t) (newest_idx + FBK_ONE_UINT);
   }

   /*Iterate starting from the next point with respect to the host vehicle. Ending with the oldest index.
   Here an interval is searched which has a similar heading inbetween.*/
   for (outer_loop_counter = FBK_ZERO_UINT; outer_loop_counter < outer_counter_limit; outer_loop_counter++)
   {
      uint8_t inner_loop_counter;
      uint8_t end_idx = PT_DEFAULT_DISCR_BORDER;
      uint8_t start_idx;

      if ((((int8_t) newest_idx) - ((int8_t) outer_loop_counter)) < FBK_ZERO_INT)
      {
         start_idx = (uint8_t) ((uint8_t) (FBK_NUM_HOST_TRAIL_POINTS + newest_idx) - outer_loop_counter);
      }
      else
      {
         start_idx = (uint8_t) (newest_idx - outer_loop_counter);
      }

      inner_counter_limit = (uint8_t) ((uint8_t) (outer_counter_limit - outer_loop_counter) - FBK_ONE_UINT);

      for (inner_loop_counter = FBK_ZERO_UINT; inner_loop_counter < inner_counter_limit; inner_loop_counter++)
      {
         /*Check the next points starting from our temporary start index, consisting of start_idx-inner_loop_counter*/
         if ((((int8_t) start_idx) - ((int8_t) inner_loop_counter) - FBK_ONE_INT) < FBK_ZERO_INT)
         {
            /*Check for the underflow caused by uint8_t limits.*/
            end_idx = (uint8_t) ((uint8_t) ((uint8_t) (FBK_NUM_HOST_TRAIL_POINTS + start_idx) - inner_loop_counter) - FBK_ONE_UINT);
         }
         else
         {
            end_idx = (uint8_t) ((uint8_t) (start_idx - inner_loop_counter) - FBK_ONE_UINT);
         }

         if (Fbk_Abs_F(p_fbk_host_trail->segments[start_idx].heading - p_fbk_host_trail->segments[end_idx].heading)
             > p_cals->k_pt_max_heading_diff_valid_interval)
         {
            /*Break when interval end is reached.*/
            break;
         }
      }

      if (end_idx != PT_DEFAULT_DISCR_BORDER)
      {
         if (end_idx > start_idx)
         {
            diff_idx = (uint8_t) (start_idx + FBK_NUM_HOST_TRAIL_POINTS - end_idx);
         }
         else
         {
            diff_idx = (uint8_t) (start_idx - end_idx);
         }

         if (diff_idx >= p_cals->k_pt_minimum_amount_of_trail_points)
         {
            f_interval_found           = FBK_TRUE;
            p_interval->interval_start = start_idx;
            p_interval->interval_end   = end_idx;
            p_interval->interval_range = (uint8_t) (diff_idx + FBK_ONE_UINT);
            break;
         }
      }
   }

   return f_interval_found;
}
