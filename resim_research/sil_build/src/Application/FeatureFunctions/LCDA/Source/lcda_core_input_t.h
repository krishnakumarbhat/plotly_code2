#ifndef LCDA_CORE_INPUT_T_H
#define LCDA_CORE_INPUT_T_H

/**
 * @file lcda_core_input.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Definition of LCDA core input struct
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_types.h"
#include "pa_data.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Core_Enabled_Flags_T structure summarizes the enable flags which are part of Lcda and also part of the input
 * interface.
 *
 * @SDD{SF-6513}
 */
typedef struct
{
   boolean_T f_lcda_enabled; /**< Flag indicating that LCDA is enabled */

   boolean_T f_bsw_enabled; /**< Flag indicating that BSW is enabled */
   boolean_T f_cvw_enabled; /**< Flag indicating that CVW is enabled */
   boolean_T f_slc_enabled; /**< Flag indicating that Sim Lane Change is enabled */
   boolean_T f_elc_enabled; /**< Flag indicating that Evasive Lane Change is enabled */

   boolean_T f_dropback_enabled; /**< Flag indicating that dropback handler is enabled */
   boolean_T f_fallback_enabled; /**< Flag indicating that fallback handler is enabled */

} Lcda_Core_Enabled_Flags_T;

/**
 * @brief Lcda_Core_Input_T structure summarizes the input interface of Lcda.
 *
 * @SRS{SF-981,SF-1049}
 * @SAE{SF-2811,SF-2814,SF-2816}
 * @SDD{SF-6514}
 */
typedef struct
{
   Fbk_Field_Of_Interest_T initial_bsw_zone; /**< Initial BSW zone in case of not using the default method of zone calculation */
   Fbk_Field_Of_Interest_T initial_bsw_zone_hys; /**< Initial BSW hysteresis zone */
   Fbk_Field_Of_Interest_T initial_cvw_zone;     /**< Initial CVW zone */
   Fbk_Field_Of_Interest_T initial_cvw_zone_hys; /**< Initial CVW hysteresis zone */

   float32_T lane_width;         /**< [m] Lane width from external sources for example from camera info */
   float32_T lane_center_offset; /**< [m] Lane center offset in VCS co-ord (positive when to the right of the ego, negative
                                  when to the left of the ego) */


   float32_T lane_lateral_speed[FBK_NUMBER_OF_SIDES]; /**< [m/s] Lateral Speed of the ego (positive when to the right of the ego,
                                  negative when to the left of the ego) */

   Lcda_Warn_Settings_T warn_settings;                           /**< Struct for warn settings */
   Lcda_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES]; /**< Struct for guardrail data */
   Lcda_Core_Enabled_Flags_T enabled_flags;                      /**< Struct for enable flags */

   Lcda_Bsw_Zone_Calculation_Mode_T bsw_zone_calculation_mode; /* BSW zone calculation mode */
   boolean_T f_lane_change[FBK_NUMBER_OF_SIDES]; /**< Flag indicating that the host is changing lanes in direction of the side */
   Lcda_Cvw_Criticality_Mode_T cvw_crit_mode[FBK_NUMBER_OF_SIDES]; /**< Flag indicating if critical object is taken by
                                                                                  lateral distance */
   Lcda_Trailer_Object_T trailer; /**< Information about the trailer that might be attached to the host vehicle */

   const Pa_Data_T *p_pa_data; /**< Pointer to pa_data containing the vehicle and tracker data */

} Lcda_Core_Input_T;

#endif /* LCDA_CORE_INPUT_T_H*/
