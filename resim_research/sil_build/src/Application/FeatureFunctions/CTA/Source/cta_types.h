#ifndef CTA_TYPES_H
#define CTA_TYPES_H

/**
 * @file cta_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the CTA specific data types.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_ref_point.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define CTA_COUNTER_MAX (255u) /**< maximum value of a counter*/

#define CTA_NUM_CRIT_LEVEL (2u) /**<number of criticality level*/

#define CTA_HIGH_DEFAULT_VAL (100.0f) /**< Default value for TTC and other properties */

/* Number of maximum array size for all available objects to save ID */
#define CTA_OBJ_MAX_ARRAY_SIZE ((uint8_t) PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT)

/*===========================================================================*\
* Enums
\*===========================================================================*/

/**
 * @brief Summarizes the modes which can be used in CTA.
 * @SDD{CSCSA-27467}
 */
typedef enum
{
   CTA_MODE_REAR  = (0) /**< rear mode for evaluation of object criticality at the rear bumper */,
   CTA_MODE_FRONT = (1) /**< front mode for evaluation of object criticality at the rear bumper */,
   CTA_NUM_MODES  = (2) /**< Invalid mode. */
} Cta_Mode_T;

/**
 * @brief Summarizes the criticality levels in which the tracked objects can be classified
 * @SDD{SF-3905}
 */
typedef enum
{
   CTA_CRIT_LEVEL_NONE = (0) /**< no criticality level for the current object*/,
   CTA_CRIT_LEVEL_1    = (1) /**< indicates that criticality level one is reached*/,
   CTA_CRIT_LEVEL_2    = (2) /**< indicates that criticality level two is reached*/
} Cta_Crit_Level_T;

/**
 * @brief Summarizes the alert stop modes for CTA. Alert is suppressed when the TTC or TTP of the criticall object drops below
 * threshold defined by calibration.
 * @SDD{CSCSA-226689}
 */
typedef enum
{
   CTA_STOP_MODE_TTC = (0) /**< time to cross (target front to crash line) is used to suppress the alert  */,
   CTA_STOP_MODE_TTP = (1) /**< time to pass (target rear to crash line) is used to suppress the alert  */,
   CTA_STOP_MODE_NUM = (2) /**< Invalid mode. */
} Cta_Stop_Mode_T;


/*===========================================================================*\
* typedefs
\*===========================================================================*/


/**
 * @brief Summarizes the criticality level thresholds and properties which are also evaluated in criticality level determination.
 * Internally they are gathered at first from the calibration structure and might then be
 * adapted by host properties as well as hystereses dependent on the customer configuration.
 *
 * @SDD{CSCSA-27461}
 */
typedef struct
{
   float32_T min_long_point_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL]; /**< min intersection point criticality level
                                                                                       logic*/
   float32_T max_long_point_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL]; /**< max intersection point criticality level
                                                                                       logic*/
   float32_T ttc_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL];            /**< TTC criticality level logic*/
   float32_T speed_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL];          /**< Object speed criticality level logic*/
   float32_T max_eclipse_value; /**< maximum eclipse value for an object to qualify for current level (same for all level)*/
   uint16_t mature_cycles; /**< num of cycles an object needs to be within the mature status before it is a valid candidate for
                           current level*/
   uint8_t minimum_age;    /**<minimum age which an object needs to have to be considered for being critical*/
} Cta_Crit_Level_Calibration_T;


/**
 * Provides information about the target attributes
 *
 * @SAE{SF-2459}
 * @SDD{CSCSA-27460}
 */
typedef struct
{
   const Pt_Path_Object_Pair_Output_T *p_pt_match_info;      /**<pt output which gives information about the matched path*/
   const Pt_Nearest_Path_T *p_pt_nearest_path_info;          /**<pt output which gives information about the nearest path*/
   Vector_2d_T relative_velocity;                            /**<relative velocity. cta_heading can be used within here*/
   Fbk_Ref_Point_T ref_point_candidate[FBK_NUMBER_OF_SIDES]; /**< reference points use for checking intersection  */
   Fbk_Ref_Point_T ref_point;                                /**< reference point of the object*/
   Fbk_Ref_Point_T ref_point_ttp;                            /**< reference point of the object used for ttp calculation*/
   float32_T long_isect_point_candidate[FBK_NUMBER_OF_SIDES][CTA_NUM_MODES]; /**< longitudinal intersection points of the object
                                                                                corner*/
   float32_T long_isect_point[CTA_NUM_MODES]; /**<longitudinal intersection point of the object in both cta modes.
                                      This can be heading compensated and due to that the values can differ between the modes.*/
   float32_T ttc;                             /**<time to collision of the considered object*/
   float32_T CTA_heading; /**<heading used for objects may either consist of tracker heading or path tracking heading*/
   uint8_t approach_side; /**<approach side of the object*/
   float32_T ttp;         /**<time to pass collision line of the considered object*/

   boolean_T f_stop_time_below_ths; /**<flag indicating if the time to collision/pass is below calibration threshold. TTP or TTC
                                      comparision mode is determined by the input flag cta_stop_mode */
   boolean_T f_standstill_qualifier;
   float32_T brake_deceleration;
} Cta_Object_Attributes_T;

/**
 * @brief Contains persistent variables
 *
 * @SAE{SF-2459}
 * @SDD{SF-3691}
 */
typedef struct
{
   boolean_T f_prev_cta_alert_suppress;      /**<flag indicating whether cta alert was supressed previously*/
   uint8_t prev_approach_side;               /**<approach side of the previous cycle*/
   uint8_t obj_validity_suppression_counter; /**<suppression counter in dependency of matched path. Needed in object validation and
                                                independent of any mode */
   uint8_t crit_level_suppression_counter[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL]; /**<suppression counter for each criticality level*/
   uint8_t prev_cycle_crit_level[CTA_NUM_MODES]; /**<criticality level in previous (only core internally the previous cycle
                                                  *in postrun it is the current) cycle.*/
   uint8_t n_alert_cycles[CTA_NUM_MODES];        /**< number of cycles this object caused an alert*/
   float32_T cta_object_heading;                 /** heading of the object in previous cycle */
} Cta_Object_Persistent_T;

/**
 * @brief Summarizes object data needed within CTA
 *
 * @SAE{SF-2459}
 * @SDD{SF-3690}
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data;      /**< tracker data*/
   Cta_Object_Attributes_T *attributes; /**< pointer to target attributes*/
   Cta_Object_Persistent_T *persistent; /**< pointer to persistent CTA variables*/
} Cta_Object_Data_T;

/**
 * @brief Extensions of the intersection zone
 *
 * @SAE{SF-2459}
 * @SDD{SF-3683}
 */
typedef struct
{
   float32_T host_steer_fac[CTA_NUM_MODES]; /**< extension due to steering angle of the host which is depicted by a factor for
                                criticality level zone*/
   float32_T target_head_fac; /**< extension due to heading of the object which is depicted by a factor for criticality level zone*/
   float32_T host_vel_fac;    /**<absolute extension due to host velocity. This will be added to the length of criticality levels
                                  zone*/
} Cta_Inters_Zone_Ext_Param_T;


/**
 * @brief Summarizes data of CTA which is used in one cylce for object comparisons and evaluation of the hightest criticality
 * level.
 *
 * @SAE{SF-2459}
 * @SDD{CSCSA-27458}
 */
typedef struct
{
   Cta_Crit_Level_T max_level[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];                 /**< Maximum criticality level on each side*/
   Cta_Object_Data_T object_with_highest_crit[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< data array to store object with highest
                                                                       criticality for both approaching directions*/
} Cta_Comparison_Data_T;

#endif /*CTA_TYPES_H*/
