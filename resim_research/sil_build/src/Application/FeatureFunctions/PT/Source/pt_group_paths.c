/**
 * @file pt_group_paths.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions for the grouping of paths so that redundancy is reduced.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_group_paths.h"
#include "fbk_array_interpolation.h"
#include "fbk_macros.h"
#include "ml_int_range_t.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_output_t.h"
#include "pt_reset.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Constants
\*===========================================================================*/

#define PT_INITIAL_PATH_POINT_OVERLAP_COUNT_CURRENT_PATH (0)
#define PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS (2u)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Summing up the quantities which are responsible for path grouping
 */
typedef struct
{
   float32_T path_point_diff;           /**< Difference of two path points of seperate paths in [m]*/
   float32_T match_value;               /**< Match value indicating the degree of overlapping of two paths*/
   float32_T max_path_point_diff;       /**< Maximum difference across all path points at equal grid indices of path pair.*/
   int16_t index_to_path_overlap_count; /**< Overlap count which is defined by the interval of definition of the path.*/
   int16_t count_path_points_overlap;   /**< Overlaps which are indicating either a low distance between two path points or nested
                                           intervals of two consecutive path point pairs*/
   boolean_T f_intervals_overlapping;   /**< Flag indicating that discrete intervals of two paths are overlapping*/
} Pt_Local_Grouping_Criteria_T;

/**
 * @brief Summing which are describing grouping properties across all paths
 */
typedef struct
{
   float32_T min_match_value; /**<minimum match value indicating which path should be grouped with path_to_check*/
   uint8_t count_path_point_overlaps_across_all_paths; /**<indicates how many paths are providing strong overlaps*/
   uint8_t best_match_overlapping_paths;     /**<index of the path which discrete interval is overlapping with the interval of
                                path_to_check and helds a low match_value*/
   uint8_t best_match_non_overlapping_paths; /**<index of the path which discrete interval is not overlapping with the interval of
                                path_to_check and helds a low match_value*/
} Pt_Global_Grouping_Criteria_T;

/**
 * @brief Enums which is defining the quality of overlaps
 */
typedef enum
{
   POINT_NOT_IN_PATH_PAIR = (0), /**<Grid point index is not contained in path pair*/
   POINT_IN_ONLY_ONE_PATH = (1), /**<Grid point index is contained in only one of both paths*/
   POINT_IN_BOTH_PATHS    = (2)  /**<Grid point index is contained in both paths*/
} Pt_Overlap_Quality_T;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief groups paths which cross multiple established paths
 *
 * @return void
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7385}
 * @verification{}
 */
static void
Pt_Group_Cross_Paths(Pt_Persistent_T *p_pt_persistent /**< persistent data*/,
                     Pt_Path_T *path_to_check /**< path which gets checked against all other paths (and might be reset)*/,
                     const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                     const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief Groups paths that overlap multiple established paths and cause redundancy.
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7386}
 * @verification{}
 */
static void
Pt_Group_Overlap_Paths(Pt_Persistent_T *p_pt_persistent /**< Persistent data of PT*/,
                       Pt_Path_T *path_to_check /**< path which gets checked against all other paths (and might be reset)*/,
                       const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                       const Pt_Input_T *p_pt_input /**< Path tracking input*/);

/**
 * @brief Updates the outer loop criteria in case that submodules conditions are fulfilled.
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7398}
 * @verification{Verify that the grouping properties are updated in case that internal conditions are fulfilled.}
 */
static void Pt_Update_Overlapping_Paths_Criteria(
   Pt_Global_Grouping_Criteria_T *p_outer_loop_properties /**< output loop criteria*/,
   const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit /**< inner loop criteria*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration*/,
   const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals /**< amount of grid points dependent calibrations*/,
   const uint8_t path_idx /**<path index*/);

/**
 * @brief Evaluates overlapping path criteria. This might lead to grouping of two paths.
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7383}
 * @verification{Verify that grouping only occurs, when the criteria is containing information about the best grouping path pair.}
 */
static void Pt_Evaluate_Overlapping_Paths_Criteria(
   Pt_Persistent_T *p_pt_persistent /**<pt persistent data*/,
   Pt_Path_T *path_to_check /**< path which gets checked against all other paths (and might be reset)*/,
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS] /**< internal overlap counter within grouping*/,
   const Pt_Global_Grouping_Criteria_T *p_outer_loop_properties /**< output loop criteria*/,
   const Pt_Core_Calibration_T *p_cals /**< pt calibrations*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief groups paths with appropriate heuristic weights in an weighted arithmetic mean framework:
 * point_extra_g = (w_extra * point_extra_o + w_kill * point_kill_o)/(w_extra + w_kill)
 *	- point_extra_g = grouped path point in extrapolated path
 *	- w_extra		= weight for point of path_to_extrapolate
 *	- w_kill		= weight for point of path_to_kill
 *	- point_extra_o = point of path_to_extrapolate which shall be grouped
 *	- point_kill_o  = point of path_to_kill which shall be grouped
 * Function also updates properties of grouped path
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7387}
 * @verification{}
 */
static void
Pt_Group_Weight_Path(Pt_Path_T *path_to_extrapolate /**< path which gets weighted and extrapolated*/,
                     Pt_Path_T *path_to_kill /**< path which gets reset*/,
                     Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< path-obj pairs*/,
                     const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                     const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/,
                     const Pt_Overlap_Quality_T overlaps[PT_NUM_GRID_POINTS] /**< overlap array for weighting*/);

/**
 * @brief Returns a grouping weight when iterating outside of at least ones path border
 *
 * @return Grouping weight of type float32_T
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7384}
 * @verification{}
 */
static float32_T
Pt_Get_Grouping_Weight_Outside_Path_Borders(const Pt_Path_T *path_to_extrapolate /*path info of path to extrapolate*/,
                                            const Pt_Path_T *path_to_kill /*path info of path to kill*/,
                                            const Pt_Core_Calibration_T *p_cals /*Path tracking calibrations*/,
                                            const uint8_t point_idx /*point index to check*/);

/**
 * @brief Resets a path, which crosses multiple established paths
 *
 * @return void
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7393}
 * @verification{}
 */
static void
Pt_Process_Cross_Path(Pt_Path_T *path_to_kill /**< path which shall be killed*/,
                      Pt_Path_T *path_to_extrapolate /**< path which shall be extrapolated*/,
                      Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< path-obj pairs*/,
                      const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                      const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief Checks whether the given path_point_index is located in the interval between discrete path borders
 * of both paths
 *
 * @return overlaps between the paths and the path point index coded in the way above
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7382}
 * @verification{}
 */
static Pt_Overlap_Quality_T Pt_Determine_If_Index_Is_Contained_In_Paths(
   const Pt_Path_T *path_a /**< first path whose range between first_p and last_p is compared*/,
   const Pt_Path_T *path_b /**< second path whose range between first_p and last_p is compared*/,
   const uint8_t path_point_index /**<index of currently considered point within path point array*/);

/**
 * @brief Checks whether the inputs paths are valid for further cross grouping analysis.
 *
 * @return true in case that both paths are valid for cross grouping
 *
 * @SRS{SF-1536}
 * @SAE{SF-2918}
 * @SDD{SF-7389}
 * @verification{Verify that two paths are only valid when they are describing different paths, are still nested and when they are
 * finished with their respective creation phase.}
 */
static boolean_T Pt_Is_Path_Pair_Valid_For_Cross_Grouping(const Pt_Path_T *p_path_to_check /**<path to check for crossing behavior*/,
                                                          const Pt_Path_T *p_path_candidate /**<other path candidate*/);

/**
 * @brief Checks whether the path pair is currently the best pair for cross grouping.
 *
 * @return true when the input path pair is a better candidate for cross grouping
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7388}
 * @verification{Verify that only true is returned in case that the input pair is a better cross grouping candidate. This will be
 * the case, when this pairs minimum distance is lower than the currently established one.}
 */
static boolean_T Pt_Is_Path_Pair_Better_For_Cross_Grouping(
   const float32_T min_diff_in_comp /**<minimum difference of the current path pair*/,
   const float32_T min_diff /**<minimum difference threshold*/,
   const boolean_T f_valid_path_intervals_nested /**< flag indicating whether intervals are nested*/,
   const boolean_T f_have_one_cross /**< flag indicating whether a crossing path section is alredy existing*/);

/**
 * @brief checks whether the current points of two paths are strongly overlapping.
 *
 * @return True if path points are strongly overlapping
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7381}
 * @verification{}
 */
static boolean_T
Pt_Are_Path_Points_Overlapping(const Pt_Path_T *path_a /**< first path whose interval is compared*/,
                               const Pt_Path_T *path_b /**< second path whose interval is compared*/,
                               const uint8_t path_point_index /**<index of currently considered point within path point array*/,
                               const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit /**< Inner loop criteria for path grouping*/,
                               const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief merges the path borders
 * - depending on paths orientation points in first and last_mat are chosen as start or end
 * - for longitudinal paths: compare x-coordinate
 * - for lateral paths: compare y-coordinate
 *
 * @return void
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7391}
 * @verification{}
 */
static void Pt_Merge_Path_Borders(Pt_Path_T *p_path_to_extrapolate /**< path whose object borders might be adapted*/,
                                  const Pt_Path_T *p_path_to_kill /**< path which provides its object borders*/);

/**
 * @brief merges the path borders with help of extrapolation
 *	- Since the Group_Path function calculates a resulting path in a weighted mean manner,
 *	  it is not sufficient to simply pick one of the coordinates. This would cause non static paths
 *	  when considering the transistion of last path point to last_mat.
 *	- Since those paths are extrapolated when the detection of them is finished, this could cause
 *	  undefined behaviour. Therefore an extrapolation of last_mat needs to be done.
 * In case of longitudinal paths extrapolate to the maximum x-coordinate of last_mat and for first equivalently
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7392}
 * @verification{}
 */
static void Pt_Merge_Path_Borders_With_Extrapol(Pt_Path_T *p_path_to_extrapolate /**< path whose object borders might be adapted*/,
                                                const Pt_Path_T *p_path_to_kill /**< path which provides its object borders*/,
                                                const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/);

/**
 * @brief Kills the less established path.
 * This means that if the difference of num of path points exceed a threshold
 * the less established path based on number of groupings will get reset.
 *
 * @return void
 *
 * @SRS{SF-1577,SF-1578}
 * @SAE{SF-2918}
 * @SDD{SF-7395}
 * @verification{}
 */
static void Pt_Reset_Less_Established_Path(
   Pt_Path_T *path_a /**<  first candidate which may be reset*/,
   Pt_Path_T *path_b /**<  second candidate which may be reset*/,
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< path-object pairs*/,
   const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals /**<  calibration parameters depending on amount of grid points*/);

/**
 * @brief Checks whether the path pair is fulfilling the following conditions:
 *   - Grouping shall only be applied on two different paths
 *   - Paths need to have the same direction
 *   - The targets used for creation of either paths shall already be unassigned
 *
 * @return True when above conditions are true
 *
 * @SRS{SF-1580}
 * @SAE{SF-2918}
 * @SDD{SF-7390}
 * @verification{}
 */
static boolean_T Pt_Is_Path_Pair_Valid_For_Group_Overlapping(
   const Pt_Path_T *path_to_check /**< path which is currently compared with every other path*/,
   const Pt_Path_T *p_path /**< second path for comparison*/);

/**
 * @brief Initializes the local grouping criteria with default values
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7396}
 * @verification{}
 */
static void
Pt_Reset_Local_Grouping_Criteria(Pt_Local_Grouping_Criteria_T *p_inner_loop_crit /**< criteria for the current path pair*/);

/**
 * @brief Initializes the global grouping criteria with default values
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7394}
 * @verification{}
 */
static void Pt_Reset_Global_Grouping_Properties(
   Pt_Global_Grouping_Criteria_T *p_outer_loop_properties /**< criteria for the currently most redundant path pair*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief Checks whether basic conditions for a grouping candidate are fulfilled
 *
 * @return True when basic conditions are fulfilled
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7380}
 * @verification{}
 */
static boolean_T Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(
   const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit /**< criteria for the current path pair*/,
   const Pt_Global_Grouping_Criteria_T *p_outer_loop_properties /**< criteria for the currently most redundant path pair*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief Updates the criteria which are indicating whether two paths shall be overlapped.
 *
 * @return void
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7397}
 * @verification{}
 */
static void Pt_Update_Grouping_Crit(
   Pt_Local_Grouping_Criteria_T *p_inner_loop_crit /**< criteria for the current path pair*/,
   Pt_Global_Grouping_Criteria_T *p_outer_loop_properties /**< criteria for the currently most redundant path pair*/,
   const Pt_Path_T *p_path /**< path information*/,
   const Pt_Path_T *path_to_check /**< path which is currently compared with every other path*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
   const uint8_t point_idx /**< currently considered point index*/,
   const Pt_Overlap_Quality_T point_quality /**< quality of current point index*/);

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/

void Pt_Group_Paths(Pt_Persistent_T *p_pt_persistent,
                    Pt_Path_T *path_to_check,
                    const Pt_Core_Calibration_T *p_cals,
                    const Pt_Input_T *p_pt_input)
{
   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != path_to_check);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);

   if (Fbk_Is_True(Pt_Is_Object_Tracking_This_Path_Already_Unassigned(path_to_check)))
   {
      if ((path_to_check->first_p == path_to_check->last_p) && (path_to_check->direction != PATH_DIRECTION_NONE))
      {
         Pt_Reset_Path_And_Associations_To_It(path_to_check, p_pt_persistent->best_path_obj_pairs, PATH_RESET_FIRST_P_EQ_LAST_P);
      }
      else
      {
         if ((PATH_STATUS_GROUPED_IN_CURRENT_CYCLE != path_to_check->path_state)
             && (PATH_STATUS_HOST_TRAIL != path_to_check->path_state))
         {
            Pt_Group_Cross_Paths(p_pt_persistent, path_to_check, p_cals, p_pt_input->grid_pt_array);

            Pt_Group_Overlap_Paths(p_pt_persistent, path_to_check, p_cals, p_pt_input);
         }
      }
   }
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static void Pt_Group_Cross_Paths(Pt_Persistent_T *p_pt_persistent,
                                 Pt_Path_T *path_to_check,
                                 const Pt_Core_Calibration_T *p_cals,
                                 const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t i;
   uint8_t k;
   uint8_t best_in_between_match = PT_DEFAULT_MATCH_INDEX;
   boolean_T f_have_one_cross    = FBK_FALSE;
   float32_T min_diff;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != path_to_check);
   assert(NULL != p_cals);
   assert(NULL != grid_array);
   assert(path_to_check->path_index < PT_NUMBER_OF_PATHS);

   min_diff = p_cals->k_pt_group_paths_min_interval_dist;

   for (i = 0; i < PT_NUMBER_OF_PATHS; i++)
   {
      if (Pt_Is_Path_Pair_Valid_For_Cross_Grouping(path_to_check, &p_pt_persistent->paths[i]))
      {
         float32_T min_diff_in_comp              = p_cals->k_pt_group_max_match_value;
         boolean_T f_valid_path_intervals_nested = FBK_FALSE;

         for (k = Min(p_pt_persistent->paths[i].last_p, path_to_check->last_p);
              k < Max(p_pt_persistent->paths[i].first_p, path_to_check->first_p); k++)
         {
            float32_T path_point_diff = Fbk_Abs_F(p_pt_persistent->paths[i].path_points[k] - path_to_check->path_points[k]);

            assert(k < PT_NUM_GRID_POINTS);

            if ((k + 1u) < PT_NUM_GRID_POINTS)
            {
               if (Pt_Are_Path_Intervals_Nested(p_pt_persistent->paths[i].path_points[k],
                                                p_pt_persistent->paths[i].path_points[k + PT_SINGLE_GRID_POINT_OFFSET],
                                                path_to_check->path_points[k],
                                                path_to_check->path_points[k + PT_SINGLE_GRID_POINT_OFFSET]))
               {
                  f_valid_path_intervals_nested = FBK_TRUE;
               }
            }

            if (path_point_diff < min_diff_in_comp)
            {
               min_diff_in_comp = path_point_diff;
            }
         }

         if (Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross))
         {
            if (Fbk_Is_True(f_valid_path_intervals_nested))
            {
               f_have_one_cross = FBK_TRUE;
            }
            min_diff              = min_diff_in_comp;
            best_in_between_match = i;
         }
      }
   }

   if (PT_DEFAULT_MATCH_INDEX != best_in_between_match)
   {
      /* kill ith path & extrapolate path_to_check */
      /* subfunction Pt_Process_Cross_Path */
      Pt_Process_Cross_Path(&p_pt_persistent->paths[best_in_between_match], path_to_check, p_pt_persistent->best_path_obj_pairs,
                            p_cals, grid_array);
   }
}

static void Pt_Group_Overlap_Paths(Pt_Persistent_T *p_pt_persistent,
                                   Pt_Path_T *path_to_check,
                                   const Pt_Core_Calibration_T *p_cals,
                                   const Pt_Input_T *p_pt_input)
{
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS] = {{POINT_NOT_IN_PATH_PAIR}};
   uint8_t path_idx, point_idx;

   Pt_Global_Grouping_Criteria_T outer_loop_properties;
   Pt_Local_Grouping_Criteria_T inner_loop_crit;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != path_to_check);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);

   Pt_Reset_Global_Grouping_Properties(&outer_loop_properties, p_cals);

   for (path_idx = 0u; path_idx < PT_NUMBER_OF_PATHS; path_idx++)
   {
      if (Fbk_Is_True(Pt_Is_Path_Pair_Valid_For_Group_Overlapping(path_to_check, &p_pt_persistent->paths[path_idx])))
      {
         Pt_Reset_Local_Grouping_Criteria(&inner_loop_crit);

         inner_loop_crit.f_intervals_overlapping = Pt_Are_Intervals_Overlapping((int32_t) p_pt_persistent->paths[path_idx].first_p,
                                                                                (int32_t) p_pt_persistent->paths[path_idx].last_p,
                                                                                (int32_t) path_to_check->first_p,
                                                                                (int32_t) path_to_check->last_p);

         for (point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx <= PT_HIGHEST_GRID_POINT_INDEX; point_idx++)
         {
            overlaps[path_idx][point_idx] =
               Pt_Determine_If_Index_Is_Contained_In_Paths(&p_pt_persistent->paths[path_idx], path_to_check, point_idx);

            if ((p_cals->k_pt_start_of_lane_change_processing <= point_idx) && (p_cals->k_pt_end_of_lane_change_processing > point_idx))
            {
               inner_loop_crit.path_point_diff =
                  Fbk_Abs_F(p_pt_persistent->paths[path_idx].path_points[point_idx] - path_to_check->path_points[point_idx]);
               Pt_Update_Grouping_Crit(&inner_loop_crit, &outer_loop_properties, &p_pt_persistent->paths[path_idx], path_to_check,
                                       p_cals, point_idx, overlaps[path_idx][point_idx]);
            }
         }

         inner_loop_crit.match_value /= (float32_T) (Max(FBK_ONE_F, (float32_T) inner_loop_crit.index_to_path_overlap_count));

         if (inner_loop_crit.count_path_points_overlap > PT_INITIAL_PATH_POINT_OVERLAP_COUNT_CURRENT_PATH)
         {
            if (inner_loop_crit.max_path_point_diff < p_cals->k_pt_group_overlap_paths_min_diff)
            {
               Pt_Update_Overlapping_Paths_Criteria(&outer_loop_properties, &inner_loop_crit, p_cals,
                                                    &p_pt_input->Num_Grid_Pts_Dep_Cals, path_idx);
            } /* end check whether maximum path point difference is in allowed boundaries */
            else
            {
               Pt_Reset_Less_Established_Path(&p_pt_persistent->paths[path_idx], path_to_check,
                                              p_pt_persistent->best_path_obj_pairs, &p_pt_input->Num_Grid_Pts_Dep_Cals);
               if (PATH_STATUS_DEFAULT == path_to_check->path_state)
               {
                  /*When path_to_check has been killed another path shall be used as path for grouping checks*/
                  Pt_Reset_Global_Grouping_Properties(&outer_loop_properties, p_cals);
                  break;
               }
            }
         } /* end check whether a good overlap exists in the current path pair*/
      }    /* end check whether path pair is valid*/
   }       /* end for loop over paths */

   Pt_Evaluate_Overlapping_Paths_Criteria(p_pt_persistent, path_to_check, overlaps, &outer_loop_properties, p_cals,
                                          p_pt_input->grid_pt_array);
}

static void Pt_Evaluate_Overlapping_Paths_Criteria(Pt_Persistent_T *p_pt_persistent,
                                                   Pt_Path_T *path_to_check,
                                                   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS],
                                                   const Pt_Global_Grouping_Criteria_T *p_outer_loop_properties,
                                                   const Pt_Core_Calibration_T *p_cals,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_outer_loop_properties);

   if (p_outer_loop_properties->count_path_point_overlaps_across_all_paths <= PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS)
   {
      uint8_t path_to_group_index;

      if (PT_DEFAULT_MATCH_INDEX != p_outer_loop_properties->best_match_overlapping_paths)
      {
         path_to_group_index = p_outer_loop_properties->best_match_overlapping_paths;
      }
      else
      {
         path_to_group_index = p_outer_loop_properties->best_match_non_overlapping_paths;
      }

      if ((PT_DEFAULT_MATCH_INDEX != path_to_group_index) && (p_pt_persistent->paths[path_to_group_index].path_age > FBK_ZERO_UINT))
      {
         /* Pt_Group_Weight_Path delete path at p_paths[path_idx] and do some grouping at path_to_check*/
         Pt_Group_Weight_Path(path_to_check, &p_pt_persistent->paths[path_to_group_index], p_pt_persistent->best_path_obj_pairs,
                              p_cals, grid_array, overlaps[path_to_group_index]);
      }
   }
   else
   {
      Pt_Reset_Path_And_Associations_To_It(path_to_check, p_pt_persistent->best_path_obj_pairs,
                                           PATH_RESET_GROUP_OVERLAP_REDUNDANT_TO_MULTIPLE_PATHS);
   }
}

static void Pt_Update_Overlapping_Paths_Criteria(Pt_Global_Grouping_Criteria_T *p_outer_loop_properties,
                                                 const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit,
                                                 const Pt_Core_Calibration_T *p_cals,
                                                 const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals,
                                                 const uint8_t path_idx)
{
   /* Asserts */
   assert(NULL != p_outer_loop_properties);
   assert(NULL != p_inner_loop_crit);
   assert(NULL != p_num_grid_pts_dep_cals);
   assert(PT_NUMBER_OF_PATHS > path_idx);

   if (Fbk_Is_True(Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(p_inner_loop_crit, p_outer_loop_properties, p_cals)))
   {
      if ((Fbk_Is_True(p_inner_loop_crit->f_intervals_overlapping))
          && (p_inner_loop_crit->count_path_points_overlap > (int16_t) (p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count)))
      {
         /*Group overlapping paths which are causing redundancy*/
         p_outer_loop_properties->best_match_overlapping_paths = path_idx;
         p_outer_loop_properties->min_match_value              = p_inner_loop_crit->match_value;
      }
      else if ((Fbk_Is_False(p_inner_loop_crit->f_intervals_overlapping))
               && (p_inner_loop_crit->count_path_points_overlap
                   > (int16_t) (p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count_ad)))
      {
         /*Group non-overlapping paths which are causing redundancy*/
         p_outer_loop_properties->best_match_non_overlapping_paths = path_idx;
         p_outer_loop_properties->min_match_value                  = p_inner_loop_crit->match_value;
      }
      else
      {
         /*Do nothing*/
      }
   }
}

static boolean_T Pt_Is_Path_Pair_Valid_For_Cross_Grouping(const Pt_Path_T *p_path_to_check, const Pt_Path_T *p_path_candidate)
{
   boolean_T f_intervals_overlapping;
   boolean_T f_paths_valid_for_cross_grouping = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_path_candidate);
   assert(NULL != p_path_to_check);

   f_intervals_overlapping = Pt_Are_Intervals_Overlapping((int32_t) p_path_candidate->first_p, (int32_t) p_path_candidate->last_p,
                                                          (int32_t) p_path_to_check->first_p, (int32_t) p_path_to_check->last_p);

   if (Fbk_Is_False(f_intervals_overlapping) && Fbk_Is_True(Pt_Is_Object_Tracking_This_Path_Already_Unassigned(p_path_candidate))
       && (p_path_candidate->path_index != p_path_to_check->path_index)
       && (p_path_candidate->direction == p_path_to_check->direction) && (PATH_STATUS_HOST_TRAIL != p_path_candidate->path_state))
   {
      f_paths_valid_for_cross_grouping = FBK_TRUE;
   }

   return f_paths_valid_for_cross_grouping;
}

static boolean_T Pt_Is_Path_Pair_Better_For_Cross_Grouping(const float32_T min_diff_in_comp,
                                                           const float32_T min_diff,
                                                           const boolean_T f_valid_path_intervals_nested,
                                                           const boolean_T f_have_one_cross)
{
   boolean_T f_is_path_a_better_match = FBK_FALSE;

   if ((Fbk_Is_False(f_have_one_cross) && Fbk_Is_True(f_valid_path_intervals_nested))
       || ((min_diff_in_comp < min_diff) && (Fbk_Is_False(f_have_one_cross) || Fbk_Is_True(f_valid_path_intervals_nested))))
   {
      f_is_path_a_better_match = FBK_TRUE;
   }

   return f_is_path_a_better_match;
}

static boolean_T Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit,
                                                                const Pt_Global_Grouping_Criteria_T *p_outer_loop_properties,
                                                                const Pt_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_inner_loop_crit);
   assert(NULL != p_outer_loop_properties);
   assert(NULL != p_cals);

   return (boolean_T) ((p_inner_loop_crit->match_value < p_cals->k_pt_group_dir_max_avg_diff_value)
                       && (p_inner_loop_crit->match_value < p_outer_loop_properties->min_match_value));
}

static void Pt_Group_Weight_Path(Pt_Path_T *path_to_extrapolate,
                                 Pt_Path_T *path_to_kill,
                                 Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                 const Pt_Core_Calibration_T *p_cals,
                                 const float32_T grid_array[PT_NUM_GRID_POINTS],
                                 const Pt_Overlap_Quality_T overlaps[PT_NUM_GRID_POINTS])
{
   uint8_t point_idx;
   uint8_t num_grouping_incremented;

   /* Asserts */
   assert(NULL != overlaps);
   assert(NULL != path_to_extrapolate);
   assert(NULL != path_to_kill);
   assert(NULL != best_path_object_pairs);
   assert(NULL != p_cals);
   assert(NULL != grid_array);

   for (point_idx = 0u; point_idx < PT_NUM_GRID_POINTS; point_idx++)
   {
      float32_T weight_extrapolate = FBK_ONE_F;
      float32_T weight_kill        = FBK_ONE_F;
      float32_T denominator;

      if ((PATH_STATUS_GROUPED == path_to_extrapolate->path_state) && (point_idx >= path_to_extrapolate->first_p)
          && (point_idx <= path_to_extrapolate->last_p))
      {
         /* Weight is set according to number of groupings, when path_to_extrapolate has been grouped previously. */
         weight_extrapolate = ((float32_T) (path_to_extrapolate->num_groupings));
      }

      if ((PATH_STATUS_GROUPED == path_to_kill->path_state) && (point_idx >= path_to_kill->first_p)
          && (point_idx <= path_to_kill->last_p))
      {
         /* Weight is set according to number of groupings, when path_to_kill has been grouped previously. */
         weight_kill = ((float32_T) (path_to_kill->num_groupings));
      }

      if (POINT_IN_ONLY_ONE_PATH == overlaps[point_idx])
      {
         float32_T weighting_factor;

         /*Get weighting factor for points outside the boundaries.*/
         weighting_factor = Pt_Get_Grouping_Weight_Outside_Path_Borders(path_to_extrapolate, path_to_kill, p_cals, point_idx);

         weight_extrapolate *= weighting_factor;
         weight_kill *= (FBK_ONE_F - weighting_factor);
      }

      /* Denominator for weight calculation */
      denominator = weight_extrapolate + weight_kill;

      /* Set weights */
      weight_extrapolate /= denominator;
      weight_kill /= denominator;

      /* Set path points according to weights */
      path_to_extrapolate->path_points[point_idx] =
         (weight_extrapolate * path_to_extrapolate->path_points[point_idx]) + (weight_kill * path_to_kill->path_points[point_idx]);
   }

   /* Set new number of groupings */
   num_grouping_incremented = Max(path_to_extrapolate->num_groupings, path_to_kill->num_groupings); /* Set maximum number of
                                                                                                       groupings */
   Sat_Inc_Uint8(&num_grouping_incremented);                      /* Increment the number of groupings */
   path_to_extrapolate->num_groupings = num_grouping_incremented; /* Set incremented number of groupings */

   /* Set properties of new path grouping */
   path_to_extrapolate->path_age   = Max(path_to_extrapolate->path_age, path_to_kill->path_age);         /* Set maximum age */
   path_to_extrapolate->max_speed  = Fbk_Half(path_to_extrapolate->max_speed + path_to_kill->max_speed); /* Set mean max speed */
   path_to_extrapolate->path_state = PATH_STATUS_GROUPED_IN_CURRENT_CYCLE; /* Set path state to grouped currently */
   path_to_extrapolate->last_p     = Max(path_to_extrapolate->last_p, path_to_kill->last_p);   /* Set last_p */
   path_to_extrapolate->first_p    = Min(path_to_extrapolate->first_p, path_to_kill->first_p); /* Set first_p */

   Pt_Merge_Path_Borders_With_Extrapol(path_to_kill, path_to_extrapolate, grid_array);
   Pt_Reset_Path_And_Associations_To_It(path_to_kill, best_path_object_pairs, PATH_RESET_GROUP_OVERLAP_GROUPED_INTO_OTHER_PATH);
   Pt_Extrapolate_Path(path_to_extrapolate, p_cals, grid_array);
}

static float32_T Pt_Get_Grouping_Weight_Outside_Path_Borders(const Pt_Path_T *path_to_extrapolate,
                                                             const Pt_Path_T *path_to_kill,
                                                             const Pt_Core_Calibration_T *p_cals,
                                                             const uint8_t point_idx)
{
   uint8_t more_points;
   uint8_t idx;
   float32_T weighting_factor;
   float32_T sign = FBK_ONE_F;

   /* Asserts */
   assert(NULL != path_to_extrapolate);
   assert(NULL != path_to_kill);
   assert(NULL != p_cals);
   assert(PT_NUM_GRID_POINTS > point_idx);

   /*Check whether point_idx is out of range */
   if ((point_idx > path_to_extrapolate->last_p) || (point_idx > path_to_kill->last_p))
   {
      if (path_to_extrapolate->last_p >= path_to_kill->last_p)
      {
         more_points = (uint8_t) (path_to_extrapolate->last_p - path_to_kill->last_p);
      }
      else
      {
         more_points = (uint8_t) (path_to_kill->last_p - path_to_extrapolate->last_p);
         sign        = -FBK_ONE_F;
      }
   }
   else
   {
      if (path_to_kill->first_p >= path_to_extrapolate->first_p)
      {
         more_points = (uint8_t) (path_to_kill->first_p - path_to_extrapolate->first_p);
      }
      else
      {
         more_points = (uint8_t) (path_to_extrapolate->first_p - path_to_kill->first_p);
         sign        = -FBK_ONE_F;
      }
   }

   /*Calculate grouping factor for given overlap*/
   more_points = ((uint8_t) (Enforce_Range(
      (float32_T) more_points, (float32_T) p_cals->k_pt_point_diff_grouping_borders_lut[0],
      (float32_T) p_cals->k_pt_point_diff_grouping_borders_lut[PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT])));
   idx         = Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(p_cals->k_pt_point_diff_grouping_borders_lut,
                                                    PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_SIZE_DIM0, more_points);
   assert(idx > FBK_ZERO_UINT);
   weighting_factor = Get_Y_Value_From_Line_By_Coordinates(
      ((float32_T) p_cals->k_pt_point_diff_grouping_borders_lut[idx - FBK_ONE_UINT]),
      ((float32_T) p_cals->k_pt_point_diff_weighting_factor_lut[idx - FBK_ONE_UINT]),
      ((float32_T) p_cals->k_pt_point_diff_grouping_borders_lut[idx]),
      ((float32_T) p_cals->k_pt_point_diff_weighting_factor_lut[idx]), ((float32_T) more_points));

   /*Reassign the sign. This is done so that less information in the LUT needs to be used.*/
   if (sign < FBK_ZERO_F)
   {
      weighting_factor = FBK_ONE_F - weighting_factor;
   }

   return weighting_factor;
}

static void Pt_Process_Cross_Path(Pt_Path_T *path_to_kill,
                                  Pt_Path_T *path_to_extrapolate,
                                  Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                  const Pt_Core_Calibration_T *p_cals,
                                  const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t last_left;
   uint8_t first_right;
   uint8_t idx;

   /* Asserts */
   assert(NULL != path_to_kill);
   assert(NULL != path_to_extrapolate);

   for (idx = 0u; idx < PT_NUM_GRID_POINTS; idx++)
   {
      /*Copy the tracked segment of the path which shall be reset*/
      if (((idx > path_to_extrapolate->last_p) || (idx < path_to_extrapolate->first_p))
          && ((idx >= path_to_kill->first_p) && (idx <= path_to_kill->last_p)))
      {
         path_to_extrapolate->path_points[idx] = path_to_kill->path_points[idx];
      }
   }

   /*Interpolate not real tracked path points in between the two path segments, if any are given. and adapt the absolute borders of
    * the path afterwards.*/
   last_left   = Min(path_to_kill->last_p, path_to_extrapolate->last_p);
   first_right = Max(path_to_kill->first_p, path_to_extrapolate->first_p);
   if (first_right > last_left)
   {
      Pt_Extrapolate_Lateral_Points(path_to_extrapolate, p_cals, grid_array, last_left, first_right,
                                    (uint8_t) (last_left + PT_SINGLE_GRID_POINT_OFFSET), first_right);
   }
   path_to_extrapolate->last_p  = Max(path_to_kill->last_p, path_to_extrapolate->last_p);
   path_to_extrapolate->first_p = Min(path_to_kill->first_p, path_to_extrapolate->first_p);

   /*Merge the path borders, reset the path to kill and then extrapolate the missing points of the remaining paths outside of the
    * new absolute borders.*/
   Pt_Merge_Path_Borders(path_to_extrapolate, path_to_kill);
   Pt_Reset_Path_And_Associations_To_It(path_to_kill, best_path_object_pairs, PATH_RESET_GROUP_CROSS_PATH);
   Pt_Extrapolate_Path(path_to_extrapolate, p_cals, grid_array);
}

static Pt_Overlap_Quality_T Pt_Determine_If_Index_Is_Contained_In_Paths(const Pt_Path_T *path_a,
                                                                        const Pt_Path_T *path_b,
                                                                        const uint8_t path_point_index)
{
   Int_Range_T path_a_edge_indices;
   Int_Range_T path_b_edge_indices;
   Pt_Overlap_Quality_T overlap;
   boolean_T f_path_a_contains_index;
   boolean_T f_path_b_contains_index;

   /* Asserts */
   assert(NULL != path_a);
   assert(NULL != path_b);

   path_a_edge_indices     = Create_Int_Range((int32_t) path_a->first_p, (int32_t) path_a->last_p);
   path_b_edge_indices     = Create_Int_Range((int32_t) path_b->first_p, (int32_t) path_b->last_p);
   f_path_a_contains_index = Is_Int_Contained_In_Int_Range((int32_t) path_point_index, &path_a_edge_indices);
   f_path_b_contains_index = Is_Int_Contained_In_Int_Range((int32_t) path_point_index, &path_b_edge_indices);

   if (Fbk_Is_True(f_path_a_contains_index))
   {
      if (Fbk_Is_True(f_path_b_contains_index))
      {
         overlap = POINT_IN_BOTH_PATHS;
      }
      else
      {
         overlap = POINT_IN_ONLY_ONE_PATH;
      }
   }
   else
   {
      if (Fbk_Is_True(f_path_b_contains_index))
      {
         overlap = POINT_IN_ONLY_ONE_PATH;
      }
      else
      {
         overlap = POINT_NOT_IN_PATH_PAIR;
      }
   }

   return overlap;
}

static boolean_T Pt_Are_Path_Points_Overlapping(const Pt_Path_T *path_a,
                                                const Pt_Path_T *path_b,
                                                const uint8_t path_point_index,
                                                const Pt_Local_Grouping_Criteria_T *p_inner_loop_crit,
                                                const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_valid_path_intervals_nested     = FBK_FALSE;
   boolean_T f_points_are_strongly_overlapping = FBK_FALSE;

   /* Asserts */
   assert(NULL != path_a);
   assert(NULL != path_b);
   assert(NULL != p_inner_loop_crit);
   assert(NULL != p_cals);

   if ((path_point_index + PT_SINGLE_GRID_POINT_OFFSET) <= p_cals->k_pt_end_of_lane_change_processing)
   {
      f_valid_path_intervals_nested = Pt_Are_Path_Intervals_Nested(
         path_a->path_points[path_point_index], path_a->path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET],
         path_b->path_points[path_point_index], path_b->path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET]);
   }

   if ((p_inner_loop_crit->path_point_diff < p_cals->k_pt_group_max_match_value_ad) || (Fbk_Is_True(f_valid_path_intervals_nested)))
   {
      f_points_are_strongly_overlapping = FBK_TRUE;
   }
   return (f_points_are_strongly_overlapping);
}

static void Pt_Merge_Path_Borders(Pt_Path_T *p_path_to_extrapolate, const Pt_Path_T *p_path_to_kill)
{
   /* Asserts */
   assert(NULL != p_path_to_kill);
   assert(NULL != p_path_to_extrapolate);

   if (Fbk_Is_True(Pt_Are_Paths_Longitudinally_Orientated(p_path_to_kill, p_path_to_extrapolate)))
   {
      if (p_path_to_kill->last_mat.x > p_path_to_extrapolate->last_mat.x)
      {
         p_path_to_extrapolate->last_mat = p_path_to_kill->last_mat;
      }

      if (p_path_to_extrapolate->first.x > p_path_to_kill->first.x)
      {
         p_path_to_extrapolate->first = p_path_to_kill->first;
      }
   }
   else
   {
      if (p_path_to_kill->last_mat.y > p_path_to_extrapolate->last_mat.y)
      {
         p_path_to_extrapolate->last_mat = p_path_to_kill->last_mat;
      }

      if (p_path_to_extrapolate->first.y > p_path_to_kill->first.y)
      {
         p_path_to_extrapolate->first = p_path_to_kill->first;
      }
   }
}

static void Pt_Merge_Path_Borders_With_Extrapol(Pt_Path_T *p_path_to_extrapolate,
                                                const Pt_Path_T *p_path_to_kill,
                                                const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   /* Asserts */
   assert(NULL != p_path_to_kill);
   assert(NULL != p_path_to_extrapolate);
   assert(NULL != grid_array);

   if (Fbk_Is_True(Pt_Are_Paths_Longitudinally_Orientated(p_path_to_kill, p_path_to_extrapolate)))
   {
      /*Extrapolate last_mat y-value for long. paths and take the outermost border of both paths for that*/
      if (p_path_to_kill->last_mat.x > p_path_to_extrapolate->last_mat.x)
      {
         p_path_to_extrapolate->last_mat.x = p_path_to_kill->last_mat.x;
      }

      p_path_to_extrapolate->last_mat.y = Get_Y_Value_From_Line_By_Coordinates(
         grid_array[p_path_to_extrapolate->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         p_path_to_extrapolate->path_points[p_path_to_extrapolate->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         grid_array[p_path_to_extrapolate->last_p], p_path_to_extrapolate->path_points[p_path_to_extrapolate->last_p],
         p_path_to_extrapolate->last_mat.x);

      /*Extrapolate first y-value for longitudinal paths and take the outermost border of both paths for that*/
      if (p_path_to_kill->first.x < p_path_to_extrapolate->first.x)
      {
         p_path_to_extrapolate->first.x = p_path_to_kill->first.x;
      }

      p_path_to_extrapolate->first.y = Get_Y_Value_From_Line_By_Coordinates(
         grid_array[p_path_to_extrapolate->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_path_to_extrapolate->path_points[p_path_to_extrapolate->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         grid_array[p_path_to_extrapolate->first_p], p_path_to_extrapolate->path_points[p_path_to_extrapolate->first_p],
         p_path_to_extrapolate->first.x);
   }
   else
   {
      /*Extrapolate last_mat y-value for lateral paths and take the outermost border of both paths for that*/
      if (p_path_to_kill->last_mat.y > p_path_to_extrapolate->last_mat.y)
      {
         p_path_to_extrapolate->last_mat.y = p_path_to_kill->last_mat.y;
      }

      p_path_to_extrapolate->last_mat.x = Get_Y_Value_From_Line_By_Coordinates(
         grid_array[p_path_to_extrapolate->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         p_path_to_extrapolate->path_points[p_path_to_extrapolate->last_p - PT_SINGLE_GRID_POINT_OFFSET],
         grid_array[p_path_to_extrapolate->last_p], p_path_to_extrapolate->path_points[p_path_to_extrapolate->last_p],
         p_path_to_extrapolate->last_mat.y);

      /*Extrapolate first y-value for lat. paths and take the outermost border of both paths for that*/
      if (p_path_to_kill->first.y < p_path_to_extrapolate->first.y)
      {
         p_path_to_extrapolate->first.y = p_path_to_kill->first.y;
      }

      p_path_to_extrapolate->first.x = Get_Y_Value_From_Line_By_Coordinates(
         grid_array[p_path_to_extrapolate->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         p_path_to_extrapolate->path_points[p_path_to_extrapolate->first_p + PT_SINGLE_GRID_POINT_OFFSET],
         grid_array[p_path_to_extrapolate->first_p], p_path_to_extrapolate->path_points[p_path_to_extrapolate->first_p],
         p_path_to_extrapolate->first.y);
   }
}

static void Pt_Reset_Less_Established_Path(Pt_Path_T *path_a,
                                           Pt_Path_T *path_b,
                                           Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                           const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals)
{
   uint8_t num_of_points_path_a = Pt_Get_Number_Of_Path_Points(path_a);
   uint8_t num_of_points_path_b = Pt_Get_Number_Of_Path_Points(path_b);

   /* Asserts */
   assert(NULL != path_a);
   assert(NULL != path_b);
   assert(NULL != best_path_object_pairs);
   assert(NULL != p_num_grid_pts_dep_cals);

   if ((num_of_points_path_a > (p_num_grid_pts_dep_cals->k_pt_min_diff_num_path_points + num_of_points_path_b))
       || (num_of_points_path_b > (p_num_grid_pts_dep_cals->k_pt_min_diff_num_path_points + num_of_points_path_a)))
   {
      if (path_a->num_groupings > path_b->num_groupings)
      {
         Pt_Reset_Path_And_Associations_To_It(path_b, best_path_object_pairs, PATH_RESET_GROUP_OVERLAP_LESS_ESTABLISHED_PATH);
      }
      else if (path_a->num_groupings < path_b->num_groupings)
      {
         Pt_Reset_Path_And_Associations_To_It(path_a, best_path_object_pairs, PATH_RESET_GROUP_OVERLAP_LESS_ESTABLISHED_PATH);
      }
      else
      {
         /* Do nothing */
      }
   }
}

static void Pt_Update_Grouping_Crit(Pt_Local_Grouping_Criteria_T *p_inner_loop_crit,
                                    Pt_Global_Grouping_Criteria_T *p_outer_loop_properties,
                                    const Pt_Path_T *p_path,
                                    const Pt_Path_T *path_to_check,
                                    const Pt_Core_Calibration_T *p_cals,
                                    const uint8_t point_idx,
                                    const Pt_Overlap_Quality_T point_quality)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != path_to_check);
   assert(NULL != p_inner_loop_crit);
   assert(NULL != p_outer_loop_properties);
   assert(NULL != p_cals);

   if (Fbk_Is_True(Pt_Are_Path_Points_Overlapping(p_path, path_to_check, point_idx, p_inner_loop_crit, p_cals)))
   {
      if (PT_INITIAL_PATH_POINT_OVERLAP_COUNT_CURRENT_PATH == p_inner_loop_crit->count_path_points_overlap)
      {
         Sat_Inc_Uint8(&(p_outer_loop_properties->count_path_point_overlaps_across_all_paths));
      }
      Sat_Inc_Int16(&(p_inner_loop_crit->count_path_points_overlap));
   }
   if ((POINT_IN_BOTH_PATHS == point_quality) || (Fbk_Is_False(p_inner_loop_crit->f_intervals_overlapping)))
   {
      Sat_Inc_Int16(&(p_inner_loop_crit->index_to_path_overlap_count));
      if (p_inner_loop_crit->path_point_diff >= p_cals->k_pt_group_dir_max_diff_value)
      {
         p_inner_loop_crit->match_value += p_inner_loop_crit->path_point_diff;
      }
      if (p_inner_loop_crit->path_point_diff > p_inner_loop_crit->max_path_point_diff)
      {
         p_inner_loop_crit->max_path_point_diff = p_inner_loop_crit->path_point_diff;
      }
   }
}

static boolean_T Pt_Is_Path_Pair_Valid_For_Group_Overlapping(const Pt_Path_T *path_to_check, const Pt_Path_T *p_path)
{
   boolean_T f_path_is_valid_for_group_overlapping;
   boolean_T f_unas;
   boolean_T f_index;
   boolean_T f_dir;
   boolean_T f_state;
   boolean_T f_state_host;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != path_to_check);

   f_unas       = (boolean_T) (Fbk_Is_True(Pt_Is_Object_Tracking_This_Path_Already_Unassigned(p_path)));
   f_index      = (boolean_T) (p_path->path_index != path_to_check->path_index);
   f_dir        = (boolean_T) Fbk_Is_True(Pt_Do_Paths_Have_The_Same_Direction(p_path, path_to_check));
   f_state      = (boolean_T) (PATH_STATUS_GROUPED_IN_CURRENT_CYCLE != p_path->path_state);
   f_state_host = (boolean_T) (PATH_STATUS_HOST_TRAIL != p_path->path_state);

   f_path_is_valid_for_group_overlapping = (boolean_T) (f_unas && f_index && f_dir && f_state && f_state_host);

   return f_path_is_valid_for_group_overlapping;
}

static void Pt_Reset_Local_Grouping_Criteria(Pt_Local_Grouping_Criteria_T *p_inner_loop_crit)
{
   /* Assert */
   assert(NULL != p_inner_loop_crit);

   p_inner_loop_crit->path_point_diff             = FBK_ZERO_F;
   p_inner_loop_crit->count_path_points_overlap   = FBK_ZERO_INT;
   p_inner_loop_crit->index_to_path_overlap_count = FBK_ZERO_INT;
   p_inner_loop_crit->match_value                 = FBK_ZERO_F;
   p_inner_loop_crit->max_path_point_diff         = FBK_ZERO_F;
   p_inner_loop_crit->f_intervals_overlapping     = FBK_FALSE;
}

static void Pt_Reset_Global_Grouping_Properties(Pt_Global_Grouping_Criteria_T *p_outer_loop_properties,
                                                const Pt_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_outer_loop_properties);
   assert(NULL != p_cals);

   p_outer_loop_properties->best_match_overlapping_paths               = PT_DEFAULT_MATCH_INDEX;
   p_outer_loop_properties->best_match_non_overlapping_paths           = PT_DEFAULT_MATCH_INDEX;
   p_outer_loop_properties->count_path_point_overlaps_across_all_paths = FBK_ZERO_INT;
   p_outer_loop_properties->min_match_value                            = p_cals->k_pt_overlap_max_match_value;
}
