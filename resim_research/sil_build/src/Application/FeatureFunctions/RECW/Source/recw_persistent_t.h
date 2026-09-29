#ifndef RECW_PERSISTENT_T_H
#define RECW_PERSISTENT_T_H

/**
 * @file recw_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW persistent data header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "recw_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Recw_Object_Persistent_T structure
 *
 * Stores object persistent data
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7829}
 * @verification{}
 */
typedef struct
{
   Vector_2d_T last_object_pos;
   Vector_2d_T last_object_pos_filtered;
   uint8_t filter_counter;
   uint8_t object_within_lane_counter;
   uint8_t consecutive_min_crash_prob_counter;
   uint8_t age; /* Use own age since RECW cannot reset the Tracker age */
} Recw_Object_Persistent_T;

/**
 * Macro is used to define the array size in a way that the array includes
 * the minimum number of bytes in order to have one bit for each object available
 */
#define RECW_CAR_WASH_FLAG_ARRAY_SIZE ((PA_OBJ_NUMBER_OF_OBJECTS + 7u) / 8u)

typedef struct
{
   uint8_t possible_car_wash_scenario_flags[RECW_CAR_WASH_FLAG_ARRAY_SIZE];
} Car_Wash_Scenario_Flags_T;


/**
 * @brief Recw_Object_Persistent_T structure
 *
 * Stores overall Recw persistent data
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7826}
 * @verification{}
 */
typedef struct
{
   Recw_Object_Persistent_T object_data[RECW_MAX_ID_ARRAY_SIZE];

   uint8_t recw_alert_qualifying_counter;
   uint8_t recw_alert_holding_counter;
   uint8_t recw_alert_duration_counter;

   uint8_t recw_id_prev_cycle;
   uint8_t recw_index_prev_cycle;
   float32_T recw_ttc_value_hold;
   Recw_Alert_T recw_alert_prev_cycle;

   uint8_t recw_rear_blockage_qualifying_counter;
   uint8_t recw_rear_blockage_object_index;
   boolean_T f_is_rear_blocked;
   boolean_T f_host_speed_in_allowed_range;
   Car_Wash_Scenario_Flags_T car_wash_scenario_flags;

} Recw_Persistent_T;

#endif /* RECW_PERSISTENT_T_H */
