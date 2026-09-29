
/**
 * @file lcda_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the Lane Change Decision Aid (LCDA) Function Interface
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_iface.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "lcda.h"
#include "lcda_core_calibration.h"
#include "lcda_customer_calibration.h"
#include "lcda_debug_interface.h"
#include "lcda_debug_writer.h"
#include "lcda_post_run.h"
#include "lcda_pre_run.h"
#include "lcda_update_calibration.h"
#include <assert.h>


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Lcda_Sw_Major_Version = 22u;

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Lcda_Sw_Minor_Version = 3u;

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Lcda_Init_Platform(Lcda_Instance_T *p_lcda_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));

   /* Initialize customer pre run */
   Lcda_Pre_Run_Init(p_lcda_instance);

   /* initialize cal values */
   Lcda_Core_Cal_Update_Defaults(&p_lcda_instance->calibration);
   Lcda_Customer_Cal_Update_Defaults(&p_lcda_instance->customer_calibration);

   /* Initialize core */
   Lcda_Reset(p_lcda_instance, NULL);

   /* Initialize customer post run */
   /* coverity[misra_c_2012_rule_2_2_violation][Function call "Lcda_Post_Run_Init(void)" has no effect for some customer.] */
   Lcda_Post_Run_Init();
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Lcda_Run_Platform(Lcda_Instance_T *p_lcda_instance,
                               const Lcda_Input_T *p_lcda_input,
                               const Fbk_Output_T *p_fbk_output,
                               Lcda_Output_T *p_lcda_output)
{
   if (Fbk_Is_False((p_lcda_instance == NULL) || (p_lcda_input == NULL) || (p_fbk_output == NULL) || (p_lcda_output == NULL)))
   {
      Lcda_Pre_Run(p_lcda_instance, p_lcda_input, p_fbk_output);
      Lcda_Core_Run(p_lcda_instance, p_fbk_output);
      Lcda_Post_Run(p_lcda_instance, p_lcda_input, p_lcda_output, p_fbk_output);

      /* Pass software version to debug structure */
      Binary_Lcda_Debug_Pass_Sw_Version(Lcda_Sw_Major_Version, Lcda_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Lcda_Write_Bin_File();
   }
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Lcda_Update_Calibration_Platform(Lcda_Instance_T *p_lcda_instance, const Lcda_Public_Calibration_T *cal_src)
{
   boolean_T f_update_core_success   = Lcda_Update_Core_Cal_By_Public(&p_lcda_instance->calibration, cal_src);
   boolean_T f_customer_core_success = Lcda_Update_Customer_Cal_By_Public(&p_lcda_instance->customer_calibration, cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      /* Initialize core */
      Lcda_Reset(p_lcda_instance, NULL);

      /* Initialize customer post run */
      /* coverity[misra_c_2012_rule_2_2_violation][Function call "Lcda_Post_Run_Init(void)" has no effect for some customer.] */
      Lcda_Post_Run_Init();
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Lcda_Get_Sw_Major_Version(void)
{
   return (Lcda_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Lcda_Get_Sw_Minor_Version(void)
{
   return (Lcda_Sw_Minor_Version);
}
