#ifndef LCDA_RNA_SWEET400_DEBUG_INTERFACE_H
#define LCDA_RNA_SWEET400_DEBUG_INTERFACE_H

/**
 * @file lcda_rna_sweet400_debug_interface.h
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

#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   float32_T lat_start;
   float32_T lat_end;
   float32_T lon_start;
   float32_T lon_end;
} Lcda_Rna_Sweet400_Debug_Data_LKA_Zone_T;

typedef struct
{
   Lcda_Rna_Sweet400_Debug_Data_LKA_Zone_T lka_zone;
   Lcda_Rna_Sweet400_Debug_Data_LKA_Zone_T lka_zone_hys;
} Lcda_Rna_Sweet400_Debug_Output_T;


typedef struct
{
   Lcda_Input_T lcda_input;
   Lcda_Output_T lcda_output;
   Lcda_Rna_Sweet400_Debug_Output_T lcda_debug_output;
} Lcda_Rna_Sweet400_Debug_Data_T;

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   void Pass_Lcda_Debug_Rna_Sweet400_zones(float32_T lka_zone_lat_start,
                                           float32_T lka_zone_lat_end,
                                           float32_T lka_zone_lon_start,
                                           float32_T lka_zone_lon_end,
                                           float32_T lka_zone_hys_lat_start,
                                           float32_T lka_zone_hys_lat_end,
                                           float32_T lka_zone_hys_lon_start,
                                           float32_T lka_zone_hys_lon_end);

   Lcda_Rna_Sweet400_Debug_Data_T *Lcda_Get_Rna_Sweet400_Debug_Data(void);

   void Lcda_Rna_Sweet400_Fill_Debug_Data(const Lcda_Input_T *p_lcda_input, const Lcda_Output_T *p_lcda_output);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

#define Binary_Lcda_Rna_Sweet400_Fill_Debug_Data(p_lcda_input, p_lcda_output) \
   Lcda_Rna_Sweet400_Fill_Debug_Data(p_lcda_input, p_lcda_output)

#define Binary_Pass_Lcda_Debug_Rna_Sweet400_zones(lka_zone_lat_start, lka_zone_lat_end, lka_zone_lon_start, lka_zone_lon_end, \
                                                  lka_zone_hys_lat_start, lka_zone_hys_lat_end, lka_zone_hys_lon_start,       \
                                                  lka_zone_hys_lon_end)                                                       \
   Pass_Lcda_Debug_Rna_Sweet400_zones(lka_zone_lat_start, lka_zone_lat_end, lka_zone_lon_start, lka_zone_lon_end,             \
                                      lka_zone_hys_lat_start, lka_zone_hys_lat_end, lka_zone_hys_lon_start, lka_zone_hys_lon_end)

#else

#define Binary_Lcda_Rna_Sweet400_Fill_Debug_Data(p_lcda_input, p_lcda_output)

#define Binary_Pass_Lcda_Debug_Rna_Sweet400_zones(lka_zone_lat_start, lka_zone_lat_end, lka_zone_lon_start, lka_zone_lon_end, \
                                                  lka_zone_hys_lat_start, lka_zone_hys_lat_end, lka_zone_hys_lon_start,       \
                                                  lka_zone_hys_lon_end)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* LCDA_RNA_SWEET400_DEBUG_INTERFACE_H */
