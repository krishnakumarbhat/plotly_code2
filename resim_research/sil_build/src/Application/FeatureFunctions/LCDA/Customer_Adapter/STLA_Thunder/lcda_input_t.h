#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the STLA_Thunder input data structure for LCDA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Input_T structure
 */
typedef struct
{
   boolean_T f_lcda_enable;     /**< Flag indicating that LCDA is enabled */
   boolean_T f_lcda_enable_bsw; /**< Flag indicating that BSW is enabled */
   boolean_T f_lcda_enable_cvw; /**< Flag indicating that CVW is enabled */

   boolean_T f_trailer_present; /**< Flag indicating that a trailer is attached */
   float32_T trailer_length;    /**< [m] Trailer length */
   float32_T trailer_width;     /**< [m] Trailer width */
   float32_T trailer_angle;     /**< [rad] Trailer angle */

} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */
