/**
* @file pt_core_calibration_check.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides boundary checks for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef PT_CORE_CALIBRATION_CAL_CHECK_H
#define PT_CORE_CALIBRATION_CAL_CHECK_H

/**************************************************
 * Includes
 **************************************************/

#include "pt_core_calibration_t.h"
#include "pa_reuse.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief Checks whether all calibrations of Pt are in given boundaries
 *
 * @return True when containing calibrations are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216698}
 * @verification{Create a superordinate test to check whether all calibrations are in given boundaries}
 **/
boolean_T Pt_Core_Cal_In_Boundary(const Pt_Core_Calibration_T *p_calibration);

#endif /*PT_CORE_CALIBRATION_CAL_CHECK_H*/

