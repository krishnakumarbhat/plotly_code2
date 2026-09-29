# ifndef LCDA_CUSTOMER_CALIBRATION_H
# define LCDA_CUSTOMER_CALIBRATION_H

/**
* @file lcda_customer_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "lcda_customer_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_GUARDRAIL_REL_DIFF_THRESH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_EXIST_PROB_LC_INTENTION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_CVW_LIMIT_ZONE_RANGE_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_LOWEST_PROBABILTY_PERCENTAGE_CAL_FOR_ADJUSTMENT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_TRAILER_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_BUFFER ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_LANE_CHANGE_DETECTION_HOST_SPEED_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_LANE_CHANGE_DIST_TO_LANELINE_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_EXIST_PROB_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_SMOOTH_CAMERA_SIGNALS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_F_ENABLE_LANE_CHANGE_DETECTION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_GUARDRAIL_AGE_STAGE_THRESH ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_MAX_BAD_GUARDRAIL_HOLDING_COUNTER ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_LANE_CHANGE_COUNTER_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_LANE_CHANGE_COUNTER_MAX ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_COUNTER_MAX ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_GUARDRAIL_REL_DIFF_THRESH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_EXIST_PROB_LC_INTENTION ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_CVW_LIMIT_ZONE_RANGE_HYS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_LOWEST_PROBABILTY_PERCENTAGE_CAL_FOR_ADJUSTMENT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_TRAILER_LENGTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_DISTANCE ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_BUFFER ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_LANE_CHANGE_DETECTION_HOST_SPEED_MIN ((float32_T)(30.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_LANE_CHANGE_DIST_TO_LANELINE_MAX ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_EXIST_PROB_MIN ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_SMOOTH_CAMERA_SIGNALS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_F_ENABLE_LANE_CHANGE_DETECTION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_GUARDRAIL_AGE_STAGE_THRESH ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_MAX_BAD_GUARDRAIL_HOLDING_COUNTER ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_LANE_CHANGE_COUNTER_MIN ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_LANE_CHANGE_COUNTER_MAX ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_COUNTER_MAX ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(255u)) 


/*===========================================================================*\
* Global function declarations
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
/**
 * @brief Reverses arrays with variable length of 1, 2 or 4 bytes. Dependent on whether arrays are given.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Lcda_Customer_Cal_Reverse_Array_Lcda_Cal(Lcda_Customer_Calibration_T* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Lcda_Customer_Cal_Print(FILE* c_file_ptr, const Lcda_Customer_Calibration_T* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Lcda_Customer_Cal_Update_Defaults(Lcda_Customer_Calibration_T* cal_dst);



#endif /* LCDA_CUSTOMER_CALIBRATION_H */
