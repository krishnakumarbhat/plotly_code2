/**
* @file ta_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef TA_UPDATE_CALIBRATION_H
#define TA_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_customer_calibration_t.h"
#include "ta_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Ta_Core_Calibration_T and Ta_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ta_Update_Core_Cal_By_Public(Ta_Core_Calibration_T* cal_dst, const Ta_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ta_Core_Calibration_T and Ta_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ta_Update_Core_Cal_By_Core(Ta_Core_Calibration_T* cal_dst, const Ta_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ta_Customer_Calibration_T and Ta_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ta_Update_Customer_Cal_By_Public(Ta_Customer_Calibration_T* cal_dst, const Ta_Public_Calibration_T* cal_src);


#endif /* TA_UPDATE_CALIBRATION_H */

