#ifndef PT_COMMON_FUNCTIONS_H
#define PT_COMMON_FUNCTIONS_H

/**
 * @file pt_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains declarations of functions which are shared across path tracking modules.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ml_float_range_t.h"
#include "ml_int_range_t.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration_t.h"
#include "pt_directions.h"
#include "pt_persistent_t.h"

/*===========================================================================*\
* Global Defines
\*===========================================================================*/

#define PT_HIGH_DISTANCE_DEFAULT_VAL (90.0f)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Pair of int range used for
 */
typedef struct
{
   Int_Range_T range_A; /**< first range component of int32_t range pair */
   Int_Range_T range_B; /**< second range component of int32_t range pair */
} Pt_Pair_Of_Intervals_Int32_T;

/**
 * @brief Pair of float range
 */
typedef struct
{
   Float_Range_T range_A; /**< first range component of float32_T range pair */
   Float_Range_T range_B; /**< second range component of float32_T range pair */
} Pt_Pair_Of_Intervals_Float_T;


/**
 * @brief Summarizes the indices of the next points independent of object direction
 */
typedef struct
{
   uint8_t next_point_idx_lat_path;  /**< Point index independent of the object moving direction for lateral path comparison*/
   uint8_t next_point_idx_long_path; /**< Point index independent of the object moving direction for longitudinal path comparison*/
} Pt_Point_Indices_Dir_Indep_Obj_T;


/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief This function is a wrapper for extrapolation of missing path points in 2 cases
 * - extrapolation of upper end points (everything after last_p)
 * - extrapolation of lower end points (everything before first_p)
 *
 * @return void
 *
 * @SRS{SF-1567}
 * @SAE{SF-2918}
 * @SDD{SF-7338}
 * @verification{}
 */
void Pt_Extrapolate_Path(Pt_Path_T *p_path /**<  respective path*/,
                         const Pt_Core_Calibration_T *p_cals /**<  calibration parameters*/,
                         const float32_T grid_array[PT_NUM_GRID_POINTS] /**<  pt input*/);

/**
 * @brief wrapper for extrapolation of missing path points to the next point after the zero point
 *	-The next point after PT_MID_GRID_POINT_INDEX is also needed for the safe exit function
 *    or calculation of f_path_part_is_linear in Check_Path_Linear function.
 *
 * @return void
 *
 * @SRS{SF-1583}
 * @SAE{SF-2918}
 * @SDD{SF-7339}
 * @verification{}
 */
void Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt(Pt_Path_T *p_path /**<  respective path*/,
                                                  const Pt_Core_Calibration_T *p_cals /**<  calibration parameters*/,
                                                  const float32_T grid_array[PT_NUM_GRID_POINTS] /**<  grid point array*/);

/**
 * @brief returns the num of path points defined by the range between first_p and last_p
 *
 * @return num of path points between discrete path boundauries of type uint8_t
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7341}
 * @verification{}
 */
uint8_t Pt_Get_Number_Of_Path_Points(const Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief Calls the subroutines Pt_Check_If_Intervals_Contain_Default_Path_Points
 * and Is_Float_Interval_Subset_Of_Float_Interval to check if the given intervals
 * contain the default value and if the intervals are nested.
 * If the intervals contain the default value then it is not clear
 * if paths are nested or not. In that case return False.
 *
 * @return True when the intervals are nested
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7331}
 * @verification{}
 */
boolean_T Pt_Are_Path_Intervals_Nested(float32_T interval_a_lower_limit /**< interval A lower limit */,
                                       float32_T interval_a_upper_limit /**< interval A upper limit */,
                                       float32_T interval_b_lower_limit /**< interval B lower limit */,
                                       float32_T interval_b_upper_limit /**< interval B upper limit */);

/**
 * @brief Returns true if the given integer intervals are overlapping.
 * Application for the integer boundaries first and last of path tracking algo, which
 * indicate the first and last discrete grid_point index where object has been used to define path
 *
 * @return True when intervals are overlapping
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7330}
 * @verification{}
 */
boolean_T Pt_Are_Intervals_Overlapping(int32_t interval_a_lower_limit /**< interval A lower limit */,
                                       int32_t interval_a_upper_limit /**< interval A upper limit */,
                                       int32_t interval_b_lower_limit /**< interval B lower limit */,
                                       int32_t interval_b_upper_limit /**< interval B upper limit */);


/**
 * @brief Checks whether the directions of two paths are equal. The direction
 * none is not inclusive here.
 *
 * @return True if both paths have the same direction
 *
 * @SRS{SF-1580}
 * @SAE{SF-2918}
 * @SDD{SF-7336}
 * @verification{}
 */
boolean_T Pt_Do_Paths_Have_The_Same_Direction(const Pt_Path_T *p_path_one /**< first path to compare*/,
                                              const Pt_Path_T *p_path_two /**< second path to compare*/);

/**
 * @brief Calls linear extrapolation defined as y_ext=y_1+(y_2-y_1)/(x_2-x_1)*(x_ext-x_1),
 * with	i.)		y_ext = extrapolated y-value
 *		ii.)	x_ext = x-coordinate where extrapolation shall be applied
 *		iii.)	(x_k,y_k) = k-th data point
 * linear extrapolation is applied to a range of values in k_pt_grid_points_table
 *
 * @return void
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7337}
 * @verification{}
 */
void Pt_Extrapolate_Lateral_Points(Pt_Path_T *p_path /**<  respective path*/,
                                   const Pt_Core_Calibration_T *p_cals /**<  calibration parameters*/,
                                   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/,
                                   const uint8_t extrapol_ref_idx_a /**< reference index for lateral lines taken for extrapolation*/,
                                   const uint8_t extrapol_ref_idx_b /**< reference index for lateral lines taken for extrapolation*/,
                                   const uint8_t for_loop_init_index /**< index for loop initialization*/,
                                   const uint8_t for_loop_exit_index /**< index for loop break*/);

/**
 * @brief Checks if the path is longitudinal orientated.
 *
 * @return True when direction is longitudinal forward or backward
 *
 * @SRS{SF-1567}
 * @SAE{SF-2918}
 * @SDD{SF-7352}
 * @verification{}
 */
boolean_T Pt_Is_Path_Longitudinal(const Pt_Path_T *p_path /**<  respective path*/);

/**
 * @brief Checks if the path has longitudinal tendencies. This is used when no direction is yet given.
 *
 * @return True when no direction is specified yet and when path has a longitudinal tendency
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7344}
 * @verification{Verify that true is only returned in case that no direction is given yet and when the path is oriented more
 * longitudinally than laterally.}
 */
boolean_T Pt_Has_Path_Longitudinal_Tendencies(const Pt_Path_T *p_path /**< input path*/);

/**
 * @brief Checks if the path is longitudinal orientated.
 *
 * @return True when direction is lateral left or right
 *
 * @SRS{SF-1526,SF-1525,SF-1527,SF-1528}
 * @SAE{SF-2918}
 * @SDD{SF-7351}
 * @verification{}
 */
boolean_T Pt_Is_Path_Lateral(const Pt_Path_T *p_path /**<  respective path*/);

/**
 * @brief Checks if the path has lateral tendencies. This is used when no direction is yet given.
 *
 * @return True when no direction is specified yet and when path has a lateral tendency
 *
 * @SRS{SF-1575,SF-1573}
 * @SAE{SF-2918}
 * @SDD{SF-7343}
 * @verification{Verify that true is only returned in case that no direction is given yet and when the path is oriented more
 * laterally than longitudinally.}
 */
boolean_T Pt_Has_Path_Lateral_Tendencies(const Pt_Path_T *p_path /**< input path*/);

/**
 * @brief checks if the path direction is opposed with ego vehicle coordinate system (x:=long, y:=lat).
 *
 * @return True when direction is lateral left or longitudinal backward
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7350}
 * @verification{}
 */
boolean_T Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(const Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief Checks whether object moves longitudinal.
 *
 * @return True when object moves longitudinal
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7348}
 * @verification{}
 */
boolean_T Pt_Is_Obj_Moving_Longitudinal(const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< object moving direction*/);


/**
 * @brief Checks whether object moves lateral.
 *
 * @return True when object moves lateral
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7347}
 * @verification{}
 */
boolean_T Pt_Is_Obj_Moving_Lateral(const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< object moving direction*/);

/**
 * @brief Checks whether object moves lateral right or longitudinal forward.
 *
 * @return True if object moves to positive lateral/long directions
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7346}
 * @verification{}
 */
boolean_T Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< object moving direction*/);

/**
 * @brief Checks whether object moves lateral left or longitudinal backwards.
 *
 * @return True if object moves to positive lateral/long directions
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7345}
 * @verification{}
 */
boolean_T Pt_Is_Obj_Mov_Dir_Against_Vcs(const Pt_Object_Mov_Direction_T *p_obj_move_dir /**< object moving direction*/);

/**
 * @brief Returns the object orientation in dependence of thresholds considering the object heading.
 *
 * @return orientation of the respective object of Pt_Object_Mov_Direction_T type
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7334}
 * @verification{}
 */
Pt_Object_Orientation_T Pt_Determine_Object_Orientation(const float32_T heading /**< heading of the object*/,
                                                        const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief Checks if the path direction and object orientation match.
 *
 * @return True when object and path direction match
 *
 * @SRS{SF-1526,SF-1525,SF-1527,SF-1528}
 * @SAE{SF-2918}
 * @SDD{SF-7335}
 * @verification{}
 */
boolean_T
Pt_Do_Path_Direction_And_Object_Orientation_Match(const Pt_Path_T *p_path /**< path information*/,
                                                  const Pt_Object_Orientation_T *p_obj_orientation /**< direction of the object*/);

/**
 * @brief Returns the number of nonzero path points in the range of the whole grid point array.
 *
 * @return number of path points unequal to the path point default value
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7340}
 * @verification{}
 */
uint8_t Pt_Get_Number_Of_Non_Zero_Path_Points(const Pt_Path_T *p_path /**< respective path*/);


/**
 * @brief Checks if an object is still assigned to the respective path.
 * Returns true if object is unassigned
 *
 * @return True if the object used to build this path is unassigned
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7349}
 * @verification{}
 */
boolean_T Pt_Is_Object_Tracking_This_Path_Already_Unassigned(const Pt_Path_T *p_path /**< respective path*/);


/**
 * @brief Checks the direction attribute of both paths and returns
 * true if and only if both paths are longitudinally orientated (independend of forward and backward)
 *
 * @return True if both paths are longitudinal orientated
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7332}
 * @verification{}
 */
boolean_T Pt_Are_Paths_Longitudinally_Orientated(const Pt_Path_T *p_path_a /**< first path to compare*/,
                                                 const Pt_Path_T *p_path_b /**< second path to compare*/);

/**
 * @brief Calculates path heading near object using a path grid point interval with object in between.
 *
 * @return path heading near object of type float32_T in rad
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7333}
 * @verification{}
 */
float32_T Pt_Calculate_Path_Heading_Near_Object(
   const uint8_t next_point_idx /**<grid point index which is closest to object (left side of grid array)*/,
   const Pt_Path_T *p_path /**< respective path*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);


/**
 * @brief Initializes the point indices for lateral and longitudinal path to object comparison
 *
 * @return returns the next point index dependent on the input path direction
 *
 * @SRS{SF-1564}
 * @SAE{}
 * @SDD{SF-7582}
 * @verification{}
 */
uint8_t Pt_Read_Next_Point_Dep_On_Path_Dir(
   const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx /**<indices of the next point for lateral and longitudinal paths*/,
   const Pt_Path_T *p_path /**< Input path*/);


/**
 * @brief Searches for an empty path slot and returns true in case that an empty slot has been found.
 *
 * @return true when an empty path slot has been found
 *
 * @SRS{SF-1554}
 * @SAE{}
 * @SDD{SF-7630}
 * @verification{}
 */
boolean_T Pt_Search_Empty_Path_Slot(uint8_t *p_path_idx, const Pt_Path_T paths[PT_NUMBER_OF_PATHS]);

#endif
