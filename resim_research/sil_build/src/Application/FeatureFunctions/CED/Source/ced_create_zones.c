/**
 * @file ced_create_zones.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions to set up CED zones.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_create_zones.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

void Ced_Create_Funnel_Zone(Fbk_Field_Of_Interest_T *p_funnel_zone,
                            const Fbk_Vehicle_Data_T *p_vehicle_data,
                            const Ced_Core_Calibration_T *p_ced_cal)
{
   /* Asserts */
   assert(NULL != p_funnel_zone);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);
   /* Construct funnel zone */
   /*  x = long, y = lat    */
   /*       0 --------- 1   */
   /*       |         /     */
   /*       |        /      */
   /*       |       /       */
   /* [ego] 3 ---- 2        */

   p_funnel_zone->size = CED_NUMBER_OF_ZONE_POINTS;

   /* Zone is offset to be in positive coordinate quadrant */
   p_funnel_zone->points[0] = Create_2d_Vector_Coordinates(p_ced_cal->k_ced_funnel_zone_length, FBK_ZERO_F);
   p_funnel_zone->points[1] = Create_2d_Vector_Coordinates(p_ced_cal->k_ced_funnel_zone_length, p_ced_cal->k_ced_funnel_zone_width);
   p_funnel_zone->points[2] =
      Create_2d_Vector_Coordinates(FBK_ZERO_F, (Fbk_Half(p_vehicle_data->host_width) + p_ced_cal->k_ced_collision_zone_width));
   p_funnel_zone->points[3] = Create_2d_Vector_Coordinates(FBK_ZERO_F, FBK_ZERO_F);
}

void Ced_Create_Collision_Zone(Fbk_Field_Of_Interest_T *p_collision_zone,
                               const Fbk_Vehicle_Data_T *p_vehicle_data,
                               const Ced_Core_Calibration_T *p_ced_cal)
{
   float32_T half_ego_width;

   /* Asserts */
   assert(NULL != p_collision_zone);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);

   /* set up collision zones */
   /*  x = long, y = lat     */
   /*    [   ] 0 ---- 1      */
   /*    [   ] |      |      */
   /*    [ego] |      |      */
   /*    [   ] |      |      */
   /*    [   ] 3 ---- 2      */

   p_collision_zone->size = CED_NUMBER_OF_ZONE_POINTS;

   half_ego_width              = Fbk_Half(p_vehicle_data->host_width);
   p_collision_zone->points[0] = Create_2d_Vector_Coordinates(FBK_ZERO_F, half_ego_width);
   p_collision_zone->points[1] = Create_2d_Vector_Coordinates(FBK_ZERO_F, half_ego_width + p_ced_cal->k_ced_collision_zone_width);
   p_collision_zone->points[2] =
      Create_2d_Vector_Coordinates(-p_vehicle_data->host_length, half_ego_width + p_ced_cal->k_ced_collision_zone_width);
   p_collision_zone->points[3] = Create_2d_Vector_Coordinates(-p_vehicle_data->host_length, half_ego_width);
}
