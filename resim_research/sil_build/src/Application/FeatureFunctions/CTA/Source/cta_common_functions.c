/**
 * @file cta_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the definitions of common Cta functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_common_functions.h"
#include "fbk_macros.h"
#include "ml_trigonometry.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Cta_Adapt_Intersec_Point_To_Object_Heading(Cta_Object_Attributes_T *p_attributes,
                                                const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                const Cta_Core_Calibration_T *p_cta_cal,
                                                const Cta_Mode_T cta_mode)
{
   float32_T sin_object_heading;
   float32_T cos_object_heading;
   float32_T sin_heading_sat_thres;
   float32_T coordinate_shift;
   float32_T inters_pt_adapt;
   uint8_t side_idx;

   /* Asserts */
   assert(NULL != p_attributes);
   assert(NULL != p_vehicle_data);

   coordinate_shift   = FBK_ZERO_F;
   sin_object_heading = Fast_Sin(p_attributes->CTA_heading);

   sin_heading_sat_thres = Fast_Sin(p_cta_cal->k_cta_min_park_angle);

   /* when target heading is outside the defined interval,
    * heading threshold will be used instead of estimated heading in the following computations */
   if (Fbk_Abs_F(sin_object_heading) < sin_heading_sat_thres)
   {
      sin_object_heading = sin_heading_sat_thres;
      cos_object_heading = Fast_Cos(p_cta_cal->k_cta_min_park_angle);
   }
   else
   {
      cos_object_heading = Fast_Cos(p_attributes->CTA_heading);
   }
   /* Half of ego width is not divided by tanges of heading but instead multiplied with cosine and divided by sine.
    * This is basically the same as using tangens but avoids dividing by an undefined value at 90 or 270 deg.  */
   inters_pt_adapt = Fbk_Abs_F((Fbk_Half(p_vehicle_data->host_width) * cos_object_heading) / sin_object_heading);

   /* In order to calculate the distance of the intersection point to the relevant bumper, the length of the ego vehicle needs to
    * be considered in case of rear CTA. */
   if (CTA_MODE_REAR == cta_mode)
   {
      coordinate_shift = p_vehicle_data->host_length;
   }

   for (side_idx = FBK_SIDE_LEFT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      /* The sign of intersection point adaption is chosen here by comparing the distance to the rear or front bumper of the
       * intersection point after applying the correction with negative and positive sign. The sign is chosen that leads to the
       * closest distance */
      if ((Fbk_Abs_F((p_attributes->long_isect_point_candidate[side_idx][cta_mode] + inters_pt_adapt) + coordinate_shift)
           > Fbk_Abs_F((p_attributes->long_isect_point_candidate[side_idx][cta_mode] - inters_pt_adapt) + coordinate_shift)))
      {
         inters_pt_adapt = -inters_pt_adapt;
      }

      p_attributes->long_isect_point_candidate[side_idx][cta_mode] += inters_pt_adapt;
   }
}
