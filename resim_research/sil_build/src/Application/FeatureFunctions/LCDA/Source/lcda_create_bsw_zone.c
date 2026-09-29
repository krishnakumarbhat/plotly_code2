/**
 * @file lcda_create_bsw_zone.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Creates object specific zones in blind spot area.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_create_bsw_zone.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_common_functions.h"
#include "ml_interval.h"
#include "ml_lookup_table_2d.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>
/*===========================================================================*\
* Defines
\*===========================================================================*/

#define LCDA_NUM_OF_DYNZONE_SPEED_DATA_POINTS (6u)
#define LCDA_NUM_OF_DYNZONE_OBJECT_REL_VEL_DATA_POINTS (10u)
#define LCDA_NUM_OF_DYNZONE_SPEED_DROPB_DATA_POINTS (8u)

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/**
 * @brief Enlarges the zone based on the host speed.
 *        This adaption is calculated via a 2d-lookuptable approach.
 *
 * @return void
 *
 * @SRS{SF-1112,CSCSA-136482}
 * @SAE{SF-2779}
 * @SDD{SF-6588}
 * @verification{Check whether the zone is enlarged dependent on the host speed.}
 */
static void Lcda_Adjust_Zones_For_Ego_Speed(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                                            Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                                            const float32_T ego_length /**< Host length */,
                                            const float32_T ego_abs_speed /**< Absolute host speed*/,
                                            const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Enlarges the zone based on the object relative velocity.
 *        This adaption is calculated via a 2d-lookuptable approach.
 *
 * @return void
 *
 * @SRS{SF-997}
 * @SAE{SF-2779}
 * @SDD{CSCSA-284696}
 * @verification{Check whether the zone is enlarged dependent on the object longitudinal relative velocity.}
 */
static void Lcda_Adjust_Zones_For_Object_Rel_Vel(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                                                 Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                                                 const float32_T obj_long_rel_vel /**< Object longitudinal relative velocity*/,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Returns bsw zones dependent on the bsw calculation mode.
 *
 * @return void
 *
 * @SRS{SF-995,SF-997,SF-1052,SF-1054}
 * @SAE{SF-2779}
 * @SDD{SF-6591}
 * @verification{Check that the zone and the hysteresis zone pointer are initialized.}
 */
static void Lcda_Get_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                                       Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                                       const Bsw_Object_T *p_bsw_object /**< Bsw object */,
                                       const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Creates the initial bsw zone based on fixed calibration values in VCS.
 *
 * @return void
 *
 * @SRS{SF-1053}
 * @SAE{SF-2779}
 * @SDD{SF-6927}
 * @verification{Check that object in fixed zone causes BSW alert.}
 */
static void Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                                                 Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                                                 const float32_T host_vehicle_length /**< Length of host vehicle */,
                                                 const float32_T object_width /**< Width of object */,
                                                 const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

/**
 * @brief Creates the initial bsw zone based on host length and lane width.
 *
 * @return void
 *
 * @SRS{SF-1053}
 * @SAE{SF-2779}
 * @SDD{SF-6593}
 * @verification{Check whether the initial bsw zone is initialized correctly.}
 */
static void Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                                                   Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                                                   const Bsw_Object_T *p_bsw_object /**< Bsw object */,
                                                   const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                                   const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);


/**
 * @brief Calculates a shrink factor and applies it to the bsw zone in case of dropback.
 *
 * @return void
 *
 * @SRS{SF-997}
 * @SAE{SF-2779}
 * @SDD{SF-6594}
 * @verification{Check for a dropback object that the zone is shrinking by a calculated factor.}
 */
static void Lcda_Shrink_Bsw_Zone_Dropback_Obj(Fbk_Field_Of_Interest_T *p_bsw_zone /**< Bsw zone */,
                                              Fbk_Field_Of_Interest_T *p_bsw_zone_hys /**< Bsw zone with hysteresis applied */,
                                              const Bsw_Object_T *p_bsw_object /**< Bsw object */,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                              const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);
/**
 * @brief Applies a shrinking dropback factor to the bsw zone.
 *
 * @return void
 *
 * @SRS{SF-997}
 * @SAE{SF-2779}
 * @SDD{SF-6589}
 * @verification{Check that the zone length reduces after application of dropback factor.}
 */
static void Lcda_Apply_Dropback_Factor(Fbk_Field_Of_Interest_T *p_zone /**< Input zone */,
                                       const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */,
                                       const float32_T x_shrink_factor /**< Shrinking factor */,
                                       const float32_T veh_length /**< Vehicle length */);

/**
 * @brief This function checks the core input and adjusts the zone size depending on information about an attached trailer.
 *
 * @return void
 *
 * @SRS{SF-1055}
 * @SAE{SF-2779}
 * @SDD{SF-6587}
 * @verification{For an attached trailer check that the zone size of bsw zones is adjusted.}
 */
static void Lcda_Adjust_Bsw_Zone_Size_For_Trailer(Fbk_Field_Of_Interest_T *p_bsw_zone /**< Bsw zone */,
                                                  Fbk_Field_Of_Interest_T *p_bsw_zone_hys /**< Bsw Hysteresis zone */,
                                                  const Lcda_Core_Input_T *p_lcda_core_input /**< Lcda Core input */,
                                                  const Lcda_Core_Calibration_T *p_lcda_cal /**< Lcda Calibration */,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief This function enlarges the bsw zone according to the given adjustment values by subtraction or addition of
          enlargement values.
 *
 * @return void
 *
 * @SRS{SF-997}
 * @SAE{SF-2779}
 * @SDD{SF-6590}
 * @verification{Check that the input zone is enlarged by the input factors.}
 */
static void Lcda_Enlarge_Bsw_Zone(Fbk_Field_Of_Interest_T *p_bsw_zone /**< Input zone */,
                                  const float32_T lon_adj /**< longitudinal adjustment */,
                                  const float32_T lat_adj /**< lateral adjustment */,
                                  const Lcda_Core_Calibration_T *p_lcda_cal /**< Lcda Calibration */);


/*============================================================================*\
 * Global Function Definition
\*============================================================================*/

void Lcda_Create_Bsw_Zone(Fbk_Field_Of_Interest_T *p_zone,
                          Fbk_Field_Of_Interest_T *p_zone_hys,
                          const Bsw_Object_T *p_bsw_object,
                          const Lcda_Core_Input_T *p_core_input,
                          const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t side = p_bsw_object->ego_side;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_bsw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   p_vehicle_data = &p_core_input->p_pa_data->vehicle_data;

   p_zone->size     = LCDA_NUMBER_OF_ZONE_POINTS;
   p_zone_hys->size = LCDA_NUMBER_OF_ZONE_POINTS;

   Lcda_Get_Initial_Bsw_Zones(p_zone, p_zone_hys, p_bsw_object, p_core_input, p_vehicle_data, p_cals);

   if (Fbk_Is_True(p_cals->k_bsw_enable_dynspeed_zone))
   {
      Lcda_Adjust_Zones_For_Ego_Speed(p_zone, p_zone_hys, p_vehicle_data->host_length, Fbk_Abs_F(p_vehicle_data->host_speed), p_cals);
   }

   /* Apply the configurable factor to the bsw zones */
   Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(p_core_input->warn_settings.bsw_len_factor, p_zone, p_vehicle_data->host_length);
   Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(p_core_input->warn_settings.bsw_len_factor, p_zone_hys, p_vehicle_data->host_length);

   if (Fbk_Is_True(p_core_input->enabled_flags.f_dropback_enabled))
   {
      Lcda_Shrink_Bsw_Zone_Dropback_Obj(p_zone, p_zone_hys, p_bsw_object, p_vehicle_data, p_cals);
   }

   /* Adjust BSW zone length for trailer */
   if (Fbk_Is_True(p_cals->k_bsw_enable_trailer_zone_extension))
   {
      Lcda_Adjust_Bsw_Zone_Size_For_Trailer(p_zone, p_zone_hys, p_core_input, p_cals, p_vehicle_data);
   }

   /* Adjust BSW zone length for object longitudinal relative velocity*/
   if (Fbk_Is_True(p_cals->k_bsw_f_enable_object_rel_vel_dynzone))
   {
      Lcda_Adjust_Zones_For_Object_Rel_Vel(p_zone, p_zone_hys, p_bsw_object->p_tracker_data->vcs_vel_rel.x, p_cals);
   }

   /* Since the zone is built for the right side by default, we create a mirror image of the zone for the left side */
   if (FBK_SIDE_LEFT == side)
   {
      Lcda_Mirror_Zone_Across_Long_Axis(p_zone);
      Lcda_Mirror_Zone_Across_Long_Axis(p_zone_hys);
   }
   else
   {
      /* Do nothing since the zone is already built for the right side */
   }
}

/*============================================================================*\
 * Local Function Definition
\*============================================================================*/

static void Lcda_Adjust_Zones_For_Ego_Speed(Fbk_Field_Of_Interest_T *p_zone,
                                            Fbk_Field_Of_Interest_T *p_zone_hys,
                                            const float32_T ego_length,
                                            const float32_T ego_abs_speed,
                                            const Lcda_Core_Calibration_T *p_cals)
{
   float32_T offset_based_on_ego_speed;
   float32_T zone_length_behind_ego;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cals);

   /* Get the ego speed dependent offset for the rear edge of the bsw zone */
   offset_based_on_ego_speed = Get_Value_From_2d_Lookup_Table(p_cals->k_bsw_dynzone_speed, p_cals->k_bsw_dynzone_range,
                                                              LCDA_NUM_OF_DYNZONE_SPEED_DATA_POINTS, ego_abs_speed);

   /* Get the length of the bsw zone behind the ego
    * ASSUMPTION: Zone is standard Lcda zone represented by 6 points
    *             and points 2 and 3 lie behind or at the ego vehicle rear */
   zone_length_behind_ego = Fbk_Abs_F((p_zone->points[2].x + p_zone->points[3].x) * 0.5f) - ego_length;
   if (Fbk_Is_True(p_cals->k_bsw_enable_factor_based_host_speed_adjustment) && (zone_length_behind_ego > FBK_ZERO_F)
       && (offset_based_on_ego_speed < FBK_ZERO_F))
   {
      float32_T dyn_factor;
      /* Calculate the dyn factor. dyn factor = 1 + abs(dyn offset)/(bsw zone length behind ego)
       * Keep in mind: dyn offset is defined as negative and zone will always be enlarged
       * This factor is then applied to the bsw zone and the bsw hys zone
       * Negative value applied instead of absolute since value is always negative
       */
      dyn_factor = 1.0f + (-offset_based_on_ego_speed / zone_length_behind_ego);
      Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(dyn_factor, p_zone, ego_length);
      Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(dyn_factor, p_zone_hys, ego_length);
   }
   else
   {
      /* Adjust rear points of the bsw zone based on the ego speed  */
      p_zone->points[REAR_OUTER_SIDE].x = p_zone->points[REAR_OUTER_SIDE].x + offset_based_on_ego_speed;
      p_zone->points[REAR_EGO_SIDE].x   = p_zone->points[REAR_EGO_SIDE].x + offset_based_on_ego_speed;

      /* Adjust rear points for the hysteresis zone */
      p_zone_hys->points[REAR_OUTER_SIDE].x = p_zone_hys->points[REAR_OUTER_SIDE].x + offset_based_on_ego_speed;
      p_zone_hys->points[REAR_EGO_SIDE].x   = p_zone_hys->points[REAR_EGO_SIDE].x + offset_based_on_ego_speed;
   }
}


static void Lcda_Adjust_Zones_For_Object_Rel_Vel(Fbk_Field_Of_Interest_T *p_zone,
                                                 Fbk_Field_Of_Interest_T *p_zone_hys,
                                                 const float32_T obj_long_rel_vel,
                                                 const Lcda_Core_Calibration_T *p_cals)
{
   float32_T offset_based_on_object_speed;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cals);

   /* Get the ego speed dependent offset for the rear edge of the bsw zone */
   offset_based_on_object_speed = Get_Value_From_2d_Lookup_Table(p_cals->k_bsw_dynzone_object_rel_vel,
                                                                 p_cals->k_bsw_dynzone_object_range,
                                                                 LCDA_NUM_OF_DYNZONE_OBJECT_REL_VEL_DATA_POINTS, obj_long_rel_vel);

   /* Adjust rear points of the bsw zone based on the ego speed  */
   p_zone->points[REAR_OUTER_SIDE].x = p_zone->points[REAR_OUTER_SIDE].x + offset_based_on_object_speed;
   p_zone->points[REAR_EGO_SIDE].x   = p_zone->points[REAR_EGO_SIDE].x + offset_based_on_object_speed;

   /* Adjust rear points for the hysteresis zone */
   p_zone_hys->points[REAR_OUTER_SIDE].x = p_zone_hys->points[REAR_OUTER_SIDE].x + offset_based_on_object_speed;
   p_zone_hys->points[REAR_EGO_SIDE].x   = p_zone_hys->points[REAR_EGO_SIDE].x + offset_based_on_object_speed;
}


static void Lcda_Get_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone,
                                       Fbk_Field_Of_Interest_T *p_zone_hys,
                                       const Bsw_Object_T *p_bsw_object,
                                       const Lcda_Core_Input_T *p_core_input,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                       const Lcda_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_bsw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_cals);

   switch (p_core_input->bsw_zone_calculation_mode)
   {
      case BSW_ZONE_CALC_FIXED_INPUT:
      {
         /* Calculate BSW zones based on zone definition in core input */
         assert(p_core_input->initial_bsw_zone.size == LCDA_NUMBER_OF_ZONE_POINTS);
         assert(p_core_input->initial_bsw_zone_hys.size == LCDA_NUMBER_OF_ZONE_POINTS);
         *p_zone     = p_core_input->initial_bsw_zone;
         *p_zone_hys = p_core_input->initial_bsw_zone_hys;
         break;
      }
      case BSW_ZONE_CALC_FIXED_ZONE_VCS:
      {
         /* Calculate BSW zones based on fixed zone calibration values (in VCS) */
         Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(p_zone, p_zone_hys, p_vehicle_data->host_length, p_bsw_object->p_tracker_data->width,
                                              p_cals);
         break;
      }
      case BSW_ZONE_CALC_VL_LW:
      case BSW_ZONE_CALC_DEFAULT:
      default:
      {
         /* Calculate BSW zones based on vehicle dimension and lane width information */
         Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(p_zone, p_zone_hys, p_bsw_object, p_core_input, p_vehicle_data, p_cals);
         break;
      }
   }

   /* Ensure that rear points of zone are at ego rear or behind ego rear
    * since always negative used minus instead of absolute value  */
   p_zone->points[REAR_OUTER_SIDE].x = -Max(-(p_zone->points[REAR_OUTER_SIDE].x), p_vehicle_data->host_length);
   p_zone->points[REAR_EGO_SIDE].x   = -Max(-(p_zone->points[REAR_EGO_SIDE].x), p_vehicle_data->host_length);
}

static void Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone,
                                                 Fbk_Field_Of_Interest_T *p_zone_hys,
                                                 const float32_T host_vehicle_length,
                                                 const float32_T object_width,
                                                 const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t i;
   float32_T vehicle_length;

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_cals);

   /* Force ego vehicle length to be within the plausible range */
   vehicle_length = Enforce_Range(host_vehicle_length, p_cals->k_lcda_min_ego_vehicle_length, p_cals->k_lcda_max_ego_vehicle_length);

   /* Set some special coordinates first */
   p_zone->points[FRONT_EGO_SIDE].x   = -(vehicle_length * p_cals->k_bsw_fixed_zone_x[FRONT_EGO_SIDE]);
   p_zone->points[FRONT_OUTER_SIDE].x = -(vehicle_length * p_cals->k_bsw_fixed_zone_x[FRONT_OUTER_SIDE]);

   /* Iterate over zone points */
   for (i = FBK_ZERO_UINT; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      float32_T bsw_fixed_zone_y_hys;

      /* Define x coordinates of bsw zone in relation to vehicle length */
      if (((uint8_t) FRONT_EGO_SIDE != i) && ((uint8_t) FRONT_OUTER_SIDE != i))
      {
         p_zone->points[i].x = -(vehicle_length) + p_cals->k_bsw_fixed_zone_x[i];
      }

      /* Set y zone points based on fixed calibration values */
      p_zone->points[i].y = p_cals->k_bsw_fixed_zone_y[i];

      /* Set x co-ord hysteresis zone points */
      p_zone_hys->points[i].x = p_zone->points[i].x + p_cals->k_bsw_fixed_zone_x_hys[i];

      /* Calculate the offset for the y co-ord of hysteresis zone */
      bsw_fixed_zone_y_hys = p_cals->k_bsw_fixed_zone_y_hys[i] * object_width;

      /* Clamp value between min and max */
      if (FBK_ONE_INT == Fbk_Sign(bsw_fixed_zone_y_hys))
      {
         bsw_fixed_zone_y_hys = Fbk_Clamp(bsw_fixed_zone_y_hys, p_cals->k_bsw_zone_y_hys_min, p_cals->k_bsw_zone_y_hys_max);
      }
      else
      {
         bsw_fixed_zone_y_hys = Fbk_Clamp(bsw_fixed_zone_y_hys, -p_cals->k_bsw_zone_y_hys_max, -p_cals->k_bsw_zone_y_hys_min);
      }

      p_zone_hys->points[i].y = p_zone->points[i].y + bsw_fixed_zone_y_hys;
   }
}

static void Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(Fbk_Field_Of_Interest_T *p_zone,
                                                   Fbk_Field_Of_Interest_T *p_zone_hys,
                                                   const Bsw_Object_T *p_bsw_object,
                                                   const Lcda_Core_Input_T *p_core_input,
                                                   const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                   const Lcda_Core_Calibration_T *p_cals)
{
   uint8_t j;
   float32_T lane_width;
   float32_T vehicle_width;
   float32_T vehicle_length;
   float32_T width_obj;
   float32_T lane_center_offset;
   float32_T hys_offsets_y[LCDA_NUMBER_OF_ZONE_POINTS] = {FBK_ZERO_F};

   /* Asserts */
   assert(NULL != p_zone);
   assert(NULL != p_zone_hys);
   assert(NULL != p_bsw_object);
   assert(NULL != p_core_input);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);

   width_obj          = p_bsw_object->p_tracker_data->width;
   lane_center_offset = p_core_input->lane_center_offset;

   /* Since the zone will be built for the right hand side initially, we mirror the lane center offset to correspond to the right
    * side */
   if (FBK_SIDE_LEFT == p_bsw_object->ego_side)
   {
      lane_center_offset = -lane_center_offset;
   }

   /* Force lane width to be within the plausible range */
   lane_width = Fbk_Max(p_cals->k_lcda_min_lane_width, p_core_input->lane_width);

   /* Force ego vehicle width to be within the plausible range */
   vehicle_width =
      Enforce_Range(p_vehicle_data->host_width, p_cals->k_lcda_min_ego_vehicle_width, p_cals->k_lcda_max_ego_vehicle_width);

   /* Force ego vehicle length to be within the plausible range */
   vehicle_length =
      Enforce_Range(p_vehicle_data->host_length, p_cals->k_lcda_min_ego_vehicle_length, p_cals->k_lcda_max_ego_vehicle_length);

   /* Set some special coordinates first */
   p_zone->points[FRONT_OUTER_SIDE].x = -(vehicle_length * p_cals->k_bsw_zone_x[FRONT_OUTER_SIDE]);
   p_zone->points[FRONT_EGO_SIDE].x   = -(vehicle_length * p_cals->k_bsw_zone_x[FRONT_EGO_SIDE]);
   p_zone->points[FRONT_EGO_SIDE].y   = ((Fbk_Half(vehicle_width)) + p_cals->k_bsw_lateral_distance_zone);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      /*define x coordinate of bsw zone */
      if (((uint8_t) FRONT_EGO_SIDE != j) && ((uint8_t) FRONT_OUTER_SIDE != j))
      {
         p_zone->points[j].x = -(vehicle_length) + p_cals->k_bsw_zone_x[j];
      }

      /*define y coordinate of bsw zone */
      if ((uint8_t) FRONT_EGO_SIDE != j)
      {
         p_zone->points[j].y = (lane_width * p_cals->k_bsw_zone_y[j]);
      }
   }

   /* Limit zone width */
   Lcda_Limit_Outer_Zone_Points(p_zone, lane_width, p_cals);

   for (j = FBK_ZERO_UINT; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      float32_T offset_sign = FBK_ONE_F;

      /*define x co-ordinate of hysteresis zone */
      p_zone_hys->points[j].x = p_zone->points[j].x + p_cals->k_bsw_zone_x_hys[j];

      /* Calculate the offset for the y co-ord of hysteresis zone */
      hys_offsets_y[j] = width_obj * p_cals->k_bsw_zone_y_hys[j];
      hys_offsets_y[j] = Min(p_cals->k_bsw_zone_y_hys_max, hys_offsets_y[j]); /* saturate at max value */
      hys_offsets_y[j] = Max(p_cals->k_bsw_zone_y_hys_min, hys_offsets_y[j]); /* saturate at min value */

      if (((uint8_t) FRONT_EGO_SIDE == j) || ((uint8_t) MIDDLE_EGO_SIDE == j) || ((uint8_t) REAR_EGO_SIDE == j))
      {
         offset_sign = -FBK_ONE_F;
      }

      /* define y co-ord of hys zone points by adding the hys offsets */
      p_zone_hys->points[j].y = p_zone->points[j].y + (offset_sign * hys_offsets_y[j]);

      /* Apply lane center offset to get y co-ord of all points except for FRONT_EGO_SIDE.
       * FRONT_EGO_SIDE y co-ord is already in VCS co-ord since it is calcuated using the k_bsw_lateral_distance_zone value and is
       * not based on lane width
       */
      if ((uint8_t) FRONT_EGO_SIDE != j)
      {
         p_zone->points[j].y     = p_zone->points[j].y + lane_center_offset;
         p_zone_hys->points[j].y = p_zone_hys->points[j].y + lane_center_offset;
      }
   }
}


static void Lcda_Shrink_Bsw_Zone_Dropback_Obj(Fbk_Field_Of_Interest_T *p_bsw_zone,
                                              Fbk_Field_Of_Interest_T *p_bsw_zone_hys,
                                              const Bsw_Object_T *p_bsw_object,
                                              const Fbk_Vehicle_Data_T *p_vehicle_data,
                                              const Lcda_Core_Calibration_T *p_cals)
{
   float32_T x_dropback_shrink;
   float32_T vehicle_length;

   /* Asserts */
   assert(NULL != p_bsw_zone);
   assert(NULL != p_bsw_zone_hys);
   assert(NULL != p_bsw_object);
   assert(NULL != p_cals);

   /* Check if the object is dropping back and the value lies within the min and max values of the speed in the lookup table */
   if ((p_bsw_object->p_tracker_data->curvi_vel_rel.x
        < p_cals->k_bsw_dynzone_speed_dropback[LCDA_NUM_OF_DYNZONE_SPEED_DROPB_DATA_POINTS - 1u])
       && (Fbk_Abs_F(p_vehicle_data->host_speed) < p_cals->k_bsw_dynzone_speed_dropback_max))
   {
      x_dropback_shrink = Get_Value_From_2d_Lookup_Table(p_cals->k_bsw_dynzone_speed_dropback, p_cals->k_bsw_dynzone_range_dropback,
                                                         LCDA_NUM_OF_DYNZONE_SPEED_DROPB_DATA_POINTS,
                                                         p_bsw_object->p_tracker_data->curvi_vel_rel.x);

      /* Force vehicle length to be within the plausible range */
      vehicle_length =
         Enforce_Range(p_vehicle_data->host_length, p_cals->k_lcda_min_ego_vehicle_length, p_cals->k_lcda_max_ego_vehicle_length);

      /* Apply the shrink factor to the initial bsw zone */
      Lcda_Apply_Dropback_Factor(p_bsw_zone, p_cals, x_dropback_shrink, vehicle_length);

      /* Apply the shrink factor to the hysteresis zone */
      Lcda_Apply_Dropback_Factor(p_bsw_zone_hys, p_cals, x_dropback_shrink, vehicle_length);
   }
}

static void Lcda_Apply_Dropback_Factor(Fbk_Field_Of_Interest_T *p_zone,
                                       const Lcda_Core_Calibration_T *p_cals,
                                       const float32_T x_shrink_factor,
                                       const float32_T veh_length)
{
   /* Assert */
   assert(NULL != p_zone);

   /* Shrink the zone by given factor (with the mode selected), but saturate at ego rear bumper */
   if (Fbk_Is_False(p_cals->k_bsw_shrink_zone_method))
   {
      p_zone->points[REAR_OUTER_SIDE].x = Fbk_Min(p_zone->points[REAR_OUTER_SIDE].x * x_shrink_factor, -veh_length);
      p_zone->points[REAR_EGO_SIDE].x   = Fbk_Min(p_zone->points[REAR_EGO_SIDE].x * x_shrink_factor, -veh_length);
   }
   else
   {
      p_zone->points[REAR_OUTER_SIDE].x = Fbk_Min(p_zone->points[REAR_OUTER_SIDE].x + x_shrink_factor, -veh_length);
      p_zone->points[REAR_EGO_SIDE].x   = Fbk_Min(p_zone->points[REAR_EGO_SIDE].x + x_shrink_factor, -veh_length);
   }


   /* Make sure that middle points are still in front of rear points  */
   if (p_zone->points[REAR_EGO_SIDE].x > p_zone->points[MIDDLE_EGO_SIDE].x)
   {
      p_zone->points[MIDDLE_EGO_SIDE].x = p_zone->points[REAR_EGO_SIDE].x;
   }

   if (p_zone->points[REAR_OUTER_SIDE].x > p_zone->points[MIDDLE_OUTER_SIDE].x)
   {
      p_zone->points[MIDDLE_OUTER_SIDE].x = p_zone->points[REAR_OUTER_SIDE].x;
   }
}

static void Lcda_Adjust_Bsw_Zone_Size_For_Trailer(Fbk_Field_Of_Interest_T *p_bsw_zone,
                                                  Fbk_Field_Of_Interest_T *p_bsw_zone_hys,
                                                  const Lcda_Core_Input_T *p_lcda_core_input,
                                                  const Lcda_Core_Calibration_T *p_lcda_cal,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /* Asserts */
   assert(NULL != p_bsw_zone);
   assert(NULL != p_bsw_zone_hys);
   assert(NULL != p_lcda_core_input);
   assert(NULL != p_lcda_cal);

   /* Check if bsw zone should be enlarged */
   if (Fbk_Is_True(p_lcda_core_input->trailer.f_trailer_present))
   {
      float32_T zone_extension_long = p_lcda_core_input->trailer.length;
      float32_T zone_extension_lat  = FBK_ZERO_F;

      /* If the trailer is wider than the host, modify the size of the zone with this value */
      if ((Fbk_Is_True(p_lcda_cal->k_bsw_f_zone_extension_by_diff_width_host_vs_trailer))
          && (p_lcda_core_input->trailer.width > p_vehicle_data->host_width))
      {
         zone_extension_lat = Fbk_Half(p_lcda_core_input->trailer.width - p_vehicle_data->host_width);
         if (zone_extension_lat > (p_lcda_cal->k_bsw_y_width - p_lcda_cal->k_bsw_trailer_zone_min_width))
         {
            zone_extension_lat = p_lcda_cal->k_bsw_y_width - p_lcda_cal->k_bsw_trailer_zone_min_width;
         }
      }

      /* Add safety margin behind trailer */
      zone_extension_long += p_lcda_cal->k_bsw_trailer_zone_ext_safety_margin;

      /* Enlarge regular zone */
      Lcda_Enlarge_Bsw_Zone(p_bsw_zone, zone_extension_long, zone_extension_lat, p_lcda_cal);

      /* Add hysteresis for safety margin behind trailer */
      zone_extension_long += p_lcda_cal->k_bsw_trailer_zone_ext_safety_margin_hys;

      /* Enlarge hysteresis zone */
      Lcda_Enlarge_Bsw_Zone(p_bsw_zone_hys, zone_extension_long, zone_extension_lat, p_lcda_cal);
   }
}

static void Lcda_Enlarge_Bsw_Zone(Fbk_Field_Of_Interest_T *p_bsw_zone,
                                  const float32_T lon_adj,
                                  const float32_T lat_adj,
                                  const Lcda_Core_Calibration_T *p_lcda_cal)
{
   /* Assert */
   assert(NULL != p_bsw_zone);
   assert(NULL != p_lcda_cal);

   /* Increases zone dimensions rearwards */
   p_bsw_zone->points[REAR_EGO_SIDE].x -= lon_adj;
   p_bsw_zone->points[REAR_OUTER_SIDE].x -= lon_adj;

   /* Modify zone dimensions by shifting the outside edge */
   if (Fbk_Is_True(p_lcda_cal->k_bsw_f_enable_trailer_zone_adjustment_on_outer_side))
   {
      p_bsw_zone->points[FRONT_OUTER_SIDE].y += lat_adj;
      p_bsw_zone->points[MIDDLE_OUTER_SIDE].y += lat_adj;
      p_bsw_zone->points[REAR_OUTER_SIDE].y += lat_adj;
   }

   /* Modify zone dimensions by shifting the ego edge */
   if (Fbk_Is_True(p_lcda_cal->k_bsw_f_enable_trailer_zone_adjustment_on_ego_side))
   {
      p_bsw_zone->points[FRONT_EGO_SIDE].y += lat_adj;
      p_bsw_zone->points[MIDDLE_EGO_SIDE].y += lat_adj;
      p_bsw_zone->points[REAR_EGO_SIDE].y += lat_adj;
   }
}
