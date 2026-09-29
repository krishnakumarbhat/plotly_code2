/**
 * @file pt_path_rotation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions for the path rotation module within path tracking algorithm
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_path_rotation.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_output.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_debug_interface.h"
#include "pt_output_t.h"
#include "pt_reset.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Structs
\*===========================================================================*/

/**
 * @brief This struct summarizes the path borders of the given path. Those path borders are dependend on the path direction
 * (backward/forward), which is the reason for the need to search for the lower and upper border. But also the distinction between
 * lateral and longitudinal path is needed, since the grid and value component differ between those directions.
 */
typedef struct
{
   float32_T first_obj_pos_grid_component; /**<grid component of first of path struct (long paths - x / lat paths - y)*/
   float32_T first_obj_pos_val_component;  /**<val component of first of path struct (long paths - x / lat paths - y)*/
   float32_T last_obj_pos_grid_component;  /**<grid component of last_mat of path struct (long paths - x / lat paths - y)*/
   float32_T last_obj_pos_val_component;   /**<val component of last_mat of path struct (long paths - x / lat paths - y)*/
} Pt_Obj_Borders_Of_Path_T;

/**
 * @brief This struct is needed for the discrete temporary boundaries which need to be rotated at each time step. Those can differ
 * from the path boundaries provided by original path struct due to new path points (created within same scanindex) or due to an
 * extending process of path where those boundaries may be updated.
 */
typedef struct
{
   uint8_t temp_discr_first_boundary; /**< discrete boundary of path beginning (similar to first_p with upper exceptions)*/
   uint8_t temp_discr_last_boundary;  /**< discrete boundary of path ending (similar to last_p with upper exceptions)*/
} Pt_Discr_Borders_Of_Path_T;

/**
 * @brief This struct is needed for processing of rotation and adaption onto the original grid point array. Especially the grid
 * point array makes the usage of this necessary, since the grid point is also subject to the rotation of ego vehicle.
 */
typedef struct
{
   float32_T grid_point_table_transform[PT_NUM_GRID_POINTS]; /**< temporaray grid point array which is needed for rotation and
                                                              * adaption before values of that are returned in original path
                                                              * struct*/
   float32_T path_point_array_transform[PT_NUM_GRID_POINTS]; /**< temporaray path point array which is needed for rotation and
                                                              * adaption before values of that are returned in original path
                                                              * struct*/
} Pt_Temporary_Path_T;

/**
 * @brief Summarizing size change for a given path, indicating whether the path has changed its size at lower or upper border.
 */
typedef struct
{
   boolean_T f_path_changed_size_at_lower_border; /**< flag indicating whether path has changed its size at the lower border*/
   boolean_T f_path_changed_size_at_upper_border; /**< flag indicating whether path has changed its size at the upper border*/
} Pt_Path_Size_Change_Flags_T;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief maps coordinates of the path borders to a grid component and value component.
 * Needed for path adaption on vcs local grid.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7516}
 * @verification{}
 */
static void Pt_Get_Relev_Pos_For_Border_Adaption(
   const Pt_Path_T *p_path /**<  respective path*/,
   Pt_Obj_Borders_Of_Path_T *p_path_borders /**< struct that summarizes the path borders defined by the tracked object*/);

/**
 * @brief checks if the path has shrunked and adapts it, if this is the case.
 * This can occure if the recorded paths are subject to pure translation but also due to rotation.
 * For this procedure the positions of the vehicles (boundaries of paths) are considered.
 *
 * @return void
 *
 * @SRS{SF-1574}
 * @SAE{SF-2918}
 * @SDD{SF-7507}
 * @verification{}
 */
static void Pt_Adapt_Path_For_Shrinkage(
   Pt_Path_T *p_path /**<respective path*/,
   Pt_Temporary_Path_T *p_temp_path_to_transform /**< temp paths local array needed for changing operations of path point values*/,
   const Pt_Obj_Borders_Of_Path_T *path_borders /**< struct that summarizes the path borders defined by the tracked object*/,
   Pt_Discr_Borders_Of_Path_T
      *p_discr_borders_of_path /**< temporary borders in which the path needs to be written back after rotation*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief checks if the path has extended and adapts it, if this is the case.
 * This can occure if the recorded path is subject to pure shift but also due to rotation (fold back effect).
 * For this procedure the positions of the vehicles (path boundaries) are considered.
 *
 * @return void
 *
 * @SRS{SF-1576}
 * @SAE{SF-2918}
 * @SDD{SF-7506}
 * @verification{}
 */
static void Pt_Adapt_Path_For_Extension(
   Pt_Path_T *p_path /**<respective path*/,
   Pt_Temporary_Path_T *p_temp_path_to_transform /**<temporary path which may be adapted due to extension*/,
   const Pt_Obj_Borders_Of_Path_T *path_borders /**< struct that summarizes the path borders defined by the tracked object*/,
   Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**< temporary borders of path points which need to be written back*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief returns the index of the first point which is unequal to zero when iterating from first_p to last_p in array.
 * This point might differ from first_p in special cases like lane change detection or turn maneuvers of ego while target
 * is driving behind it and does not follow.
 *
 * @return index first point which is unequal to zero when iterating from first_p to last_p
 *
 * @SRS{SF-1576}
 * @SAE{SF-2918}
 * @SDD{SF-7514}
 * @verification{}
 */
static uint8_t Pt_Get_First_Point_Unequal_Zero(const Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief returns the index of the first point which is unequal to zero when iterating from last_p to first_p in array.
 * This point might differ from last_p in special cases like lane change detection or turn maneuvers of ego while target
 * is driving behind it and does not follow.
 *
 * @return index of first point unequal to zero when iterating from last_p to first_p
 *
 * @SRS{SF-1576}
 * @SAE{SF-2918}
 * @SDD{SF-7515}
 * @verification{}
 */
static uint8_t Pt_Get_Last_Point_Unequal_Zero(const Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief determines the shifting vector of the ego vehicle
 * (indirectly depending on position in last scanindex).
 * Also a case distinction is applied here for small angles in which the taylor series is applied.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7512}
 * @verification{}
 */
static void Pt_Determine_Ego_Shifting_Vector(Vector_2d_T *p_delta_ego_veh_posn /**<  ego shifting vector*/,
                                             const Pa_Data_T *p_pa_data,
                                             const Pt_Core_Calibration_T *p_cals /**<  calibration parameters*/,
                                             const Angle_T *p_rotation_angle /**<  [rad] rotation angle of ego vehicle*/,
                                             const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief checks if the path boundaries are shifted out of the zone for
 * the respective path_direction.
 * If x-coordinate exceeds a threshold for lateral paths, then the path shall be resetted
 * The same adapts to longitudinal paths with the y-coordinate.
 *
 * @return True if boundaries of path exceed limits
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7513}
 * @verification{}
 */
static boolean_T Pt_Do_Path_Boundary_Exceed_Limits(const Pt_Path_T *p_path /**<  respective path*/,
                                                   const Pt_Core_Calibration_T *p_cals /**<  calibration parameters*/);

/**
 * @brief is a wrapper for rotation and shifting of first and last_mat.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7519}
 * @verification{}
 */
static void Pt_Rotate_Path_Boundary_Points(Pt_Path_T *p_path /**< respective path*/,
                                           const Angle_T *p_rotation_angle /**<  [rad] rotation angle of ego vehicle*/,
                                           const Vector_2d_T *p_delta_ego_veh_posn /**<  ego shifting vector*/,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief This superordinate function consists of mainly to steps:
 *	1.) Rotate the path points and the grid array.
 *	2.) Adapt those points onto the local grid array of the ego vehicle within the
 *		current scanindex.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7520}
 * @verification{}
 */
static void Pt_Rotate_Path_Points(Pt_Path_T *p_path /**< respective path*/,
                                  const Angle_T *p_rotation_angle /**<  [rad] rotation angle of ego vehicle*/,
                                  const Vector_2d_T *p_delta_ego_veh_posn /**<  ego shifting vector*/,
                                  const Pt_Input_T *p_pt_input /**<Path tracking input*/,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data*/);

/**
 * @brief adapts the rotated path points onto the local
 * grid array of the ego vehicle. Also the extension or shrinking of a path which
 * may be caused by rotation or translation is considered by function calls of the respectives.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7508}
 * @verification{}
 */
static void Pt_Adapt_Rotated_Paths_Points_To_Grid(
   Pt_Path_T *p_path /**< respective path*/,
   Pt_Temporary_Path_T *p_temp_path_to_transform /**< temporary path which gets adapted onto the original grid array*/,
   Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**<  temporary borders of path points which need to be written back*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief  rotates and shifts a given vector. In case of pathTracking algorithm a case
 * distinction is not needed here, since borders of paths are independent of direction at this point.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7523}
 * @verification{}
 */
static void Pt_Transform_Path_Point_To_New_Vcs_Pos(Vector_2d_T *p_vector_to_transform /**<vector to shift and rotate*/,
                                                   const Vector_2d_T *p_ego_vehicle_shift_vector /**<  shifting vector of ego*/,
                                                   const Angle_T *p_rotation_angle /**<  rotation of ego*/,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**<vehicle_data*/);

/**
 * @brief rotates and shifts the path points and grid coordinates within the temp struct.
 * Also a case discrimination for different path directions is applied here:
 * If the path is longitudinal, then the x-coord constitutes the grid and for lateral
 * path the y-coord represents the grid. For paths whose directions are not defined yet
 * (since 2 pts are needed for that) an approximation check for direction is applied.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7524}
 * @verification{}
 */
static void Pt_Transform_Path_Points_And_Grid_Idx(
   Pt_Temporary_Path_T *p_temp_path /**<temporary path with grid and value component*/,
   const Pt_Discr_Borders_Of_Path_T
      *p_discr_borders_of_path /**< borders in which the original path values and grid components are considered*/,
   const Pt_Path_T *p_path /**< respective path*/,
   const Vector_2d_T *p_ego_vehicle_shift_vector /**< ego shifting vector*/,
   const Angle_T *p_rotation_angle /**< [rad] rotation angle of ego*/,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data*/);

/**
 * @brief initializes the temporary path struct with default values out of the path borders and with
 * the original path values within specified borders.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7518}
 * @verification{}
 */
static void Pt_Init_Temp_Path_Struct(
   Pt_Temporary_Path_T *p_temp_path /**< temporary path struct*/,
   const Pt_Discr_Borders_Of_Path_T
      *p_discr_borders_of_path /**< borders of path which may differ due to creation of points in same scanindex*/,
   const Pt_Path_T *p_path /**< path to rotate*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief presents the wrapper needed for extrapolation. This is done within the adaption function where different points are used
 * to extrapolate the rotated path point values to the original grid.
 *
 * @return return extrapolated path point of the original grid with float32_T type
 *
 * @SRS{SF-1576}
 * @SAE{SF-2918}
 * @SDD{SF-7509}
 * @verification{}
 */
static float32_T Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
   const float32_T path_pt_previous_index /**<path point of previous index*/,
   const float32_T path_pt_current_index /**<path point of current index*/,
   const float32_T grid_pt_previous_index /**<grid point of previous index*/,
   const float32_T grid_pt_current_index /**<grid point of current index*/,
   const float32_T grid_pt_to_extrapolate_to /**<index to which the extrapolation shall be executed*/);

/**
 * @brief Sets the temporary path borders where rotation shall occure. This can be lower when the original path borders e.g.
 * when the path is in its creation phase and points are added on the grid.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7521}
 * @verification{Verify that in case the path has extended in its creation phase, that those points are excluded from the discrete
 * borders.}
 */
static void
Pt_Set_Temporary_Path_Borders(Pt_Discr_Borders_Of_Path_T *p_discr_borders /**< Discrete path borders where rotation shall occure*/,
                              const Pt_Path_T *p_path /**< Path information*/);

/**
 * @brief Adds temporary path points to the temporary path structure.
 *
 * @return void
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7522}
 * @verification{Verify that temporary path points are set correctly.}
 */
static void Pt_Set_Temporary_Path_Points(Pt_Temporary_Path_T *p_temp_path_to_transform /**< Temporary path for adaption*/,
                                         Pt_Path_T *p_path /**< path information*/);

/**
 * @brief Checks whether the path has expanded its range with respect to extension on the grid.
 *
 * @return void
 *
 * @SRS{SF-1576}
 * @SAE{SF-2918}
 * @SDD{SF-7510}
 * @verification{Verify that in case the vehicles grid border has surpassed the next grid point of the grid array, that the
 * respective flag is set to True.}
 */
static void
Pt_Check_Whether_Path_Has_Extended(Pt_Path_Size_Change_Flags_T *p_size_change_flags /**< boolean flags for path size changes*/,
                                   const Pt_Path_T *p_path /**< path information*/,
                                   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief Checks whether the path has reduced its size with respect to shrinkage on the grid.
 *
 * @return void
 *
 * @SRS{SF-1574}
 * @SAE{SF-2918}
 * @SDD{SF-7511}
 * @verification{Verify that in case the vehicles grid border has surpassed the next grid point of the grid array to the inner
 * direction of the path itself, that the respective flag is set to True.}
 */
static void
Pt_Check_Whether_Path_Has_Shrunk(Pt_Path_Size_Change_Flags_T *p_size_change_flags /**< boolean flags for path size changes*/,
                                 const Pt_Path_T *p_path /**< path information*/,
                                 const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief Initializes path size change flag structure.
 *
 * @return void
 *
 * @SRS{SF-1574}
 * @SAE{SF-2918}
 * @SDD{SF-7517}
 * @verification{Verify that the constructor initializes path size change flags correctly.}
 */
static void
Pt_Pt_Init_Path_Size_Change_Flags(Pt_Path_Size_Change_Flags_T *p_size_change_flags /**< boolean flags for path size changes*/);

/**
 * @brief Checks whether interpolation shall occure inbetween two rotated path points.
 *
 * @return true in case that path points shall be used as source of interpolation
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{}
 * @SDD{SF-7614}
 * @verification{Verify that true is returned in case that path points shall be used for .}
 */
static boolean_T Pt_Shall_Points_Be_Used_For_Adaption(
   const Pt_Temporary_Path_T *p_temp_path_to_transform /**<rotated path points and grid*/,
   const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**< index borders of rotated entities*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**<grid array*/,
   const uint8_t grid_index /**< grid index*/);

/**
 * @brief Checks whether interpolation shall occure inbetween the upper target path border and the last path point.
 *
 * @return true in case that upper target border shall be used for interpolation
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{}
 * @SDD{SF-7610}
 * @verification{Verify that true is returned in case that the last path point lies to the left and the target border to the right
 * of the last grid point of a path.}
 */
static boolean_T Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol(
   const Pt_Temporary_Path_T *p_temp_path_to_transform /**<rotated path points and grid*/,
   const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**<discrete path borders*/,
   const Pt_Obj_Borders_Of_Path_T *p_path_borders /**<target path borders*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid_array*/,
   const uint8_t grid_index /**< grid index*/);

/**
 * @brief Checks whether interpolation shall occure inbetween the lower target path border and the first path point.
 *
 * @return true in case that lower target border shall be used for interpolation
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{}
 * @SDD{SF-7611}
 * @verification{Verify that true is returned in case that the first path point lies to the right and the target border to the left
 * of the first grid point of a path.}
 */
static boolean_T Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol(
   const Pt_Temporary_Path_T *p_temp_path_to_transform /**<rotated path points and grid*/,
   const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**<discrete path borders*/,
   const Pt_Obj_Borders_Of_Path_T *p_path_borders /**<target path borders*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid_array*/,
   const uint8_t grid_index /**< grid index*/);

/**
 * @brief Returns the lower surrounding path point for grid interpolation.
 *
 * @return index of the lower boundary
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{}
 * @SDD{SF-7612}
 * @verification{Verify that the correct lower boundary is returned in case of exclusive path point interpolation.}
 */
static uint8_t
Pt_Get_Lower_Surrounding_Point(const Pt_Temporary_Path_T *p_temp_path_to_transform /**<rotated path points and grid*/,
                               const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**< index borders of rotated entities*/,
                               const float32_T grid_array[PT_NUM_GRID_POINTS] /**<grid array*/,
                               const uint8_t grid_index /**< grid index*/);

/**
 * @brief Returns the upper surrounding path point for grid interpolation.
 *
 * @return index of the upper boundary
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{}
 * @SDD{SF-7613}
 * @verification{Verify that the correct upper boundary is returned in case of exclusive path point interpolation.}
 */
static uint8_t
Pt_Get_Upper_Surrounding_Point(const Pt_Temporary_Path_T *p_temp_path_to_transform /**<rotated path points and grid*/,
                               const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path /**< index borders of rotated entities*/,
                               const float32_T grid_array[PT_NUM_GRID_POINTS] /**<grid array*/,
                               const uint8_t grid_index /**< grid index*/);

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/

void Pt_Rotate_Recorded_Paths(Pt_Persistent_T *p_pt_persistent,
                              const Pt_Core_Calibration_T *p_cals,
                              const Pt_Input_T *p_pt_input,
                              const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t path_idx;
   Angle_T rotation_angle;
   Vector_2d_T delta_ego_veh_posn;
   const Pa_Data_T *p_pa_data;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_vehicle_data);

   p_pa_data = p_pt_input->p_fbk_output->p_pa_data;
   assert(NULL != p_pa_data);

   /*Determine rotation and translation respective to the last cycle.*/
   rotation_angle = Create_Angle(p_vehicle_data->yawrate * p_pa_data->time_diff_to_last_cycle);
   Pt_Determine_Ego_Shifting_Vector(&delta_ego_veh_posn, p_pa_data, p_cals, &rotation_angle, p_vehicle_data);

   for (path_idx = 0u; path_idx < PT_NUMBER_OF_PATHS; path_idx++)
   {
      /*Only rotate recorded paths*/
      if (PATH_STATUS_DEFAULT != p_pt_persistent->paths[path_idx].path_state)
      {
         /*Rotate path boundaries which are not limited to the grid*/
         Pt_Rotate_Path_Boundary_Points(&p_pt_persistent->paths[path_idx], &rotation_angle, &delta_ego_veh_posn, p_vehicle_data);

         /*Check whether the boundaries are rotated out of interesting limits of path tracking.*/
         if (Pt_Do_Path_Boundary_Exceed_Limits(&p_pt_persistent->paths[path_idx], p_cals))
         {
            Pt_Reset_Path_And_Associations_To_It(&p_pt_persistent->paths[path_idx], p_pt_persistent->best_path_obj_pairs,
                                                 PATH_RESET_ROTATE_PATHS_BOUNDARY_EXCEED_LIMITS);
         }
         else
         {
            /*Rotate all path points and adapt them onto the original grid.*/
            Pt_Rotate_Path_Points(&p_pt_persistent->paths[path_idx], &rotation_angle, &delta_ego_veh_posn, p_pt_input, p_vehicle_data);

            /*Check whether paths which are not currently in building process whether they are still valid.*/
            if ((PATH_DIRECTION_NONE != p_pt_persistent->paths[path_idx].direction)
                && (FBK_ZERO_UINT == p_pt_persistent->paths[path_idx].obj_curr_used_for_path_build.id))
            {
               /*Check whether paths are still big enough due to possible shrinking in rotation process.*/
               if (Pt_Get_Number_Of_Path_Points(&(p_pt_persistent->paths[path_idx]))
                   > p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot)
               {
                  Pt_Extrapolate_Path(&p_pt_persistent->paths[path_idx], p_cals, p_pt_input->grid_pt_array);
               }
               else
               {
                  Pt_Reset_Path_And_Associations_To_It(&p_pt_persistent->paths[path_idx], p_pt_persistent->best_path_obj_pairs,
                                                       PATH_RESET_ROTATE_PATHS_TOO_SHORT_AFTER_ROTATION);
               }
            }
         }
      }
   }
}


/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static boolean_T Pt_Do_Path_Boundary_Exceed_Limits(const Pt_Path_T *p_path, const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_path_boundary_exceed_limits;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);

   f_path_boundary_exceed_limits = (boolean_T) (((Pt_Is_Path_Lateral(p_path))
                                                 && ((Fbk_Abs_F(p_path->first.x) > p_cals->k_pt_move_max_value)
                                                     || (Fbk_Abs_F(p_path->last_mat.x) > p_cals->k_pt_move_max_value)))
                                                || ((Pt_Is_Path_Longitudinal(p_path))
                                                    && ((Fbk_Abs_F(p_path->first.y) > p_cals->k_pt_move_max_value)
                                                        || (Fbk_Abs_F(p_path->last_mat.y) > p_cals->k_pt_move_max_value))));

   return f_path_boundary_exceed_limits;
}

static void Pt_Determine_Ego_Shifting_Vector(Vector_2d_T *p_delta_ego_veh_posn,
                                             const Pa_Data_T *p_pa_data,
                                             const Pt_Core_Calibration_T *p_cals,
                                             const Angle_T *p_rotation_angle,
                                             const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_delta_ego_veh_posn);
   assert(NULL != p_pa_data);
   assert(NULL != p_cals);
   assert(NULL != p_rotation_angle);
   assert(NULL != p_vehicle_data);

   if (Fbk_Abs_F(p_rotation_angle->angle) > p_cals->k_pt_move_point_yaw_rate_thres_calc_ego_shift)
   {
      float32_T radius;
      radius                  = p_vehicle_data->host_speed / p_vehicle_data->yawrate;
      p_delta_ego_veh_posn->x = radius * p_rotation_angle->sin;
      p_delta_ego_veh_posn->y = radius * (FBK_ONE_F - p_rotation_angle->cos);
   }
   else
   {
      p_delta_ego_veh_posn->x = p_vehicle_data->host_speed * p_pa_data->time_diff_to_last_cycle;
      p_delta_ego_veh_posn->y = Fbk_Half(p_vehicle_data->host_speed * p_vehicle_data->yawrate * p_pa_data->time_diff_to_last_cycle
                                         * p_pa_data->time_diff_to_last_cycle);
   }
}

static void Pt_Rotate_Path_Boundary_Points(Pt_Path_T *p_path,
                                           const Angle_T *p_rotation_angle,
                                           const Vector_2d_T *p_delta_ego_veh_posn,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_path);

   if (PATH_BOTH_BORDERS_NEW != p_path->path_border_status)
   {
      if (PATH_BORDER_NEW_FIRST != p_path->path_border_status)
      {
         /*In case that the lower boundary is not updated in the current cycle (e.g. via path building process) it needs to be
          * rotated.*/
         Pt_Transform_Path_Point_To_New_Vcs_Pos(&p_path->first, p_delta_ego_veh_posn, p_rotation_angle, p_vehicle_data);
      }
      if (PATH_BORDER_NEW_LAST != p_path->path_border_status)
      {
         /*In case that the upper boundary is not updated in the current cycle (e.g. via path building process) it needs to be
          * rotated.*/
         Pt_Transform_Path_Point_To_New_Vcs_Pos(&p_path->last_mat, p_delta_ego_veh_posn, p_rotation_angle, p_vehicle_data);
      }
   }
   else
   {
      /*Both path boundaries were created in the current cycle. Thus the rotation is already applied to those points and no
       * additional rotation is needed.*/
      p_path->path_border_status = PATH_BORDER_POINTS_MATURE;
   }
}

static void Pt_Adapt_Rotated_Paths_Points_To_Grid(Pt_Path_T *p_path,
                                                  Pt_Temporary_Path_T *p_temp_path_to_transform,
                                                  Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                                  const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t i;
   uint8_t lower_idx_border;
   uint8_t upper_idx_border;
   Pt_Obj_Borders_Of_Path_T path_borders;
   path_borders.first_obj_pos_grid_component = FBK_ZERO_F;
   path_borders.first_obj_pos_val_component  = FBK_ZERO_F;
   path_borders.last_obj_pos_grid_component  = FBK_ZERO_F;
   path_borders.last_obj_pos_val_component   = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);

   /*Get relevant path border position with respect to path direction*/
   Pt_Get_Relev_Pos_For_Border_Adaption(p_path, &(path_borders));

   /*Check if path has shrunked*/
   Pt_Adapt_Path_For_Shrinkage(p_path, p_temp_path_to_transform, &(path_borders), p_discr_borders_of_path, grid_array);

   if ((PT_DEFAULT_DISCR_BORDER != p_path->first_p) && (PT_DEFAULT_DISCR_BORDER != p_path->last_p))
   {
      /*Check for extension of path due to rotation*/
      Pt_Adapt_Path_For_Extension(p_path, p_temp_path_to_transform, &path_borders, p_discr_borders_of_path, grid_array);

      /*Search for surrounding rotated points for a specific grid component. Ensure that only interpolation is applied here instead
       * of extrapolation.*/
      for (i = p_discr_borders_of_path->temp_discr_first_boundary; i <= p_discr_borders_of_path->temp_discr_last_boundary; i++)
      {
         /*Interpolate inbetween path points*/
         if (Pt_Shall_Points_Be_Used_For_Adaption(p_temp_path_to_transform, p_discr_borders_of_path, grid_array, i))
         {
            lower_idx_border = Pt_Get_Lower_Surrounding_Point(p_temp_path_to_transform, p_discr_borders_of_path, grid_array, i);
            upper_idx_border = Pt_Get_Upper_Surrounding_Point(p_temp_path_to_transform, p_discr_borders_of_path, grid_array, i);

            p_path->path_points[i] = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
               p_temp_path_to_transform->path_point_array_transform[lower_idx_border],
               p_temp_path_to_transform->path_point_array_transform[upper_idx_border],
               p_temp_path_to_transform->grid_point_table_transform[lower_idx_border],
               p_temp_path_to_transform->grid_point_table_transform[upper_idx_border], grid_array[i]);
         }
         else if (Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol(p_temp_path_to_transform, p_discr_borders_of_path, &path_borders,
                                                                 grid_array, i))
         {
            /*Interpolate between first object border and first point*/
            p_path->path_points[p_discr_borders_of_path->temp_discr_first_boundary] =
               Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
                  path_borders.first_obj_pos_val_component,
                  p_temp_path_to_transform->path_point_array_transform[p_discr_borders_of_path->temp_discr_first_boundary],
                  path_borders.first_obj_pos_grid_component,
                  p_temp_path_to_transform->grid_point_table_transform[p_discr_borders_of_path->temp_discr_first_boundary],
                  grid_array[p_discr_borders_of_path->temp_discr_first_boundary]);
         }
         else if (Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol(p_temp_path_to_transform, p_discr_borders_of_path, &path_borders,
                                                                 grid_array, i))
         {
            /*Interpolate between last object border and last point*/
            p_path->path_points[p_discr_borders_of_path->temp_discr_last_boundary] = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
               p_temp_path_to_transform->path_point_array_transform[p_discr_borders_of_path->temp_discr_last_boundary],
               path_borders.last_obj_pos_val_component,
               p_temp_path_to_transform->grid_point_table_transform[p_discr_borders_of_path->temp_discr_last_boundary],
               path_borders.last_obj_pos_grid_component, grid_array[p_discr_borders_of_path->temp_discr_last_boundary]);
         }
         else
         {
            /*Do nothing */
         }
      }
   }
}

static uint8_t Pt_Get_Lower_Surrounding_Point(const Pt_Temporary_Path_T *p_temp_path_to_transform,
                                              const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                              const float32_T grid_array[PT_NUM_GRID_POINTS],
                                              const uint8_t grid_index)
{
   uint8_t idx_to_return = PT_LOWEST_GRID_POINT_INDEX;
   uint8_t iter;

   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_index);

   for (iter = p_discr_borders_of_path->temp_discr_first_boundary; iter <= p_discr_borders_of_path->temp_discr_last_boundary; iter++)
   {
      if (p_temp_path_to_transform->grid_point_table_transform[iter] < grid_array[grid_index])
      {
         idx_to_return = iter;
      }
      else
      {
         /*idx to return has been found*/
         break;
      }
   }

   /* Result assert */
   assert(idx_to_return < PT_NUM_GRID_POINTS);

   return idx_to_return;
}

static uint8_t Pt_Get_Upper_Surrounding_Point(const Pt_Temporary_Path_T *p_temp_path_to_transform,
                                              const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                              const float32_T grid_array[PT_NUM_GRID_POINTS],
                                              const uint8_t grid_index)
{
   uint8_t idx_to_return = PT_HIGHEST_GRID_POINT_INDEX;
   uint8_t iter;

   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_index);

   for (iter = p_discr_borders_of_path->temp_discr_last_boundary; iter >= p_discr_borders_of_path->temp_discr_first_boundary; iter--)
   {
      if (p_temp_path_to_transform->grid_point_table_transform[iter] > grid_array[grid_index])
      {
         idx_to_return = iter;
      }
      else
      {
         /*idx to return has been found*/
         break;
      }
   }

   /* Result assert */
   assert(idx_to_return < PT_NUM_GRID_POINTS);

   return idx_to_return;
}

static boolean_T Pt_Shall_Points_Be_Used_For_Adaption(const Pt_Temporary_Path_T *p_temp_path_to_transform,
                                                      const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                                      const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                      const uint8_t grid_index)
{
   uint8_t idx;
   boolean_T f_left_side_point_found  = FBK_FALSE;
   boolean_T f_right_side_point_found = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_index);

   for (idx = p_discr_borders_of_path->temp_discr_first_boundary; idx <= p_discr_borders_of_path->temp_discr_last_boundary; idx++)
   {
      if (p_temp_path_to_transform->grid_point_table_transform[idx] < grid_array[grid_index])
      {
         f_left_side_point_found = FBK_TRUE;
      }
      if (p_temp_path_to_transform->grid_point_table_transform[idx] > grid_array[grid_index])
      {
         f_right_side_point_found = FBK_TRUE;
      }
   }

   return (boolean_T) (f_left_side_point_found && f_right_side_point_found);
}

static boolean_T Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol(const Pt_Temporary_Path_T *p_temp_path_to_transform,
                                                                const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                                                const Pt_Obj_Borders_Of_Path_T *p_path_borders,
                                                                const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                                const uint8_t grid_index)
{
   boolean_T f_use_lower_tgt_border = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != p_path_borders);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_index);

   if ((p_path_borders->first_obj_pos_grid_component < grid_array[grid_index])
       && (grid_array[grid_index]
           <= p_temp_path_to_transform->grid_point_table_transform[p_discr_borders_of_path->temp_discr_first_boundary]))
   {
      f_use_lower_tgt_border = FBK_TRUE;
   }

   return f_use_lower_tgt_border;
}

static boolean_T Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol(const Pt_Temporary_Path_T *p_temp_path_to_transform,
                                                                const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                                                const Pt_Obj_Borders_Of_Path_T *p_path_borders,
                                                                const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                                const uint8_t grid_index)
{
   boolean_T f_use_upper_tgt_border = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != p_path_borders);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > grid_index);

   if ((p_path_borders->last_obj_pos_grid_component > grid_array[grid_index])
       && (grid_array[grid_index]
           >= p_temp_path_to_transform->grid_point_table_transform[p_discr_borders_of_path->temp_discr_last_boundary]))
   {
      f_use_upper_tgt_border = FBK_TRUE;
   }

   return f_use_upper_tgt_border;
}

static void Pt_Rotate_Path_Points(Pt_Path_T *p_path,
                                  const Angle_T *p_rotation_angle,
                                  const Vector_2d_T *p_delta_ego_veh_posn,
                                  const Pt_Input_T *p_pt_input,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_pt_input);

   /*Initialize Temporaray Path Borders*/
   Pt_Set_Temporary_Path_Borders(&discr_borders_of_path, p_path);

   /*First check is needed for creation of first point in same scanindex and to prevent rotation of those. Second check is
    * needed for paths, which don't have points yet.*/
   if ((discr_borders_of_path.temp_discr_last_boundary >= discr_borders_of_path.temp_discr_first_boundary)
       && (p_path->first_p != PT_DEFAULT_DISCR_BORDER) && (discr_borders_of_path.temp_discr_last_boundary != PT_DEFAULT_DISCR_BORDER))
   {
      Pt_Temporary_Path_T temp_path_to_transform;

      /*Init arrays with default and also */
      Pt_Init_Temp_Path_Struct(&(temp_path_to_transform), &(discr_borders_of_path), p_path, p_pt_input->grid_pt_array);

      /*Rotate path points on local arrays*/
      Pt_Transform_Path_Points_And_Grid_Idx(&(temp_path_to_transform), &(discr_borders_of_path), p_path, p_delta_ego_veh_posn,
                                            p_rotation_angle, p_vehicle_data);

      /*Add newly added point to temporary path*/
      Pt_Set_Temporary_Path_Points(&temp_path_to_transform, p_path);

      /*Adapt the rotated grid and path point values onto grid array*/
      Pt_Adapt_Rotated_Paths_Points_To_Grid(p_path, &(temp_path_to_transform), &(discr_borders_of_path), p_pt_input->grid_pt_array);
   }
}

static void Pt_Transform_Path_Points_And_Grid_Idx(Pt_Temporary_Path_T *p_temp_path,
                                                  const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                                  const Pt_Path_T *p_path,
                                                  const Vector_2d_T *p_ego_vehicle_shift_vector,
                                                  const Angle_T *p_rotation_angle,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Vector_2d_T vector_to_transform;
   uint8_t path_idx;

   /* Asserts */
   assert(NULL != p_temp_path);
   assert(NULL != p_discr_borders_of_path);

   for (path_idx = p_discr_borders_of_path->temp_discr_first_boundary;
        path_idx <= p_discr_borders_of_path->temp_discr_last_boundary; path_idx++)
   {
      if (Pt_Is_Path_Longitudinal(p_path) || Pt_Has_Path_Longitudinal_Tendencies(p_path))
      {
         vector_to_transform = Create_2d_Vector_Coordinates(p_temp_path->grid_point_table_transform[path_idx],
                                                            p_temp_path->path_point_array_transform[path_idx]);
      }
      else
      {
         vector_to_transform = Create_2d_Vector_Coordinates(p_temp_path->path_point_array_transform[path_idx],
                                                            p_temp_path->grid_point_table_transform[path_idx]);
      }

      Pt_Transform_Path_Point_To_New_Vcs_Pos(&vector_to_transform, p_ego_vehicle_shift_vector, p_rotation_angle, p_vehicle_data);

      if (Pt_Is_Path_Longitudinal(p_path) || Pt_Has_Path_Longitudinal_Tendencies(p_path))
      {
         p_temp_path->grid_point_table_transform[path_idx] = vector_to_transform.x;
         p_temp_path->path_point_array_transform[path_idx] = vector_to_transform.y;
      }
      else
      {
         p_temp_path->grid_point_table_transform[path_idx] = vector_to_transform.y;
         p_temp_path->path_point_array_transform[path_idx] = vector_to_transform.x;
      }
   }
}

static void Pt_Transform_Path_Point_To_New_Vcs_Pos(Vector_2d_T *p_vector_to_transform,
                                                   const Vector_2d_T *p_ego_vehicle_shift_vector,
                                                   const Angle_T *p_rotation_angle,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Vector_2d_T shifted_vector;

   /* Asserts */
   assert(NULL != p_vector_to_transform);
   assert(NULL != p_vehicle_data);

   p_vector_to_transform->x -= p_vehicle_data->rear_axle_position;

   shifted_vector = Vector_2d_Alg_Diff(p_vector_to_transform, p_ego_vehicle_shift_vector);

   Fbk_Swap_Float(&(shifted_vector.x), &(shifted_vector.y));

   *p_vector_to_transform = Vector_2d_Alg_Rotate(p_rotation_angle, &shifted_vector);

   Fbk_Swap_Float(&(p_vector_to_transform->x), &(p_vector_to_transform->y));

   p_vector_to_transform->x += p_vehicle_data->rear_axle_position;
}

static void Pt_Adapt_Path_For_Shrinkage(Pt_Path_T *p_path,
                                        Pt_Temporary_Path_T *p_temp_path_to_transform,
                                        const Pt_Obj_Borders_Of_Path_T *path_borders,
                                        Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                        const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   Pt_Path_Size_Change_Flags_T pt_shrinkage_flags;
   uint8_t idx_for_shrink_proc;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != path_borders);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);

   Pt_Pt_Init_Path_Size_Change_Flags(&pt_shrinkage_flags);
   Pt_Check_Whether_Path_Has_Shrunk(&pt_shrinkage_flags, p_path, grid_array);

   if (Fbk_Is_True(pt_shrinkage_flags.f_path_changed_size_at_lower_border))
   {
      idx_for_shrink_proc = p_path->first_p;
      if (PT_HIGHEST_GRID_POINT_INDEX != idx_for_shrink_proc)
      {
         for (idx_for_shrink_proc = p_path->first_p;
              idx_for_shrink_proc <= (PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET); idx_for_shrink_proc++)
         {
            if (grid_array[idx_for_shrink_proc] >= path_borders->first_obj_pos_grid_component)
            {
               break;
            }
            p_temp_path_to_transform->path_point_array_transform[idx_for_shrink_proc] = PT_PATH_POINTS_DEFAULT_VAL;
            p_path->path_points[idx_for_shrink_proc]                                  = PT_PATH_POINTS_DEFAULT_VAL;
         }
         p_path->first_p                                    = idx_for_shrink_proc;
         p_discr_borders_of_path->temp_discr_first_boundary = idx_for_shrink_proc;
      }
      else
      {
         p_path->last_p                                     = PT_DEFAULT_DISCR_BORDER;
         p_path->first_p                                    = PT_DEFAULT_DISCR_BORDER;
         p_discr_borders_of_path->temp_discr_last_boundary  = PT_DEFAULT_DISCR_BORDER;
         p_discr_borders_of_path->temp_discr_first_boundary = PT_DEFAULT_DISCR_BORDER;
      }
   }

   /*Second condition is only added to make coverity happy. By the overall logic a shrinking process can not happen at both
    * borders for paths consisting of only one point at the outermost path tracking zone.*/
   if (Fbk_Is_True(pt_shrinkage_flags.f_path_changed_size_at_upper_border) && (PT_DEFAULT_DISCR_BORDER != p_path->last_p))
   {
      idx_for_shrink_proc = p_path->last_p;

      if (PT_LOWEST_GRID_POINT_INDEX != idx_for_shrink_proc)
      {
         for (idx_for_shrink_proc = p_path->last_p;
              idx_for_shrink_proc >= (PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET); idx_for_shrink_proc--)
         {
            if (grid_array[idx_for_shrink_proc] < path_borders->last_obj_pos_grid_component)
            {
               break;
            }
            p_temp_path_to_transform->path_point_array_transform[idx_for_shrink_proc] = PT_PATH_POINTS_DEFAULT_VAL;
            p_path->path_points[idx_for_shrink_proc]                                  = PT_PATH_POINTS_DEFAULT_VAL;
         }
         p_path->last_p                                    = idx_for_shrink_proc;
         p_discr_borders_of_path->temp_discr_last_boundary = idx_for_shrink_proc;
      }
      else
      {
         p_path->last_p                                     = PT_DEFAULT_DISCR_BORDER;
         p_path->first_p                                    = PT_DEFAULT_DISCR_BORDER;
         p_discr_borders_of_path->temp_discr_last_boundary  = PT_DEFAULT_DISCR_BORDER;
         p_discr_borders_of_path->temp_discr_first_boundary = PT_DEFAULT_DISCR_BORDER;
      }
   }
}

static void Pt_Adapt_Path_For_Extension(Pt_Path_T *p_path,
                                        Pt_Temporary_Path_T *p_temp_path_to_transform,
                                        const Pt_Obj_Borders_Of_Path_T *path_borders,
                                        Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                        const float32_T grid_array[PT_NUM_GRID_POINTS])
{

   float32_T extrapolated_value;
   uint8_t index_for_extrapolation;
   Pt_Path_Size_Change_Flags_T pt_expansion_flags;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != path_borders);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != grid_array);

   Pt_Pt_Init_Path_Size_Change_Flags(&pt_expansion_flags);
   Pt_Check_Whether_Path_Has_Extended(&pt_expansion_flags, p_path, grid_array);

   /* Check whether expansion of path at lower border has occured*/
   if (Fbk_Is_True(pt_expansion_flags.f_path_changed_size_at_lower_border))
   {
      /* Search for points usable for extension calculation.*/
      /*In case that multiple indices have been expanded, extrapolate those.*/
      for (index_for_extrapolation = Pt_Get_First_Point_Unequal_Zero(p_path);
           index_for_extrapolation >= (PT_SINGLE_GRID_POINT_OFFSET + PT_SINGLE_GRID_POINT_OFFSET - FBK_ONE_UINT);
           index_for_extrapolation--)
      {
         if ((grid_array[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET] < path_borders->first_obj_pos_grid_component))
         {
            break;
         }
         extrapolated_value = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
            path_borders->first_obj_pos_val_component, p_temp_path_to_transform->path_point_array_transform[index_for_extrapolation],
            path_borders->first_obj_pos_grid_component, p_temp_path_to_transform->grid_point_table_transform[index_for_extrapolation],
            grid_array[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET]);

         /*Set the extrapolated point. This does not need to be adapted in this cycle*/
         p_temp_path_to_transform->path_point_array_transform[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET] =
            extrapolated_value;
         p_temp_path_to_transform->grid_point_table_transform[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET] =
            grid_array[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET];
         p_path->path_points[index_for_extrapolation - PT_SINGLE_GRID_POINT_OFFSET] = extrapolated_value;
      }

      p_path->first_p                                    = index_for_extrapolation;
      p_discr_borders_of_path->temp_discr_first_boundary = p_path->first_p;
   }

   /* Check whether expansion of path at upper border has occured*/
   if (Fbk_Is_True(pt_expansion_flags.f_path_changed_size_at_upper_border))
   {
      for (index_for_extrapolation = Pt_Get_Last_Point_Unequal_Zero(p_path);
           index_for_extrapolation
           <= (PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET - PT_SINGLE_GRID_POINT_OFFSET + FBK_ONE_UINT);
           index_for_extrapolation++)
      {
         if (grid_array[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET] > path_borders->last_obj_pos_grid_component)
         {
            break;
         }
         extrapolated_value = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(
            p_temp_path_to_transform->path_point_array_transform[index_for_extrapolation],
            path_borders->last_obj_pos_val_component, p_temp_path_to_transform->grid_point_table_transform[index_for_extrapolation],
            path_borders->last_obj_pos_grid_component, grid_array[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET]);

         /*Set the extrapolated point. This does not need to be adapted in this cycle*/
         p_temp_path_to_transform->path_point_array_transform[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET] =
            extrapolated_value;
         p_temp_path_to_transform->grid_point_table_transform[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET] =
            grid_array[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET];
         p_path->path_points[index_for_extrapolation + PT_SINGLE_GRID_POINT_OFFSET] = extrapolated_value;
      }

      p_path->last_p                                    = index_for_extrapolation;
      p_discr_borders_of_path->temp_discr_last_boundary = p_path->last_p;
   }
}

static void Pt_Get_Relev_Pos_For_Border_Adaption(const Pt_Path_T *p_path, Pt_Obj_Borders_Of_Path_T *p_path_borders)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_path_borders);

   if (Pt_Is_Path_Longitudinal(p_path) || Pt_Has_Path_Longitudinal_Tendencies(p_path))
   {
      if (p_path->first.x > p_path->last_mat.x)
      {
         p_path_borders->first_obj_pos_grid_component = p_path->last_mat.x;
         p_path_borders->first_obj_pos_val_component  = p_path->last_mat.y;
         p_path_borders->last_obj_pos_grid_component  = p_path->first.x;
         p_path_borders->last_obj_pos_val_component   = p_path->first.y;
      }
      else
      {
         p_path_borders->first_obj_pos_grid_component = p_path->first.x;
         p_path_borders->first_obj_pos_val_component  = p_path->first.y;
         p_path_borders->last_obj_pos_grid_component  = p_path->last_mat.x;
         p_path_borders->last_obj_pos_val_component   = p_path->last_mat.y;
      }
   }
   else if (Pt_Is_Path_Lateral(p_path) || Pt_Has_Path_Lateral_Tendencies(p_path))
   {
      if (p_path->first.y > p_path->last_mat.y)
      {
         p_path_borders->first_obj_pos_grid_component = p_path->last_mat.y;
         p_path_borders->first_obj_pos_val_component  = p_path->last_mat.x;
         p_path_borders->last_obj_pos_grid_component  = p_path->first.y;
         p_path_borders->last_obj_pos_val_component   = p_path->first.x;
      }
      else
      {
         p_path_borders->first_obj_pos_grid_component = p_path->first.y;
         p_path_borders->first_obj_pos_val_component  = p_path->first.x;
         p_path_borders->last_obj_pos_grid_component  = p_path->last_mat.y;
         p_path_borders->last_obj_pos_val_component   = p_path->last_mat.x;
      }
   }
   else
   {
      /*Do nothing*/
   }
}

static uint8_t Pt_Get_First_Point_Unequal_Zero(const Pt_Path_T *p_path)
{
   uint8_t i, first_index_unequal_zero;

   /* Assert */
   assert(NULL != p_path);

   first_index_unequal_zero = p_path->first_p;
   if (Fbk_Abs_F(p_path->path_points[p_path->first_p]) < THRESHOLD_IS_ZERO)
   {
      /*Iterat*/
      for (i = p_path->first_p; i < p_path->last_p; i++)
      {
         if (Fbk_Abs_F(p_path->path_points[i]) > THRESHOLD_IS_ZERO)
         {
            first_index_unequal_zero = i;
            break;
         }
      }
   }
   return first_index_unequal_zero;
}

static uint8_t Pt_Get_Last_Point_Unequal_Zero(const Pt_Path_T *p_path)
{
   uint8_t i, first_index_unequal_zero;

   /* Assert */
   assert(NULL != p_path);

   first_index_unequal_zero = p_path->last_p;
   if (Fbk_Abs_F(p_path->path_points[p_path->last_p]) < THRESHOLD_IS_ZERO)
   {
      for (i = p_path->last_p; i > p_path->first_p; i--)
      {
         if (Fbk_Abs_F(p_path->path_points[i]) > THRESHOLD_IS_ZERO)
         {
            first_index_unequal_zero = i;
            break;
         }
      }
   }
   return first_index_unequal_zero;
}

static void Pt_Init_Temp_Path_Struct(Pt_Temporary_Path_T *p_temp_path,
                                     const Pt_Discr_Borders_Of_Path_T *p_discr_borders_of_path,
                                     const Pt_Path_T *p_path,
                                     const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t path_index;

   /* Asserts */
   assert(NULL != p_temp_path);
   assert(NULL != p_discr_borders_of_path);
   assert(NULL != p_path);
   assert(NULL != grid_array);

   for (path_index = PT_LOWEST_GRID_POINT_INDEX; path_index <= PT_HIGHEST_GRID_POINT_INDEX; path_index++)
   {
      if ((path_index >= p_discr_borders_of_path->temp_discr_first_boundary)
          && (path_index <= p_discr_borders_of_path->temp_discr_last_boundary))
      {
         p_temp_path->grid_point_table_transform[path_index] = grid_array[path_index];
         p_temp_path->path_point_array_transform[path_index] = p_path->path_points[path_index];
      }
      else
      {
         p_temp_path->grid_point_table_transform[path_index] = PT_PATH_POINTS_DEFAULT_VAL;
         p_temp_path->path_point_array_transform[path_index] = PT_PATH_POINTS_DEFAULT_VAL;
      }
   }
}

static void Pt_Check_Whether_Path_Has_Extended(Pt_Path_Size_Change_Flags_T *p_size_change_flags,
                                               const Pt_Path_T *p_path,
                                               const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   boolean_T f_upper_path_border_extendable;
   boolean_T f_lower_path_border_extendable;

   /* Asserts */
   assert(NULL != p_size_change_flags);
   assert(NULL != p_path);
   assert(NULL != grid_array);

   /*Check whether borders are extendable*/
   f_lower_path_border_extendable =
      (boolean_T) ((p_path->first_p > PT_LOWEST_GRID_POINT_INDEX) && (p_path->first_p <= PT_HIGHEST_GRID_POINT_INDEX));
   f_upper_path_border_extendable = (boolean_T) (p_path->last_p < PT_HIGHEST_GRID_POINT_INDEX);

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      if (Fbk_Is_True(f_lower_path_border_extendable))
      {
         /*Path has extended, when the grid coordinate of vehicle border passes the next grid point in negative vcs direction on
          * the grid array*/
         if (p_path->first.x < grid_array[p_path->first_p - PT_SINGLE_GRID_POINT_OFFSET])
         {
            p_size_change_flags->f_path_changed_size_at_lower_border = FBK_TRUE;
         }
      }
      if (Fbk_Is_True(f_upper_path_border_extendable))
      {
         /*Path has extended, when the grid coordinate of vehicle border passes the next grid point in positive vcs direction on
          * the grid array*/
         if (p_path->last_mat.x > grid_array[p_path->last_p + PT_SINGLE_GRID_POINT_OFFSET])
         {
            p_size_change_flags->f_path_changed_size_at_upper_border = FBK_TRUE;
         }
      }
   }
   else if (Pt_Is_Path_Lateral(p_path))
   {
      if (Fbk_Is_True(f_lower_path_border_extendable))
      {
         if (p_path->first.y < grid_array[p_path->first_p - PT_SINGLE_GRID_POINT_OFFSET])
         {
            p_size_change_flags->f_path_changed_size_at_lower_border = FBK_TRUE;
         }
      }
      if (Fbk_Is_True(f_upper_path_border_extendable))
      {
         if (p_path->last_mat.y > grid_array[p_path->last_p + PT_SINGLE_GRID_POINT_OFFSET])
         {
            p_size_change_flags->f_path_changed_size_at_upper_border = FBK_TRUE;
         }
      }
   }
   else
   {
      /* Don't check extendability for paths which have less than two path points, since they don't have a direction yet. */
   }
   Binary_Pt_Debug_Pass_Path_Size_Change_Flags(p_size_change_flags->f_path_changed_size_at_lower_border,
                                               p_size_change_flags->f_path_changed_size_at_upper_border, p_path->path_index,
                                               FBK_FALSE);
}

static void Pt_Check_Whether_Path_Has_Shrunk(Pt_Path_Size_Change_Flags_T *p_size_change_flags,
                                             const Pt_Path_T *p_path,
                                             const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   /* Asserts */
   assert(NULL != p_size_change_flags);
   assert(NULL != p_path);
   assert(NULL != grid_array);

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      if (p_path->first.x > grid_array[p_path->first_p])
      {
         p_size_change_flags->f_path_changed_size_at_lower_border = FBK_TRUE;
      }
      if (p_path->last_mat.x < grid_array[p_path->last_p])
      {
         p_size_change_flags->f_path_changed_size_at_upper_border = FBK_TRUE;
      }
   }
   else if (Pt_Is_Path_Lateral(p_path))
   {
      if (p_path->first.y > grid_array[p_path->first_p])
      {
         p_size_change_flags->f_path_changed_size_at_lower_border = FBK_TRUE;
      }
      if (p_path->last_mat.y < grid_array[p_path->last_p])
      {
         p_size_change_flags->f_path_changed_size_at_upper_border = FBK_TRUE;
      }
   }
   else
   {
      /* do nothing */
   }
   Binary_Pt_Debug_Pass_Path_Size_Change_Flags(p_size_change_flags->f_path_changed_size_at_lower_border,
                                               p_size_change_flags->f_path_changed_size_at_upper_border, p_path->path_index,
                                               FBK_TRUE);
}


static void Pt_Pt_Init_Path_Size_Change_Flags(Pt_Path_Size_Change_Flags_T *p_size_change_flags)
{
   /* Assert */
   assert(NULL != p_size_change_flags);

   p_size_change_flags->f_path_changed_size_at_lower_border = FBK_FALSE;
   p_size_change_flags->f_path_changed_size_at_upper_border = FBK_FALSE;
}

static float32_T Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(const float32_T path_pt_previous_index,
                                                                    const float32_T path_pt_current_index,
                                                                    const float32_T grid_pt_previous_index,
                                                                    const float32_T grid_pt_current_index,
                                                                    const float32_T grid_pt_to_extrapolate_to)
{
   float32_T extrapolated_value;

   if (Fbk_Abs_F(grid_pt_previous_index - grid_pt_current_index) <= EPSILON)
   {
      extrapolated_value = path_pt_current_index;
   }
   else
   {
      extrapolated_value = Get_Y_Value_From_Line_By_Coordinates(grid_pt_previous_index, path_pt_previous_index, grid_pt_current_index,
                                                                path_pt_current_index, grid_pt_to_extrapolate_to);
   }

   return extrapolated_value;
}

static void Pt_Set_Temporary_Path_Borders(Pt_Discr_Borders_Of_Path_T *p_discr_borders, const Pt_Path_T *p_path)
{
   /* Asserts */
   assert(NULL != p_discr_borders);
   assert(NULL != p_path);

   /*Check if in current scanindex an additional path point is added. If so then this one should not
    * be rotated with the shift vector since this corresponds to the displacement of ego from
    * the previous to the current scanindex*/
   if (p_path->new_path_point_status == PATH_POINT_NEW_FIRST)
   {
      p_discr_borders->temp_discr_first_boundary = (uint8_t) (p_path->first_p + PT_SINGLE_GRID_POINT_OFFSET);
      p_discr_borders->temp_discr_last_boundary  = p_path->last_p;
   }
   else if (p_path->new_path_point_status == PATH_POINT_NEW_LAST)
   {
      p_discr_borders->temp_discr_first_boundary = p_path->first_p;
      if (FBK_ZERO_UINT == p_path->last_p)
      {
         p_discr_borders->temp_discr_last_boundary = PT_DEFAULT_DISCR_BORDER;
      }
      else
      {
         p_discr_borders->temp_discr_last_boundary = (uint8_t) (p_path->last_p - PT_SINGLE_GRID_POINT_OFFSET);
      }
   }
   else
   {
      p_discr_borders->temp_discr_first_boundary = p_path->first_p;
      p_discr_borders->temp_discr_last_boundary  = p_path->last_p;
   }
}

static void Pt_Set_Temporary_Path_Points(Pt_Temporary_Path_T *p_temp_path_to_transform, Pt_Path_T *p_path)
{
   /* Asserts */
   assert(NULL != p_temp_path_to_transform);
   assert(NULL != p_path);

   /*Add the excluded points to the temporary path_point_array since in special cases it might be possible,
    * that those are needed. Prevent situation with a missing point*/
   if (p_path->new_path_point_status == PATH_POINT_NEW_FIRST)
   {
      p_temp_path_to_transform->path_point_array_transform[p_path->first_p] = p_path->path_points[p_path->first_p];
      p_path->new_path_point_status                                         = PATH_POINT_NEW_MATURE;
   }
   if (p_path->new_path_point_status == PATH_POINT_NEW_LAST)
   {
      p_temp_path_to_transform->path_point_array_transform[p_path->last_p] = p_path->path_points[p_path->last_p];
      p_path->new_path_point_status                                        = PATH_POINT_NEW_MATURE;
   }
}
