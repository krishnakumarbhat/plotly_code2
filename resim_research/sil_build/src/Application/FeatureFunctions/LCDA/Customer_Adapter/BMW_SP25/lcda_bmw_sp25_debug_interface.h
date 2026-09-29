#ifndef LCDA_BMW_SP25_DEBUG_INTERFACE_H
#define LCDA_BMW_SP25_DEBUG_INTERFACE_H

/**
 * @file lcda_bmw_sp25_debug_information.h
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

#include "fbk_macros.h"
#include "lane_model.h"
#include "lane_model_camera_data.h"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   uint8_t lane_change_counter[FBK_NUMBER_OF_SIDES];
   uint8_t camera_lane_plausibilisation_counter[FBK_NUMBER_OF_SIDES];
} Lcda_Bmw_Sp25_Debug_Information_Pre_Run_T;

typedef struct
{
   uint16_t vdyn_count_city2hway;
   uint16_t vdyn_count_hway2city;
   uint16_t navi_count_city;
   uint16_t navi_count_hway;
   uint8_t vdyn_road_type;
   uint8_t navi_road_type;

   Lane_Model_Output_Camera_T lane_model_output_camera;

   Lane_Model_Output_T lane_model_output;
} Lcda_Bmw_Sp25_Debug_Information_Lane_Model_T;

typedef struct
{
   Lcda_Bmw_Sp25_Debug_Information_Pre_Run_T pre_run;
   Lcda_Bmw_Sp25_Debug_Information_Lane_Model_T lane_model;
} Lcda_Bmw_Sp25_Debug_Output_T;

typedef struct
{
   Lcda_Input_T lcda_input;
   Lcda_Output_T lcda_output;
   Lcda_Bmw_Sp25_Debug_Output_T lcda_debug_output;
} Lcda_Bmw_Sp25_Debug_Data_T;

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   void Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(const Lane_Model_Output_Camera_T *p_lane_output_camera,
                                            const Lane_Model_Output_T *p_lane_model_output,
                                            uint16_t vdyn_count_city2hway,
                                            uint16_t vdyn_count_hway2city,
                                            uint16_t navi_count_city,
                                            uint16_t navi_count_hway,
                                            uint8_t vdyn_road_type,
                                            uint8_t navi_road_type);

   void Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(uint8_t lane_change_counter[FBK_NUMBER_OF_SIDES],
                                         uint8_t camera_lane_plausibilisation_counter[FBK_NUMBER_OF_SIDES]);

   Lcda_Bmw_Sp25_Debug_Data_T *Lcda_Get_Bmw_Sp25_Debug_Data(void);

   void Lcda_Bmw_Sp25_Fill_Debug_Data(const Lcda_Input_T *p_lcda_input, const Lcda_Output_T *p_lcda_output);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

#define Binary_Lcda_Bmw_Sp25_Fill_Debug_Data(p_lcda_input, p_lcda_output) \
   Lcda_Bmw_Sp25_Fill_Debug_Data(p_lcda_input, p_lcda_output)

#define Binary_Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(p_lane_output_camera, p_lane_model_output, vdyn_count_city2hway,           \
                                                   vdyn_count_hway2city, navi_count_city, navi_count_hway, vdyn_road_type,    \
                                                   navi_road_type)                                                            \
   Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(p_lane_output_camera, p_lane_model_output, vdyn_count_city2hway, vdyn_count_hway2city, \
                                       navi_count_city, navi_count_hway, vdyn_road_type, navi_road_type)

#define Binary_Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(lane_change_counter, camera_lane_plausibilisation_counter) \
   Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(lane_change_counter, camera_lane_plausibilisation_counter);

#else

#define Binary_Lcda_Bmw_Sp25_Fill_Debug_Data(p_lcda_input, p_lcda_output)

#define Binary_Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(p_lane_output_camera, p_lane_model_output, vdyn_count_city2hway,        \
                                                   vdyn_count_hway2city, navi_count_city, navi_count_hway, vdyn_road_type, \
                                                   navi_road_type)

#define Binary_Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(lane_change_counter, camera_lane_plausibilisation_counter)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* LCDA_BMW_SP25_DEBUG_INFORMATION_H */
