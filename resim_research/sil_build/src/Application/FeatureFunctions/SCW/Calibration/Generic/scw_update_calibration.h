/**
* @file scw_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef SCW_UPDATE_CALIBRATION_H
#define SCW_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "scw_core_calibration_t.h"
#include "scw_customer_calibration_t.h"
#include "scw_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Scw_Core_Calibration_T and Scw_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Scw_Update_Core_Cal_By_Public(Scw_Core_Calibration_T* cal_dst, const Scw_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Scw_Core_Calibration_T and Scw_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Scw_Update_Core_Cal_By_Core(Scw_Core_Calibration_T* cal_dst, const Scw_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Scw_Customer_Calibration_T and Scw_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Scw_Update_Customer_Cal_By_Public(Scw_Customer_Calibration_T* cal_dst, const Scw_Public_Calibration_T* cal_src);


#endif /* SCW_UPDATE_CALIBRATION_H */

