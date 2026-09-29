/**
* @file ced_customer_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ced_customer_calibration_check.h" // IWYU pragma: keep
#include "ced_customer_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ced_Customer_Cal_In_Boundary(const Ced_Customer_Calibration_T *p_calibration)
{
   boolean_T f_ced_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS, p_calibration->k_ced_honda_srr6_custom_ttc_alert_hysteresis[x], CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD, p_calibration->k_ced_honda_srr6_custom_ttc_alert_threshold[x], CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD, p_calibration->k_ced_honda_srr6_long_dist_threshold[x], CED_MAX_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_HONDA_ELATCH_ZONES_WIDTH_TABLE, p_calibration->k_honda_elatch_zones_width_table[x], CED_MAX_K_HONDA_ELATCH_ZONES_WIDTH_TABLE);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Bool_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD, p_calibration->k_ced_f_honda_use_alert_ttc_threshold, CED_MAX_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT, p_calibration->k_ced_honda_object_acceleration_weight, CED_MAX_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_HONDA_MIN_ALERT_DURATION, p_calibration->k_honda_min_alert_duration, CED_MAX_K_HONDA_MIN_ALERT_DURATION);
Ct_Is_Float_In_Bondaries(&f_ced_calibration_in_boundaries, CED_MIN_K_HONDA_MIN_ERATCH_ALERT_DURATION, p_calibration->k_honda_min_eratch_alert_duration, CED_MAX_K_HONDA_MIN_ERATCH_ALERT_DURATION);


   return f_ced_calibration_in_boundaries;
}

