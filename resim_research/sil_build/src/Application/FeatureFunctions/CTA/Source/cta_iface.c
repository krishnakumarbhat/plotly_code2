/**
 * @file cta_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains interface function definitions of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_iface.h"
#include "cta.h"
#include "cta_core_calibration.h"
#include "cta_customer_calibration.h"
#include "cta_debug_interface.h"
#include "cta_debug_writer.h"
#include "cta_instance.h"
#include "cta_post_run.h"
#include "cta_pre_run.h"
#include "cta_update_calibration.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Cta_Sw_Major_Version = 14u;

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Cta_Sw_Minor_Version = 0u;


/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief This function checks if all pointers which are needed for the CTA are valid.
 *
 * @return True if all pointers are valid and updated error flags
 *
 * @SAD{SF-2460,SF-2458}
 * @SDD{SF-3826}
 * @verification{Check that this function detects the validity of all provided pointers correctly.}
 */
static boolean_T Cta_Are_All_Pointers_Valid(const Cta_Input_T *p_cta_input /**< CTA input*/,
                                            const Cta_Output_T *p_cta_output /**< CTA output*/,
                                            const Cta_Instance_T *p_cta_instance /**< CTA instance*/,
                                            const Fbk_Output_T *p_fbk_output /**< FBK output */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Cta_Init_Platform(Cta_Instance_T *p_cta_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_cta_instance);

   /* Initialize cal values */
   Cta_Core_Cal_Update_Defaults(&p_cta_instance->calibration);
   Cta_Customer_Cal_Update_Defaults(&p_cta_instance->customer_calibration);

   /* Initialize customer pre run */
   Cta_Pre_Run_Init(p_cta_instance);

   /* Reset Cta */
   Cta_Reset(p_cta_instance);

   /* Initialize customer post run */
   Cta_Post_Run_Init(p_cta_instance);

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Cta_Run_Platform(const Cta_Input_T *p_cta_input,
                              const Fbk_Output_T *p_fbk_output,
                              const Pt_Output_T *p_pt_output,
                              Cta_Instance_T *p_cta_instance,
                              Cta_Output_T *p_cta_output)
{
   /* Runs the CTA feature function algorithm including customer pre and post run */
   if (Fbk_Is_True(Cta_Are_All_Pointers_Valid(p_cta_input, p_cta_output, p_cta_instance, p_fbk_output)))
   {
      Cta_Pre_Run(p_cta_instance, p_cta_input, p_fbk_output, p_pt_output);
      Cta_Core_Run(p_cta_instance);
      Cta_Post_Run(p_cta_instance, p_cta_input, p_cta_output);

      /* Pass software version to debug structure */
      Binary_Cta_Debug_Pass_Sw_Version(Cta_Sw_Major_Version, Cta_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Cta_Write_Bin_File();
   }

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Cta_Update_Calibration_Platform(Cta_Instance_T *p_cta_instance, const Cta_Public_Calibration_T *p_cal_src)
{
   boolean_T f_update_core_success   = Cta_Update_Core_Cal_By_Public(&p_cta_instance->calibration, p_cal_src);
   boolean_T f_customer_core_success = Cta_Update_Customer_Cal_By_Public(&p_cta_instance->customer_calibration, p_cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      /* Reset Cta */
      Cta_Reset(p_cta_instance);

      /* Initialize customer post run */
      Cta_Post_Run_Init(p_cta_instance);
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Cta_Get_Sw_Major_Version(void)
{
   return (Cta_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Cta_Get_Sw_Minor_Version(void)
{
   return (Cta_Sw_Minor_Version);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Cta_Are_All_Pointers_Valid(const Cta_Input_T *p_cta_input,
                                            const Cta_Output_T *p_cta_output,
                                            const Cta_Instance_T *p_cta_instance,
                                            const Fbk_Output_T *p_fbk_output)
{
   boolean_T f_all_pointers_valid = FBK_FALSE;

   if ((NULL != p_cta_input) && (NULL != p_cta_output) && (NULL != p_cta_instance) && (NULL != p_fbk_output))
   {
      f_all_pointers_valid = FBK_TRUE;
   }

   return f_all_pointers_valid;
}
