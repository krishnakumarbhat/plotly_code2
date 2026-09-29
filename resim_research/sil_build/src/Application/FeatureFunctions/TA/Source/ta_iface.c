/**
 * @file ta_iface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief TA configuration file with the interface for the state machine.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_iface.h"
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "ta.h"
#include "ta_core_calibration.h"
#include "ta_core_calibration_check.h"
#include "ta_core_calibration_t.h"
#include "ta_customer_calibration.h"
#include "ta_debug_interface.h"
#include "ta_debug_writer.h"
#include "ta_input_boundary_check.h"
#include "ta_output_boundary_check.h"
#include "ta_post_run.h"
#include "ta_pre_run.h"
#include "ta_update_calibration.h"
#include <assert.h>

/*===========================================================================*\
* Macros
\*===========================================================================*/
/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

/**
 * @brief Ta_Sw_Major_Version static memory
 * @SDD{SF-8709}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ta_Sw_Major_Version = 12u;

/**
 * @brief Ta_Sw_Minor_Version static memory
 * @SDD{SF-8710}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const uint16_t Ta_Sw_Minor_Version = 0u;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Check for NULL pointers
 *
 * @return True for Null poiner detected otherwise false
 *
 * @SRS{SF-2352}
 * @SAE{}
 * @SDD{SF-8702}
 * @verification{Create tests where NULL pointers are detected. Shall only return false when no null pointers are detected.}
 */

/*===========================================================================*\
* Local Functions
\*===========================================================================*/

/*===========================================================================*\
* Backward compatibility
\*===========================================================================*/

/*============================================================================*\
 * Global Functions
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ta_Update_Calibration_Platform(Ta_Instance_T *p_ta_instance, const Ta_Public_Calibration_T *cal_src)
{
   boolean_T f_update_core_success   = Ta_Update_Core_Cal_By_Public(&p_ta_instance->calibration, cal_src);
   boolean_T f_customer_core_success = Ta_Update_Customer_Cal_By_Public(&p_ta_instance->customer_calibration, cal_src);
   boolean_T f_success               = (boolean_T) (f_update_core_success && f_customer_core_success);

   if (f_success)
   {
      /* Reset TA */
      Ta_Reset(&p_ta_instance->core_output, &p_ta_instance->calibration, &p_ta_instance->persistent);

      /* Initialize customer post run */
      /* coverity[misra_c_2012_rule_2_2_violation][	Function call "Ta_Post_Run_Init(void)" has no effect and can be removed.]*/
      Ta_Post_Run_Init();
   }
   return f_success;
}
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Ta_Init_Platform(Ta_Instance_T *p_ta_instance)
{
   /* Asserts */
   assert(Fbk_Is_Version_Compatible(8u, 0u));
   assert(NULL != p_ta_instance);

   /* Initialize cal values */
   Ta_Core_Cal_Update_Defaults(&p_ta_instance->calibration);
   Ta_Customer_Cal_Update_Defaults(&p_ta_instance->customer_calibration);

   /* Reset TA output structs */
   Ta_Reset(&p_ta_instance->core_output, &p_ta_instance->calibration, &p_ta_instance->persistent);
   p_ta_instance->ego_traj_predictor_instance.F_Host_Circle_Props_Initialized = FBK_FALSE;

   /* Initialize customer post run */
   /* coverity[misra_c_2012_rule_2_2_violation][	Function call "Ta_Post_Run_Init(void)" has no effect and can be removed.]*/
   Ta_Post_Run_Init();
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
Sfl_Status_T Ta_Run_Platform(Ta_Instance_T *p_ta_instance,
                             const Ta_Input_T *p_ta_input,
                             const Fbk_Output_T *p_fbk_output,
                             Ta_Output_T *p_ta_output)
{
   /* Run the TA feature function algorithm including customer pre and post run */
   if ((NULL != p_ta_instance) && (NULL != p_ta_input) && (NULL != p_fbk_output) && (NULL != p_ta_output))
   {
      const Ta_Core_Calibration_T *p_cal = &p_ta_instance->calibration;
      boolean_T f_ta_run_approved        = (boolean_T) (Ta_Core_Cal_In_Boundary(p_cal) && Ta_Are_Inputs_In_Boundary(p_ta_input));

      if (Fbk_Is_True(f_ta_run_approved))
      {
         Ta_Pre_Run(p_ta_instance, p_ta_input, p_fbk_output);
         Ta_Core_Run(&p_ta_instance->core_output, &p_ta_instance->core_input, &p_ta_instance->calibration,
                     &p_ta_instance->ego_traj_predictor_instance, &p_ta_instance->persistent);
         Ta_Post_Run(p_ta_instance, p_ta_input, p_ta_output);
      }

      if (Fbk_Is_False(f_ta_run_approved) || Fbk_Is_False(Ta_Are_Outputs_In_Boundary(p_ta_output)))
      {
         /* Reset Ta when the run wasn't approved due to calibrations or inputs not in bounds
            or processing has finished and invalid values are present in Ta_Output */
         Ta_Reset(&p_ta_instance->core_output, &p_ta_instance->calibration, &p_ta_instance->persistent);
         /* coverity[misra_c_2012_rule_2_2_violation][	Function call "Ta_Post_Run_Init(void)" has no effect and can be removed.]*/
         Ta_Post_Run_Init();
      }

      /* Pass software version to debug structure */
      Binary_Ta_Debug_Pass_Sw_Version(Ta_Sw_Major_Version, Ta_Sw_Minor_Version);

      /* Write debug output to bin file */
      Binary_Ta_Write_Bin_File();
   }
   return SFL_STATUS_OK;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ta_Get_Sw_Major_Version(void)
{
   /* Return Ta_Sw_Major_Version */
   return (Ta_Sw_Major_Version);
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
uint16_t Ta_Get_Sw_Minor_Version(void)
{
   /* Return Ta_Sw_Minor_Version */
   return (Ta_Sw_Minor_Version);
}
