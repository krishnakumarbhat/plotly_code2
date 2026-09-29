/**
* @file esa_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in esa_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef ESA_UPDATE_CALIBRATION_H
#define ESA_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "esa_core_calibration_t.h"
#include "esa_customer_calibration_t.h"
#include "esa_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Esa_Core_Calibration_T and Esa_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Esa_Update_Core_Cal_By_Public(Esa_Core_Calibration_T* cal_dst, const Esa_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Esa_Core_Calibration_T and Esa_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Esa_Update_Core_Cal_By_Core(Esa_Core_Calibration_T* cal_dst, const Esa_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Esa_Customer_Calibration_T and Esa_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Esa_Update_Customer_Cal_By_Public(Esa_Customer_Calibration_T* cal_dst, const Esa_Public_Calibration_T* cal_src);


#endif /* ESA_UPDATE_CALIBRATION_H */

