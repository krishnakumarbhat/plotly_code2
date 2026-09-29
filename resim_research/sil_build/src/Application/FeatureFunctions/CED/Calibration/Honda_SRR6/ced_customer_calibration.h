# ifndef CED_CUSTOMER_CALIBRATION_H
# define CED_CUSTOMER_CALIBRATION_H

/**
* @file ced_customer_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ced_customer_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_MIN_ALERT_DURATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_ELATCH_ZONES_WIDTH_TABLE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_MIN_ERATCH_ALERT_DURATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_UNUSED_PADDING_BYTE_2 ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD ((float32_T)(40.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_MIN_ALERT_DURATION ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_ELATCH_ZONES_WIDTH_TABLE ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_MIN_ERATCH_ALERT_DURATION ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_UNUSED_PADDING_BYTE_2 ((uint8_t)(255u)) 


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
void Ced_Customer_Cal_Reverse_Array_Ced_Cal(Ced_Customer_Calibration_T* cal_dst);
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
void Ced_Customer_Cal_Print(FILE* c_file_ptr, const Ced_Customer_Calibration_T* p_cals);

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
void Ced_Customer_Cal_Update_Defaults(Ced_Customer_Calibration_T* cal_dst);



#endif /* CED_CUSTOMER_CALIBRATION_H */
