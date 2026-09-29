/**
* @file fbk_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "fbk_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "fbk_core_calibration_check.h"
#include "fbk_core_calibration_t.h"
#include "fbk_customer_calibration_t.h"
#include "fbk_public_calibration_check.h"
#include "fbk_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Fbk_Update_Core_Cal_By_Public(Fbk_Core_Calibration_T* cal_dst, const Fbk_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Fbk_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_fbk_host_trail_max_recording_speed = cal_src->k_fbk_host_trail_max_recording_speed;
        cal_dst->k_fbk_host_trail_dist_separation = cal_src->k_fbk_host_trail_dist_separation;
        cal_dst->k_fbk_host_trail_heading_separation = cal_src->k_fbk_host_trail_heading_separation;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Fbk_Update_Core_Cal_By_Core(Fbk_Core_Calibration_T* cal_dst, const Fbk_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Fbk_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_fbk_host_trail_max_recording_speed = cal_src->k_fbk_host_trail_max_recording_speed;
        cal_dst->k_fbk_host_trail_dist_separation = cal_src->k_fbk_host_trail_dist_separation;
        cal_dst->k_fbk_host_trail_heading_separation = cal_src->k_fbk_host_trail_heading_separation;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Fbk_Update_Customer_Cal_By_Public(Fbk_Customer_Calibration_T* cal_dst, const Fbk_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Fbk_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


