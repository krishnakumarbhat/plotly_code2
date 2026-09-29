#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Honda SRR6 input data structure for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* FBK includes */
#include "pa_data.h"
#include "pa_reuse.h"

typedef enum
{
   RANGE_STT_DEFAULT = (0),
   RANGE_STT_EARLY   = (1),
   RANGE_STT_NORMAL  = (2),
   RANGE_STT_LATE    = (3)
} RANGE_STT_T;

/**
 * @brief  Lcda_Input_T structure summarizes the necessary input data for Honda SRR6 customer.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6992}
 */
typedef struct
{
   /* TODO: Clarify for what the input signales in RNA were used (for me it seems that they are not used anywhere in RNA pre or
    * post run and can thus be reomoved) */

   uint8_t f_lcda_enable;     /* enable/disable LCDA feature */
   uint8_t f_lcda_enable_bsw; /* enable/disable LCDA BSW subfeature */
   uint8_t f_lcda_enable_cvw; /* enable/disable LCDA CVW subfeature */

   boolean_T f_trailer_present; /**< Flag indicating that a trailer is attached */
   float32_T trailer_length;    /**< [m] Trailer length */
   float32_T trailer_width;     /**< [m] Trailer width */
   float32_T trailer_angle;     /**< [rad] Trailer angle */

   boolean_T f_beeper_zone;   /**< Flag indicating that a logic for buzzer zone (Honda alert level 3) is enabled */
   boolean_T f_slide_through; /**< Flag indicating that a logic for slide through (Honda alert level 2) is enabled */

   RANGE_STT_T cvw_range_stt; /**< Input signal for the TTC threshold level for the CVW objects */
} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */
