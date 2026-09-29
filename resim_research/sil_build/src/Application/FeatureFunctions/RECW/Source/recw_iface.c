/**
 * @file recw_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is RECW interface source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_iface.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "recw.h"
#include "recw_core_calibration.h"
#include "recw_customer_calibration.h"
#include "recw_debug_interface.h"
#include "recw_debug_writer.h"
#include "recw_post_run.h"
#include "recw_pre_run.h"
#include "recw_update_calibration.h"

#include <assert.h>

/*===========================================================================*\
* Macros
\*===========================================================================*/

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/**
 * @brief Recw_Sw_Major_Version static memory
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Recw_Sw_Major_Version = 8u;

/**
 * @brief Recw_Sw_Minor_Version static memory
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Recw_Sw_Minor_Version = 2u;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Check for NULL pointers
 *
 * @return True when NULL pointer has been detected
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7894}
 * @verification{}
 */
static boolean_T Recw_Null_Pointer_Detected(const Recw_Instance_T *p_recw_instance,
                                            const Recw_Input_T *p_recw_input,
                                            const Fbk_Output_T *p_fbk_output,
                                            const Recw_Output_T *recw_output);

/*===========================================================================* \
* Local Functions Definitions
\*===========================================================================*/

static boolean_T Recw_Null_Pointer_Detected(const Recw_Instance_T *p_recw_instance,
                                            const Recw_Input_T *p_recw_input,
                                            const Fbk_Output_T *p_fbk_output,
                                            const Recw_Output_T *recw_output)
{
   /* Return flag */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /* Check that no NULL pointers are in use */
   if ((NULL == p_recw_instance) || (NULL == p_recw_input) || (NULL == recw_output) || (NULL == p_fbk_output)
       || (NULL == p_fbk_output->p_pa_data))
   {
      f_null_pointer_detected = FBK_TRUE;
   }

   return f_null_pointer_detected;
}

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Recw_Init_Platform(Recw_Instance_T *p_recw_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_recw_instance);

   /* Get cal pointer */
   Recw_Core_Cal_Update_Defaults(&p_recw_instance->calibration);
   Recw_Customer_Cal_Update_Defaults(&p_recw_instance->customer_calibration);

   /* Initialize customer pre run */
   Recw_Pre_Run_Init(p_recw_instance);

   /* Reset Recw Core */
   Recw_Reset(&p_recw_instance->core_output, &p_recw_instance->persistent);

   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Recw_Update_Calibration_Platform(Recw_Instance_T *p_recw_instance, const Recw_Public_Calibration_T *cal_src)
{
   boolean_T f_success = FBK_FALSE;
   if ((p_recw_instance != NULL) && (cal_src != NULL))
   {
      boolean_T f_update_core_success   = Recw_Update_Core_Cal_By_Public(&p_recw_instance->calibration, cal_src);
      boolean_T f_customer_core_success = Recw_Update_Customer_Cal_By_Public(&p_recw_instance->customer_calibration, cal_src);
      f_success                         = (boolean_T) (f_update_core_success && f_customer_core_success);
   }
   if (f_success)
   {
      /* Reset RECW */
      Recw_Reset(&p_recw_instance->core_output, &p_recw_instance->persistent);
   }
   return f_success;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Recw_Run_Platform(Recw_Instance_T *p_recw_instance,
                               const Recw_Input_T *p_recw_input,
                               const Fbk_Output_T *p_fbk_output,
                               Recw_Output_T *p_recw_output)
{
   /* Run the Recw feature function algorithm including customer pre and post run */
   if (Fbk_Is_False(Recw_Null_Pointer_Detected(p_recw_instance, p_recw_input, p_fbk_output, p_recw_output)))
   {
      Recw_Pre_Run(p_recw_instance, p_recw_input, p_fbk_output);
      Recw_Core_Run(&p_recw_instance->core_output, &p_recw_instance->core_input, &p_recw_instance->calibration,
                    &p_recw_instance->persistent);
      Recw_Post_Run(p_recw_instance, p_recw_input, p_recw_output);

      /* Pass software version to debug structure */
      Binary_Recw_Debug_Pass_Sw_Version(Recw_Sw_Major_Version, Recw_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Recw_Write_Bin_File();
   }
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Recw_Get_Sw_Major_Version(void)
{
   /* Return Recw_Sw_Major_Version */
   return (Recw_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Recw_Get_Sw_Minor_Version(void)
{
   /* Return Recw_Sw_Minor_Version */
   return (Recw_Sw_Minor_Version);
}
