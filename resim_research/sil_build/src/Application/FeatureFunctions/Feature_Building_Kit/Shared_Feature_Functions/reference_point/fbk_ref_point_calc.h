#ifndef FBK_REF_POINT_CALC_H
#define FBK_REF_POINT_CALC_H

/**
 * @file fbk_ref_point_calc.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions for reference point calculations.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ref_point.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief Returns a field of interest formed by the given x and y coordinates.
 *
 * @return void
 *
 * @SRS{SF-269}
 * @SAE{SF-2552}
 * @SDD{SF-4095}
 * @verification{}
 */
void Fbk_Calculate_Target_Corners(Fbk_Object_Corners_T *p_target_corners /**< target corners */,
                                  const Vector_2d_T *p_target_vcs_pos /**< position of the target in vcs */,
                                  const float32_T *p_heading /**< heading of the target */,
                                  const float32_T *p_length /**< length of the target */,
                                  const float32_T *p_width /**< width of the target */);

/**
 * @brief Calculates the reference point of a given target.
 *
 * @return void
 *
 * @SRS{SF-268}
 * @SAE{SF-2552}
 * @SDD{SF-4096}
 * @verification{}
 */
void Fbk_Calculate_Ref_Point(
   Fbk_Ref_Point_T *p_target_ref_point /**< target reference point */,
   const Vector_2d_T *p_host_ref_point /**< host reference point */,
   const Fbk_Object_Corners_T *p_target_corners /**< target corners */,
   const boolean_T f_use_lateral_approximation /**< flag indicating whether lateral approximation should be used */);

/**
 * @brief  Returns the reference point on the opposite side of the vehicle ( left - right)
 *
 * @return Fbk_Reference_Position_T
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4252}
 * @verification{}
 */
Fbk_Reference_Position_T Fbk_Get_Opposite_Point(const Fbk_Reference_Position_T point_index);

#endif /* FBK_REF_POINT_CALC_H */
