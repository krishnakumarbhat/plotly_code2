#ifndef TA_BMW_SP25_DEBUG_INTERFACE_H
#define TA_BMW_SP25_DEBUG_INTERFACE_H

/**
 * @file ta_bmw_sp25_debug_information.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains some debug logic required for debugging
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"
#include "ta_input_t.h"
#include "ta_output_t.h"
#include "ta_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   float32_T ta_host_speed_at_brake_start;
   float32_T ta_host_speed_reduction_requested;
   float32_T ta_host_speed_reduction_achieved;
   uint8_t pfgs_qualification_counter_min;
   uint8_t pfgs_qualification_counter;

   float32_T ta_lookup_turning_host_curvature_threshold;
   float32_T f_pfgs_relevant_host_speed;
   uint8_t ta_core_maneuver;
} Ta_Bmw_Sp25_Debug_Output_T;

typedef struct
{
   Ta_Input_T ta_input;
   Ta_Output_T ta_output;
   Ta_Bmw_Sp25_Debug_Output_T ta_debug_output;
} Ta_Bmw_Sp25_Debug_Data_T;

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   void Pass_Ta_Debug_Bmw_Sp25_Various(const Ta_Input_T *p_ta_input,
                                       const Fbk_Vehicle_Data_T *p_vehicle_data,
                                       const Ta_Core_Calibration_T *p_ta_cal,
                                       const float32_T ta_host_speed_at_brake_start,
                                       const float32_T ta_host_speed_reduction_requested,
                                       const float32_T ta_host_speed_reduction_achieved,
                                       const uint8_t pfgs_qualification_counter_min,
                                       const uint8_t pfgs_qualification_counter);

   Ta_Bmw_Sp25_Debug_Data_T *Ta_Get_Bmw_Sp25_Debug_Data(void);

   void Ta_Bmw_Sp25_Fill_Debug_Data(const Ta_Input_T *p_ta_input, const Ta_Output_T *p_ta_output);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

#define Binary_Ta_Bmw_Sp25_Fill_Debug_Data(p_ta_input, p_ta_output) Ta_Bmw_Sp25_Fill_Debug_Data(p_ta_input, p_ta_output)

#define Binary_Pass_Ta_Debug_Bmw_Sp25_Various(p_ta_input, p_vehicle_data, p_ta_cal, ta_host_speed_at_brake_start,  \
                                              ta_host_speed_reduction_requested, ta_host_speed_reduction_achieved, \
                                              pfgs_qualification_counter_min, pfgs_qualification_counter)          \
   Pass_Ta_Debug_Bmw_Sp25_Various(p_ta_input, p_vehicle_data, p_ta_cal, ta_host_speed_at_brake_start,              \
                                  ta_host_speed_reduction_requested, ta_host_speed_reduction_achieved,             \
                                  pfgs_qualification_counter_min, pfgs_qualification_counter)

#else

#define Binary_Ta_Bmw_Sp25_Fill_Debug_Data(p_ta_input, p_ta_output)

#define Binary_Pass_Ta_Debug_Bmw_Sp25_Various(p_ta_input, p_vehicle_data, p_ta_cal, ta_host_speed_at_brake_start,  \
                                              ta_host_speed_reduction_requested, ta_host_speed_reduction_achieved, \
                                              pfgs_qualification_counter_min, pfgs_qualification_counter)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* TA_BMW_SP24_DEBUG_INFORMATION_H */
