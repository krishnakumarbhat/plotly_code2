/**
 * @file pt.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Path tracking main functions for path creations.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_saturated_math.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_common_functions.h"
#include "pt_constants.h"
#include "pt_debug_interface.h"
#include "pt_directions.h"
#include "pt_group_paths.h"
#include "pt_host_lane.h"
#include "pt_object_matching.h"
#include "pt_output_t.h"
#include "pt_path_rotation.h"
#include "pt_persistent_handler.h"
#include "pt_reset.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define PT_TEMP_GROUPING_AMOUNT_PATH_PRIORITY (UINT8_MAX)
#define PT_TEMP_DISTANCE_TO_HOST_PATH_PRIORITY (FBK_ZERO_F)
#define PT_TEMP_LENGTH_OF_PATH_HOST_PATH_PRIORITY (PT_NUM_GRID_POINTS)

/**
 * @brief Summarizes priority metrics for paths. In case that all slots of path tracking are filled, one path needs to be
 * destroyed. This is the one with the least amount of grouping. When multiple paths have the same least amount of groupings, the
 * shorter path will be chosen. If this is also equal, the path with the highest distance to the host will be deleted.
 */
typedef struct
{
   uint8_t temp_amount_path_grouping; /**<grouping metric used for reset decision*/
   uint8_t temp_path_length;          /**<path length metric used for reset decision*/
   uint8_t path_slot_to_liberate;     /**<index of the path to reset */
   float32_T temp_dist_to_host;       /**<distance between host an mid grid point index metric used for reset decision*/
} Pt_Path_Priority_Metrics_T;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/
/**
 * @brief Represents the first great module of path tracking algorithm.
 * Here and within subfunctions paths are created, updated and processed.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7318}
 * @verification{}
 */
static void Pt_Record_Paths(Pt_Persistent_T *p_pt_persistent /**< persistent information of PT*/,
                            const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                            const Pt_Input_T *p_pt_input /**< input for PT*/,
                            const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data*/);

/**
 * This function calls the main step procedures needed for the path algorithm.
 * At first the cal values which depend on the absolute num of grid points are adapted to the
 * grid point array range which is considered. Then Paths are created within Path_Track. And at
 * the end objects are matched to those paths within the Pt_Objects_To_Path_Matching function.
 *
 * @return     void
 *
 * @SRS{SF-1522,SF-1520,SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7406}
 * @verification{}
 *
 */
static void Pt_Run_Path_Tracking(Pt_Output_T *p_pt_output /**< output for path tracking algorithm*/,
                                 Pt_Persistent_T *p_pt_persistent /**<persistent values of path algorithm*/,
                                 const Pt_Input_T *p_pt_input /**<input for path tracking algorithm*/,
                                 const Pt_Core_Calibration_T *p_cals /**<calibration parameters*/,
                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief Resets paths which exceed a defined distance threshold
 * in relation to the ego vehicle.
 *
 * @return void
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7307}
 * @verification{}
 */
static void
Pt_Reset_Far_Field_Path(Pt_Path_T *p_path /**< all paths*/,
                        Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< matching pairs*/,
                        const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief Derives the path priority, resets the path with the worst priority and returns
 * the index of the respective path.
 * The higher the value of path_priority the worse the really priority is.
 * The priority consists of a weight between age, num of path points and the distance to the ego
 * at the PT_MID_GRID_POINT_INDEX
 *
 * @return index of the path, which has the least priority and which is deleted in order for creation of another path at this
 * index.
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7305}
 * @verification{}
 */
static uint8_t Pt_Make_Path_Room(Pt_Persistent_T *p_pt_persistent /**< persistent path data*/);

/**
 * @brief Initializes the path priority metrics
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7300}
 * @verification{Check whether the initialization is fulfilled correctly.}
 */
static void Pt_Pt_Init_Path_Priority_Metric(Pt_Path_Priority_Metrics_T *p_path_priority_metric /**< path priority metric*/);

/**
 * @brief Sets the path priority metric structure to the given function inputs
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7309}
 * @verification{Check whether the mapping is done correctly.}
 */
static void Pt_Set_Path_Priority_Metric(Pt_Path_Priority_Metrics_T *p_path_priority_metric /**< path priority metric*/,
                                        const Pt_Path_T *p_path /**< input path */,
                                        const uint8_t path_idx_to_liberate /**< path index to free */);

/**
 * @brief Searches an empty slot for a new path to create and returns the
 * index of the slot in order create a path there.
 * If every slot is used then the function Pt_Make_Path_Room is called.
 *
 * @return first index within the paths array which is not yet used for a path or resolved for a new path.
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7295}
 * @verification{}
 */
static uint8_t Pt_Find_Available_Path_Slot(Pt_Persistent_T *p_pt_persistent /**< persistent data*/);

/**
 * @brief Calls procedures which are needed for
 * the paths to be kept. For example rotation, lane change detection and grouping
 * of paths. Also third and fourth lane paths are filtered here.
 *
 * @return void
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7292}
 * @verification{}
 */
static void Pt_Book_Keep_Paths(Pt_Persistent_T *p_pt_persistent /**<persistent path tracking data*/,
                               const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                               const Pt_Input_T *p_pt_input /**< path tracking input*/,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**<vehicle data*/);

/**
 * @brief Initializes a path with default values.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7299}
 * @verification{}
 */
static void Pt_Init_Path(const uint8_t available_path_idx /**<path index where a new path is initialized*/,
                         Pt_Path_T *p_path /**< path which gets initialized*/,
                         const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/);

/**
 * @brief Adds a new point to an existing path which may be lateral or longitudinal.
 * Side to where the point is added depends on path direction.
 * Since spartial sampling is not arbitrarily high, extrapolation is needed.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7311}
 * @verification{}
 */
static void Pt_Track_Path_Wrapper(Pt_Path_T *p_path /**< respective longitudinal path*/,
                                  const uint8_t path_point_index /**<index of the point to be added*/,
                                  const float32_T *p_path_first_grid_comp /**<  grid component of first property*/,
                                  const float32_T *p_path_first_point_comp /**<  path point component of first property*/,
                                  const float32_T *p_path_last_grid_comp /**<  grid component of last property*/,
                                  const float32_T *p_path_last_point_comp /**<  path point component of last property*/,
                                  const float32_T *p_obj_vcs_grid_comp /**<  grid component of object in vcs*/,
                                  const float32_T *p_obj_vcs_point_comp /**<  path point component of object in vcs*/,
                                  const Pt_Input_T *p_pt_input /**< Pt input*/,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief Adds a new point to an existing path which may be lateral or longitudinal.
 * Side to where the point is added depends on path direction.
 * Since spartial sampling is not arbitrarily high, extrapolation is needed.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7314}
 * @verification{}
 */
static void Pt_Update_Path_Borders(Pt_Path_T *p_path /**< respective path whose borders (defined by object) shall be updated*/,
                                   const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/);

/**
 * @brief Updates maximum path speed for a specific path, if the
 * object which is used to create that path exceeds the previous maximum speed.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7312}
 * @verification{}
 */
static void Pt_Update_Maximum_Path_Speed(Pt_Path_T *p_path /**< respective path whose max speed shall be updated*/,
                                         const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/);

/**
 * @brief Derives the path direction from the object direction and relationship
 * of path boundaries. At the end the attributes of the path are aligned to ego coordinate system.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7293}
 * @verification{}
 */
static void Pt_Calculate_Path_Dir(Pt_Path_T *p_path /**< respective path whose direction shall be computed*/,
                                  const Pt_Object_Orientation_T *p_obj_orientation /**<orientation of the object*/);

/**
 * @brief Extrapolates incomplete paths. Incomplete means here that the path was
 * not physically measured on every grid point index with help of a target and that this target is not
 * tracked anymore. Paths can get extrapolated if they consist of a minimum amount of path points otherwise they get reset.
 *
 * @return void
 *
 * @SRS{SF-1567}
 * @SAE{SF-2918}
 * @SDD{SF-7306}
 * @verification{}
 */
static void Pt_Process_Incomplete_Path(Pt_Path_T *p_path /**< path information*/,
                                       Pt_Persistent_T *p_pt_persistent /**< persistent data for PT */,
                                       const Pt_Input_T *p_pt_input /**< path tracking input*/,
                                       const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data*/);

/**
 * @brief Updates the age of object_used_for_path struct within paths.
 *  This is used for validity checks
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7313}
 * @verification{}
 */
static void Pt_Update_Object_Used_For_Path_Age(Pt_Path_T *p_path /**<path whose age needs to be updated.*/,
                                               const Pt_Input_T *p_pt_input /**< path tracking input*/,
                                               const uint8_t obj_index /**<object index*/);

/**
 * @brief Returns the value axis relevant coordinate if boundaries, since those
 * are aligned with the ego coordinate system.
 *
 * @return last path point of the considered path
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7296}
 * @verification{}
 */
static float32_T Pt_Get_Last_Path_Point(const Pt_Path_T *p_path /**< respective path point*/,
                                        const Pt_Object_Orientation_T obj_orientation /**<object orientation*/);

/**
 * @brief Returns the for the grid point array relevant position of the
 * target vehicle
 *
 * @return relevant object component which is needed for comparisons with grid point coordinate
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7297}
 * @verification{}
 */
static float32_T Pt_Get_Path_Relevant_Obj_Pos_Component(const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/,
                                                        const Pt_Object_Orientation_T obj_orientation /**<object orientation*/);

/**
 * @brief Checks whether basic conditions for an adding of a path point is given.
 *
 * @return True when last objects position and the previously added path boundary are surrounding a grid array value.
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7310}
 * @verification{Check whether true is only returned when the current object position and the last measured path boundary are
 * surrounding a grid array component.}
 */
static boolean_T
Pt_Shall_Path_Point_Be_Added(const float32_T *p_grid_array_comp /**< grid array component*/,
                             const float32_T *p_relevant_obj_pos_component /**< relevant object position component*/,
                             const float32_T *p_last_path_point /**< last measured path boundary*/);

/**
 * @brief Updates the path points of the respective path and represents a wrapper for the
 * respective path directions in case that conditions for an adding of path points is given..
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7315}
 * @verification{Check that depending on the path direction or object orientation, in case that no direction is given, the right
 * path border is extrapolated and extended.}
 */
static void Pt_Update_Path_Points(Pt_Path_T *p_path /**< respective path*/,
                                  const Fbk_Object_Data_T *p_fbk_object_data /**<object data*/,
                                  const Pt_Object_Orientation_T obj_orientation /**<object orientation*/,
                                  const Pt_Input_T *p_pt_input,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Represents a plausibility check. Path is reset if the number of
 * path points differ from the entries within path_points unequal to zero.
 *
 * @return void
 *
 * @SRS{SF-1575}
 * @SAE{SF-2918}
 * @SDD{SF-7308}
 * @verification{}
 */
static void
Pt_Reset_Implausible_Path(Pt_Path_T *p_path /**< respective path*/,
                          Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS] /**< matching pairs*/,
                          const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief Checks whether the extrapolated path is plausible based on its extrapolated path part.
 *
 * @return True in case that the path part near the host is extrapolated and crossing the host vehicle.
 *
 * @SRS{SF-1567}
 * @SAE{}
 * @SDD{SF-7585}
 * @verification{}
 */
static boolean_T Pt_Is_Extrapolated_Path_Part_Implausible(const Pt_Path_T *p_path /**< path information*/,
                                                          const Pt_Core_Calibration_T *p_cals /**<path tracking calibration*/,
                                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< vehicle data */);

/**
 * @brief Checks if an object is out of the path tracking zone.
 * If this is the case then path attribute updates are prevented.
 *
 * @return True if the considered object is out of tracking range
 *
 * @SRS{SF-1526,SF-1525,SF-1527,SF-1528}
 * @SAE{SF-2918}
 * @SDD{SF-7304}
 * @verification{}
 */
static boolean_T Pt_Is_Object_Out_Of_Tracking_Range(const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/,
                                                    const Pt_Core_Calibration_T *p_cals /**<path tracking calibration*/);

/**
 * @brief Checks whether an object in coasted mode is became invalid for building up a path.
 *
 * @return True when the object comes to standstill or when the object drives out of the path tracking range.
 *
 * @SRS{SF-1526,SF-1525}
 * @SAE{SF-2918}
 * @SDD{SF-7302}
 * @verification{}
 */
static boolean_T
Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/,
                                               const Pt_Core_Calibration_T *p_cals /**<path tracking calibration*/);

/**
 * @brief Checks whether path object pair fulfills one of the properties to abort the creation process.
 *
 * @return True when the object comes to standstill or when the object drives out of the path tracking range.
 *
 * @SRS{SF-1526,SF-1525,SF-1527,SF-1528}
 * @SAE{SF-2918}
 * @SDD{SF-7298}
 * @verification{}
 */
static boolean_T
Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(Pt_Path_T *p_path /**< path data */,
                                            Pt_Persistent_T *p_pt_persistent /**<persistent data */,
                                            const Pt_Input_T *p_pt_input /**<Path tracking input*/,
                                            const Pt_Object_Orientation_T *p_obj_orientation /**<object orientation*/,
                                            const Fbk_Object_Data_T *p_fbk_object_data /**< object data*/,
                                            const Pt_Core_Calibration_T *p_cals /**< calibration data*/,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data /**<vehicle data*/);

/**
 * @brief aligns the attributes of the path to ego coordinate system.
 *
 * @return void
 *
 * @SRS{SF-1554}
 * @SAE{SF-2918}
 * @SDD{SF-7294}
 * @verification{}
 */
static void Pt_Check_Direction_Integrity(Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief checks whether the considered object is valid for path creation.
 *
 * @return True if object is valid for path creation
 *
 * @SRS{SF-1556,SF-1555,SF-1557,SF-1558}
 * @SAE{SF-2918}
 * @SDD{SF-7303}
 * @verification{Verify that the object is only valid for path creation when it is not a reflection, when it is fast enough and
 * when it has a sufficient existence probability.}
 */
static boolean_T Pt_Is_Obj_Valid_For_Path_Creation(const Fbk_Object_Data_T *p_fbk_object_data /**< object data */,
                                                   const Pt_Core_Calibration_T *p_cals /**< pt calibrations*/);

/**
 * @brief Checks whether the object is implausible based on its age and the
 * age of the object used to build given path at the last time.
 *
 * @return True when obj has been switched, thus has same ID but shows
 *         non expected behaviour
 *
 * @SRS{SF-1526}
 * @SAE{SF-2918}
 * @SDD{SF-7301}
 * @verification{}
 */
static boolean_T Pt_Is_Associated_Obj_Implausible(const Fbk_Object_Data_T *p_fbk_object_data /**< object used to generate a path*/,
                                                  const Pt_Path_T *p_path /**< path supposed to be generated by input object*/,
                                                  const Pt_Core_Calibration_T *p_cals /**< calibration parameters*/);

/**
 * @brief This function returns a path by abstraction layer without any usage of fbk object.
 *        This should prevent filling the fbk object even when it is not used by PT.
 *
 * @return pointer to path which is created by by the given object
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7641}
 * @verification{}
 */
static Pt_Path_T *Pt_Get_Path_By_Abstraction_Layer(Pt_Persistent_T *p_pt_persistent /**<persistent data of pt*/,
                                                   const Pt_Input_T *p_pt_input /**<persistent data of pt*/,
                                                   const uint8_t obj_index /**<object index*/);

/*===========================================================================*\
* Global Functions	Definition
\*===========================================================================*/

void Pt_Core_Run(Pt_Output_T *p_pt_output,
                 Pt_Persistent_T *p_pt_persistent,
                 const Pt_Input_T *p_pt_input,
                 const Pt_Core_Calibration_T *p_cals)
{
   Fbk_Vehicle_Data_T vehicle_data;

   /* Asserts */
   assert(NULL != p_pt_output);
   assert(NULL != p_pt_persistent);
   assert(NULL != p_pt_input);
   assert(NULL != p_cals);

   /* Reset debug data */
   Binary_Pt_Debug_Reset_Data();

   vehicle_data = p_pt_input->p_fbk_output->p_pa_data->vehicle_data;
   Pt_Reset_Path_Output(p_pt_output);

   if (Fbk_Is_False(p_pt_persistent->f_was_pt_executed))
   {
      /* When Path Tracking was not running before, it shall only run when the host is moving slowly. */
      if (vehicle_data.host_speed < p_cals->k_pt_en_algo_min_vel_inactive)
      {
         Pt_Run_Path_Tracking(p_pt_output, p_pt_persistent, p_pt_input, p_cals, &vehicle_data);
      }
   }
   else
   {
      /* When Path Tracking was running before, it shall only continue to run, when host is not moving too fast. */
      if (vehicle_data.host_speed > p_cals->k_pt_en_algo_max_val_active)
      {
         /* Initialize Reset persistent pt data. */
         Pt_Reset_All_Paths(p_pt_persistent->paths, p_pt_output, p_pt_persistent->best_path_obj_pairs);
         Pt_Reset_Persistent(p_pt_persistent);
         Pt_Reset_All_Matching_Pairs(p_pt_persistent->best_path_obj_pairs);
      }
      else
      {
         /* Check if trail information is available and can be used to create a new host path. */
         Pt_Transform_Trail_To_Path(p_pt_persistent, p_cals, p_pt_input, &vehicle_data, p_pt_input->p_fbk_output->p_host_trail);

         /* Run Path Tracking algorithm from within extended velocity range. */
         Pt_Run_Path_Tracking(p_pt_output, p_pt_persistent, p_pt_input, p_cals, &vehicle_data);
      }
   }

   /* Pass general data to debug data */
   Binary_Pt_Debug_Pass_General_Data(p_pt_input, p_pt_output, p_pt_persistent, p_cals);
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static void Pt_Record_Paths(Pt_Persistent_T *p_pt_persistent,
                            const Pt_Core_Calibration_T *p_cals,
                            const Pt_Input_T *p_pt_input,
                            const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t obj_index;
   Pt_Object_T pt_object;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);

   for (obj_index = FBK_ZERO_UINT; obj_index < PA_OBJ_NUMBER_OF_OBJECTS; obj_index++)
   {
      Pt_Path_T *p_path;
      p_path = Pt_Get_Path_By_Abstraction_Layer(p_pt_persistent, p_pt_input, obj_index);
      if (p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index].f_moveable)
      {
         switch (p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index].status)
         {
            case PA_OBJ_STATUS_INVALID:
            case PA_OBJ_STATUS_NEW:

               if (NULL != p_path)
               {
                  /* Finish a path with extrapolation in case of the track status turning
                  to invalid or when the id is reused in the next cycle*/
                  Pt_Process_Incomplete_Path(p_path, p_pt_persistent, p_pt_input, p_cals, p_vehicle_data);
               }
               break;

            case PA_OBJ_STATUS_MATURE:

               /* Fill object information */
               pt_object.tracker_data = p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index];

               /*Checks object properties whether the object is valid for building up a path*/
               if (Pt_Is_Obj_Valid_For_Path_Creation(&pt_object.tracker_data, p_cals))
               {
                  if (NULL != p_path)
                  {
                     Pt_Object_Orientation_T obj_orientation;

                     /*Check whether the object orientation changes from lateral to longitudinal or vice versa.
                       Depending on the path range path information can then be used afterwards.*/
                     obj_orientation = Pt_Determine_Object_Orientation(pt_object.tracker_data.vcs_heading, p_cals);
                     Pt_Calculate_Path_Dir(p_path, &obj_orientation);

                     if (!(Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(p_path, p_pt_persistent, p_pt_input, &obj_orientation,
                                                                       &pt_object.tracker_data, p_cals, p_vehicle_data)))
                     {
                        /* Updates properties of a given path*/
                        p_path->new_path_point_status = PATH_POINT_NEW_MATURE;
                        Pt_Update_Path_Points(p_path, &pt_object.tracker_data, obj_orientation, p_pt_input, p_vehicle_data);
                        Pt_Update_Maximum_Path_Speed(p_path, &pt_object.tracker_data);
                        Pt_Update_Path_Borders(p_path, &pt_object.tracker_data);
                     }
                  }
                  else
                  {
                     /*Object has not started to create a path, thus a path index needs to be found for the new creation process.*/
                     uint8_t available_path_idx;
                     /* Find available path slot and initialize path*/
                     available_path_idx = Pt_Find_Available_Path_Slot(p_pt_persistent);
                     Pt_Init_Path(available_path_idx, &p_pt_persistent->paths[available_path_idx], &pt_object.tracker_data);
                  }
               }
               else
               {
                  /* In case that an object became invalid after path creation, it shall complete its path*/
                  if (NULL != p_path)
                  {
                     Pt_Process_Incomplete_Path(p_path, p_pt_persistent, p_pt_input, p_cals, p_vehicle_data);
                  }
               }
               break;
            case PA_OBJ_STATUS_COASTED:

               /* Fill object information */
               pt_object.tracker_data = p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index];

               /* Complete paths of coasted objects that are out of range or below the min speed */
               if (Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(&pt_object.tracker_data, p_cals))
               {
                  if (NULL != p_path)
                  {
                     Pt_Process_Incomplete_Path(p_path, p_pt_persistent, p_pt_input, p_cals, p_vehicle_data);
                  }
               }
               break;
            default:
               /* this should not happen. When this happens, the abstraction layer might be wrongly initialized*/
               assert(FBK_FALSE && "object.status is undefined");
               break;
         }
      }


      /* Update object age used for path building management */
      if (NULL != p_path)
      {
         Pt_Update_Object_Used_For_Path_Age(p_path, p_pt_input, obj_index);
      }
   }

   /*Rotate and group paths*/
   Pt_Book_Keep_Paths(p_pt_persistent, p_cals, p_pt_input, p_vehicle_data);

   /* Write output path struct*/
   Binary_Pt_Debug_Pass_Path_Data(p_pt_persistent->paths);
}

static void Pt_Reset_Far_Field_Path(Pt_Path_T *p_path,
                                    Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                    const Pt_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != best_path_object_pairs);
   assert(NULL != p_cals);

   if ((Pt_Is_Object_Tracking_This_Path_Already_Unassigned(p_path))
       && (Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX]) > p_cals->k_pt_kill_path_exceed_dist_thres)
       && (Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]
                     - p_path->path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET])
           < p_cals->k_pt_kill_path_max_diff_posn))
   {
      Pt_Reset_Path_And_Associations_To_It(p_path, best_path_object_pairs, PATH_RESET_FAR_FIELD_PATH);
   }
}

static uint8_t Pt_Make_Path_Room(Pt_Persistent_T *p_pt_persistent)
{
   uint8_t i;
   Pt_Path_Priority_Metrics_T path_priority_metric;

   /* Asserts */
   assert(NULL != p_pt_persistent);

   Pt_Pt_Init_Path_Priority_Metric(&path_priority_metric);

   /* ensure kill one path with lowest priority */
   for (i = FBK_ZERO_UINT; i < PT_NUMBER_OF_PATHS; i++)
   {
      if (Pt_Is_Object_Tracking_This_Path_Already_Unassigned(&p_pt_persistent->paths[i]))
      {
         if (p_pt_persistent->paths[i].num_groupings < path_priority_metric.temp_amount_path_grouping)
         {
            Pt_Set_Path_Priority_Metric(&path_priority_metric, &p_pt_persistent->paths[i], i);
         }
         else if (p_pt_persistent->paths[i].num_groupings == path_priority_metric.temp_amount_path_grouping)
         {
            uint8_t length_of_path = Pt_Get_Number_Of_Path_Points(&p_pt_persistent->paths[i]);
            if (length_of_path < path_priority_metric.temp_path_length)
            {
               Pt_Set_Path_Priority_Metric(&path_priority_metric, &p_pt_persistent->paths[i], i);
            }
            else if (length_of_path == path_priority_metric.temp_path_length)
            {
               if (Fbk_Abs_F(p_pt_persistent->paths[i].path_points[PT_MID_GRID_POINT_INDEX]) > path_priority_metric.temp_dist_to_host)
               {
                  Pt_Set_Path_Priority_Metric(&path_priority_metric, &p_pt_persistent->paths[i], i);
               }
            }
            else
            {
               /* Do nothing */
            }
         }
         else
         {
            /* Do nothing */
         }
      }
   }

   if (PT_DEFAULT_MATCH_INDEX != path_priority_metric.path_slot_to_liberate)
   {
      Pt_Reset_Path_And_Associations_To_It(&p_pt_persistent->paths[path_priority_metric.path_slot_to_liberate],
                                           p_pt_persistent->best_path_obj_pairs, PATH_RESET_MAKE_PATH_ROOM);
   }

   return path_priority_metric.path_slot_to_liberate;
}

static void Pt_Run_Path_Tracking(Pt_Output_T *p_pt_output,
                                 Pt_Persistent_T *p_pt_persistent,
                                 const Pt_Input_T *p_pt_input,
                                 const Pt_Core_Calibration_T *p_cals,
                                 const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Assert */
   assert(NULL != p_pt_output);
   assert(NULL != p_pt_persistent);
   assert(NULL != p_pt_input);
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);

   /*Main algorithm*/
   Pt_Record_Paths(p_pt_persistent, p_cals, p_pt_input, p_vehicle_data);
   Pt_Objects_To_Path_Matching(p_pt_persistent, p_pt_output, p_cals, p_pt_input, p_vehicle_data);

   /*Set operational flag to true*/
   p_pt_output->f_pt_operational = FBK_TRUE;

   /*Updating persistent variables*/
   Pt_Update_Persistent(p_pt_persistent);
}

static void Pt_Set_Path_Priority_Metric(Pt_Path_Priority_Metrics_T *p_path_priority_metric,
                                        const Pt_Path_T *p_path,
                                        const uint8_t path_idx_to_liberate)
{
   /* Asserts */
   assert(NULL != p_path_priority_metric);
   assert(NULL != p_path);

   p_path_priority_metric->path_slot_to_liberate     = path_idx_to_liberate;
   p_path_priority_metric->temp_amount_path_grouping = p_path->num_groupings;
   p_path_priority_metric->temp_dist_to_host         = Fbk_Abs_F(p_path->path_points[PT_MID_GRID_POINT_INDEX]);
   p_path_priority_metric->temp_path_length          = Pt_Get_Number_Of_Path_Points(p_path);
}

static void Pt_Pt_Init_Path_Priority_Metric(Pt_Path_Priority_Metrics_T *p_path_priority_metric)
{
   /* Assert */
   assert(NULL != p_path_priority_metric);

   p_path_priority_metric->path_slot_to_liberate     = PT_DEFAULT_MATCH_INDEX;
   p_path_priority_metric->temp_amount_path_grouping = PT_TEMP_GROUPING_AMOUNT_PATH_PRIORITY;
   p_path_priority_metric->temp_dist_to_host         = PT_TEMP_DISTANCE_TO_HOST_PATH_PRIORITY;
   p_path_priority_metric->temp_path_length          = PT_TEMP_LENGTH_OF_PATH_HOST_PATH_PRIORITY;
}

static uint8_t Pt_Find_Available_Path_Slot(Pt_Persistent_T *p_pt_persistent)
{
   uint8_t path_idx;
   boolean_T f_empty_path_slot_found;

   /* Asserts */
   assert(NULL != p_pt_persistent);

   f_empty_path_slot_found = Pt_Search_Empty_Path_Slot(&path_idx, p_pt_persistent->paths);

   /* In case that no empty path slot has been found, a low priority path shall be removed */
   if (Fbk_Is_False(f_empty_path_slot_found))
   {
      path_idx = Pt_Make_Path_Room(p_pt_persistent);
   }
   return path_idx;
}

static void Pt_Book_Keep_Paths(Pt_Persistent_T *p_pt_persistent,
                               const Pt_Core_Calibration_T *p_cals,
                               const Pt_Input_T *p_pt_input,
                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   uint8_t i;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_pt_input);
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);

   /* introduce calibration to switch off function move points. Affects adapt intersecton point based on steering angle */
   if ((Fbk_Is_True(p_cals->k_pt_f_apply_move_point))
       && ((Fbk_Abs_F(p_vehicle_data->host_speed) > p_cals->k_pt_apply_move_point_min_speed)
           || (Fbk_Abs_F(p_vehicle_data->yawrate) > p_cals->k_pt_apply_move_point_min_yaw_rate)))
   {
      Pt_Rotate_Recorded_Paths(p_pt_persistent, p_cals, p_pt_input, p_vehicle_data);
   }

   for (i = 0u; i < PT_NUMBER_OF_PATHS; i++)
   {
      Pt_Group_Paths(p_pt_persistent, &p_pt_persistent->paths[i], p_cals, p_pt_input);

      if ((PATH_STATUS_MATURE == p_pt_persistent->paths[i].path_state) || (PATH_STATUS_GROUPED == p_pt_persistent->paths[i].path_state)
          || (PATH_STATUS_GROUPED_IN_CURRENT_CYCLE == p_pt_persistent->paths[i].path_state))
      {
         /* Increase path_age for valid paths */
         Sat_Inc_Uint16(&p_pt_persistent->paths[i].path_age);
      }

      Pt_Reset_Implausible_Path(&p_pt_persistent->paths[i], p_pt_persistent->best_path_obj_pairs, p_cals);
      Pt_Reset_Far_Field_Path(&p_pt_persistent->paths[i], p_pt_persistent->best_path_obj_pairs, p_cals);
   }

   for (i = 0u; i < PT_NUMBER_OF_PATHS; i++)
   {
      /* Reset paths with state PATH_STATUS_GROUPED_IN_CURRENT_CYCLE to PATH_STATUS_GROUPED.
      The state is used to restrict the grouping to only once per cycle. */
      if (PATH_STATUS_GROUPED_IN_CURRENT_CYCLE == p_pt_persistent->paths[i].path_state)
      {
         p_pt_persistent->paths[i].path_state = PATH_STATUS_GROUPED;
      }
   }
}

static void Pt_Reset_Implausible_Path(Pt_Path_T *p_path,
                                      Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                      const Pt_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != best_path_object_pairs);
   assert(NULL != p_cals);

   if ((p_path->obj_curr_used_for_path_build.id > FBK_ZERO_UINT) && (PATH_DIRECTION_NONE != p_path->direction))
   {
      uint8_t num_of_path_points_by_index;
      uint8_t num_of_path_points_non_zero;
      uint8_t num_of_path_points_difference;

      num_of_path_points_by_index = Pt_Get_Number_Of_Path_Points(p_path);
      num_of_path_points_non_zero = Pt_Get_Number_Of_Non_Zero_Path_Points(p_path);

      if (num_of_path_points_by_index >= num_of_path_points_non_zero)
      {
         num_of_path_points_difference = (uint8_t) (num_of_path_points_by_index - num_of_path_points_non_zero);
      }
      else
      {
         num_of_path_points_difference = (uint8_t) (num_of_path_points_non_zero - num_of_path_points_by_index);
      }

      if (num_of_path_points_difference > p_cals->k_pt_cond_kill_implaus_path)
      {
         Pt_Reset_Path_And_Associations_To_It(p_path, best_path_object_pairs, PATH_RESET_IMPLAUSIBLE_PATH);
      }
   }
}

static void Pt_Calculate_Path_Dir(Pt_Path_T *p_path, const Pt_Object_Orientation_T *p_obj_orientation)
{
   /* Assert */
   assert(NULL != p_path);
   assert(NULL != p_obj_orientation);

   if ((p_path->first_p != p_path->last_p) && (PATH_DIRECTION_NONE == p_path->direction))
   {
      if (p_path->first_p > p_path->last_p)
      {
         if (PT_OBJECT_ORIENTATION_LONGITUDINAL == *(p_obj_orientation))
         {
            p_path->direction = PATH_DIRECTION_LONG_BACKWARD;
         }
         else
         {
            p_path->direction = PATH_DIRECTION_LAT_LEFT;
         }

         Fbk_Swap_Uint8(&(p_path->first_p), &(p_path->last_p));

         Fbk_Swap_Float(&(p_path->first.x), &(p_path->last_mat.x));
         Fbk_Swap_Float(&(p_path->first.y), &(p_path->last_mat.y));
      }
      else
      {
         if (PT_OBJECT_ORIENTATION_LONGITUDINAL == *(p_obj_orientation))
         {
            p_path->direction = PATH_DIRECTION_LONG_FORWARD;
         }
         else
         {
            p_path->direction = PATH_DIRECTION_LAT_RIGHT;
         }
      }

      Pt_Check_Direction_Integrity(p_path);
   }
}

static void Pt_Check_Direction_Integrity(Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   if (p_path->first_p > p_path->last_p)
   {
      Fbk_Swap_Uint8(&(p_path->first_p), &(p_path->last_p));
   }

   if ((PATH_DIRECTION_LAT_RIGHT == p_path->direction) && (p_path->last_mat.y < p_path->first.y))
   {
      p_path->direction = PATH_DIRECTION_LAT_LEFT;
      Fbk_Swap_Float(&(p_path->first.x), &(p_path->last_mat.x));
      Fbk_Swap_Float(&(p_path->first.y), &(p_path->last_mat.y));
   }
   else if ((PATH_DIRECTION_LONG_FORWARD == p_path->direction) && (p_path->last_mat.x < p_path->first.x))
   {
      p_path->direction = PATH_DIRECTION_LONG_BACKWARD;
      Fbk_Swap_Float(&(p_path->first.x), &(p_path->last_mat.x));
      Fbk_Swap_Float(&(p_path->first.y), &(p_path->last_mat.y));
   }
   else
   {
      /*Do nothing*/
   }
}

static void Pt_Init_Path(const uint8_t available_path_idx, Pt_Path_T *p_path, const Fbk_Object_Data_T *p_fbk_object_data)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_fbk_object_data);

   if ((PT_DEFAULT_MATCH_INDEX != available_path_idx) && (available_path_idx < PT_NUMBER_OF_PATHS))
   {
      p_path->path_state                       = PATH_STATUS_CREATION;
      p_path->obj_curr_used_for_path_build.id  = p_fbk_object_data->id;
      p_path->obj_curr_used_for_path_build.age = p_fbk_object_data->age;
      p_path->first.x                          = p_fbk_object_data->vcs_pos.x;
      p_path->first.y                          = p_fbk_object_data->vcs_pos.y;
      p_path->last_mat.x                       = p_fbk_object_data->vcs_pos.x;
      p_path->last_mat.y                       = p_fbk_object_data->vcs_pos.y;
      p_path->first_p                          = PT_DEFAULT_DISCR_BORDER;
      p_path->last_p                           = PT_DEFAULT_DISCR_BORDER;
      p_path->direction                        = PATH_DIRECTION_NONE;
      p_path->path_age                         = 1;
      p_path->max_speed                        = p_fbk_object_data->speed;
   }
}

static void Pt_Track_Path_Wrapper(Pt_Path_T *p_path,
                                  const uint8_t path_point_index,
                                  const float32_T *p_path_first_grid_comp,
                                  const float32_T *p_path_first_point_comp,
                                  const float32_T *p_path_last_grid_comp,
                                  const float32_T *p_path_last_point_comp,
                                  const float32_T *p_obj_vcs_grid_comp,
                                  const float32_T *p_obj_vcs_point_comp,
                                  const Pt_Input_T *p_pt_input,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_path_first_grid_comp);
   assert(NULL != p_path_first_point_comp);
   assert(NULL != p_path_last_grid_comp);
   assert(NULL != p_path_last_point_comp);
   assert(NULL != p_obj_vcs_grid_comp);
   assert(NULL != p_obj_vcs_point_comp);
   assert(NULL != p_pt_input);
   assert(NULL != p_pt_input->grid_pt_array);
   assert(NULL != p_vehicle_data);

   if (Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(p_path))
   {
      if (Fbk_Abs_F(*(p_path_first_grid_comp) - *(p_obj_vcs_grid_comp)) <= EPSILON)
      {
         p_path->path_points[path_point_index] = *(p_obj_vcs_point_comp);
      }
      else
      {
         p_path->path_points[path_point_index] =
            Get_Y_Value_From_Line_By_Coordinates(*(p_obj_vcs_grid_comp), *(p_obj_vcs_point_comp), *(p_path_first_grid_comp),
                                                 *(p_path_first_point_comp), p_pt_input->grid_pt_array[path_point_index]);
      }

      if (PT_DEFAULT_DISCR_BORDER == p_path->last_p)
      {
         p_path->last_p = path_point_index;
      }

      /* If object has been coasted and returns with multiple path points which weren't added to path yet,
       * then only apply first_p to the first path_point_index. Else one point will be missing if extrapolation in matching is
       * reversed.*/
      if (path_point_index < p_path->first_p)
      {
         p_path->first_p = path_point_index;
      }
      if (Fbk_Abs_F(p_vehicle_data->host_speed) > EPSILON)
      {
         p_path->new_path_point_status = PATH_POINT_NEW_FIRST;
      }
   }
   else
   {
      if (Fbk_Abs_F(*(p_path_last_grid_comp) - *(p_obj_vcs_grid_comp)) <= EPSILON)
      {
         p_path->path_points[path_point_index] = *(p_obj_vcs_point_comp);
      }
      else
      {
         p_path->path_points[path_point_index] =
            Get_Y_Value_From_Line_By_Coordinates(*(p_obj_vcs_grid_comp), *(p_obj_vcs_point_comp), *(p_path_last_grid_comp),
                                                 *(p_path_last_point_comp), p_pt_input->grid_pt_array[path_point_index]);
      }

      if (PT_DEFAULT_DISCR_BORDER == p_path->first_p)
      {
         p_path->first_p = path_point_index;
      }
      p_path->last_p = path_point_index;

      if (Fbk_Abs_F(p_vehicle_data->host_speed) > EPSILON)
      {
         p_path->new_path_point_status = PATH_POINT_NEW_LAST;
      }
   }
}

static void Pt_Update_Maximum_Path_Speed(Pt_Path_T *p_path, const Fbk_Object_Data_T *p_fbk_object_data)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_fbk_object_data);

   if (p_fbk_object_data->speed > p_path->max_speed)
   {
      p_path->max_speed = p_fbk_object_data->speed;
   }
}

static void Pt_Update_Path_Borders(Pt_Path_T *p_path, const Fbk_Object_Data_T *p_fbk_object_data)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_fbk_object_data);

   if (Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(p_path))
   {
      p_path->first              = p_fbk_object_data->vcs_pos;
      p_path->path_border_status = PATH_BORDER_NEW_FIRST;
   }
   else
   {
      p_path->last_mat           = p_fbk_object_data->vcs_pos;
      p_path->path_border_status = PATH_BORDER_NEW_LAST;
   }
}

static void Pt_Process_Incomplete_Path(Pt_Path_T *p_path,
                                       Pt_Persistent_T *p_pt_persistent,
                                       const Pt_Input_T *p_pt_input,
                                       const Pt_Core_Calibration_T *p_cals,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T half_grid_point_width;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != p_pt_input);
   assert(NULL != p_pt_persistent);

   half_grid_point_width = Fbk_Half(p_pt_input->grid_pt_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);

   if (Pt_Get_Number_Of_Path_Points(p_path) <= p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_max_diff_num_path_point)
   {
      Pt_Reset_Path_And_Associations_To_It(p_path, p_pt_persistent->best_path_obj_pairs, PATH_RESET_INCOMPLETE_PATH_TOO_SHORT);
   }
   else
   {
      Pt_Extrapolate_Path(p_path, p_cals, p_pt_input->grid_pt_array);

      /*Object position related path borders are now extrapolated on the discrete borders +/- half of the grid point width*/
      if (Pt_Is_Path_Longitudinal(p_path))
      {
         if (Fbk_Abs_F(p_path->last_mat.x - p_pt_input->grid_pt_array[p_path->last_p]) > EPSILON)
         {
            p_path->last_mat.y = Get_Y_Value_From_Line_By_Coordinates(
               p_path->last_mat.x, p_path->last_mat.y, p_pt_input->grid_pt_array[p_path->last_p],
               p_path->path_points[p_path->last_p], p_pt_input->grid_pt_array[p_path->last_p] + half_grid_point_width);
            p_path->last_mat.x = p_pt_input->grid_pt_array[p_path->last_p] + half_grid_point_width;
         }

         if (Fbk_Abs_F(p_path->first.x - p_pt_input->grid_pt_array[p_path->first_p]) > EPSILON)
         {
            p_path->first.y = Get_Y_Value_From_Line_By_Coordinates(
               p_path->first.x, p_path->first.y, p_pt_input->grid_pt_array[p_path->first_p], p_path->path_points[p_path->first_p],
               p_pt_input->grid_pt_array[p_path->first_p] - half_grid_point_width);
            p_path->first.x = p_pt_input->grid_pt_array[p_path->first_p] - half_grid_point_width;
         }
      }
      else
      {
         if (Fbk_Abs_F(p_path->last_mat.y - p_pt_input->grid_pt_array[p_path->last_p]) > EPSILON)
         {
            p_path->last_mat.x = Get_Y_Value_From_Line_By_Coordinates(
               p_path->last_mat.y, p_path->last_mat.x, p_pt_input->grid_pt_array[p_path->last_p],
               p_path->path_points[p_path->last_p], p_pt_input->grid_pt_array[p_path->last_p] + half_grid_point_width);
            p_path->last_mat.y = p_pt_input->grid_pt_array[p_path->last_p] + half_grid_point_width;
         }

         if (Fbk_Abs_F(p_path->first.y - p_pt_input->grid_pt_array[p_path->first_p]) > EPSILON)
         {
            p_path->first.x = Get_Y_Value_From_Line_By_Coordinates(
               p_path->first.y, p_path->first.x, p_pt_input->grid_pt_array[p_path->first_p], p_path->path_points[p_path->first_p],
               p_pt_input->grid_pt_array[p_path->first_p] - half_grid_point_width);
            p_path->first.y = p_pt_input->grid_pt_array[p_path->first_p] - half_grid_point_width;
         }
      }
      p_path->path_border_status = PATH_BORDER_POINTS_MATURE;
      p_path->path_state         = PATH_STATUS_MATURE;

      if (Pt_Is_Extrapolated_Path_Part_Implausible(p_path, p_cals, p_vehicle_data))
      {
         Pt_Reset_Path_And_Associations_To_It(p_path, p_pt_persistent->best_path_obj_pairs, PATH_RESET_EXTRAPOLATED_PATH_IMPLAUSIBLE);
      }
   }

   p_path->obj_curr_used_for_path_build.id  = FBK_ZERO_UINT;
   p_path->obj_curr_used_for_path_build.age = 0;
   p_path->new_path_point_status            = PATH_POINT_NEW_MATURE;
}

static boolean_T Pt_Is_Extrapolated_Path_Part_Implausible(const Pt_Path_T *p_path,
                                                          const Pt_Core_Calibration_T *p_cals,
                                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_path_is_implausible         = FBK_FALSE;
   boolean_T f_long_path_point_implausible = FBK_FALSE;
   boolean_T f_lat_path_point_implausible  = FBK_FALSE;
   uint8_t index_to_compare;
   uint8_t dist_nearest_path_border;
   uint8_t dist_to_last_p;
   uint8_t dist_to_first_p;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != p_vehicle_data);

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      /*In case of ced scenarios take a look at the mid - 1 point. This is not symmetric for front and rear ced scenarios.*/
      index_to_compare = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

      if (((-Fbk_Half(p_vehicle_data->host_width) - p_cals->k_pt_host_implausibilty_range) <= p_path->path_points[index_to_compare])
          && ((Fbk_Half(p_vehicle_data->host_width) + p_cals->k_pt_host_implausibilty_range) >= p_path->path_points[index_to_compare]))
      {
         f_long_path_point_implausible = FBK_TRUE;
      }
   }
   else
   {
      /*In case of cta scenarios take a look at the mid point. This is symmetric for scenarios from left and right.*/
      index_to_compare = PT_MID_GRID_POINT_INDEX;

      if (((-p_vehicle_data->host_length - p_cals->k_pt_host_implausibilty_range) <= p_path->path_points[index_to_compare])
          && (p_cals->k_pt_host_implausibilty_range >= p_path->path_points[index_to_compare]))
      {
         f_lat_path_point_implausible = FBK_TRUE;
      }
   }

   if (index_to_compare >= p_path->last_p)
   {
      dist_to_last_p = (uint8_t) (index_to_compare - p_path->last_p);
   }
   else
   {
      dist_to_last_p = (uint8_t) (p_path->last_p - index_to_compare);
   }

   if (index_to_compare >= p_path->first_p)
   {
      dist_to_first_p = (uint8_t) (index_to_compare - p_path->first_p);
   }
   else
   {
      dist_to_first_p = (uint8_t) (p_path->first_p - index_to_compare);
   }

   dist_nearest_path_border = Min(dist_to_last_p, dist_to_first_p);

   if ((dist_nearest_path_border >= p_cals->k_pt_range_nearest_border_impl_path)
       && (f_long_path_point_implausible || f_lat_path_point_implausible))
   {
      f_path_is_implausible = FBK_TRUE;
   }

   return f_path_is_implausible;
}

static boolean_T Pt_Is_Object_Out_Of_Tracking_Range(const Fbk_Object_Data_T *p_fbk_object_data, const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_object_is_out_of_tracking_range = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_fbk_object_data);
   assert(NULL != p_cals);

   if ((Fbk_Abs_F(p_fbk_object_data->vcs_pos.x) >= p_cals->k_pt_path_track_long_range_limit)
       || (Fbk_Abs_F(p_fbk_object_data->vcs_pos.y) >= p_cals->k_pt_path_track_lat_range_limit))
   {
      f_object_is_out_of_tracking_range = FBK_TRUE;
   }

   return f_object_is_out_of_tracking_range;
}

static boolean_T Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(const Fbk_Object_Data_T *p_fbk_object_data,
                                                                const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_coasted_obj_became_invalid = FBK_FALSE;

   if (Fbk_Is_False(Pt_Is_Obj_Valid_For_Path_Creation(p_fbk_object_data, p_cals)))
   {
      f_coasted_obj_became_invalid = FBK_TRUE;
   }

   return f_coasted_obj_became_invalid;
}

static boolean_T Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(Pt_Path_T *p_path,
                                                             Pt_Persistent_T *p_pt_persistent,
                                                             const Pt_Input_T *p_pt_input,
                                                             const Pt_Object_Orientation_T *p_obj_orientation,
                                                             const Fbk_Object_Data_T *p_fbk_object_data,
                                                             const Pt_Core_Calibration_T *p_cals,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_has_path_been_finished = FBK_FALSE;
   boolean_T f_is_impl;
   boolean_T f_obj_range;
   boolean_T f_direction;

   /* Assert */
   assert(NULL != p_path);

   f_is_impl   = Pt_Is_Associated_Obj_Implausible(p_fbk_object_data, p_path, p_cals);
   f_obj_range = Pt_Is_Object_Out_Of_Tracking_Range(p_fbk_object_data, p_cals);
   f_direction = (boolean_T) ((PATH_DIRECTION_NONE != p_path->direction)
                              && Fbk_Is_False(Pt_Do_Path_Direction_And_Object_Orientation_Match(p_path, p_obj_orientation)));

   /*Check whether the path object pair fulfills a property and thus the creation process is aborted.*/
   if (f_is_impl || f_obj_range || f_direction)
   {
      Pt_Process_Incomplete_Path(p_path, p_pt_persistent, p_pt_input, p_cals, p_vehicle_data);
      f_has_path_been_finished = FBK_TRUE;
   }

   return f_has_path_been_finished;
}

static void Pt_Update_Path_Points(Pt_Path_T *p_path,
                                  const Fbk_Object_Data_T *p_fbk_object_data,
                                  const Pt_Object_Orientation_T obj_orientation,
                                  const Pt_Input_T *p_pt_input,
                                  const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T last_path_point;
   float32_T relevant_obj_pos_component;
   uint8_t path_grid_idx;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_fbk_object_data);
   assert(NULL != p_pt_input);
   assert(NULL != p_vehicle_data);

   last_path_point            = Pt_Get_Last_Path_Point(p_path, obj_orientation);
   relevant_obj_pos_component = Pt_Get_Path_Relevant_Obj_Pos_Component(p_fbk_object_data, obj_orientation);

   for (path_grid_idx = 0u; path_grid_idx < PT_NUM_GRID_POINTS; path_grid_idx++)
   {
      /* Check whether a path point shall be added. Here the loop is not broken out since multiple adding of path points could
       * occure due to long coasting objects. */
      if (Pt_Shall_Path_Point_Be_Added(&p_pt_input->grid_pt_array[path_grid_idx], &relevant_obj_pos_component, &last_path_point))
      {
         if (Pt_Is_Path_Longitudinal(p_path))
         {
            Pt_Track_Path_Wrapper(p_path, path_grid_idx, &(p_path->first.x), &(p_path->first.y), &(p_path->last_mat.x),
                                  &(p_path->last_mat.y), &(p_fbk_object_data->vcs_pos.x), &(p_fbk_object_data->vcs_pos.y),
                                  p_pt_input, p_vehicle_data);
         }
         else if (Pt_Is_Path_Lateral(p_path))
         {
            Pt_Track_Path_Wrapper(p_path, path_grid_idx, &(p_path->first.y), &(p_path->first.x), &(p_path->last_mat.y),
                                  &(p_path->last_mat.x), &(p_fbk_object_data->vcs_pos.y), &(p_fbk_object_data->vcs_pos.x),
                                  p_pt_input, p_vehicle_data);
         }
         else
         {
            if ((PT_OBJECT_ORIENTATION_LONGITUDINAL == obj_orientation))
            {
               Pt_Track_Path_Wrapper(p_path, path_grid_idx, &(p_path->first.x), &(p_path->first.y), &(p_path->last_mat.x),
                                     &(p_path->last_mat.y), &(p_fbk_object_data->vcs_pos.x), &(p_fbk_object_data->vcs_pos.y),
                                     p_pt_input, p_vehicle_data);
            }
            else
            {
               Pt_Track_Path_Wrapper(p_path, path_grid_idx, &(p_path->first.y), &(p_path->first.x), &(p_path->last_mat.y),
                                     &(p_path->last_mat.x), &(p_fbk_object_data->vcs_pos.y), &(p_fbk_object_data->vcs_pos.x),
                                     p_pt_input, p_vehicle_data);
            }
         }
      }
   }
}

static boolean_T Pt_Shall_Path_Point_Be_Added(const float32_T *p_grid_array_comp,
                                              const float32_T *p_relevant_obj_pos_component,
                                              const float32_T *p_last_path_point)
{
   boolean_T f_path_point_shall_be_added = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_last_path_point);
   assert(NULL != p_relevant_obj_pos_component);
   assert(NULL != p_grid_array_comp);

   if (((*p_last_path_point > *p_grid_array_comp) && (*p_relevant_obj_pos_component <= *p_grid_array_comp))
       || ((*p_last_path_point < *p_grid_array_comp) && (*p_relevant_obj_pos_component >= *p_grid_array_comp)))
   {
      f_path_point_shall_be_added = FBK_TRUE;
   }

   return f_path_point_shall_be_added;
}

static float32_T Pt_Get_Path_Relevant_Obj_Pos_Component(const Fbk_Object_Data_T *p_fbk_object_data,
                                                        const Pt_Object_Orientation_T obj_orientation)
{
   float32_T relevant_obj_pos_component;

   /* Assert */
   assert(NULL != p_fbk_object_data);

   if (PT_OBJECT_ORIENTATION_LONGITUDINAL == obj_orientation)
   {
      relevant_obj_pos_component = p_fbk_object_data->vcs_pos.x;
   }
   else
   {
      relevant_obj_pos_component = p_fbk_object_data->vcs_pos.y;
   }

   return relevant_obj_pos_component;
}

static float32_T Pt_Get_Last_Path_Point(const Pt_Path_T *p_path, const Pt_Object_Orientation_T obj_orientation)
{
   float32_T last_path_point;

   /* Assert */
   assert(NULL != p_path);

   if ((PATH_DIRECTION_LONG_BACKWARD == p_path->direction))
   {
      last_path_point = p_path->first.x;
   }
   else if ((PATH_DIRECTION_LONG_FORWARD == p_path->direction))
   {
      last_path_point = p_path->last_mat.x;
   }
   else if ((PATH_DIRECTION_LAT_LEFT == p_path->direction))
   {
      last_path_point = p_path->first.y;
   }
   else if ((PATH_DIRECTION_LAT_RIGHT == p_path->direction))
   {
      last_path_point = p_path->last_mat.y;
   }
   else
   {
      if (PT_OBJECT_ORIENTATION_LONGITUDINAL == obj_orientation)
      {
         last_path_point = p_path->last_mat.x;
      }
      else
      {
         last_path_point = p_path->last_mat.y;
      }
   }

   return last_path_point;
}

static void Pt_Update_Object_Used_For_Path_Age(Pt_Path_T *p_path, const Pt_Input_T *p_pt_input, const uint8_t obj_index)
{
   const Fbk_Object_Data_T *p_current_object_data;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_pt_input);
   assert(obj_index < PA_OBJ_NUMBER_OF_OBJECTS);

   p_current_object_data = &p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index];
   if ((PA_INVALID_OBJ_ID != p_current_object_data->id) && (p_path->obj_curr_used_for_path_build.id == p_current_object_data->id))
   {
      p_path->obj_curr_used_for_path_build.age = p_current_object_data->age;
   }
}

static boolean_T Pt_Is_Obj_Valid_For_Path_Creation(const Fbk_Object_Data_T *p_fbk_object_data, const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_obj_valid_for_path_creation;
   boolean_T f_ref;
   boolean_T f_speed;
   boolean_T f_prob;
   boolean_T f_in_range;

   /* Asserts */
   assert(NULL != p_fbk_object_data);
   assert(NULL != p_cals);

   f_ref      = (boolean_T) Fbk_Is_False(p_fbk_object_data->f_reflection);
   f_speed    = (boolean_T) (p_fbk_object_data->speed >= p_cals->k_pt_min_obj_speed);
   f_prob     = (boolean_T) (p_fbk_object_data->existence_probability >= p_cals->k_pt_min_exist_prob_to_be_valid);
   f_in_range = (boolean_T) Fbk_Is_False(Pt_Is_Object_Out_Of_Tracking_Range(p_fbk_object_data, p_cals));

   f_obj_valid_for_path_creation = (boolean_T) (f_in_range && f_ref && f_speed && f_prob);

   return f_obj_valid_for_path_creation;
}

static boolean_T Pt_Is_Associated_Obj_Implausible(const Fbk_Object_Data_T *p_fbk_object_data,
                                                  const Pt_Path_T *p_path,
                                                  const Pt_Core_Calibration_T *p_cals)
{
   boolean_T f_object_is_implausible = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_fbk_object_data);
   assert(NULL != p_path);

   if (Fbk_Is_True(p_cals->k_pt_f_check_object_age_plausibility))
   {
      if (p_fbk_object_data->age < p_path->obj_curr_used_for_path_build.age)
      {
         f_object_is_implausible = FBK_TRUE;
      }
   }

   return f_object_is_implausible;
}

static Pt_Path_T *Pt_Get_Path_By_Abstraction_Layer(Pt_Persistent_T *p_pt_persistent, const Pt_Input_T *p_pt_input, const uint8_t obj_index)
{
   uint8_t path_index = PT_DEFAULT_MATCH_INDEX;
   Pt_Path_T *p_path  = NULL;
   uint8_t i;
   const Fbk_Object_Data_T *p_current_object_data;

   /* Asserts */
   assert(NULL != p_pt_persistent);
   assert(NULL != p_pt_input);
   assert(obj_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /*Searh for any associated paths*/
   p_current_object_data = &p_pt_input->p_fbk_output->p_pa_data->object_data[obj_index];
   if (PA_INVALID_OBJ_ID != p_current_object_data->id)
   {
      for (i = FBK_ZERO_UINT; i < PT_NUMBER_OF_PATHS; i++)
      {
         if (p_current_object_data->id == p_pt_persistent->paths[i].obj_curr_used_for_path_build.id)
         {
            path_index = i;
            break;
         }
      }
   }

   /*Check whether any path is associated to the object as creation object*/
   if (PT_DEFAULT_MATCH_INDEX != path_index)
   {
      /* Set pointer to current path via path index */
      p_path = &p_pt_persistent->paths[path_index];

      /* Set path border status */
      p_path->path_border_status = PATH_BORDER_POINTS_MATURE;
   }

   return p_path;
}
