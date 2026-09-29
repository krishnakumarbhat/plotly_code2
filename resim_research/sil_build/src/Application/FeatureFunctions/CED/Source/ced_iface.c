/**
 * @file ced_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the interface functions to initialize and run the CED feature.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_iface.h"
#include "ced.h"
#include "ced_core_calibration.h"
#include "ced_customer_calibration.h"
#include "ced_debug_interface.h"
#include "ced_debug_writer.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_output_t.h"
#include "ced_post_run.h"
#include "ced_pre_run.h"
#include "ced_update_calibration.h"
#include "fbk_iface.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "sfl_status.h"
#include <assert.h>


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/**
 * @brief Ced_Sw_Major_Version static memory
 * @SDD{SF-3620}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ced_Sw_Major_Version = 15u;

/**
 * @brief Ced_Sw_Minor_Version static memory
 * @SDD{SF-3621}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ced_Sw_Minor_Version = 9u;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Checks if any pointers are invalid.
 *
 * @return True if null pointer is detected
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3613}
 * @verification{Create a test to check whether a null pointer is detected.}
 */
static boolean_T Ced_Null_Pointer_Detected(const Ced_Instance_T *p_ced_instance,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Ced_Input_T *p_ced_input,
                                           const Ced_Output_T *p_ced_output);


/*===========================================================================*\
 * Global function definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
Sfl_Status_T Ced_Init_Platform(Ced_Instance_T *p_ced_instance)
{
   Fbk_Index_Id_Lookup_Table_T empty_lookup_table = {0};

   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));

   /* Initialization before customer pre run function */
   Ced_Pre_Run_Init(p_ced_instance);

   /* Initialize cal values */
   Ced_Core_Cal_Update_Defaults(&(p_ced_instance->calibration));
   Ced_Customer_Cal_Update_Defaults(&(p_ced_instance->customer_calibration));

   /* Initialize the core output */
   Fbk_Reset_Index_Id_Lookup_Table(&empty_lookup_table);

   Ced_Reset(&(p_ced_instance->core_output), &p_ced_instance->persistance, &empty_lookup_table);

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
Sfl_Status_T Ced_Run_Platform(Ced_Instance_T *p_ced_instance,
                              const Ced_Input_T *p_ced_input,
                              Ced_Output_T *p_ced_output,
                              const Fbk_Output_T *p_fbk_output,
                              const Pt_Output_T *p_pt_output)
{
   if (Fbk_Is_False(Ced_Null_Pointer_Detected(p_ced_instance, p_fbk_output, p_ced_input, p_ced_output)))
   {

      Ced_Pre_Run(p_ced_instance, p_ced_input, p_pt_output, p_fbk_output);

      Ced_Core_Run(&(p_ced_instance->core_output), &(p_ced_instance->core_input), &(p_ced_instance->persistance),
                   &(p_ced_instance->calibration), p_fbk_output);

      Ced_Post_Run(p_ced_instance, p_ced_input, p_ced_output);

      /* Pass software version to debug structure */
      Binary_Ced_Debug_Pass_Sw_Version(Ced_Sw_Major_Version, Ced_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Ced_Write_Bin_File();
   }

   return SFL_STATUS_OK;
}


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ced_Update_Calibration_Platform(Ced_Instance_T *p_ced_instance, const Ced_Public_Calibration_T *cal_src)
{
   boolean_T f_update_core_success   = Ced_Update_Core_Cal_By_Public(&p_ced_instance->calibration, cal_src);
   boolean_T f_customer_core_success = Ced_Update_Customer_Cal_By_Public(&p_ced_instance->customer_calibration, cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      Fbk_Index_Id_Lookup_Table_T empty_lookup_table = {0};
      /* Reset Cta */
      Ced_Reset(&(p_ced_instance->core_output), &(p_ced_instance->persistance), &empty_lookup_table);

      /* Initialize customer post run */
      Ced_Post_Run_Init(p_ced_instance);
   }
   return f_success;
}


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ced_Get_Sw_Major_Version(void)
{
   return (Ced_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ced_Get_Sw_Minor_Version(void)
{
   return (Ced_Sw_Minor_Version);
}


/*===========================================================================*\
* Local Functions
\*===========================================================================*/

static boolean_T Ced_Null_Pointer_Detected(const Ced_Instance_T *p_ced_instance,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Ced_Input_T *p_ced_input,
                                           const Ced_Output_T *p_ced_output)
{
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /* Check if NULL pointers are in use */
   if ((p_ced_instance == NULL) || (p_fbk_output == NULL) || (p_ced_input == NULL) || (p_ced_output == NULL))
   {
      f_null_pointer_detected = FBK_TRUE;
   }

   return f_null_pointer_detected;
}
