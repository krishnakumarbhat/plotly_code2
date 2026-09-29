/**
* @file lcda_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef LCDA_UPDATE_CALIBRATION_H
#define LCDA_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "lcda_core_calibration_t.h"
#include "lcda_customer_calibration_t.h"
#include "lcda_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Lcda_Core_Calibration_T and Lcda_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Lcda_Update_Core_Cal_By_Public(Lcda_Core_Calibration_T* cal_dst, const Lcda_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Lcda_Core_Calibration_T and Lcda_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Lcda_Update_Core_Cal_By_Core(Lcda_Core_Calibration_T* cal_dst, const Lcda_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Lcda_Customer_Calibration_T and Lcda_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Lcda_Update_Customer_Cal_By_Public(Lcda_Customer_Calibration_T* cal_dst, const Lcda_Public_Calibration_T* cal_src);


#endif /* LCDA_UPDATE_CALIBRATION_H */

