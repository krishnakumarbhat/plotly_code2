/**
* @file cta_customer_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "cta_customer_calibration_check.h" // IWYU pragma: keep
#include "cta_customer_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Cta_Customer_Cal_In_Boundary(const Cta_Customer_Calibration_T *p_calibration)
{
   boolean_T f_cta_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   
   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_D_C_LINE, p_calibration->k_stla_crit_zone_D_C_line, CTA_MAX_K_STLA_CRIT_ZONE_D_C_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_G_E_LINE, p_calibration->k_stla_crit_zone_G_E_line, CTA_MAX_K_STLA_CRIT_ZONE_G_E_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_N_Q_LINE, p_calibration->k_stla_crit_zone_N_Q_line, CTA_MAX_K_STLA_CRIT_ZONE_N_Q_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_Q_QH_LINE, p_calibration->k_stla_crit_zone_Q_QH_line, CTA_MAX_K_STLA_CRIT_ZONE_Q_QH_LINE);


   return f_cta_calibration_in_boundaries;
}

