#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the RNA_SWEET400 output data structure for LCDA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define LCDA_RNA_NUMBER_OF_OBJECTS (4u)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/* coverity[misra_c_2012_rule_20_9_violation][It is intentional, that _MSC_VER is only defined for MSVC compiler]  */
#if _MSC_VER
#pragma warning(disable : 4214)
#endif

typedef struct
{
   uint8_t f_LCDA_input_is_NULL : 1;
   uint8_t f_LCDA_output_is_NULL : 1;
   uint8_t f_LCDA_cals_is_NULL : 1;
   uint8_t f_LCDA_input_vehicle_data_is_NULL : 1;
   uint8_t f_LCDA_input_tracker_output_is_NULL : 1;

   uint8_t f_createCVWZone_cvw_zone_is_NULL : 1;
   uint8_t f_createCVWZone_cvw_zone_hys_is_NULL : 1;
   uint8_t f_createCVWZone_LCDA_input_is_NULL : 1;
   uint8_t f_createCVWZone_cals_is_NULL : 1;
   uint8_t f_createCVWZone_tracker_output_is_NULL : 1;
   uint8_t f_createCVWZone_vehicle_data_is_NULL : 1;

   uint8_t f_createBSWZone_bsw_zone_is_NULL : 1;
   uint8_t f_createBSWZone_bsw_zone_hys_is_NULL : 1;
   uint8_t f_createBSWZone_LCDA_input_is_NULL : 1;
   uint8_t f_createBSWZone_cals_is_NULL : 1;
   uint8_t f_createBSWZone_tracker_output_is_NULL : 1;
   uint8_t f_createBSWZone_vehicle_data_is_NULL : 1;

   uint8_t f_CheckBSW_LCDA_input_is_NULL : 1;
   uint8_t f_CheckBSW_vehicle_data_is_NULL : 1;
   uint8_t f_CheckBSW_tracker_output_is_NULL : 1;
   uint8_t f_CheckBSW_cals_is_NULL : 1;
   uint8_t f_CheckBSW_candidateInfo_is_NULL : 1;

   uint8_t f_CheckCVW_LCDA_input_is_NULL : 1;
   uint8_t f_CheckCVW_vehicle_data_is_NULL : 1;
   uint8_t f_CheckCVW_tracker_output_is_NULL : 1;
   uint8_t f_CheckCVW_cals_is_NULL : 1;
   uint8_t f_CheckCVW_candidateInfo_is_NULL : 1;

   uint8_t f_input_vehicle_data_lane_width_not_plausible : 1;
   uint8_t f_input_vehicle_data_vehicle_length_not_plausible : 1;
   uint8_t f_input_vehicle_data_vehicle_width_not_plausible : 1;

   /*Update the unused bits below when you make changes above. Ensure that it is 4 byte aligned for resim*/
   uint8_t unused : 2;
} LCDA_ERRORS_T;

/* coverity[misra_c_2012_rule_20_9_violation][It is intentional, that _MSC_VER is only defined for MSVC compiler]  */
#if _MSC_VER
#pragma warning(default : 4214)
#endif

typedef enum
{
   RENAULT_LSS_ALERT_CONDITION_OFF   = (0), /**< 0*/
   RENAULT_LSS_ALERT_CONDITION_TOS   = (1), /**< 1*/
   RENAULT_LSS_ALERT_CONDITION_SOT   = (2), /**< 2*/
   RENAULT_LSS_ALERT_CONDITION_OTHER = (3)  /**< 3*/
} RENAULT_LSS_ALERT_CONDITION_T;

typedef enum
{
   RENAULT_LSS_CHANGE_STATUS_NO_CHANGE = (0), /**< 0*/
   RENAULT_LSS_CHANGE_STATUS_CHANGE    = (1)  /**< 1*/
} RENAULT_LSS_CHANGE_STATUS_T;

typedef enum
{
   RENAULT_LSS_MOTION_CLASS_UNKNOWN           = (0), /**< 0*/
   RENAULT_LSS_MOTION_CLASS_STATIONARY        = (1), /**< 1*/
   RENAULT_LSS_MOTION_CLASS_MOVING_TO_STOPPED = (2), /**< 2*/
   RENAULT_LSS_MOTION_CLASS_MOVING_OBJECT     = (3)  /**< 3*/
} RENAULT_LSS_MOTION_CLASS_T;

typedef enum
{
   RENAULT_LSS_OBJECT_CLASS_UNKNOWN    = (0),
   RENAULT_LSS_OBJECT_CLASS_CAR        = (1),
   RENAULT_LSS_OBJECT_CLASS_BUS        = (2),
   RENAULT_LSS_OBJECT_CLASS_TRUCK      = (3),
   RENAULT_LSS_OBJECT_CLASS_MOTORCYCLE = (4),
   RENAULT_LSS_OBJECT_CLASS_BICYCLE    = (5),
   RENAULT_LSS_OBJECT_CLASS_PEDESTRIAN = (6),
   RENAULT_LSS_OBJECT_CLASS_ANIMAL     = (7),
   RENAULT_LSS_OBJECT_CLASS_OTHER      = (8)
} RENAULT_LSS_OBJECT_CLASS_T;

typedef struct
{
   float32_T lka_curvi_pos_long;
   float32_T lka_curvi_vel_long;

   float32_T lka_curvi_pos_lat;
   float32_T lka_curvi_vel_lat;

   float32_T lka_vcs_pos_long;
   float32_T lka_vcs_vel_long;

   float32_T lka_vcs_pos_lat;
   float32_T lka_vcs_vel_lat;

   float32_T lka_ttc;

   RENAULT_LSS_MOTION_CLASS_T lka_motion_class;
   RENAULT_LSS_ALERT_CONDITION_T lka_alert_condition;
   RENAULT_LSS_CHANGE_STATUS_T lka_change_status;
   RENAULT_LSS_OBJECT_CLASS_T lka_object_class;

   uint8_t lka_obj_id;
   uint8_t lka_tracker_id;
} LKA_Object_T;

typedef struct
{
   LKA_Object_T LKA_Object_Right[LCDA_RNA_NUMBER_OF_OBJECTS];
   LKA_Object_T LKA_Object_Left[LCDA_RNA_NUMBER_OF_OBJECTS];
} LCDA_CUST_SPEC_OUTPUT_T;

typedef struct
{
   uint8_t f_lcda_enabled; /*  Flag indicating that LCMA is enabled */

   uint8_t f_bsw_enabled;   /*  Flag indicating that the BLIS alert functionality is enabled */
   uint8_t bsw_alert_left;  /*  Flag indicating that there is an active BLIS alert for the left side */
   uint8_t bsw_id_left;     /*  ID of the object responsible for the BLIS alert on the left side (0 for none) */
   uint8_t bsw_alert_right; /*  Flag indicating that there is an active BLIS alert for the right side */
   uint8_t bsw_id_right;    /*  ID of the object responsible for the BLIS alert on the right side (0 for none) */

   uint8_t f_cvw_enabled;   /*  Flag indicating that the CVW alert functionality is enabled */
   uint8_t cvw_alert_left;  /*  Flag indicating that there is an active CVW alert for the left side */
   uint8_t cvw_id_left;     /*  ID of the object responsible for the CVW alert on the left side (0 for none) */
   float32_T cvw_ttc_left;  /*  TTC (time to conflict) of the object given by cvw_id_left */
   uint8_t cvw_alert_right; /*  Flag indicating that there is an active CVW alert for the right side */
   uint8_t cvw_id_right;    /*  ID of the object responsible for the CVW alert on the right side (0 for none) */
   float32_T cvw_ttc_right; /*  TTC (time to conflict) of the object given by cvw_id_right */

   LCDA_ERRORS_T LCDA_errors; /*  Error flag output */

} LCDA_CORE_OUTPUT_RNA_T;

typedef struct
{
   LCDA_CORE_OUTPUT_RNA_T core_output;
   LCDA_CUST_SPEC_OUTPUT_T customer_output;
} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
