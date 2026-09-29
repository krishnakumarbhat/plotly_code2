#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Honda_SRR6 customer input declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "pt_output_t.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/* Defines the e-latch information signal. */
typedef enum
{
   HONDA_EW_NO_ZONE       = (0),
   HONDA_EW_SHORT_ZONE    = (1),
   HONDA_EW_STANDARD_ZONE = (2)
} EW_Zone_Information_T;

/**
 * @brief Ced_Input_T Structure summarizing Honda SRR6 specific CED input data.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3493}
 */
typedef struct
{
   boolean_T f_ced_enable;                    /* Flag indicating the status of the CED function */
   boolean_T f_ced_front_mode;                /* Flag indicating the front mode is enabled */
   boolean_T f_ced_rear_mode;                 /* Flag indicating the rear mode is enabled */
   EW_Zone_Information_T ew_elatch_sense_stt; /* E-latch zone information from Honda input */
   uint8_t threshold_timer_for_ced_enable;
} Ced_Input_T;

#endif /* CED_INPUT_T_H */
