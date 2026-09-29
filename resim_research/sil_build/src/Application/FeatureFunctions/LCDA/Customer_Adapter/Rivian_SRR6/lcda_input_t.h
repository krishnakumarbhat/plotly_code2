#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Rivian_SRR6 input data structure for LCDA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/* Defines the HMI warntrigger switches so that warn settings can be set according to the respective situation for the core. */
typedef enum
{
   LCDA_RIVIAN_SRR6_WARNTRIGGER_LATE       = (0),
   LCDA_RIVIAN_SRR6_WARNTRIGGER_NORMAL     = (1),
   LCDA_RIVIAN_SRR6_WARNTRIGGER_EARLY      = (2),
   LCDA_RIVIAN_SRR6_WARNTRIGGER_VERY_EARLY = (3)
} Lcda_Rivian_Srr6_Hmi_Warntrigger_T;

/**
 * @brief Lcda_Input_T structure
 */
typedef struct
{
   boolean_T f_lcda_enable;     /**< Flag indicating that LCDA is enabled */
   boolean_T f_lcda_enable_bsw; /**< Flag indicating that BSW is enabled */
   boolean_T f_lcda_enable_cvw; /**< Flag indicating that CVW is enabled */

   Lcda_Rivian_Srr6_Hmi_Warntrigger_T lcda_warntrigger_hmi; /* LCDA warntrigger (early, middle, late) which driver can choose */

   boolean_T f_trailer_present; /**< Flag indicating that a trailer is attached */
   float32_T trailer_length;    /**< [m] Trailer length */
   float32_T trailer_width;     /**< [m] Trailer width */
   float32_T trailer_angle;     /**< [rad] Trailer angle */

} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */
