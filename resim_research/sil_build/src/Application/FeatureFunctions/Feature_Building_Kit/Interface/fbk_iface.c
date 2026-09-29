/**
 * @file fbk_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Feature building kit configuration file with the interface for shared functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_iface.h"
#include "fbk_core_calibration.h"
#include "fbk_debug_interface.h"
#include "fbk_debug_writer.h"
#include "fbk_fill_pa_data.h"
#include "fbk_host_trail.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_obj_ageing.h"
#include "fbk_output_boundary_check.h"
#include "pa_context.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/**
 * @brief Fbk_Sw_Major_Version static memory
 */
static const uint16_t Fbk_Sw_Major_Version = 8u;

/**
 * @brief Fbk_Sw_Minor_Version static memory
 */
static const uint16_t Fbk_Sw_Minor_Version = 1u;

/*============================================================================*\
 * Global Functions
\*============================================================================*/
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Fbk_Init_Platform(Fbk_Instance_T *p_fbk_instance, Pa_Data_T *p_pa_data)
{
   assert(NULL != p_fbk_instance);
   /*Initialize Cal values*/
   Fbk_Core_Cal_Update_Defaults(&p_fbk_instance->calibration);

   /* Reset object aging */
   Fbk_Reset_Object_Ageing(&p_fbk_instance->fbk_obj_ages);

   p_fbk_instance->p_pa_data = p_pa_data;

   /* Initializes host trail */
   Fbk_Init_Host_Trail(&p_fbk_instance->host_trail);
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Fbk_Run_Platform(Fbk_Instance_T *p_fbk_instance, Fbk_Output_T *p_fbk_output, Pa_Context_T *p_context)
{
   Sfl_Status_T sfl_status;

   assert(NULL != p_fbk_instance);
   assert(NULL != p_fbk_output);
   assert(NULL != p_context);

   sfl_status = Fbk_Fill_Pa_Data(p_fbk_instance->p_pa_data, p_context);

   /* Reset debug data */
   Binary_Fbk_Debug_Reset_Data();

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&p_fbk_instance->fbk_index_id_lookup_table, p_fbk_instance->p_pa_data);

   /* Run Feature Building Kit object aging */
   Fbk_Update_Object_Ageing(&p_fbk_instance->fbk_obj_ages, p_fbk_instance->p_pa_data);

   /* Run host trail module */
   Fbk_Update_Host_Trail(&p_fbk_instance->host_trail, p_fbk_instance->p_pa_data, &p_fbk_instance->calibration);
   p_fbk_output->p_host_trail            = &p_fbk_instance->host_trail;
   p_fbk_output->p_index_id_lookup_table = &p_fbk_instance->fbk_index_id_lookup_table;
   p_fbk_output->p_pa_data               = p_fbk_instance->p_pa_data;

   if (Fbk_Is_False(Fbk_Are_Outputs_In_Boundary(&p_fbk_instance->fbk_index_id_lookup_table, &p_fbk_instance->fbk_obj_ages)))
   {
      Fbk_Reset_Index_Id_Lookup_Table(&p_fbk_instance->fbk_index_id_lookup_table);
      Fbk_Reset_Object_Ageing(&p_fbk_instance->fbk_obj_ages);
   }

   /* Pass calibration values, platform abstraction data and software version to debug structure */
   Binary_Fbk_Debug_Pass_General_Data(&p_fbk_instance->calibration, p_fbk_instance->p_pa_data, sfl_status);
   Binary_Fbk_Debug_Pass_Sw_Version(Fbk_Sw_Major_Version, Fbk_Sw_Minor_Version);

   /* Write debug output to bin file */
   Binary_Fbk_Write_Bin_File();
   return sfl_status;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Fbk_Get_Sw_Major_Version(void)
{
   /* Return Fbk_Sw_Major_Version */
   return (Fbk_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Fbk_Get_Sw_Minor_Version(void)
{
   /* Return Fbk_Sw_Minor_Version */
   return (Fbk_Sw_Minor_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Fbk_Is_Version_Compatible(const uint16_t major_version_min, const uint16_t minor_version_min)
{
   /* Return value */
   boolean_T f_version_compatible = FBK_FALSE;

   /* Check current FBK version against given minimum version requirement. */
   if (((Fbk_Sw_Minor_Version >= minor_version_min) && (Fbk_Sw_Major_Version == major_version_min))
       || (Fbk_Sw_Major_Version > major_version_min))
   {
      f_version_compatible = FBK_TRUE;
   }

   /* Return result of version compatibility check */
   return f_version_compatible;
}
