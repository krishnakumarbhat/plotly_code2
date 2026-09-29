/**
 * @file ltb_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the interface functions to initialize and run the LTB feature.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_iface.h"
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "ltb.h"
#include "ltb_core_calibration.h"
#include "ltb_customer_calibration.h"
#include "ltb_debug_interface.h"
#include "ltb_debug_writer.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_output_t.h"
#include "ltb_post_run.h"
#include "ltb_pre_run.h"
#include "ltb_update_calibration.h"
#include "pa_reuse.h"
#include "sfl_status.h"
#include <assert.h>

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/


/**
 * @brief Ltb_Sw_Major_Version static memory
 * @SDD{CSCSA-53912}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ltb_Sw_Major_Version = 3u;

/**
 * @brief Ltb_Sw_Minor_Version static memory
 * @SDD{CSCSA-53914}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ltb_Sw_Minor_Version = 0u;

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
 * @SDD{CSCSA-53908}
 * @verification{Create a test to check whether a null pointer is detected.}
 */
static boolean_T Ltb_Null_Pointer_Detected(const Ltb_Instance_T *p_ltb_instance,
                                           const Ltb_Input_T *p_ltb_input,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Ltb_Output_T *p_ltb_output);

/*===========================================================================*\
 * Global function definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ltb_Update_Calibration_Platform(Ltb_Instance_T *p_ltb_instance, const Ltb_Public_Calibration_T *cal_src)
{
   boolean_T f_update_core_success   = Ltb_Update_Core_Cal_By_Public(&p_ltb_instance->calibration, cal_src);
   boolean_T f_customer_core_success = Ltb_Update_Customer_Cal_By_Public(&p_ltb_instance->customer_calibration, cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      /* Reset LTB */
      Ltb_Reset(&p_ltb_instance->core_output, &p_ltb_instance->calibration, &p_ltb_instance->persistent);
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Ltb_Init_Platform(Ltb_Instance_T *p_ltb_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_ltb_instance);

   /* Initialize Ltb pointers */
   Ltb_Core_Cal_Update_Defaults(&p_ltb_instance->calibration);
   Ltb_Customer_Cal_Update_Defaults(&p_ltb_instance->customer_calibration);

   /* Initialize the core output */
   Ltb_Reset(&p_ltb_instance->core_output, &p_ltb_instance->calibration, &p_ltb_instance->persistent);
   p_ltb_instance->ego_traj_predictor_instance.F_Host_Circle_Props_Initialized = FBK_FALSE;

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
Sfl_Status_T Ltb_Run_Platform(Ltb_Instance_T *p_ltb_instance,
                              const Ltb_Input_T *p_ltb_input,
                              const Fbk_Output_T *p_fbk_output,
                              Ltb_Output_T *p_ltb_output)
{
   if (Fbk_Is_False(Ltb_Null_Pointer_Detected(p_ltb_instance, p_ltb_input, p_fbk_output, p_ltb_output)))
   {
      Ltb_Pre_Run(p_ltb_instance, p_ltb_input, p_fbk_output);
      Ltb_Core_Run(&p_ltb_instance->core_output, &p_ltb_instance->core_input, &p_ltb_instance->calibration,
                   &p_ltb_instance->persistent, &p_ltb_instance->ego_traj_predictor_instance);
      Ltb_Post_Run(p_ltb_instance, p_ltb_output, p_ltb_input);

      /* Pass software version to debug structure */
      Binary_Ltb_Debug_Pass_Sw_Version(Ltb_Sw_Major_Version, Ltb_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Ltb_Write_Bin_File();
   }
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ltb_Get_Sw_Major_Version(void)
{
   return (Ltb_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ltb_Get_Sw_Minor_Version(void)
{
   return (Ltb_Sw_Minor_Version);
}

/*===========================================================================*\
* Local Functions
\*===========================================================================*/

static boolean_T Ltb_Null_Pointer_Detected(const Ltb_Instance_T *p_ltb_instance,
                                           const Ltb_Input_T *p_ltb_input,
                                           const Fbk_Output_T *p_fbk_output,
                                           const Ltb_Output_T *p_ltb_output)
{
   /* Return flag */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /* Check if Ltb_Input.p_context or P_Ltb_Cal NULL pointers are not null */
   if ((NULL == p_ltb_instance) || (NULL == p_ltb_input) || (NULL == p_fbk_output) || (NULL == p_ltb_output))
   {
      f_null_pointer_detected = FBK_TRUE;
   }

   return f_null_pointer_detected;
}
