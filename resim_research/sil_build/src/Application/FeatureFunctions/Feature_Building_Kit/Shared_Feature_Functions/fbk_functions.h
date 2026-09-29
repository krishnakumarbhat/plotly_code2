#ifndef FBK_FUNCTIONS_H
#define FBK_FUNCTIONS_H

/**
 * @file fbk_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declare FBK functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "pa_reuse.h"

/*===========================================================================*\
* EXTERNAL FUNCTIONS
\*===========================================================================*/

/**
 * @brief Swaps two values of given type.
 *
 * @SRS{SF-278}
 * @SAE{SF-2552}
 * @SDD{SF-4186}
 */
void Fbk_Swap_Float(float32_T *const x, float32_T *const y);

/**
 * @brief Swaps two values of given type.
 *
 * @SRS{SF-278}
 * @SAE{SF-2552}
 * @SDD{SF-4185}
 */
void Fbk_Swap_Uint8(uint8_t *const x, uint8_t *const y);

/**
 * @brief Checks if given float is in the value range.
 *
 * It checks extended range (hysteresis) if f_check_extended_range is true.
 *
 * @return true if given float is within the specified range.
 *
 * @SRS{CSCSA-116569}
 * @SAE{SF-2552}
 * @SDD{CSCSA-116572}
 */
boolean_T Fbk_Is_Float_In_Given_Range(const float32_T value /**< float input value */,
                                      const float32_T lower_threshold /**< lower threshold */,
                                      const float32_T upper_threshold /**< upper threshold */,
                                      const boolean_T f_check_extended_range /**< flag for extended range check */,
                                      const float32_T lower_threshold_extension /**< lower threshold extension */,
                                      const float32_T upper_threshold_extension /**< upper threshold extension */);

#endif /* FBK_FUNCTIONS_H */
