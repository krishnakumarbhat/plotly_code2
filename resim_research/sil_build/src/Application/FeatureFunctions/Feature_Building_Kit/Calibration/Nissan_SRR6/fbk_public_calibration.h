# ifndef FBK_PUBLIC_CALIBRATION_H
# define FBK_PUBLIC_CALIBRATION_H

/**
* @file fbk_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "fbk_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MIN_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MIN_K_FBK_HOST_TRAIL_DIST_SEPARATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MIN_K_FBK_HOST_TRAIL_HEADING_SEPARATION ((float32_T)(0.0f)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MAX_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED ((float32_T)(30.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MAX_K_FBK_HOST_TRAIL_DIST_SEPARATION ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_MAX_K_FBK_HOST_TRAIL_HEADING_SEPARATION ((float32_T)(3.141f)) 


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
void Fbk_Public_Cal_Reverse_Array_Fbk_Cal(Fbk_Public_Calibration_T* cal_dst);
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
void Fbk_Public_Cal_Print(FILE* c_file_ptr, const Fbk_Public_Calibration_T* p_cals);

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
void Fbk_Public_Cal_Update_Defaults(Fbk_Public_Calibration_T* cal_dst);



#endif /* FBK_PUBLIC_CALIBRATION_H */
