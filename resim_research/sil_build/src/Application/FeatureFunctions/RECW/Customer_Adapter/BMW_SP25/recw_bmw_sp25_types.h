#ifndef RECW_BMW_SP25_TYPES_H
#define RECW_BMW_SP25_TYPES_H

/**
 * @file recw_bmw_sp25_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides BMW_SP25 project specific definitions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===================================================================*\
* Defines
\*===================================================================*/

/* Conversion to percentage */
#define RECW_BMW_SP25_CONVERT_TO_PERCENTAGE(_val) (100.0f * (_val))
/* Velocity threshold for mapping of two wheel for CDC (0.277778 = km/h to m/s) */
#define RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD (30.0f * 0.277778f)


/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_0 (0x0)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_FC (0x0FC)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_1 (0x01)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_4 (0x04)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_6 (0x06)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_FE (0x0FE)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define RECW_BMW_SP25_SM_HEX_E (0x0E)

/*===================================================================*\
* Enums
\*===================================================================*/

typedef enum
{
   RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH = (0),
   RECW_BMW_SP25_TYPE_WARNING_ONLY         = (1),
   RECW_BMW_SP25_TYPE_PRECRASH_ONLY        = (2),
   RECW_BMW_SP25_TYPE_NO_FUNCTION          = (3)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Bmw_Sp25_Type_Input_T;

typedef enum
{
   RECW_BMW_SP25_INACTIVE = (0),
   RECW_BMW_SP25_ACTIVE   = (1)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Bmw_Sp25_Status_Output_T;

typedef enum
{
   RECW_BMW_SP25_WARNING_LEVEL_WARNING_INACTIVE = (0), /* 0h Warnung_inaktiv */
   RECW_BMW_SP25_WARNING_LEVEL_WARNING_ACTIVE   = (1)  /* 1h Warnung_aktiv */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Bmw_Sp25_Warning_Output_T;

typedef enum
{
   RECW_BMW_SP25_WARNING_LEVEL_PRECRASH_INACTIVE = (0), /* 0h Kein_Kollisionsobjekt_detektiert */
   RECW_BMW_SP25_WARNING_LEVEL_PRECRASH_ACTIVE   = (4)  /* 4h Kollision_unvermeidbar */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Bmw_Sp25_PreCrash_Output_T;

typedef enum
{
   RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN    = (0),
   RECW_BMW_SP25_OBJECT_CLASS_CDC_PEDESTRIAN = (1),
   RECW_BMW_SP25_OBJECT_CLASS_CDC_MOTORCYCLE = (2),
   RECW_BMW_SP25_OBJECT_CLASS_CDC_CAR        = (3),
   RECW_BMW_SP25_OBJECT_CLASS_CDC_TRUCK      = (4),
   RECW_BMW_SP25_OBJECT_CLASS_CDC_BICYCLE    = (5)
} Recw_Bmw_Sp25_Object_Class_Cdc_T;

// State machine Types

typedef enum
{
   RECW_SM_NOT_AVAILABLE = (0), /**< code unavilable */
   RECW_SM_READY         = (1), /**< recw not activated */
   RECW_SM_ACTIVE        = (2), /**< recw activated */
   RECW_SM_DEGRADED      = (3), /**< recw degraded */
   RECW_SM_ERROR         = (4)  /**< recw error mode */
} Recw_SM_State_T;

typedef enum
{
   RECW_VEHICLE_STANDSTILL      = (0),
   RECW_VEHICLE_MOVING_FORWARD  = (1),
   RECW_VEHICLE_MOVING_BACKWARD = (2),
   RECW_VEHICLE_MOVING          = (3),
   RECW_VEHICLE_MOVING_ERROR    = (4),
   RECW_VEHICLE_MOVING_UNFILLED = (5)
} Recw_Vehicle_Movement_Status_T;

typedef enum
{
   RECW_NO_TRAILER_AVAILABLE = (0),
   RECW_TRAILER_AVAILABLE    = (1)
} Recw_Trailer_Status_T;

typedef enum
{
   RECW_STATE_DISABLED = (0),
   RECW_STATE_ENABLED  = (1)
} Recw_State_T;

typedef enum
{
   RECW_NO_ERROR           = (0),
   RECW_NON_CRITICAL_ERROR = (1),
   RECW_CRITICAL_ERROR     = (2)
} Recw_Error_T;

typedef enum
{
   RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER            = (0),
   RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER = (1),
   RECW_STATUS_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER  = (2),
   RECW_STATUS_DYNAMOMETER_TWO_AXLE_DYNAMOMETER      = (3),
   RECW_STATUS_DYNAMOMETER_FUNCTION_REPORTS_ERROR    = (14),
   RECW_STATUS_DYNAMOMETER_SIGNAL_UNFILLED           = (15)
} Status_Dynamometer_T;

typedef enum
{
   RECW_STATUS_END_OF_LINE_MODE_NOT_SET           = (0),
   RECW_STATUS_END_OF_LINE_MODE_SET               = (1),
   RECW_STATUS_END_OF_LINE_FUNCTION_REPORTS_ERROR = (14),
   RECW_STATUS_END_OF_LINE_SIGNAL_UNFILLED        = (15)
} Status_End_Of_Line_T;

/*===========================================================================*\
* Type definitions
\*===========================================================================*/

#endif /* RECW_BMW_SP25_TYPES_H */
