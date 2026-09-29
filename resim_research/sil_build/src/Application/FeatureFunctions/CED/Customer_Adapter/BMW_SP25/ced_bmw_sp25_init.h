#ifndef CED_BMW_SP25_INIT_H
#define CED_BMW_SP25_INIT_H

/**
 * @file ced_bmw_sp25_init.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 initialization functions for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_bmw_sp25_types.h"
#include "ced_input_t.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

/**
 * @brief Initialization of the CED boardnet signals.
 *
 * @return filling p_bmw_boardnet_signals
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Init_Boardnet_Signals(Ced_Bmw_Boardnet_T *p_bmw_boardnet_signals);

/**
 * @brief Initialization of the CED coding parameters.
 *
 * @return filling p_ced_coding_parameters
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Init_Coding_Parameters(Ced_Coding_Parameters_T *p_ced_coding_parameters);

/**
 * @brief Initialization of the CED Input Bus Signals.
 *
 * @return filling p_bmw_ced_input_bus_signals
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Init_Input_Bus_Signals(Bmw_Ced_Input_Bus_Signals_T *p_bmw_ced_input_bus_signals);

#endif /* CED_BMW_SP25_INIT_H */
