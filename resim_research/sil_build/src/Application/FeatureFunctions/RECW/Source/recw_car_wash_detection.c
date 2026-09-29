/**
 * @file recw_car_wash_detection.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the car wash detection source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_car_wash_detection.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_shared_types.h"
#include <assert.h>
#include <string.h>

/*===========================================================================*\
 * Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Checks if target is likely to be ghost object in car wash scenario
 *
 * @return void
 *
 * @SRS{SF-1693}
 * @SAE{SF-2959}
 * @SDD{SF-7857}
 * @verification{}
 */
static void
Recw_Eval_Car_Wash_Conditions(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                              const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */,
                              const Recw_Object_T *p_recw_object /**< RECW object data */,
                              Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /* Possible car  wash scenario flags */);

/**
 * @brief Sets the bit dedicated to the corresponding object idx to 1.
 *
 * @return void
 *
 * @SRS{SF-1693}
 * @SAE{SF-2959}
 * @SDD{SF-7860}
 * @verification{}
 */
static void Recw_Set_Car_Wash_Flag_True(Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /* Possible car  wash scenario flags*/,
                                        const uint8_t obj_idx /**< object index*/);

/**
 * @brief Sets the bit dedicated to the corresponding object idx to 0.
 *
 * @return void
 *
 * @SRS{SF-1693}
 * @SAE{SF-2959}
 * @SDD{SF-7859}
 * @verification{}
 */
static void
Recw_Set_Car_Wash_Flag_False(Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /* Possible car  wash scenario flags */,
                             const uint8_t obj_idx /**< object index*/);

/**
 * @brief Checks whether scenario is a car wash scenario
 *
 * @return True if bit dedicated to the object idx is 1 and returns FBK_FALSE otherwise.
 *
 * @SRS{SF-1713}
 * @SAE{SF-2959}
 * @SDD{SF-7858}
 * @verification{}
 */
static boolean_T
Recw_Is_Car_Wash_Scenario(const Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /* Possible car  wash scenario flags */,
                          const uint8_t obj_idx /**< object index*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

boolean_T Recw_Is_Obj_Car_Wash_Ghost(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                     const Recw_Core_Calibration_T *p_cals,
                                     const Recw_Object_T *p_recw_object,
                                     Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags)
{
   boolean_T f_obj_is_car_wash_ghost = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);
   assert(NULL != p_recw_object);

   switch (p_cals->k_recw_en_active_car_wash_logic)
   {
      case (uint8_t) RECW_CAR_WASH_TARGET_STATE:
      {
         Recw_Eval_Car_Wash_Conditions(p_vehicle_data, p_cals, p_recw_object, p_car_wash_scenario_flags);
         f_obj_is_car_wash_ghost = Recw_Is_Car_Wash_Scenario(p_car_wash_scenario_flags, p_recw_object->tracker_data.index);
         break;
      }
      case (uint8_t) RECW_CAR_WASH_NEUTRAL_GEAR:
      {
         if ((PA_VEH_PRNDL_STATE_NEUTRAL == p_vehicle_data->prndl)
             && (Fbk_Abs_F(p_vehicle_data->host_speed) < p_cals->k_recw_max_speed_ego_car_wash))
         {
            f_obj_is_car_wash_ghost = FBK_TRUE;
         }
         break;
      }
      case (uint8_t) RECW_CAR_WASH_OFF:
      default:
      {
         /* Do nothing, car wash is supposed to be off */
         break;
      }
   }

   return f_obj_is_car_wash_ghost;
}

void Recw_Init_Car_Wash_Flags(uint8_t possible_car_wash_scenario_flags[RECW_CAR_WASH_FLAG_ARRAY_SIZE])
{
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(possible_car_wash_scenario_flags, 0, RECW_CAR_WASH_FLAG_ARRAY_SIZE);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Recw_Eval_Car_Wash_Conditions(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                          const Recw_Core_Calibration_T *p_cals,
                                          const Recw_Object_T *p_recw_object,
                                          Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags)
{
   float32_T lon_distance;
   float32_T lat_distance;

   /* Asserts */
   assert(NULL != p_vehicle_data);
   assert(NULL != p_cals);
   assert(NULL != p_recw_object);

   if (p_recw_object->tracker_data.vcs_vel_rel.x < p_cals->k_recw_max_rel_lon_vel_release_car_wash)
   {
      Recw_Set_Car_Wash_Flag_False(p_car_wash_scenario_flags, p_recw_object->tracker_data.index);
   }
   else if (p_recw_object->tracker_data.age == (p_cals->k_recw_min_object_age + 1u))
   {
      lon_distance = Fbk_Abs_F(
         (p_recw_object->tracker_data.vcs_pos.x + p_vehicle_data->host_length + (0.5f * p_recw_object->tracker_data.length)));
      lat_distance = Fbk_Abs_F(p_recw_object->tracker_data.vcs_pos.y);
      if ((lon_distance < p_cals->k_recw_max_lon_distance_car_wash) && (lat_distance < p_cals->k_recw_max_lat_distance_car_wash)
          && (p_recw_object->tracker_data.vcs_vel_rel.x > p_cals->k_recw_min_rel_lon_vel_car_wash)
          && (p_vehicle_data->host_speed < p_cals->k_recw_max_speed_ego_car_wash))
      {
         Recw_Set_Car_Wash_Flag_True(p_car_wash_scenario_flags, p_recw_object->tracker_data.index);
      }
      else
      {
         Recw_Set_Car_Wash_Flag_False(p_car_wash_scenario_flags, p_recw_object->tracker_data.index);
      }
   }
   else
   {
      /* do nothing */
   }
}

static void Recw_Set_Car_Wash_Flag_True(Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags, const uint8_t obj_idx)
{
   uint8_t array_idx;
   uint8_t shift_val;

   /* Assert */
   assert(obj_idx < PA_OBJ_NUMBER_OF_OBJECTS);

   /**
    * array_idx is calculated (division by 8) to find the byte in which the bit is included dedicated to the object idx
    * shift_val is calculated (modulo 8) to find the position of the bit within the byte accessed by array_idx
    */
   array_idx = obj_idx >> 3u;
   shift_val = obj_idx & (uint8_t) (0x07u);
   p_car_wash_scenario_flags->possible_car_wash_scenario_flags[array_idx] |= (uint8_t) (0x01u << shift_val);
}

static void Recw_Set_Car_Wash_Flag_False(Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags, const uint8_t obj_idx)
{
   uint8_t array_idx;
   uint8_t shift_val;

   /* Assert */
   assert(obj_idx < PA_OBJ_NUMBER_OF_OBJECTS);

   /**
    * array_idx is calculated (division by 8) to find the byte in which the bit is included dedicated to the object idx
    * shift_val is calculated (modulo 8) to find the position of the bit within the byte accessed by array_idx
    */
   array_idx = obj_idx >> 3u;
   shift_val = obj_idx & (uint8_t) (0x07u);
   p_car_wash_scenario_flags->possible_car_wash_scenario_flags[array_idx] &= (uint8_t) ~(0x01u << shift_val);
}

static boolean_T Recw_Is_Car_Wash_Scenario(const Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags, const uint8_t obj_idx)
{
   uint8_t array_idx;
   uint8_t shift_val;
   boolean_T result;

   /* Assert */
   assert(obj_idx < PA_OBJ_NUMBER_OF_OBJECTS);

   /**
    * array_idx is calculated (division by 8) to find the byte in which the bit is included dedicated to the object idx
    * shift_val is calculated (modulo 8) to find the position of the bit within the byte accessed by array_idx
    */
   array_idx = obj_idx >> 3u;
   shift_val = obj_idx & (uint8_t) (0x07u);
   /* coverity[misra_c_2012_rule_10_4_violation][Intentional to unsigned int type] */
   result = (boolean_T) (Fbk_Is_True(
      (p_car_wash_scenario_flags->possible_car_wash_scenario_flags[array_idx] & ((uint8_t) (0x01u) << shift_val))));

   return result;
}
