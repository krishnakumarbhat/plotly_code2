#ifndef LTB_IFACE_H
#define LTB_IFACE_H

/**
 * @file ltb_iface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for the LTB interface.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
  * Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_output_t.h"
#include "ltb_public_calibration_t.h"
#include "pa_reuse.h"
#include "sfl_status.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /**
    * @brief Change LTB calibration and reset LTB.
    *
    * @return true if success
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-122669}
    * @verification{Check whether the calibrations are updated correctly.}
    */
   boolean_T Ltb_Update_Calibration_Platform(Ltb_Instance_T *p_ltb_instance,
                                             const Ltb_Public_Calibration_T *cal_src /**< Ltb calibrations */);

   /**
    * @brief Initializes and resets the LTB feature.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-216614}
    * @verification{Create a test which checks whether LTB is initialized correctly with its default parameters.}
    */
   Sfl_Status_T Ltb_Init_Platform(Ltb_Instance_T *p_ltb_instance /**< LTB instance */);

   /**
    * @brief Runs the main LTB feature including pre- and post-run.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-216615}
    * @verification{Create a test to verify that Ltb is giving an alert for a critical object. Also verify that Ltb is not giving
    * an alert for non critical objects.}
    */
   Sfl_Status_T Ltb_Run_Platform(Ltb_Instance_T *p_ltb_instance /**< LTB instance */,
                                 const Ltb_Input_T *p_ltb_input /**< LTB input data */,
                                 const Fbk_Output_T *p_fbk_output /**< FBK output data */,
                                 Ltb_Output_T *p_ltb_output /**< LTB output data */);

   /**
    * @brief Returns Ltb_Sw_Major_Version.
    *
    * @return Ltb_Sw_Major_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-53922}
    * @verification{Check that the correct software major version is returned.}
    */
   uint16_t Ltb_Get_Sw_Major_Version(void);

   /**
    * @brief Returns Ltb_Sw_Minor_Version.
    *
    * @return Ltb_Sw_Minor_Version
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-53923}
    * @verification{Check that the correct software minor version is returned.}
    */
   uint16_t Ltb_Get_Sw_Minor_Version(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LTB_IFACE_H */
