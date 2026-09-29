#ifndef FBK_OUTPUT_H
#define FBK_OUTPUT_H

/**
 * @file fbk_output.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the FBK output data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "pa_data.h"
/**
 * @brief Fbk_Output_T structure
 *
 * @SDD{}
 * @verification{}
 */
typedef struct
{
   const Fbk_Host_Trail_T *p_host_trail;
   const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table;
   const Pa_Data_T *p_pa_data;
} Fbk_Output_T;

#endif /* FBK_OUTPUT_H */
