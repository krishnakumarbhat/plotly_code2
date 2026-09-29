#ifndef FBK_GUARDRAIL_DATA_T_H
#define FBK_GUARDRAIL_DATA_T_H

/**
 * @file fbk_guardrail_data_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for fbk guardrail data type definition.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Typedef
\*===========================================================================*/

/**
 * @brief Fbk_Guardrail_Data_T structure
 *
 * Summarizes the consumed guardrail data of the Sfl stack.
 *
 */
typedef struct
{
   boolean_T f_active;              /** is guardrail detection activated */
   boolean_T f_present;             /** is any guardrail detected */
   Pa_Obj_Status_T status;          /**< available values are INVALID, NEW, MATURE, COASTED and COASTED_IMPLAUSIBLE */
   float32_T lat_pos;               /** [m] distance to guardrail from VCS */
   float32_T existence_probability; /** [0,1] existence probability of guardrail */
   uint8_t age;                     /**< number of scans this object has existed */
} Fbk_Guardrail_Data_T;

#endif /* FBK_GUARDRAIL_DATA_T_H */
