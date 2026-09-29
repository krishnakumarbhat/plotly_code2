/**
 * @file lcda_rna_sweet400_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains some debug logic required for debugging
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "lcda_rna_sweet400_debug_interface.h"
#include "lcda_output_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* Static variable definitions
\*===========================================================================*/

static Lcda_Rna_Sweet400_Debug_Data_T Lcda_Rna_Sweet400_Debug_Data;

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Lcda_Rna_Sweet400_Debug_Data_T *Lcda_Get_Rna_Sweet400_Debug_Data(void)
{
   return &Lcda_Rna_Sweet400_Debug_Data;
}

void Lcda_Rna_Sweet400_Fill_Debug_Data(const Lcda_Input_T *p_lcda_input, const Lcda_Output_T *p_lcda_output)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);

   /* Copy data from internal interfaces to debug output interface. */
   Lcda_Rna_Sweet400_Debug_Data.lcda_input  = *p_lcda_input;
   Lcda_Rna_Sweet400_Debug_Data.lcda_output = *p_lcda_output;
}

void Pass_Lcda_Debug_Rna_Sweet400_zones(float32_T lka_zone_lat_start,
                                        float32_T lka_zone_lat_end,
                                        float32_T lka_zone_lon_start,
                                        float32_T lka_zone_lon_end,
                                        float32_T lka_zone_hys_lat_start,
                                        float32_T lka_zone_hys_lat_end,
                                        float32_T lka_zone_hys_lon_start,
                                        float32_T lka_zone_hys_lon_end)
{
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone.lat_start     = lka_zone_lat_start;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone.lat_end       = lka_zone_lat_end;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone.lon_start     = lka_zone_lon_start;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone.lon_end       = lka_zone_lon_end;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone_hys.lat_start = lka_zone_hys_lat_start;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone_hys.lat_end   = lka_zone_hys_lat_end;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone_hys.lon_start = lka_zone_hys_lon_start;
   Lcda_Rna_Sweet400_Debug_Data.lcda_debug_output.lka_zone_hys.lon_end   = lka_zone_hys_lon_end;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
