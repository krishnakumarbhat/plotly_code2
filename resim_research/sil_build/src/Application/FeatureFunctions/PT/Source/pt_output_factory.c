/**
 * @file pt_output_factory.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Additional information to the path-object pairs is calculated here.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_output_factory.h"
#include "fbk_array_interpolation.h"
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_angle_normalize.h"
#include "ml_line.h"
#include "ml_line_hesse.h"
#include "ml_line_hesse_t.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include <assert.h>

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief checks if the object has already passed the host vehicle
 *
 * @return True if the object has already passed the host
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7492}
 * @verification{}
 */
static boolean_T Pt_Has_Object_Already_Passed_The_Host(const Vector_2d_T *p_obj_pos /**< twodimensional position of object*/,
                                                       const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/);

/**
 * @brief Calculates the range between the given input point and the given path.
 * This input point may be a value between [0 and -host_length] for longitudinal paths
 * or [-0.5*host_width, 0.5*host_width] for lateral paths.
 *
 * @return range between input point and path in m
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7485}
 * @verification{}
 */
static float32_T Pt_Calc_Range_To_Path_At_Input_Pt(const Pt_Path_T *p_path,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                   const float32_T *p_input_point);

/**
 * @brief Cals it subfunction for calculation of range between host and path
 *  with a suitable wrapped input point.
 *
 * @return returns distance of predicted object to the respective host edge
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7486}
 * @verification{}
 */
static float32_T Pt_Calc_Range_To_Path_Wrapper(const Pt_Path_T *p_path,
                                               const Pt_Object_Mov_Direction_T *p_obj_move_dir,
                                               const Pt_Input_T *p_pt_input,
                                               const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief In case of calculations considering the mid grid point index e.g. range
 * path information is needed. If a path is not providing path information at this
 * point, the path shall be extrapolated.
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7489}
 * @verification{}
 */
static void Pt_Extrap_For_Zero_Point_Dep_Calc(Pt_Path_T *p_path,
                                              const Pt_Core_Calibration_T *p_cals,
                                              const float32_T grid_array[PT_NUM_GRID_POINTS]);

/**
 * @brief Calculates the length of the path. This length starts at the center of the object which gets
 * orthogonally projected on the path and ends at the intersection with coordinate axis.
 *
 *
 * @return length of the path in m
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7482}
 * @verification{}
 */
static float32_T Pt_Calc_Length_Of_Trajectory_To_Zero(
   const Pt_Path_T *p_path /**< respective path*/,
   const Fbk_Object_Data_T *p_object /**< object data*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/,
   const uint8_t next_point_idx /**<grid point index which is closest to object (left side of grid array)*/);

/**
 * @brief Calculates the path heading by use of range at zero under consideration
 * of obj moving direction
 *
 *
 * @return heading of path of type float32_T in rad
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7483}
 * @verification{}
 */
static float32_T Pt_Calc_Path_Heading(const Pt_Output_T *p_pt_output /**< path output struct*/,
                                      const Fbk_Object_Data_T *p_object /**< object data*/,
                                      const Pt_Object_Mov_Direction_T obj_move_dir /**<moving object direction*/);

/**
 * @brief summarizes which conditions need to be matched in order for the
 *  age counter to be reset.
 *
 *
 * @return True when path match has changed over successive cycles or when a
 *               falling edge has occured from match_last_cycle != -1 to match_current_cycle = -1
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7493}
 * @verification{}
 */
static boolean_T Pt_Has_Path_Change_Occured(const Pt_Output_T *p_pt_output, const Fbk_Object_Data_T *p_object);

/**
 * @brief This function summarizes which conditions need to be matched in order for the
 *  age counter to be incremented.
 *
 *
 * @return True when path match has been kept over successive cycles or when a
 *               rising edge has occured from match_last_cycle=-1 to match_current_cycle != -1
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7491}
 * @verification{}
 */
static boolean_T Pt_Has_Obj_Matched_For_Succ_Cycles(const Pt_Output_T *p_pt_output /**< path output structure*/,
                                                    const Fbk_Object_Data_T *p_object /**< object data*/);

/**
 * @brief increments or updates the track_match_age counter which is considered for the time
 * an object has been matched to a specific path.
 *
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7496}
 * @verification{}
 */
static void Pt_Update_Track_Match_Age(Pt_Output_T *p_pt_output /**<path output struct*/,
                                      const Fbk_Object_Data_T *p_object /**< object information*/);

/**
 * @brief  Wrapper for correction of the extrapolation of the points
 * This prevents the killing of paths, which get extrapolated in matching.
 * Mainly reverts changes by extrapolation in the output .
 *
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7488}
 * @verification{}
 */
static void Pt_Correct_For_Extrapolation(Pt_Path_T *p_path /**<respective path*/);

/**
 * @brief returns the object range to zero point.
 *
 * @return two dimensional vector from object to range at zero
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7487}
 * @verification{}
 */
static void Pt_Calc_Vector_Obj_To_Range_At_Zero(Vector_2d_T *p_vec_obj_to_range_at_zero /**<output vector for range at zero*/,
                                                const Pt_Output_T *p_pt_output /**< direction of a path*/,
                                                const Fbk_Object_Data_T *p_object /**< object info*/,
                                                const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/);

/**
 * @brief Calculates the distance between object and next path point.
 * For this the center position is projected orthogonally onto the
 * path segment.
 *
 * @return distance between orthogonally projected center and next path point in m
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7481}
 * @verification{}
 */
static float32_T Pt_Calc_Dist_To_First_Path_Point(
   const Pt_Path_T *p_path /**< path*/, const Fbk_Object_Data_T *p_object /**< object data*/, const float32_T grid_array[PT_NUM_GRID_POINTS] /**<  grid point array*/, const Fbk_Point_Pair_T *p_grid_point_interval /**< interval consisting of the closest grid point index and the following index, depending on path direction*/);

/**
 * @brief Maps path points and grid component to vector_2d_t types depending on the
 * direction of the path
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7494}
 * @verification{}
 */
static void Pt_Map_Path_Entities_To_Vectors(Vector_2d_T *p_vector_next /**<path point which will be passed next by obj*/,
                                            Vector_2d_T *p_vector_passed /**<path point which was passed previously by obj*/,
                                            const Pt_Path_T *p_path /**<path point which was passed previously by obj*/,
                                            const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/,
                                            const Fbk_Point_Pair_T *p_grid_point_interval /**< grid point interval*/);

/**
 * @brief resets defined parts of path_points array.
 *  after running this procedure the considered path is again in its prematching form.
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7495}
 * @verification{}
 */
static void Pt_Reset_Extrapolated_Points(Pt_Path_T *p_path /**<respective path*/,
                                         const uint8_t for_loop_start /**< start index of range which needs to be reset*/,
                                         const uint8_t for_loop_end /**< end index of range which needs to be reset*/);

/**
 * @brief Calculates the range between object and its current match. When the object is moving on the right side of a path, the
 * distance is expected to be negative while it is positive when the object is moving on the left side of the path.
 *
 * @return returns range between the object to its currently matched path segment
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7484}
 * @verification{}
 */
static float32_T Pt_Calc_Range_to_Path_At_Curr_Pos(
   const Pt_Path_T *p_path /**< path matched to obj*/,
   const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< moving direction*/,
   const Fbk_Object_Data_T *p_object /**< moving direction*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/,
   const uint8_t next_point_idx /**<grid point index which is closest to object (left side of grid array)*/);

/**
 * @brief  Returns the surrounding path points for an object position masked as closest_grid_point_index
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7490}
 * @verification{}
 */
static void Pt_Get_Surrounding_Path_Points(
   Fbk_Point_Pair_T *p_grid_point_interval /**<surrounding point pair*/,
   const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< object moving direction*/,
   const uint8_t next_point_idx /**<grid point index which is closest to object (left side of grid array)*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Pt_Set_Best_Matching_Path_Output(Pt_Output_T *p_pt_output,
                                      Pt_Persistent_T *p_pt_persistent,
                                      const Pt_Path_Obj_Pair_Consumer_Info_T *p_path_consumer_info,
                                      const Pt_Input_T *p_pt_input,
                                      const Pt_Object_T *p_object,
                                      const Pt_Core_Calibration_T *p_cals,
                                      const Pt_Object_Mov_Direction_T obj_move_dir,
                                      const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_pt_output);
   assert(NULL != p_pt_persistent);
   assert(NULL != p_path_consumer_info);
   assert(NULL != p_pt_input);
   assert(NULL != p_object);
   assert(NULL != p_cals);

   /*Set index related values for object even if no match has occured.
   Every other output shall be filled only in case of valid match*/
   p_pt_output->path_obj_pair_output[p_object->tracker_data.index].track_match_last_cycle =
      p_pt_persistent->path_index_last_cycle[p_object->tracker_data.id];
   p_pt_output->path_obj_pair_output[p_object->tracker_data.index].track_match =
      p_pt_persistent->best_path_obj_pairs[p_object->tracker_data.id].path_index;
   p_pt_output->nearest_path_output[p_object->tracker_data.index].range_vcs_proj_to_path_segment =
      p_path_consumer_info->distance_to_path;
   p_pt_output->nearest_path_output[p_object->tracker_data.index].segment_heading_diff = p_path_consumer_info->segment_heading_diff;
   p_pt_output->nearest_path_output[p_object->tracker_data.index].track_idx_nearest_path = p_path_consumer_info->path_index;
   Pt_Update_Track_Match_Age(p_pt_output, &p_object->tracker_data);

   if (PT_DEFAULT_MATCH_INDEX != p_pt_persistent->best_path_obj_pairs[p_object->tracker_data.id].path_index)
   {
      Pt_Path_T *p_best_path;
      uint8_t next_point_idx;

      p_best_path    = &(p_pt_persistent->paths[p_pt_persistent->best_path_obj_pairs[p_object->tracker_data.id].path_index]);
      next_point_idx = Pt_Read_Next_Point_Dep_On_Path_Dir(p_next_point_idx, p_best_path);

      /*Extrapolate in case that path does not have points around PT_MID_GRID_POINT_INDEX yet*/
      Pt_Extrap_For_Zero_Point_Dep_Calc(p_best_path, p_cals, p_pt_input->grid_pt_array);

      /*Fill Path output structure*/
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_at_host_edge =
         Pt_Calc_Range_To_Path_Wrapper(p_best_path, &(obj_move_dir), p_pt_input, p_vehicle_data);
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_to_current_path_part = Pt_Calc_Range_to_Path_At_Curr_Pos(
         p_best_path, &(obj_move_dir), &p_object->tracker_data, p_pt_input->grid_pt_array, next_point_idx);

      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_at_zero =
         p_best_path->path_points[PT_MID_GRID_POINT_INDEX];
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].length_of_trajectory = Pt_Calc_Length_Of_Trajectory_To_Zero(
         p_best_path, &p_object->tracker_data, p_pt_input->grid_pt_array, obj_move_dir, next_point_idx);
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_direction = p_best_path->direction;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_heading =
         Pt_Calc_Path_Heading(p_pt_output, &p_object->tracker_data, obj_move_dir);
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_state = p_best_path->path_state;

      /*Revert extrapolation if it occured*/
      Pt_Correct_For_Extrapolation(p_best_path);
   }
   else
   {
      /*Set default values for non track match related values*/
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_at_zero              = PT_HIGH_DISTANCE_DEFAULT_VAL;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_to_current_path_part = PT_HIGH_DISTANCE_DEFAULT_VAL;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].range_at_host_edge         = PT_HIGH_DISTANCE_DEFAULT_VAL;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].length_of_trajectory       = -FBK_ONE_F;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_heading               = FBK_ZERO_F;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_direction             = PATH_DIRECTION_NONE;
      p_pt_output->path_obj_pair_output[p_object->tracker_data.index].path_state                 = PATH_STATUS_DEFAULT;
   }

   /* update persistent data
      this cycles best match becomes last cycles best match */
   p_pt_persistent->path_index_last_cycle[p_object->tracker_data.id] =
      p_pt_persistent->best_path_obj_pairs[p_object->tracker_data.id].path_index;
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Pt_Has_Object_Already_Passed_The_Host(const Vector_2d_T *p_obj_pos, const Pt_Object_Mov_Direction_T obj_move_dir)
{
   /* Assert */
   assert(NULL != p_obj_pos);

   return (boolean_T) (((PT_OBJECT_MOV_DIR_LONG_FORWARD == obj_move_dir) && (p_obj_pos->x > 0.0f))
                       || ((PT_OBJECT_MOV_DIR_LONG_BACKWARD == obj_move_dir) && (p_obj_pos->x < 0.0f))
                       || ((PT_OBJECT_MOV_DIR_LAT_RIGHT == obj_move_dir) && (p_obj_pos->y > 0.0f))
                       || ((PT_OBJECT_MOV_DIR_LAT_LEFT == obj_move_dir) && (p_obj_pos->y < 0.0f)));
}

static void Pt_Extrap_For_Zero_Point_Dep_Calc(Pt_Path_T *p_path,
                                              const Pt_Core_Calibration_T *p_cals,
                                              const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   boolean_T f_points_mid;
   boolean_T f_points_plus;
   boolean_T f_points_minus;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != grid_array);

   f_points_mid = (boolean_T) (Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX]) > THRESHOLD_IS_ZERO);
   f_points_plus =
      (boolean_T) (Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]) > THRESHOLD_IS_ZERO);
   f_points_minus =
      (boolean_T) (Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]) > THRESHOLD_IS_ZERO);

   if (f_points_mid && f_points_plus && f_points_minus)
   {
      /*Do nothing*/
   }
   else
   {
      Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt(p_path, p_cals, grid_array);
   }
}

static float32_T Pt_Calc_Range_To_Path_Wrapper(const Pt_Path_T *p_path,
                                               const Pt_Object_Mov_Direction_T *p_obj_move_dir,
                                               const Pt_Input_T *p_pt_input,
                                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T input_point;
   float32_T range_at_host_edge;

   /* Asserts */
   assert(NULL != p_pt_input);
   assert(NULL != p_vehicle_data);

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      input_point = -p_vehicle_data->host_length;
   }
   else
   {
      if (Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(p_obj_move_dir))
      {
         input_point = -Fbk_Half(p_vehicle_data->host_width);
      }
      else
      {
         input_point = Fbk_Half(p_vehicle_data->host_width);
      }
   }

   range_at_host_edge = Pt_Calc_Range_To_Path_At_Input_Pt(p_path, p_pt_input->grid_pt_array, &(input_point));

   return range_at_host_edge;
}


static float32_T Pt_Calc_Range_to_Path_At_Curr_Pos(const Pt_Path_T *p_path,
                                                   const Pt_Object_Mov_Direction_T *p_obj_move_dir,
                                                   const Fbk_Object_Data_T *p_object,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                   const uint8_t next_point_idx)
{
   Vector_2d_T origin;
   Vector_2d_T end_of_line;
   float32_T relevant_obj_pos_extrap;
   float32_T obj_on_path_position;
   float32_T pos_for_range;
   float32_T range_to_current_path_part;
   Fbk_Point_Pair_T grid_point_interval;
   Line_Hesse_T line;
   Side_Of_Line_Hesse_T side_of_point;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_obj_move_dir);
   assert(NULL != p_object);
   assert(NULL != grid_array);

   /*Surrounding path points*/
   Pt_Get_Surrounding_Path_Points(&grid_point_interval, p_obj_move_dir, next_point_idx);

   /*Range to current position*/
   if (Pt_Is_Obj_Moving_Lateral(p_obj_move_dir))
   {
      relevant_obj_pos_extrap = p_object->vcs_pos.y;
      pos_for_range           = p_object->vcs_pos.x;
   }
   else
   {
      relevant_obj_pos_extrap = p_object->vcs_pos.x;
      pos_for_range           = p_object->vcs_pos.y;
   }

   obj_on_path_position = Get_Y_Value_From_Line_By_Coordinates(
      grid_array[grid_point_interval.passed], p_path->path_points[grid_point_interval.passed],
      grid_array[grid_point_interval.next], p_path->path_points[grid_point_interval.next], relevant_obj_pos_extrap);

   /*Get side on which the object is located dependent on the path */
   if (Pt_Is_Obj_Mov_Dir_Against_Vcs(p_obj_move_dir))
   {
      if (Pt_Is_Path_Lateral(p_path))
      {
         origin = Create_2d_Vector_Coordinates(p_path->path_points[grid_point_interval.next], grid_array[grid_point_interval.next]);
         end_of_line =
            Create_2d_Vector_Coordinates(p_path->path_points[grid_point_interval.passed], grid_array[grid_point_interval.passed]);
      }
      else
      {
         origin = Create_2d_Vector_Coordinates(grid_array[grid_point_interval.next], p_path->path_points[grid_point_interval.next]);
         end_of_line =
            Create_2d_Vector_Coordinates(grid_array[grid_point_interval.passed], p_path->path_points[grid_point_interval.passed]);
      }
   }
   else
   {
      if (Pt_Is_Path_Lateral(p_path))
      {
         origin =
            Create_2d_Vector_Coordinates(p_path->path_points[grid_point_interval.passed], grid_array[grid_point_interval.passed]);
         end_of_line =
            Create_2d_Vector_Coordinates(p_path->path_points[grid_point_interval.next], grid_array[grid_point_interval.next]);
      }
      else
      {
         origin =
            Create_2d_Vector_Coordinates(grid_array[grid_point_interval.passed], p_path->path_points[grid_point_interval.passed]);
         end_of_line =
            Create_2d_Vector_Coordinates(grid_array[grid_point_interval.next], p_path->path_points[grid_point_interval.next]);
      }
   }

   line          = Line_Hesse_Create_Using_Two_Points(&(origin), &(end_of_line));
   side_of_point = Line_Hesse_Get_Side_of_Point(&(line), &(p_object->vcs_pos), FBK_ZERO_F);

   if (LINE_HESSE_SIDE_NEGATIVE == side_of_point)
   {
      range_to_current_path_part = -Fbk_Abs_F(obj_on_path_position - pos_for_range);
   }
   else
   {
      range_to_current_path_part = Fbk_Abs_F(obj_on_path_position - pos_for_range);
   }

   return range_to_current_path_part;
}

static void Pt_Get_Surrounding_Path_Points(Fbk_Point_Pair_T *p_grid_point_interval,
                                           const Pt_Object_Mov_Direction_T *p_obj_move_dir,
                                           const uint8_t next_point_idx)
{
   /* Asserts */
   assert(NULL != p_grid_point_interval);
   assert(NULL != p_obj_move_dir);

   /* Get surrounding path points near the object*/
   p_grid_point_interval->next = next_point_idx;
   if (Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(p_obj_move_dir))
   {
      /*Objects aligned with coordinate system*/
      p_grid_point_interval->passed = (uint8_t) (next_point_idx - PT_SINGLE_GRID_POINT_OFFSET);
   }
   else
   {
      p_grid_point_interval->passed = (uint8_t) (next_point_idx + PT_SINGLE_GRID_POINT_OFFSET);
   }
}

static float32_T Pt_Calc_Range_To_Path_At_Input_Pt(const Pt_Path_T *p_path,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                   const float32_T *p_input_point)
{
   float32_T range_to_path;
   uint8_t idx_point_left_to_in_pt;
   uint8_t idx_point_right_to_in_pt;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != grid_array);
   assert(NULL != p_input_point);

   idx_point_left_to_in_pt  = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(grid_array, PT_NUM_GRID_POINTS, *p_input_point);
   idx_point_right_to_in_pt = (uint8_t) (idx_point_left_to_in_pt - PT_SINGLE_GRID_POINT_OFFSET);

   range_to_path = Get_Y_Value_From_Line_By_Coordinates(grid_array[idx_point_left_to_in_pt],
                                                        p_path->path_points[idx_point_left_to_in_pt],
                                                        grid_array[idx_point_right_to_in_pt],
                                                        p_path->path_points[idx_point_right_to_in_pt], *p_input_point);
   return range_to_path;
}

static float32_T Pt_Calc_Length_Of_Trajectory_To_Zero(const Pt_Path_T *p_path,
                                                      const Fbk_Object_Data_T *p_object,
                                                      const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                      const Pt_Object_Mov_Direction_T obj_move_dir,
                                                      const uint8_t next_point_idx)
{
   boolean_T f_mid_grid_point_passed;
   float32_T distance = -FBK_ONE_F;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != grid_array);

   /* check if current object already passed middle grid point */
   f_mid_grid_point_passed = Pt_Has_Object_Already_Passed_The_Host(&p_object->vcs_pos, obj_move_dir);

   if ((Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX]) > PT_PATH_POINTS_DEFAULT_VAL) && Fbk_Is_False(f_mid_grid_point_passed))
   {
      uint8_t i;
      Fbk_Point_Pair_T grid_point_interval;
      Vector_2d_T vec_passed;
      Vector_2d_T vec_next;

      /* Get surrounding path points near the object*/
      Pt_Get_Surrounding_Path_Points(&grid_point_interval, &obj_move_dir, next_point_idx);

      /* calculate initial distance from object to first grid point */
      distance = Pt_Calc_Dist_To_First_Path_Point(p_path, p_object, grid_array, &grid_point_interval);

      /*Accumulate path segment distances.*/
      if (grid_point_interval.passed < grid_point_interval.next)
      {
         for (i = (uint8_t) (grid_point_interval.passed + PT_SINGLE_GRID_POINT_OFFSET); i < PT_MID_GRID_POINT_INDEX; i++)
         {
            grid_point_interval.passed = i;
            grid_point_interval.next   = (uint8_t) (i + PT_SINGLE_GRID_POINT_OFFSET);
            Pt_Map_Path_Entities_To_Vectors(&vec_next, &vec_passed, p_path, grid_array, &grid_point_interval);
            distance += Vector_2d_Alg_Distance(&(vec_next), &(vec_passed));
         }
      }
      else
      {
         for (i = (uint8_t) (grid_point_interval.passed - PT_SINGLE_GRID_POINT_OFFSET); i > PT_MID_GRID_POINT_INDEX; i--)
         {
            grid_point_interval.passed = i;
            grid_point_interval.next   = (uint8_t) (i - PT_SINGLE_GRID_POINT_OFFSET);
            Pt_Map_Path_Entities_To_Vectors(&vec_next, &vec_passed, p_path, grid_array, &grid_point_interval);
            distance += Vector_2d_Alg_Distance(&(vec_next), &(vec_passed));
         }
      }
   }

   return distance;
}

static float32_T Pt_Calc_Path_Heading(const Pt_Output_T *p_pt_output,
                                      const Fbk_Object_Data_T *p_object,
                                      const Pt_Object_Mov_Direction_T obj_move_dir)
{
   Vector_2d_T vector_obj_to_range_at_zero;
   Vector_2d_T long_unit_vector;
   float32_T path_heading;

   /* Asserts */
   assert(NULL != p_pt_output);
   assert(NULL != p_object);

   Pt_Calc_Vector_Obj_To_Range_At_Zero(&(vector_obj_to_range_at_zero), p_pt_output, p_object, obj_move_dir);
   long_unit_vector = Create_2d_Vector_X_Normal();
   path_heading     = p_object->vcs_heading;

   if (Vector_2d_Alg_Abs(&vector_obj_to_range_at_zero) > THRESHOLD_IS_ZERO)
   {
      if (PT_OBJECT_MOV_DIR_LAT_RIGHT == obj_move_dir)
      {
         path_heading = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &vector_obj_to_range_at_zero));
         path_heading = Normalize_Angle(path_heading, FBK_ZERO_F);
      }
      else if (PT_OBJECT_MOV_DIR_LAT_LEFT == obj_move_dir)
      {
         path_heading = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &vector_obj_to_range_at_zero));
         path_heading = -Normalize_Angle(path_heading, FBK_ZERO_F);
      }
      else if ((PT_OBJECT_MOV_DIR_LONG_FORWARD == obj_move_dir) || (PT_OBJECT_MOV_DIR_LONG_BACKWARD == obj_move_dir))
      {
         path_heading = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &vector_obj_to_range_at_zero));

         if (vector_obj_to_range_at_zero.y > FBK_ZERO_F)
         {
            path_heading = Normalize_Angle(path_heading, FBK_ZERO_F);
         }
         else
         {
            path_heading = -Normalize_Angle(path_heading, FBK_ZERO_F);
         }
      }
      else
      {
         /* Do nothing*/
      }
   }

   return path_heading;
}

static boolean_T Pt_Has_Path_Change_Occured(const Pt_Output_T *p_pt_output, const Fbk_Object_Data_T *p_object)
{
   /* Assert */
   assert(NULL != p_pt_output);
   assert(NULL != p_object);

   return (boolean_T) (((p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle
                         != p_pt_output->path_obj_pair_output[p_object->index].track_match)
                        && (PT_DEFAULT_MATCH_INDEX == p_pt_output->path_obj_pair_output[p_object->index].track_match)
                        && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle))
                       || ((p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle
                            != p_pt_output->path_obj_pair_output[p_object->index].track_match)
                           && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle)
                           && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match)));
}

static boolean_T Pt_Has_Obj_Matched_For_Succ_Cycles(const Pt_Output_T *p_pt_output, const Fbk_Object_Data_T *p_object)
{
   /* Assert */
   assert(NULL != p_pt_output);
   assert(NULL != p_object);

   return (boolean_T) (((p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle
                         != p_pt_output->path_obj_pair_output[p_object->index].track_match)
                        && (PT_DEFAULT_MATCH_INDEX == p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle)
                        && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match))
                       || ((p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle
                            == p_pt_output->path_obj_pair_output[p_object->index].track_match)
                           && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match_last_cycle)
                           && (PT_DEFAULT_MATCH_INDEX != p_pt_output->path_obj_pair_output[p_object->index].track_match)));
}

static void Pt_Update_Track_Match_Age(Pt_Output_T *p_pt_output, const Fbk_Object_Data_T *p_object)
{
   /* Assert */
   assert(NULL != p_pt_output);
   assert(NULL != p_object);

   if (Pt_Has_Obj_Matched_For_Succ_Cycles(p_pt_output, p_object))
   {
      Sat_Inc_Uint8(&p_pt_output->path_obj_pair_output[p_object->index].track_match_age);
   }
   else if (Pt_Has_Path_Change_Occured(p_pt_output, p_object))
   {
      p_pt_output->path_obj_pair_output[p_object->index].track_match_age = FBK_ZERO_UINT;
   }
   else
   {
      /*Do nothing*/
   }
}

static void Pt_Correct_For_Extrapolation(Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   if ((p_path->first_p > (PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET))
       && (FBK_ZERO_UINT != p_path->obj_curr_used_for_path_build.id))
   {
      Pt_Reset_Extrapolated_Points(p_path, PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET,
                                   (uint8_t) (p_path->first_p - PT_SINGLE_GRID_POINT_OFFSET));
   }

   if ((p_path->last_p < (PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET))
       && (FBK_ZERO_UINT != p_path->obj_curr_used_for_path_build.id))
   {
      Pt_Reset_Extrapolated_Points(p_path, (uint8_t) (p_path->last_p + PT_SINGLE_GRID_POINT_OFFSET),
                                   PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET);
   }
}

static void Pt_Calc_Vector_Obj_To_Range_At_Zero(Vector_2d_T *p_vec_obj_to_range_at_zero,
                                                const Pt_Output_T *p_pt_output,
                                                const Fbk_Object_Data_T *p_object,
                                                const Pt_Object_Mov_Direction_T obj_move_dir)
{
   boolean_T f_obj_already_driven_by;

   /* Asserts */
   assert(NULL != p_vec_obj_to_range_at_zero);
   assert(NULL != p_pt_output);
   assert(NULL != p_object);

   if ((PT_OBJECT_MOV_DIR_LONG_BACKWARD == obj_move_dir) || (PT_OBJECT_MOV_DIR_LONG_FORWARD == obj_move_dir))
   {
      p_vec_obj_to_range_at_zero->x = -p_object->vcs_pos.x;
      p_vec_obj_to_range_at_zero->y = p_pt_output->path_obj_pair_output[p_object->index].range_at_zero
                                      + p_pt_output->path_obj_pair_output[p_object->index].range_to_current_path_part
                                      - p_object->vcs_pos.y;
   }
   else
   {
      /*Correct for the sign information of range to current path part*/
      p_vec_obj_to_range_at_zero->x = p_pt_output->path_obj_pair_output[p_object->index].range_at_zero
                                      - p_pt_output->path_obj_pair_output[p_object->index].range_to_current_path_part
                                      - p_object->vcs_pos.x;
      p_vec_obj_to_range_at_zero->y = -p_object->vcs_pos.y;
   }

   f_obj_already_driven_by = Pt_Has_Object_Already_Passed_The_Host(&p_object->vcs_pos, obj_move_dir);

   if (Fbk_Is_True(f_obj_already_driven_by))
   {
      *p_vec_obj_to_range_at_zero = Vector_2d_Alg_Multiply_Scalar(p_vec_obj_to_range_at_zero, -FBK_ONE_F);
   }
}

static float32_T Pt_Calc_Dist_To_First_Path_Point(const Pt_Path_T *p_path,
                                                  const Fbk_Object_Data_T *p_object,
                                                  const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                  const Fbk_Point_Pair_T *p_grid_point_interval)
{
   Vector_2d_T orthogonal_proj_vec;
   Vector_2d_T directional_vec_path_points;
   Vector_2d_T vec_passed;
   Vector_2d_T vec_next;
   float32_T scalar_product;

   /* Asserts */
   assert(NULL != p_object);

   /*get points of paths and direction vector between them*/
   Pt_Map_Path_Entities_To_Vectors(&vec_next, &vec_passed, p_path, grid_array, p_grid_point_interval);
   directional_vec_path_points = Vector_2d_Alg_Diff(&(vec_next), &(vec_passed));
   directional_vec_path_points = Vector_2d_Alg_Normalize_Vector(&(directional_vec_path_points));

   /*do the orthogonal projection*/
   orthogonal_proj_vec = Vector_2d_Alg_Diff(&p_object->vcs_pos, &(vec_passed));
   scalar_product      = Vector_2d_Alg_Scalar_Product(&(orthogonal_proj_vec), &(directional_vec_path_points));
   orthogonal_proj_vec = Vector_2d_Alg_Multiply_Scalar(&(directional_vec_path_points), scalar_product);
   orthogonal_proj_vec = Vector_2d_Alg_Add(&(orthogonal_proj_vec), &(vec_passed));

   /*Get distance*/
   return Vector_2d_Alg_Distance(&(orthogonal_proj_vec), &(vec_next));
}

static void Pt_Map_Path_Entities_To_Vectors(Vector_2d_T *p_vector_next,
                                            Vector_2d_T *p_vector_passed,
                                            const Pt_Path_T *p_path,
                                            const float32_T grid_array[PT_NUM_GRID_POINTS],
                                            const Fbk_Point_Pair_T *p_grid_point_interval)
{
   /* Asserts */
   assert(NULL != p_vector_next);
   assert(NULL != p_vector_passed);
   assert(NULL != p_path);
   assert(NULL != grid_array);
   assert(NULL != p_grid_point_interval);

   if (Pt_Is_Path_Lateral(p_path))
   {
      *p_vector_next =
         Create_2d_Vector_Coordinates(p_path->path_points[p_grid_point_interval->next], grid_array[p_grid_point_interval->next]);
      *p_vector_passed = Create_2d_Vector_Coordinates(p_path->path_points[p_grid_point_interval->passed],
                                                      grid_array[p_grid_point_interval->passed]);
   }
   else
   {
      *p_vector_next =
         Create_2d_Vector_Coordinates(grid_array[p_grid_point_interval->next], p_path->path_points[p_grid_point_interval->next]);
      *p_vector_passed = Create_2d_Vector_Coordinates(grid_array[p_grid_point_interval->passed],
                                                      p_path->path_points[p_grid_point_interval->passed]);
   }
}

static void Pt_Reset_Extrapolated_Points(Pt_Path_T *p_path, const uint8_t for_loop_start, const uint8_t for_loop_end)
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_path);
   assert(for_loop_end <= PT_HIGHEST_GRID_POINT_INDEX);

   for (idx = for_loop_start; idx <= for_loop_end; idx++)
   {
      p_path->path_points[idx] = PT_PATH_POINTS_DEFAULT_VAL;
   }
}
