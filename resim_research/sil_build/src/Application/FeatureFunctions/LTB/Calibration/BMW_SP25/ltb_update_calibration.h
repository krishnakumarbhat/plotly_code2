/**
* @file ltb_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef LTB_UPDATE_CALIBRATION_H
#define LTB_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "ltb_core_calibration_t.h"
#include "ltb_customer_calibration_t.h"
#include "ltb_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Ltb_Core_Calibration_T and Ltb_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ltb_Update_Core_Cal_By_Public(Ltb_Core_Calibration_T* cal_dst, const Ltb_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ltb_Core_Calibration_T and Ltb_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ltb_Update_Core_Cal_By_Core(Ltb_Core_Calibration_T* cal_dst, const Ltb_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Ltb_Customer_Calibration_T and Ltb_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Ltb_Update_Customer_Cal_By_Public(Ltb_Customer_Calibration_T* cal_dst, const Ltb_Public_Calibration_T* cal_src);


#endif /* LTB_UPDATE_CALIBRATION_H */

