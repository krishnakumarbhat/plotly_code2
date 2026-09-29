/**
* @file lcda_customer_calibration_check.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides boundary checks for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef LCDA_CUSTOMER_CALIBRATION_CAL_CHECK_H
#define LCDA_CUSTOMER_CALIBRATION_CAL_CHECK_H

/**************************************************
 * Includes
 **************************************************/

#include "lcda_customer_calibration_t.h"
#include "pa_reuse.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief Checks whether all calibrations of Lcda are in given boundaries
 *
 * @return True when containing calibrations are within their boundaries
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{Create a superordinate test to check whether all calibrations are in given boundaries}
 **/
boolean_T Lcda_Customer_Cal_In_Boundary(const Lcda_Customer_Calibration_T *p_calibration);

#endif /*LCDA_CUSTOMER_CALIBRATION_CAL_CHECK_H*/

