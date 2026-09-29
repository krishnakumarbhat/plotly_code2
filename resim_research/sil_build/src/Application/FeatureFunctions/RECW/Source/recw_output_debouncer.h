#ifndef RECW_OUTPUT_DEBOUNCER_H
#define RECW_OUTPUT_DEBOUNCER_H

/**
 * @file recw_output_debouncer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW output debouncer header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_core_calibration_t.h"
#include "recw_core_output_t.h"
#include "recw_persistent_t.h"

/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Debounces the alert level.
 *
 * @return void
 *
 * @SRS{SF-1712}
 * @SAE{SF-2959}
 * @SDD{SF-7933}
 * @verification{}
 */
void Recw_Debounce_Alert_Level(Recw_Core_Output_T *p_core_output /**< Core output */,
                               Recw_Persistent_T *p_persistent /**<Recw persistent data*/,
                               const Recw_Core_Calibration_T *p_cals /**< Recw calibration*/);

#endif /* RECW_OUTPUT_DEBOUNCER_H */
