#ifndef RECW_H
#define RECW_H

/**
 * @file recw.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is main RECW algorithm header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_core_calibration_t.h"
#include "recw_core_input_t.h"
#include "recw_core_output_t.h"
#include "recw_persistent_t.h"

/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Resets the RECW core output and persistent data.
 *
 * @return void
 *
 * @SRS{SF-1662}
 * @SAE{SF-2959}
 * @SDD{SF-7851}
 * @verification{}
 */
void Recw_Reset(Recw_Core_Output_T *p_core_output, Recw_Persistent_T *p_recw_persistent);

/**
 * @brief Runs the main routine of Recw in case it is enabled.
 *
 * @return void
 *
 * @SRS{SF-1662,SF-1680}
 * @SAE{SF-2959}
 * @SDD{SF-7852}
 * @verification{}
 */
void Recw_Core_Run(Recw_Core_Output_T *p_core_output,
                   const Recw_Core_Input_T *p_recw_core_input,
                   const Recw_Core_Calibration_T *p_cals,
                   Recw_Persistent_T *p_recw_persistent);

/**
 * @brief Resets every persistent data of Recw.
 *
 * @return void
 *
 * @SRS{SF-1662}
 * @SAE{SF-2959}
 * @SDD{SF-7841}
 * @verification{}
 */
void Recw_Reset_Persistent(Recw_Persistent_T *p_persistent /**< RECW persistent data */);

/**
 * @brief Resets the core output to default values.
 *
 * @return void
 *
 * @SRS{SF-1697}
 * @SAE{SF-2959}
 * @SDD{SF-7850}
 * @verification{}
 */
void Recw_Reset_Core_Output(Recw_Core_Output_T *p_core_output /**< Recw core output*/);
#endif /* RECW_H */
