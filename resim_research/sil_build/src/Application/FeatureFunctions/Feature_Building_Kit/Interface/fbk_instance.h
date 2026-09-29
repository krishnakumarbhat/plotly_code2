#ifndef FBK_INSTANCE_H
#define FBK_INSTANCE_H

/**
 * @file fbk_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the FBK instance data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_core_calibration_t.h"
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_host_trail.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "pa_data.h"
/**
 * @brief Fbk_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_4_violation][Type has tag "Fbk_Instance_T" but that tag is never used.] */
typedef struct Fbk_Instance_T
{
#ifdef __cplusplus
   Fbk_Core_Calibration_T calibration;
#else  //#ifdef __cplusplus
   Fbk_Core_Calibration_T calibration;
#endif //#ifdef __cplusplus
   Fbk_Host_Trail_T host_trail;
   Fbk_Index_Id_Lookup_Table_T fbk_index_id_lookup_table;
   Fbk_Age_Ctr_T fbk_obj_ages;
   Pa_Data_T *p_pa_data;
} Fbk_Instance_T;

#endif /* FBK_INSTANCE_H */
