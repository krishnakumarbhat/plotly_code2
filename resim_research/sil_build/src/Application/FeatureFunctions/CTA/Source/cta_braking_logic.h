#ifndef CTA_BRAKING_LOGIC_H
#define CTA_BRAKING_LOGIC_H

/**
 * @file cta_braking_logic.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides exported function for ctb.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_instance.h"
#include "cta_types.h"
#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"


/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Superordinate function which checks whether the brake qualifier shall be set.
 *
 * @return True when the brake qualifier shall be set.
 *
 * @SRS{SF-242,SF-238}
 * @SAE{SF-2459}
 * @SDD{SF-3732}
 * @verification{Check that the function correctly detects the necessary setting of the brake qualifier.}
 */
boolean_T
Cta_Shall_Brake_Qualifier_Be_Set(uint8_t *p_brake_qual_hold_ctr /**< Holding counter of brake qualifier */,
                                 uint8_t *p_brake_qual_qualification_ctr /**< Qualification counter of brake qualifier */,
                                 boolean_T *p_last_side_brake_qualifier /**< Last Brake qualifier on the specific side */,
                                 Cta_Object_Attributes_T *p_obj_att_highest_crit_side /**< object attributes on respective side */,
                                 const Cta_Instance_T *p_cta_instance /**< CTA instance */,
                                 const uint8_t approach_side /**< approach side */,
                                 const Cta_Mode_T cta_mode_idx /**< cta mode */,
                                 const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle information*/);


#endif /* CTA_BRAKING_LOGIC_H */
