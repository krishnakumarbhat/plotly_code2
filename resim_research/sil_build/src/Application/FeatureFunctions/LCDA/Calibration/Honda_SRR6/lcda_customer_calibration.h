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
#define LCDA_MIN_K_HONDA_MAX_HOLD_TIME_AFTER_OUT_OF_FOV ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_EGO_SPEED_STOP_HOLDING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_MIN_RELATIVE_SPEED_FOR_ALERT_LEVEL_TWO ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_EGO_LAT_OVERLAP_SLIDE_THROUGH_ZONE ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_OBJECT_LAT_OVERLAP_SLIDE_THROUGH_ZONE ((float32_T)(-0.2f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_LONG_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_LAT_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_IS_SLIDE_THROUGH_ZONE_CONSIDERED_FOR_CVW_ALERT_LEVEL_TWO ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_MAX_HOLD_TIME_AFTER_OUT_OF_FOV ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_EGO_SPEED_STOP_HOLDING ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_MIN_RELATIVE_SPEED_FOR_ALERT_LEVEL_TWO ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_EGO_LAT_OVERLAP_SLIDE_THROUGH_ZONE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_OBJECT_LAT_OVERLAP_SLIDE_THROUGH_ZONE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_LENGTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_LONG_HYS ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_LAT_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_IS_SLIDE_THROUGH_ZONE_CONSIDERED_FOR_CVW_ALERT_LEVEL_TWO ((boolean_T)(1u)) 
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
