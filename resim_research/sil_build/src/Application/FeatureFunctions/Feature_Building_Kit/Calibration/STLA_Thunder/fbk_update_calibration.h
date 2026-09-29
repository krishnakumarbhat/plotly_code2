/**
* @file fbk_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef FBK_UPDATE_CALIBRATION_H
#define FBK_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "fbk_core_calibration_t.h"
#include "fbk_customer_calibration_t.h"
#include "fbk_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Fbk_Core_Calibration_T and Fbk_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Fbk_Update_Core_Cal_By_Public(Fbk_Core_Calibration_T* cal_dst, const Fbk_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Fbk_Core_Calibration_T and Fbk_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Fbk_Update_Core_Cal_By_Core(Fbk_Core_Calibration_T* cal_dst, const Fbk_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Fbk_Customer_Calibration_T and Fbk_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Fbk_Update_Customer_Cal_By_Public(Fbk_Customer_Calibration_T* cal_dst, const Fbk_Public_Calibration_T* cal_src);


#endif /* FBK_UPDATE_CALIBRATION_H */

