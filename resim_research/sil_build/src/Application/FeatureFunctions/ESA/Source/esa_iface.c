/**
 * @file esa_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the interface functions to initialize and run the ESA feature.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_iface.h"
#include "esa.h"
#include "esa_core_calibration.h"
#include "esa_customer_calibration.h"
#include "esa_debug_interface.h"
#include "esa_debug_writer.h"
#include "esa_input_t.h"
#include "esa_output_t.h"
#include "esa_post_run.h"
#include "esa_pre_run.h"
#include "esa_update_calibration.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/**
 * @brief Esa_Sw_Major_Version static memory
 * @SDD{CSCSA-66542}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Esa_Sw_Major_Version = 2u;

/**
 * @brief Esa_Sw_Minor_Version static memory
 * @SDD{CSCSA-66540}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Esa_Sw_Minor_Version = 0u;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Checks if any pointers are invalid.
 *
 * @return True if null pointer is detected
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-66535}
 * @verification{Create a test to check whether a null pointer is detected.}
 */
static boolean_T Esa_Null_Pointer_Detected(const Esa_Instance_T *p_esa_instance /**< ESA instance */,
                                           const Esa_Input_T *p_esa_input /**< ESA input */,
                                           const Fbk_Output_T *p_fbk_output /**< FBK output */,
                                           const Esa_Output_T *p_esa_output /**< ESA output */);

/*===========================================================================*\
 * Global function definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Esa_Init_Platform(Esa_Instance_T *p_esa_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_esa_instance);

   /* Initialize customer pre run */
   Esa_Pre_Run_Init(p_esa_instance);

   /* Initialize Esa pointers */
   Esa_Core_Cal_Update_Defaults(&(p_esa_instance->calibration));
   Esa_Customer_Cal_Update_Defaults(&(p_esa_instance->customer_calibration));

   /* Initialize the core output */
   Esa_Reset(&(p_esa_instance->core_input), &(p_esa_instance->core_output), &(p_esa_instance->persistent));

   /* Initialize customer post run */
   Esa_Post_Run_Init(p_esa_instance);

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
Sfl_Status_T Esa_Run_Platform(Esa_Instance_T *p_esa_instance,
                              const Esa_Input_T *p_esa_input,
                              const Fbk_Output_T *p_fbk_output,
                              Esa_Output_T *p_esa_output)
{
   /* Asserts */
   assert(NULL != p_esa_instance);

   if (Fbk_Is_False(Esa_Null_Pointer_Detected(p_esa_instance, p_esa_input, p_fbk_output, p_esa_output)))
   {
      Esa_Pre_Run(p_esa_instance, p_esa_input, p_fbk_output);
      Esa_Core_Run(&(p_esa_instance->core_output), &(p_esa_instance->core_input), &(p_esa_instance->persistent),
                   &(p_esa_instance->calibration));
      Esa_Post_Run(p_esa_instance, p_esa_input, p_esa_output);

      /* Pass software version to debug structure */
      Binary_Esa_Debug_Pass_Sw_Version(Esa_Sw_Major_Version, Esa_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Esa_Write_Bin_File();
   }

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Esa_Update_Calibration_Platform(Esa_Instance_T *p_esa_instance, const Esa_Public_Calibration_T *cal_src)
{
   boolean_T f_all_pointer_valid = (boolean_T) ((NULL != p_esa_instance) && (NULL != cal_src));
   boolean_T f_success           = FBK_FALSE;
   if (f_all_pointer_valid)
   {
      boolean_T f_update_core_success   = Esa_Update_Core_Cal_By_Public(&p_esa_instance->calibration, cal_src);
      boolean_T f_customer_core_success = Esa_Update_Customer_Cal_By_Public(&p_esa_instance->customer_calibration, cal_src);
      f_success                         = (boolean_T) (f_update_core_success && f_customer_core_success);
   }
   if (f_success)
   {
      /* reset ESA */
      Esa_Reset(&(p_esa_instance->core_input), &(p_esa_instance->core_output), &(p_esa_instance->persistent));

      /* initialize customer post run */
      Esa_Post_Run_Init(p_esa_instance);
   }

   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Esa_Get_Sw_Major_Version(void)
{
   return (Esa_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Esa_Get_Sw_Minor_Version(void)
{
   return (Esa_Sw_Minor_Version);
}

/*===========================================================================*\
* Local Functions
\*===========================================================================*/

static boolean_T Esa_Null_Pointer_Detected(const Esa_Instance_T *p_esa_instance,
                                           const Esa_Input_T *p_esa_input,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Esa_Output_T *p_esa_output)
{
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /* Check if NULL pointers are in use */
   if ((NULL == p_esa_instance) || (NULL == p_esa_input) || (NULL == p_fbk_output) || (NULL == p_esa_output))
   {
      f_null_pointer_detected = FBK_TRUE;
   }

   return f_null_pointer_detected;
}
