/**
 * @file scw_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief SCW configuration file with the interface for the state machine.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_iface.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "scw.h"
#include "scw_core_calibration.h"
#include "scw_customer_calibration.h"
#include "scw_debug_interface.h"
#include "scw_debug_writer.h"
#include "scw_post_run.h"
#include "scw_pre_run.h"
#include "scw_update_calibration.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint8_t Scw_Sw_Major_Version = 10u;

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint8_t Scw_Sw_Minor_Version = 0u;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/


/**
 * @brief Check for NULL pointers
 *
 * @return True for Null poiner detected otherwise false
 *
 * @SRS{}
 * @SAE{SF-2990}
 * @SDD{SF-8071}
 * @verification{Create a test to check if the null pointer is detected correctly}
 */
static boolean_T Scw_Null_Pointer_Detected(const Scw_Instance_T *p_scw_instance /**< Scw instance */,
                                           const Scw_Input_T *p_scw_input /**< Scw input */,
                                           const Fbk_Output_T *p_fbk_output /**< Fbk output */,
                                           const Scw_Output_T *p_scw_output /**< Scw output */);

/*===========================================================================*\
* Local Functions
\*===========================================================================*/

static boolean_T Scw_Null_Pointer_Detected(const Scw_Instance_T *p_scw_instance,
                                           const Scw_Input_T *p_scw_input,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Scw_Output_T *p_scw_output)
{
   /* Return flag */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /* Check that no NULL pointers are in use */
   if ((NULL == p_scw_instance) || (NULL == p_scw_input) || (NULL == p_fbk_output) || (NULL == p_scw_output))
   {
      f_null_pointer_detected = FBK_TRUE;
   }

   return f_null_pointer_detected;
}

/*============================================================================*\
 * Global Functions
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Scw_Init_Platform(Scw_Instance_T *p_scw_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_scw_instance);

   /* Initialize customer pre run */
   Scw_Pre_Run_Init(p_scw_instance);

   /* Set cal pointer */
   Scw_Core_Cal_Update_Defaults(&p_scw_instance->calibration);
   Scw_Customer_Cal_Update_Defaults(&p_scw_instance->customer_calibration);

   /* Reset SCW */
   Scw_Reset(&p_scw_instance->core_input, &p_scw_instance->core_output, &p_scw_instance->persistent, &p_scw_instance->calibration);

   /* Initialize customer post run */
   Scw_Post_Run_Init(p_scw_instance);

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Scw_Run_Platform(Scw_Instance_T *p_scw_instance,
                              const Scw_Input_T *p_scw_input,
                              const Fbk_Output_T *p_fbk_output,
                              Scw_Output_T *p_scw_output)
{
   /* Asserts */
   assert(NULL != p_scw_instance);

   /* Run the SCW feature function algorithm including customer pre and post run */
   if (Fbk_Is_False(Scw_Null_Pointer_Detected(p_scw_instance, p_scw_input, p_fbk_output, p_scw_output)))
   {
      Scw_Pre_Run(p_scw_instance, p_scw_input, p_fbk_output);
      Scw_Core_Run(&p_scw_instance->core_output, &p_scw_instance->core_input, &p_scw_instance->persistent,
                   &p_scw_instance->calibration);
      Scw_Post_Run(p_scw_instance, p_scw_input, p_scw_output);

      /* Pass software version to debug structure */
      Binary_Scw_Debug_Pass_Sw_Version(Scw_Sw_Major_Version, Scw_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Scw_Write_Bin_File();
   }

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Scw_Update_Calibration_Platform(Scw_Instance_T *p_scw_instance, const Scw_Public_Calibration_T *cal_src)
{
   boolean_T f_success = FBK_FALSE;

   if ((NULL != p_scw_instance) && (NULL != cal_src))
   {
      boolean_T f_update_core_success   = Scw_Update_Core_Cal_By_Public(&p_scw_instance->calibration, cal_src);
      boolean_T f_customer_core_success = Scw_Update_Customer_Cal_By_Public(&p_scw_instance->customer_calibration, cal_src);
      f_success                         = (boolean_T) (f_update_core_success && f_customer_core_success);
   }
   if (f_success)
   {
      /* Reset SCW */
      Scw_Reset(&p_scw_instance->core_input, &p_scw_instance->core_output, &p_scw_instance->persistent, &p_scw_instance->calibration);

      /* Initialize customer post run */
      Scw_Post_Run_Init(p_scw_instance);
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint8_t Scw_Get_Sw_Major_Version(void)
{
   /* Return Scw_Sw_Major_Version */
   return (Scw_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint8_t Scw_Get_Sw_Minor_Version(void)
{
   /* Return Scw_Sw_Minor_Version */
   return (Scw_Sw_Minor_Version);
}
