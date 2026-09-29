#ifndef TA_BMW_ENUMS_H
#define TA_BMW_ENUMS_H

/*============================================================================*\
* BMW SPECIFIC ENUM DEFINITIONS
\*============================================================================*/

/* Defines the HMI warntrigger switches so that warn settings can be set according to the respective situation for the core */
typedef enum
{
   BMW_HMI_WARNTRIGGER_LATE       = (0), /**< HMI trigger for late warning */
   BMW_HMI_WARNTRIGGER_NORMAL     = (1), /**< HMI trigger for normal warning */
   BMW_HMI_WARNTRIGGER_EARLY      = (2), /**< HMI trigger for early warning */
   BMW_HMI_WARNTRIGGER_VERY_EARLY = (3)  /**< HMI trigger for very early warning */
} Ta_Bmw_Hmi_Warntrigger_T;

/* Contains the Symbol-Request bus signal states */
typedef enum
{
   BMW_SYMBOL_REQUEST_NO_WARNING           = (0), /* 0h Keine_Warnung */
   BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE = (1), /* 1h Person_mittig_nah */
   BMW_SYMBOL_REQUEST_PERSON_LEFT          = (3), /* 3h Person_links */
   BMW_SYMBOL_REQUEST_PERSON_RIGHT         = (4)  /* 4h Person_rechts */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Symbol_Request_T;

/* Contains the Brake-Conditioning bus signal states */
typedef enum
{
   BMW_BRAKE_CONDITIONING_NO_REQUEST        = (0), /* 0h No_request */
   BMW_BRAKE_CONDITIONING_REQUEST_IN_800_MS = (1), /* 1h Request_in_800_ms */
   BMW_BRAKE_CONDITIONING_REQUEST_IN_600_MS = (2), /* 2h Request_in_600_ms */
   BMW_BRAKE_CONDITIONING_REQUEST_IN_400_MS = (3), /* 3h Request_in_400_ms */
   BMW_BRAKE_CONDITIONING_REQUEST_IN_200_MS = (4)  /* 4h Request_in_200_ms */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Brake_Conditioning_T;

/* Contains the Alert-Level bus signal states */
typedef enum
{
   BMW_ALERT_LEVEL_NO_WARNING    = (0), /* 0h Keine_Warnung */
   BMW_ALERT_LEVEL_PRE_WARNING   = (1), /* 1h Vorwarnung */
   BMW_ALERT_LEVEL_ACUTE_WARNING = (4)  /* 4h Akutwarnung */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Alert_Level_T;

/* Contains the Brake-Threshold-Reduction bus signal states */
typedef enum
{
   BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC   = (0), /* 0h Defaultparametersatz_DBC */
   BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY = (3)  /* 3h Hoechste_Empfindlichkeit */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Brake_Threshold_Reduction_T;

/* Contains the Maneuver state (which should more-or-less correspond to statemachine) */
typedef enum
{
   BMW_MANEUVER_DIRECTION_STRAIGHT = (0), /* 0h Straight maneuver detected */
   BMW_MANEUVER_DIRECTION_TURN     = (1)  /* 1h Turn maneuver detected */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Bmw_Maneuver_Direction_State_T;

/**
 * @brief Different sensitivities for the alert trigger.
 *
 * @SRS{n/a}
 */
typedef enum
{
   TA_ALERT_TRIGGER_EARLY  = (0), /**< Alert trigger for early warning */
   TA_ALERT_TRIGGER_NORMAL = (1), /**< Alert trigger for normal warning */
   TA_ALERT_TRIGGER_LATE   = (2)  /**< Alert trigger for late warning */
} Ta_Alert_Trigger_T;

#endif /* TA_BMW_ENUMS_H */
