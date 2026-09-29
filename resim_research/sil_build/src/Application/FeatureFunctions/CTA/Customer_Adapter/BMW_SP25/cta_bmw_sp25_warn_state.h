#ifndef CTA_BMW_SP25_WARN_STATE_H
#define CTA_BMW_SP25_WARN_STATE_H

/**
 * @file cta_bmw_sp25_warn_state.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for BMW_SP25 specific CTA post run contents
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_bmw_sp25_types.h"
#include "cta_input_t.h"
#include "cta_types.h"
#include "fbk_vehicle_data_t.h" // for Fbk_Vehicle_Data_T
/*============================================================================*\
* BMW SPECIFIC TYPE DEFINITIONS
\*============================================================================*/

/**
 * @brief Determine the overall Warning-State of BMW CTA
 *
 * @return Overall Warning-State of type enum
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4043}
 * @verification{}
 */
Bmw_Ctb_Warn_Control_Output_State_T
Cta_Bmw_Ctb_Get_Warn_Output_Control_State(const Cta_Input_T *p_cta_input /**< CTA input */,
                                          const Cta_Crit_Level_T *cta_alert_level /**< CTA-Core-Output alert level */,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< Fbk Vehicle Data */);

#endif /* CTA_BMW_SP25_WARN_STATE_H */
