/**
* @file pt_update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef PT_UPDATE_CALIBRATION_H
#define PT_UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "pa_reuse.h"
#include "pt_core_calibration_t.h"
#include "pt_customer_calibration_t.h"
#include "pt_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Pt_Core_Calibration_T and Pt_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Pt_Update_Core_Cal_By_Public(Pt_Core_Calibration_T* cal_dst, const Pt_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Pt_Core_Calibration_T and Pt_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Pt_Update_Core_Cal_By_Core(Pt_Core_Calibration_T* cal_dst, const Pt_Core_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Pt_Customer_Calibration_T and Pt_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Pt_Update_Customer_Cal_By_Public(Pt_Customer_Calibration_T* cal_dst, const Pt_Public_Calibration_T* cal_src);


#endif /* PT_UPDATE_CALIBRATION_H */

