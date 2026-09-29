/**
 * @file cta_conflict_zone_adapter.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the CTA conflict zone adapter functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_conflict_zone_adapter.h"
#include "cta_types.h"
#include "fbk_array_interpolation.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Local Functions Declaration
\*===========================================================================*/

/**
 * @brief Calculates adaption factor based on object heading
 * When the heading is near +-90 deg the function will return an adaption factor of 1, thus conflict zone will be untouched.
 * For approaching targets at an angle increasing to the borders of allowed CTA angles, the adaption
 * factor will increase. This is done so that the level logic calibration farer away from the host will be increased (FCTA),
 * decreased (RCTA)
 *
 * @return void
 *
 * @SRS{SF-229}
 * @SAE{SF-2459}
 * @SDD{SF-3742}
 * @verification{Check that the adaption factor based on object heading is calculated correctly.}
 */
static void
Cta_Calc_Fac_Obj_Head_Adapt(const float32_T obj_heading /**<object heading*/,
                            const Cta_Core_Calibration_T *p_cta_cal /**< Cta calibrations */,
                            Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**< conflict zone extension parameters*/);

/**
 * @brief Calculates an adaption factor with which the line segments within
 * the criticality leve logic are changed, based on the host steering angle
 *
 * @return void
 *
 * @SRS{SF-179}
 * @SAE{SF-2459}
 * @SDD{SF-3743}
 * @verification{Check that the adaption factor based on steering angle is calculated correctly.}
 */
static void Cta_Calc_Fac_Steer_Angle_Adapt(Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**> conflict zone extensions*/,
                                           const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/**
 * @brief Calculates an adaption factor with which the line segments within
 * the criticality leve logic are changed, based on the host speed.
 *
 * @return void
 *
 * @SRS{CSCSA-187872}
 * @SAE{SF-2459}
 * @SDD{CSCSA-187874}
 * @verification{Check that the adaption factor based on host speed is calculated correctly.}
 */
static void Cta_Calc_Fac_Host_Speed_Adapt(Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**> conflict zone extensions*/,
                                          const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/**
 * @brief Interpolates the adaption factor to be applied to the conflict zone length
 * for the given entity (steering angle or host speed) given with help of two calibration tables belonging
 * to the respective entity.
 *
 * @return interpolated factor which is needed for adaption in case of steering angle and host speed of intersection lines
 *
 * @SRS{SF-179}
 * @SAE{SF-2459}
 * @SDD{SF-3744}
 * @verification{Check that the interpolated factor is calculated and returned correctly.}
 */
static float32_T
Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor(const float32_T entity /**<steering angle*/,
                                                    const float32_T *entity_table /**< steering angle table*/,
                                                    const float32_T *entity_factor_table /**< steering factor table*/,
                                                    const uint8_t length_of_entity_table /**< length of the entity table array*/);

/**
 * @brief Applies the activated extensions to the level logic calibrations.
 *
 * @return void
 *
 * @SRS{SF-230}
 * @SAE{SF-2459}
 * @SDD{SF-3740}
 * @verification{Check that the extensions are applied correctly.}
 */
static void
Cta_Apply_Ext_To_Level_Logic_Calib(Cta_Crit_Level_Calibration_T *p_crit_level_calibration /**< criticality level calibration*/,
                                   const Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params /**< conflict zone extension factors*/,
                                   const Cta_Core_Calibration_T *p_cta_cal /**< calibration parameters*/,
                                   const Cta_Mode_T cta_mode /**< mode of cta */);

/**
 * @brief Applies the level extension to the intersection range of longitudinal axis for each level.
 * Here the border which is closer to the host will remain untouched, while the
 * zone border, which is further away from the host, will be modified. This takes the
 * radar direction into account (front/rear)
 *
 * @return void
 *
 * @SRS{SF-230}
 * @SAE{SF-2459}
 * @SDD{SF-3741}
 * @verification{Check that the level extensions are applied correctly.}
 */
static void
Cta_Apply_Factor_For_Level_Extension(Cta_Crit_Level_Calibration_T *p_crit_level_calibration /**< criticality level calibration*/,
                                     const float32_T adaption_factor /**<relative adaption factor*/,
                                     const uint8_t level /**<currently considered level*/,
                                     const Cta_Mode_T cta_mode /**< cta mode */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
void Cta_Calc_Host_Dep_Ext_Fac(Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_fac,
                               const Cta_Core_Calibration_T *p_cta_cal,
                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_cta_cal);
   assert(NULL != p_confl_zone_ext_fac);

   /*Adapt the zone extension by host steering angle */
   if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_steering_angle))
   {
      Cta_Calc_Fac_Steer_Angle_Adapt(p_confl_zone_ext_fac, p_cta_cal, p_vehicle_data);
   }

   /*Adapt the zone extension by host velocity */
   if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_host_speed))
   {
      Cta_Calc_Fac_Host_Speed_Adapt(p_confl_zone_ext_fac, p_cta_cal, p_vehicle_data);
   }
}


void Cta_Adapt_Long_Crit_Level_Ranges(Cta_Crit_Level_Calibration_T *p_crit_level_calibration,
                                      Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params,
                                      const Cta_Object_Data_T *p_object,
                                      const Cta_Core_Calibration_T *p_cta_cal,
                                      const Cta_Mode_T cta_mode)
{
   /* Asserts */
   assert(NULL != p_object);
   assert(NULL != p_cta_cal);
   assert(NULL != p_crit_level_calibration);
   assert(NULL != p_confl_zone_ext_params);

   if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_obj_heading))
   {
      Cta_Calc_Fac_Obj_Head_Adapt(p_object->attributes->CTA_heading, p_cta_cal, p_confl_zone_ext_params);
   }

   /*Merge all extension*/
   Cta_Apply_Ext_To_Level_Logic_Calib(p_crit_level_calibration, p_confl_zone_ext_params, p_cta_cal, cta_mode);
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/
static void Cta_Calc_Fac_Steer_Angle_Adapt(Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params,
                                           const Cta_Core_Calibration_T *p_cta_cal,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_cta_cal);
   assert(NULL != p_confl_zone_ext_params);
   assert(NULL != p_vehicle_data);

   p_confl_zone_ext_params->host_steer_fac[CTA_MODE_FRONT] = Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor(
      Fbk_Abs_F(p_vehicle_data->steering_angle), p_cta_cal->k_cta_fcta_steer_angle_table, p_cta_cal->k_cta_fcta_steer_factor_table,
      CTA_K_CTA_RCTA_STEER_ANGLE_TABLE_ARRAY_SIZE_DIM0);

   p_confl_zone_ext_params->host_steer_fac[CTA_MODE_REAR] = Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor(
      Fbk_Abs_F(p_vehicle_data->steering_angle), p_cta_cal->k_cta_rcta_steer_angle_table, p_cta_cal->k_cta_rcta_steer_factor_table,
      CTA_K_CTA_RCTA_STEER_ANGLE_TABLE_ARRAY_SIZE_DIM0);
}

static void Cta_Calc_Fac_Host_Speed_Adapt(Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params,
                                          const Cta_Core_Calibration_T *p_cta_cal,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_cta_cal);
   assert(NULL != p_confl_zone_ext_params);
   assert(NULL != p_vehicle_data);

   p_confl_zone_ext_params->host_vel_fac =
      FBK_ONE_F + (float32_T) Fbk_Abs_F(p_vehicle_data->host_speed * p_cta_cal->k_cta_rcta_host_speed_factor);
}

static float32_T Cta_Get_Entity_Based_Intersec_Lines_Adaption_Factor(const float32_T entity,
                                                                     const float32_T *entity_table,
                                                                     const float32_T *entity_factor_table,
                                                                     const uint8_t length_of_entity_table)
{
   uint16_t index_entity;
   float32_T interpol_entity_factor;
   float32_T entity_saturated;

   /* Asserts */
   assert(NULL != entity_table);
   assert(NULL != entity_factor_table);

   entity_saturated = Enforce_Range(entity, entity_table[0], entity_table[length_of_entity_table - 1u]);

   index_entity = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(entity_table, length_of_entity_table, entity_saturated);

   interpol_entity_factor =
      Get_Y_Value_From_Line_By_Coordinates(entity_table[index_entity - 1u], entity_factor_table[index_entity - 1u],
                                           entity_table[index_entity], entity_factor_table[index_entity], entity_saturated);

   return interpol_entity_factor;
}


static void Cta_Calc_Fac_Obj_Head_Adapt(const float32_T obj_heading,
                                        const Cta_Core_Calibration_T *p_cta_cal,
                                        Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params)
{
   float32_T heading_factor_inverse;

   /* Asserts */
   assert(NULL != p_cta_cal);
   assert(NULL != p_confl_zone_ext_params);

   heading_factor_inverse = Max(Fast_Sin(Fbk_Abs_F(obj_heading)), Fast_Sin(p_cta_cal->k_cta_min_park_angle));

   assert(heading_factor_inverse > EPSILON);

   p_confl_zone_ext_params->target_head_fac = FBK_ONE_F / heading_factor_inverse;
}


static void Cta_Apply_Ext_To_Level_Logic_Calib(Cta_Crit_Level_Calibration_T *p_crit_level_calibration,
                                               const Cta_Inters_Zone_Ext_Param_T *p_confl_zone_ext_params,
                                               const Cta_Core_Calibration_T *p_cta_cal,
                                               const Cta_Mode_T cta_mode)
{
   uint8_t level;

   /* Asserts */
   assert(NULL != p_crit_level_calibration);
   assert(NULL != p_confl_zone_ext_params);
   assert(NULL != p_cta_cal);

   /* Modify the zones on each level.
   For this process the steering angle adaption needs to come first,
   since when the zone is already adapted by other adaption factors,
   multiple calibrations for the reduction by the steering adaption need to be provided.*/
   for (level = FBK_ZERO_UINT; level < CTA_NUM_CRIT_LEVEL; level++)
   {
      /*Adapt by steering angle*/
      if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_steering_angle))
      {
         Cta_Apply_Factor_For_Level_Extension(p_crit_level_calibration, p_confl_zone_ext_params->host_steer_fac[cta_mode], level,
                                              cta_mode);
      }
      /*Adapt by target heading*/
      if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_obj_heading))
      {
         Cta_Apply_Factor_For_Level_Extension(p_crit_level_calibration, p_confl_zone_ext_params->target_head_fac, level, cta_mode);
      }

      if (Fbk_Is_True(p_cta_cal->k_cta_f_adapt_intersect_lines_by_host_speed) && (CTA_MODE_REAR == cta_mode))
      {
         Cta_Apply_Factor_For_Level_Extension(p_crit_level_calibration, p_confl_zone_ext_params->host_vel_fac, level, cta_mode);
      }
   }
}


static void Cta_Apply_Factor_For_Level_Extension(Cta_Crit_Level_Calibration_T *p_crit_level_calibration,
                                                 const float32_T adaption_factor,
                                                 const uint8_t level,
                                                 const Cta_Mode_T cta_mode)
{
   float32_T adapted_line_segment;

   /* Asserts */
   assert(NULL != p_crit_level_calibration);

   adapted_line_segment = (p_crit_level_calibration->max_long_point_criticality_level[cta_mode][level]
                           - p_crit_level_calibration->min_long_point_criticality_level[cta_mode][level])
                          * adaption_factor;

   if (CTA_MODE_FRONT == cta_mode)
   {
      p_crit_level_calibration->max_long_point_criticality_level[cta_mode][level] =
         p_crit_level_calibration->min_long_point_criticality_level[cta_mode][level] + adapted_line_segment;
   }
   else
   {
      p_crit_level_calibration->min_long_point_criticality_level[cta_mode][level] =
         p_crit_level_calibration->max_long_point_criticality_level[cta_mode][level] - adapted_line_segment;
   }
}
