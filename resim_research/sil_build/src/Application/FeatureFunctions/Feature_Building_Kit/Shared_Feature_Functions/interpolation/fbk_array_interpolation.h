#ifndef FBK_ARRAY_INTERPOLATION_H
#define FBK_ARRAY_INTERPOLATION_H

/**
 * @file fbk_array_interpolation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions for array interpolation.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief Assumes that the array is orderd in a ascending order, x is inside the intervals defined by the array and
 * the length of the array is at least 2.
 * The index of the upper bound of the interval is returned. Prerequisites: It is assumed that x is forced onto the range of input
 * array.
 *
 * @return index of the upper bound
 *
 * @SRS{SF-264}
 * @SAE{SF-2552}
 * @SDD{SF-4085}
 * @verification{}
 */
uint8_t Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(const uint8_t *p_array_in /**< LUT which is considered in that case */,
                                           const uint8_t length_of_array /**< Length of the array provided */,
                                           const uint8_t val_to_check /**< Value which represents border to check against */);


/**
 * @brief Assumes that the array is orderd in a ascending order, x is inside the intervals defined by the array and
 * the length of the array is at least 2.
 * The index of the upper bound of the interval is returned. Prerequisites: It is assumed that x is forced onto the range of input
 * array.
 *
 * @return index of the upper bound
 *
 * @SRS{SF-264}
 * @SAE{SF-2552}
 * @SDD{SF-4086}
 * @verification{}
 */
uint8_t Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(const float32_T *p_array_in /**< LUT which is considered in that case */,
                                           const uint8_t length_of_array /**< Length of the array provided */,
                                           const float32_T val_to_check /**< Value which represents border to check against */);

#endif /* FBK_ARRAY_INTERPOLATION_H */
