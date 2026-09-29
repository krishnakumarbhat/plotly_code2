/**
 * @file pt_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the interface implementation of path tracking algorithm.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_iface.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pt.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_customer_calibration.h"
#include "pt_debug_interface.h"
#include "pt_debug_writer.h"
#include "pt_input_t.h"
#include "pt_modify_grid_cals.h"
#include "pt_persistent_handler.h"
#include "pt_persistent_t.h"
#include "pt_reset.h"
#include "pt_update_calibration.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/
/**
 * @brief Pt_Sw_Major_Version static memory
 * @SDD{SF-7405}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Pt_Sw_Major_Version = 9u;

/**
 * @brief Pt_Sw_Minor_Version static memory
 * @SDD{SF-7405}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Pt_Sw_Minor_Version = 1u;

/*===========================================================================*\
* Global Functions Prototypes
\*===========================================================================*/

/**
 * This function checks if all pointers to path tracking structs are valid
 *
 * @SRS{SF-1522,SF-1520,SF-1521}
 * @SAE{SF-2918}
 * @SDD{SF-7405}
 * @verification{}
 * @return     boolean_T whether all points are valid
 *
 */
static boolean_T Pt_Are_All_Pointers_Valid(const Pt_Instance_T *p_pt_instance,
                                           const Pt_Output_T *p_pt_output,
                                           const Fbk_Output_T *p_fbk_output);


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Pt_Init_Platform(Pt_Instance_T *p_pt_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_pt_instance);

   /* Initialize cal values */
   Pt_Core_Cal_Update_Defaults(&p_pt_instance->calibration);
   Pt_Customer_Cal_Update_Defaults(&p_pt_instance->customer_calibration);

   /*Initialize customer dependent grid array*/
   Pt_Update_Grid_Array_Defaults(p_pt_instance->pt_core_input.grid_pt_array);

   /*adapt amount of grid points dependent calibrations to the customer specific grid*/
   Pt_Set_Num_Grid_Pts_Dependend_Cals(&p_pt_instance->calibration, p_pt_instance->pt_core_input.grid_pt_array,
                                      &p_pt_instance->pt_core_input.Num_Grid_Pts_Dep_Cals);

   /*Reset path array, persistent data and matches*/
   Pt_Reset_All_Paths(p_pt_instance->persistent.paths, NULL, p_pt_instance->persistent.best_path_obj_pairs);
   Pt_Reset_Persistent(&p_pt_instance->persistent);
   Pt_Reset_All_Matching_Pairs(p_pt_instance->persistent.best_path_obj_pairs);
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Pt_Run_Platform(Pt_Instance_T *p_pt_instance, Pt_Output_T *p_pt_output, const Fbk_Output_T *p_fbk_output)
{
   if (Pt_Are_All_Pointers_Valid(p_pt_instance, p_pt_output, p_fbk_output))
   {
      p_pt_instance->pt_core_input.p_fbk_output = p_fbk_output;
      Pt_Core_Run(p_pt_output, &p_pt_instance->persistent, &p_pt_instance->pt_core_input, &p_pt_instance->calibration);

      /* Pass software version to debug structure */
      Binary_Pt_Debug_Pass_Sw_Version(Pt_Sw_Major_Version, Pt_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Pt_Write_Bin_File();
   }
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Pt_Update_Calibration_Platform(Pt_Instance_T *p_pt_instance, const Pt_Public_Calibration_T *cal_src)
{
   boolean_T f_update_core_success   = Pt_Update_Core_Cal_By_Public(&p_pt_instance->calibration, cal_src);
   boolean_T f_customer_core_success = Pt_Update_Customer_Cal_By_Public(&p_pt_instance->customer_calibration, cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      /*adapt amount of grid points dependent calibrations to the customer specific grid*/
      Pt_Set_Num_Grid_Pts_Dependend_Cals(&p_pt_instance->calibration, p_pt_instance->pt_core_input.grid_pt_array,
                                         &p_pt_instance->pt_core_input.Num_Grid_Pts_Dep_Cals);

      /*Reset path array, persistent data and matches*/
      Pt_Reset_All_Paths(p_pt_instance->persistent.paths, NULL, p_pt_instance->persistent.best_path_obj_pairs);
      Pt_Reset_Persistent(&p_pt_instance->persistent);
      Pt_Reset_All_Matching_Pairs(p_pt_instance->persistent.best_path_obj_pairs);
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
const Pt_Nearest_Path_T *Pt_Get_Nearest_Path_Info(const Pt_Output_T *p_pt_output, uint8_t index)
{
   /*Return nearest path information*/
   return (&p_pt_output->nearest_path_output[index]);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
const Pt_Path_Object_Pair_Output_T *Pt_Get_Match_Information(const Pt_Output_T *p_pt_output, uint8_t index)
{
   /*Return match information for a given index*/
   return (&p_pt_output->path_obj_pair_output[index]);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Pt_Get_Sw_Major_Version(void)
{
   /* Return Pt_Sw_Major_Version */
   return (Pt_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Pt_Get_Sw_Minor_Version(void)
{
   /* Return Pt_Sw_Minor_Version */
   return (Pt_Sw_Minor_Version);
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/
static boolean_T Pt_Are_All_Pointers_Valid(const Pt_Instance_T *p_pt_instance,
                                           const Pt_Output_T *p_pt_output,
                                           const Fbk_Output_T *p_fbk_output)
{
   /* Return flag */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   /* Check that no NULL pointers are in use */
   if ((NULL != p_pt_instance) && (NULL != p_fbk_output) && (NULL != p_fbk_output->p_host_trail)
       && (NULL != p_fbk_output->p_pa_data) && (NULL != p_pt_output))
   {
      f_all_pointers_valid = FBK_TRUE;
   }

   return f_all_pointers_valid;
}
