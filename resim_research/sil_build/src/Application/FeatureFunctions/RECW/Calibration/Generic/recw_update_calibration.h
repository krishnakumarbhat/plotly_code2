/**
* @file recw_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef RECW_UPDATE_CALIBRATION_H
#define RECW_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "recw_core_calibration_t.h"
#include "recw_customer_calibration_t.h"
#include "recw_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Recw_Core_Calibration_T and Recw_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Recw_Update_Core_Cal_By_Public(Recw_Core_Calibration_T* cal_dst, const Recw_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Recw_Core_Calibration_T and Recw_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Recw_Update_Core_Cal_By_Core(Recw_Core_Calibration_T* cal_dst, const Recw_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Recw_Customer_Calibration_T and Recw_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Recw_Update_Customer_Cal_By_Public(Recw_Customer_Calibration_T* cal_dst, const Recw_Public_Calibration_T* cal_src);


#endif /* RECW_UPDATE_CALIBRATION_H */

