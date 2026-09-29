/**
 * @file esa_create_zones.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions to set up ESA zones.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "esa_create_zone.h"
#include "esa_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>


/**
 * @brief Creates (mirrors) the left side zone using the right side zone.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-87218}
 * @verification{Create a test which checks that the zone is mirrored correctly.}
 */
static void Esa_Mirror_Zone_Across_Long_Axis(Fbk_Field_Of_Interest_T *p_zone);


static void Esa_Mirror_Zone_Across_Long_Axis(Fbk_Field_Of_Interest_T *p_zone)
{
   uint8_t i;

   /* Assert */
   assert(NULL != p_zone);

   for (i = FBK_ZERO_UINT; i < p_zone->size; i++)
   {
      p_zone->points[i].y = -1.0f * p_zone->points[i].y;
   }
}


/* coverity[misra_c_2012_rule_8_7_violation] */
void Esa_Create_Zone(Esa_Object_T *p_esa_object,
                     const uint8_t mature_count_in_esa_zone,
                     const Esa_Core_Input_T *p_esa_core_input,
                     const Esa_Core_Calibration_T *p_esa_calibration)
{
   float32_T lane_width;
   float32_T lane_center_offset;

   uint8_t p;

   Fbk_Field_Of_Interest_T esa_zone;
   Fbk_Field_Of_Interest_T esa_zone_hys;

   /* Asserts */
   assert(NULL != p_esa_object);
   assert(NULL != p_esa_core_input);
   assert(NULL != p_esa_calibration);

   lane_width =
      Fbk_Clamp(p_esa_core_input->lane_width, p_esa_calibration->k_esa_min_lane_width, p_esa_calibration->k_esa_max_lane_width);
   lane_center_offset = p_esa_core_input->lane_center_offset;

   /* set up zones */
   /*  x = long, y = lat      */
   /*                         */
   /*    ==|==                */
   /*   []/ \[] 5 ---- 0      */
   /*    / ^ \  |      |      */
   /*   /| O |\ |      |      */
   /*   []_|_[] |      |      */
   /*   |_ego_| |      |      */
   /*           4      1      */
   /*           \      |      */
   /*            3 --- 2      */
   /*                         */

   esa_zone.size     = ESA_NUMBER_OF_ZONE_POINTS;
   esa_zone_hys.size = ESA_NUMBER_OF_ZONE_POINTS;

   /* Since the zone will be built for the right hand side initially, */
   /* we mirror the lane center offset to correspond to the right side */
   if (FBK_SIDE_LEFT == p_esa_object->ego_side)
   {
      lane_center_offset = -lane_center_offset;
   }

   for (p = (uint8_t) ESA_FRONT_OUTER_SIDE; p <= (uint8_t) ESA_FRONT_EGO_SIDE; p++)
   {
      /* define x coordinate of esa zone and hysteresis */
      esa_zone.points[p].x = p_esa_calibration->k_esa_zone_x[p];
      if (((uint8_t) ESA_FRONT_OUTER_SIDE == p) || ((uint8_t) ESA_FRONT_EGO_SIDE == p))
      {
         esa_zone_hys.points[p].x = esa_zone.points[p].x + p_esa_calibration->k_esa_zone_x_hys[p];
      }
      else if (((uint8_t) ESA_REAR_OUTER_SIDE == p) || ((uint8_t) ESA_REAR_EGO_SIDE == p))
      {
         esa_zone_hys.points[p].x = esa_zone.points[p].x - p_esa_calibration->k_esa_zone_x_hys[p];
      }
      else
      {
         esa_zone_hys.points[p].x = esa_zone.points[p].x;
      }

      /* define y co-ord of esa zone and calculate hys zone y-offset */
      esa_zone.points[p].y = (lane_width * p_esa_calibration->k_esa_zone_y[p]) + lane_center_offset;
      if ((uint8_t) ESA_REAR_OUTER_SIDE >= p)
      {
         esa_zone_hys.points[p].y = esa_zone.points[p].y + p_esa_calibration->k_esa_zone_y_hys[p];
      }
      else
      {
         esa_zone_hys.points[p].y = esa_zone.points[p].y - p_esa_calibration->k_esa_zone_y_hys[p];
      }
   }

   /* Note the zone is by default built for the right side so mirror the zone for the left side */
   if (FBK_SIDE_LEFT == p_esa_object->ego_side)
   {
      Esa_Mirror_Zone_Across_Long_Axis(&esa_zone);
      Esa_Mirror_Zone_Across_Long_Axis(&esa_zone_hys);
   }

   /* If the object has already been in the zone for some time then use the hysteresis zone */
   if (mature_count_in_esa_zone > p_esa_calibration->k_esa_min_mature_cycles)
   {
      p_esa_object->zone = esa_zone_hys;
   }
   else
   {
      p_esa_object->zone = esa_zone;
   }
}
