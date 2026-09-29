#ifndef SCW_INPUT_T_H
#define SCW_INPUT_T_H

/**
 * @file scw_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the input data structure for SCW.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/
typedef struct
{

   boolean_T f_scw_enable;           /**< enable/disable SCW feature */
   boolean_T f_scw_enable_dynamic;   /**< enable/disable SCW dynamic object */
   boolean_T f_scw_enable_guardrail; /**< enable/disable SCW guardrail sub */

   boolean_T f_trailer_present; /**< Flag indicating that a trailer is attached */
   float32_T trailer_length;    /**< [m] Trailer length */
   float32_T trailer_width;     /**< [m] Trailer width */
   float32_T trailer_angle;     /**< [rad] Trailer angle */
} Scw_Input_T;

#endif /* SCW_INPUT_T_H */
