/**
* @file cta_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef CTA_UPDATE_CALIBRATION_H
#define CTA_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "cta_core_calibration_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Cta_Core_Calibration_T and Cta_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Cta_Update_Core_Cal_By_Public(Cta_Core_Calibration_T* cal_dst, const Cta_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Cta_Core_Calibration_T and Cta_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Cta_Update_Core_Cal_By_Core(Cta_Core_Calibration_T* cal_dst, const Cta_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Cta_Customer_Calibration_T and Cta_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Cta_Update_Customer_Cal_By_Public(Cta_Customer_Calibration_T* cal_dst, const Cta_Public_Calibration_T* cal_src);


#endif /* CTA_UPDATE_CALIBRATION_H */

