#ifndef RECW_INPUT_T_H
#define RECW_INPUT_T_H

/**
 * @file recw_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements input type definition for bmw_sp25 project.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"
#include "recw_bmw_sp25_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/


/**
 * @brief Definition of the Recw_Input_T structure
 *
 * @SRS{SF-1665}
 * @SAE{SF-2962}
 * @SDD{SF-7987}
 */
typedef struct
{
   float32_T accelerator_pedal_gradient; /**< Gradient of accelerator pedal */
   Recw_Vehicle_Movement_Status_T vehicle_movement_status;
   Recw_Trailer_Status_T status_trailer;
   Status_Dynamometer_T status_roller_dynamometer;
   Status_End_Of_Line_T status_end_of_line;
   Recw_State_T c_recw_enable; /**< Coding Parameter indicating if the RECW feature should be operational (coding parameter) */
   Recw_Bmw_Sp25_Type_Input_T recw_type; /**< Indicating if RECW feature should output warning and precrash, only warning, only
                         precrash or nothing at
                         all (coding paramter)*/
   Recw_Error_T recw_error;       /**< Function parameters. TODO: Degradation matrix to be designed and this is to be provided from
                                       that*/
   uint8_t f_recw_enable_m_drive; /**< describes the deactivation requirement for the RECW function from the M Drive Master*/
} Recw_Input_T;

#endif /* RECW_INPUT_T_H */
