/**
 * @file pt_object_matching.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions for the object to path matching process within path tracking.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_object_matching.h"
#include "fbk_array_interpolation.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "ml_angle_normalize.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_debug_interface.h"
#include "pt_directions.h"
#include "pt_output_factory.h"
#include "pt_persistent_handler.h"
#include "pt_reset.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Typedefs
\*===========================================================================*/
/**
 * @brief Summarizes the desired path points of the path created by the respective object used for matching.
 */
typedef struct
{
   uint8_t path_point_index_higher_prio; /**< point which was created behind the target most recently*/
   uint8_t path_point_index_lower_prio;  /**< point which is created second last behind the target*/
} Pt_Trail_Path_Comp_Indices_T;


/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief calculates the moving direction of object where forward and backward moving directions are also considered
 *				of Pt_Object_Mov_Direction_T type
 *
 * @return object moving direction of type Pt_Object_Mov_Direction_T
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7455}
 * @verification{}
 */
static Pt_Object_Mov_Direction_T Pt_Determine_Object_Movement_Direction(
   const float32_T heading /**<heading of the object*/, const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief calculates the grid point index which is closest to the object starting from the left side of grid array. This is done
 * for both the lateral grid and the longitudinal grid such that a comparison of an object and paths is independent of their
 * orientation .
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7459}
 * @verification{}
 */
static void Pt_Get_Next_Point_For_Object_To_Pass(Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx,
                                                 const Fbk_Object_Data_T *p_object /**< object data*/,
                                                 const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief In case of the object which is building up the path used within this function is unassigned and
 * thus the path is extrapolated, this function checks whether the object is within a bound around
 * discrete borders.
 *
 * @return True if closest grid index of object is within specified bounds around discrete path border
 *
 * @SRS{SF-1566}
 * @SAE{SF-2918}
 * @SDD{SF-7464}
 * @verification{}
 */
static boolean_T Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(const uint8_t next_point_idx,
                                                     const Pt_Path_T *p_path,
                                                     const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals);

/**
 * @brief In case that an object is still assigned to the current path in order to build it up,
 * another object still might be matched to this path.
 * Here it's checked whether the match is possible.
 *
 * @return True if closest grid index of object is within discrete path borders
 *
 * @SRS{SF-1569}
 * @SAE{SF-2918}
 * @SDD{SF-7463}
 * @verification{}
 */
static boolean_T Pt_Is_Idx_In_Path_Borders(const uint8_t next_point_idx, const Pt_Path_T *p_path);

/**
 * @brief checks wether path can be used for object matching.
 *
 * @return True if path is valid for object matching
 *
 * @SRS{SF-1565}
 * @SAE{SF-2918}
 * @SDD{SF-7472}
 * @verification{}
 */
static boolean_T Pt_Is_Path_Valid_For_Object_Matching(const Pt_Path_T *p_path /**< respective path*/,
                                                      const Fbk_Object_Data_T *p_object /**< object data*/,
                                                      const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief checks wether the input point is included in the relevant path segment.
 *
 * @return True if the point is included in the current path segment
 *
 * @SRS{SF-1565}
 * @SAE{}
 * @SDD{SF-7586}
 * @verification{}
 */
static boolean_T Pt_Is_Next_Point_Contained_In_Valid_Path_Borders(
   const uint8_t next_point_idx /**<index of the next point*/,
   const Pt_Path_T *p_path /**<input path*/,
   const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals /**<grid dependent cals*/);

/**
 * @brief checks if an object is invalid for being matched to a path (true if invalid).
 *
 * @return True if object is valid for being matched to a path
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7469}
 * @verification{}
 */
static boolean_T
Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(const Pt_Object_Mov_Direction_T obj_move_dir /**<moving direction of the object*/,
                                                 const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                                                 const Pt_Input_T *p_pt_input /**< path tracking input*/,
                                                 const Fbk_Object_Data_T *p_object /**< object data*/);

/**
 * @brief checks whether given path object pair is better than the current best one for the object.
 *
 * @return True if path object pair confidences exceed the current best one
 *
 * @SRS{SF-1570}
 * @SAE{SF-2918}
 * @SDD{SF-7471}
 * @verification{}
 */
static boolean_T Pt_Is_Path_Obj_Pair_Better_Match(
   const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence /**< confidence of path object pair */,
   const Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**<Path object pair information*/,
   const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair /**< best path object pair for current object */,
   const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< path information */,
   const Fbk_Object_Data_T *p_object /**<object data*/,
   const Pt_Core_Calibration_T *p_cals /**< pt calibration */,
   const uint8_t path_candidate_idx /**< idx of the path to be examined*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**< moving direction of the object*/);

/**
 * @brief returns confidence hysteresis for the comparison of two paths in matching.
 *
 * @return hysteresis in the range from -0.25 to 0.25.
 *
 * @SRS{SF-1538}
 * @SAE{SF-2918}
 * @SDD{SF-7571}
 * @verification{}
 */
static float32_T Pt_Return_Confidence_Hysteresis_For_Path_Change(
   const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair /**< best path object pair for current object*/,
   const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< information about all paths*/,
   const Pt_Core_Calibration_T *p_cals /**< path tracking calibrations*/,
   const uint8_t path_candidate_idx /**< index of path to compare*/);

/**
 * @brief Check whether the given scenario is valid for nearest path match.
 *
 * @return returns true in case that the paths are finished with their creation and when the given object is valid.
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7572}
 * @verification{}
 */
static boolean_T Pt_Are_Paths_Valid_For_Nearest_Path_Match(
   const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair /**< information about the currently best matching pair*/,
   const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< path information for all paths*/,
   const Fbk_Object_Data_T *p_object /**< object data*/,
   const uint8_t path_candidate_idx /**< index of path to compare*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**< object moving direction*/);

/**
 * @brief Checks whether two paths are similar at host and object level.
 *
 * @return True when two paths are similar at host and object level.
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7447}
 * @verification{}
 */
static boolean_T Pt_Are_Paths_Similar_At_Host_And_Object_Level(
   const Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< info about the path object pair*/,
   const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair /**< currently best path for the given object*/,
   const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< information about each other path*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration values*/,
   const uint8_t path_candidate_idx /**< index of the path examined at the current cycle*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**< moving direction of the object*/,
   const Fbk_Object_Data_T *p_object /**< object data*/);

/**
 * @brief Checks whether the given path shall be used as path for the path object pair in disregard of a high confidence.
 *
 * @return True when a point near the critical axis is nearer the host of the path candidate.
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7470}
 * @verification{}
 */
static boolean_T
Pt_Is_Path_Candidate_Nearer_To_Host(const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< information about all paths */,
                                    const Pt_Object_Mov_Direction_T obj_move_dir /**< object moving direction */,
                                    const uint8_t path_candidate_idx /**< index of current path candidate */,
                                    const uint8_t best_path_object_pair_idx /**< index of current persistent best path candidate*/);

/**
 * @brief Checks whether the mid grid point index is included in both input paths.
 *
 * @return True when the mid grid point index is included in both input paths.
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7465}
 * @verification{Verify that true is only returned when the mid grid points index is included in both input paths.}
 */
static boolean_T Pt_Is_Mid_Point_Included_In_Paths(const Pt_Path_T *p_path_a /**< first path to check*/,
                                                   const Pt_Path_T *p_path_b /**< second path to check*/);

/**
 * @brief Checks whether the given object is valid for the matching via path similarities.
 *
 * @return True when the object has not passed the critical coordinate axis
 *
 * @SRS{}
 * @SAE{SF-2918}
 * @SDD{SF-7468}
 * @verification{}
 */
static boolean_T
Pt_Is_Obj_Valid_For_Path_Similarity_Check(const Pt_Object_Mov_Direction_T obj_move_dir /**< object moving direction*/,
                                          const Fbk_Object_Data_T *p_object /**< object data*/);

/**
 * @brief Checks whether the next point index is valid.
 *
 * @return True when the input point index is within the total boundaries.
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7466}
 * @verification{Verify that a point index is valid in case that it is within the total boundaries}
 */
static boolean_T Pt_Is_Next_Point_Idx_Valid(const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx /**<input point index*/);

/**
 * @brief determines the best matching path for the considered object based on the following factors:
 *	- Proximity between target and path edge
 *	- Proximity between ego and path edge
 *	- Path age
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7457}
 * @verification{}
 */
static void Pt_Find_Best_Matching_Path(
   Pt_Persistent_T *p_pt_persistent /**<Persistent Path Tracking data*/,
   Pt_Path_Obj_Pair_Info_T *p_best_path_obj_pair_info /**< info about the best path match for considered object*/,
   Pt_Path_Obj_Pair_Consumer_Info_T *p_path_consumer_info /**< consumer info about possible path object pairs*/,
   const Pt_Input_T *p_pt_input /**< Input data of Path Tracking*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**<moving direction of the object*/,
   const Pt_Point_Indices_Dir_Indep_Obj_T
      *p_next_point_idx /**<next grid point index for the given object for lateral and longitudinal paths*/,
   const Fbk_Object_Data_T *p_object /**< object data*/);

/**
 * @brief Initializes path obj pair information structure with default values
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7462}
 * @verification{}
 */
static void
Pt_Pt_Init_Path_Obj_Pair_Info(Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info /**< information about the current path-object pair*/);

/**
 * @brief Initializes path obj pair consumer info structure with default values
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7461}
 * @verification{}
 */
static void Pt_Pt_Init_Path_Obj_Pair_Consumer_Info(Pt_Path_Obj_Pair_Consumer_Info_T *p_path_obj_pair_con_info);

/**
 * @brief Initializes the path object pair confidence structure with default values
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7460}
 * @verification{}
 */
static void Pt_Pt_Init_Path_Obj_Pair_Confidence(
   Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence /**< path object pair confidence structure*/);

/**
 * @brief Initializes the point indices for lateral and longitudinal path to object comparison
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{}
 * @SDD{SF-7583}
 * @verification{}
 */
static void
Pt_Init_Next_Point_Indices(Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx /**< Point indices which shall be adapted*/);

/**
 * @brief Set information for the respective path-object pair
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7474}
 * @verification{}
 */
static void
Pt_Set_Path_Obj_Pair_Border_Info(Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info /**< information about the current path-object pair*/,
                                 const Pt_Path_T *p_path /**< respective path of the path-object pair*/,
                                 const uint8_t next_point_idx /**<grid point index which is closest to the object*/,
                                 const Pt_Object_Mov_Direction_T obj_move_dir /**< current object moving direction*/);

/**
 * @brief Calculate total confidence and submetrics of the current path object pair.
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7448}
 * @verification{}
 */
static void Pt_Calc_Confidence_Of_Current_Pair(
   Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence /**< confidence structure*/,
   const Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< information about the current path-object pair*/,
   const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
   const uint8_t object_index /**< object index used for debugging purpose*/,
   const uint8_t path_index /**< object index used for debugging purpose*/);

/**
 * @brief Calculates the relevant distance component between path and object for classification of a confidence
 * factor independent on the grid axis. This means each time an object is passing a grid point, the distance shall not be reduced
 * to at least the grid array granularity.
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7449}
 * @verification{}
 */
static void
Pt_Calc_Relevant_Match_Dist_Comp(Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< information about the current path-object pair*/,
                                 const Pt_Path_T *p_path /**< current considered path for pair*/,
                                 const Fbk_Object_Data_T *p_object /**< current considered object for pair*/,
                                 const float32_T grid_array[PT_NUM_GRID_POINTS] /**<  grid point array*/,
                                 const uint8_t next_point_idx /**<grid point index which is closest to the object*/);

/**
 * @brief Calculates Weighted distance of the last two path points and the trail of the considered object
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7450}
 * @verification{}
 */
static void
Pt_Calc_Weighted_Dist_Of_Path_Pt(Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< information about the current path-object pair*/,
                                 const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< information about the current path-object pair*/,
                                 const Pt_Path_T *p_path /**< current considered path for pair*/,
                                 const Fbk_Object_Data_T *p_object /**< current considered object for pair*/,
                                 const Pt_Core_Calibration_T *p_cals /**< calibration parameter*/);

/**
 * @brief Determines if the non default weighted dist mean shall be calculated.
 *
 * @return true when non default weighted dist mean shall be calculated
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7475}
 * @verification{}
 */
static boolean_T Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(const Pt_Trail_Path_Comp_Indices_T *p_point_to_compare_idx,
                                                                       const Pt_Path_T *p_path);

/**
 * @brief Maps one point border of the given path and a successive path point to a structure for
 * later comparison
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7458}
 * @verification{}
 */
static void Pt_Get_Last_Two_Point_Idx_Of_Trail(
   Pt_Trail_Path_Comp_Indices_T
      *p_point_to_compare_idx /**< mapping of point indices which shall be compared in confidence calculation*/,
   const Pt_Path_T *p_path /**< path information*/);

/**
 * @brief calculates the distance between path and object.
 *
 * @return distance between object and path of type float32_T in m
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7451}
 * @verification{}
 */
static void Pt_Calculate_Distance_Between_Object_And_Path(
   Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< path object pair info which containts the radial distance*/,
   const Pt_Path_T *p_path /**< respective path*/,
   const Fbk_Object_Data_T *p_object /**< object data*/,
   const uint8_t next_point_idx /**<grid point index which is closest to object (left side of grid array)*/,
   const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/,
   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid point array*/);

/**
 * @brief Calculates the heading difference confidence based on the next three heading differences.
 *
 * @return confidence value for heading difference of type float
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7452}
 * @verification{Verify that based on given heading differences, a weighted confidence metric is given.}
 */
static float32_T
Pt_Calculate_Heading_Diff_Confidence(const Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info /**< Path object pair information*/,
                                     const Pt_Core_Calibration_T *p_cals /**< pt calibrations*/,
                                     const uint8_t object_index /**< object index used for debugging purpose*/,
                                     const uint8_t path_index /**< path index used for debugging purpose*/);

/**
 * @brief Calculates the next three heading differences.
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7453}
 * @verification{Verify that for a given scenario three heading differences for the next three path segments for object perspective
 * are returned.}
 */
static void Pt_Calculate_Heading_Diffs(Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< Path object pair information*/,
                                       const Pt_Input_T *p_pt_input /**< input data of Path Tracking*/,
                                       const Fbk_Object_Data_T *p_object /**< object data*/,
                                       const Pt_Path_T *p_path /**< path information*/,
                                       const Pt_Object_Mov_Direction_T obj_move_dir /**< object moving direction*/,
                                       const uint8_t next_point_idx /**< point left to object*/);

/**
 * @brief Calculates the heading difference by object position and independent of its movement direction. This independency is
 * needed for nearest path information output.
 *
 * @return Returns the path heading of the object surrounding path segment independent of the object movement direction
 *
 * @SRS{SF-1564}
 * @SAE{}
 * @SDD{SF-7584}
 * @verification{Verify that based on the specified path segment the correct heading difference is returned.}
 */
static void Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< path object info*/,
                                                    const uint8_t next_point_idx /**< next point index for the given scenario*/,
                                                    const Pt_Path_T *p_path /**<input path*/,
                                                    const Fbk_Object_Data_T *p_object /**< object to be matched*/,
                                                    const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array information*/);

/**
 * @brief Logic for calculation of heading differences.
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7454}
 * @verification{Verify that based on the specified path segment the heading difference of that and the object is returned..}
 */
static void Pt_Calculate_Path_Segment_Heading_Diff(Pt_Path_Obj_Pair_Info_T *p_path_obj_info /**< Path object pair information*/,
                                                   const Pt_Path_T *p_path /**< pt calibrations*/,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS] /**< grid array*/,
                                                   const Fbk_Object_Data_T *p_object /**< object data*/,
                                                   const uint8_t segment_idx /**< index of the current path segment*/,
                                                   const uint8_t idx_to_map /**< index for mapping of heading difference*/,
                                                   const Pt_Object_Mov_Direction_T obj_move_dir /**< object moving direction*/);

/**
 * @brief calls all subroutines which are needed for object to path matching.
 * Here the closest grid point index to the left of the object is calculated which is passed to the subfunctions
 * After the call path output struct is filled which gives insights about the object to path matches and several more informations
 * about paths
 *
 * @return void
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7473}
 * @verification{}
 */
static void Pt_Match_Object_To_Path(Pt_Persistent_T *p_pt_persistent /**<persistent data of Path Tracking*/,
                                    Pt_Output_T *pt_output /**< pt output struct*/,
                                    const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                                    const Pt_Input_T *p_pt_input /**< input of Path tracking*/,
                                    const Pt_Object_T *p_object /**< object data*/,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief checks the object is out of path tracking range.
 *
 * @return True if object is out of path tracking range
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7467}
 * @verification{}
 */
static boolean_T Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range(const Vector_2d_T *p_obj_pos /**< twodimensional object position*/,
                                                          const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/,
                                                          const Pt_Input_T *p_pt_input /**<path tracking input*/);

/**
 * @brief checks wether path direction and object movement direction are aligned to each other.
 * Both need to be either longitudinal or lateral in order to return true.
 *
 * @return True if object direction and path direction are both lateral or longitudinal
 *
 * @SRS{SF-1564}
 * @SAE{SF-2918}
 * @SDD{SF-7456}
 * @verification{}
 */
static boolean_T Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(
   const Pt_Path_T *p_path /**< path information*/, const Pt_Object_Mov_Direction_T obj_move_dir /**<object moving direction*/);

/**
 * @brief Returns the path_index of the path which is created with the
 * object considered.
 *
 * @return path_index where the considered object was used to track this path
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7342}
 * @verification{}
 */
static uint8_t Pt_Get_Path_Index(const Pt_Path_T paths[PT_NUMBER_OF_PATHS] /**< Persistent information of PT*/,
                                 const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Pt_Objects_To_Path_Matching(Pt_Persistent_T *p_pt_persistent,
                                 Pt_Output_T *pt_output,
                                 const Pt_Core_Calibration_T *p_cals,
                                 const Pt_Input_T *p_pt_input,
                                 const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t obj_loop_index;

   /* Asserts */
   assert(NULL != pt_output);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_pt_persistent);

   for (obj_loop_index = 0; obj_loop_index < PA_OBJ_NUMBER_OF_OBJECTS; obj_loop_index++)
   {
      const Fbk_Object_Data_T *p_obj_loop = &p_pt_input->p_fbk_output->p_pa_data->object_data[obj_loop_index];
      if (p_obj_loop->f_moveable)
      {
         if (PA_OBJ_STATUS_NEW == p_obj_loop->status)
         {
            Pt_Reset_Single_Path_Output(&pt_output->path_obj_pair_output[obj_loop_index],
                                        &pt_output->nearest_path_output[obj_loop_index]);
         }
         if (PA_OBJ_STATUS_INVALID != p_obj_loop->status)
         {
            Pt_Object_T object;
            object.tracker_data = *p_obj_loop;
            Pt_Match_Object_To_Path(p_pt_persistent, pt_output, p_cals, p_pt_input, &object, p_vehicle_data);
         }
         else
         {
            Pt_Reset_Single_Path_Output(&pt_output->path_obj_pair_output[obj_loop_index],
                                        &pt_output->nearest_path_output[obj_loop_index]);
         }
      }
   }
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/
static void Pt_Match_Object_To_Path(Pt_Persistent_T *p_pt_persistent,
                                    Pt_Output_T *pt_output,
                                    const Pt_Core_Calibration_T *p_cals,
                                    const Pt_Input_T *p_pt_input,
                                    const Pt_Object_T *p_object,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Pt_Object_Mov_Direction_T obj_move_dir;
   Pt_Path_Obj_Pair_Info_T best_path_obj_pair_info;
   Pt_Path_Obj_Pair_Consumer_Info_T path_consumer_info;

   /* Asserts */
   assert(NULL != p_pt_input);
   assert(NULL != p_pt_persistent);
   assert(NULL != pt_output);
   assert(NULL != p_cals);
   assert(NULL != p_object);

   Pt_Pt_Init_Path_Obj_Pair_Info(&(best_path_obj_pair_info));
   Pt_Pt_Init_Path_Obj_Pair_Consumer_Info(&path_consumer_info);

   obj_move_dir = Pt_Determine_Object_Movement_Direction(p_object->tracker_data.vcs_heading, p_cals);

   if (Fbk_Is_False(Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, p_cals, p_pt_input, &p_object->tracker_data)))
   {
      Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;

      Pt_Get_Next_Point_For_Object_To_Pass(&next_point_idx, &p_object->tracker_data, p_pt_input->grid_pt_array);
      if (Pt_Is_Next_Point_Idx_Valid(&next_point_idx))
      {
         Pt_Find_Best_Matching_Path(p_pt_persistent, &(best_path_obj_pair_info), &(path_consumer_info), p_pt_input, p_cals,
                                    obj_move_dir, &next_point_idx, &p_object->tracker_data);
         Pt_Set_Best_Matching_Path_Output(pt_output, p_pt_persistent, &(path_consumer_info), p_pt_input, p_object, p_cals,
                                          obj_move_dir, &next_point_idx, p_vehicle_data);
      }
   }
}

static boolean_T Pt_Is_Next_Point_Idx_Valid(const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx)
{
   boolean_T f_point_index_is_valid = FBK_FALSE;

   /* Assert */
   assert(NULL != p_next_point_idx);

   if ((PT_LOWEST_GRID_POINT_INDEX < p_next_point_idx->next_point_idx_lat_path)
       && (PT_HIGHEST_GRID_POINT_INDEX > p_next_point_idx->next_point_idx_lat_path)
       && (PT_LOWEST_GRID_POINT_INDEX < p_next_point_idx->next_point_idx_long_path)
       && (PT_HIGHEST_GRID_POINT_INDEX > p_next_point_idx->next_point_idx_long_path))
   {
      f_point_index_is_valid = FBK_TRUE;
   }
   return f_point_index_is_valid;
}

static void Pt_Find_Best_Matching_Path(Pt_Persistent_T *p_pt_persistent,
                                       Pt_Path_Obj_Pair_Info_T *p_best_path_obj_pair_info,
                                       Pt_Path_Obj_Pair_Consumer_Info_T *p_path_consumer_info,
                                       const Pt_Input_T *p_pt_input,
                                       const Pt_Core_Calibration_T *p_cals,
                                       const Pt_Object_Mov_Direction_T obj_move_dir,
                                       const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx,
                                       const Fbk_Object_Data_T *p_object)
{
   uint8_t i;
   uint8_t next_point_idx;
   boolean_T f_best_path_obj_match_found = FBK_FALSE;
   Pt_Path_Obj_Pair_Info_T path_obj_info;
   Pt_Path_Obj_Pair_Confidence_T path_obj_pair_confidence;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_object);
   assert(NULL != p_best_path_obj_pair_info);
   assert(NULL != p_path_consumer_info);

   for (i = 0; i < PT_NUMBER_OF_PATHS; i++)
   {
      if (Pt_Is_Path_Valid_For_Object_Matching(&p_pt_persistent->paths[i], p_object, p_cals))
      {
         next_point_idx = Pt_Read_Next_Point_Dep_On_Path_Dir(p_next_point_idx, &p_pt_persistent->paths[i]);
         if (Pt_Is_Next_Point_Contained_In_Valid_Path_Borders(next_point_idx, &p_pt_persistent->paths[i],
                                                              &p_pt_input->Num_Grid_Pts_Dep_Cals))
         {
            /*Initialize information for current path-object pair*/
            Pt_Pt_Init_Path_Obj_Pair_Info(&(path_obj_info));
            Pt_Pt_Init_Path_Obj_Pair_Confidence(&(path_obj_pair_confidence));

            /* Calculate entities independent of the object moving direction for nearest path information */
            Pt_Calc_Relevant_Match_Dist_Comp(&path_obj_info, &p_pt_persistent->paths[i], p_object, p_pt_input->grid_pt_array,
                                             next_point_idx);
            /*Get heading diffs for closest to successive path points*/
            Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(&path_obj_info, next_point_idx, &p_pt_persistent->paths[i], p_object,
                                                    p_pt_input->grid_pt_array);

            if (Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(&p_pt_persistent->paths[i], obj_move_dir))
            {

               /*Set information of path-object pair for later confidence calculation*/
               Pt_Set_Path_Obj_Pair_Border_Info(&(path_obj_info), &p_pt_persistent->paths[i], next_point_idx, obj_move_dir);

               Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, p_pt_persistent->paths, &p_pt_persistent->paths[i], p_object, p_cals);

               Pt_Calculate_Heading_Diffs(&path_obj_info, p_pt_input, p_object, &p_pt_persistent->paths[i], obj_move_dir,
                                          next_point_idx);

               /*Calculate confidence of the current path-object pair*/
               Pt_Calc_Confidence_Of_Current_Pair(&path_obj_pair_confidence, &(path_obj_info), p_cals, p_object->index, i);

               /*Condition for the best currently possible match*/
               if ((Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info,
                                                     &p_pt_persistent->best_path_obj_pairs[p_object->id], p_pt_persistent->paths,
                                                     p_object, p_cals, i, obj_move_dir)))
               {
                  p_pt_persistent->best_path_obj_pairs[p_object->id].confidence_factor =
                     path_obj_pair_confidence.confidence_factor_total;
                  p_pt_persistent->best_path_obj_pairs[p_object->id].relevant_dist_comp_matching =
                     path_obj_info.relevant_dist_comp_matching;
                  p_pt_persistent->best_path_obj_pairs[p_object->id].path_index = p_pt_persistent->paths[i].path_index;
                  *p_best_path_obj_pair_info                                    = path_obj_info;
                  f_best_path_obj_match_found                                   = FBK_TRUE;

                  /*Since there is now additional copying of path_pair happening, only write best result, when path_obj_pair
                  is currently the best.*/
                  Binary_Pt_Debug_Pass_Best_Pair_Conf_To_Intern(&path_obj_pair_confidence, p_object->index,
                                                                p_pt_persistent->paths[i].path_index);
               }
            }
            Binary_Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(&path_obj_pair_confidence, &path_obj_info, p_object->index,
                                                                   p_pt_persistent->paths[i].path_index);

            /*Condition for the best currently possible match*/
            if (Fbk_Abs_F(path_obj_info.relevant_dist_comp_matching) < p_path_consumer_info->distance_to_path)
            {
               p_path_consumer_info->distance_to_path     = Fbk_Abs_F(path_obj_info.relevant_dist_comp_matching);
               p_path_consumer_info->segment_heading_diff = path_obj_info.heading_diff_pt_segment[FBK_ZERO_UINT];
               p_path_consumer_info->path_index           = p_pt_persistent->paths[i].path_index;
            }
         }
      }
   }
   if (Fbk_Is_False(f_best_path_obj_match_found))
   {
      Pt_Reset_Single_Matching_Pair(&(p_pt_persistent->best_path_obj_pairs[p_object->id]));
   }
   else
   {
      next_point_idx = Pt_Read_Next_Point_Dep_On_Path_Dir(
         p_next_point_idx, &p_pt_persistent->paths[p_pt_persistent->best_path_obj_pairs[p_object->id].path_index]);
      Pt_Calculate_Distance_Between_Object_And_Path(
         p_best_path_obj_pair_info, &p_pt_persistent->paths[p_pt_persistent->best_path_obj_pairs[p_object->id].path_index],
         p_object, next_point_idx, obj_move_dir, p_pt_input->grid_pt_array);
   }
}

static boolean_T Pt_Is_Path_Obj_Pair_Better_Match(const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                                  const Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                  const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair,
                                                  const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                                  const Fbk_Object_Data_T *p_object,
                                                  const Pt_Core_Calibration_T *p_cals,
                                                  const uint8_t path_candidate_idx,
                                                  const Pt_Object_Mov_Direction_T obj_move_dir)
{
   boolean_T f_path_obj_pair_is_better_match  = FBK_FALSE;
   boolean_T f_path_change_due_to_nearer_path = FBK_FALSE;
   boolean_T f_path_change_due_to_grouping    = FBK_FALSE;
   float32_T path_change_matching_hysteresis  = FBK_ZERO_F;
   boolean_T f_prev_best_pair_updates_confidence;
   float32_T minimum_required_confidence_value;
   boolean_T f_best_path_pair_change_criteria_fulfilled;

   /* Asserts */
   assert(NULL != p_path_obj_pair_confidence);
   assert(NULL != p_best_path_object_pair);
   assert(NULL != paths);
   assert(NULL != p_object);
   assert(NULL != p_cals);
   assert(PT_NUMBER_OF_PATHS > path_candidate_idx);

   /*Check whether two paths are similar*/
   if (Pt_Are_Paths_Similar_At_Host_And_Object_Level(p_path_obj_info, p_best_path_object_pair, paths, p_cals, path_candidate_idx,
                                                     obj_move_dir, p_object))
   {
      /*Check whether current path is nearer. Only then best path shall be overwritten.*/
      if (Pt_Is_Path_Candidate_Nearer_To_Host(paths, obj_move_dir, path_candidate_idx, p_best_path_object_pair->path_index))
      {
         f_path_change_due_to_nearer_path = FBK_TRUE;
      }
   }
   else
   {
      if (PT_DEFAULT_MATCH_INDEX != p_best_path_object_pair->path_index)
      {
         path_change_matching_hysteresis =
            Pt_Return_Confidence_Hysteresis_For_Path_Change(p_best_path_object_pair, paths, p_cals, path_candidate_idx);
      }

      f_path_change_due_to_grouping = (boolean_T) (p_path_obj_pair_confidence->confidence_factor_total
                                                   >= (p_best_path_object_pair->confidence_factor + path_change_matching_hysteresis));
   }

   /*Evaluate path change criteria */
   f_best_path_pair_change_criteria_fulfilled = (boolean_T) (f_path_change_due_to_grouping || f_path_change_due_to_nearer_path);

   /*In case that a previous matched object to a path is shown here, a hysteresis minimum needed confidence value shall be given.*/
   f_prev_best_pair_updates_confidence = (boolean_T) (paths[path_candidate_idx].path_index == p_best_path_object_pair->path_index);
   if (Fbk_Is_True(f_prev_best_pair_updates_confidence))
   {
      minimum_required_confidence_value = p_cals->k_pt_min_confidence_valid_match - p_cals->k_pt_path_change_match_hyst_default;
   }
   else
   {
      minimum_required_confidence_value = p_cals->k_pt_min_confidence_valid_match;
   }

   if ((p_path_obj_pair_confidence->confidence_factor_total >= minimum_required_confidence_value)
       && ((f_best_path_pair_change_criteria_fulfilled) || (f_prev_best_pair_updates_confidence)))
   {
      f_path_obj_pair_is_better_match = FBK_TRUE;
      Binary_Pt_Debug_Pass_Match_Information(f_path_change_due_to_grouping, f_path_change_due_to_nearer_path,
                                             f_prev_best_pair_updates_confidence, path_change_matching_hysteresis, p_object->index,
                                             p_cals);
   }

   return f_path_obj_pair_is_better_match;
}

static float32_T Pt_Return_Confidence_Hysteresis_For_Path_Change(const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair,
                                                                 const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                                                 const Pt_Core_Calibration_T *p_cals,
                                                                 const uint8_t path_candidate_idx)
{
   float32_T hysteresis_to_return;
   int32_t difference_num_groupings;

   /* Asserts */
   assert(NULL != p_best_path_object_pair);
   assert(NULL != paths);
   assert(NULL != p_cals);
   assert(PT_NUMBER_OF_PATHS > path_candidate_idx);

   /*In case of a negative difference, the path candidate is more established. In case that the differnce is positive, the
    * currently best path is the most established. For the value zero, a default hysteresis shall be returned.*/
   difference_num_groupings =
      ((int32_t) paths[p_best_path_object_pair->path_index].num_groupings) - ((int32_t) paths[path_candidate_idx].num_groupings);

   if (difference_num_groupings < FBK_ZERO_INT)
   {
      /*Since the hysteresis is externally additive used, the path candidate needs to have a way lower confidence value so that
       * it is able to be matched to the object. That is why the hysteresis is chosen negative here.*/
      if (PATH_STATUS_CREATION == paths[p_best_path_object_pair->path_index].path_state)
      {
         /*Ensure that creation phase paths are weighted even less.*/
         hysteresis_to_return = -p_cals->k_pt_path_change_one_grouped_one_creation;
      }
      else if (PATH_STATUS_MATURE == paths[p_best_path_object_pair->path_index].path_state)
      {
         /*Ensure paths without grouping are weighted a little more.*/
         hysteresis_to_return = -p_cals->k_pt_path_change_one_grouped_one_mature;
      }
      else
      {
         /*Use a hysteresis for comparison between paths in grouped states.*/
         hysteresis_to_return = -p_cals->k_pt_path_change_match_hyst_more_established;
      }
   }
   else if (difference_num_groupings > FBK_ZERO_INT)
   {
      /*Since the hysteresis is externally additive used, the path candidate needs to have a way higher confidence value so that it
       * is able to be matched to the object.*/
      if (PATH_STATUS_CREATION == paths[path_candidate_idx].path_state)
      {
         /*Ensure that creation phase paths are weighted even less.*/
         hysteresis_to_return = p_cals->k_pt_path_change_one_grouped_one_creation;
      }
      else if (PATH_STATUS_MATURE == paths[path_candidate_idx].path_state)
      {
         hysteresis_to_return = p_cals->k_pt_path_change_one_grouped_one_mature;
      }
      else
      {
         hysteresis_to_return = p_cals->k_pt_path_change_match_hyst_more_established;
      }
   }
   else
   {
      if ((PATH_STATUS_MATURE == paths[p_best_path_object_pair->path_index].path_state)
          && (PATH_STATUS_CREATION == paths[path_candidate_idx].path_state))
      {
         hysteresis_to_return = p_cals->k_pt_path_change_differing_states_hyst_default;
      }
      else if ((PATH_STATUS_CREATION == paths[p_best_path_object_pair->path_index].path_state)
               && (PATH_STATUS_MATURE == paths[path_candidate_idx].path_state))
      {
         hysteresis_to_return = -p_cals->k_pt_path_change_differing_states_hyst_default;
      }
      else
      {
         hysteresis_to_return = p_cals->k_pt_path_change_match_hyst_default;
      }
   }

   return hysteresis_to_return;
}

static boolean_T Pt_Are_Paths_Similar_At_Host_And_Object_Level(const Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                               const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair,
                                                               const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                                               const Pt_Core_Calibration_T *p_cals,
                                                               const uint8_t path_candidate_idx,
                                                               const Pt_Object_Mov_Direction_T obj_move_dir,
                                                               const Fbk_Object_Data_T *p_object)
{
   boolean_T f_paths_are_similar_at_host_and_object_level = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_path_obj_info);
   assert(NULL != p_best_path_object_pair);
   assert(NULL != paths);
   assert(NULL != p_cals);
   assert(PT_NUMBER_OF_PATHS > path_candidate_idx);

   /*Check whether the given scenario, specified by object and paths are valid for nearest path matching.*/
   if (Pt_Are_Paths_Valid_For_Nearest_Path_Match(p_best_path_object_pair, paths, p_object, path_candidate_idx, obj_move_dir))
   {
      boolean_T f_paths_similar_at_obj_level  = FBK_FALSE;
      boolean_T f_paths_similar_at_host_level = FBK_FALSE;
      boolean_T f_mid_points_in_range;
      uint8_t idx;

      /*Check whether the mid point is contained in the path so that it can be judged whether two paths are similar there.*/
      f_mid_points_in_range =
         Pt_Is_Mid_Point_Included_In_Paths(&paths[path_candidate_idx], &paths[p_best_path_object_pair->path_index]);

      /*In case that the mid points of both paths are not included, further checks do not need to be performed.*/
      if (Fbk_Is_True(f_mid_points_in_range))
      {
         f_paths_similar_at_obj_level =
            (boolean_T) (Fbk_Abs_F(p_path_obj_info->relevant_dist_comp_matching - p_best_path_object_pair->relevant_dist_comp_matching)
                         <= p_cals->k_pt_dist_betw_paths_similarity_matching);

         /*Check whether at least one point near the host is similar*/
         if (Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir))
         {
            for (idx = PT_MID_GRID_POINT_INDEX; idx <= (PT_MID_GRID_POINT_INDEX + (2u * PT_SINGLE_GRID_POINT_OFFSET)); idx++)
            {
               if (Fbk_Abs_F(paths[path_candidate_idx].path_points[idx] - paths[p_best_path_object_pair->path_index].path_points[idx])
                   <= p_cals->k_pt_dist_betw_paths_similarity_matching)
               {
                  f_paths_similar_at_host_level = FBK_TRUE;
                  break;
               }
            }
         }
         else
         {
            for (idx = PT_MID_GRID_POINT_INDEX; idx >= (PT_MID_GRID_POINT_INDEX - (2u * PT_SINGLE_GRID_POINT_OFFSET)); idx--)
            {
               if (Fbk_Abs_F(paths[path_candidate_idx].path_points[idx] - paths[p_best_path_object_pair->path_index].path_points[idx])
                   <= p_cals->k_pt_dist_betw_paths_similarity_matching)
               {
                  f_paths_similar_at_host_level = FBK_TRUE;
                  break;
               }
            }
         }
      }

      f_paths_are_similar_at_host_and_object_level =
         (boolean_T) (f_mid_points_in_range && f_paths_similar_at_obj_level && f_paths_similar_at_host_level);
   }

   return f_paths_are_similar_at_host_and_object_level;
}

static boolean_T Pt_Are_Paths_Valid_For_Nearest_Path_Match(const Pt_Best_Path_Obj_Pair_Persistent_T *p_best_path_object_pair,
                                                           const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                                           const Fbk_Object_Data_T *p_object,
                                                           const uint8_t path_candidate_idx,
                                                           const Pt_Object_Mov_Direction_T obj_move_dir)
{
   boolean_T f_paths_are_valid_for_nearest_path_matching = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_best_path_object_pair);
   assert(NULL != paths);
   assert(PT_NUMBER_OF_PATHS > path_candidate_idx);

   /*When no path is already classified as best path, then this check shall not be applied.*/
   if ((PT_DEFAULT_MATCH_INDEX != p_best_path_object_pair->path_index)
       && Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, p_object)
       && (PATH_STATUS_CREATION != paths[path_candidate_idx].path_state))
   {
      /*The nearest path matching shall be only applied, when both paths are out of their creation phase.*/
      if (PATH_STATUS_CREATION != paths[p_best_path_object_pair->path_index].path_state)
      {
         f_paths_are_valid_for_nearest_path_matching = FBK_TRUE;
      }
   }

   return f_paths_are_valid_for_nearest_path_matching;
}

static boolean_T Pt_Is_Mid_Point_Included_In_Paths(const Pt_Path_T *p_path_a, const Pt_Path_T *p_path_b)
{
   boolean_T f_mid_included_in_path_a;
   boolean_T f_mid_included_in_path_b;

   /* Asserts */
   assert(NULL != p_path_a);
   assert(NULL != p_path_b);

   f_mid_included_in_path_a =
      (boolean_T) ((p_path_a->first_p <= PT_MID_GRID_POINT_INDEX) && (p_path_a->last_p >= PT_MID_GRID_POINT_INDEX));
   f_mid_included_in_path_b =
      (boolean_T) ((p_path_b->first_p <= PT_MID_GRID_POINT_INDEX) && (p_path_b->last_p >= PT_MID_GRID_POINT_INDEX));

   return (boolean_T) (f_mid_included_in_path_a && f_mid_included_in_path_b);
}

static boolean_T Pt_Is_Path_Candidate_Nearer_To_Host(const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                                     const Pt_Object_Mov_Direction_T obj_move_dir,
                                                     const uint8_t path_candidate_idx,
                                                     const uint8_t best_path_object_pair_idx)
{
   boolean_T f_path_candidate_is_nearer_to_host = FBK_FALSE;
   float32_T dist_betw_best_and_host            = FBK_ZERO_F;
   float32_T dist_betw_candidate_and_host       = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != paths);
   assert(PT_NUMBER_OF_PATHS > path_candidate_idx);
   assert(PT_NUMBER_OF_PATHS > best_path_object_pair_idx);

   /*Based on moving direction of the target, different path points shall be compared*/
   if (Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir))
   {
      uint8_t idx;
      uint8_t start_idx;
      uint8_t end_idx;

      if (Pt_Is_Obj_Moving_Longitudinal(&obj_move_dir))
      {
         /*This difference is due to front Clear Exit Detection cases. PT_MID_GRID_POINT_INDEX alone would be too far away from the
          * crash line in those cases.*/
         assert(PT_MID_GRID_POINT_INDEX >= PT_SINGLE_GRID_POINT_OFFSET);
         start_idx = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
         end_idx   = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
      }
      else
      {
         /*In case of lateral Cross Traffic alert cases both directions can be setup symmetrical.*/
         start_idx = PT_MID_GRID_POINT_INDEX;
         end_idx   = PT_MID_GRID_POINT_INDEX + (2u * PT_SINGLE_GRID_POINT_OFFSET);
      }

      for (idx = start_idx; idx <= end_idx; idx++)
      {
         dist_betw_candidate_and_host += Fbk_Abs_F(paths[path_candidate_idx].path_points[idx]);
         dist_betw_best_and_host += Fbk_Abs_F(paths[best_path_object_pair_idx].path_points[idx]);
      }
   }
   else
   {
      uint8_t idx;
      assert(PT_MID_GRID_POINT_INDEX >= (2 * PT_SINGLE_GRID_POINT_OFFSET));

      for (idx = PT_MID_GRID_POINT_INDEX; idx >= (PT_MID_GRID_POINT_INDEX - (2u * PT_SINGLE_GRID_POINT_OFFSET)); idx--)
      {
         dist_betw_candidate_and_host += Fbk_Abs_F(paths[path_candidate_idx].path_points[idx]);
         dist_betw_best_and_host += Fbk_Abs_F(paths[best_path_object_pair_idx].path_points[idx]);
      }
   }

   /*Check whether the absolute distances is smaller for the nearest path.*/
   if (dist_betw_candidate_and_host < dist_betw_best_and_host)
   {
      f_path_candidate_is_nearer_to_host = FBK_TRUE;
   }

   return f_path_candidate_is_nearer_to_host;
}

static boolean_T Pt_Is_Obj_Valid_For_Path_Similarity_Check(const Pt_Object_Mov_Direction_T obj_move_dir,
                                                           const Fbk_Object_Data_T *p_object)
{
   /* Assert */
   assert(NULL != p_object);

   /*Check whether object has already passed the critical coordinate axis in vcs, since another check shall only be applied in case
    * that the object has not surpassed feature critical coordinate axes.*/
   return (boolean_T) (((PT_OBJECT_MOV_DIR_LAT_LEFT == obj_move_dir) && (p_object->vcs_pos.y > FBK_ZERO_F))
                       || ((PT_OBJECT_MOV_DIR_LAT_RIGHT == obj_move_dir) && (p_object->vcs_pos.y < FBK_ZERO_F))
                       || ((PT_OBJECT_MOV_DIR_LONG_FORWARD == obj_move_dir) && (p_object->vcs_pos.x < FBK_ZERO_F))
                       || ((PT_OBJECT_MOV_DIR_LONG_BACKWARD == obj_move_dir) && (p_object->vcs_pos.x > FBK_ZERO_F)));
}

static void Pt_Calculate_Distance_Between_Object_And_Path(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                          const Pt_Path_T *p_path,
                                                          const Fbk_Object_Data_T *p_object,
                                                          const uint8_t next_point_idx,
                                                          const Pt_Object_Mov_Direction_T obj_move_dir,
                                                          const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   float32_T delta_lat;
   float32_T delta_long;
   float32_T rad_object_to_path_dist;

   /* Asserts */
   assert(NULL != p_path_obj_info);
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   if (Pt_Is_Obj_Moving_Longitudinal(&obj_move_dir))
   {
      delta_long = p_object->vcs_pos.x - grid_array[next_point_idx];
      rad_object_to_path_dist =
         p_object->vcs_pos.y
         - (p_path->path_points[next_point_idx] + (delta_long * Fast_Tan(p_path_obj_info->closest_to_successive_pt_heading)));
   }
   else
   {
      delta_lat = p_object->vcs_pos.y - grid_array[next_point_idx];
      rad_object_to_path_dist =
         p_object->vcs_pos.x
         - (p_path->path_points[next_point_idx] + (delta_lat / Fast_Tan(p_path_obj_info->closest_to_successive_pt_heading)));
   }

   p_path_obj_info->rad_object_to_path_dist = rad_object_to_path_dist;
}

static void Pt_Calculate_Heading_Diffs(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                       const Pt_Input_T *p_pt_input,
                                       const Fbk_Object_Data_T *p_object,
                                       const Pt_Path_T *p_path,
                                       const Pt_Object_Mov_Direction_T obj_move_dir,
                                       const uint8_t next_point_idx)
{
   uint8_t start_idx;                 /*index to start looking at heading differences of path segments and tracker heading*/
   uint8_t end_idx;                   /*index to stop looking at heading differences of path segments and tracker heading*/
   uint8_t idx_to_saturate;           /*helper index to saturate the end index so that no incorrect memory usage is happening. */
   uint8_t idx;                       /*loop index*/
   uint8_t idx_to_map = FBK_ONE_UINT; /*index for the mapping onto the internal heading difference array*/

   /* Asserts */
   assert(NULL != p_pt_input);
   assert(NULL != p_path);
   assert(PT_SINGLE_GRID_POINT_OFFSET <= next_point_idx);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   /*Get heading diffs for the next segments*/
   if (Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir))
   {
      start_idx = (uint8_t) (next_point_idx - PT_SINGLE_GRID_POINT_OFFSET);
      end_idx   = PT_LOWEST_GRID_POINT_INDEX;

      if (next_point_idx > (PT_NUM_HEADING_SEGMENTS + PT_LOWEST_GRID_POINT_INDEX))
      {
         end_idx = (uint8_t) (next_point_idx - PT_NUM_HEADING_SEGMENTS);
      }

      if (PATH_STATUS_CREATION == p_path->path_state)
      {
         /*For paths in creation state we are only checking heading differences in real tracked path segments and saturate this
          * border at the lower border*/
         idx_to_saturate = p_path->first_p;
         end_idx         = Max(end_idx, idx_to_saturate);
      }
      else
      {
         /*For paths which are in mature or grouped states we are also checking heading differences in a given border of the real
          * tracked path. This border is above the upper border in an extrapolated path part.*/
         idx_to_saturate = PT_LOWEST_GRID_POINT_INDEX;
         if (p_path->first_p > (p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points + PT_LOWEST_GRID_POINT_INDEX))
         {
            idx_to_saturate = (uint8_t) (p_path->first_p - p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points);
         }
         end_idx = Max(end_idx, idx_to_saturate);
      }

      for (idx = start_idx; idx > end_idx; idx--)
      {
         Pt_Calculate_Path_Segment_Heading_Diff(p_path_obj_info, p_path, p_pt_input->grid_pt_array, p_object, idx, idx_to_map,
                                                obj_move_dir);
         idx_to_map++;
      }
   }
   else
   {
      start_idx = (uint8_t) (next_point_idx + PT_SINGLE_GRID_POINT_OFFSET);
      end_idx   = Min(PT_HIGHEST_GRID_POINT_INDEX, next_point_idx + PT_NUM_HEADING_SEGMENTS);

      if (PATH_STATUS_CREATION == p_path->path_state)
      {
         /*For paths in creation state we are only checking heading differences in real tracked path segments and saturate this
          * border at the upper border*/
         idx_to_saturate = p_path->last_p;
         end_idx         = Min(end_idx, idx_to_saturate);
      }
      else
      {
         /*For paths which are in mature or grouped states we are also checking heading differences in a given border of the real
          * tracked path. This border is above the upper border in an extrapolated path part.*/
         idx_to_saturate = (uint8_t) (p_path->last_p + p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points);
         end_idx         = Min(end_idx, idx_to_saturate);
      }

      for (idx = start_idx; idx < end_idx; idx++)
      {
         Pt_Calculate_Path_Segment_Heading_Diff(p_path_obj_info, p_path, p_pt_input->grid_pt_array, p_object, idx, idx_to_map,
                                                obj_move_dir);
         idx_to_map++;
      }
   }
}

static void Pt_Calculate_Path_Segment_Heading_Diff(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                   const Pt_Path_T *p_path,
                                                   const float32_T grid_array[PT_NUM_GRID_POINTS],
                                                   const Fbk_Object_Data_T *p_object,
                                                   const uint8_t segment_idx,
                                                   const uint8_t idx_to_map,
                                                   const Pt_Object_Mov_Direction_T obj_move_dir)
{
   float32_T path_segment_heading;

   /* Asserts */
   assert(NULL != p_path_obj_info);
   assert(NULL != p_object);
   assert(PT_NUM_HEADING_SEGMENTS > idx_to_map);

   path_segment_heading = Pt_Calculate_Path_Heading_Near_Object(segment_idx, p_path, obj_move_dir, grid_array);

   p_path_obj_info->heading_diff_pt_segment[idx_to_map] =
      Fbk_Abs_F(Normalize_Angle(p_object->vcs_heading, FBK_ZERO_F) - path_segment_heading);

   p_path_obj_info->heading_diff_pt_segment[idx_to_map] =
      Normalize_Angle(p_path_obj_info->heading_diff_pt_segment[idx_to_map], FBK_ZERO_F);
}

static void Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                    const uint8_t next_point_idx,
                                                    const Pt_Path_T *p_path,
                                                    const Fbk_Object_Data_T *p_object,
                                                    const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   Vector_2d_T prev_to_next_point_vector;
   Vector_2d_T long_unit_vector;
   uint8_t previous_idx;
   float32_T sign                         = FBK_ONE_F;
   float32_T pos_for_border_determination = FBK_ZERO_F;

   /* Assert */
   assert(NULL != p_path_obj_info);
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   long_unit_vector = Create_2d_Vector_X_Normal();

   /* Chose border for index determination*/
   if (Pt_Is_Path_Lateral(p_path))
   {
      pos_for_border_determination = p_object->vcs_pos.y;
   }
   else if (Pt_Is_Path_Longitudinal(p_path))
   {
      pos_for_border_determination = p_object->vcs_pos.x;
   }
   else
   {
      /* Do nothing */
   }

   /*Search for the path boundaries where the object is included.*/
   if (grid_array[next_point_idx] < pos_for_border_determination)
   {
      previous_idx = (uint8_t) (next_point_idx + PT_SINGLE_GRID_POINT_OFFSET);
   }
   else
   {
      previous_idx = (uint8_t) (next_point_idx - PT_SINGLE_GRID_POINT_OFFSET);
   }

   /*Always build up a vector from previous point index to the next depending on the object moving direction*/
   prev_to_next_point_vector.x = p_path->path_points[next_point_idx] - p_path->path_points[previous_idx];
   prev_to_next_point_vector.y = grid_array[next_point_idx] - grid_array[previous_idx];

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      Fbk_Swap_Float(&(prev_to_next_point_vector.x), &(prev_to_next_point_vector.y));
   }

   if (prev_to_next_point_vector.y < FBK_ZERO_F)
   {
      sign = -FBK_ONE_F;
   }

   p_path_obj_info->closest_to_successive_pt_heading =
      sign * Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &prev_to_next_point_vector));

   p_path_obj_info->heading_diff_pt_segment[FBK_ZERO_UINT] =
      Fbk_Abs_F(Normalize_Angle(p_object->vcs_heading, FBK_ZERO_F) - p_path_obj_info->closest_to_successive_pt_heading);

   p_path_obj_info->heading_diff_pt_segment[FBK_ZERO_UINT] =
      Normalize_Angle(p_path_obj_info->heading_diff_pt_segment[FBK_ZERO_UINT], FBK_ZERO_F);
}

static void Pt_Calc_Relevant_Match_Dist_Comp(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                             const Pt_Path_T *p_path,
                                             const Fbk_Object_Data_T *p_object,
                                             const float32_T grid_array[PT_NUM_GRID_POINTS],
                                             const uint8_t next_point_idx)
{
   uint8_t first_point_border; /* Point idx behind the object.*/
   uint8_t sec_point_border;   /* Next point index.*/
   float32_T pos_of_interpolation = FBK_ZERO_F;
   float32_T extrapolated_point_comp;

   /* Asserts */
   assert(NULL != p_path_obj_info);
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   first_point_border = PT_DEFAULT_MATCH_INDEX;
   sec_point_border   = PT_DEFAULT_MATCH_INDEX;

   if (Pt_Is_Path_Lateral(p_path))
   {
      pos_of_interpolation = p_object->vcs_pos.y;
   }
   else if (Pt_Is_Path_Longitudinal(p_path))
   {
      pos_of_interpolation = p_object->vcs_pos.x;
   }
   else
   {
      /* Do nothing */
   }

   /*Search for the path boundaries where the object is included.*/
   if (grid_array[next_point_idx] < pos_of_interpolation)
   {
      first_point_border = (uint8_t) (next_point_idx + PT_SINGLE_GRID_POINT_OFFSET);
      sec_point_border   = next_point_idx;
   }
   else if (grid_array[next_point_idx] > pos_of_interpolation)
   {
      first_point_border = (uint8_t) (next_point_idx - PT_SINGLE_GRID_POINT_OFFSET);
      sec_point_border   = next_point_idx;
   }
   else
   {
      /*Do nothing*/
   }

   if ((PT_DEFAULT_MATCH_INDEX != first_point_border) && (PT_DEFAULT_MATCH_INDEX != sec_point_border))
   {
      if (Pt_Is_Path_Lateral(p_path))
      {
         if (Fbk_Abs_F(grid_array[first_point_border] - grid_array[sec_point_border]) > EPSILON)
         {
            extrapolated_point_comp = Get_Y_Value_From_Line_By_Coordinates(
               grid_array[first_point_border], p_path->path_points[first_point_border], grid_array[sec_point_border],
               p_path->path_points[sec_point_border], pos_of_interpolation);

            p_path_obj_info->relevant_dist_comp_matching = extrapolated_point_comp - p_object->vcs_pos.x;
         }
      }
      else if (Pt_Is_Path_Longitudinal(p_path))
      {
         if (Fbk_Abs_F(grid_array[first_point_border] - grid_array[sec_point_border]) > EPSILON)
         {
            extrapolated_point_comp = Get_Y_Value_From_Line_By_Coordinates(
               grid_array[first_point_border], p_path->path_points[first_point_border], grid_array[sec_point_border],
               p_path->path_points[sec_point_border], pos_of_interpolation);

            p_path_obj_info->relevant_dist_comp_matching = extrapolated_point_comp - p_object->vcs_pos.y;
         }
      }
      else
      {
         /*Do nothing*/
      }
   }
}

static void Pt_Calc_Weighted_Dist_Of_Path_Pt(Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                             const Pt_Path_T paths[PT_NUMBER_OF_PATHS],
                                             const Pt_Path_T *p_path,
                                             const Fbk_Object_Data_T *p_object,
                                             const Pt_Core_Calibration_T *p_cals)
{
   float32_T diff_high_prio;
   float32_T diff_low_prio;
   float32_T denominator;
   Pt_Trail_Path_Comp_Indices_T point_to_compare_idx;
   uint8_t path_created_by_obj_idx;

   /* Asserts */
   assert(NULL != p_path_obj_info);
   assert(NULL != paths);
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != p_cals);
   assert(FBK_ZERO_F < p_cals->k_pt_weight_of_last_trail_point + p_cals->k_pt_weight_of_sec_last_trail_point);

   path_created_by_obj_idx = Pt_Get_Path_Index(paths, p_object);

   if (PT_DEFAULT_MATCH_INDEX == path_created_by_obj_idx)
   {
      /*In case that a path is not yet associated to the object, then this metric shall not be effective*/
      p_path_obj_info->weighted_mean_diff_last_points = 0.0f;
   }
   else
   {
      if (Pt_Get_Number_Of_Path_Points(&(paths[path_created_by_obj_idx])) >= p_cals->k_pt_min_path_length_obj_trail)
      {
         Pt_Get_Last_Two_Point_Idx_Of_Trail(&(point_to_compare_idx), &(paths[path_created_by_obj_idx]));

         if (Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(&point_to_compare_idx, p_path))
         {
            diff_high_prio =
               Fbk_Abs_F(p_path->path_points[point_to_compare_idx.path_point_index_higher_prio]
                         - paths[path_created_by_obj_idx].path_points[point_to_compare_idx.path_point_index_higher_prio]);
            diff_low_prio = Fbk_Abs_F(p_path->path_points[point_to_compare_idx.path_point_index_lower_prio]
                                      - paths[path_created_by_obj_idx].path_points[point_to_compare_idx.path_point_index_lower_prio]);

            denominator = p_cals->k_pt_weight_of_last_trail_point + p_cals->k_pt_weight_of_sec_last_trail_point;

            p_path_obj_info->weighted_mean_diff_last_points = ((p_cals->k_pt_weight_of_last_trail_point * diff_high_prio)
                                                               + (p_cals->k_pt_weight_of_sec_last_trail_point * diff_low_prio))
                                                              / denominator;
         }
         else
         {
            /*Edge Case: closest_grid_point_idx enters not extrapolated path for first time. Check whether
             * points of path pair candidate are filled sufficiently*/
            p_path_obj_info->weighted_mean_diff_last_points = FBK_ZERO_F;
         }
      }
      else
      {
         /*In case that a path does not have a sufficient amount of points, this metric shall not be effective*/
         p_path_obj_info->weighted_mean_diff_last_points = FBK_ZERO_F;
      }
   }
}

static boolean_T Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(const Pt_Trail_Path_Comp_Indices_T *p_point_to_compare_idx,
                                                                       const Pt_Path_T *p_path)
{
   boolean_T f_calculate_non_default_weighted_dist = FBK_FALSE;
   boolean_T f_is_criteria_for_path_in_creation_fulfilled;
   boolean_T f_is_path_finished;

   /* Asserts */
   assert(NULL != p_point_to_compare_idx);
   assert(NULL != p_path);

   f_is_path_finished = (boolean_T) ((PATH_STATUS_CREATION != p_path->path_state) && (PATH_STATUS_DEFAULT != p_path->path_state));

   f_is_criteria_for_path_in_creation_fulfilled =
      (boolean_T) ((PATH_STATUS_CREATION == p_path->path_state)
                   && ((p_path->first_p <= p_point_to_compare_idx->path_point_index_higher_prio)
                       && (p_path->last_p >= p_point_to_compare_idx->path_point_index_higher_prio))
                   && ((p_path->first_p <= p_point_to_compare_idx->path_point_index_lower_prio)
                       && (p_path->last_p >= p_point_to_compare_idx->path_point_index_lower_prio)));

   if (f_is_path_finished || f_is_criteria_for_path_in_creation_fulfilled)
   {
      f_calculate_non_default_weighted_dist = FBK_TRUE;
   }

   return f_calculate_non_default_weighted_dist;
}

static void Pt_Get_Last_Two_Point_Idx_Of_Trail(Pt_Trail_Path_Comp_Indices_T *p_point_to_compare_idx, const Pt_Path_T *p_path)
{
   /* Asserts */
   assert(NULL != p_point_to_compare_idx);
   assert(NULL != p_path);

   if (Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(p_path))
   {
      p_point_to_compare_idx->path_point_index_higher_prio = p_path->first_p;
      p_point_to_compare_idx->path_point_index_lower_prio  = (uint8_t) (p_path->first_p + PT_SINGLE_GRID_POINT_OFFSET);
   }
   else
   {
      p_point_to_compare_idx->path_point_index_higher_prio = p_path->last_p;
      p_point_to_compare_idx->path_point_index_lower_prio  = (uint8_t) (p_path->last_p - PT_SINGLE_GRID_POINT_OFFSET);
   }
}

static void Pt_Calc_Confidence_Of_Current_Pair(Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                               const Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                               const Pt_Core_Calibration_T *p_cals,
                                               const uint8_t object_index,
                                               const uint8_t path_index)
{
   uint8_t index_of_val;
   float32_T saturated_val;

   /* Asserts */
   assert(NULL != p_path_obj_pair_confidence);
   assert(NULL != p_path_obj_info);
   assert(NULL != p_cals);

   /*Calculate confidence for the discrete distance between object and path border*/
   saturated_val =
      Enforce_Range((float32_T) p_path_obj_info->border_info.dist_border_to_obj, (float32_T) p_cals->k_pt_dist_obj_to_border_lut[0],
                    (float32_T) p_cals->k_pt_dist_obj_to_border_lut[PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
   index_of_val = Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(p_cals->k_pt_dist_obj_to_border_lut,
                                                     PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0, (uint8_t) saturated_val);
   p_path_obj_pair_confidence->dist_obj_to_border_confidence =
      Get_Y_Value_From_Line_By_Coordinates(((float32_T) p_cals->k_pt_dist_obj_to_border_lut[index_of_val - FBK_ONE_UINT]),
                                           p_cals->k_pt_dist_obj_to_border_conf_lut[index_of_val - FBK_ONE_UINT],
                                           ((float32_T) p_cals->k_pt_dist_obj_to_border_lut[index_of_val]),
                                           ((float32_T) p_cals->k_pt_dist_obj_to_border_conf_lut[index_of_val]), saturated_val);

   /*Calculate confidence for the discrete distance between intersection with coordinate axis and path border*/
   saturated_val = Enforce_Range(
      (float32_T) p_path_obj_info->border_info.dist_border_to_mid, (float32_T) p_cals->k_pt_dist_border_to_isect_lut[0],
      (float32_T) p_cals->k_pt_dist_border_to_isect_lut[PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
   index_of_val = Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(p_cals->k_pt_dist_border_to_isect_lut,
                                                     PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0, (uint8_t) saturated_val);
   p_path_obj_pair_confidence->dist_border_to_isect_confidence =
      Get_Y_Value_From_Line_By_Coordinates(((float32_T) p_cals->k_pt_dist_border_to_isect_lut[index_of_val - FBK_ONE_UINT]),
                                           p_cals->k_pt_dist_border_to_isect_conf_lut[index_of_val - FBK_ONE_UINT],
                                           ((float32_T) p_cals->k_pt_dist_border_to_isect_lut[index_of_val]),
                                           p_cals->k_pt_dist_border_to_isect_conf_lut[index_of_val], saturated_val);

   /*Calculate confidence for the absolute radial distance between center of object and its closest grid point*/
   saturated_val = Enforce_Range(Fbk_Abs_F(p_path_obj_info->relevant_dist_comp_matching), p_cals->k_pt_dist_obj_to_path_lut[0],
                                 p_cals->k_pt_dist_obj_to_path_lut[PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
   index_of_val  = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(p_cals->k_pt_dist_obj_to_path_lut,
                                                      PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0, saturated_val);
   p_path_obj_pair_confidence->dist_obj_to_path_confidence = Get_Y_Value_From_Line_By_Coordinates(
      p_cals->k_pt_dist_obj_to_path_lut[index_of_val - FBK_ONE_UINT],
      p_cals->k_pt_dist_obj_to_path_conf_lut[index_of_val - FBK_ONE_UINT], p_cals->k_pt_dist_obj_to_path_lut[index_of_val],
      p_cals->k_pt_dist_obj_to_path_conf_lut[index_of_val], saturated_val);

   /*Calculate confidence for the similarity between the last two points left behind by target and respective path candidate*/
   saturated_val =
      Enforce_Range(p_path_obj_info->weighted_mean_diff_last_points, p_cals->k_pt_similarity_trail_path_lut[0],
                    p_cals->k_pt_similarity_trail_path_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
   index_of_val = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(p_cals->k_pt_similarity_trail_path_lut,
                                                     PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0, saturated_val);
   p_path_obj_pair_confidence->similarity_trail_path_confidence =
      Get_Y_Value_From_Line_By_Coordinates(p_cals->k_pt_similarity_trail_path_lut[index_of_val - FBK_ONE_UINT],
                                           p_cals->k_pt_similarity_trail_path_conf_lut[index_of_val - FBK_ONE_UINT],
                                           p_cals->k_pt_similarity_trail_path_lut[index_of_val],
                                           p_cals->k_pt_similarity_trail_path_conf_lut[index_of_val], saturated_val);

   /*Calculate heading difference confidence*/
   p_path_obj_pair_confidence->heading_difference_confidence =
      Pt_Calculate_Heading_Diff_Confidence(p_path_obj_info, p_cals, object_index, path_index);

   /*Result confidence within range 0 to 1 where a path witch absolute confidence of 1 is the most trustworthy*/
   p_path_obj_pair_confidence->confidence_factor_total =
      p_path_obj_pair_confidence->dist_obj_to_border_confidence * p_path_obj_pair_confidence->dist_border_to_isect_confidence
      * p_path_obj_pair_confidence->dist_obj_to_path_confidence * p_path_obj_pair_confidence->similarity_trail_path_confidence
      * p_path_obj_pair_confidence->heading_difference_confidence;
}

static float32_T Pt_Calculate_Heading_Diff_Confidence(const Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info,
                                                      const Pt_Core_Calibration_T *p_cals,
                                                      /* clang-format off */
                                                      /* coverity[misra_c_2012_rule_2_7_violation][Object index is used here for debug purpose.] */
                                                      const uint8_t object_index,
                                                      /* coverity[misra_c_2012_rule_2_7_violation][Path index is used here for debug purpose.] */
                                                      const uint8_t path_index)
/* clang-format on */
{
   uint8_t index_of_val;
   uint8_t idx;
   float32_T absolute_heading_diff;
   float32_T saturated_val;
   float32_T heading_diff_segment_confidences[PT_NUM_HEADING_SEGMENTS];
   float32_T weigths_for_segments[PT_NUM_HEADING_SEGMENTS];
   float32_T heading_diff_confidence = FBK_ZERO_F;
   float32_T denumerator             = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_path_obj_pair_info);
   assert(NULL != p_cals);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);
   assert(path_index < PT_NUMBER_OF_PATHS);

   for (idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      absolute_heading_diff = Fbk_Abs_F(p_path_obj_pair_info->heading_diff_pt_segment[idx]);
      if (absolute_heading_diff > THRESHOLD_IS_ZERO)
      {
         /*Calculate the confidence for recorded path segments*/
         saturated_val = Enforce_Range(absolute_heading_diff, p_cals->k_pt_heading_diff_lut[0],
                                       p_cals->k_pt_heading_diff_lut[PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]);
         index_of_val = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(p_cals->k_pt_heading_diff_lut, PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0,
                                                           saturated_val);
         heading_diff_segment_confidences[idx] = Get_Y_Value_From_Line_By_Coordinates(
            p_cals->k_pt_heading_diff_lut[index_of_val - FBK_ONE_UINT],
            p_cals->k_pt_heading_diff_conf_lut[index_of_val - FBK_ONE_UINT], p_cals->k_pt_heading_diff_lut[index_of_val],
            p_cals->k_pt_heading_diff_conf_lut[index_of_val], saturated_val);
         weigths_for_segments[idx] = p_cals->k_pt_weight_heading_diff_confidence[idx];
      }
      else
      {
         /* In case that a path segment heading was not recorded, it shall not affect the weighting of the recorded path segment
       headings. Therefor the weigth to use is reset.*/
         heading_diff_segment_confidences[idx] = FBK_ZERO_F;
         weigths_for_segments[idx]             = FBK_ZERO_F;
      }
      /*Build up the denumerator for a weighted mean calculation of sub heading confidence metrics*/
      denumerator += weigths_for_segments[idx];
   }

   /*Calculate the heading diff confidence to return*/
   if (denumerator > THRESHOLD_IS_ZERO)
   {
      for (idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
      {
         heading_diff_confidence += (weigths_for_segments[idx] / denumerator) * heading_diff_segment_confidences[idx];
      }
   }
   else
   {
      heading_diff_confidence = FBK_ONE_F;
   }

   Binary_Pt_Debug_Pass_Heading_Diff_Segments_Metrics(p_path_obj_pair_info, heading_diff_segment_confidences, weigths_for_segments,
                                                      object_index, path_index);

   return heading_diff_confidence;
}

static void Pt_Pt_Init_Path_Obj_Pair_Info(Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info)
{
   uint8_t idx;

   /* Assert */
   assert(NULL != p_path_obj_pair_info);

   for (idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      p_path_obj_pair_info->heading_diff_pt_segment[idx] = FBK_ZERO_F;
   }

   p_path_obj_pair_info->border_info.dist_border_to_mid   = PT_DEFAULT_DISCR_BORDER;
   p_path_obj_pair_info->border_info.dist_border_to_obj   = PT_DEFAULT_DISCR_BORDER;
   p_path_obj_pair_info->rad_object_to_path_dist          = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_info->relevant_dist_comp_matching      = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_info->closest_to_successive_pt_heading = FBK_ZERO_F;
   p_path_obj_pair_info->weighted_mean_diff_last_points   = PT_HIGH_DISTANCE_DEFAULT_VAL;
}

static void Pt_Pt_Init_Path_Obj_Pair_Consumer_Info(Pt_Path_Obj_Pair_Consumer_Info_T *p_path_obj_pair_con_info)
{
   /* Assert */
   assert(NULL != p_path_obj_pair_con_info);

   p_path_obj_pair_con_info->path_index           = PT_DEFAULT_MATCH_INDEX;
   p_path_obj_pair_con_info->distance_to_path     = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_con_info->segment_heading_diff = FBK_ZERO_F;
}

static void Pt_Set_Path_Obj_Pair_Border_Info(Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info,
                                             const Pt_Path_T *p_path,
                                             const uint8_t next_point_idx,
                                             const Pt_Object_Mov_Direction_T obj_move_dir)
{
   /* Asserts */
   assert(NULL != p_path_obj_pair_info);
   assert(NULL != p_path);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   p_path_obj_pair_info->border_info.dist_border_to_mid = PT_DEFAULT_DISCR_BORDER;
   p_path_obj_pair_info->border_info.dist_border_to_obj = PT_DEFAULT_DISCR_BORDER;

   /*Calculate the index based distance between path border and mid as well as the distance between path border and object.*/
   if (Pt_Is_Obj_Mov_Dir_Against_Vcs(&(obj_move_dir)))
   {
      p_path_obj_pair_info->border_info.dist_border_to_mid = FBK_ZERO_UINT;
      p_path_obj_pair_info->border_info.dist_border_to_obj = FBK_ZERO_UINT;
      if (p_path->first_p >= PT_MID_GRID_POINT_INDEX)
      {
         p_path_obj_pair_info->border_info.dist_border_to_mid = (uint8_t) (p_path->first_p - PT_MID_GRID_POINT_INDEX);
      }
      if (next_point_idx >= p_path->last_p)
      {
         p_path_obj_pair_info->border_info.dist_border_to_obj = (uint8_t) (next_point_idx - p_path->last_p);
      }
   }
   else if (Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(&obj_move_dir))
   {
      p_path_obj_pair_info->border_info.dist_border_to_mid = FBK_ZERO_UINT;
      p_path_obj_pair_info->border_info.dist_border_to_obj = FBK_ZERO_UINT;
      if (p_path->last_p <= PT_MID_GRID_POINT_INDEX)
      {
         p_path_obj_pair_info->border_info.dist_border_to_mid = (uint8_t) (PT_MID_GRID_POINT_INDEX - p_path->last_p);
      }
      if (next_point_idx <= p_path->first_p)
      {
         p_path_obj_pair_info->border_info.dist_border_to_obj = (uint8_t) (p_path->first_p - next_point_idx);
      }
   }
   else
   {
      /*Do nothing*/
   }
}

static Pt_Object_Mov_Direction_T Pt_Determine_Object_Movement_Direction(const float32_T heading, const Pt_Core_Calibration_T *p_cals)
{
   Pt_Object_Mov_Direction_T obj_move_dir  = PT_OBJECT_MOV_DIR_NONE;
   Pt_Object_Orientation_T obj_orientation = Pt_Determine_Object_Orientation(heading, p_cals);

   if ((heading <= FBK_ZERO_F) || (heading >= PI))
   {
      if (PT_OBJECT_ORIENTATION_LONGITUDINAL == obj_orientation)
      {
         if (Fbk_Abs_F(heading) < Fbk_Half(PI))
         {
            obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
         }
         else
         {
            obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
         }
      }
      else
      {
         obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;
      }
   }

   if ((heading >= FBK_ZERO_F) && (heading <= PI))
   {
      if (PT_OBJECT_ORIENTATION_LONGITUDINAL == obj_orientation)
      {
         if (Fbk_Abs_F(heading) < Fbk_Half(PI))
         {
            obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
         }
         else
         {
            obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
         }
      }
      else
      {
         obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;
      }
   }

   return obj_move_dir;
}

static void Pt_Get_Next_Point_For_Object_To_Pass(Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx,
                                                 const Fbk_Object_Data_T *p_object,
                                                 const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   uint8_t i;
   float32_T obj_grid_component;

   /* Assert */
   assert(NULL != p_next_point_idx);
   assert(NULL != p_object);
   assert(NULL != grid_array);

   Pt_Init_Next_Point_Indices(p_next_point_idx);

   /*Search for the next longitudinal index and check afterwards whether the object is aligned with vcs in longitudinal direction.*/
   obj_grid_component = p_object->vcs_pos.x;
   for (i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      if (grid_array[i] < obj_grid_component)
      {
         p_next_point_idx->next_point_idx_long_path = i;
      }
      else
      {
         break;
      }
   }

   if (Fbk_Abs_F(p_object->vcs_heading) < Fbk_Half(PI))
   {
      p_next_point_idx->next_point_idx_long_path =
         (uint8_t) (p_next_point_idx->next_point_idx_long_path + PT_SINGLE_GRID_POINT_OFFSET);
   }

   /*Search for the next lateral index and check afterwards whether the object is aligned with vcs in lateral direction.*/
   obj_grid_component = p_object->vcs_pos.y;
   for (i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      if (grid_array[i] < obj_grid_component)
      {
         p_next_point_idx->next_point_idx_lat_path = i;
      }
      else
      {
         break;
      }
   }

   if (p_object->vcs_heading > FBK_ZERO_F)
   {
      p_next_point_idx->next_point_idx_lat_path = (uint8_t) (p_next_point_idx->next_point_idx_lat_path + PT_SINGLE_GRID_POINT_OFFSET);
   }
}

static boolean_T Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(const uint8_t next_point_idx,
                                                     const Pt_Path_T *p_path,
                                                     const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals)
{
   uint8_t lower_border;
   uint8_t upper_border;
   uint8_t dist_first_p = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_num_grid_pts_dep_cals);

   if (p_path->first_p >= p_num_grid_pts_dep_cals->k_pt_find_max_diff_path_points)
   {
      dist_first_p = (uint8_t) (p_path->first_p - p_num_grid_pts_dep_cals->k_pt_find_max_diff_path_points);
   }

   lower_border = Max((PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET), dist_first_p);
   upper_border = Min((uint8_t) (p_path->last_p + p_num_grid_pts_dep_cals->k_pt_find_max_diff_path_points),
                      (PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET));

   /*It shall be allowed at this point to use */
   return (boolean_T) ((lower_border <= next_point_idx) && (next_point_idx <= upper_border)
                       && (FBK_ZERO_UINT == p_path->obj_curr_used_for_path_build.id));
}

static boolean_T Pt_Is_Idx_In_Path_Borders(const uint8_t next_point_idx, const Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   /*Borders shall be handled exclusively at this point, to ensure access to defined memory of path points in submodule */
   return (boolean_T) ((FBK_ZERO_UINT != p_path->obj_curr_used_for_path_build.id)
                       && ((p_path->first_p < next_point_idx) && (next_point_idx < p_path->last_p)));
}

static boolean_T Pt_Is_Path_Valid_For_Object_Matching(const Pt_Path_T *p_path,
                                                      const Fbk_Object_Data_T *p_object,
                                                      const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_path_is_valid_for_object_matching = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_object);
   assert(NULL != p_cals);

   if ((PATH_STATUS_DEFAULT != p_path->path_state) && (p_path->obj_curr_used_for_path_build.id != (p_object->id))
       && ((p_path->last_p - p_path->first_p) > p_cals->k_pt_find_min_diff_path_points))
   {
      f_path_is_valid_for_object_matching = FBK_TRUE;
   }

   return f_path_is_valid_for_object_matching;
}

static boolean_T Pt_Is_Next_Point_Contained_In_Valid_Path_Borders(const uint8_t next_point_idx,
                                                                  const Pt_Path_T *p_path,
                                                                  const Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals)
{
   boolean_T f_point_is_in_valid_path_borders = FBK_FALSE;

   if ((Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(next_point_idx, p_path, p_num_grid_pts_dep_cals))
       || (Pt_Is_Idx_In_Path_Borders(next_point_idx, p_path)))
   {
      f_point_is_in_valid_path_borders = FBK_TRUE;
   }

   return f_point_is_in_valid_path_borders;
}

static boolean_T Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(const Pt_Path_T *p_path,
                                                                        const Pt_Object_Mov_Direction_T obj_move_dir)
{
   return (boolean_T) (((Pt_Is_Path_Longitudinal(p_path)) && (Pt_Is_Obj_Moving_Longitudinal(&(obj_move_dir))))
                       || ((Pt_Is_Path_Lateral(p_path)) && (Pt_Is_Obj_Moving_Lateral(&(obj_move_dir)))));
}

static boolean_T Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(const Pt_Object_Mov_Direction_T obj_move_dir,
                                                                  const Pt_Core_Calibration_T *p_cals,
                                                                  const Pt_Input_T *p_pt_input,
                                                                  const Fbk_Object_Data_T *p_object)
{
   boolean_T f_obj_out_of_track_range;
   boolean_T f_obj_is_invalid_for_being_matched_to_path;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_object);

   f_obj_out_of_track_range = Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range(&p_object->vcs_pos, obj_move_dir, p_pt_input);

   f_obj_is_invalid_for_being_matched_to_path =
      (boolean_T) (f_obj_out_of_track_range || (PA_OBJ_STATUS_INVALID == p_object->status) || (PT_OBJECT_MOV_DIR_NONE == obj_move_dir)
                   || (PA_OBJ_STATUS_NEW == p_object->status) || (p_object->speed < p_cals->k_pt_find_min_speed));

   return f_obj_is_invalid_for_being_matched_to_path;
}

static boolean_T Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range(const Vector_2d_T *p_obj_pos,
                                                          const Pt_Object_Mov_Direction_T obj_move_dir,
                                                          const Pt_Input_T *p_pt_input)
{
   float32_T dist_thres;
   float32_T obj_pos_component;

   /* Asserts */
   assert(NULL != p_obj_pos);
   assert(NULL != p_pt_input);

   if (Pt_Is_Obj_Moving_Longitudinal(&(obj_move_dir)))
   {
      dist_thres        = p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_long_posn;
      obj_pos_component = p_obj_pos->x;
   }
   else if (Pt_Is_Obj_Moving_Lateral(&(obj_move_dir)))
   {
      dist_thres        = p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn;
      obj_pos_component = p_obj_pos->y;
   }
   else
   {
      dist_thres        = FBK_ZERO_F;
      obj_pos_component = PT_HIGH_DISTANCE_DEFAULT_VAL;
   }

   return (boolean_T) ((Fbk_Abs_F(obj_pos_component) >= dist_thres)
                       || ((obj_pos_component <= p_pt_input->grid_pt_array[PT_LOWEST_GRID_POINT_INDEX])
                           || (obj_pos_component >= p_pt_input->grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX])));
}

static void Pt_Init_Next_Point_Indices(Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx)
{
   /* Assert */
   assert(NULL != p_next_point_idx);

   p_next_point_idx->next_point_idx_lat_path  = PT_INVALID_GRID_POINT_INDEX;
   p_next_point_idx->next_point_idx_long_path = PT_INVALID_GRID_POINT_INDEX;
}

static void Pt_Pt_Init_Path_Obj_Pair_Confidence(Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence)
{
   /* Assert */
   assert(NULL != p_path_obj_pair_confidence);

   p_path_obj_pair_confidence->confidence_factor_total          = FBK_ZERO_F;
   p_path_obj_pair_confidence->dist_border_to_isect_confidence  = FBK_ZERO_F;
   p_path_obj_pair_confidence->dist_obj_to_border_confidence    = FBK_ZERO_F;
   p_path_obj_pair_confidence->dist_obj_to_path_confidence      = FBK_ZERO_F;
   p_path_obj_pair_confidence->similarity_trail_path_confidence = FBK_ZERO_F;
   p_path_obj_pair_confidence->heading_difference_confidence    = FBK_ZERO_F;
}

static uint8_t Pt_Get_Path_Index(const Pt_Path_T paths[PT_NUMBER_OF_PATHS], const Fbk_Object_Data_T *p_fbk_object_data)
{
   uint8_t path_index = PA_INVALID_OBJ_INDEX;
   uint8_t i;

   /* Asserts */
   assert(NULL != paths);
   assert(NULL != p_fbk_object_data);

   if (FBK_ZERO_UINT != p_fbk_object_data->id)
   {
      for (i = 0; i < PT_NUMBER_OF_PATHS; i++)
      {
         if ((p_fbk_object_data->id) == paths[i].obj_curr_used_for_path_build.id)
         {
            path_index = i;
            break;
         }
      }
   }

   return path_index;
}
