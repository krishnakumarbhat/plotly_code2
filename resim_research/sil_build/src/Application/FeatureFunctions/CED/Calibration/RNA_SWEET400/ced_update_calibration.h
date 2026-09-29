/**
* @file ced_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef CED_UPDATE_CALIBRATION_H
#define CED_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "ced_core_calibration_t.h"
#include "ced_customer_calibration_t.h"
#include "ced_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Ced_Core_Calibration_T and Ced_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ced_Update_Core_Cal_By_Public(Ced_Core_Calibration_T* cal_dst, const Ced_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ced_Core_Calibration_T and Ced_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ced_Update_Core_Cal_By_Core(Ced_Core_Calibration_T* cal_dst, const Ced_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ced_Customer_Calibration_T and Ced_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ced_Update_Customer_Cal_By_Public(Ced_Customer_Calibration_T* cal_dst, const Ced_Public_Calibration_T* cal_src);


#endif /* CED_UPDATE_CALIBRATION_H */

