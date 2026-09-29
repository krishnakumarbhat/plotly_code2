/**
 * @file lcda_bmw_sp25_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains some debug logic required for debugging
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "lcda_bmw_sp25_debug_interface.h"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* Static variable definitions
\*===========================================================================*/

static Lcda_Bmw_Sp25_Debug_Data_T Lcda_Bmw_Sp25_Debug_Data;

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Lcda_Bmw_Sp25_Debug_Data_T *Lcda_Get_Bmw_Sp25_Debug_Data(void)
{
   return &Lcda_Bmw_Sp25_Debug_Data;
}

void Lcda_Bmw_Sp25_Fill_Debug_Data(const Lcda_Input_T *p_lcda_input, const Lcda_Output_T *p_lcda_output)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);

   /* Copy data from internal interfaces to debug output interface. */
   Lcda_Bmw_Sp25_Debug_Data.lcda_input  = *p_lcda_input;
   Lcda_Bmw_Sp25_Debug_Data.lcda_output = *p_lcda_output;
}


void Pass_Lcda_Debug_Bmw_Sp25_Lane_Model(const Lane_Model_Output_Camera_T *p_lane_output_camera,
                                         const Lane_Model_Output_T *p_lane_model_output,
                                         uint16_t vdyn_count_city2hway,
                                         uint16_t vdyn_count_hway2city,
                                         uint16_t navi_count_city,
                                         uint16_t navi_count_hway,
                                         uint8_t vdyn_road_type,
                                         uint8_t navi_road_type)
{
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.lane_model_output        = *p_lane_model_output;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.lane_model_output_camera = *p_lane_output_camera;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.vdyn_count_city2hway     = vdyn_count_city2hway;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.vdyn_count_hway2city     = vdyn_count_hway2city;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.navi_count_city          = navi_count_city;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.navi_count_hway          = navi_count_hway;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.vdyn_road_type           = vdyn_road_type;
   Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.lane_model.navi_road_type           = navi_road_type;
}

void Pass_Lcda_Debug_Bmw_Sp25_Pre_Run(uint8_t lane_change_counter[FBK_NUMBER_OF_SIDES],
                                      uint8_t camera_lane_plausibilisation_counter[FBK_NUMBER_OF_SIDES])
{
   uint8_t i;

   for (i = 0; i < FBK_NUMBER_OF_SIDES; ++i)
   {
      Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.pre_run.lane_change_counter[i] = lane_change_counter[i];
      Lcda_Bmw_Sp25_Debug_Data.lcda_debug_output.pre_run.camera_lane_plausibilisation_counter[i] =
         camera_lane_plausibilisation_counter[i];
   }
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
