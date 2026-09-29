#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the LCDA output header file of the Honda SRR6 customer.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* fbk includes */
#include "fbk_macros.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define LCDA_HONDA_NUMBER_OF_OBJECTS (4u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef enum
{
   HONDA_ALERT_STATE_OFF    = (0), /**< Alert is off */
   HONDA_ALERT_STATE_LEVEL1 = (1), /**< Level 1: Turn indicator is OFF on the side of the critical object */
   HONDA_ALERT_STATE_LEVEL2 = (2), /**< Level 2: Turn indicator is OFF on the side of the critical object AND 2nd critical object
                                      is detected sliding through */
   HONDA_ALERT_STATE_LEVEL3 = (3), /**< Level 3: Turn indicator is ON on the side of the critical object and object is in beeper
                                      zone */
   HONDA_ALERT_STATE_LEVEL4 = (4)  /**< Level 4: Turn indicator is ON on the side of the critical object and object is out of the
                                      beeper zone */
} Honda_Alert_State_T;

typedef enum
{
   HONDA_SIDE_BSW_LEFT  = (0),
   HONDA_SIDE_BSW_RIGHT = (1),
   HONDA_SIDE_CVW_LEFT  = (2),
   HONDA_SIDE_CVW_RIGHT = (3),
   HONDA_SIDE_NUMBER    = (4)
} HONDA_SIDE_T;

typedef enum
{
   HONDA_LVL_TWO_UNKNOWN      = (0),
   HONDA_LVL_TWO_SAME_OBJECTS = (1),
   HONDA_LVL_TWO_DIFF_OBJECTS = (2)
} HONDA_LVL_TWO_OBJECTS_INFO;

typedef enum
{
   HONDA_ALERT_CONDITION_OFF   = (0), /**< 0*/
   HONDA_ALERT_CONDITION_TOS   = (1), /**< 1*/
   HONDA_ALERT_CONDITION_SOT   = (2), /**< 2*/
   HONDA_ALERT_CONDITION_OTHER = (3)  /**< 3*/
} HONDA_ALERT_CONDITION_T;

typedef enum
{
   HONDA_CHANGE_STATUS_NO_CHANGE = (0), /**< 0*/
   HONDA_CHANGE_STATUS_CHANGE    = (1)  /**< 1*/
} HONDA_CHANGE_STATUS_T;

typedef enum
{
   HONDA_MOTION_CLASS_UNKNOWN           = (0), /**< 0*/
   HONDA_MOTION_CLASS_STATIONARY        = (1), /**< 1*/
   HONDA_MOTION_CLASS_MOVING_TO_STOPPED = (2), /**< 2*/
   HONDA_MOTION_CLASS_MOVING_OBJECT     = (3)  /**< 3*/
} HONDA_MOTION_CLASS_T;

typedef enum
{
   HONDA_OBJECT_CLASS_UNKNOWN    = (0),
   HONDA_OBJECT_CLASS_CAR        = (1),
   HONDA_OBJECT_CLASS_BUS        = (2),
   HONDA_OBJECT_CLASS_TRUCK      = (3),
   HONDA_OBJECT_CLASS_MOTORCYCLE = (4),
   HONDA_OBJECT_CLASS_BICYCLE    = (5),
   HONDA_OBJECT_CLASS_PEDESTRIAN = (6),
   HONDA_OBJECT_CLASS_ANIMAL     = (7),
   HONDA_OBJECT_CLASS_OTHER      = (8)
} HONDA_OBJECT_CLASS_T;

typedef struct
{
   HONDA_ALERT_CONDITION_T lka_alert_condition;
   HONDA_CHANGE_STATUS_T lka_change_status;
   HONDA_MOTION_CLASS_T lka_motion_class;
   HONDA_OBJECT_CLASS_T lka_object_class;

   float32_T lka_curvi_pos_long;
   float32_T lka_curvi_vel_long;
   float32_T lka_curvi_pos_lat;
   float32_T lka_curvi_vel_lat;
   float32_T lka_ttc;
   uint8_t lka_obj_id;
} LKA_Object_T;

typedef struct
{
   LKA_Object_T LKA_Object_Right[LCDA_HONDA_NUMBER_OF_OBJECTS];
   LKA_Object_T LKA_Object_Left[LCDA_HONDA_NUMBER_OF_OBJECTS];
} LCDA_CUST_SPEC_OUTPUT_T;

/**
 * @brief Lcda_Output_T structure
 *
 *
 * @SRS{SF-1038}
 * @SAE{SF-2802}
 * @SDD{SF-6950}
 */
typedef struct
{
   uint8_t f_lcda_enabled; /* Flag indicating if LCDA is enabled */
   uint8_t f_bsw_enabled;  /* Flag indicating that the BSW subfunction is enabled */
   uint8_t f_cvw_enabled;  /* Flag indicating that the CVW subfunction is enabled */

   uint8_t bsw_alert_left;  /* flag indicating if there is a BSW alert for left side */
   uint8_t bsw_id_left;     /* object ID of critical BSW object on the left side if present (0 for none)*/
   uint8_t bsw_alert_right; /* flag indicating if there is a BSW alert for right side */
   uint8_t bsw_id_right;    /* object ID of critical BSW object on the right side if present (0 for none)*/

   uint8_t cvw_alert_left;  /* flag indicating if there is a CVW alert for left side */
   uint8_t cvw_id_left;     /* object ID of critical CVW object on the left side if present (0 for none)*/
   float32_T cvw_ttc_left;  /* [s] calculated TTC of critical CVW object on the left side if present */
   uint8_t cvw_alert_right; /* flag indicating if there is a CVW alert for right side */
   uint8_t cvw_id_right;    /* object ID of critical CVW object on the right side if present (0 for none)*/
   float32_T cvw_ttc_right; /* [s] calculated TTC of critical CVW object on the right side if present */

   Honda_Alert_State_T honda_alert_state[FBK_NUMBER_OF_SIDES]; /* Contains the Honda alert states for left and right side. */

   LCDA_CUST_SPEC_OUTPUT_T customer_output;

   float32_T hold_time[LCDA_HONDA_NUMBER_OF_OBJECTS]; /* [s] holding time for honda alerts, for both CVW and BSW, right and left
                                                          sides*/
   float32_T predicted_exit_time[LCDA_HONDA_NUMBER_OF_OBJECTS]; /* [s] predicted exit time for objects in the zones, transfered to
                                                                    hold time if conditions are met*/
   uint32_t hold_obj_index[LCDA_HONDA_NUMBER_OF_OBJECTS];       /* indexes of the objects which caused alert holding */
   uint8_t hold_alert_level[LCDA_HONDA_NUMBER_OF_OBJECTS];      /* alert level triggered by the held object*/
   LKA_Object_T hold_object[LCDA_HONDA_NUMBER_OF_OBJECTS];      /* current copies of LKA objects, used in hold mode*/

} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
