/**
 * @file lcda_create_cvw_zone.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Creates object specific zones on adjacent lanes.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_create_cvw_zone.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "lcda_process_cvw.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* typedef
\*===========================================================================*/

/**
 * Summarizes Zone properties as longitudinal or lateral points so that internally zones can be switched dependent
 * on core input flag.
 */
typedef struct
{
   const float32_T *p_x_points;
   const float32_T *p_y_points;
   const float32_T *p_y_hys;
} Lcda_Cvw_Zone_Properties_T;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Computes curve zone factor for CVW zone if the ego drives around a curve.
 *
 * @return void
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6951}
 *
 * @verification{Create parameters for curve zone adaption and check that the corresponding factor is reduced due to the curve
 * driven for the specified side.}
 */
static void Lcda_Compute_Curve_Zone_Factor(float32_T *p_curve_zone_factor /**< Factor for zone adaptation */,
                                           const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                           const uint8_t side /**< Side index */,
                                           const boolean_T f_alert_active_on_side /**< Flag if an alert is active on the side */,
                                           const float32_T curve_radius /**< Curve radius */);

/**
 * @brief Computes applied curve zone factor for CVW zone if the ego drives around a curve.
 *
 * @return void
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6956}
 *
 * @verification{Create parameters for curve zone adaption and check that the corresponding factor is reduced due to the curve
 * driven for the specified side.}
 */
static void Lcda_Compute_Applied_Curve_Zone_Factor(float32_T *p_apply_curve_zone_factor /**< Applied factor for zone adaptation */,
                                                   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                                   const uint8_t side /**< Side index */,
                                                   const float32_T length_unmodified /**< Unmodified length of the zone*/,
                                                   const float32_T curve_radius /**< Curve radius */,
                                                   const float32_T curve_zone_factor /**< Precomputed factor for zone adaptation */,
                                                   const Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Adapts the length of the CVW zone if the ego drives around a curve.
 *
 * @return void
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6601}
 * @verification{Check whether in curve scenario the cvw zone is adapted.}
 */
static void Lcda_Curve_Zone_Adaptation(const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                       const uint8_t side /**< Side index */,
                                       Lcda_Cvw_Persistent_T *p_cvw_persistent);

/**
 * @brief Applies the curve zone adaption factor.
 *
 * @return void
 *
 * @SRS{SF-1114}
 * @SAE{SF-2779}
 * @SDD{SF-6600}
 * @verification{Check that the curve zone has been enlarged.}
 */
static void Lcda_Apply_Curve_Zone_Adaptation(const Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                             Fbk_Field_Of_Interest_T *p_cvw_zone /**< Input zone */,
                                             uint8_t side /**< Side index */);

/**
 * @brief Returns bsw zones dependent on the cvw calculation mode.
 *
 * @return void
 *
 * @SRS{SF-1001,SF-1058}
 * @SAE{SF-2779}
 * @SDD{SF-6602}
 * @verification{Check that the zone and the hysteresis zone pointer are initialized.}
 */
static void Lcda_Get_Initial_Cvw_Zones(
   Fbk_Field_Of_Interest_T *p_zone /**< Cvw Zone */,
   Fbk_Field_Of_Interest_T *p_zone_hys /**< Cvw Zone with hysteresis applied */,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Cvw_Object_T *p_cvw_object /**< Cvw object */,
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Creates the initial cvw zone based on lane width.
 *
 * @return void
 *
 * @SRS{SF-1060,SF-1061}
 * @SAE{SF-2779}
 * @SDD{SF-6603}
 * @verification{Check that the zone and the hysteresis zone pointer are initialized.}
 */
static void Lcda_Get_Lw_Based_Initial_Cvw_Zones(
   Fbk_Field_Of_Interest_T *p_zone /**< Cvw Zone */,
   Fbk_Field_Of_Interest_T *p_zone_hys /**< Cvw Zone with hysteresis applied */,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Cvw_Object_T *p_cvw_object /**< Cvw object */,
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Returns Cvw zone properties.
 *
 * @return void
 *
 * @SRS{SF-1001,SF-1064}
 * @SAE{SF-2779}
 * @SDD{SF-6812}
 * @verification{Check that depending on the warning state.}
 */
static Lcda_Cvw_Zone_Properties_T
Lcda_Return_Cvw_Zone_Properties(const boolean_T f_use_small_lc_intention_zone /**< Flag indicating which zone to use */,
                                const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/*============================================================================*\
 * Global Function Definition
\*============================================================================*/

void Lcda_Create_Cvw_Zone(Fbk_Field_Of_Interest_T *p_zone,
                          Fbk_Field_Of_Interest_T *p_zone_hys,
                          const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                          const Cvw_Object_T *p_cvw_object,
                          const Lcda_Core_Input_T *p_core_input,
                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                          const Lcda_Core_Calibration_T *p_cals,
                          Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cvw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   p_zone->size     = LCDA_NUMBER_OF_ZONE_POINTS;
   p_zone_hys->size = LCDA_NUMBER_OF_ZONE_POINTS;

   Lcda_Get_Initial_Cvw_Zones(p_zone, p_zone_hys, f_use_small_lc_intention_zone, p_cvw_object, p_core_input, p_cals);

   /* Since the zone is built for the right side by default, we create a mirror image of the zone for the left side */
   if (FBK_SIDE_LEFT == p_cvw_object->ego_side)
   {
      Lcda_Mirror_Zone_Across_Long_Axis(p_zone);
      Lcda_Mirror_Zone_Across_Long_Axis(p_zone_hys);
   }

   if (Fbk_Is_True(p_cals->k_enable_cvw_curve_zone_adaptation))
   {
      Lcda_Curve_Zone_Adaptation(p_cals, p_core_input, p_vehicle_data, p_cvw_object->ego_side, p_cvw_persistent);
      Lcda_Apply_Curve_Zone_Adaptation(p_cvw_persistent, p_zone, p_cvw_object->ego_side);
   }
}

/*============================================================================*\
 * Local function definition
\*============================================================================*/


static void Lcda_Get_Initial_Cvw_Zones(Fbk_Field_Of_Interest_T *p_zone,
                                       Fbk_Field_Of_Interest_T *p_zone_hys,
                                       const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                                       const Cvw_Object_T *p_cvw_object,
                                       const Lcda_Core_Input_T *p_core_input,
                                       const Lcda_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cvw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   switch (p_cals->k_cvw_zone_calculation_mode)
   {
      case (uint8_t) CVW_ZONE_CALC_FIXED_INPUT:
      {
         assert(p_core_input->initial_cvw_zone.size == LCDA_NUMBER_OF_ZONE_POINTS);
         assert(p_core_input->initial_cvw_zone_hys.size == LCDA_NUMBER_OF_ZONE_POINTS);
         *p_zone     = p_core_input->initial_cvw_zone;
         *p_zone_hys = p_core_input->initial_cvw_zone_hys;
         break;
      }
      /* NOTE: For now only the Vehicle Length, Lane Width based zone is implemented so using this as the default */
      case (uint8_t) CVW_ZONE_CALC_VL_LW:
      case (uint8_t) CVW_ZONE_CALC_DEFAULT:
      default:
      {
         Lcda_Get_Lw_Based_Initial_Cvw_Zones(p_zone, p_zone_hys, f_use_small_lc_intention_zone, p_cvw_object, p_core_input, p_cals);
         break;
      }
   }
}

static void Lcda_Get_Lw_Based_Initial_Cvw_Zones(Fbk_Field_Of_Interest_T *p_zone,
                                                Fbk_Field_Of_Interest_T *p_zone_hys,
                                                const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES],
                                                const Cvw_Object_T *p_cvw_object,
                                                const Lcda_Core_Input_T *p_core_input,
                                                const Lcda_Core_Calibration_T *p_cals)
{
   Lcda_Cvw_Zone_Properties_T zone_properties;
   float32_T hys_offsets_y[LCDA_NUMBER_OF_ZONE_POINTS] = {FBK_ZERO_F};
   float32_T lane_width;
   float32_T lane_center_offset;
   float32_T object_width;
   float32_T hysteresis_factor_by_object_width = FBK_ONE_F;
   uint8_t j;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cvw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   lane_width         = Fbk_Max(p_cals->k_lcda_min_lane_width, p_core_input->lane_width);
   lane_center_offset = p_core_input->lane_center_offset;
   object_width       = p_cvw_object->p_tracker_data->width;

   /* Since the zone will be built for the right hand side initially, we mirror the lane center offset to correspond to the right
    * side */
   if (FBK_SIDE_LEFT == p_cvw_object->ego_side)
   {
      lane_center_offset = -lane_center_offset;
   }

   /* Check that the cal value is not zero before dividing to get the hys factor */
   if (p_cals->k_zone_hys_obj_width_correction > THRESHOLD_IS_ZERO)
   {
      hysteresis_factor_by_object_width = Min(FBK_ONE_F, Max(FBK_ZERO_F, object_width / p_cals->k_zone_hys_obj_width_correction));
   }

   zone_properties = Lcda_Return_Cvw_Zone_Properties(f_use_small_lc_intention_zone[p_cvw_object->ego_side], p_core_input, p_cals);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      /* define x coordinate of cvw zone and hysteresis zone */
      p_zone->points[j].x     = zone_properties.p_x_points[j];
      p_zone_hys->points[j].x = zone_properties.p_x_points[j];

      /* define y co-ord of zone and calculate hys zone y-offset */
      p_zone->points[j].y = (lane_width * zone_properties.p_y_points[j]);
   }

   /* Limit zone width */
   Lcda_Limit_Outer_Zone_Points(p_zone, lane_width, p_cals);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      float32_T offset_sign = FBK_ONE_F;

      /* Calculate the offset for the y co-ord of hysteresis zone */
      hys_offsets_y[j] = (lane_width * zone_properties.p_y_hys[j] * hysteresis_factor_by_object_width);
      hys_offsets_y[j] = Min(p_cals->k_cvw_zone_y_hys_max, hys_offsets_y[j]);
      hys_offsets_y[j] = Max(p_cals->k_cvw_zone_y_hys_min, hys_offsets_y[j]);

      if (((uint8_t) FRONT_EGO_SIDE == j) || ((uint8_t) MIDDLE_EGO_SIDE == j) || ((uint8_t) REAR_EGO_SIDE == j))
      {
         offset_sign = -FBK_ONE_F;
      }

      /* define y co-ord of hys zone points by adding the hys offsets */
      p_zone_hys->points[j].y = p_zone->points[j].y + (offset_sign * hys_offsets_y[j]);

      /* Apply lane center offset */
      p_zone->points[j].y     = p_zone->points[j].y + lane_center_offset;
      p_zone_hys->points[j].y = p_zone_hys->points[j].y + lane_center_offset;
   }
}

static void Lcda_Compute_Curve_Zone_Factor(float32_T *p_curve_zone_factor,
                                           const Lcda_Core_Calibration_T *p_cals,
                                           const Fbk_Vehicle_Data_T *p_vehicle_data,
                                           const uint8_t side,
                                           const boolean_T f_alert_active_on_side,
                                           const float32_T curve_radius)
{
   /* if the ego is driving a curve, the CVW zone on the opposite side can be shortened by a factor specified by the p_cals struct
    * do not apply curve zone adaption if:
    * 1. There is already an active alert on current side
    * 2. The curve radius is zero (car not in a curve)
    */
   if (Fbk_Is_False(f_alert_active_on_side) && (Fbk_Abs_F(curve_radius) > THRESHOLD_IS_ZERO)
       && (Fbk_Abs_F(curve_radius) < p_cals->k_lcda_curve_radius_threshold_for_zone_adaptation))
   {
      if (((FBK_SIDE_RIGHT == side) && (p_vehicle_data->yawrate < FBK_ZERO_F))
          || ((FBK_SIDE_LEFT == side) && (p_vehicle_data->yawrate > FBK_ZERO_F)))
      {
         /* target is on outer curve side */
         (*p_curve_zone_factor) = p_cals->k_cvw_curve_zone_factor_outer;
      }
      else if (Fbk_Not_Equal_F(FBK_ZERO_F, p_vehicle_data->yawrate))
      {
         /* target is on inner curve side */
         (*p_curve_zone_factor) = p_cals->k_cvw_curve_zone_factor_inner;
      }
      else
      {
         /* ego is driving straight */
      }
   }
   else
   {
      /* no relevant target */
   }
}

static void Lcda_Compute_Applied_Curve_Zone_Factor(float32_T *p_apply_curve_zone_factor,
                                                   const Lcda_Core_Input_T *p_core_input,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals,
                                                   const uint8_t side,
                                                   const float32_T length_unmodified,
                                                   const float32_T curve_radius,
                                                   const float32_T curve_zone_factor,
                                                   const Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   float32_T length_new, length_last;
   float32_T distance_traveled;
   if (length_unmodified > THRESHOLD_IS_ZERO)
   {
      /* limit maximal zone length adaption to avoid unwanted warnings: Zone can only expand by the same amount as the ego is
       * traveled
       */
      length_last       = Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(side, p_cvw_persistent) * length_unmodified;
      distance_traveled = p_cals->k_lcda_distance_traveled_scale_factor * p_vehicle_data->host_speed
                          * p_core_input->p_pa_data->time_diff_to_last_cycle;

      if (Fbk_Not_Equal_F(curve_zone_factor, FBK_ONE_F))
      {
         /* shorten CVW zone */
         length_new = Min(curve_zone_factor * 2.0f * PI * Fbk_Abs_F(curve_radius), length_last + distance_traveled);
      }
      else
      {
         /* not driving curve or feature disabled -> always lengthen the zone */
         length_new = Min(length_unmodified, length_last + distance_traveled);
      }
      (*p_apply_curve_zone_factor) = Min(length_new / length_unmodified, 1.0f);
   }
   else
   {
      /* CVW zones are defined -> dont shorten the CVW zone */
   }
}

static void Lcda_Curve_Zone_Adaptation(const Lcda_Core_Calibration_T *p_cals,
                                       const Lcda_Core_Input_T *p_core_input,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                       const uint8_t side,
                                       Lcda_Cvw_Persistent_T *p_cvw_persistent)
{
   boolean_T alert_active_on_side;
   float32_T min_val, max_val;
   float32_T length_unmodified;
   float32_T apply_curve_zone_factor = 1.0f;
   float32_T curve_zone_factor       = 1.0f;
   uint8_t j;
   float32_T curve_radius = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);

   alert_active_on_side = (boolean_T) (PA_INVALID_OBJ_ID != Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(side, p_cvw_persistent));

   /* Calculate the curve radius using velocity and yawrate */
   if (Fbk_Abs_F(p_vehicle_data->yawrate) > THRESHOLD_IS_ZERO)
   {
      curve_radius = p_vehicle_data->long_vel / p_vehicle_data->yawrate;
   }

   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, p_cals, p_vehicle_data, side, alert_active_on_side, curve_radius);

   /* compute length of unmodified CVW */
   max_val = p_cals->k_cvw_zone_x[0];
   min_val = p_cals->k_cvw_zone_x[0];
   for (j = 1; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      max_val = Max(p_cals->k_cvw_zone_x[j], max_val);
      min_val = Min(p_cals->k_cvw_zone_x[j], min_val);
   }
   length_unmodified = max_val - min_val;

   Lcda_Compute_Applied_Curve_Zone_Factor(&apply_curve_zone_factor, p_core_input, p_vehicle_data, p_cals, side, length_unmodified,
                                          curve_radius, curve_zone_factor, p_cvw_persistent);

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(p_cvw_persistent, apply_curve_zone_factor, side); /* save the ApplyCurveZoneFactor for
                                                                                            next cycle */
}

static void Lcda_Apply_Curve_Zone_Adaptation(const Lcda_Cvw_Persistent_T *p_cvw_persistent,
                                             Fbk_Field_Of_Interest_T *p_cvw_zone,
                                             uint8_t side)
{

   float32_T max_val;
   uint8_t j;

   /* Assert */
   assert(NULL != p_cvw_zone);

   /* compute length of unmodified CVW */
   max_val = p_cvw_zone->points[0].x;
   for (j = 1; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      max_val = Max(p_cvw_zone->points[j].x, max_val);
   }

   /* to avoid unintentional distortions (lengthening instead of shortening etc) the zone is relocated at x = 0.
    * here we assume that the zone is always expanding behind the ego and not in front */
   for (j = 0; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      p_cvw_zone->points[j].x =
         ((p_cvw_zone->points[j].x - max_val) * Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(side, p_cvw_persistent)) + max_val;
   }
}


static Lcda_Cvw_Zone_Properties_T Lcda_Return_Cvw_Zone_Properties(const boolean_T f_use_small_lc_intention_zone,
                                                                  const Lcda_Core_Input_T *p_core_input,
                                                                  const Lcda_Core_Calibration_T *p_cals)
{
   Lcda_Cvw_Zone_Properties_T zone_properties;

   if (Fbk_Is_True(p_core_input->warn_settings.f_use_cvw_lane_change_intention_zone))
   {

      if (f_use_small_lc_intention_zone)
      {
         zone_properties.p_x_points = &(p_cals->k_cvw_lane_change_intention_zone_small_x[0]);
         zone_properties.p_y_points = &(p_cals->k_cvw_lane_change_intention_zone_small_y[0]);
         zone_properties.p_y_hys    = &(p_cals->k_cvw_lane_change_intention_zone_hys_y[0]);
      }
      else
      {
         zone_properties.p_x_points = &(p_cals->k_cvw_lane_change_intention_zone_x[0]);
         zone_properties.p_y_points = &(p_cals->k_cvw_lane_change_intention_zone_y[0]);
         zone_properties.p_y_hys    = &(p_cals->k_cvw_lane_change_intention_zone_hys_y[0]);
      }
   }
   else
   {
      zone_properties.p_x_points = &(p_cals->k_cvw_zone_x[0]);
      zone_properties.p_y_points = &(p_cals->k_cvw_zone_y[0]);
      zone_properties.p_y_hys    = &(p_cals->k_cvw_zone_y_hys[0]);
   }
   return zone_properties;
}
