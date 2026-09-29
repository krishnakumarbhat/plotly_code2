/**
* @file ced_public_calibration_check.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides boundary checks for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef CED_PUBLIC_CALIBRATION_CAL_CHECK_H
#define CED_PUBLIC_CALIBRATION_CAL_CHECK_H

/**************************************************
 * Includes
 **************************************************/

#include "ced_public_calibration_t.h"
#include "pa_reuse.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief Checks whether all calibrations of Ced are in given boundaries
 *
 * @return True when containing calibrations are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a superordinate test to check whether all calibrations are in given boundaries}
 **/
boolean_T Ced_Public_Cal_In_Boundary(const Ced_Public_Calibration_T *p_calibration);

#endif /*CED_PUBLIC_CALIBRATION_CAL_CHECK_H*/

